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

#include "helper/ndn-strategy-choice-helper.hpp"
#include "helper/ndn-face-container.hpp"
#include "model/ndn-net-device-face.hpp"

#include "ns3/node-container.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-grid.h"
#include "ns3/node.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(TestStrategyChoiceHelper, CleanupFixture)

BOOST_AUTO_TEST_CASE(GridScenario)
{
  ofstream file("/tmp/topo3.txt");
  file  << "router\n\n"
        << "#node city  y x mpi-partition\n"
        << "A3  NA  10 10 1\n"
        << "B3  NA  20 10 1\n"
        << "C3  NA  10 20 1\n"
        << "D3  NA  20 20 1\n\n"
        << "link\n\n"
        << "# from  to  capacity  metric  delay queue\n"
        << "A3      B3  10Mbps    100 1ms 500\n"
        << "A3      C3  10Mbps    200 1ms 500\n"
        << "B3      D3  10Mbps    100 1ms 500\n"
        << "C3      D3  10Mbps    200 1ms 500\n";
  file.close();

  AnnotatedTopologyReader topologyReader("");
  topologyReader.SetFileName("/tmp/topo3.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  Ptr<FaceContainer> node_faceContainer = ndnHelper.InstallAll();
  GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Install different forwarding strategies
  StrategyChoiceHelper::Install(Names::Find<Node>("A3"), "/prefix",
                                "/localhost/nfd/strategy/broadcast");
  StrategyChoiceHelper::Install(Names::Find<Node>("B3"), "/prefix",
                                "/localhost/nfd/strategy/best-route");
  StrategyChoiceHelper::Install(Names::Find<Node>("C3"), "/prefix",
                                "/localhost/nfd/strategy/broadcast");
  StrategyChoiceHelper::Install(Names::Find<Node>("D3"), "/prefix",
                                "/localhost/nfd/strategy/best-route");

  AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", StringValue("100")); // 100 interests a second
  consumerHelper.Install(Names::Find<Node>("A3"));

  AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(Names::Find<Node>("D3"));

  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins("/prefix", Names::Find<Node>("D3"));

  // Calculate and install FIBs
  GlobalRoutingHelper::CalculateRoutes();

  // Node0 is consumer which is connected to intermediate Node1 and Node2

  FaceContainer::Iterator IntermediateNode1Face1Iterator = node_faceContainer->Begin() + 2; // Node1 with strategy broadcast
  FaceContainer::Iterator IntermediateNode1Face2Iterator = node_faceContainer->Begin() + 3;

  FaceContainer::Iterator IntermediateNode2Face1Iterator = node_faceContainer->Begin() + 4; // Node2 with strategy best route
  FaceContainer::Iterator IntermediateNode2Face2Iterator = node_faceContainer->Begin() + 5;

  FaceContainer::Iterator ProducerFaceIterator1 = node_faceContainer->Begin() + 6;
  FaceContainer::Iterator ProducerFaceIterator2 = node_faceContainer->Begin() + 7;

  auto Producer_netDeviceFace1 = std::dynamic_pointer_cast<NetDeviceFace>(node_faceContainer->Get(ProducerFaceIterator1));
  auto Producer_netDeviceFace2 = std::dynamic_pointer_cast<NetDeviceFace>(node_faceContainer->Get(ProducerFaceIterator2));

  auto IntermediateNode1_netDeviceFace1 = std::dynamic_pointer_cast<NetDeviceFace>(node_faceContainer->Get(IntermediateNode1Face1Iterator));
  auto IntermediateNode1_netDeviceFace2 = std::dynamic_pointer_cast<NetDeviceFace>(node_faceContainer->Get(IntermediateNode1Face2Iterator));

  auto IntermediateNode2_netDeviceFace1 = std::dynamic_pointer_cast<NetDeviceFace>(node_faceContainer->Get(IntermediateNode2Face1Iterator));
  auto IntermediateNode2_netDeviceFace2 = std::dynamic_pointer_cast<NetDeviceFace>(node_faceContainer->Get(IntermediateNode2Face2Iterator));

  Simulator::Stop(Seconds(5.0));
  Simulator::Run();

  ::ndn::nfd::FaceStatus producerFace1Status = Producer_netDeviceFace1->getFaceStatus();
  ::ndn::nfd::FaceStatus producerFace2Status = Producer_netDeviceFace2->getFaceStatus();

  ::ndn::nfd::FaceStatus IntermediateNode1Face1Status = IntermediateNode1_netDeviceFace1->getFaceStatus();
  ::ndn::nfd::FaceStatus IntermediateNode1Face2Status = IntermediateNode1_netDeviceFace2->getFaceStatus();

  ::ndn::nfd::FaceStatus IntermediateNode2Face1Status = IntermediateNode2_netDeviceFace1->getFaceStatus();
  ::ndn::nfd::FaceStatus IntermediateNode2Face2Status = IntermediateNode2_netDeviceFace2->getFaceStatus();

  BOOST_CHECK_EQUAL(producerFace1Status.getNInInterests(), 500);
  BOOST_CHECK_EQUAL(producerFace2Status.getNInInterests(), 0);
  BOOST_CHECK_EQUAL(IntermediateNode1Face1Status.getNInInterests(), 500);
  BOOST_CHECK_EQUAL(IntermediateNode1Face2Status.getNInInterests(), 0);
  BOOST_CHECK_EQUAL(IntermediateNode2Face1Status.getNInInterests(), 0);
  BOOST_CHECK_EQUAL(IntermediateNode2Face2Status.getNInInterests(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
