/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ccnx-producer.h"


NS_LOG_COMPONENT_DEFINE ("CcnxProducer");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxProducer);
    
TypeId
CcnxProducer::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::CcnxProducer")
    .SetParent<Application> ()
    .AddConstructor<CcnxProducer> ()
    /*.AddAttribute ("Capacity", "Capacity of the ContentStore",
                    UintegerValue(100),
                    MakeUintegerAccessor(&CcnxProducer::m_storeCapacity),
                    MakeUintegerChecker<uint32_t>())*/
    .AddAttribute ("Prefix","Prefix, for which producer has the data",
                   CcnxNameComponentsValue (),
                   MakeCcnxNameComponentsAccessor (&CcnxProducer::m_prefix),
                   MakeCcnxNameComponentsChecker ())
    .AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
                   UintegerValue(100),
                   MakeUintegerAccessor(&CcnxProducer::m_virtualPayloadSize),
                   MakeUintegerChecker<uint32_t>())
    .AddTraceSource ("InterestTrace", "Interests that were received",
                    MakeTraceSourceAccessor (&CcnxProducer::m_interestsTrace))
    .AddTraceSource ("ContentObjectTrace", "ContentObjects that were sent",
                    MakeTraceSourceAccessor (&CcnxProducer::m_contentObjectsTrace))
    ;
        
    return tid;
}
    
CcnxProducer::CcnxProducer ()
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
CcnxProducer::~CcnxProducer()
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
void
CcnxProducer::DoDispose (void)
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Application::DoDispose ();
}
    
    // Application Methods
void 
CcnxProducer::StartApplication () // Called at time specified by Start
{
    NS_LOG_FUNCTION_NOARGS ();
    
    NS_ASSERT_MSG (m_face == 0, "Face should not exist");
    m_face = Create<CcnxLocalFace> ();
    
    // step 1. Set up forwarding from face to application
    m_face->SetNode (GetNode ());
    m_face->SetInterestHandler (MakeCallback (&CcnxProducer::OnInterest, this));
    
    // step 2. Set up forwarding to and from ccnx
    NS_ASSERT_MSG (GetNode ()->GetObject<Ccnx> () !=0,
                   "Ccnx stack should be installed on the node " << GetNode ());
    GetNode ()->GetObject<Ccnx> ()->AddFace (m_face);
    //Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric);
    GetNode ()->GetObject<Ccnx> ()->GetObject<CcnxFib> ()->Add(m_prefix, m_face, 0);
    // step 3. Enable face
    m_face->SetUp ();
}
    
void 
CcnxProducer::StopApplication () // Called at time specified by Stop
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
void
CcnxProducer::OnInterest(const Ptr<const CcnxInterestHeader> &interest)
{
    NS_LOG_FUNCTION (this);
    
    
    
    //Ptr<Packet> data = Lookup (interest);
    
    
    
    Ptr<Packet> incomingPacket = Create<Packet>(m_virtualPayloadSize);
    incomingPacket->AddHeader (*interest);
    m_interestsTrace(m_face,incomingPacket);
    
   

    static CcnxContentObjectTail tail; ///< \internal for optimization purposes
    Ptr<Packet> outgoingPacket = Create<Packet> (m_virtualPayloadSize);
    Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader>();
    header->SetName(Create<CcnxNameComponents>(interest->GetName()));
    outgoingPacket->AddHeader(*header);
    outgoingPacket->AddTrailer (tail);
    
    m_contentObjectsTrace(m_face,outgoingPacket);
        
    m_face->ReceiveFromApplication(outgoingPacket);
   
}
    
void 
CcnxProducer::CancelEvents ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    // Simulator::Cancel (m_sendEvent);
}
  
CcnxNameComponents
CcnxProducer::GetPrefix() const
{
  return m_prefix;
}
    
/*uint32_t
CcnxProducer::GetStoreCapacity()
{
  return m_storeCapacity;
}
    
void
CcnxProducer::SetStoreCapacity(uint32_t capacity)
{
  m_storeCapacity = capacity;
}
  */  
/*void
CcnxProducer::HandlePacket(const Ptr<CcnxFace> &face, const Ptr<const Packet> &packet)
{
    uint8_t type[2];
    uint32_t read = packet->CopyData (type,2);
    if (read!=2)
    {
        NS_LOG_INFO ("Unknown CcnxPacket");
        return;
    }
        
    if (type[0] == INTEREST_BYTE0 && type[1] == INTEREST_BYTE1)
    {
        m_interestsTrace(face,packet);
    }
    else if (type[0] == CONTENT_OBJECT_BYTE0 && type[1] == CONTENT_OBJECT_BYTE1)
    {
        m_contentObjectsTrace(face,packet);
    }
}*/
    
/*Ptr<Packet>
CcnxProducer::Lookup (Ptr<const CcnxInterestHeader> interest)
{
  NS_LOG_FUNCTION_NOARGS ();
  DataStoreContainer::type::iterator it = m_availableData.get<i_prefix> ().find (interest->GetName ());
        
  if (it != m_availableData.end ())
  {
    // return fully formed CCNx packet
    return it->GetFullyFormedCcnxPacket ();
  }
    
  return 0;
}   
    
void 
CcnxProducer::Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION_NOARGS ();
  DataStoreContainer::type::iterator it = m_availableData.get<i_prefix> ().find (header->GetName ());
        
  if (it == m_availableData.end ())
  { // add entry to the top
    m_availableData.get<i_mru> ().push_front (DataStoreEntry (header, packet));
            
    if (m_availableData.size () > m_storeCapacity)
      m_availableData.get<i_mru> ().pop_back ();
  }
  else
  {
    // promote entry to the top
    //m_contentStore.get<i_mru> ().relocate (m_contentStore.get<i_mru> ().begin (),
      //                                             m_contentStore.project<i_mru> (it));
  }
}*/
}
