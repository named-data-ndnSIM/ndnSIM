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

// ndn-simple-withl2tracer.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 10);
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-tree-25-node.txt");
  topologyReader.Read();

  /****************************************************************************/
  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "1000");
  ndnHelper.InstallAll();
  /****************************************************************************/
  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();
  /****************************************************************************/
  // Getting containers for the consumer/producer
  Ptr<Node> consumer1 = Names::Find<Node>("Src1");
  Ptr<Node> consumer2 = Names::Find<Node>("Src2");
  Ptr<Node> consumer3 = Names::Find<Node>("Src3");
  Ptr<Node> consumer4 = Names::Find<Node>("Src4");
  Ptr<Node> consumer5 = Names::Find<Node>("Src5");
  Ptr<Node> consumer6 = Names::Find<Node>("Src6");
  Ptr<Node> consumer7 = Names::Find<Node>("Src7");
  Ptr<Node> consumer8 = Names::Find<Node>("Src8");
  Ptr<Node> consumer9 = Names::Find<Node>("Src9");

  Ptr<Node> producer1 = Names::Find<Node>("Dst1");
  Ptr<Node> producer2 = Names::Find<Node>("Dst2");
  Ptr<Node> producer3 = Names::Find<Node>("Dst3");
  Ptr<Node> producer4 = Names::Find<Node>("Dst4");
  Ptr<Node> producer5 = Names::Find<Node>("Dst5");
  Ptr<Node> producer6 = Names::Find<Node>("Dst6");
  Ptr<Node> producer7 = Names::Find<Node>("Dst7");
  Ptr<Node> producer8 = Names::Find<Node>("Dst8");
  Ptr<Node> producer9 = Names::Find<Node>("Dst9");
  /****************************************************************************/
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetAttribute("Frequency", StringValue("1000")); // interests per Second
  consumerHelper.SetAttribute("Randomize", StringValue("uniform"));
  /****************************************************************************/
  // on the first to ninth consumer node install a Consumer application
  // that will express interests in /dst1 to /dst9 namespace
  consumerHelper.SetPrefix("/dst9");
  consumerHelper.Install(consumer1);

  consumerHelper.SetPrefix("/dst8");
  consumerHelper.Install(consumer2);

  consumerHelper.SetPrefix("/dst7");
  consumerHelper.Install(consumer3);

  consumerHelper.SetPrefix("/dst6");
  consumerHelper.Install(consumer4);

  consumerHelper.SetPrefix("/dst5");
  consumerHelper.Install(consumer5);

  consumerHelper.SetPrefix("/dst4");
  consumerHelper.Install(consumer6);

  consumerHelper.SetPrefix("/dst3");
  consumerHelper.Install(consumer7);

  consumerHelper.SetPrefix("/dst2");
  consumerHelper.Install(consumer8);

  consumerHelper.SetPrefix("/dst1");
  consumerHelper.Install(consumer9);

  /****************************************************************************/
  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  /****************************************************************************/
  // Register /dst1 to /dst9 prefix with global routing controller and
  // install producer that will satisfy Interests in /dst1 to /dst9 namespace
  ndnGlobalRoutingHelper.AddOrigins("/dst1", producer1);
  producerHelper.SetPrefix("/dst1");
  producerHelper.Install(producer1);

  ndnGlobalRoutingHelper.AddOrigins("/dst2", producer2);
  producerHelper.SetPrefix("/dst2");
  producerHelper.Install(producer2);

  ndnGlobalRoutingHelper.AddOrigins("/dst3", producer3);
  producerHelper.SetPrefix("/dst3");
  producerHelper.Install(producer3);

  ndnGlobalRoutingHelper.AddOrigins("/dst4", producer4);
  producerHelper.SetPrefix("/dst4");
  producerHelper.Install(producer4);

  ndnGlobalRoutingHelper.AddOrigins("/dst5", producer5);
  producerHelper.SetPrefix("/dst5");
  producerHelper.Install(producer5);

  ndnGlobalRoutingHelper.AddOrigins("/dst6", producer6);
  producerHelper.SetPrefix("/dst6");
  producerHelper.Install(producer6);

  ndnGlobalRoutingHelper.AddOrigins("/dst7", producer7);
  producerHelper.SetPrefix("/dst7");
  producerHelper.Install(producer7);

  ndnGlobalRoutingHelper.AddOrigins("/dst8", producer8);
  producerHelper.SetPrefix("/dst8");
  producerHelper.Install(producer8);

  ndnGlobalRoutingHelper.AddOrigins("/dst9", producer9);
  producerHelper.SetPrefix("/dst9");
  producerHelper.Install(producer9);

  /*****************************************************************************/
  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(10.0));

  /****************************************************************************/
  // Tracer:

  L2RateTracer::InstallAll("drop-trace.txt", Seconds(0.5));

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
