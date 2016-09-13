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


#include "model/ndn-net-device-link-service.hpp"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnNetDeviceFace, ScenarioHelperWithCleanupFixture)

class FixtureWithTracers : public ScenarioHelperWithCleanupFixture
{
public:
  void
  InInterests(const Interest&, const Face& face)
  {
    nInInterests[boost::lexical_cast<std::string>(face)] += 1;
  }

  void
  OutInterests(const Interest&, const Face& face)
  {
    nOutInterests[boost::lexical_cast<std::string>(face)] += 1;
  }

  void
  InData(const Data&, const Face& face)
  {
    nInData[boost::lexical_cast<std::string>(face)] += 1;
  }

  void
  OutData(const Data&, const Face& face)
  {
    nOutData[boost::lexical_cast<std::string>(face)] += 1;
  }

public:
  std::map<std::string, uint32_t> nInInterests;
  std::map<std::string, uint32_t> nOutInterests;
  std::map<std::string, uint32_t> nInData;
  std::map<std::string, uint32_t> nOutData;

  // TODO add NACKs
};

BOOST_FIXTURE_TEST_CASE(Basic, FixtureWithTracers)
{
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  createTopology({
      {"1", "2"},
    }, false);

  getNetDevice("1", "2")->SetAttribute("Address", StringValue("00:00:00:ff:ff:01"));
  getNetDevice("2", "1")->SetAttribute("Address", StringValue("00:00:00:ff:ff:02"));

  getStackHelper().InstallAll();

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

  Config::ConnectWithoutContext("/NodeList/*/$ns3::ndn::L3Protocol/InInterests", MakeCallback(&FixtureWithTracers::InInterests, this));
  Config::ConnectWithoutContext("/NodeList/*/$ns3::ndn::L3Protocol/OutInterests", MakeCallback(&FixtureWithTracers::OutInterests, this));

  Config::ConnectWithoutContext("/NodeList/*/$ns3::ndn::L3Protocol/InData", MakeCallback(&FixtureWithTracers::InData, this));
  Config::ConnectWithoutContext("/NodeList/*/$ns3::ndn::L3Protocol/OutData", MakeCallback(&FixtureWithTracers::OutData, this));

  // TODO: implement Nack testing
  // Config::Connect("/NodeList/*/InNacks", ...);
  // Config::Connect("/NodeList/*/OutNacks", ...);

  Simulator::Stop(Seconds(20.001));
  Simulator::Run();

  BOOST_CHECK_EQUAL(getFace("1", "2")->getCounters().nInInterests, 0);
  BOOST_CHECK_EQUAL(getFace("1", "2")->getCounters().nOutInterests, 100);
  BOOST_CHECK_EQUAL(getFace("1", "2")->getCounters().nInData, 100);
  BOOST_CHECK_EQUAL(getFace("1", "2")->getCounters().nOutData, 0);
  BOOST_CHECK_EQUAL(getFace("1", "2")->getCounters().nInNacks, 0);
  BOOST_CHECK_EQUAL(getFace("1", "2")->getCounters().nOutNacks, 0);

  BOOST_CHECK_EQUAL(nInInterests [boost::lexical_cast<std::string>(*getFace("1", "2"))], 0);
  BOOST_CHECK_EQUAL(nOutInterests[boost::lexical_cast<std::string>(*getFace("1", "2"))], 100);
  BOOST_CHECK_EQUAL(nInData      [boost::lexical_cast<std::string>(*getFace("1", "2"))], 100);
  BOOST_CHECK_EQUAL(nOutData     [boost::lexical_cast<std::string>(*getFace("1", "2"))], 0);
  // TODO add nacks

  BOOST_CHECK_EQUAL(getFace("2", "1")->getCounters().nInInterests, 100);
  BOOST_CHECK_EQUAL(getFace("2", "1")->getCounters().nOutInterests, 0);
  BOOST_CHECK_EQUAL(getFace("2", "1")->getCounters().nInData, 0);
  BOOST_CHECK_EQUAL(getFace("2", "1")->getCounters().nOutData, 100);
  BOOST_CHECK_EQUAL(getFace("2", "1")->getCounters().nInNacks, 0);
  BOOST_CHECK_EQUAL(getFace("2", "1")->getCounters().nOutNacks, 0);

  BOOST_CHECK_EQUAL(nInInterests [boost::lexical_cast<std::string>(*getFace("2", "1"))], 100);
  BOOST_CHECK_EQUAL(nOutInterests[boost::lexical_cast<std::string>(*getFace("2", "1"))], 0);
  BOOST_CHECK_EQUAL(nInData      [boost::lexical_cast<std::string>(*getFace("2", "1"))], 0);
  BOOST_CHECK_EQUAL(nOutData     [boost::lexical_cast<std::string>(*getFace("2", "1"))], 100);
  // TODO add nacks

  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(*getFace("1", "2")), "netdev://[00:00:00:ff:ff:01]");
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(*getFace("2", "1")), "netdev://[00:00:00:ff:ff:02]");

  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(getFace("1", "2")->getLocalUri()),  "netdev://[00:00:00:ff:ff:01]");
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(getFace("1", "2")->getRemoteUri()), "netdev://[00:00:00:ff:ff:02]");
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(getFace("2", "1")->getLocalUri()),  "netdev://[00:00:00:ff:ff:02]");
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(getFace("2", "1")->getRemoteUri()), "netdev://[00:00:00:ff:ff:01]");
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
