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

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_AUTO_TEST_SUITE(HelperNdnFibHelper)

class AddRouteFixture : public ScenarioHelperWithCleanupFixture
{
public:
  AddRouteFixture()
  {
    createTopology({
        {"1", "2"}
      });

    addApps({
        {"1", "ns3::ndn::ConsumerCbr",
            {{"Prefix", "/prefix"}, {"Frequency", "1"}},
            "0s", "9.99s"},
        {"2", "ns3::ndn::Producer",
            {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
            "0s", "100s"}
      });
  }

  ~AddRouteFixture()
  {
    Simulator::Stop(Seconds(20.001));
    Simulator::Run();

    BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNOutInterests(), 10);
    BOOST_CHECK_EQUAL(getFace("1", "2")->getFaceStatus().getNInDatas(), 10);

    BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNInInterests(), 10);
    BOOST_CHECK_EQUAL(getFace("2", "1")->getFaceStatus().getNOutDatas(), 10);
  }
};

BOOST_FIXTURE_TEST_SUITE(AddRoute, AddRouteFixture)

// static void
// AddRoute(Ptr<Node> node, const Name& prefix, shared_ptr<Face> face, int32_t metric);
BOOST_AUTO_TEST_CASE(Base)
{
  FibHelper::AddRoute(getNode("1"), Name("/prefix"), getFace("1", "2"), 1);
}


// static void
// AddRoute(const std::string& nodeName, const Name& prefix, uint32_t faceId, int32_t metric);
BOOST_AUTO_TEST_CASE(Helper1)
{
  FibHelper::AddRoute("1", Name("/prefix"), getFace("1", "2")->getId(), 10);
}

// static void
// AddRoute(Ptr<Node> node, const Name& prefix, uint32_t faceId, int32_t metric);
BOOST_AUTO_TEST_CASE(Helper2)
{
  FibHelper::AddRoute(getNode("1"), Name("/prefix"), getFace("1", "2")->getId(), 10);
}

// static void
// AddRoute(const std::string& nodeName, const Name& prefix, const std::string& otherNodeName,
//          int32_t metric);
BOOST_AUTO_TEST_CASE(Helper3)
{
  FibHelper::AddRoute("1", "/prefix", "2", 1);
}

// static void
// AddRoute(Ptr<Node> node, const Name& prefix, Ptr<Node> otherNode, int32_t metric);
BOOST_AUTO_TEST_CASE(Helper4)
{
  FibHelper::AddRoute(getNode("1"), Name("/prefix"), getNode("2"), 10);
}

BOOST_AUTO_TEST_SUITE_END() // AddRoute

BOOST_AUTO_TEST_SUITE_END() // HelperNdnFibHelper

} // namespace ndn
} // namespace ns3
