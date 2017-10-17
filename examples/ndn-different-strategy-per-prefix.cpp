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

// ndn-different-strategy-per-prefix.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;
using ns3::ndn::StackHelper;
using ns3::ndn::AppHelper;
using ns3::ndn::GlobalRoutingHelper;
using ns3::ndn::StrategyChoiceHelper;

/**
 * This scenario simulates a grid topology (using PointToPointGrid module)
 *
 * In this scenario, thanks to NFD, we can choose a different forwarding
 * strategy for each prefix in each node.
 *
 * (consumer) -- ( ) -- (producer)
 *     |          |         |
 *    ( ) ------ ( ) ----- ( )
 *     |          |         |
 * (consumer) -- ( ) -- (producer)
 *
 * All links are 1Mbps with propagation 10ms delay.
 *
 * FIB is populated using NdnGlobalRoutingHelper.
 *
 * Consumer requests data from producer with frequency 100 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-different-strategy-per-prefix
 */

int
main(int argc, char* argv[])
{
  // Setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(10));

  // Read optional command-line parameters
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating 3x3 topology
  PointToPointHelper p2p;
  PointToPointGridHelper grid(3, 3, p2p);
  grid.BoundingBox(100, 100, 200, 200);

  // Install NDN stack on all nodes
  StackHelper ndnHelper;
  ndnHelper.InstallAll();

  // Installing global routing interface on all nodes
  GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting pointers to the producer/consumer nodes
  Ptr<Node> producer1 = grid.GetNode(2, 2);
  Ptr<Node> producer2 = grid.GetNode(0, 2);
  Ptr<Node> consumer1 = grid.GetNode(0, 0);
  Ptr<Node> consumer2 = grid.GetNode(2, 0);

  // Define two name prefixes
  std::string prefix1 = "/prefix1";
  std::string prefix2 = "/prefix2";

  // Install different forwarding strategies for prefix1, prefix2
  StrategyChoiceHelper::InstallAll(prefix1, "/localhost/nfd/strategy/multicast");
  StrategyChoiceHelper::InstallAll(prefix2, "/localhost/nfd/strategy/best-route");

  // Install NDN applications
  AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetPrefix(prefix1);
  consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second
  consumerHelper.Install(consumer1);

  consumerHelper.SetPrefix(prefix2);
  consumerHelper.Install(consumer2);

  AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix(prefix1);
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(producer1);

  producerHelper.SetPrefix(prefix2);
  producerHelper.Install(producer2);

  // Add /prefix1 and /prefix2 origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins(prefix1, producer1);
  ndnGlobalRoutingHelper.AddOrigins(prefix2, producer2);

  // Calculate and install FIBs
  GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(20.0));
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
