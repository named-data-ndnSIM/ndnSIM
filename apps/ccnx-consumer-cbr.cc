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

#include "ccnx-consumer-cbr.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

#include "ns3/ccnx.h"
#include "ns3/ccnx-app-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxConsumerCbr");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxConsumerCbr);
    
TypeId
CcnxConsumerCbr::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxConsumerCbr")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxConsumer> ()
    .AddConstructor<CcnxConsumerCbr> ()

    .AddAttribute ("Frequency", "Frequency of interest packets",
                   StringValue ("1.0"),
                   MakeDoubleAccessor (&CcnxConsumerCbr::m_frequency),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Randomize", "Type of send time randomization: none (default), uniform, exponential",
                   StringValue ("none"),
                   MakeStringAccessor (&CcnxConsumerCbr::SetRandomize, &CcnxConsumerCbr::GetRandomize),
                   MakeStringChecker ())
    ;

  return tid;
}
    
CcnxConsumerCbr::CcnxConsumerCbr ()
  : m_frequency (1.0)
  , m_firstTime (true)
  , m_random (0)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_seqMax = std::numeric_limits<uint32_t>::max ();
}

CcnxConsumerCbr::~CcnxConsumerCbr ()
{
  if (m_random)
    delete m_random;
}

void
CcnxConsumerCbr::ScheduleNextPacket ()
{
  // double mean = 8.0 * m_payloadSize / m_desiredRate.GetBitRate ();
  // std::cout << "next: " << Simulator::Now().ToDouble(Time::S) + mean << "s\n";

  if (m_firstTime)
    {
      m_sendEvent = Simulator::Schedule (Seconds (0.0),
                                         &CcnxConsumer::SendPacket, this);
      m_firstTime = false;
    }
  else if (!m_sendEvent.IsRunning ())
    m_sendEvent = Simulator::Schedule (
                                       (m_random == 0) ?
                                         Seconds(1.0 / m_frequency)
                                       :
                                         Seconds(m_random->GetValue ()),
                                       &CcnxConsumer::SendPacket, this);
}

void
CcnxConsumerCbr::SetRandomize (const std::string &value)
{
  if (m_random)
    delete m_random;

  else if (value == "uniform")
    {
      m_random = new UniformVariable (0.0, 2 * 1.0 / m_frequency);
    }
  else if (value == "exponential")
    {
      m_random = new ExponentialVariable (1.0 / m_frequency, 50 * 1.0 / m_frequency);
    }
  else
    m_random = 0;

  m_randomType = value;  
}

std::string
CcnxConsumerCbr::GetRandomize () const
{
  return m_randomType;
}


///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

// void
// CcnxConsumer::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
//                                const Ptr<const Packet> &payload)
// {
//   CcnxConsumer::OnContentObject (contentObject, payload); // tracing inside
// }

// void
// CcnxConsumer::OnNack (const Ptr<const CcnxInterestHeader> &interest)
// {
//   CcnxConsumer::OnNack (interest); // tracing inside
// }

} // namespace ns3
