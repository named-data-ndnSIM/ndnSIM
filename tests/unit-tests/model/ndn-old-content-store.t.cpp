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


#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnOldContentStore, ScenarioHelperWithCleanupFixture)

BOOST_AUTO_TEST_CASE(RandomPolicy)
{
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

  getStackHelper().SetOldContentStore("ns3::ndn::cs::Random", "MaxSize", "10");

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

  std::map<std::string, std::vector<Name>> entries;
  for (const std::string& node : {"1", "2"}) {
    auto cs = getNode(node)->GetObject<ContentStore>();
    auto& nodeCs = entries[node];
    for (auto it = cs->Begin(); it != cs->End(); it = cs->Next(it)) {
      nodeCs.push_back(it->GetName());
    }
  }

  BOOST_CHECK_EQUAL(entries["1"].size(), 10);
  BOOST_CHECK_EQUAL(entries["2"].size(), 10);
  BOOST_CHECK(entries["1"] != entries["2"]); // this test has a small chance of failing
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
