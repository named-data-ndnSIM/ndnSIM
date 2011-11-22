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
#include "ns3/boolean.h"

#include "ns3/ccnx-header-helper.h"

#include "ccnx-face.h"
#include "ccnx-route.h"
#include "ccnx-forwarding-strategy.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

#include "ccnx-net-device-face.h"

#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

using namespace boost::tuples;
using namespace boost::lambda;

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
    // .AddTraceSource ("Tx", "Send ccnx packet to outgoing interface.",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_txTrace))
    // .AddTraceSource ("Rx", "Receive ccnx packet from incoming interface.",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_rxTrace))
    // .AddTraceSource ("Drop", "Drop ccnx packet",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_dropTrace))
    // .AddAttribute ("InterfaceList", "The set of Ccnx interfaces associated to this Ccnx stack.",
    //                ObjectVectorValue (),
    //                MakeObjectVectorAccessor (&CcnxL3Protocol::m_faces),
    //                MakeObjectVectorChecker<CcnxFace> ())

    // .AddTraceSource ("SendOutgoing", "A newly-generated packet by this node is about to be queued for transmission",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_sendOutgoingTrace))

  ;
  return tid;
}

CcnxL3Protocol::CcnxL3Protocol()
: m_faceCounter (0)
{
  NS_LOG_FUNCTION (this);
  
  m_rit = CreateObject<CcnxRit> ();
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
  m_rit = 0;
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
  // m_forwardingStrategy->SetCcnx (this);
}

Ptr<CcnxForwardingStrategy>
CcnxL3Protocol::GetForwardingStrategy (void) const
{
  return m_forwardingStrategy;
}

uint32_t 
CcnxL3Protocol::AddFace (const Ptr<CcnxFace> &face)
{
  NS_LOG_FUNCTION (this << &face);

  face->SetNode (m_node);
  face->SetId (m_faceCounter); // sets a unique ID of the face. This ID serves only informational purposes

  // ask face to register in lower-layer stack
  face->RegisterProtocolHandler (MakeCallback (&CcnxL3Protocol::Receive, this));

  m_faces.push_back (face);
  m_faceCounter ++;
  return face->GetId ();
}

void
CcnxL3Protocol::RemoveFace (Ptr<CcnxFace> face)
{
  // ask face to register in lower-layer stack
  face->RegisterProtocolHandler (MakeNullCallback<void,const Ptr<CcnxFace>&,const Ptr<const Packet>&> ());
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

void
CcnxL3Protocol::TransmittedDataTrace (Ptr<Packet> packet,
                                      ContentObjectSource type,
                                      Ptr<Ccnx> ccnx, Ptr<const CcnxFace> face)
{
  // a "small" inefficiency for logging purposes
  Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
  static CcnxContentObjectTail tail;
  packet->RemoveHeader (*header);
  packet->RemoveTrailer (tail);
      
  m_transmittedDataTrace (header, packet/*payload*/, type, ccnx, face);
  
  packet->AddHeader (*header);
  packet->AddTrailer (tail);
}


// Callback from lower layer
void 
CcnxL3Protocol::Receive (const Ptr<CcnxFace> &face, const Ptr<const Packet> &p)
{
  if (!face->IsUp ())
    {
      NS_LOG_LOGIC ("Dropping received packet -- interface is down");
      // m_dropTrace (p, INTERFACE_DOWN, m_node->GetObject<Ccnx> ()/*this*/, face);
      return;
    }
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
    }
}

void
CcnxL3Protocol::OnNack (const Ptr<CcnxFace> &face,
                        Ptr<CcnxInterestHeader> &header,
                        const Ptr<const Packet> &p)
{
  NS_LOG_FUNCTION (face << header << p);

  // Huh... Ignore all this for now
  
  /*if( header->IsCongested () == false )
    m_pit->LeakBucket(incomingFace,1);
        
        
    m_droppedInterestsTrace (header, DROP_CONGESTION,
    m_node->GetObject<Ccnx> (), incomingFace);
        
    m_pit->modify(pitEntry, CcnxPitEntry::DeleteOutgoing(incomingFace));*/

  // No matter is it duplicate or not, if it is a NACK message, remove all possible incoming
  // entries for this interface (NACK means that neighbor gave up trying and there is no
  // point of sending data in this direction)
  
  // NS_LOG_INFO("Before (header->IsNack()) && (pitEntry != m_pit->end ())");
  // if ((header->IsNack()) && (pitEntry != m_pit->end ()))
  //   {
  //     //m_pit->erase (pitEntry);
  //     NS_LOG_INFO("TRUE");
  //     m_pit->modify(pitEntry, CcnxPitEntry::DeleteIncoming(incomingFace));
  //   }

  //   m_fib->modify (m_fib->iterator_to (pitEntry->m_fibEntry),
  // CcnxFibEntry::UpdateStatus(incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));

  // if (!pitEntry.AreThereMoreFacesToTry ())
  //   {
  //     BOOST_FOREACH (const CcnxPitEntryIncomingFace face, pitEntry.m_incoming)
  //       {
  //         // check all entries if the name of RIT entry matches the name of interest
  //         for (CcnxRitByNonce::type::iterator it = m_rit->begin(); it != m_rit->end(); it++)
  //           {
  //             if (it->m_prefix == iter->GetPrefix() )
  //               {
  //                 header->SetNonce(it->m_nonce);
  //                 header->SetNack(true);
  //                 SendInterest(face.m_face, header, packet->Copy());
  //               }
  //           }
  //       }

  //     m_pit->erase(pitEntry);

  //     return;
  //   }
}

// Processing Interests
//
// !!! Key point.
// !!! All interests should be answerred!!! Either later with data, immediately with data, or immediately with NACK
void CcnxL3Protocol::OnInterest (const Ptr<CcnxFace> &incomingFace,
                                 Ptr<CcnxInterestHeader> &header,
                                 const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (incomingFace << header << packet);
  // m_receivedInterestsTrace (header, m_node->GetObject<Ccnx> (), incomingFace);

  // Lookup of Pit (and associated Fib) entry for this Interest 
  const CcnxPitEntry &pitEntry;
  bool isNew;
  bool isDuplicated;
  tie (pitEntry, isNew, isDuplicated) = m_pit->Lookup (*header);

  if (isDuplicated) 
    {
      /**
       * This condition will handle "routing" loops and also recently satisfied interests.
       * Every time interest is satisfied, PIT entry (with empty incoming and outgoing faces)
       * is kept for another small chunk of time.
       */
      
      // //Trace duplicate interest  
      // m_droppedInterestsTrace (header, NDN_DUPLICATE_INTEREST, m_node->GetObject<Ccnx> (), incomingFace);

      header->SetNack (CcnxInterestHeader::NACK_LOOP);
      Ptr<Packet> packet = Create<Packet> ();
      packet->AddHeader (*header);

      SendInterest (m_incomingFace, header, packet);
      
      // //Trace duplicate interest  
      // m_droppedInterestsTrace (header, NDN_DUPLICATE_INTEREST, m_node->GetObject<Ccnx> (), incomingFace);
      return;
    }

  Ptr<Packet> contentObject;
  Ptr<CcnxContentObjectHeader> contentObjectHeader;
  tie (contentObject, contentObjectHeader) = m_contentStore->Lookup (header);
  if (contentObject != 0)
    {
      NS_ASSERT_MSG (pitEntry.m_incoming.size () == 0,
                     "Something strange. Data is cached, but size of incoming interests is not zero...");
      NS_ASSERT (contentObjectHeader != 0);
      
      NS_LOG_LOGIC("Found in cache");
        
      // TransmittedDataTrace (contentObject, CACHED,
      //                       m_node->GetObject<Ccnx> (), incomingFace);
      SendContentObject (incomingFace, contentObjectHeader, contentObject);

      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     boost::bind (&CcnxPitEntry::SetExpireTime, _1,
                                  Simulator::Now () + m_pit->GetPitEntryPruningTimeout ()));
      return;
    }

  // \todo Detect retransmissions. Not yet sure how...
  
  // Data is not in cache
  CcnxPitEntryIncomingFaceContainer::type::iterator inFace = pitEntry.m_incoming.find (incomingFace);
  CcnxPitEntryOutgoingFaceContainer::type::iterator outFace = pitEntry.m_outgoing.find (incomingFace);

  if (inFace != pitEntry.m_incoming.end ())
    {
      // CcnxPitEntryIncomingFace.m_arrivalTime keeps track arrival time of the first packet... why?
      
      // this is almost definitely a retransmission. But should we trust the user on that?
    }
  else
    {
      m_pit->modify (m_pit->iterator_to (m_pitpitEntry),
                    iface = _1->m_incoming.insert (CcnxPitEntryIncoming (incomingFace, Simulator::Now ())));
    }

  if (outFace != pitEntry.m_outgoing.end ())
    {
      // got a non-duplicate interest from the face we have sent interest to
      // Probably, there is no point in waiting data from that face... Not sure yet

      // If we're expecting data from the interface we got the interest from ("producer" asks us for "his own" data)
      // Mark interface YELLOW, but keep a small hope that data will come eventually.

      // ?? not sure if we need to do that ?? ...
      
      m_fib->modify(m_fib->iterator_to (pitEntry.m_fibEntry),
                    boost::bind (&CcnxFibEntry::UpdateStatus,
                                 _1, incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));

      // suppress?
    }
  else if (pitEntry->m_outgoing.size() > 0) // Suppress this interest if we're still expecting data from some other face

    {
      // We are already expecting data later in future. Suppress the interest
      // m_droppedInterestsTrace (header, NDN_SUPPRESSED_INTEREST, m_node->GetObject<Ccnx> (), incomingFace);
      return; 
    }

  /////////////////////////////////////////////////////////////////////
  // Propagate
  /////////////////////////////////////////////////////////////////////
  
  NS_ASSERT_MSG (m_forwardingStrategy != 0, "Need a forwarding protocol object to process packets");
  
  bool propagated = m_forwardingStrategy->
    PropagateInterest (pitEntry, fibEntry,incomingFace, header, packet,
                       MakeCallback (&CcnxL3Protocol::SendInterest, this)
                       );

  // ForwardingStrategy will try its best to forward packet to at least one interface.
  // If no interests was propagated, then there is not other option for forwarding or
  // ForwardingStrategy failed to find it. 
  if (!propagated)
    {
      Ptr<Packet> packet = Create<Packet> ();
      header->SetNack (CcnxInterestHeader::NACK_CONGESTION);
      packet.AddHeader (*header);

      while (pitEntry.m_incoming.size () > 0)
        {
          SendInterest (pitEntry.m_incoming.front ().m_face, header, packet->Copy ());

          pitEntry.m_incoming.pop_front ();

          // m_droppedInterestsTrace (header, DROP_CONGESTION,
          //                          m_node->GetObject<Ccnx> (), incomingFace);
        }

      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     boost::bind (&CcnxPitEntry::SetExpireTime, _1,
                                  Simulator::Now () + m_pit->GetPitEntryPruningTimeout ()));
    }
}

// Processing ContentObjects
void CcnxL3Protocol::OnData (const Ptr<CcnxFace> &incomingFace,
                             Ptr<CcnxContentObjectHeader> &header,
                             Ptr<Packet> &payload,
                             const Ptr<const Packet> &packet)
{
    
  NS_LOG_FUNCTION (incomingFace << header, payload, packet);
  // m_receivedDataTrace (header, payload, m_node->GetObject<Ccnx> ()/*this*/, incomingFace);

  // 1. Lookup PIT entry
  try
    {
      const CcnxPitEntry &pitEntry = m_pit->Lookup (*header);

      // Note that with MultiIndex we need to modify entries indirectly
  
      // Update metric status for the incoming interface in the corresponding FIB entry
      m_fib->modify (m_fib->iterator_to (pitEntry.m_fibEntry),
                     boost::bind (CcnxFibEntry::UpdateStatus, _1, incomingFace, CcnxFibFaceMetric::NDN_FIB_GREEN));
  
      // Add or update entry in the content store
      m_contentStore->Add (header, payload);

      CcnxPitEntryOutgoingFaceContainer::type::iterator out = pitEntry.m_outgoing.find (incomingFace);
  
      // If we have sent interest for this data via this face, then update stats.
      if (out != pitEntry.m_outgoing.end ())
        {
          m_fib->modify (m_fib->iterator_to (pitEntry.m_fibEntry),
                         boost::bind (&CcnxFibEntry::UpdateRtt,
                                      _1,
                                      Simulator::Now () - out->m_sendTime));
        }
      else
        {
          // Unsolicited data, but we're interested in it... should we get it?
          // Potential hole for attacks
          
          NS_LOG_ERROR ("Node "<< m_node->GetId() <<
                       ". PIT entry for "<< header->GetName ()<<" is valid, "
                       "but outgoing entry for interface "<< incomingFace <<" doesn't exist\n");

          NS_ASSERT (false); // temporary put false here
        }

      //satisfy all pending incoming Interests
      while (pitEntry.m_incoming.size () > 0)
        {
          if (pitEntry.m_incoming.front ().m_face != incomingFace)
            SendInterest (pitEntry.m_incoming.front ().m_face, header, packet->Copy ());

          pitEntry.m_incoming.pop_front ();

          // m_transmittedDataTrace (header, payload, FORWARDED, m_node->GetObject<Ccnx> (), interest.m_face);
        }

      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->modify (m_pit->iterator_to (pitEntry),
                     boost::bind (&CcnxPitEntry::SetExpireTime, _1,
                                  Simulator::Now () + m_pit->GetPitEntryPruningTimeout ()));
    }
  catch (CcnxPitEntryNotFound)
    {
      // 2. Drop data packet if PIT entry is not found
      //    (unsolicited data packets should not "poison" content store)
      
      //drop dulicated or not requested data packet
      // m_droppedDataTrace (header, payload, NDN_UNSOLICITED_DATA, m_node->GetObject<Ccnx> (), incomingFace);
      return; // do not process unsoliced data packets
    }
}

void
CcnxL3Protocol::SendInterest (const Ptr<CcnxFace> &face,
                              const Ptr<CcnxInterestHeader> &header,
                              const Ptr<Packet> &packet)
{
  NS_LOG_FUNCTION (this << "packet: " << &packet << ", face: "<< &face);
  NS_ASSERT_MSG (face != 0, "Face should never be NULL");

  if (face->IsUp ())
    {
      NS_LOG_LOGIC ("Sending via face " << &face); //
      // m_transmittedInterestsTrace (header, m_node->GetObject<Ccnx> (), face);
      face->Send (packet);
    }
  else
    {
      NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << &face);
      // m_droppedInterestsTrace (header, INTERFACE_DOWN, m_node->GetObject<Ccnx> (), face);
    }
}

void
CcnxL3Protocol::SendContentObject (const Ptr<CcnxFace> &face,
                                   const Ptr<CcnxContentObjectHeader> &header,
                                   const Ptr<Packet> &packet)
{
  NS_LOG_FUNCTION (this << "packet: " << &packet << ", face: "<< &face);
  NS_ASSERT_MSG (face != 0, "Face should never be NULL");

  if (face->IsUp ())
    {
      NS_LOG_LOGIC ("Sending via face " << &face); //
      // m_txTrace (packet, m_node->GetObject<Ccnx> (), face);
      face->Send (packet);
    }
  else
    {
      NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << &face);
      // m_dropTrace (packet, INTERFACE_DOWN, m_node->GetObject<Ccnx> (), face);
    }
}

Ptr<CcnxPit>
CcnxL3Protocol::GetPit()
{
    return m_pit;
}

// void
// CcnxL3Protocol::ScheduleLeakage()
// {
//     m_pit->LeakBuckets();
//     Time interval = MilliSeconds (NDN_INTEREST_RESET_PERIOD);
    
//     Simulator::Schedule (interval, &CcnxL3Protocol::ScheduleLeakage, this);
// }
} //namespace ns3
