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
                     TimeValue (Seconds (0.1)),
                     MakeTimeAccessor (&CcnxConsumer::m_offTime),
                     MakeTimeChecker ())
      .AddAttribute ("InterestName","CcnxName of the Interest (use CcnxNameComponents)",
                     CcnxNameComponentsValue (),
                     MakeCcnxNameComponentsAccessor (&CcnxConsumer::m_interestName),
                     MakeCcnxNameComponentsChecker ())
      .AddAttribute ("LifeTime", "LifeTime fo interest packet",
                     TimeValue (Seconds (0)),
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
    : m_seq (0)
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

    NS_ASSERT_MSG (m_face == 0, "Face should not exist");
    m_face = Create<CcnxLocalFace> ();

    // step 1. Set up forwarding from face to application
    m_face->SetNode (GetNode ());
    m_face->SetContentObjectHandler (MakeCallback (&CcnxConsumer::OnContentObject, this));

    // step 2. Set up forwarding to and from ccnx
    NS_ASSERT_MSG (GetNode ()->GetObject<Ccnx> () !=0,
                   "Ccnx stack should be installed on the node " << GetNode ());
    GetNode ()->GetObject<Ccnx> ()->AddFace (m_face);

    // step 3. Enable face
    m_face->SetUp ();

    // Send first packet immediately
    m_sendEvent = Simulator::Schedule (Seconds(0.0), &CcnxConsumer::SendPacket, this);
}
    
void 
CcnxConsumer::StopApplication () // Called at time specified by Stop
{
    NS_LOG_FUNCTION_NOARGS ();
        
    CancelEvents ();

    // step 1. Disable face
    m_face->SetDown ();

    // step 2. Remove face from ccnx stack
    GetNode ()->GetObject<Ccnx> ()->RemoveFace (m_face);

    // step 3. Disable callbacks
    m_face->SetContentObjectHandler (MakeNullCallback<void,
                                                      const Ptr<const CcnxContentObjectHeader> &,
                                                      const Ptr<const Packet> &> ());
}
    
void 
CcnxConsumer::CancelEvents ()
{
    NS_LOG_FUNCTION_NOARGS ();
        
    Simulator::Cancel (m_sendEvent);
}
    
void
CcnxConsumer::SendPacket ()
{
    NS_LOG_FUNCTION_NOARGS ();
    NS_LOG_INFO ("Sending Interest at " << Simulator::Now ());
    
    UniformVariable rand(1, std::numeric_limits<uint32_t>::max ());
    uint32_t randomNonce = rand.GetValue();
    
    CcnxInterestHeader interestHeader;
    interestHeader.SetNonce(randomNonce);
    
    Ptr<CcnxNameComponents> name = Create<CcnxNameComponents> (m_interestName);
    std::ostringstream os;
    os << m_seq++;
    (*name) (os.str ());
    
    interestHeader.SetName (name);
    interestHeader.SetInterestLifetime(m_interestLifeTime);
    interestHeader.SetChildSelector(m_childSelector);
    interestHeader.SetExclude(Create<CcnxNameComponents> (m_exclude));
    interestHeader.SetMaxSuffixComponents(m_maxSuffixComponents);
    interestHeader.SetMinSuffixComponents(m_minSuffixComponents);
        
    NS_LOG_INFO ("Interest: \n" << interestHeader);

    Ptr<Packet> packet = Create<Packet> ();
    packet->AddHeader (interestHeader);

    NS_LOG_INFO ("Packet: " << packet);
    m_face->ReceiveFromApplication (packet);
    
    m_interestsTrace (m_face,packet);
    
    NS_LOG_INFO("time = " << m_offTime);
    m_sendEvent = Simulator::Schedule (m_offTime, &CcnxConsumer::SendPacket, this);
}
    
// void
// CcnxConsumer::OnInterest (const Ptr<const CcnxInterestHeader> &interest)
// {
// }

void
CcnxConsumer::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                               const Ptr<const Packet> &payload)
{
  // do stuff
  NS_LOG_FUNCTION ("Received contentObject " << contentObject );
    NS_LOG_INFO ("Preved!");
  m_contentObjectsTrace (m_face,payload);
}


}
