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

#include "ndn-face.hpp"

#include "ndn-net-device-face.hpp"
#include "../helper/ndn-stack-helper.hpp"
#include "cs/ndn-content-store.hpp"

#include <boost/property_tree/info_parser.hpp>

#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/internal-face.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/fib-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/face-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/status-server.hpp"
#include "ns3/ndnSIM/NFD/rib/rib-manager.hpp"

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
                      MakeTraceSourceAccessor(&L3Protocol::m_outInterests),
                      "ns3::ndn::L3Protocol::InterestTraceCallback")
      .AddTraceSource("InInterests", "InInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_inInterests),
                      "ns3::ndn::L3Protocol::InterestTraceCallback")

      ////////////////////////////////////////////////////////////////////

      .AddTraceSource("OutData", "OutData", MakeTraceSourceAccessor(&L3Protocol::m_outData),
                      "ns3::ndn::L3Protocol::DataTraceCallback")
      .AddTraceSource("InData", "InData", MakeTraceSourceAccessor(&L3Protocol::m_inData),
                      "ns3::ndn::L3Protocol::DataTraceCallback")

      ////////////////////////////////////////////////////////////////////

      .AddTraceSource("SatisfiedInterests", "SatisfiedInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_satisfiedInterests),
                      "ns3::ndn::L3Protocol::SatisfiedInterestsCallback")
      .AddTraceSource("TimedOutInterests", "TimedOutInterests",
                      MakeTraceSourceAccessor(&L3Protocol::m_timedOutInterests),
                      "ns3::ndn::L3Protocol::TimedOutInterestsCallback")
    ;
  return tid;
}

class L3Protocol::Impl {
private:
  Impl()
  {
    // Do not modify initial config file. Use helpers to set specific NFD parameters
    std::string initialConfig =
      "general\n"
      "{\n"
      "}\n"
      "\n"
      "tables\n"
      "{\n"
      "  cs_max_packets 100\n"
      "\n"
      "  strategy_choice\n"
      "  {\n"
      "    /               /localhost/nfd/strategy/best-route\n"
      "    /localhost      /localhost/nfd/strategy/multicast\n"
      "    /localhost/nfd  /localhost/nfd/strategy/best-route\n"
      "    /ndn/multicast  /localhost/nfd/strategy/multicast\n"
      "  }\n"
      "}\n"
      "\n"
      // "face_system\n"
      // "{\n"
      // "}\n"
      "\n"
      "authorizations\n"
      "{\n"
      "  authorize\n"
      "  {\n"
      "    certfile any\n"
      "    privileges\n"
      "    {\n"
      "      faces\n"
      "      fib\n"
      "      strategy-choice\n"
      "    }\n"
      "  }\n"
      "}\n"
      "\n"
      "rib\n"
      "{\n"
      "  localhost_security\n"
      "  {\n"
      "    trust-anchor\n"
      "    {\n"
      "      type any\n"
      "    }\n"
      "  }\n"
      "}\n"
      "\n";

    std::istringstream input(initialConfig);
    boost::property_tree::read_info(input, m_config);
  }

  friend class L3Protocol;

  shared_ptr<nfd::Forwarder> m_forwarder;

  shared_ptr<nfd::InternalFace> m_internalFace;
  shared_ptr<nfd::FibManager> m_fibManager;
  shared_ptr<nfd::FaceManager> m_faceManager;
  shared_ptr<nfd::StrategyChoiceManager> m_strategyChoiceManager;
  shared_ptr<nfd::StatusServer> m_statusServer;
  shared_ptr<nfd::rib::RibManager> m_ribManager;
  shared_ptr< ::ndn::Face> m_face;

  nfd::ConfigSection m_config;

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
L3Protocol::initialize()
{
  m_impl->m_forwarder = make_shared<nfd::Forwarder>();

  initializeManagement();
  Simulator::ScheduleWithContext(m_node->GetId(), Seconds(0), &L3Protocol::initializeRibManager, this);

  m_impl->m_forwarder->getFaceTable().addReserved(make_shared<nfd::NullFace>(), nfd::FACEID_NULL);

  m_impl->m_forwarder->beforeSatisfyInterest.connect(std::ref(m_satisfiedInterests));
  m_impl->m_forwarder->beforeExpirePendingInterest.connect(std::ref(m_timedOutInterests));
}

class IgnoreSections
{
public:
  IgnoreSections(const std::vector<std::string>& ignored)
    : m_ignored(ignored)
  {
  }

  void
  operator()(const std::string& filename, const std::string& sectionName,
             const nfd::ConfigSection& section, bool isDryRun)

  {
    if (std::find(m_ignored.begin(), m_ignored.end(), sectionName) == m_ignored.end()) {
      nfd::ConfigFile::throwErrorOnUnknownSection(filename, sectionName, section, isDryRun);
    }
  }
private:
  std::vector<std::string> m_ignored;
};

void
L3Protocol::initializeManagement()
{
  auto& keyChain = StackHelper::getKeyChain();
  auto& forwarder = m_impl->m_forwarder;
  using namespace nfd;

  m_impl->m_internalFace = make_shared<InternalFace>();

  m_impl->m_fibManager = make_shared<FibManager>(std::ref(forwarder->getFib()),
                                                 bind(&Forwarder::getFace, forwarder.get(), _1),
                                                 m_impl->m_internalFace, keyChain);

  m_impl->m_faceManager = make_shared<FaceManager>(std::ref(forwarder->getFaceTable()),
                                                   m_impl->m_internalFace,
                                                   keyChain);

  m_impl->m_strategyChoiceManager =
    make_shared<StrategyChoiceManager>(std::ref(forwarder->getStrategyChoice()),
                                       m_impl->m_internalFace,
                                       keyChain);

  m_impl->m_statusServer = make_shared<StatusServer>(m_impl->m_internalFace,
                                                     ref(*forwarder),
                                                     keyChain);

  ConfigFile config((IgnoreSections({"general", "log", "rib"})));

  TablesConfigSection tablesConfig(forwarder->getCs(),
                                   forwarder->getPit(),
                                   forwarder->getFib(),
                                   forwarder->getStrategyChoice(),
                                   forwarder->getMeasurements());
  tablesConfig.setConfigFile(config);

  m_impl->m_internalFace->getValidator().setConfigFile(config);

  forwarder->getFaceTable().addReserved(m_impl->m_internalFace, FACEID_INTERNAL_FACE);

  m_impl->m_faceManager->setConfigFile(config);

  // apply config
  config.parse(m_impl->m_config, false, "ndnSIM.conf");

  tablesConfig.ensureTablesAreConfigured();

  // add FIB entry for NFD Management Protocol
  shared_ptr<fib::Entry> entry = forwarder->getFib().insert("/localhost/nfd").first;
  entry->addNextHop(m_impl->m_internalFace, 0);
}

void
L3Protocol::initializeRibManager()
{
  using namespace nfd;

  m_impl->m_face = make_shared< ::ndn::Face>();
  m_impl->m_ribManager = make_shared<rib::RibManager>(*(m_impl->m_face),
                                                      StackHelper::getKeyChain());

  ConfigFile config([] (const std::string& filename, const std::string& sectionName,
                        const ConfigSection& section, bool isDryRun) {
      // Ignore "log" and sections belonging to NFD,
      // but raise an error if we're missing a handler for a "rib" section.
      if (sectionName != "rib" || sectionName == "log") {
        // do nothing
      }
      else {
        // missing NRD section
        ConfigFile::throwErrorOnUnknownSection(filename, sectionName, section, isDryRun);
      }
    });

  m_impl->m_ribManager->setConfigFile(config);

  // apply config
  config.parse(m_impl->m_config, false, "ndnSIM.conf");

  m_impl->m_ribManager->registerWithNfd();

  m_impl->m_ribManager->enableLocalControlHeader();
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

nfd::ConfigSection&
L3Protocol::getConfig()
{
  return m_impl->m_config;
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
      initialize();

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
  face->onReceiveInterest.connect
    ([this, face](const Interest& interest) { this->m_inInterests(interest, *face); });

  face->onSendInterest.connect
    ([this, face](const Interest& interest) { this->m_outInterests(interest, *face); });

  face->onReceiveData.connect([this, face](const Data& data) { this->m_inData(data, *face); });

  face->onSendData.connect([this, face](const Data& data) { this->m_outData(data, *face); });

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

Ptr<L3Protocol>
L3Protocol::getL3Protocol(Ptr<Object> node)
{
  Ptr<L3Protocol> retval = node->GetObject<L3Protocol>();
  NS_ASSERT_MSG(retval != nullptr, "L3Protocol is not aggregated on this object");
  return retval;
}

} // namespace ndn
} // namespace ns3
