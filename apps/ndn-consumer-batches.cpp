/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-consumer-batches.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

#include "utils/batches.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.ConsumerBatches");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(ConsumerBatches);

TypeId
ConsumerBatches::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::ConsumerBatches")
      .SetGroupName("Ndn")
      .SetParent<Consumer>()
      .AddConstructor<ConsumerBatches>()

      .AddAttribute("Batches",
                    "Batches to schedule. Should be vector, containing pairs of time and amount",
                    // TypeId::ATTR_SET,
                    StringValue(""), MakeBatchesAccessor(&ConsumerBatches::m_batches),
                    MakeBatchesChecker());

  return tid;
}

ConsumerBatches::ConsumerBatches()
  : m_initial(true)
{
}

void
ConsumerBatches::StartApplication()
{
  Consumer::StartApplication();

  // std::cout << "Batches: " << batches << "\n";
  for (Batches::const_iterator i = m_batches.begin(); i != m_batches.end(); i++) {
    Simulator::ScheduleWithContext(GetNode()->GetId(), std::get<0>(*i), &ConsumerBatches::AddBatch,
                                   this, std::get<1>(*i));
  }
}

void
ConsumerBatches::AddBatch(uint32_t amount)
{
  // std::cout << Simulator::Now () << " adding batch of " << amount << "\n";
  m_seqMax += amount;
  m_rtt->ClearSent(); // this is important, otherwise RTT estimation for the new batch will be
                      // affected by previous batch history
  m_initial = true;
  ScheduleNextPacket();
}

void
ConsumerBatches::ScheduleNextPacket()
{
  if (!m_sendEvent.IsRunning()) {
    Time delay = Seconds(0);
    if (!m_initial)
      delay = m_rtt->RetransmitTimeout();

    m_initial = false;
    m_sendEvent = Simulator::Schedule(delay, &Consumer::SendPacket, this);
  }
}

} // namespace ndn
} // namespace ns3
