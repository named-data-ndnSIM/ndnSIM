/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2016  Regents of the University of California.
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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *
 *                             ----------------------------- (Producer)
 *                            /                                   |
 *                           /                                    |
 *    (Consumer) ------ (Non-Producer-Region Router) ----- (Produer-Region-Network Router)
 *                                                                |
 *                                                                |
 *                                                               ( )
 *
 *
 *
 *
 * Consumer sends interest with Link object attatched. Non-Producer-Region Router will
 * forward the interest based on Forwarding Hint(dalegationName in Link object) toward
 * delegationName direction because the router is not in
 * ProducerRegion. Producer-Region-Network checks incomming interests and finds out the
 * interest has reached ProducerRegion, thus the router will forward the incoming interest
 * based on interestName.
 *
 * For every received interest, producer replies with a data packet, containing 1024 bytes
 * of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=nfd.Forwarder=logic ./waf --run=ndn-simple-for-nrt-helper
 */

namespace ns3 {

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  Ptr<Node> consumer = CreateObject<Node>();
  Ptr<Node> intRouter = CreateObject<Node>();
  NodeContainer uclaRegion;
  uclaRegion.Create(3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(consumer, intRouter);
  p2p.Install(intRouter, uclaRegion.Get(0));
  p2p.Install(intRouter, uclaRegion.Get(1));
  p2p.Install(uclaRegion.Get(0), uclaRegion.Get(1));
  p2p.Install(uclaRegion.Get(1), uclaRegion.Get(2));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Install Routes Manually
  ndn::FibHelper::AddRoute(intRouter, ndn::Name("/ucla"), uclaRegion.Get(1), 1);

  // Configure NetworkRegionTable
  ndn::NetworkRegionTableHelper::AddRegionName(uclaRegion, ndn::Name("/ucla"));

  // Installing applications

  // Consumer
  ndn::AppHelper requesterHelper("RequesterApp");
  requesterHelper.SetAttribute("Name", StringValue("/ndnSIM/someData"));
  requesterHelper.SetAttribute("Delegation", StringValue("/ucla1"));
  requesterHelper.Install(consumer).Start(Seconds(1));

  requesterHelper.SetAttribute("Name", StringValue("/ndnSIM/anotherData"));
  requesterHelper.Install(consumer).Start(Seconds(2));

  requesterHelper.SetAttribute("Name", StringValue("/ndnSIM/yetAnotherData"));
  requesterHelper.SetAttribute("Delegation", StringValue("/non-existing"));
  requesterHelper.Install(consumer).Start(Seconds(3));

  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/ndnSIM");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(uclaRegion.Get(0));

  Simulator::Stop(Seconds(20.0));

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
