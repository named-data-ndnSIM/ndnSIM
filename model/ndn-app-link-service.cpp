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

#include "ndn-app-link-service.hpp"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"

#include "apps/ndn-app.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.AppLinkService");

namespace ns3 {
namespace ndn {

AppLinkService::AppLinkService(Ptr<App> app)
  : m_node(app->GetNode())
  , m_app(app)
{
  NS_LOG_FUNCTION(this << app);

  NS_ASSERT(m_app != 0);
}

AppLinkService::~AppLinkService()
{
  NS_LOG_FUNCTION_NOARGS();
}

void
AppLinkService::doSendInterest(const Interest& interest)
{
  NS_LOG_FUNCTION(this << &interest);

  // to decouple callbacks
  Simulator::ScheduleNow(&App::OnInterest, m_app, interest.shared_from_this());
}

void
AppLinkService::doSendData(const Data& data)
{
  NS_LOG_FUNCTION(this << &data);

  // to decouple callbacks
  Simulator::ScheduleNow(&App::OnData, m_app, data.shared_from_this());
}

void
AppLinkService::doSendNack(const lp::Nack& nack)
{
  NS_LOG_FUNCTION(this << &nack);

  // to decouple callbacks
  Simulator::ScheduleNow(&App::OnNack, m_app, make_shared<lp::Nack>(nack));
}

//

void
AppLinkService::onReceiveInterest(const Interest& interest)
{
  this->receiveInterest(interest);
}

void
AppLinkService::onReceiveData(const Data& data)
{
  this->receiveData(data);
}

void
AppLinkService::onReceiveNack(const lp::Nack& nack)
{
  this->receiveNack(nack);
}

} // namespace ndn
} // namespace ns3
