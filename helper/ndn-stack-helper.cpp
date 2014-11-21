/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 */

#include "ndn-stack-helper.hpp"

#include "ns3/log.h"
#include "ns3/names.h"
#include "ns3/point-to-point-net-device.h"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-face.hpp"
#include "utils/ndn-time.hpp"
#include "utils/dummy-keychain.hpp"

#include <limits>
#include <map>
#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE("ndn.StackHelper");

namespace ns3 {
namespace ndn {

StackHelper::StackHelper()
  : m_needSetDefaultRoutes(false)
{
  setCustomNdnCxxClocks();

  m_ndnFactory.SetTypeId("ns3::ndn::L3Protocol");
  // m_contentStoreFactory.SetTypeId("ns3::ndn::cs::Lru");

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
  static ::ndn::DummyKeyChain keyChain;
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
    NS_FATAL_ERROR("StackHelper::Install (): Installing "
                   "a NdnStack to a node with an existing Ndn object");
    return 0;
  }

  Ptr<L3Protocol> ndn = m_ndnFactory.Create<L3Protocol>();
  // Aggregate L3Protocol on node
  node->AggregateObject(ndn);

  // NFD initialization
  ndn->initialize();

  for (uint32_t index = 0; index < node->GetNDevices(); index++) {
    Ptr<NetDevice> device = node->GetDevice(index);
    // This check does not make sense: LoopbackNetDevice is installed only if IP stack is installed,
    // Normally, ndnSIM works without IP stack, so no reason to check
    // if (DynamicCast<LoopbackNetDevice> (device) != 0)
    //   continue; // don't create face for a LoopbackNetDevice

    shared_ptr<NetDeviceFace> face;

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

    faces->Add(face);
  }

  return faces;
}

void
StackHelper::AddNetDeviceFaceCreateCallback(TypeId netDeviceType,
                                            StackHelper::NetDeviceFaceCreateCallback callback)
{
  m_netDeviceCallbacks.push_back(std::make_pair(netDeviceType, callback));
}

void
StackHelper::UpdateNetDeviceFaceCreateCallback(TypeId netDeviceType,
                                               NetDeviceFaceCreateCallback callback)
{
  for (auto& i : m_netDeviceCallbacks) {
    if (i.first == netDeviceType) {
      i.second = callback;
      return;
    }
  }
}

void
StackHelper::RemoveNetDeviceFaceCreateCallback(TypeId netDeviceType,
                                               NetDeviceFaceCreateCallback callback)
{
  m_netDeviceCallbacks.remove_if([&] (const std::pair<TypeId, NetDeviceFaceCreateCallback>& i) {
      return (i.first == netDeviceType);
    });
}

shared_ptr<NetDeviceFace>
StackHelper::DefaultNetDeviceCallback(Ptr<Node> node, Ptr<L3Protocol> ndn,
                                      Ptr<NetDevice> netDevice) const
{
  NS_LOG_DEBUG("Creating default NetDeviceFace on node " << node->GetId());

  shared_ptr<NetDeviceFace> face = std::make_shared<NetDeviceFace>(node, netDevice);

  ndn->addFace(face);
  NS_LOG_LOGIC("Node " << node->GetId() << ": added NetDeviceFace as face #"
                       << face->getLocalUri());

  return face;
}

shared_ptr<NetDeviceFace>
StackHelper::PointToPointNetDeviceCallback(Ptr<Node> node, Ptr<L3Protocol> ndn,
                                           Ptr<NetDevice> device) const
{
  NS_LOG_DEBUG("Creating point-to-point NetDeviceFace on node " << node->GetId());

  shared_ptr<NetDeviceFace> face = std::make_shared<NetDeviceFace>(node, device);

  ndn->addFace(face);
  NS_LOG_LOGIC("Node " << node->GetId() << ": added NetDeviceFace as face #"
                       << face->getLocalUri());

  return face;
}

Ptr<FaceContainer>
StackHelper::Install(const std::string& nodeName) const
{
  Ptr<Node> node = Names::Find<Node>(nodeName);
  return Install(node);
}

} // namespace ndn
} // namespace ns3
