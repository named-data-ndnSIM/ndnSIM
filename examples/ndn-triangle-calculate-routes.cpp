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

// ndn-triangle-calculate-routes.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

#include "ns3/ndnSIM/model/ndn-net-device-transport.hpp"

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

  ofstream file1("/tmp/topo1.txt");
  file1 << "router\n\n"
        << "#node	city	y	x	mpi-partition\n"
        << "A1	NA	1	1	1\n"
        << "B1	NA	80	-40	1\n"
        << "C1	NA	80	40	1\n"
        << "A2	NA	1	1	1\n"
        << "B2	NA	80	-40	1\n"
        << "C2	NA	80	40	1\n\n"
        << "link\n\n"
        << "# from  to  capacity	metric	delay	queue\n"
        << "A1	    B1	10Mbps		100	1ms	100\n"
        << "A1	    C1	10Mbps		50	1ms	100\n"
        << "B1	    C1	10Mbps		1	1ms	100\n"
        << "A2	    B2	10Mbps		50	1ms	100\n"
        << "A2	    C2	10Mbps		100	1ms	100\n"
        << "B2	    C2	10Mbps		1	1ms	100\n";
  file1.close();

  AnnotatedTopologyReader topologyReader("");
  topologyReader.SetFileName("/tmp/topo1.txt");
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll();

  topologyReader.ApplyOspfMetric();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  ndnGlobalRoutingHelper.AddOrigins("/test/prefix", Names::Find<Node>("C1"));
  ndnGlobalRoutingHelper.AddOrigins("/test/prefix", Names::Find<Node>("C2"));
  ndn::GlobalRoutingHelper::CalculateRoutes();

  auto printFib = [](Ptr<Node> node) {
    auto ndn = node->GetObject<ndn::L3Protocol>();
    for (const auto& entry : ndn->getForwarder()->getFib()) {
      cout << entry.getPrefix() << " (";

      bool isFirst = true;
      for (auto& nextHop : entry.getNextHops()) {
        cout << nextHop.getFace();
        auto& face = nextHop.getFace();
        auto transport = dynamic_cast<ndn::NetDeviceTransport*>(face.getTransport());
        if (transport == nullptr) {
          continue;
        }

        cout << " towards ";

        if (!isFirst)
          cout << ", ";
        cout << Names::FindName(transport->GetNetDevice()->GetChannel()->GetDevice(1)->GetNode());
        isFirst = false;
      }
      cout << ")" << endl;
    }
  };

  cout << "FIB content on node A1" << endl;
  printFib(Names::Find<Node>("A1"));

  cout << "FIB content on node A2" << endl;
  printFib(Names::Find<Node>("A2"));

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
