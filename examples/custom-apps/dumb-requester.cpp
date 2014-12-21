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

#include "dumb-requester.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"
#include "ns3/string.h"

#include "ns3/ndn-app-face.hpp"

NS_LOG_COMPONENT_DEFINE("DumbRequester");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(DumbRequester);

// register NS-3 type
TypeId
DumbRequester::GetTypeId()
{
  static TypeId tid =
    TypeId("DumbRequester")
      .SetParent<ndn::App>()
      .AddConstructor<DumbRequester>()

      .AddAttribute("Prefix", "Requested name", StringValue("/dumb-interest"),
                    ndn::MakeNameAccessor(&DumbRequester::m_name), ndn::MakeNameChecker());
  return tid;
}

DumbRequester::DumbRequester()
  : m_isRunning(false)
{
}

// Processing upon start of the application
void
DumbRequester::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  m_isRunning = true;
  Simulator::ScheduleNow(&DumbRequester::SendInterest, this);
}

// Processing when application is stopped
void
DumbRequester::StopApplication()
{
  m_isRunning = false;
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
DumbRequester::SendInterest()
{
  if (!m_isRunning)
    return;

  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
  auto interest = make_shared<ndn::Interest>(*m_name);
  UniformVariable rand(0, std::numeric_limits<uint32_t>::max());
  interest->setNonce(rand.GetValue());
  interest->setName(*prefix);
  interest->setInterestLifetime(time::seconds(1));

  NS_LOG_DEBUG("Sending Interest packet for " << *prefix);

  // Call trace (for logging purposes)
  m_transmittedInterests(interest, this, m_face);

  // Forward packet to lower (network) layer
  m_face->onReceiveInterest(*interest);

  Simulator::Schedule(Seconds(1.0), &DumbRequester::SendInterest, this);
}

void
DumbRequester::OnData(shared_ptr<const ndn::Data> contentObject)
{
  NS_LOG_DEBUG("Receiving Data packet for " << contentObject->getName());
}

} // namespace ns3
