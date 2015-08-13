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


#include "model/ndn-net-device-face.hpp"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnNetDeviceFace, ScenarioHelperWithCleanupFixture)

BOOST_AUTO_TEST_CASE(Basic)
{
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
          {{"Prefix", "/prefix"}, {"Frequency", "10"}},
          "0s", "9.99s"},
      {"2", "ns3::ndn::Producer",
          {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
          "0s", "100s"}
    });

  Simulator::Stop(Seconds(20.001));
  Simulator::Run();

  BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNOutInterests(), 100);
  BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNInDatas(), 100);

  BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 100);
  BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNOutDatas(), 100);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
