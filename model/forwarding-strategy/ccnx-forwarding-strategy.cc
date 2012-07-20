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
#include "ns3/ccnx-face.h"

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

    .AddTraceSource ("OutData",  "OutData",  MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_outData))
    .AddTraceSource ("InData",   "InData",   MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_inData))
    .AddTraceSource ("DropData", "DropData", MakeTraceSourceAccessor (&CcnxForwardingStrategy::m_dropData))

    .AddAttribute ("CacheUnsolicitedData", "Cache overheard data that have not been requested",
                   BooleanValue (false),
                   MakeBooleanAccessor (&CcnxForwardingStrategy::m_cacheUnsolicitedData),
                   MakeBooleanChecker ())

    .AddAttribute ("DetectRetransmissions", "If non-duplicate interest is received on the same face more than once, "
                                            "it is considered a retransmission",
                   BooleanValue (true),
                   MakeBooleanAccessor (&CcnxForwardingStrategy::m_detectRetransmissions),
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
      if (pitEntry != 0)
        {
          DidCreatePitEntry (incomingFace, header, packet, pitEntry);
        }
      else
        {
          FailedToCreatePitEntry (incomingFace, header, packet);
          return;
        }
    }

  bool isDuplicated = true;
  if (!pitEntry->IsNonceSeen (header->GetNonce ()))
    {
      pitEntry->AddSeenNonce (header->GetNonce ());
      isDuplicated = false;
    }

  if (isDuplicated) 
    {
      DidReceiveDuplicateInterest (incomingFace, header, packet, pitEntry);
      return;
    }

  Ptr<Packet> contentObject;
  Ptr<const CcnxContentObjectHeader> contentObjectHeader; // used for tracing
  Ptr<const Packet> payload; // used for tracing
  boost::tie (contentObject, contentObjectHeader, payload) = m_contentStore->Lookup (header);
  if (contentObject != 0)
    {
      NS_ASSERT (contentObjectHeader != 0);      

      pitEntry->AddIncoming (incomingFace/*, Seconds (1.0)*/);

      // Do data plane performance measurements
      WillSatisfyPendingInterest (0, pitEntry);

      // Actually satisfy pending interest
      SatisfyPendingInterest (0, contentObjectHeader, payload, contentObject, pitEntry);
      return;
    }

  if (ShouldSuppressIncomingInterest (incomingFace, pitEntry))
    {
      pitEntry->AddIncoming (incomingFace/*, header->GetInterestLifetime ()*/);
      // update PIT entry lifetime
      pitEntry->UpdateLifetime (header->GetInterestLifetime ());

      // Suppress this interest if we're still expecting data from some other face
      NS_LOG_DEBUG ("Suppress interests");
      m_dropInterests (header, incomingFace);
      return;
    }

  PropagateInterest (incomingFace, header, packet, pitEntry);
}

void
CcnxForwardingStrategy::OnData (const Ptr<CcnxFace> &incomingFace,
                                Ptr<CcnxContentObjectHeader> &header,
                                Ptr<Packet> &payload,
                                const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (incomingFace << header->GetName () << payload << packet);
  m_inData (header, payload, incomingFace);
  
  // Lookup PIT entry
  Ptr<CcnxPitEntry> pitEntry = m_pit->Lookup (*header);
  if (pitEntry == 0)
    {
      DidReceiveUnsolicitedData (incomingFace, header, payload);
      return;
    }
  else
    {
      // Add or update entry in the content store
      m_contentStore->Add (header, payload);
    }

  while (pitEntry != 0)
    {
      // Do data plane performance measurements
      WillSatisfyPendingInterest (incomingFace, pitEntry);

      // Actually satisfy pending interest
      SatisfyPendingInterest (incomingFace, header, payload, packet, pitEntry);

      // Lookup another PIT entry
      pitEntry = m_pit->Lookup (*header);
    }
}


void
CcnxForwardingStrategy::DidReceiveDuplicateInterest (const Ptr<CcnxFace> &incomingFace,
                                                     Ptr<CcnxInterestHeader> &header,
                                                     const Ptr<const Packet> &packet,
                                                     Ptr<CcnxPitEntry> pitEntry)
{
  NS_LOG_FUNCTION (this << boost::cref (*incomingFace));
  /////////////////////////////////////////////////////////////////////////////////////////
  //                                                                                     //
  // !!!! IMPORTANT CHANGE !!!! Duplicate interests will create incoming face entry !!!! //
  //                                                                                     //
  /////////////////////////////////////////////////////////////////////////////////////////
  pitEntry->AddIncoming (incomingFace);
  m_dropInterests (header, incomingFace);
}

void
CcnxForwardingStrategy::DidExhaustForwardingOptions (const Ptr<CcnxFace> &incomingFace,
                                                     Ptr<CcnxInterestHeader> header,
                                                     const Ptr<const Packet> &packet,
                                                     Ptr<CcnxPitEntry> pitEntry)
{
  NS_LOG_FUNCTION (this << boost::cref (*incomingFace));
  m_dropInterests (header, incomingFace);
}

void
CcnxForwardingStrategy::FailedToCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                                                Ptr<CcnxInterestHeader> header,
                                                const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (this);
  m_dropInterests (header, incomingFace);
}
  
void
CcnxForwardingStrategy::DidCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                                           Ptr<CcnxInterestHeader> header,
                                           const Ptr<const Packet> &packet,
                                           Ptr<CcnxPitEntry> pitEntrypitEntry)
{
}

bool
CcnxForwardingStrategy::DetectRetransmittedInterest (const Ptr<CcnxFace> &incomingFace,
                                                     Ptr<CcnxPitEntry> pitEntry)
{
  CcnxPitEntry::in_iterator inFace = pitEntry->GetIncoming ().find (incomingFace);

  bool isRetransmitted = false;
  
  if (inFace != pitEntry->GetIncoming ().end ())
    {
      // this is almost definitely a retransmission. But should we trust the user on that?
      isRetransmitted = true;
    }

  return isRetransmitted;
}

void
CcnxForwardingStrategy::SatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                                                Ptr<const CcnxContentObjectHeader> header,
                                                Ptr<const Packet> payload,
                                                const Ptr<const Packet> &packet,
                                                Ptr<CcnxPitEntry> pitEntry)
{
  if (incomingFace != 0)
    pitEntry->RemoveIncoming (incomingFace);

  //satisfy all pending incoming Interests
  BOOST_FOREACH (const CcnxPitEntryIncomingFace &incoming, pitEntry->GetIncoming ())
    {
      incoming.m_face->Send (packet->Copy ());
      m_outData (header, payload, false, incoming.m_face);
      NS_LOG_DEBUG ("Satisfy " << *incoming.m_face);
          
      // successfull forwarded data trace
    }

  // All incoming interests are satisfied. Remove them
  pitEntry->ClearIncoming ();

  // Remove all outgoing faces
  pitEntry->ClearOutgoing ();
          
  // Set pruning timout on PIT entry (instead of deleting the record)
  m_pit->MarkErased (pitEntry);
}

void
CcnxForwardingStrategy::DidReceiveUnsolicitedData (const Ptr<CcnxFace> &incomingFace,
                                                   Ptr<const CcnxContentObjectHeader> header,
                                                   Ptr<const Packet> payload)
{
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
}

void
CcnxForwardingStrategy::WillSatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                                                    Ptr<CcnxPitEntry> pitEntry)
{
  CcnxPitEntry::out_iterator out = pitEntry->GetOutgoing ().find (incomingFace);
  
  // If we have sent interest for this data via this face, then update stats.
  if (out != pitEntry->GetOutgoing ().end ())
    {
      pitEntry->GetFibEntry ()->UpdateFaceRtt (incomingFace, Simulator::Now () - out->m_sendTime);
    } 
}

bool
CcnxForwardingStrategy::ShouldSuppressIncomingInterest (const Ptr<CcnxFace> &incomingFace,
                                                        Ptr<CcnxPitEntry> pitEntry)
{
  bool isNew = pitEntry->GetIncoming ().size () == 0 && pitEntry->GetOutgoing ().size () == 0;

  if (isNew) return false; // never suppress new interests
  
  bool isRetransmitted = m_detectRetransmissions && // a small guard
                         DetectRetransmittedInterest (incomingFace, pitEntry);  

  if (pitEntry->GetOutgoing ().find (incomingFace) != pitEntry->GetOutgoing ().end ())
    {
      NS_LOG_DEBUG ("Non duplicate interests from the face we have sent interest to. Don't suppress");
      // got a non-duplicate interest from the face we have sent interest to
      // Probably, there is no point in waiting data from that face... Not sure yet

      // If we're expecting data from the interface we got the interest from ("producer" asks us for "his own" data)
      // Mark interface YELLOW, but keep a small hope that data will come eventually.

      // ?? not sure if we need to do that ?? ...
      
      // pitEntry->GetFibEntry ()->UpdateStatus (incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW);
    }
  else
    if (!isNew && !isRetransmitted)
      {
        return true;
      }

  return false;
}

void
CcnxForwardingStrategy::PropagateInterest (const Ptr<CcnxFace> &incomingFace,
                                           Ptr<CcnxInterestHeader> header,
                                           const Ptr<const Packet> &packet,
                                           Ptr<CcnxPitEntry> pitEntry)
{
  bool isRetransmitted = m_detectRetransmissions && // a small guard
                         DetectRetransmittedInterest (incomingFace, pitEntry);  
  
  pitEntry->AddIncoming (incomingFace/*, header->GetInterestLifetime ()*/);
  /// @todo Make lifetime per incoming interface
  pitEntry->UpdateLifetime (header->GetInterestLifetime ());
  
  bool propagated = DoPropagateInterest (incomingFace, header, packet, pitEntry);

  if (!propagated && isRetransmitted) //give another chance if retransmitted
    {
      // increase max number of allowed retransmissions
      pitEntry->IncreaseAllowedRetxCount ();

      // try again
      propagated = DoPropagateInterest (incomingFace, header, packet, pitEntry);
    }

  // ForwardingStrategy will try its best to forward packet to at least one interface.
  // If no interests was propagated, then there is not other option for forwarding or
  // ForwardingStrategy failed to find it. 
  if (!propagated && pitEntry->GetOutgoing ().size () == 0)
    {
      DidExhaustForwardingOptions (incomingFace, header, packet, pitEntry);
    }
}

bool
CcnxForwardingStrategy::WillSendOutInterest (const Ptr<CcnxFace> &outgoingFace,
                                             Ptr<CcnxInterestHeader> header,
                                             Ptr<CcnxPitEntry> pitEntry)
{
  CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
    pitEntry->GetOutgoing ().find (outgoingFace);
      
  if (outgoing != pitEntry->GetOutgoing ().end () &&
      outgoing->m_retxCount >= pitEntry->GetMaxRetxCount ())
    {
      NS_LOG_ERROR (outgoing->m_retxCount << " >= " << pitEntry->GetMaxRetxCount ());
      return false; // already forwarded before during this retransmission cycle
    }

  
  bool ok = outgoingFace->IsBelowLimit ();
  if (!ok)
    return false;

  pitEntry->AddOutgoing (outgoingFace);
  return true;
}

void
CcnxForwardingStrategy::DidSendOutInterest (const Ptr<CcnxFace> &outgoingFace,
                                            Ptr<CcnxInterestHeader> header,
                                            Ptr<CcnxPitEntry> pitEntry)
{
  m_outInterests (header, outgoingFace);
}

void
CcnxForwardingStrategy::WillErasePendingInterest (Ptr<CcnxPitEntry> pitEntry)
{
  // do nothing for now. may be need to do some logging
}

} //namespace ns3
