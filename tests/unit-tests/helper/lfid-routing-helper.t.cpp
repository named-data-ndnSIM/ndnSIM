/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2019  Regents of the University of California.
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

#include "helper/ndn-global-routing-helper.hpp"
#include "helper/ndn-stack-helper.hpp"

#include "model/ndn-global-router.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-transport.hpp"

#include "NFD/daemon/fw/best-route-strategy2.hpp"

#include "ns3/channel.h"
#include "ns3/net-device.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"

#include "../tests-common.hpp"

#include <boost/filesystem.hpp>

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperLfidRoutingHelper, CleanupFixture)

BOOST_AUTO_TEST_CASE(CalculateRouteAbilene)
{
  AnnotatedTopologyReader topologyReader;
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-abilene.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper stackHelper{};
  stackHelper.InstallAll();

  // IMPORTANT: Has to be run after StackHelper!
  topologyReader.ApplyOspfMetric();

  const std::string prefix{"/prefix"};

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  BOOST_CHECK_NO_THROW(ndnGlobalRoutingHelper.InstallAll());

//  auto producer = Names::Find<Node>("producer");
  const NodeContainer allNodes {topologyReader.GetNodes()};

  // Make every node a producer for their prefix:
  for (int i = 0; i < allNodes.size(); i++) {
    ndnGlobalRoutingHelper.AddOrigins(prefix + std::to_string(i), allNodes.Get(i));
  }
  BOOST_CHECK_NO_THROW(ndn::GlobalRoutingHelper::CalculateLfidRoutes());

  // IMPORTANT: Some strategy needs to be installed for test to work.
  ndn::StrategyChoiceHelper str;
  str.InstallAll<nfd::fw::BestRouteStrategy2>("/");

  int numNexthops = 0;

  // For all nodes
  for (const auto& n : allNodes) {

    // For all producer prefixes i
    for (int i = 0; i < allNodes.size(); i++) {
      if (n->GetId() == i) continue;

      std::string prodPrefix = prefix + std::to_string(i);

      const auto& fib = n->GetObject<ndn::L3Protocol>()->getForwarder()->getFib();
      auto& e = fib.findLongestPrefixMatch(prodPrefix);

      // Check that each node has at least 1 nexthop
      BOOST_CHECK_GE(e.getNextHops().size(), 1);

      for (const auto& nh : e.getNextHops()) {
        // Get remote nodeId from face:
        const auto& transport =
            dynamic_cast<ndn::NetDeviceTransport*>(nh.getFace().getTransport());
        BOOST_ASSERT(transport);

        const auto& nd1 = transport->GetNetDevice()->GetObject<PointToPointNetDevice>();
        BOOST_ASSERT(nd1);

        const auto& ppChannel = DynamicCast<PointToPointChannel>(nd1->GetChannel());
        BOOST_ASSERT(ppChannel);

        auto nd2 = ppChannel->GetDevice(0);
        // If node in channel is own node -> Switch to other node.
        if (nd2->GetNode() == n) {
          nd2 = ppChannel->GetDevice(1);
        }

        // Cost must be greater than 0.
        BOOST_CHECK_GE(nh.getCost(), 1);
      }

      numNexthops += e.getNextHops().size();
    }
  }

  BOOST_CHECK_EQUAL(numNexthops, 226);
}


BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
