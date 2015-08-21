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

#include "helper/ndn-link-control-helper.hpp"
#include "NFD/core/scheduler.hpp"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(HelperNdnLinkControlHelper, ScenarioHelperWithCleanupFixture)

BOOST_AUTO_TEST_CASE(TwoNodeTopology)
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  createTopology({
      {"1", "2"},
    });

  addRoutes({
      {"1", "2", "/prefix", 1},
    });

  addApps({
      {"1", "ns3::ndn::ConsumerCbr",
          {{"Prefix", "/prefix"}, {"Frequency", "1"}},
          "0s", "100s"},
      {"2", "ns3::ndn::Producer",
          {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
          "0s", "100s"}
    });

  Simulator::Schedule(Seconds(5.1), ndn::LinkControlHelper::FailLink, getNode("1"), getNode("2"));
  Simulator::Schedule(Seconds(10.1), ndn::LinkControlHelper::UpLink, getNode("1"), getNode("2"));

  nfd::scheduler::schedule(time::milliseconds(5200), [&] {
      BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 6);
      BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNInDatas(), 6);
    });

  nfd::scheduler::schedule(time::milliseconds(10200), [&] {
      BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 6);
      BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNInDatas(), 6);
    });
  nfd::scheduler::schedule(time::milliseconds(15100), [&] {
      BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 11);
      BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNInDatas(), 11);
    });

  Simulator::Stop(Seconds(15.2));
  Simulator::Run();
}

BOOST_AUTO_TEST_CASE(SixNodeTopology) // Bug #2783
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Connecting nodes in 6 node topology:                        //
  //                                                             //
  //                            +---+                            //
  //                         +- | 2 | -+                         //
  //                        /   +---+   \                        //
  //              +---+    /             \    +---+              //
  // +---+        |   | --+               +-- |   |        +---+ //
  // | 0 | ------ | 1 |                       | 4 | ------ | 5 | //
  // +---+        |   | --+               +-- |   |        +---+ //
  //              +---+    \             /    +---+              //
  //                        \   +---+   /                        //
  //                         +- | 3 | -+                         //
  //                            +---+                            //

  createTopology({
      {"0", "1"},
      {"1", "2"}, {"1", "3"},
      {"2", "4"}, {"3", "4"},
      {"4", "5"},
    });

  addRoutes({
      {"0", "1", "/prefix", 1},
      {"1", "2", "/prefix", 1}, {"1", "3", "/prefix", 1},
      {"2", "4", "/prefix", 1}, {"3", "4", "/prefix", 1},
      {"4", "5", "/prefix", 1},
    });

  // Choosing forwarding strategy
  ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/multicast");

  addApps({
      {"0", "ns3::ndn::ConsumerCbr",
          {{"Prefix", "/prefix"}, {"Frequency", "1"}},
          "0s", "100s"},
      {"5", "ns3::ndn::Producer",
          {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
          "0s", "100s"}
    });

  nfd::scheduler::schedule(time::milliseconds(10100), [&] {
      LinkControlHelper::FailLink(getNode("1"), getNode("2"));
    });

  // just before link failure
  nfd::scheduler::schedule(time::milliseconds(10050), [&] {
      BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 11);
      BOOST_CHECK_EQUAL(getFace("3", "1")->getFaceStatus().getNInInterests(), 11);
    });

  // just before link recovery
  nfd::scheduler::schedule(time::milliseconds(20050), [&] {
      BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 11);
      BOOST_CHECK_EQUAL(getFace("3", "1")->getFaceStatus().getNInInterests(), 21);
    });

  nfd::scheduler::schedule(time::milliseconds(20100), [&] {
      LinkControlHelper::UpLink(getNode("1"), getNode("2"));
    });

  nfd::scheduler::schedule(time::milliseconds(30050), [&] {
      BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 21);
      BOOST_CHECK_EQUAL(getFace("3", "1")->getFaceStatus().getNInInterests(), 31);
    });

  Simulator::Stop(Seconds(30.1));
  Simulator::Run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
