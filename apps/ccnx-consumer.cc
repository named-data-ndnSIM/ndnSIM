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

#include "ccnx-consumer.h"

NS_LOG_COMPONENT_DEFINE ("CcnxConsumer");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxConsumer);
    
TypeId
CcnxConsumer::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::CcnxConsumer")
    .SetParent<Application> ()
    .AddConstructor<CcnxConsumer> ()
    .AddAttribute ("OffTime", "Time interval between packets",
                    TimeValue (Seconds (0.1)),
                    MakeTimeAccessor (&CcnxConsumer::m_offTime),
                    MakeTimeChecker ())
    .AddAttribute ("Face","Local face to be used",
                    PointerValue (CreateObject<CcnxLocalFace> ()),
                    MakePointerAccessor (&CcnxConsumer::m_face),
                    MakePointerChecker<CcnxLocalFace> ())
    .AddAttribute ("InterestName","CcnxName of the Interest (use CcnxNameComponents)",
                    CcnxNameComponentsValue (CcnxNameComponents ()),
                    MakeCcnxNameComponentsAccessor (&CcnxConsumer::m_interestName),
                    MakeCcnxNameComponentsChecker ())
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
                   CcnxNameComponentsValue (CcnxNameComponents ()),
                   MakeCcnxNameComponentsAccessor (&CcnxConsumer::m_exclude),
                   MakeCcnxNameComponentsChecker ())
    .AddAttribute ("Initial Nonce", "If 0 then nonce is not used",
                    UintegerValue(1),
                    MakeUintegerAccessor(&CcnxConsumer::m_initialNonce),
                    MakeUintegerChecker<uint32_t>())
    .AddTraceSource ("InterestTrace", "Interests that were sent",
                     MakeTraceSourceAccessor (&CcnxConsumer::m_interestsTrace))
    .AddTraceSource ("ContentObjectTrace", "ContentObjects that were received",
                     MakeTraceSourceAccessor (&CcnxConsumer::m_contentObjectsTrace))
    ;

    return tid;
}
    
CcnxConsumer::CcnxConsumer ()
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
CcnxConsumer::~CcnxConsumer()
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
void
CcnxConsumer::DoDispose (void)
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Application::DoDispose ();
}
    
// Application Methods
void 
CcnxConsumer::StartApplication () // Called at time specified by Start
{
    NS_LOG_FUNCTION_NOARGS ();
    ScheduleNextTx();
}
    
void 
CcnxConsumer::StopApplication () // Called at time specified by Stop
{
    NS_LOG_FUNCTION_NOARGS ();
        
    CancelEvents ();
}
    
void 
CcnxConsumer::CancelEvents ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Simulator::Cancel (m_sendEvent);
}
    
void 
CcnxConsumer::ScheduleNextTx ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Time nextTime = Seconds(m_offTime);
    m_sendEvent = Simulator::Schedule (nextTime, &CcnxConsumer::SendPacket, this);
}
    
void
CcnxConsumer::SendPacket ()
{
    NS_LOG_FUNCTION_NOARGS ();
    NS_LOG_INFO ("Sending Interest at " << Simulator::Now ());
        
    uint32_t randomNonce = UniformVariable().GetInteger(1, std::numeric_limits<uint32_t>::max ());
    CcnxInterestHeader interestHeader;
    interestHeader.SetNonce(randomNonce);
    interestHeader.SetName(Create<CcnxNameComponents> (m_interestName));
    interestHeader.SetInterestLifetime(m_interestLifeTime);
    interestHeader.SetChildSelector(m_childSelector);
    interestHeader.SetExclude(Create<CcnxNameComponents> (m_exclude));
    interestHeader.SetMaxSuffixComponents(m_maxSuffixComponents);
    interestHeader.SetMinSuffixComponents(m_minSuffixComponents);
        
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (interestHeader);
        
    m_face->Receive(packet);
        
    ScheduleNextTx();
}
    
void
CcnxConsumer::HandlePacket(const Ptr<CcnxFace> &face, const Ptr<const Packet> &packet)
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
}
