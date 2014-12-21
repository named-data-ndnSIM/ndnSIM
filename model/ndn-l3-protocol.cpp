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

#include "ndn-l3-protocol.hpp"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"

#include "ndn-face.hpp"

#include "ndn-net-device-face.hpp"
#include "../helper/ndn-stack-helper.hpp"
#include "cs/ndn-content-store.hpp"

#include <boost/foreach.hpp>

#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/internal-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/face-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/status-server.hpp"

#include "ns3/ndnSIM/NFD/daemon/face/null-face.hpp"
#include "ns3/ndnSIM/NFD/core/config-file.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/general-config-section.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/tables-config-section.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.L3Protocol");

namespace ns3 {
namespace ndn {

const uint16_t L3Protocol::ETHERNET_FRAME_TYPE = 0x7777;
const uint16_t L3Protocol::IP_STACK_PORT = 9695;

NS_OBJECT_ENSURE_REGISTERED(L3Protocol);

TypeId
L3Protocol::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::L3Protocol")
      .SetGroupName("ndn")
      .SetParent<Object>()
      .AddConstructor<L3Protocol>()

      .AddTraceSource("OutInterests", "OutInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_outInterests))
      .AddTraceSource("InInterests", "InInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_inInterests))

      ////////////////////////////////////////////////////////////////////

      .AddTraceSource("OutData", "OutData", MakeTraceSourceAccessor(&L3Protocol::m_outData))
      .AddTraceSource("InData", "InData", MakeTraceSourceAccessor(&L3Protocol::m_inData));
  return tid;
}

class L3Protocol::Impl {
private:
  friend class L3Protocol;

  shared_ptr<nfd::Forwarder> m_forwarder;

  shared_ptr<nfd::InternalFace> m_internalFace;
  shared_ptr<nfd::FibManager> m_fibManager;
  shared_ptr<nfd::FaceManager> m_faceManager;
  shared_ptr<nfd::StrategyChoiceManager> m_strategyChoiceManager;
  shared_ptr<nfd::StatusServer> m_statusServer;

  Ptr<ContentStore> m_csFromNdnSim;
};

L3Protocol::L3Protocol()
  : m_impl(new Impl())
{
  NS_LOG_FUNCTION(this);
}

L3Protocol::~L3Protocol()
{
  NS_LOG_FUNCTION(this);
}

void
L3Protocol::initialize(bool shouldUseNfdCs)
{
  m_impl->m_forwarder = make_shared<nfd::Forwarder>();

  initializeManagement(shouldUseNfdCs);

  m_impl->m_forwarder->getFaceTable().addReserved(make_shared<nfd::NullFace>(), nfd::FACEID_NULL);
  m_impl->m_forwarder->getFaceTable().addReserved(make_shared<nfd::NullFace>(
                                                    FaceUri("contentstore://")),
                                                  nfd::FACEID_CONTENT_STORE);
}

void
L3Protocol::initializeManagement(bool shouldUseNfdCs)
{
  m_impl->m_internalFace = make_shared<nfd::InternalFace>();

  m_impl->m_fibManager =
    make_shared<nfd::FibManager>(ref(m_impl->m_forwarder->getFib()),
                                 bind(&nfd::Forwarder::getFace, m_impl->m_forwarder.get(), _1),
                                 m_impl->m_internalFace, std::ref(StackHelper::getKeyChain()));

  m_impl->m_faceManager =
    make_shared<nfd::FaceManager>(ref(m_impl->m_forwarder->getFaceTable()), m_impl->m_internalFace,
                                  std::ref(StackHelper::getKeyChain()));

  m_impl->m_strategyChoiceManager =
    make_shared<nfd::StrategyChoiceManager>(ref(m_impl->m_forwarder->getStrategyChoice()),
                                            m_impl->m_internalFace,
                                            std::ref(StackHelper::getKeyChain()));

  m_impl->m_statusServer =
    make_shared<nfd::StatusServer>(m_impl->m_internalFace, ref(*m_impl->m_forwarder),
                                   std::ref(StackHelper::getKeyChain()));

  nfd::TablesConfigSection tablesConfig(m_impl->m_forwarder->getCs(),
                                        m_impl->m_forwarder->getPit(),
                                        m_impl->m_forwarder->getFib(),
                                        m_impl->m_forwarder->getStrategyChoice(),
                                        m_impl->m_forwarder->getMeasurements());

  m_impl->m_forwarder->getFaceTable().addReserved(m_impl->m_internalFace,
                                                  nfd::FACEID_INTERNAL_FACE);

  // add FIB entry for NFD Management Protocol
  shared_ptr<::nfd::fib::Entry> entry =
    m_impl->m_forwarder->getFib().insert("/localhost/nfd").first;
  entry->addNextHop(m_impl->m_internalFace, 0);
}

shared_ptr<nfd::Forwarder>
L3Protocol::getForwarder()
{
  return m_impl->m_forwarder;
}

shared_ptr<nfd::FibManager>
L3Protocol::getFibManager()
{
  return m_impl->m_fibManager;
}

shared_ptr<nfd::StrategyChoiceManager>
L3Protocol::getStrategyChoiceManager()
{
  return m_impl->m_strategyChoiceManager;
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ndn stack
 */
void
L3Protocol::NotifyNewAggregate()
{
  if (m_node == nullptr) {
    m_node = GetObject<Node>();
    if (m_node != nullptr) {
      NS_ASSERT(m_impl->m_forwarder != nullptr);
      m_impl->m_csFromNdnSim = GetObject<ContentStore>();
      if (m_impl->m_csFromNdnSim != nullptr) {
        m_impl->m_forwarder->setCsFromNdnSim(m_impl->m_csFromNdnSim);
      }
    }
  }

  Object::NotifyNewAggregate();
}

void
L3Protocol::DoDispose(void)
{
  NS_LOG_FUNCTION(this);

  m_node = 0;

  Object::DoDispose();
}

nfd::FaceId
L3Protocol::addFace(shared_ptr<Face> face)
{
  NS_LOG_FUNCTION(this << face.get());

  m_impl->m_forwarder->addFace(face);

  // Connect Signals to TraceSource
  face->onReceiveInterest +=
    [this, face](const Interest& interest) { this->m_inInterests(interest, *face); };

  face->onSendInterest +=
    [this, face](const Interest& interest) { this->m_outInterests(interest, *face); };

  face->onReceiveData += [this, face](const Data& data) { this->m_inData(data, *face); };

  face->onSendData += [this, face](const Data& data) { this->m_outData(data, *face); };

  return face->getId();
}

// void
// L3Protocol::removeFace(shared_ptr<Face> face)
// {
//   NS_LOG_FUNCTION(this << std::cref(*face));
// }

shared_ptr<Face>
L3Protocol::getFaceById(nfd::FaceId id) const
{
  return m_impl->m_forwarder->getFaceTable().get(id);
}

shared_ptr<Face>
L3Protocol::getFaceByNetDevice(Ptr<NetDevice> netDevice) const
{
  for (const auto& i : m_impl->m_forwarder->getFaceTable()) {
    shared_ptr<NetDeviceFace> netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(i);
    if (netDeviceFace == nullptr)
      continue;

    if (netDeviceFace->GetNetDevice() == netDevice)
      return i;
  }
  return nullptr;
}

} // namespace ndn
} // namespace ns3
