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
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"

#include "ns3/ccnx.h"
#include "../model/ccnx-local-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

namespace ll = boost::lambda;

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
    .AddAttribute ("StartSeq", "Initial sequence number",
                   IntegerValue(0),
                   MakeIntegerAccessor(&CcnxConsumer::m_seq),
                   MakeIntegerChecker<int32_t>())
    
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

    .AddAttribute ("RTO",
                   "Initial retransmission timeout",
                   StringValue ("1s"),
                   MakeTimeAccessor (&CcnxConsumer::m_rto),
                   MakeTimeChecker ())
    .AddAttribute ("RetxTimer",
                   "Timeout defining how frequent retransmission timeouts should be checked",
                   StringValue ("1s"),
                   MakeTimeAccessor (&CcnxConsumer::GetRetxTimer, &CcnxConsumer::SetRetxTimer),
                   MakeTimeChecker ())

    // .AddTraceSource ("InterestTrace", "Interests that were sent",
    //                  MakeTraceSourceAccessor (&CcnxConsumer::m_interestsTrace))
    // .AddTraceSource ("NackTrace", "NACKs received",
    //                  MakeTraceSourceAccessor (&CcnxConsumer::m_nackTrace))
    // .AddTraceSource ("ContentObjectTrace", "ContentObjects received",
    //                  MakeTraceSourceAccessor (&CcnxConsumer::m_contentObjectsTrace))
    ;

  return tid;
}
    
CcnxConsumer::CcnxConsumer ()
  : m_rand (0, std::numeric_limits<uint32_t>::max ())
  , m_seq (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CcnxConsumer::SetRetxTimer (Time retxTimer)
{
  m_retxTimer = retxTimer;
  if (m_retxEvent.IsRunning ())
    m_retxEvent.Cancel (); // cancel any scheduled cleanup events

  // schedule even with new timeout
  m_retxEvent = Simulator::Schedule (m_retxTimer,
                                     &CcnxConsumer::CheckRetxTimeout, this); 
}

Time
CcnxConsumer::GetRetxTimer () const
{
  return m_retxTimer;
}

void
CcnxConsumer::CheckRetxTimeout ()
{
  Time now = Simulator::Now ();

  while (!m_seqTimeouts.empty ())
    {
      SeqTimeoutsContainer::index<i_timestamp>::type::iterator entry =
        m_seqTimeouts.get<i_timestamp> ().begin ();
      if (entry->time + m_rto <= now) // timeout expired?
        {
          m_retxSeqs.insert (entry->seq);
          m_seqTimeouts.get<i_timestamp> ().modify (entry,
                                                    ll::bind(&SeqTimeout::time, ll::_1) = now);
        }
      else
        break; // nothing else to do. All later packets need not be retransmitted
    }
  
  m_retxEvent = Simulator::Schedule (m_retxTimer,
                                     &CcnxConsumer::CheckRetxTimeout, this); 
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

  uint32_t seq;
  
  if (m_retxSeqs.size () != 0)
    {
      seq = *m_retxSeqs.begin ();
      m_retxSeqs.erase (m_retxSeqs.begin ());
    }
  else
    seq = m_seq++;
  
  //
  Ptr<CcnxNameComponents> nameWithSequence = Create<CcnxNameComponents> (m_interestName);
  (*nameWithSequence) (seq);
  //

  CcnxInterestHeader interestHeader;
  interestHeader.SetNonce               (m_rand.GetValue ());
  interestHeader.SetName                (nameWithSequence);
  interestHeader.SetInterestLifetime    (m_interestLifeTime);
  interestHeader.SetChildSelector       (m_childSelector);
  if (m_exclude.size ()>0)
    {
      interestHeader.SetExclude             (Create<CcnxNameComponents> (m_exclude));
    }
  interestHeader.SetMaxSuffixComponents (m_maxSuffixComponents);
  interestHeader.SetMinSuffixComponents (m_minSuffixComponents);
        
  // NS_LOG_INFO ("Requesting Interest: \n" << interestHeader);
  NS_LOG_INFO ("> Interest for " << seq);

  Ptr<Packet> packet = Create<Packet> ();
  packet->AddHeader (interestHeader);

  m_protocolHandler (packet);

  std::pair<SeqTimeoutsContainer::iterator, bool>
    res = m_seqTimeouts.insert (SeqTimeout (seq, Simulator::Now ()));
  if (!res.second)
    m_seqTimeouts.modify (res.first,
                          ll::bind(&SeqTimeout::time, ll::_1) = Simulator::Now ());
  
  m_sendEvent = Simulator::Schedule (m_offTime, &CcnxConsumer::SendPacket, this);

  // \todo Trace
}

void
CcnxConsumer::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                               const Ptr<const Packet> &payload)
{
  NS_LOG_FUNCTION (this << contentObject << payload);

  // NS_LOG_INFO ("Received content object: " << boost::cref(*contentObject));
  
  uint32_t seq = boost::lexical_cast<uint32_t> (contentObject->GetName ().GetComponents ().back ());
  NS_LOG_INFO ("< DATA for " << seq);

  SeqTimeoutsContainer::iterator entry = m_seqTimeouts.find (seq);

  NS_ASSERT_MSG (entry != m_seqTimeouts.end (),
                 "Comment out this assert, if it causes problems");

  if (entry != m_seqTimeouts.end ())
    m_seqTimeouts.erase (entry);

  // \todo Trace
}

void
CcnxConsumer::OnNack (const Ptr<const CcnxInterestHeader> &interest)
{
  NS_LOG_FUNCTION (this << interest);

  // NS_LOG_INFO ("Received NACK: " << boost::cref(*interest));
  uint32_t seq = boost::lexical_cast<uint32_t> (interest->GetName ().GetComponents ().back ());
  NS_LOG_INFO ("< NACK for " << seq);

  // put in the queue of interests to be retransmitted
  m_retxSeqs.insert (seq);

  // \todo Trace?
}

} // namespace ns3
