/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 *
 */

#include "ndn-app-face.hpp"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/simulator.h"

#include "apps/ndn-app.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.AppFace");

namespace ns3 {
namespace ndn {

AppFace::AppFace(Ptr<App> app)
  : LocalFace(FaceUri("appFace://"), FaceUri("appFace://"))
  , m_node(app->GetNode())
  , m_app(app)
{
  NS_LOG_FUNCTION(this << app);

  NS_ASSERT(m_app != 0);
}

AppFace::~AppFace()
{
  NS_LOG_FUNCTION_NOARGS();
}

void
AppFace::close()
{
  this->fail("Close connection");
}

void
AppFace::sendInterest(const Interest& interest)
{
  NS_LOG_FUNCTION(this << &interest);

  this->onSendInterest(interest);

  // to decouple callbacks
  Simulator::ScheduleNow(&App::OnInterest, m_app, interest.shared_from_this());
}

void
AppFace::sendData(const Data& data)
{
  NS_LOG_FUNCTION(this << &data);

  this->onSendData(data);

  // to decouple callbacks
  Simulator::ScheduleNow(&App::OnData, m_app, data.shared_from_this());
}

} // namespace ndn
} // namespace ns3
