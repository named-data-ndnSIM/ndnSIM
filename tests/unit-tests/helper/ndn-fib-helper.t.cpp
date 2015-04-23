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

#include "helper/ndn-fib-helper.hpp"
#include "model/ndn-net-device-face.hpp"

#include "ns3/node-container.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/node.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperNdnFibHelper, CleanupFixture)

BOOST_AUTO_TEST_CASE(AddRoute)
{
 NodeContainer nodes;
 nodes.Create(2);

 PointToPointHelper p2p;
 p2p.Install(nodes.Get(0), nodes.Get(1));

 StackHelper ndnHelper;
 Ptr<FaceContainer> node0_faceContainer = ndnHelper.InstallAll();

 ndn::FibHelper::AddRoute(nodes.Get(0), "/prefix/1", nodes.Get(1), 1);
 // Installing applications
 ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
 consumerHelper.SetAttribute("Frequency", StringValue("10")); // 10 interests a second

 ndn::AppHelper producerHelper("ns3::ndn::Producer");
 producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

 consumerHelper.SetPrefix("/prefix/1");
 consumerHelper.Install(nodes.Get(0));

 producerHelper.SetPrefix("/prefix/1");
 producerHelper.Install(nodes.Get(1));

 FaceContainer::Iterator node1Face_iterator = node0_faceContainer->Begin() + 1;

 auto node1_netDeviceFace = std::dynamic_pointer_cast<NetDeviceFace>(node0_faceContainer->Get(node1Face_iterator));

 Simulator::Stop(Seconds(20.0));
 Simulator::Run();

 ::ndn::nfd::FaceStatus faceStatus = node1_netDeviceFace->getFaceStatus();
 BOOST_CHECK_EQUAL(faceStatus.getNInInterests(), 200);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
