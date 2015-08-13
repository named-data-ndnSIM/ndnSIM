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

#include "helper/ndn-global-routing-helper.hpp"

#include "model/ndn-global-router.hpp"
#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-face.hpp"
#include "model/ndn-net-device-face.hpp"

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

const boost::filesystem::path TEST_TOPO_TXT = boost::filesystem::path(TEST_CONFIG_PATH) / "topo.txt";

class GlobalRoutingHelperFixture : public CleanupFixture
{
public:
  GlobalRoutingHelperFixture()
  {
    boost::filesystem::create_directories(TEST_CONFIG_PATH);
  }

  ~GlobalRoutingHelperFixture()
  {
    boost::filesystem::remove(TEST_TOPO_TXT);
  }
};

BOOST_FIXTURE_TEST_SUITE(HelperGlobalRoutingHelper, GlobalRoutingHelperFixture)

BOOST_AUTO_TEST_CASE(CalculateRouteCase1)
{
  ofstream file1(TEST_TOPO_TXT.string().c_str());
  file1 << "router\n\n"
        << "#node city  y x mpi-partition\n"
        << "A1  NA  1 1 1\n"
        << "B1  NA  80  -40 1\n"
        << "C1  NA  80  40  1\n\n"
        << "link\n\n"
        << "# from  to  capacity  metric  delay queue\n"
        << "A1      B1  10Mbps    100 1ms 100\n"
        << "A1      C1  10Mbps    50  1ms 100\n"
        << "B1      C1  10Mbps    1 1ms 100\n";
  file1.close();

  AnnotatedTopologyReader topologyReader("");
  topologyReader.SetFileName(TEST_TOPO_TXT.string().c_str());
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll();

  topologyReader.ApplyOspfMetric();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  BOOST_CHECK_NO_THROW(ndnGlobalRoutingHelper.InstallAll());

  ndnGlobalRoutingHelper.AddOrigins("/test/prefix", Names::Find<Node>("C1"));
  BOOST_CHECK_NO_THROW(ndn::GlobalRoutingHelper::CalculateRoutes());

  auto ndn = Names::Find<Node>("A1")->GetObject<ndn::L3Protocol>();
  for (const auto& entry : ndn->getForwarder()->getFib()) {
    bool isFirst = true;
    for (auto& nextHop : entry.getNextHops()) {
      auto face = dynamic_pointer_cast<ndn::NetDeviceFace>(nextHop.getFace());
      if (face == nullptr)
        continue;
      BOOST_CHECK_EQUAL(Names::FindName(face->GetNetDevice()->GetChannel()->GetDevice(1)->GetNode()), "C1");
      isFirst = false;
    }
  }
}

BOOST_AUTO_TEST_CASE(CalculateRouteCase2)
{
  ofstream file1(TEST_TOPO_TXT.string().c_str());
  file1 << "router\n\n"
        << "#node city  y x mpi-partition\n"
        << "A2  NA  1 1 1\n"
        << "B2  NA  80  -40 1\n"
        << "C2  NA  80  40  1\n\n"
        << "link\n\n"
        << "# from  to  capacity  metric  delay queue\n"
        << "A2      B2  10Mbps    100 1ms 100\n"
        << "A2      C2  10Mbps    500  1ms 100\n"
        << "B2      C2  10Mbps    1 1ms 100\n";
  file1.close();

  AnnotatedTopologyReader topologyReader("");
  topologyReader.SetFileName(TEST_TOPO_TXT.string().c_str());
  topologyReader.Read();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll();

  topologyReader.ApplyOspfMetric();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  ndnGlobalRoutingHelper.AddOrigins("/prefix", Names::Find<Node>("C2"));
  ndn::GlobalRoutingHelper::CalculateRoutes();

  auto ndn = Names::Find<Node>("A2")->GetObject<ndn::L3Protocol>();
  for (const auto& entry : ndn->getForwarder()->getFib()) {
    bool isFirst = true;
    for (auto& nextHop : entry.getNextHops()) {
      auto face = dynamic_pointer_cast<ndn::NetDeviceFace>(nextHop.getFace());
      if (face == nullptr)
        continue;
      BOOST_CHECK_EQUAL(Names::FindName(face->GetNetDevice()->GetChannel()->GetDevice(1)->GetNode()), "B2");
      isFirst = false;
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
