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

#include "helper/ndn-strategy-choice-helper.hpp"

#include "ns3/ndnSIM/NFD/daemon/fw/strategy.hpp"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

class NccFixture : public ScenarioHelperWithCleanupFixture
{
public:
  NccFixture()
  {
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
    Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("1ms"));
    Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(500));

      //            Creating a 3 node topology                //
      //                                                      //
      //                                                      //
      //          +----+       +----+         +----+          //
      //          |    |       |    |         |    |          //
      //          | A1 | <---> | A2 |  <--->  | A3 |          //
      //          |    |       |    |         |    |          //
      //          +----+       +----+         +----+          //
      //                                                      //
      //                                                      //
      //                                                      //

      createTopology({
          {"A1", "A2"},
          {"A2", "A3"}
        });

      addRoutes({
          {"A1", "A2", "/prefix", 100},
          {"A2", "A3", "/prefix", 100},
        });

      addApps({
          {"A1", "ns3::ndn::ConsumerCbr",
              {{"Prefix", "/prefix"}, {"Frequency", "1000"}},
              "0s", "5s"},
          {"A3", "ns3::ndn::Producer",
              {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
              "0s", "10s"},
        });
    }
};

BOOST_FIXTURE_TEST_SUITE(TestNccStrategy, NccFixture)

BOOST_AUTO_TEST_CASE(DetachedPitEntries)
{
  StrategyChoiceHelper::Install(getNode("A1"), "/prefix", "/localhost/nfd/strategy/ncc");
  StrategyChoiceHelper::Install(getNode("A2"), "/prefix", "/localhost/nfd/strategy/ncc");
  StrategyChoiceHelper::Install(getNode("A3"), "/prefix", "/localhost/nfd/strategy/ncc");

  Simulator::Stop(Seconds(5.2));
  BOOST_CHECK_NO_THROW(Simulator::Run());

  BOOST_CHECK_EQUAL(getFace("A1", "A2")->getCounters().nOutInterests, 5000);
  BOOST_CHECK_EQUAL(getFace("A2", "A3")->getCounters().nOutInterests, 5000);

  BOOST_CHECK_EQUAL(getFace("A3", "A2")->getCounters().nOutData, 5000);
  BOOST_CHECK_EQUAL(getFace("A2", "A1")->getCounters().nOutData, 5000);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
