/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2018  Regents of the University of California.
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

#include "ndn-consumer-pcon.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.ConsumerPcon");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(ConsumerPcon);

TypeId
ConsumerPcon::GetTypeId()
{
  static TypeId tid =
    TypeId("ns3::ndn::ConsumerPcon")
      .SetGroupName("Ndn")
      .SetParent<ConsumerWindow>()
      .AddConstructor<ConsumerPcon>()

      .AddAttribute("Beta", "TCP Multiplicative Decrease factor",
                    DoubleValue(0.5),
                    MakeDoubleAccessor(&ConsumerPcon::m_beta),
                    MakeDoubleChecker<double>())

      .AddAttribute("AddRttSupress", "Minimum number of RTTs (1 + this factor) between window decreases",
                    DoubleValue(0.5), // This default value was chosen after manual testing
                    MakeDoubleAccessor(&ConsumerPcon::m_addRttSupress),
                    MakeDoubleChecker<double>())

      .AddAttribute("ShouldReactToCongestionMarks", "If true, process received congestion marks",
                    BooleanValue(true),
                    MakeBooleanAccessor(&ConsumerPcon::m_shouldReactToCongestionMarks),
                    MakeBooleanChecker())

      .AddAttribute("ShouldUseCwa", "If true, use Conservative Window Adaptation",
                    BooleanValue(true),
                    MakeBooleanAccessor(&ConsumerPcon::m_shouldUseCwa),
                    MakeBooleanChecker());

  return tid;
}

ConsumerPcon::ConsumerPcon()
  : m_ssthresh(std::numeric_limits<double>::max())
  , m_highData(0)
  , m_recPoint(0.0)
{
}

void
ConsumerPcon::OnData(shared_ptr<const Data> data)
{
  Consumer::OnData(data);

  uint64_t sequenceNum = data->getName().get(-1).toSegment();

  // Set highest received Data to sequence number
  if (m_highData < sequenceNum) {
    m_highData = sequenceNum;
  }

  if (data->getCongestionMark() > 0) {
    if (m_shouldReactToCongestionMarks) {
      NS_LOG_DEBUG("Received congestion mark: " << data->getCongestionMark());
      WindowDecrease();
    }
    else {
      NS_LOG_DEBUG("Ignored received congestion mark: " << data->getCongestionMark());
    }
  }
  else {
    WindowIncrease();
  }

  if (m_inFlight > static_cast<uint32_t>(0)) {
    m_inFlight--;
  }

  NS_LOG_DEBUG("Window: " << m_window << ", InFlight: " << m_inFlight);

  ScheduleNextPacket();
}

void
ConsumerPcon::OnTimeout(uint32_t sequenceNum)
{
  WindowDecrease();

  if (m_inFlight > static_cast<uint32_t>(0)) {
    m_inFlight--;
  }

  NS_LOG_DEBUG("Window: " << m_window << ", InFlight: " << m_inFlight);

  Consumer::OnTimeout(sequenceNum);
}

void
ConsumerPcon::WindowIncrease()
{
  if (m_window < m_ssthresh) {
    m_window += 1.0;
  }
  else {
    m_window += (1.0 / m_window);
  }

  NS_LOG_DEBUG("Window size increased to " << m_window);
}

void
ConsumerPcon::WindowDecrease()
{
  if (m_shouldUseCwa || m_highData > m_recPoint) {
    const double diff = m_seq - m_highData;
    assert(diff > 0);

    m_recPoint = m_seq + (m_addRttSupress * diff);

    m_ssthresh = m_window * m_beta;
    m_window = m_ssthresh;

    // Window size cannot be reduced below initial size
    if (m_window < m_initialWindow) {
      m_window = m_initialWindow;
    }

    NS_LOG_DEBUG("Window size decreased to " << m_window);
  }
  else {
    NS_LOG_DEBUG("Window decrease suppressed, HighData: " << m_highData << ", RecPoint: " << m_recPoint);
  }
}

} // namespace ndn
} // namespace ns3
