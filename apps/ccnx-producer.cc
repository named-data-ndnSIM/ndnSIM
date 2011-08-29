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
    .AddAttribute ("Face","Local face to be used",
                    PointerValue (CreateObject<CcnxLocalFace> ()),
                    MakePointerAccessor (&CcnxProducer::m_face),
                    MakePointerChecker<CcnxLocalFace> ())
    .AddAttribute ("Ccnx","Ccnx is needed to access ContentStore",
                   PointerValue (NULL),
                   MakePointerAccessor (&CcnxProducer::m_ccnx),
                   MakePointerChecker<Ccnx> ())

        /*.AddAttribute ("InterestName","CcnxName of the Interest (use CcnxNameComponents)",
                       PointerValue (CreateObject<CcnxNameComponents> ()),
                       MakePointerAccessor (&CcnxConsumer::m_interestName),
                       MakePointerChecker<CcnxNameComponents> ())
        .AddAttribute ("LifeTime", "LifeTime fo interest packet",
                       TimeValue (Seconds (4.0)),
                       MakeTimeAccessor (&CcnxConsumer::m_interestLifeTime),
                       MakeTimeChecker ())
        .AddAttribute ("MinSuffixComponents", "MinSuffixComponents",
                       IntegerValue(-1),
                       MakeIntegerAccessor(&CcnxConsumer::m_minSuffixComponents),
                       MakeIntegerChecker<int32_t>())
        .AddAttribute ("MaxSuffixComponents", "MaxSuffixComponents",
                       IntegerValue(-1),
                       MakeIntegerAccessor(&CcnxConsumer::m_maxSuffixComponents),
                       MakeIntegerChecker<int32_t>())
        .AddAttribute ("ChildSelector", "ChildSelector",
                       BooleanValue(false),
                       MakeBooleanAccessor(&CcnxConsumer::m_childSelector),
                       MakeBooleanChecker())
        .AddAttribute ("Exclude", "only simple name matching is supported (use CcnxNameComponents)",
                       PointerValue (CreateObject<CcnxNameComponents> ()),
                       MakePointerAccessor (&CcnxConsumer::m_exclude),
                       MakePointerChecker<CcnxNameComponents> ())*/
    .AddAttribute ("Capacity", "Capacity of the ContentStore",
                    UintegerValue(100),
                    MakeUintegerAccessor(&CcnxProducer::m_storeCapacity),
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
//    ScheduleNextTx();
}
    
void 
CcnxProducer::StopApplication () // Called at time specified by Stop
{
    NS_LOG_FUNCTION_NOARGS ();
        
    CancelEvents ();
}
    
void 
CcnxProducer::CancelEvents ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Simulator::Cancel (m_sendEvent);
}
    
/*void 
CcnxProducer::ScheduleNextTx ()
{
        NS_LOG_FUNCTION_NOARGS ();
        
        Time nextTime = Seconds(m_offTime);
        m_sendEvent = Simulator::Schedule (nextTime, &CcnxConsumer::SendPacket, this);
    }
    */
    
/*void
CcnxConsumer::SendPacket ()
    {
        NS_LOG_FUNCTION_NOARGS ();
        NS_LOG_INFO ("Sending Interest at " << Simulator::Now ());
        
        uint32_t randomNonce = UniformVariable().GetInteger(1, std::numeric_limits<uint32_t>::max ());
        CcnxInterestHeader interestHeader;
        interestHeader.SetNonce(randomNonce);
        interestHeader.SetName(m_interestName);
        interestHeader.SetInterestLifetime(m_interestLifeTime);
        interestHeader.SetChildSelector(m_childSelector);
        interestHeader.SetExclude(m_exclude);
        interestHeader.SetMaxSuffixComponents(m_maxSuffixComponents);
        interestHeader.SetMinSuffixComponents(m_minSuffixComponents);
        
        Ptr<Packet> packet = Create<Packet> ();
        packet->AddHeader (interestHeader);
        
        m_face->Receive(packet);
        
        ScheduleNextTx();
    }*/
    
uint32_t
CcnxProducer::GetStoreCapacity()
{
    return m_storeCapacity;
}
    
void
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
}
    
void 
CcnxProducer::AddContentStoreEntry (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
    m_ccnx->m_contentStore->Add (header, packet);
}
}
