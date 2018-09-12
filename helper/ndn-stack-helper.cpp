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

#include "ndn-stack-helper.hpp"

#include "ns3/log.h"
#include "ns3/names.h"
#include "ns3/string.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/node-list.h"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-transport.hpp"
#include "utils/ndn-time.hpp"
#include "utils/dummy-keychain.hpp"
#include "model/cs/ndn-content-store.hpp"

#include <limits>
#include <map>
#include <boost/lexical_cast.hpp>

#include "ns3/ndnSIM/NFD/daemon/face/generic-link-service.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/cs-policy-priority-fifo.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/cs-policy-lru.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.StackHelper");

namespace ns3 {
namespace ndn {

StackHelper::StackHelper()
  : m_isRibManagerDisabled(false)
  // , m_isFaceManagerDisabled(false)
  , m_isForwarderStatusManagerDisabled(false)
  , m_isStrategyChoiceManagerDisabled(false)
  , m_needSetDefaultRoutes(false)
  , m_maxCsSize(100)
{
  setCustomNdnCxxClocks();

  m_csPolicies.insert({"nfd::cs::lru", [] { return make_unique<nfd::cs::LruPolicy>(); }});
  m_csPolicies.insert({"nfd::cs::priority_fifo", [] () { return make_unique<nfd::cs::PriorityFifoPolicy>(); }});

  m_csPolicyCreationFunc = m_csPolicies["nfd::cs::lru"];

  m_ndnFactory.SetTypeId("ns3::ndn::L3Protocol");
  m_contentStoreFactory.SetTypeId("ns3::ndn::cs::Lru");

  m_netDeviceCallbacks.push_back(
    std::make_pair(PointToPointNetDevice::GetTypeId(),
                   MakeCallback(&StackHelper::PointToPointNetDeviceCallback, this)));
  // default callback will be fired if non of others callbacks fit or did the job
}

StackHelper::~StackHelper()
{
}

KeyChain&
StackHelper::getKeyChain()
{
  static ::ndn::KeyChain keyChain("pib-dummy", "tpm-dummy");
  return keyChain;
}

void
StackHelper::setCustomNdnCxxClocks()
{
  ::ndn::time::setCustomClocks(make_shared<ns3::ndn::time::CustomSteadyClock>(),
                               make_shared<ns3::ndn::time::CustomSystemClock>());
}

void
StackHelper::SetDefaultRoutes(bool needSet)
{
  NS_LOG_FUNCTION(this << needSet);
  m_needSetDefaultRoutes = needSet;
}

void
StackHelper::SetStackAttributes(const std::string& attr1, const std::string& value1,
                                const std::string& attr2, const std::string& value2,
                                const std::string& attr3, const std::string& value3,
                                const std::string& attr4, const std::string& value4)
{
  if (attr1 != "")
    m_ndnFactory.Set(attr1, StringValue(value1));
  if (attr2 != "")
    m_ndnFactory.Set(attr2, StringValue(value2));
  if (attr3 != "")
    m_ndnFactory.Set(attr3, StringValue(value3));
  if (attr4 != "")
    m_ndnFactory.Set(attr4, StringValue(value4));
}

void
StackHelper::SetOldContentStore(const std::string& contentStore, const std::string& attr1,
                                const std::string& value1, const std::string& attr2,
                                const std::string& value2, const std::string& attr3,
                                const std::string& value3, const std::string& attr4,
                                const std::string& value4)
{
  m_maxCsSize = 0;

  m_contentStoreFactory.SetTypeId(contentStore);
  if (attr1 != "")
    m_contentStoreFactory.Set(attr1, StringValue(value1));
  if (attr2 != "")
    m_contentStoreFactory.Set(attr2, StringValue(value2));
  if (attr3 != "")
    m_contentStoreFactory.Set(attr3, StringValue(value3));
  if (attr4 != "")
    m_contentStoreFactory.Set(attr4, StringValue(value4));
}

void
StackHelper::setCsSize(size_t maxSize)
{
  m_maxCsSize = maxSize;
}

void
StackHelper::setPolicy(const std::string& policy)
{
  auto found = m_csPolicies.find(policy);
  if (found != m_csPolicies.end()) {
    m_csPolicyCreationFunc = found->second;
  }
  else {
    NS_FATAL_ERROR("Cache replacement policy " << policy << " not found");
    NS_LOG_DEBUG("Available cache replacement policies: ");
    for (auto it = m_csPolicies.begin(); it != m_csPolicies.end(); it++) {
      NS_LOG_DEBUG("    " << it->first);
    }
  }
}

Ptr<FaceContainer>
StackHelper::Install(const NodeContainer& c) const
{
  Ptr<FaceContainer> faces = Create<FaceContainer>();
  for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i) {
    faces->AddAll(Install(*i));
  }
  return faces;
}

Ptr<FaceContainer>
StackHelper::InstallAll() const
{
  return Install(NodeContainer::GetGlobal());
}

Ptr<FaceContainer>
StackHelper::Install(Ptr<Node> node) const
{
  Ptr<FaceContainer> faces = Create<FaceContainer>();

  if (node->GetObject<L3Protocol>() != 0) {
    NS_FATAL_ERROR("Cannot re-install NDN stack on node "
                   << node->GetId());
    return 0;
  }

  Ptr<L3Protocol> ndn = m_ndnFactory.Create<L3Protocol>();

  if (m_isRibManagerDisabled) {
    ndn->getConfig().put("ndnSIM.disable_rib_manager", true);
  }

  // if (m_isFaceManagerDisabled) {
  //   ndn->getConfig().put("ndnSIM.disable_face_manager", true);
  // }

  if (m_isForwarderStatusManagerDisabled) {
    ndn->getConfig().put("ndnSIM.disable_forwarder_status_manager", true);
  }

  if (m_isStrategyChoiceManagerDisabled) {
    ndn->getConfig().put("ndnSIM.disable_strategy_choice_manager", true);
  }

  ndn->getConfig().put("tables.cs_max_packets", (m_maxCsSize == 0) ? 1 : m_maxCsSize);

  // Create and aggregate content store if NFD's contest store has been disabled
  if (m_maxCsSize == 0) {
    ndn->AggregateObject(m_contentStoreFactory.Create<ContentStore>());
  }
  // if NFD's CS is enabled, check if a replacement policy has been specified
  else {
    ndn->setCsReplacementPolicy(m_csPolicyCreationFunc);
  }

  // Aggregate L3Protocol on node (must be after setting ndnSIM CS)
  node->AggregateObject(ndn);

  for (uint32_t index = 0; index < node->GetNDevices(); index++) {
    Ptr<NetDevice> device = node->GetDevice(index);
    // This check does not make sense: LoopbackNetDevice is installed only if IP stack is installed,
    // Normally, ndnSIM works without IP stack, so no reason to check
    // if (DynamicCast<LoopbackNetDevice> (device) != 0)
    //   continue; // don't create face for a LoopbackNetDevice

    faces->Add(this->createAndRegisterFace(node, ndn, device));
  }

  return faces;
}

void
StackHelper::AddFaceCreateCallback(TypeId netDeviceType,
                                   StackHelper::FaceCreateCallback callback)
{
  m_netDeviceCallbacks.push_back(std::make_pair(netDeviceType, callback));
}

void
StackHelper::UpdateFaceCreateCallback(TypeId netDeviceType,
                                      FaceCreateCallback callback)
{
  for (auto& i : m_netDeviceCallbacks) {
    if (i.first == netDeviceType) {
      i.second = callback;
      return;
    }
  }
}

void
StackHelper::RemoveFaceCreateCallback(TypeId netDeviceType,
                                      FaceCreateCallback callback)
{
  m_netDeviceCallbacks.remove_if([&] (const std::pair<TypeId, FaceCreateCallback>& i) {
      return (i.first == netDeviceType);
    });
}

std::string
constructFaceUri(Ptr<NetDevice> netDevice)
{
  std::string uri = "netdev://";
  Address address = netDevice->GetAddress();
  if (Mac48Address::IsMatchingType(address)) {
    uri += "[" + boost::lexical_cast<std::string>(Mac48Address::ConvertFrom(address)) + "]";
  }

  return uri;
}


shared_ptr<Face>
StackHelper::DefaultNetDeviceCallback(Ptr<Node> node, Ptr<L3Protocol> ndn,
                                      Ptr<NetDevice> netDevice) const
{
  NS_LOG_DEBUG("Creating default Face on node " << node->GetId());

  // Create an ndnSIM-specific transport instance
  ::nfd::face::GenericLinkService::Options opts;
  opts.allowFragmentation = true;
  opts.allowReassembly = true;
  opts.allowCongestionMarking = true;

  auto linkService = make_unique<::nfd::face::GenericLinkService>(opts);

  auto transport = make_unique<NetDeviceTransport>(node, netDevice,
                                                   constructFaceUri(netDevice),
                                                   "netdev://[ff:ff:ff:ff:ff:ff]");

  auto face = std::make_shared<Face>(std::move(linkService), std::move(transport));
  face->setMetric(1);

  ndn->addFace(face);
  NS_LOG_LOGIC("Node " << node->GetId() << ": added Face as face #"
                       << face->getLocalUri());

  return face;
}

shared_ptr<Face>
StackHelper::PointToPointNetDeviceCallback(Ptr<Node> node, Ptr<L3Protocol> ndn,
                                           Ptr<NetDevice> device) const
{
  NS_LOG_DEBUG("Creating point-to-point Face on node " << node->GetId());

  Ptr<PointToPointNetDevice> netDevice = DynamicCast<PointToPointNetDevice>(device);
  NS_ASSERT(netDevice != nullptr);

  // access the other end of the link
  Ptr<PointToPointChannel> channel = DynamicCast<PointToPointChannel>(netDevice->GetChannel());
  NS_ASSERT(channel != nullptr);

  Ptr<NetDevice> remoteNetDevice = channel->GetDevice(0);
  if (remoteNetDevice->GetNode() == node)
    remoteNetDevice = channel->GetDevice(1);

  // Create an ndnSIM-specific transport instance
  ::nfd::face::GenericLinkService::Options opts;
  opts.allowFragmentation = true;
  opts.allowReassembly = true;
  opts.allowCongestionMarking = true;

  auto linkService = make_unique<::nfd::face::GenericLinkService>(opts);

  auto transport = make_unique<NetDeviceTransport>(node, netDevice,
                                                   constructFaceUri(netDevice),
                                                   constructFaceUri(remoteNetDevice));

  auto face = std::make_shared<Face>(std::move(linkService), std::move(transport));
  face->setMetric(1);

  ndn->addFace(face);
  NS_LOG_LOGIC("Node " << node->GetId() << ": added Face as face #"
                       << face->getLocalUri());

  return face;
}

Ptr<FaceContainer>
StackHelper::Install(const std::string& nodeName) const
{
  Ptr<Node> node = Names::Find<Node>(nodeName);
  return Install(node);
}

void
StackHelper::Update(Ptr<Node> node)
{
  if (node->GetObject<L3Protocol>() == 0) {
    Install(node);
    return;
  }

  Ptr<L3Protocol> ndn = node->GetObject<L3Protocol>();

  for (uint32_t index = 0; index < node->GetNDevices(); index++) {

    Ptr<NetDevice> device = node->GetDevice(index);

    if (ndn->getFaceByNetDevice(device) == nullptr) {
      this->createAndRegisterFace(node, ndn, device);
    }
  }
}

void
StackHelper::Update(const NodeContainer& c)
{
  for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i) {
    Update(*i);
  }
}

void
StackHelper::Update(const std::string& nodeName)
{
  Ptr<Node> node = Names::Find<Node>(nodeName);
  Update(node);
}

void
StackHelper::UpdateAll()
{
  Update(NodeContainer::GetGlobal());
}

shared_ptr<Face>
StackHelper::createAndRegisterFace(Ptr<Node> node, Ptr<L3Protocol> ndn, Ptr<NetDevice> device) const
{
  shared_ptr<Face> face;

  for (const auto& item : m_netDeviceCallbacks) {
    if (device->GetInstanceTypeId() == item.first ||
        device->GetInstanceTypeId().IsChildOf(item.first)) {
      face = item.second(node, ndn, device);
      if (face != 0)
        break;
    }
  }

  if (face == 0) {
    face = DefaultNetDeviceCallback(node, ndn, device);
  }

  if (m_needSetDefaultRoutes) {
    // default route with lowest priority possible
    FibHelper::AddRoute(node, "/", face, std::numeric_limits<int32_t>::max());
  }
  return face;
}

void
StackHelper::disableRibManager()
{
  m_isRibManagerDisabled = true;
}

// void
// StackHelper::disableFaceManager()
// {
//   m_isFaceManagerDisabled = true;
// }

void
StackHelper::disableStrategyChoiceManager()
{
  m_isStrategyChoiceManagerDisabled = true;
}

void
StackHelper::disableForwarderStatusManager()
{
  m_isForwarderStatusManagerDisabled = true;
}

void
StackHelper::SetLinkDelayAsFaceMetric()
{
  for (uint32_t i = 0; i < NodeList::GetNNodes(); ++i) {
    auto ndn = NodeList::GetNode(i)->GetObject<L3Protocol>();
    if (ndn == nullptr)
      continue;

    for (auto& face : ndn->getForwarder()->getFaceTable()) {
      auto transport = dynamic_cast<NetDeviceTransport*>(face.getTransport());
      if (transport == nullptr)
        continue;
      auto p2p = dynamic_cast<PointToPointChannel*>(&(*(transport->GetNetDevice()->GetChannel())));
      TimeValue currentDelay;
      p2p->GetAttribute("Delay", currentDelay);
      face.setMetric((currentDelay.Get().ToDouble(Time::S)) * 1000);

      std::cout << "Node " << i << ": Face " << face.getId()
                << " with metric " << face.getMetric() << "\n";
    }
  }
}

} // namespace ndn
} // namespace ns3
