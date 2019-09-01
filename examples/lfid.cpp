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

#include "ns3/command-line.h"
#include "ns3/double.h"
#include "ns3/names.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/uinteger.h"

#include "ns3/ndnSIM/helper/ndn-app-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-global-routing-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-strategy-choice-helper.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/model/ndn-net-device-transport.hpp"
//#include "ns3/ndnSIM/NFD/daemon/fw/random-strategy.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/best-route-strategy2.hpp"
#include "ns3/ndnSIM/utils/topology/annotated-topology-reader.hpp"

namespace ns3 {

void
displayRoutes(const NodeContainer& allNodes, const std::string& prefix)
{
  for (const auto& n : allNodes) {
    const auto& fib = n->GetObject<ndn::L3Protocol>()->getForwarder()->getFib();
    const auto& e = fib.findLongestPrefixMatch(prefix);

    std::cout << "Node " << n->GetId() << ", prefix: " << prefix << "\n";

    for (const auto& nh : e.getNextHops()) {
      // Get remote nodeId from face:
      const auto transport = dynamic_cast<ndn::NetDeviceTransport*>(nh.getFace().getTransport());
      if (transport == nullptr)
        continue;

      const auto nd1 = transport->GetNetDevice()->GetObject<PointToPointNetDevice>();
      if (nd1 == nullptr)
        continue;

      const auto ppChannel = DynamicCast<PointToPointChannel>(nd1->GetChannel());
      if (ppChannel == nullptr)
        continue;

      auto nd2 = ppChannel->GetDevice(0);
      if (nd2->GetNode() == n)
        nd2 = ppChannel->GetDevice(1);

      std::cout << "    NextHop: " << nd2->GetNode()->GetId() << ", cost: " << nh.getCost() << "\n";
    }
    std::cout << "\n";
  }
}

int
main(int argc, char* argv[])
{
  bool grid = false; // Use grid topology?
  std::string routing = "lfid";
  CommandLine cmd;
  cmd.AddValue("grid", "use grid topology (instead of abilene)", grid);
  cmd.AddValue("routing", "which route computation to use (lfid, sp, allroutes)", routing);
  cmd.Parse(argc, argv);

  std::string topoName = "abilene";
  if (grid) {
    topoName = "grid";
  }

  std::cout << "Using " << topoName << " topology\n\n";

  AnnotatedTopologyReader topologyReader{};
  topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-" + topoName + ".txt");
  topologyReader.Read();

  ndn::StackHelper stackHelper{};
  stackHelper.InstallAll();

  // IMPORTANT: Has to be run after StackHelper!
  topologyReader.ApplyOspfMetric();

  const std::string prefix{"/prefix"};

  Ptr<Node> consumerN = Names::Find<Node>("router0");
  Ptr<Node> producerN = Names::Find<Node>("producer");
  NS_ABORT_MSG_UNLESS(consumerN && producerN, "consumer or producer name does not exist in topo file!");

  ndn::GlobalRoutingHelper routingHelper;
  routingHelper.InstallAll(); // Fills GlobalRouter with incidencies.
  routingHelper.AddOrigin(prefix, producerN);

  if (routing == "lfid") {
    routingHelper.CalculateLfidRoutes();
  }
  else if (routing == "sp") {
    routingHelper.CalculateRoutes();
  }
  else if (routing == "allroutes") {
    routingHelper.CalculateAllPossibleRoutes();
  }
  else {
    NS_FATAL_ERROR("Unknown route calculation! Use --routing {lfid|sp|allroutes}");
  }

  // IMPORTANT: Some strategy needs to be installed for displayRoutes() to work.
  ndn::StrategyChoiceHelper strategyHelper;
  strategyHelper.InstallAll<nfd::fw::BestRouteStrategy2>("/");

  // TODO: Needs RandomStrategy for test to work!
  // Uncomment after NFD version has been updated.
  //  strategyHelper.InstallAll<nfd::fw::RandomStrategy>("/");

  displayRoutes(topologyReader.GetNodes(), prefix);

  // Installing applications
  ndn::AppHelper consumerHelperX{"ns3::ndn::ConsumerCbr"};
  consumerHelperX.SetPrefix(prefix);
  consumerHelperX.SetAttribute("Frequency", DoubleValue(100.0));
  consumerHelperX.Install(consumerN);

  ndn::AppHelper producerHelper0{"ns3::ndn::Producer"};
  producerHelper0.SetPrefix(prefix);
  producerHelper0.Install(producerN);

  Simulator::Stop(Seconds(30));
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
