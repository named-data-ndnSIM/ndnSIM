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

#include "ndn-app.hpp"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-app-link-service.hpp"
#include "model/null-transport.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.App");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(App);

TypeId
App::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::ndn::App")
                        .SetGroupName("Ndn")
                        .SetParent<Application>()
                        .AddConstructor<App>()

                        .AddTraceSource("ReceivedInterests", "ReceivedInterests",
                                        MakeTraceSourceAccessor(&App::m_receivedInterests),
                                        "ns3::ndn::App::InterestTraceCallback")

                        .AddTraceSource("ReceivedDatas", "ReceivedDatas",
                                        MakeTraceSourceAccessor(&App::m_receivedDatas),
                                        "ns3::ndn::App::DataTraceCallback")

                        .AddTraceSource("ReceivedNacks", "ReceivedNacks",
                                        MakeTraceSourceAccessor(&App::m_receivedNacks),
                                        "ns3::ndn::App::NackTraceCallback")

                        .AddTraceSource("TransmittedInterests", "TransmittedInterests",
                                        MakeTraceSourceAccessor(&App::m_transmittedInterests),
                                        "ns3::ndn::App::InterestTraceCallback")

                        .AddTraceSource("TransmittedDatas", "TransmittedDatas",
                                        MakeTraceSourceAccessor(&App::m_transmittedDatas),
                                        "ns3::ndn::App::DataTraceCallback")

                        .AddTraceSource("TransmittedNacks", "TransmittedNacks",
                                        MakeTraceSourceAccessor(&App::m_transmittedNacks),
                                        "ns3::ndn::App::NackTraceCallback");
  return tid;
}

App::App()
  : m_active(false)
  , m_face(0)
  , m_appId(std::numeric_limits<uint32_t>::max())
{
}

App::~App()
{
}

void
App::DoInitialize()
{
  NS_LOG_FUNCTION_NOARGS();

  // find out what is application id on the node
  for (uint32_t id = 0; id < GetNode()->GetNApplications(); ++id) {
    if (GetNode()->GetApplication(id) == this) {
      m_appId = id;
    }
  }

  Application::DoInitialize();
}

void
App::DoDispose(void)
{
  NS_LOG_FUNCTION_NOARGS();

  // Unfortunately, this causes SEGFAULT
  // The best reason I see is that apps are freed after ndn stack is removed
  // StopApplication ();
  Application::DoDispose();
}

uint32_t
App::GetId() const
{
  return m_appId;
}

void
App::OnInterest(shared_ptr<const Interest> interest)
{
  NS_LOG_FUNCTION(this << interest);
  m_receivedInterests(interest, this, m_face);
}

void
App::OnData(shared_ptr<const Data> data)
{
  NS_LOG_FUNCTION(this << data);
  m_receivedDatas(data, this, m_face);
}

void
App::OnNack(shared_ptr<const lp::Nack> nack)
{
  NS_LOG_FUNCTION(this << nack);

  // @TODO Implement
  m_receivedNacks(nack, this, m_face);
}

// Application Methods
void
App::StartApplication() // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS();

  NS_ASSERT(m_active != true);
  m_active = true;

  NS_ASSERT_MSG(GetNode()->GetObject<L3Protocol>() != 0,
                "Ndn stack should be installed on the node " << GetNode());

  // step 1. Create a face
  auto appLink = make_unique<AppLinkService>(this);
  auto transport = make_unique<NullTransport>("appFace://", "appFace://",
                                              ::ndn::nfd::FACE_SCOPE_LOCAL);
  // @TODO Consider making AppTransport instead
  m_face = std::make_shared<Face>(std::move(appLink), std::move(transport));
  m_appLink = static_cast<AppLinkService*>(m_face->getLinkService());
  m_face->setMetric(1);

  // step 2. Add face to the Ndn stack
  GetNode()->GetObject<L3Protocol>()->addFace(m_face);
}

void
App::StopApplication() // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS();

  if (!m_active)
    return; // don't assert here, just return

  m_active = false;

  m_face->close();
}

} // namespace ndn
} // namespace ns3
