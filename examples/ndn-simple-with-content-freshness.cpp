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

// ndn-simple-with-content-freshness.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 * This scenario demonstrates how to use content store that responds to Freshness parameter set in
 * data packets.  In other words, if the producer set FreshnessPeriod field to 2 seconds, the
 * corresponding data packet will not be considered fresh for more than 2 seconds (can be cached
 * for a shorter time, if entry is evicted earlier)
 *
 *     NS_LOG=DumbRequester ./waf --run ndn-simple-with-content-freshness
 */

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));
  p2p.Install(nodes.Get(1), nodes.Get(2));

  // Install Ndn stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Freshness::Lru", "MaxSize",
                               "2"); // allow just 2 entries to be cached
  ndnHelper.InstallAll();

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("OneInterestRequester");

  // /*
  //   1) at time 1 second requests Data from a producer that does not specify freshness
  //   2) at time 10 seconds requests the same Data packet as client 1

  //   3) at time 2 seconds requests Data from a producer that specifies freshness set to 2 seconds
  //   4) at time 12 seconds requests the same Data packet as client 3

  //   Expectation:
  //   Interests from 1, 3 and 4 will reach producers
  //   Interset from 2 will be served from cache
  //  */

  consumerHelper.SetPrefix("/no-freshness");
  consumerHelper.Install(nodes.Get(0)).Start(Seconds(1));
  consumerHelper.Install(nodes.Get(0)).Start(Seconds(10));

  consumerHelper.SetPrefix("/with-freshness");
  consumerHelper.Install(nodes.Get(0)).Start(Seconds(2));
  consumerHelper.Install(nodes.Get(0)).Start(Seconds(12));

  // Producer
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

  producerHelper.SetAttribute("Freshness", TimeValue(Seconds(-1.0))); // unlimited freshness
  producerHelper.SetPrefix("/no-freshness");
  producerHelper.Install(nodes.Get(2)); // last node

  producerHelper.SetAttribute("Freshness", TimeValue(Seconds(2.0))); // freshness 2 seconds (!!!
                                                                     // freshness granularity is 1
                                                                     // seconds !!!)
  producerHelper.SetPrefix("/with-freshness");
  producerHelper.Install(nodes.Get(2)); // last node

  Simulator::Stop(Seconds(30.0));

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
