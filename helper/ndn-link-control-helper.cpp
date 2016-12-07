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

#include "ndn-link-control-helper.hpp"

#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/channel.h"
#include "ns3/log.h"
#include "ns3/error-model.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/pointer.h"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-transport.hpp"
#include "NFD/daemon/face/face.hpp"

#include "fw/forwarder.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.LinkControlHelper");

namespace ns3 {
namespace ndn {

void
LinkControlHelper::setErrorRate(Ptr<Node> node1, Ptr<Node> node2, double errorRate)
{
  NS_LOG_FUNCTION(node1 << node2 << errorRate);

  NS_ASSERT(node1 != nullptr && node2 != nullptr);
  NS_ASSERT(errorRate <= 1.0);

  Ptr<ndn::L3Protocol> ndn1 = node1->GetObject<ndn::L3Protocol>();
  Ptr<ndn::L3Protocol> ndn2 = node2->GetObject<ndn::L3Protocol>();

  NS_ASSERT(ndn1 != nullptr && ndn2 != nullptr);

  // iterate over all faces to find the right one
  for (const auto& face : ndn1->getForwarder()->getFaceTable()) {
    auto transport = dynamic_cast<NetDeviceTransport*>(face.getTransport());
    if (transport == nullptr)
      continue;

    Ptr<PointToPointNetDevice> nd1 = transport->GetNetDevice()->GetObject<PointToPointNetDevice>();
    if (nd1 == nullptr)
      continue;

    Ptr<Channel> channel = nd1->GetChannel();
    if (channel == nullptr)
      continue;

    Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel>(channel);

    Ptr<NetDevice> nd2 = ppChannel->GetDevice(0);
    if (nd2->GetNode() == node1)
      nd2 = ppChannel->GetDevice(1);

    if (nd2->GetNode() == node2) {
      ObjectFactory errorFactory("ns3::RateErrorModel");
      errorFactory.Set("ErrorUnit", StringValue("ERROR_UNIT_PACKET"));
      errorFactory.Set("ErrorRate", DoubleValue(errorRate));
      if (errorRate <= 0) {
        errorFactory.Set("IsEnabled", BooleanValue(false));
      }

      nd1->SetAttribute("ReceiveErrorModel", PointerValue(errorFactory.Create<ErrorModel>()));
      nd2->SetAttribute("ReceiveErrorModel", PointerValue(errorFactory.Create<ErrorModel>()));
      return;
    }
  }
  NS_FATAL_ERROR("There is no link to fail between the requested nodes");
}

void
LinkControlHelper::FailLink(Ptr<Node> node1, Ptr<Node> node2)
{
  setErrorRate(node1, node2, 1.0);
}

void
LinkControlHelper::FailLinkByName(const std::string& node1, const std::string& node2)
{
  FailLink(Names::Find<Node>(node1), Names::Find<Node>(node2));
}

void
LinkControlHelper::UpLink(Ptr<Node> node1, Ptr<Node> node2)
{
  setErrorRate(node1, node2, -0.1); // this will ensure error model is disabled
}

void
LinkControlHelper::UpLinkByName(const std::string& node1, const std::string& node2)
{
  UpLink(Names::Find<Node>(node1), Names::Find<Node>(node2));
}

} // namespace ndn
} // namespace ns3
