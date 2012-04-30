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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ccnx-l3-protocol.h"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"

#include "ns3/ccnx-header-helper.h"

#include "ccnx-face.h"
#include "ccnx-forwarding-strategy.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

#include "ccnx-net-device-face.h"

#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::tuples;
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxL3Protocol");

namespace ns3 {

const uint16_t CcnxL3Protocol::ETHERNET_FRAME_TYPE = 0x7777;


NS_OBJECT_ENSURE_REGISTERED (CcnxL3Protocol);

TypeId 
CcnxL3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxL3Protocol")
    .SetParent<Ccnx> ()
    .SetGroupName ("Ccnx")
    .AddConstructor<CcnxL3Protocol> ()
    .AddAttribute ("FaceList", "List of faces associated with CCNx stack",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&CcnxL3Protocol::m_faces),
                   MakeObjectVectorChecker<CcnxFace> ())

    .AddAttribute ("ForwardingStrategy", "Forwarding strategy used by CCNx stack",
                   PointerValue (),
                   MakePointerAccessor (&CcnxL3Protocol::SetForwardingStrategy, &CcnxL3Protocol::GetForwardingStrategy),
                   MakePointerChecker<CcnxForwardingStrategy> ())
    
    .AddAttribute ("EnableNACKs", "Enabling support of NACKs",
                   BooleanValue (false),
                   MakeBooleanAccessor (&CcnxL3Protocol::m_nacksEnabled),
                   MakeBooleanChecker ())
    .AddAttribute ("CacheUnsolicitedData", "Cache overheard data that have not been requested",
                   BooleanValue (false),
                   MakeBooleanAccessor (&CcnxL3Protocol::m_cacheUnsolicitedData),
                   MakeBooleanChecker ())
    .AddAttribute ("RandomDataDelaying", "Delaying data processing",
                   BooleanValue (false),
                   MakeBooleanAccessor (&CcnxL3Protocol::m_delayingDataProcessing),
                   MakeBooleanChecker ())
  ;
  return tid;
}

CcnxL3Protocol::CcnxL3Protocol()
: m_faceCounter (0)
{
  NS_LOG_FUNCTION (this);
  
  m_pit = CreateObject<CcnxPit> ();
  m_contentStore = CreateObject<CcnxContentStore> ();
}

CcnxL3Protocol::~CcnxL3Protocol ()
{
  NS_LOG_FUNCTION (this);
}

void
CcnxL3Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
  m_fib = m_node->GetObject<CcnxFib> ();
  NS_ASSERT_MSG (m_fib != 0, "FIB should be created and aggregated to a node before calling Ccnx::SetNode");

  m_pit->SetFib (m_fib);
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ccnx stack
 */
void
CcnxL3Protocol::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node>();
      // verify that it's a valid node and that
      // the node has not been set before
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}

void 
CcnxL3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  for (CcnxFaceList::iterator i = m_faces.begin (); i != m_faces.end (); ++i)
    {
      *i = 0;
    }
  m_faces.clear ();
  m_node = 0;

  // Force delete on objects
  m_forwardingStrategy = 0; // there is a reference to PIT stored in here
  m_pit = 0;
  m_contentStore = 0;
  m_fib = 0;

  // m_forwardingStrategy = 0;
  Object::DoDispose ();
}

void
CcnxL3Protocol::SetForwardingStrategy (Ptr<CcnxForwardingStrategy> forwardingStrategy)
{
  NS_LOG_FUNCTION (this);
  m_forwardingStrategy = forwardingStrategy;
  m_forwardingStrategy->SetPit (m_pit);
}

Ptr<CcnxForwardingStrategy>
CcnxL3Protocol::GetForwardingStrategy (void) const
{
  return m_forwardingStrategy;
}

Ptr<CcnxPit>
CcnxL3Protocol::GetPit () const
{
  return m_pit;
}


uint32_t 
CcnxL3Protocol::AddFace (const Ptr<CcnxFace> &face)
{
  NS_LOG_FUNCTION (this << &face);

  face->SetId (m_faceCounter); // sets a unique ID of the face. This ID serves only informational purposes

  // ask face to register in lower-layer stack
  face->RegisterProtocolHandler (MakeCallback (&CcnxL3Protocol::Receive, this));

  m_faces.push_back (face);
  m_faceCounter++;
  return face->GetId ();
}

void
CcnxL3Protocol::RemoveFace (Ptr<CcnxFace> face)
{
  // ask face to register in lower-layer stack
  face->RegisterProtocolHandler (MakeNullCallback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>&> ());

  // just to be on a safe side. Do the process in two steps
  std::list<boost::reference_wrapper<const CcnxPitEntry> > entriesToRemoves;
  BOOST_FOREACH (const CcnxPitEntry &pitEntry, *m_pit)
    {
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind (&CcnxPitEntry::RemoveAllReferencesToFace, ll::_1, face));

      // If this face is the only for the associated FIB entry, then FIB entry will be removed soon.
      // Thus, we have to remove the whole PIT entry
      if (pitEntry.m_fibEntry.m_faces.size () == 1 &&
          pitEntry.m_fibEntry.m_faces.begin ()->m_face == face)
        {
          entriesToRemoves.push_back (boost::cref (pitEntry));
        }
    }
  BOOST_FOREACH (const CcnxPitEntry &removedEntry, entriesToRemoves)
    {
      m_pit->erase (m_pit->iterator_to (removedEntry));
    }

  CcnxFaceList::iterator face_it = find (m_faces.begin(), m_faces.end(), face);
  NS_ASSERT_MSG (face_it != m_faces.end (), "Attempt to remove face that doesn't exist");
  m_faces.erase (face_it);
}

Ptr<CcnxFace>
CcnxL3Protocol::GetFace (uint32_t index) const
{
  BOOST_FOREACH (const Ptr<CcnxFace> &face, m_faces) // this function is not supposed to be called often, so linear search is fine
    {
      if (face->GetId () == index)
        return face;
    }
  return 0;
}

Ptr<CcnxFace>
CcnxL3Protocol::GetFaceByNetDevice (Ptr<NetDevice> netDevice) const
{
  BOOST_FOREACH (const Ptr<CcnxFace> &face, m_faces) // this function is not supposed to be called often, so linear search is fine
    {
      Ptr<CcnxNetDeviceFace> netDeviceFace = DynamicCast<CcnxNetDeviceFace> (face);
      if (netDeviceFace == 0) continue;

      if (netDeviceFace->GetNetDevice () == netDevice)
        return face;
    }
  return 0;
}

uint32_t 
CcnxL3Protocol::GetNFaces (void) const
{
  return m_faces.size ();
}

// Callback from lower layer
void 
CcnxL3Protocol::Receive (const Ptr<CcnxFace> &face, const Ptr<const Packet> &p)
{
  if (!face->IsUp ())
    return;

  NS_LOG_DEBUG (*p);
  
  NS_LOG_LOGIC ("Packet from face " << *face << " received on node " <<  m_node->GetId ());

  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      CcnxHeaderHelper::Type type = CcnxHeaderHelper::GetCcnxHeaderType (p);
      switch (type)
        {
        case CcnxHeaderHelper::INTEREST:
          {
            Ptr<CcnxInterestHeader> header = Create<CcnxInterestHeader> ();

            // Deserialization. Exception may be thrown
            packet->RemoveHeader (*header);
            NS_ASSERT_MSG (packet->GetSize () == 0, "Payload of Interests should be zero");

            if (header->GetNack () > 0)
              OnNack (face, header, p/*original packet*/);
            else
              OnInterest (face, header, p/*original packet*/);  
            break;
          }
        case CcnxHeaderHelper::CONTENT_OBJECT:
          {
            Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
            
            static CcnxContentObjectTail contentObjectTrailer; //there is no data in this object

            // Deserialization. Exception may be thrown
            packet->RemoveHeader (*header);
            packet->RemoveTrailer (contentObjectTrailer);

            OnData (face, header, packet/*payload*/, p/*original packet*/);  
            break;
          }
        }
      
      // exception will be thrown if packet is not recognized
    }
  catch (CcnxUnknownHeaderException)
    {
      NS_ASSERT_MSG (false, "Unknown CCNx header. Should not happen");
      NS_LOG_ERROR ("Unknown CCNx header. Should not happen");
      return;
    }
}

void
CcnxL3Protocol::OnNack (const Ptr<CcnxFace> &incomingFace,
                        Ptr<CcnxInterestHeader> &header,
                        const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (incomingFace << header << packet);
  m_inNacks (header, incomingFace);

  tuple<const CcnxPitEntry&,bool,bool> ret = m_pit->Lookup (*header);
  CcnxPitEntry const& pitEntry = ret.get<0> ();
  bool isNew = ret.get<1> ();
  bool isDuplicated = ret.get<2> ();

  if (isNew || !isDuplicated) // potential flow
    {
      // somebody is doing something bad
      m_dropNacks (header, NON_DUPLICATED, incomingFace);
      return;
    }
  
  // CcnxPitEntryIncomingFaceContainer::type::iterator inFace = pitEntry.m_incoming.find (incomingFace);
  CcnxPitEntryOutgoingFaceContainer::type::iterator outFace = pitEntry.m_outgoing.find (incomingFace);

  if (outFace == pitEntry.m_outgoing.end ())
    {
//      NS_ASSERT_MSG (false,
//                     "Node " << GetObject<Node> ()->GetId () << ", outgoing entry should exist for face " << boost::cref(*incomingFace) << "\n" <<
//                     "size: " << pitEntry.m_outgoing.size ());
      
      // m_dropNacks (header, NON_DUPLICATE, incomingFace);
      return;
    }

  // This was done in error. Never, never do anything, except normal leakage. This way we ensure that we will not have losses,
  // at least when there is only one client
  //
  // incomingFace->LeakBucketByOnePacket ();

  NS_LOG_ERROR ("Nack on " << boost::cref(*incomingFace));

  m_pit->modify (m_pit->iterator_to (pitEntry),
                 ll::bind (&CcnxPitEntry::SetWaitingInVain, ll::_1, outFace));

  // If NACK is NACK_GIVEUP_PIT, then neighbor gave up trying to and removed it's PIT entry.
  // So, if we had an incoming entry to this neighbor, then we can remove it now

  if (header->GetNack () == CcnxInterestHeader::NACK_GIVEUP_PIT)
    {
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind (&CcnxPitEntry::RemoveIncoming, ll::_1, incomingFace));
    }

  m_fib->m_fib.modify (m_fib->m_fib.iterator_to (pitEntry.m_fibEntry),
                       ll::bind (&CcnxFibEntry::UpdateStatus,
                                 ll::_1, incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));

  if (pitEntry.m_incoming.size () == 0) // interest was actually satisfied
    {
      // no need to do anything
      m_dropNacks (header, AFTER_SATISFIED, incomingFace);
      return;
    }

  if (!pitEntry.AreAllOutgoingInVain ()) // not all ougtoing are in vain
    {
      NS_LOG_DEBUG ("Not all outgoing are in vain");
      // suppress
      // Don't do anything, we are still expecting data from some other face
      m_dropNacks (header, SUPPRESSED, incomingFace);
      return;
    }
  
  NS_ASSERT_MSG (m_forwardingStrategy != 0, "Need a forwarding protocol object to process packets");

  Ptr<Packet> nonNackInterest = Create<Packet> ();
  header->SetNack (CcnxInterestHeader::NORMAL_INTEREST);
  nonNackInterest->AddHeader (*header);
  
  bool propagated = m_forwardingStrategy->
    PropagateInterest (pitEntry, incomingFace, header, nonNackInterest);

  // // ForwardingStrategy will try its best to forward packet to at least one interface.
  // // If no interests was propagated, then there is not other option for forwarding or
  // // ForwardingStrategy failed to find it. 
  if (!propagated)
    {
      m_dropNacks (header, NO_FACES, incomingFace); // this headers doesn't have NACK flag set
      GiveUpInterest (pitEntry, header);
    }
}

// Processing Interests
//
// !!! Key point.
// !!! All interests should be answerred!!! Either later with data, immediately with data, or immediately with NACK
void CcnxL3Protocol::OnInterest (const Ptr<CcnxFace> &incomingFace,
                                 Ptr<CcnxInterestHeader> &header,
                                 const Ptr<const Packet> &packet)
{
  m_inInterests (header, incomingFace);
  // NS_LOG_DEBUG (*m_pit);

  // Lookup of Pit (and associated Fib) entry for this Interest 
  tuple<const CcnxPitEntry&,bool,bool> ret = m_pit->Lookup (*header);
  CcnxPitEntry const& pitEntry = ret.get<0> ();
  bool isNew = ret.get<1> ();
  bool isDuplicated = ret.get<2> ();

  // NS_LOG_DEBUG ("isNew: " << isNew << ", isDup: " << isDuplicated);
  
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
  CcnxPitEntryIncomingFaceContainer::type::iterator inFace = pitEntry.m_incoming.find (incomingFace);
  CcnxPitEntryOutgoingFaceContainer::type::iterator outFace = pitEntry.m_outgoing.find (incomingFace);

  bool isRetransmitted = false;
  
  if (inFace != pitEntry.m_incoming.end ())
    {
      // CcnxPitEntryIncomingFace.m_arrivalTime keeps track arrival time of the first packet... why?

      isRetransmitted = true;
      // this is almost definitely a retransmission. But should we trust the user on that?
    }
  else
    {
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::var(inFace) = ll::bind (&CcnxPitEntry::AddIncoming, ll::_1, incomingFace));
    }
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  
  if (isDuplicated) 
    {
      NS_LOG_DEBUG ("Received duplicatie interest on " << *incomingFace);
      m_dropInterests (header, DUPLICATED, incomingFace);

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
  tie (contentObject, contentObjectHeader, payload) = m_contentStore->Lookup (header);
  if (contentObject != 0)
    {
      NS_ASSERT (contentObjectHeader != 0);      
      NS_LOG_LOGIC("Found in cache");

      OnDataDelayed (contentObjectHeader, payload, contentObject);
      return;
    }

  // update PIT entry lifetime
  m_pit->modify (m_pit->iterator_to (pitEntry),
                 ll::bind (&CcnxPitEntry::UpdateLifetime, ll::_1,
                           header->GetInterestLifetime ()));
  
  if (outFace != pitEntry.m_outgoing.end ())
    {
      NS_LOG_DEBUG ("Non duplicate interests from the face we have sent interest to. Don't suppress");
      // got a non-duplicate interest from the face we have sent interest to
      // Probably, there is no point in waiting data from that face... Not sure yet

      // If we're expecting data from the interface we got the interest from ("producer" asks us for "his own" data)
      // Mark interface YELLOW, but keep a small hope that data will come eventually.

      // ?? not sure if we need to do that ?? ...
      
      m_fib->m_fib.modify(m_fib->m_fib.iterator_to (pitEntry.m_fibEntry),
                          ll::bind (&CcnxFibEntry::UpdateStatus,
                                    ll::_1, incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));
    }
  else
    if (!isNew && !isRetransmitted)
      {
        // Suppress this interest if we're still expecting data from some other face
        NS_LOG_DEBUG ("Suppress interests");
        m_dropInterests (header, SUPPRESSED, incomingFace);
        return;
      }
  
  /////////////////////////////////////////////////////////////////////
  // Propagate
  /////////////////////////////////////////////////////////////////////
  
  NS_ASSERT_MSG (m_forwardingStrategy != 0, "Need a forwarding protocol object to process packets");
  
  bool propagated = m_forwardingStrategy->
    PropagateInterest (pitEntry, incomingFace, header, packet);

  if (!propagated && isRetransmitted) //give another chance if retransmitted
    {
      // increase max number of allowed retransmissions
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind (&CcnxPitEntry::IncreaseAllowedRetxCount, ll::_1));

      // try again
      propagated = m_forwardingStrategy->
        PropagateInterest (pitEntry, incomingFace, header, packet);
    }
  
  // ForwardingStrategy will try its best to forward packet to at least one interface.
  // If no interests was propagated, then there is not other option for forwarding or
  // ForwardingStrategy failed to find it. 
  if (!propagated)
    {
      NS_LOG_DEBUG ("Not propagated");
      m_dropInterests (header, NO_FACES, incomingFace);
      GiveUpInterest (pitEntry, header);
    }
}

void
CcnxL3Protocol::OnDataDelayed (Ptr<const CcnxContentObjectHeader> header,
                               Ptr<const Packet> payload,
                               const Ptr<const Packet> &packet)
{
  if (m_delayingDataProcessing)
    {
      NS_LOG_DEBUG ("Delayed processing " << header->GetName ());
      // NS_LOG_DEBUG (*m_pit);
    }
  
  // 1. Lookup PIT entry
  try
    {
      CcnxPitEntryContainer::type::iterator pitEntry = m_pit->Lookup (*header);
      
      //satisfy all pending incoming Interests
      BOOST_FOREACH (const CcnxPitEntryIncomingFace &incoming, pitEntry->m_incoming)
        {
          incoming.m_face->Send (packet->Copy ());
          m_outData (header, payload, false, incoming.m_face);
          NS_LOG_DEBUG ("Satisfy " << *incoming.m_face);
          
          // successfull forwarded data trace
        }

      if (pitEntry->m_incoming.size () > 0)
        {
          // All incoming interests are satisfied. Remove them
          m_pit->modify (pitEntry,
                         ll::bind (&CcnxPitEntry::ClearIncoming, ll::_1));

          // Remove all outgoing faces
          m_pit->modify (pitEntry,
                         ll::bind (&CcnxPitEntry::ClearOutgoing, ll::_1));
          
          // Set pruning timout on PIT entry (instead of deleting the record)
          m_pit->modify (pitEntry,
                         ll::bind (&CcnxPitEntry::SetExpireTime, ll::_1,
                                   Simulator::Now () + m_pit->GetPitEntryPruningTimeout ()));
        }
    }
  catch (CcnxPitEntryNotFound)
    {
      NS_LOG_DEBUG ("Pit entry not found (was satisfied and removed before)");
      return; // do not process unsoliced data packets
    }
}

// Processing ContentObjects
void
CcnxL3Protocol::OnData (const Ptr<CcnxFace> &incomingFace,
                        Ptr<CcnxContentObjectHeader> &header,
                        Ptr<Packet> &payload,
                        const Ptr<const Packet> &packet)
{
    
  NS_LOG_FUNCTION (incomingFace << header->GetName () << payload << packet);
  m_inData (header, payload, incomingFace);
  // NS_LOG_DEBUG (*m_pit);
  
  // 1. Lookup PIT entry
  try
    {
      CcnxPitEntryContainer::type::iterator pitEntry = m_pit->Lookup (*header);

      // Note that with MultiIndex we need to modify entries indirectly

      CcnxPitEntryOutgoingFaceContainer::type::iterator out = pitEntry->m_outgoing.find (incomingFace);
  
      // If we have sent interest for this data via this face, then update stats.
      if (out != pitEntry->m_outgoing.end ())
        {
          m_fib->m_fib.modify (m_fib->m_fib.iterator_to (pitEntry->m_fibEntry),
                               ll::bind (&CcnxFibEntry::UpdateFaceRtt,
                                         ll::_1,
                                         incomingFace,
                                         Simulator::Now () - out->m_sendTime));
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
              NS_LOG_ERROR ("Node "<< m_node->GetId() <<
                            ". PIT entry for "<< header->GetName ()<<" is valid, "
                            "but outgoing entry for interface "<< boost::cref(*incomingFace) <<" doesn't exist\n");
            }
          // ignore unsolicited data
          return;
        }

      // Update metric status for the incoming interface in the corresponding FIB entry
      m_fib->m_fib.modify (m_fib->m_fib.iterator_to (pitEntry->m_fibEntry),
                           ll::bind (&CcnxFibEntry::UpdateStatus, ll::_1,
                                     incomingFace, CcnxFibFaceMetric::NDN_FIB_GREEN));
  
      // Add or update entry in the content store
      m_contentStore->Add (header, payload);

      m_pit->modify (pitEntry,
                     ll::bind (&CcnxPitEntry::RemoveIncoming, ll::_1, incomingFace));

      if (pitEntry->m_incoming.size () == 0)
        {
          // Set pruning timout on PIT entry (instead of deleting the record)
          m_pit->modify (pitEntry,
                         ll::bind (&CcnxPitEntry::SetExpireTime, ll::_1,
                                   Simulator::Now () + m_pit->GetPitEntryPruningTimeout ()));
        }
      else
        {
          if (!m_delayingDataProcessing)
            {
              OnDataDelayed (header, payload, packet);
            }
          else
            {
              NS_LOG_DEBUG ("Delaying Data forwarding " << header->GetName ());
              UniformVariable delay (0.0001, 0.002);
              Simulator::Schedule (Seconds (delay.GetValue ()),
                                   &CcnxL3Protocol::OnDataDelayed, this, header, payload, packet);
            }
        }
    }
  catch (CcnxPitEntryNotFound)
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
          m_dropData (header, payload, UNSOLICITED, incomingFace);
        }
      return; // do not process unsoliced data packets
    }
}

void
CcnxL3Protocol::GiveUpInterest (const CcnxPitEntry &pitEntry,
                                Ptr<CcnxInterestHeader> header)
{
  NS_LOG_FUNCTION (this << &pitEntry);
  // NS_LOG_DEBUG (*m_pit);
  if (m_nacksEnabled)
    {
      Ptr<Packet> packet = Create<Packet> ();
      header->SetNack (CcnxInterestHeader::NACK_GIVEUP_PIT);
      packet->AddHeader (*header);

      BOOST_FOREACH (const CcnxPitEntryIncomingFace &incoming, pitEntry.m_incoming)
        {
          NS_LOG_DEBUG ("Send NACK for " << boost::cref (header->GetName ()) << " to " << boost::cref (*incoming.m_face));
          incoming.m_face->Send (packet->Copy ());

          m_outNacks (header, incoming.m_face);
        }
  
      // All incoming interests cannot be satisfied. Remove them
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind (&CcnxPitEntry::ClearIncoming, ll::_1));

      // Remove also outgoing
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind (&CcnxPitEntry::ClearOutgoing, ll::_1));
  
      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind (&CcnxPitEntry::SetExpireTime, ll::_1,
                               Simulator::Now () + m_pit->GetPitEntryPruningTimeout ()));
    }
}

} //namespace ns3
