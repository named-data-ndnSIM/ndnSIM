/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ccnx-forwarding-strategy.h"

#include "ns3/ccnx-pit.h"
#include "ns3/ccnx-pit-entry.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx-pit.h"
#include "ns3/ccnx-fib.h"
#include "ns3/ccnx-content-store.h"

#include "ns3/assert.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/boolean.h"
#include "ns3/string.h"

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/tuple/tuple.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxForwardingStrategy");

namespace ns3 {

using namespace __ccnx_private;

NS_OBJECT_ENSURE_REGISTERED (CcnxForwardingStrategy);

TypeId CcnxForwardingStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxForwardingStrategy")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutInterests", "OutInterests",   MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_outInterests))
    .AddTraceSource ("InInterests",   "InInterests",   MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_inInterests))
    .AddTraceSource ("DropInterests", "DropInterests", MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_dropInterests))
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutNacks",  "OutNacks",  MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_outNacks))
    .AddTraceSource ("InNacks",   "InNacks",   MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_inNacks))
    .AddTraceSource ("DropNacks", "DropNacks", MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_dropNacks))
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutData",  "OutData",  MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_outData))
    .AddTraceSource ("InData",   "InData",   MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_inData))
    .AddTraceSource ("DropData", "DropData", MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_dropData))

    .AddAttribute ("EnableNACKs", "Enabling support of NACKs",
                   BooleanValue (false),
                   MakeBooleanAccessor (&CcnxForwardingStrategy::m_nacksEnabled),
                   MakeBooleanChecker ())
    .AddAttribute ("CacheUnsolicitedData", "Cache overheard data that have not been requested",
                   BooleanValue (false),
                   MakeBooleanAccessor (&CcnxForwardingStrategy::m_cacheUnsolicitedData),
                   MakeBooleanChecker ())

    ;
  return tid;
}

CcnxForwardingStrategy::CcnxForwardingStrategy ()
{
}

CcnxForwardingStrategy::~CcnxForwardingStrategy ()
{
}

void
CcnxForwardingStrategy::NotifyNewAggregate ()
{
  if (m_pit == 0)
    {
      m_pit = GetObject<CcnxPit> ();
    }
  if (m_fib == 0)
    {
      m_fib = GetObject<CcnxFib> ();
    }
  if (m_contentStore == 0)
    {
      m_contentStore = GetObject<CcnxContentStore> ();
    }

  Object::NotifyNewAggregate ();
}

void
CcnxForwardingStrategy::DoDispose ()
{
  m_pit = 0;
  m_contentStore = 0;
  m_fib = 0;

  Object::DoDispose ();
}

void
CcnxForwardingStrategy::OnInterest (const Ptr<CcnxFace> &incomingFace,
                                    Ptr<CcnxInterestHeader> &header,
                                    const Ptr<const Packet> &packet)
{
  m_inInterests (header, incomingFace);

  Ptr<CcnxPitEntry> pitEntry = m_pit->Lookup (*header);
  if (pitEntry == 0)
    {
      pitEntry = m_pit->Create (header);
    }

  if (pitEntry == 0)
    {
      // if it is still not created, then give up processing
      m_dropInterests (header, incomingFace);
      return;
    }
  
  bool isNew = pitEntry->GetIncoming ().size () == 0 && pitEntry->GetOutgoing ().size () == 0;
  bool isDuplicated = true;
  if (!pitEntry->IsNonceSeen (header->GetNonce ()))
    {
      pitEntry->AddSeenNonce (header->GetNonce ());
      isDuplicated = false;
    }

  NS_LOG_FUNCTION (header->GetName () << header->GetNonce () << boost::cref (*incomingFace) << isDuplicated);

  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                     //
  // !!!! IMPORTANT CHANGE !!!! Duplicate interests will create incoming face entry !!!! //
  //                                                                                     //
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////
  
  // Data is not in cache
  CcnxPitEntry::in_iterator inFace   = pitEntry->GetIncoming ().find (incomingFace);
  CcnxPitEntry::out_iterator outFace = pitEntry->GetOutgoing ().find (incomingFace);

  bool isRetransmitted = false;
  
  if (inFace != pitEntry->GetIncoming ().end ())
    {
      // CcnxPitEntryIncomingFace.m_arrivalTime keeps track arrival time of the first packet... why?

      isRetransmitted = true;
      // this is almost definitely a retransmission. But should we trust the user on that?
    }
  else
    {
      inFace = pitEntry->AddIncoming (incomingFace);
    }
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  
  if (isDuplicated) 
    {
      NS_LOG_DEBUG ("Received duplicatie interest on " << *incomingFace);
      m_dropInterests (header, incomingFace);

      /**
       * This condition will handle "routing" loops and also recently satisfied interests.
       * Every time interest is satisfied, PIT entry (with empty incoming and outgoing faces)
       * is kept for another small chunk of time.
       */

      if (m_nacksEnabled)
        {
          NS_LOG_DEBUG ("Sending NACK_LOOP");
          header->SetNack (CcnxInterestHeader::NACK_LOOP);
          Ptr<Packet> nack = Create<Packet> ();
          nack->AddHeader (*header);

          incomingFace->Send (nack);
          m_outNacks (header, incomingFace);
        }
      
      return;
    }

  Ptr<Packet> contentObject;
  Ptr<const CcnxContentObjectHeader> contentObjectHeader; // used for tracing
  Ptr<const Packet> payload; // used for tracing
  boost::tie (contentObject, contentObjectHeader, payload) = m_contentStore->Lookup (header);
  if (contentObject != 0)
    {
      NS_ASSERT (contentObjectHeader != 0);      
      NS_LOG_LOGIC("Found in cache");

      OnDataDelayed (contentObjectHeader, payload, contentObject);
      return;
    }

  // update PIT entry lifetime
  pitEntry->UpdateLifetime (header->GetInterestLifetime ());
  
  if (outFace != pitEntry->GetOutgoing ().end ())
    {
      NS_LOG_DEBUG ("Non duplicate interests from the face we have sent interest to. Don't suppress");
      // got a non-duplicate interest from the face we have sent interest to
      // Probably, there is no point in waiting data from that face... Not sure yet

      // If we're expecting data from the interface we got the interest from ("producer" asks us for "his own" data)
      // Mark interface YELLOW, but keep a small hope that data will come eventually.

      // ?? not sure if we need to do that ?? ...
      
      pitEntry->GetFibEntry ()->UpdateStatus (incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW);
      // StaticCast<CcnxFibImpl> (m_fib)->modify(pitEntry->GetFibEntry (),
      //                                          ll::bind (&CcnxFibEntry::UpdateStatus,
      //                                                    ll::_1, incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));
    }
  else
    if (!isNew && !isRetransmitted)
      {
        // Suppress this interest if we're still expecting data from some other face
        NS_LOG_DEBUG ("Suppress interests");
        m_dropInterests (header, incomingFace);
        return;
      }
  
  /////////////////////////////////////////////////////////////////////
  // Propagate
  /////////////////////////////////////////////////////////////////////
  
  bool propagated = PropagateInterest (pitEntry, incomingFace, header, packet);

  if (!propagated && isRetransmitted) //give another chance if retransmitted
    {
      // increase max number of allowed retransmissions
      pitEntry->IncreaseAllowedRetxCount ();

      // try again
      propagated = PropagateInterest (pitEntry, incomingFace, header, packet);
    }
  
  // ForwardingStrategy will try its best to forward packet to at least one interface.
  // If no interests was propagated, then there is not other option for forwarding or
  // ForwardingStrategy failed to find it. 
  if (!propagated)
    {
      NS_LOG_DEBUG ("Not propagated");
      m_dropInterests (header, incomingFace);
      GiveUpInterest (pitEntry, header);
    }
}

void
CcnxForwardingStrategy::OnData (const Ptr<CcnxFace> &incomingFace,
                                Ptr<CcnxContentObjectHeader> &header,
                                Ptr<Packet> &payload,
                                const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (incomingFace << header->GetName () << payload << packet);
  m_inData (header, payload, incomingFace);
  
  // 1. Lookup PIT entry
  Ptr<CcnxPitEntry> pitEntry = m_pit->Lookup (*header);
  if (pitEntry != 0)
    {
      // Note that with MultiIndex we need to modify entries indirectly

      CcnxPitEntry::out_iterator out = pitEntry->GetOutgoing ().find (incomingFace);
  
      // If we have sent interest for this data via this face, then update stats.
      if (out != pitEntry->GetOutgoing ().end ())
        {
          pitEntry->GetFibEntry ()->UpdateFaceRtt (incomingFace, Simulator::Now () - out->m_sendTime);
          // StaticCast<CcnxFibImpl> (m_fib)->modify (pitEntry->GetFibEntry (),
          //                                          ll::bind (&CcnxFibEntry::UpdateFaceRtt,
          //                                                    ll::_1,
          //                                                    incomingFace,
          //                                                    Simulator::Now () - out->m_sendTime));
        }
      else
        {
          // Unsolicited data, but we're interested in it... should we get it?
          // Potential hole for attacks

          if (m_cacheUnsolicitedData)
            {
              // Optimistically add or update entry in the content store
              m_contentStore->Add (header, payload);
            }
          else
            {
              NS_LOG_ERROR ("PIT entry for "<< header->GetName ()<<" is valid, "
                            "but outgoing entry for interface "<< boost::cref(*incomingFace) <<" doesn't exist\n");
            }
          // ignore unsolicited data
          return;
        }

      // Update metric status for the incoming interface in the corresponding FIB entry
      pitEntry->GetFibEntry ()->UpdateStatus (incomingFace, CcnxFibFaceMetric::NDN_FIB_GREEN);
      // StaticCast<CcnxFibImpl>(m_fib)->modify (pitEntry->GetFibEntry (),
      //                                          ll::bind (&CcnxFibEntry::UpdateStatus, ll::_1,
      //                                                    incomingFace, CcnxFibFaceMetric::NDN_FIB_GREEN));
  
      // Add or update entry in the content store
      m_contentStore->Add (header, payload);

      pitEntry->RemoveIncoming (incomingFace);

      if (pitEntry->GetIncoming ().size () == 0)
        {
          // Set pruning timout on PIT entry (instead of deleting the record)
          m_pit->MarkErased (pitEntry);
        }
      else
        {
          OnDataDelayed (header, payload, packet);
        }
    }
  else
    {
      NS_LOG_DEBUG ("Pit entry not found");
      if (m_cacheUnsolicitedData)
        {
          // Optimistically add or update entry in the content store
          m_contentStore->Add (header, payload);
        }
      else
        {
          // Drop data packet if PIT entry is not found
          // (unsolicited data packets should not "poison" content store)
      
          //drop dulicated or not requested data packet
          m_dropData (header, payload, incomingFace);
        }
      return; // do not process unsoliced data packets
    }
}


bool
CcnxForwardingStrategy::PropagateInterestViaGreen (Ptr<CcnxPitEntry> pitEntry, 
                                                   const Ptr<CcnxFace> &incomingFace,
                                                   Ptr<CcnxInterestHeader> &header,
                                                   const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (m_pit != 0, "PIT should be aggregated with forwarding strategy");

  int propagatedCount = 0;
  
  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<i_metric> ())
    {
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED ||
          metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_YELLOW)
        break; //propagate only to green faces

      if (pitEntry->GetIncoming ().find (metricFace.m_face) != pitEntry->GetIncoming ().end ()) 
        continue; // don't forward to face that we received interest from

      CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
        pitEntry->GetOutgoing ().find (metricFace.m_face);
      
      if (outgoing != pitEntry->GetOutgoing ().end () &&
          outgoing->m_retxCount >= pitEntry->GetMaxRetxCount ())
        {
          NS_LOG_DEBUG ("retxCount: " << outgoing->m_retxCount << ", maxRetxCount: " << pitEntry->GetMaxRetxCount ());
          continue;
        }
      
      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        {
          // let's try different green face
          continue;
        }

      pitEntry->AddOutgoing (metricFace.m_face);

      Ptr<Packet> packetToSend = packet->Copy ();

      //transmission
      metricFace.m_face->Send (packetToSend);
      m_outInterests (header, metricFace.m_face);
      
      propagatedCount++;
      break; // propagate only one interest
    }

  return propagatedCount > 0;
}

void
CcnxForwardingStrategy::OnDataDelayed (Ptr<const CcnxContentObjectHeader> header,
                                       Ptr<const Packet> payload,
                                       const Ptr<const Packet> &packet)
{
  // 1. Lookup PIT entry
  Ptr<CcnxPitEntry> pitEntry = m_pit->Lookup (*header);
  if (pitEntry != 0)
    {
      //satisfy all pending incoming Interests
      BOOST_FOREACH (const CcnxPitEntryIncomingFace &incoming, pitEntry->GetIncoming ())
        {
          incoming.m_face->Send (packet->Copy ());
          m_outData (header, payload, false, incoming.m_face);
          NS_LOG_DEBUG ("Satisfy " << *incoming.m_face);
          
          // successfull forwarded data trace
        }

      if (pitEntry->GetIncoming ().size () > 0)
        {
          // All incoming interests are satisfied. Remove them
          pitEntry->ClearIncoming ();

          // Remove all outgoing faces
          pitEntry->ClearOutgoing ();
          
          // Set pruning timout on PIT entry (instead of deleting the record)
          m_pit->MarkErased (pitEntry);
        }
    }
  else
    {
      NS_LOG_DEBUG ("Pit entry not found (was satisfied and removed before)");
      return; // do not process unsoliced data packets
    }
}

void
CcnxForwardingStrategy::GiveUpInterest (Ptr<CcnxPitEntry> pitEntry,
                                        Ptr<CcnxInterestHeader> header)
{
  NS_LOG_FUNCTION (this);

  if (m_nacksEnabled)
    {
      Ptr<Packet> packet = Create<Packet> ();
      header->SetNack (CcnxInterestHeader::NACK_GIVEUP_PIT);
      packet->AddHeader (*header);

      BOOST_FOREACH (const CcnxPitEntryIncomingFace &incoming, pitEntry->GetIncoming ())
        {
          NS_LOG_DEBUG ("Send NACK for " << boost::cref (header->GetName ()) << " to " << boost::cref (*incoming.m_face));
          incoming.m_face->Send (packet->Copy ());

          m_outNacks (header, incoming.m_face);
        }
  
      // All incoming interests cannot be satisfied. Remove them
      pitEntry->ClearIncoming ();

      // Remove also outgoing
      pitEntry->ClearOutgoing ();
  
      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->MarkErased (pitEntry);
    }
}


} //namespace ns3
