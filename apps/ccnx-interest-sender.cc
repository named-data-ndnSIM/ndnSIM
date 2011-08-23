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

#include "ccnx-interest-sender.h"

NS_LOG_COMPONENT_DEFINE ("CcnxInterestSender");

namespace ns3
{    

NS_OBJECT_ENSURE_REGISTERED (CcnxInterestSender);

TypeId
CcnxInterestSender::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::CcnxInterestSender")
    .SetParent<Application> ()
    .AddConstructor<CcnxInterestSender> ()
    .AddAttribute ("OffTime", "Time interval between packets",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&CcnxInterestSender::m_offTime),
                   MakeTimeChecker ())
    /*.AddAttribute ("NameComponents","CcnxName of the Interest (use Name::Components)",
                   Name::ComponentsValue(Name::Components()),
                   Name::MakeComponentsAccessor(&CcnxInterestSender::m_interestName),
                   Name::MakeComponentsChecker())*/
    .AddAttribute ("NameComponents","CcnxName of the Interest (use Name::Components)",
                   PointerValue (CreateObject<Name::Components> ()),
                   MakePointerAccessor (&CcnxInterestSender::m_interestName),
                   MakePointerChecker<Name::Components> ())
    .AddAttribute ("LifeTime", "LifeTime fo interest packet",
                   TimeValue (Seconds (4.0)),
                   MakeTimeAccessor (&CcnxInterestSender::m_interestLifeTime),
                   MakeTimeChecker ())
    .AddAttribute ("MinSuffixComponents", "MinSuffixComponents",
                   IntegerValue(-1),
                   MakeIntegerAccessor(&CcnxInterestSender::m_minSuffixComponents),
                   MakeIntegerChecker<int32_t>())
    .AddAttribute ("MaxSuffixComponents", "MaxSuffixComponents",
                   IntegerValue(-1),
                   MakeIntegerAccessor(&CcnxInterestSender::m_maxSuffixComponents),
                   MakeIntegerChecker<int32_t>())
    .AddAttribute ("ChildSelector", "ChildSelector",
                   BooleanValue(false),
                   MakeBooleanAccessor(&CcnxInterestSender::m_childSelector),
                   MakeBooleanChecker())
    /*.AddAttribute ("Exclude","only simple name matching is supported (use Name::Components)",
                   Name::ComponentsValue(Name::Components()),
                   Name::MakeComponentsAccessor(&CcnxInterestSender::m_exclude),
                   Name::MakeComponentsChecker())*/
    .AddAttribute ("Exclude", "only simple name matching is supported (use Name::Components)",
                   PointerValue (CreateObject<Name::Components> ()),
                   MakePointerAccessor (&CcnxInterestSender::m_exclude),
                   MakePointerChecker<Name::Components> ())
    .AddAttribute ("Initial Nonce", "If 0 then nonce is not used",
                   UintegerValue(1),
                   MakeUintegerAccessor(&CcnxInterestSender::m_initialNonce),
                   MakeUintegerChecker<uint32_t>())
    ;
    /*
    .AddAttribute ("NoiseModel",
                   "A pointer to the model of the channel ambient noise.",
                   PointerValue (CreateObject<UanNoiseModelDefault> ()),
                   MakePointerAccessor (&UanChannel::m_noise),
                   MakePointerChecker<UanNoiseModel> ())*/
    return tid;
}
    
CcnxInterestSender::CcnxInterestSender ()
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
CcnxInterestSender::~CcnxInterestSender()
{
    NS_LOG_FUNCTION_NOARGS ();
}
    
void
CcnxInterestSender::DoDispose (void)
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Application::DoDispose ();
}
    
// Application Methods
void 
CcnxInterestSender::StartApplication () // Called at time specified by Start
{
    NS_LOG_FUNCTION_NOARGS ();
    ScheduleNextTx();
}
    
void 
CcnxInterestSender::StopApplication () // Called at time specified by Stop
{
    NS_LOG_FUNCTION_NOARGS ();
    
    CancelEvents ();
}
    
void 
CcnxInterestSender::CancelEvents ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Simulator::Cancel (m_sendEvent);
}

void 
CcnxInterestSender::ScheduleNextTx ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Time nextTime = Seconds(m_offTime); //send now
    m_sendEvent = Simulator::Schedule (nextTime, &CcnxInterestSender::SendPacket, this);
}
    
void
CcnxInterestSender::SendPacket ()
{
    NS_LOG_FUNCTION_NOARGS ();
    NS_LOG_INFO ("Sending Interest at " << Simulator::Now ());
    
    uint32_t randomNonce = UniformVariable().GetInteger(1, std::numeric_limits<uint32_t>::max ());
    CcnxInterestHeader interestHeader;
    interestHeader.SetNonce(randomNonce);
    //const Ptr<Name::Components> name = Create<Name::Components>(m_interestName);
    interestHeader.SetName(m_interestName);
    interestHeader.SetInterestLifetime(m_interestLifeTime);
    interestHeader.SetChildSelector(m_childSelector);
    //const Ptr<Name::Components> exclude = Create<Name::Components>(m_exclude);
    interestHeader.SetExclude(m_exclude);
    interestHeader.SetMaxSuffixComponents(m_maxSuffixComponents);
    interestHeader.SetMinSuffixComponents(m_minSuffixComponents);
    
    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (interestHeader);
    
    m_face->Send(packet);
    
    ScheduleNextTx();
}

    
}
