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

#include "helper/ndn-stack-helper.hpp"
#include "../tests-common.hpp"

#include "ns3/point-to-point-module.h"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperStackHelper, CleanupFixture)

BOOST_AUTO_TEST_CASE(TestNfdContentStorePolicy)
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(2);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.setPolicy("nfd::cs::lru");
  ndnHelper.Install(nodes.Get(0));

  // test which CS policy has be selected for node 0
  Ptr<L3Protocol> protoNode0 = L3Protocol::getL3Protocol(nodes.Get(0));
  BOOST_CHECK_EQUAL(protoNode0->getForwarder()->getCs().getPolicy()->getName(), "lru");

  ndnHelper.setPolicy("nfd::cs::priority_fifo");
  ndnHelper.Install(nodes.Get(1));

  Ptr<L3Protocol> protoNode1 = L3Protocol::getL3Protocol(nodes.Get(1));
  // test that the CS policy for node 0 did not change
  BOOST_CHECK_EQUAL(protoNode0->getForwarder()->getCs().getPolicy()->getName(), "lru");
  // test which CS policy has be selected for node 1
  BOOST_CHECK_EQUAL(protoNode1->getForwarder()->getCs().getPolicy()->getName(), "priority_fifo");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
