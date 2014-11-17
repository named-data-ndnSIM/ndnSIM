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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-app.hpp"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-app-face.hpp"

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
                                        MakeTraceSourceAccessor(&App::m_receivedInterests))

                        .AddTraceSource("ReceivedDatas", "ReceivedDatas",
                                        MakeTraceSourceAccessor(&App::m_receivedDatas))

                        .AddTraceSource("TransmittedInterests", "TransmittedInterests",
                                        MakeTraceSourceAccessor(&App::m_transmittedInterests))

                        .AddTraceSource("TransmittedDatas", "TransmittedDatas",
                                        MakeTraceSourceAccessor(&App::m_transmittedDatas));
  return tid;
}

App::App()
  : m_active(false)
  , m_face(0)
{
}

App::~App()
{
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
  if (m_face == 0)
    return (uint32_t)-1;
  else
    return m_face->getId();
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
  m_face = std::make_shared<AppFace>(this);

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
