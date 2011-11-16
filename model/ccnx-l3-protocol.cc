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

// Processing Interests
void CcnxL3Protocol::OnInterest (const Ptr<CcnxFace> &incomingFace,
                                 Ptr<CcnxInterestHeader> &header,
                                 const Ptr<const Packet> &packet)
{
  NS_LOG_LOGIC ("Receiving interest from " << &incomingFace);
  m_receivedInterestsTrace (header, m_node->GetObject<Ccnx> (), incomingFace);

    
    if( header->IsNack () )
    {
        NS_LOG_INFO("============");
        NS_LOG_INFO("NACK");
        NS_LOG_INFO("==========");
        /*if( header->IsCongested () == false )
            m_pit->LeakBucket(incomingFace,1);
        
        
        m_droppedInterestsTrace (header, DROP_CONGESTION,
                                 m_node->GetObject<Ccnx> (), incomingFace);
        
        m_pit->modify(pitEntry, CcnxPitEntry::DeleteOutgoing(incomingFace));*/
    }

    
    
    
  // Lookup of Pit and Fib entries for this Interest 
  CcnxFibEntryContainer::type::iterator fibEntry;
  CcnxPitEntryContainer::type::iterator pitEntry = m_pit->Lookup (*header, fibEntry);  
    
  // No matter is it duplicate or not, if it is a NACK message, remove all possible incoming
  // entries for this interface (NACK means that neighbor gave up trying and there is no
  // point of sending data in this direction)
    NS_LOG_INFO("Before (header->IsNack()) && (pitEntry != m_pit->end ())");
  if ((header->IsNack()) && (pitEntry != m_pit->end ()))
    {
      //m_pit->erase (pitEntry);
        NS_LOG_INFO("TRUE");
        m_pit->modify(pitEntry, CcnxPitEntry::DeleteIncoming(incomingFace));
    }
    
    NS_LOG_INFO("Before WasRecentlySatisfied");
    /*if (m_rit->WasRecentlySatisfied (*header))
    {
        return;
    }*/
  if (m_rit->WasRecentlySatisfied (*header))
    {
        NS_LOG_INFO("------------");
        NS_LOG_INFO("Entering WasRecentlySatisfied");
        NS_LOG_INFO("------------");
      // duplicate interests (same nonce) from applications are just ignored
      if (incomingFace->IsLocal() == true) 
          return;
        
      // Update metric status for the incoming interface in the corresponding FIB entry
      /*if (fibEntry != m_fib->end())
        m_fib->modify (m_fib->iterator_to (pitEntry->m_fibEntry),
                       CcnxFibEntry::UpdateStatus(incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));
      
      //Trace duplicate interest  
      m_droppedInterestsTrace (header, NDN_DUPLICATE_INTEREST,
                               m_node->GetObject<Ccnx> (), incomingFace);
        
      bool isMine = false;
      //TypeId tid = TypeId ("ns3::CcnxProducer");
      for(uint32_t i=0; i<m_node->GetNApplications();i++)
        {
            Ptr<Application> app = m_node->GetApplication(i);
            NS_LOG_INFO("ApplicationName = " << app->GetTypeId().GetName());
            if(app->GetTypeId().GetName() == "ns3::CcnxProducer")
              {
                if((DynamicCast<CcnxProducer>(app))->GetPrefix () == header->GetName ())
                {
                    isMine = true;
                    break;
                }
              }
        }
    
      Ptr<Packet> contentObject = m_contentStore->Lookup (header);
      if ((isMine == true) || (contentObject != NULL))
        {
            //never respond with NACK to NACK
            if(header->IsNack () )
                return;
            
            // always return a duplicate packet
            header->SetNack(true);
            //Trace duplicate interest  
            m_droppedInterestsTrace (header, NDN_DUPLICATE_INTEREST,
                                     m_node->GetObject<Ccnx> (), incomingFace);

            SendInterest(incomingFace, header, packet->Copy());
			
			return;
        }  
           
        
      // check PIT.  or there is no outgoing entry for this interface,
      // silently drop the duplicate packet
        
      // If no entry found, silently drop
      if( pitEntry == m_pit->end() ) 
        return;
        
      // If PIT entry timed out, silently drop
      if( pitEntry->m_timerExpired == true ) 
        return;
        
      // loop?
        
      // Check if there is no outgoing entry for the interface or different nonce
      // (i.e., got a duplicate packet, but we haven't sent interest to this
      // interface)
      //
      // This case means that there is a loop in the network.
      // So, prune this link, but do not remove PIT entry
        
      // Alex, check this condition!!
      if(pitEntry->m_outgoing.size () == 0)
        {
            //never respond with NACK to NACK
            if(header->IsNack () )
                return;
            
            // always return a duplicate packet
            header->SetNack(true);
            //Trace duplicate interest  
            m_droppedInterestsTrace (header, NDN_DUPLICATE_INTEREST,
                                     m_node->GetObject<Ccnx> (), incomingFace);

            SendInterest(incomingFace, header, packet->Copy());
            return;
        }
      
        
        // At this point:
		// - there is a non-expired PIT entry,
		// - there is an outgoing interest to the interface, and
		// - a nonce in outgoing entry is equal to a nonce in the received duplicate packet
        
		// Should perform:
		// Cleaning outgoing entry
		// If there are no outgoing interests and available interfaces left (pe->availableInterfaces),
		// prune all incoming interests, otherwise allow forwarding of the interest
		if( header->IsNack () )
		{
			if( header->IsCongested () == false )
                m_pit->LeakBucket(incomingFace,1);
            
            m_pit->modify(pitEntry, CcnxPitEntry::DeleteOutgoing(incomingFace));
        }
		else
		{
			//poit->waitingInVain = true;
		}

        
        // prune all incoming interests
        if((pitEntry->m_outgoing.size() ==0) && (pitEntry->m_fibEntry.m_faces.size() == 0))
        {
            BOOST_FOREACH (const CcnxPitEntryIncomingFace face, pitEntry->m_incoming)
            {
                if(face.m_face->IsLocal() == false)
                {
                  // check all entries if the name of RIT entry matches the name of interest
                  for (CcnxRitByNonce::type::iterator it = m_rit->begin(); it != m_rit->end(); it++)
                  {
                    if (it->m_prefix == pitEntry->GetPrefix() )
                      {
                        
                        header->SetNonce(it->m_nonce);
                        header->SetNack(true);
                        SendInterest(face.m_face, header, packet->Copy());
                        break;
                      }
                  }
                }
             }
            
            // Finally, remote the PIT entry
            m_pit->erase (pitEntry);
            
            return; // stop processing
        }
        
      if(pitEntry->m_fibEntry.m_faces.size() == 0)  
        return;*/
        return;
    }
    
    
    
  // Otherwise,
  // propagate the interest
  //
  // method `propagateInterest' can/should try different interface
  // from `availableInterfaces' list
    
  NS_LOG_INFO("Before SetRecentlySatisfied");
  m_rit->SetRecentlySatisfied (*header); 

  NS_LOG_INFO("Cache Lookup for " << header->GetName());
  Ptr<Packet> contentObject = m_contentStore->Lookup (header);
  if (contentObject != NULL)
    {
      NS_LOG_INFO("Found in cache");
        
      TransmittedDataTrace (contentObject, CACHED,
                            m_node->GetObject<Ccnx> (), incomingFace);
      incomingFace->Send (contentObject);
      return;
    }
  
  // Data is not in cache
  NS_LOG_INFO("Before inFace and OutFace");
  CcnxPitEntryIncomingFaceContainer::type::iterator inFace = pitEntry->m_incoming.find (incomingFace);
  CcnxPitEntryOutgoingFaceContainer::type::iterator outFace = pitEntry->m_outgoing.find (incomingFace);
    
     NS_LOG_INFO("Before (pitEntry != m_pit->end()) && (pitEntry->m_timerExpired == false)");
  if ((pitEntry != m_pit->end()) && (pitEntry->m_timerExpired == false))
    {
        NS_LOG_INFO("Entering (pitEntry != m_pit->end()) && (pitEntry->m_timerExpired == false)");
        
        if(inFace->m_face == NULL)
            NS_LOG_INFO("in face is null");
        if(outFace->m_face == NULL)
            NS_LOG_INFO("outface is null");
        if(outFace == pitEntry->m_outgoing.end())
            NS_LOG_INFO("OUTFACE = END");
        
        // If we're expecting data from the interface we got the interest from ("producer" asks us for "his own" data)
        // Give up this interface, but keep a small hope when the returned packet doesn't have PRUNE status
        if(outFace != pitEntry->m_outgoing.end()) // this is correct
        {
            NS_LOG_INFO("Entering outFace != pitEntry->m_outgoing.end()");
            if( header->IsCongested() == true )
            {
                NS_LOG_INFO("Entering header->IsCongested() == true");
                m_pit->LeakBucket(incomingFace, 1);
                m_pit->modify (pitEntry, CcnxPitEntry::DeleteOutgoing(outFace->m_face));
            }
            //else
            //    poit->waitingInVain = true;
            
            // Update metric status for the incoming interface in the corresponding FIB entry
            if(fibEntry != m_fib->end())
                m_fib->modify(m_fib->iterator_to (pitEntry->m_fibEntry),
                              CcnxFibEntry::UpdateStatus(incomingFace, CcnxFibFaceMetric::NDN_FIB_YELLOW));
        }
    }

  NS_LOG_INFO("Before (pitEntry->m_outgoing.size() == 0) && (pitEntry->m_fibEntry.m_faces.size() == 0)");
  if((pitEntry->m_outgoing.size() == 0) && (pitEntry->m_fibEntry.m_faces.size() == 0))
      // prune all incoming interests
    {
        
        for(CcnxPitEntryContainer::type::iterator iter = m_pit->begin();
            iter != m_pit->end(); 
            iter++)
        {
            /*for(CcnxPitEntryIncomingFaceContainer::type::iterator face = iter->m_incoming.begin();
                face != iter->m_incoming.end();
                face++)*/
            BOOST_FOREACH (const CcnxPitEntryIncomingFace face, iter->m_incoming)
            {
              if(face.m_face->IsLocal() == true)
                {
                    //returnInterestToApp( pkt, -piit->interfaceIndex );
                    //continue;
                }
                
                // check all entries if the name of RIT entry matches the name of interest
                for (CcnxRitByNonce::type::iterator it = m_rit->begin(); it != m_rit->end(); it++)
              {
                if (it->m_prefix == iter->GetPrefix() )
                  {
                    header->SetNonce(it->m_nonce);
                    header->SetNack(true);
                    SendInterest(face.m_face, header, packet->Copy());
                  }
              }
            }

        }
            
        m_pit->erase(pitEntry);
        
        return; // there is nothing else to do
    }

    // Suppress this interest only if we're still expecting data from some other interface
    if( pitEntry->m_outgoing.size() > 0 ) 
    {
        return; //ok. Now we can suppress this interest
    }
    
    
    // Prune and delete PIT entry if there are no available interfaces to propagate interest
    if( pitEntry->m_fibEntry.m_faces.size() == 0)
    {
        //if no match is found in the FIB, drop packet
        //printf( "Node %d: cannot process Interest packet %s (no interfaces left)\n", _node->nodeId, pkt->contentName );
		
        if(incomingFace->IsLocal() == false)
        {
            header->SetNack(true);
            m_droppedInterestsTrace (header, NDN_SUPPRESSED_INTEREST,
                                     m_node->GetObject<Ccnx> (), incomingFace);
            SendInterest(incomingFace, header, packet->Copy());
        }
        
        m_pit->erase(pitEntry);
        
    }

    
    
    // otherwise, try one of the available interfaces
    
  // suppress interest if 
  /*if (pitEntry->m_incoming.size () != 0 && // not a new PIT entry and
      inFace != pitEntry->m_incoming.end ()) // existing entry, but interest received via different face
    {
      m_droppedInterestsTrace (header, NDN_SUPPRESSED_INTEREST,
                                m_node->GetObject<Ccnx> (), incomingFace);
      return;
    }*/
    
    
    //just in case of bug
    header->SetNack(false);
    header->SetCongested(false);
  
    NS_ASSERT_MSG (m_forwardingStrategy != 0, "Need a forwarding protocol object to process packets");

    m_pit->modify (pitEntry, CcnxPitEntry::AddIncoming(incomingFace));
    
    bool propagated = m_forwardingStrategy->
                      PropagateInterest (pitEntry, fibEntry,incomingFace, header, packet,
                                         MakeCallback (&CcnxL3Protocol::SendInterest, this)
                                        );

    // If interest wasn't propagated further (probably, a limit is reached),
    // prune and delete PIT entry if there are no outstanding interests.
    // Stop processing otherwise.
    if( (!propagated) && (pitEntry->m_outgoing.size() == 0)) // this line works
      {
        BOOST_FOREACH (const CcnxPitEntryIncomingFace face, pitEntry->m_incoming)
          {
            header->SetNack(true);
            header->SetCongested(true);
            NS_LOG_INFO("Sending CONGESTION packet");
            SendInterest (face.m_face, header, packet->Copy());
                
            m_droppedInterestsTrace (header, DROP_CONGESTION,
                                         m_node->GetObject<Ccnx> (), incomingFace);
          }
      
          m_pit->erase (pitEntry);
      }
    /*}
  else
    {
      m_droppedInterestsTrace (header, NDN_PIT_TIMER_EXPIRED,
                                 m_node->GetObject<Ccnx> (), incomingFace);
      return;
    }*/
}

// Processing ContentObjects
void CcnxL3Protocol::OnData (const Ptr<CcnxFace> &incomingFace,
                             Ptr<CcnxContentObjectHeader> &header,
                             Ptr<Packet> &payload,
                             const Ptr<const Packet> &packet)
{
    
  NS_LOG_LOGIC ("Receiving contentObject from " << &incomingFace);
  m_receivedDataTrace (header, payload, m_node->GetObject<Ccnx> ()/*this*/, incomingFace);

  // 1. Lookup PIT entry
  try
    {
      const CcnxPitEntry &pitEntry = m_pit->Lookup (*header);

      // Note that with MultiIndex we need to modify entries indirectly
  
      // Update metric status for the incoming interface in the corresponding FIB entry
      m_fib->modify (m_fib->iterator_to (pitEntry.m_fibEntry),
                     CcnxFibEntry::UpdateStatus (incomingFace, CcnxFibFaceMetric::NDN_FIB_GREEN));
  
      // Add or update entry in the content store
      NS_LOG_INFO("Cached " << header->GetName());
      m_contentStore->Add (header, payload);

      CcnxPitEntryOutgoingFaceContainer::type::iterator
        out = pitEntry.m_outgoing.find (incomingFace);
  
      // If we have sent interest for this data via this face, then update stats.
      if (out != pitEntry.m_outgoing.end ())
        {
          m_pit->modify (m_pit->iterator_to (pitEntry),
                         CcnxPitEntry::EstimateRttAndRemoveFace(out, m_fib));
          // face will be removed in the above call
        }
      else
        {
          NS_LOG_WARN ("Node "<< m_node->GetId() <<
                          ". PIT entry for "<< header->GetName ()<<" is valid, "
                          "but outgoing entry for interface "<< incomingFace <<" doesn't exist\n");
        }

      //satisfy all pending incoming Interests
      BOOST_FOREACH (const CcnxPitEntryIncomingFace &interest, pitEntry.m_incoming)
        {
          if (interest.m_face == incomingFace) continue; 

          // may not work either because of 'const' thing
          interest.m_face->Send (packet->Copy ()); // unfortunately, we have to copy packet... 
          m_transmittedDataTrace (header, payload, FORWARDED, m_node->GetObject<Ccnx> (), interest.m_face);
        }

      m_pit->modify (m_pit->iterator_to (pitEntry), CcnxPitEntry::ClearIncoming()); // satisfy all incoming interests

      if( pitEntry.m_outgoing.size()==0 ) // remove PIT when all outgoing interests are "satisfied"
        {
          m_pit->erase (m_pit->iterator_to (pitEntry));
        }

    }
  catch (CcnxPitEntryNotFound)
    {
      // 2. Drop data packet if PIT entry is not found
      //    (unsolicited data packets should not "poison" content store)
      
      //drop dulicated or not requested data packet
      m_droppedDataTrace (header, payload, NDN_UNSOLICITED_DATA, m_node->GetObject<Ccnx> (), incomingFace);
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
      m_transmittedInterestsTrace (header, m_node->GetObject<Ccnx> (), face);
      face->Send (packet);
    }
  else
    {
      NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << &face);
      m_droppedInterestsTrace (header, INTERFACE_DOWN, m_node->GetObject<Ccnx> (), face);
    }
}

void
CcnxL3Protocol::SendContentObject (const Ptr<CcnxFace> &face,
                                   const Ptr<CcnxContentObjectHeader> &header,
                                   const Ptr<Packet> &packet)
{
  NS_LOG_FUNCTION (this << "packet: " << &packet << ", face: "<< &face);
  NS_ASSERT_MSG (face != 0, "Face should never be NULL");

  NS_ASSERT_MSG (false, "Should not be called for now");
  
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

void
CcnxL3Protocol::ScheduleLeakage()
{
    m_pit->LeakBuckets();
    Time interval = MilliSeconds (NDN_INTEREST_RESET_PERIOD);
    Simulator::Schedule (interval, &CcnxL3Protocol::ScheduleLeakage, this);
}
} //namespace ns3
