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
#include "ns3/ptr.h"
#include "ns3/ccnx-local-face.h"
#include "ns3/ccnx.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/ccnx-content-object-header.h"

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
                   StringValue ("100ms"),
                   MakeTimeAccessor (&CcnxConsumer::m_offTime),
                   MakeTimeChecker ())
    .AddAttribute ("InterestName","CcnxName of the Interest (use CcnxNameComponents)",
                   StringValue ("/"),
                   MakeCcnxNameComponentsAccessor (&CcnxConsumer::m_interestName),
                   MakeCcnxNameComponentsChecker ())
    .AddAttribute ("LifeTime", "LifeTime fo interest packet",
                   StringValue ("2s"),
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
                   CcnxNameComponentsValue (),
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
  : m_rand (0, std::numeric_limits<uint32_t>::max ())
  , m_seq (0)
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

  // do base stuff
  CcnxApp::StartApplication ();
  
  // schedule periodic packet generation
  m_sendEvent = Simulator::Schedule (Seconds(0.0), &CcnxConsumer::SendPacket, this);
}
    
void 
CcnxConsumer::StopApplication () // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS ();

  // cancel periodic packet generation
  Simulator::Cancel (m_sendEvent);

  // cleanup base stuff
  CcnxApp::StopApplication ();
}
    
void
CcnxConsumer::SendPacket ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_INFO ("Sending Interest at " << Simulator::Now ());
    
  //
  Ptr<CcnxNameComponents> nameWithSequence = Create<CcnxNameComponents> (m_interestName);
  (*name) (m_seq++);
  //

  CcnxInterestHeader interestHeader;
  interestHeader.SetNonce               (m_rand.GetValue ());
  interestHeader.SetName                (nameWithSequence);
  interestHeader.SetInterestLifetime    (m_interestLifeTime);
  interestHeader.SetChildSelector       (m_childSelector);
  interestHeader.SetExclude             (Create<CcnxNameComponents> (m_exclude));
  interestHeader.SetMaxSuffixComponents (m_maxSuffixComponents);
  interestHeader.SetMinSuffixComponents (m_minSuffixComponents);
        
  NS_LOG_INFO ("Interest: \n" << interestHeader);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (interestHeader);

  m_protocolHandler (packet);
    
  m_sendEvent = Simulator::Schedule (m_offTime, &CcnxConsumer::SendPacket, this);
}

void
CcnxConsumer::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                               const Ptr<const Packet> &payload)
{
  NS_LOG_FUNCTION (this << contentObject << payload);

  NS_LOG_INFO ("Received content object: " << cref(*contentObject));
}

} // namespace ns3
