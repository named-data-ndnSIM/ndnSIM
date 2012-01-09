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
#include "../model/ccnx-local-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxConsumerCbr");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxConsumerCbr);
    
TypeId
CcnxConsumerCbr::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxConsumerCbr")
    .SetParent<CcnxConsumer> ()
    .AddConstructor<CcnxConsumerCbr> ()

    .AddAttribute ("MeanRate", "Mean data packet rate (relies on the PayloadSize parameter)",
                   StringValue ("100Kbps"),
                   MakeDataRateAccessor (&CcnxConsumerCbr::GetDesiredRate, &CcnxConsumerCbr::SetDesiredRate),
                   MakeDataRateChecker ())
    ;

  return tid;
}
    
CcnxConsumerCbr::CcnxConsumerCbr ()
  : m_desiredRate ("100Kbps")
{
  NS_LOG_FUNCTION_NOARGS ();

  UpdateMean ();
}


void
CcnxConsumerCbr::UpdateMean ()
{
  double mean = 8.0 * m_payloadSize / m_desiredRate.GetBitRate ();
  m_randExp = ExponentialVariable (mean, 10000 * mean); // set upper limit to inter-arrival time
}

void
CcnxConsumerCbr::SetPayloadSize (uint32_t payload)
{
  CcnxConsumer::SetPayloadSize (payload);
  UpdateMean ();
}

void
CcnxConsumerCbr::SetDesiredRate (DataRate rate)
{
  m_desiredRate = rate;
  UpdateMean ();
}

DataRate
CcnxConsumerCbr::GetDesiredRate () const
{
  return m_desiredRate;
}


void
CcnxConsumerCbr::ScheduleNextPacket ()
{
  if (!m_sendEvent.IsRunning ())
    m_sendEvent = Simulator::Schedule (
                                       Seconds(m_randExp.GetValue ()),
                                       &CcnxConsumer::SendPacket, this);
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
