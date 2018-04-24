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

// dumb-requester.cpp

#include "one-interest-requester.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable-stream.h"
#include "ns3/string.h"

NS_LOG_COMPONENT_DEFINE("OneInterestRequester");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(OneInterestRequester);

// register NS-3 type
TypeId
OneInterestRequester::GetTypeId()
{
  static TypeId tid =
    TypeId("OneInterestRequester")
      .SetParent<ndn::App>()
      .AddConstructor<OneInterestRequester>()

      .AddAttribute("Prefix", "Requested name", StringValue("/dumb-interest"),
                    ndn::MakeNameAccessor(&OneInterestRequester::m_name), ndn::MakeNameChecker());
  return tid;
}

OneInterestRequester::OneInterestRequester()
  : m_isRunning(false)
{
}

// Processing upon start of the application
void
OneInterestRequester::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  m_isRunning = true;
  Simulator::ScheduleNow(&OneInterestRequester::SendInterest, this);
}

// Processing when application is stopped
void
OneInterestRequester::StopApplication()
{
  m_isRunning = false;
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
OneInterestRequester::SendInterest()
{
  if (!m_isRunning)
    return;

  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
  auto interest = std::make_shared<ndn::Interest>(m_name);
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  interest->setInterestLifetime(ndn::time::seconds(1));
  interest->setMustBeFresh(true);

  NS_LOG_DEBUG("Sending Interest packet for " << m_name);

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  NS_LOG_DEBUG(">> I: " << m_name);

  // Forward packet to lower (network) layer
  m_appLink->onReceiveInterest(*interest);
}

void
OneInterestRequester::OnData(std::shared_ptr<const ndn::Data> data)
{
  App::OnData(data);
  NS_LOG_DEBUG("<< D: " << data->getName());
}

} // namespace ns3
