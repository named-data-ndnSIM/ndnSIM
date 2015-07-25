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

#include "helper/ndn-link-control-helper.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-net-device-face.hpp"
#include "model/ndn-app-face.hpp"
#include "NFD/daemon/fw/forwarder.hpp"
#include "NFD/core/scheduler.hpp"

#include <ndn-cxx/management/nfd-face-status.hpp>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperNdnLinkControlHelper, CleanupFixture)

BOOST_AUTO_TEST_CASE(TwoNodeTopology)
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  NodeContainer nodes;
  nodes.Create(2);

  PointToPointHelper p2p;
  auto link0_1 = p2p.Install(nodes.Get(0), nodes.Get(1));

  StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  AppHelper consumerHelper("ns3::ndn::ConsumerCbr");

  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("1"));
  ApplicationContainer consumerAppContainer = consumerHelper.Install(nodes.Get(0));

  AppHelper producerHelper("ns3::ndn::Producer");

  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  ApplicationContainer producerAppContainer = producerHelper.Install(nodes.Get(1));

  auto netDeviceFace1_0 = nodes.Get(1)->GetObject<L3Protocol>()->getFaceByNetDevice(link0_1.Get(1));

  Simulator::Schedule(Seconds(5.1), ndn::LinkControlHelper::FailLink, nodes.Get(0), nodes.Get(1));
  Simulator::Schedule(Seconds(10.1), ndn::LinkControlHelper::UpLink, nodes.Get(0), nodes.Get(1));

  nfd::scheduler::schedule(time::milliseconds(5200), [&] {
      BOOST_CHECK_EQUAL(netDeviceFace1_0->getFaceStatus().getNInInterests(), 6);
    });

  nfd::scheduler::schedule(time::milliseconds(10200), [&] {
      BOOST_CHECK_EQUAL(netDeviceFace1_0->getFaceStatus().getNInInterests(), 6);
    });
  nfd::scheduler::schedule(time::milliseconds(15100), [&] {
      BOOST_CHECK_EQUAL(netDeviceFace1_0->getFaceStatus().getNInInterests(), 11);
    });

  Simulator::Stop(Seconds(15.2));
  Simulator::Run();
}

BOOST_AUTO_TEST_CASE(SixNodeTopology) // Bug #2783
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(6);


  // Connecting nodes in 6 node topology:
  //
  //                            +---+
  //                         +- | 2 | -+
  //                        /   +---+   \
  //              +---+    /             \    +---+
  // +---+        |   | --+               +-- |   |        +---+
  // | 0 | ------ | 1 |                       | 4 | ------ | 5 |
  // +---+        |   | --+               +-- |   |        +---+
  //              +---+    \             /    +---+
  //                        \   +---+   /
  //                         +- | 3 | -+
  //                            +---+
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));
  auto link1_2 = p2p.Install(nodes.Get(1), nodes.Get(2));
  auto link1_3 = p2p.Install(nodes.Get(1), nodes.Get(3));
  p2p.Install(nodes.Get(2), nodes.Get(4));
  p2p.Install(nodes.Get(3), nodes.Get(4));
  p2p.Install(nodes.Get(4), nodes.Get(5));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/broadcast");

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("1")); // 1 interests a second
  consumerHelper.Install(nodes.Get(0));                       // first node

  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(nodes.Get(5)); // last node

  auto netDeviceFace2_1 = nodes.Get(2)->GetObject<L3Protocol>()->getFaceByNetDevice(link1_2.Get(1));
  auto netDeviceFace3_1 = nodes.Get(3)->GetObject<L3Protocol>()->getFaceByNetDevice(link1_3.Get(1));

  nfd::scheduler::schedule(time::milliseconds(10100), [&] {
      LinkControlHelper::FailLink(nodes.Get(1), nodes.Get(2));
    });

  // just before link failure
  nfd::scheduler::schedule(time::milliseconds(10050), [&] {
      BOOST_CHECK_EQUAL(netDeviceFace2_1->getFaceStatus().getNInInterests(), 11);
      BOOST_CHECK_EQUAL(netDeviceFace3_1->getFaceStatus().getNInInterests(), 11);
    });

  // just before link recovery
  nfd::scheduler::schedule(time::milliseconds(20050), [&] {
      BOOST_CHECK_EQUAL(netDeviceFace2_1->getFaceStatus().getNInInterests(), 11);
      BOOST_CHECK_EQUAL(netDeviceFace3_1->getFaceStatus().getNInInterests(), 21);
    });

  nfd::scheduler::schedule(time::milliseconds(20100), [&] {
      LinkControlHelper::UpLink(nodes.Get(1), nodes.Get(2));
    });

  nfd::scheduler::schedule(time::milliseconds(30050), [&] {
      BOOST_CHECK_EQUAL(netDeviceFace2_1->getFaceStatus().getNInInterests(), 21);
      BOOST_CHECK_EQUAL(netDeviceFace3_1->getFaceStatus().getNInInterests(), 31);
    });

  Simulator::Stop(Seconds(30.1));

  Simulator::Run();
  Simulator::Destroy();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
