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

#include "helper/ndn-network-region-table-helper.hpp"
#include "helper/ndn-app-helper.hpp"
#include <ndn-cxx/link.hpp>

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

using ::ndn::Delegation;
using ::ndn::DelegationList;

BOOST_AUTO_TEST_SUITE(HelperNdnNetworkRegionTableHelper)

class BasicFixture : public ScenarioHelperWithCleanupFixture
{
public:
  BasicFixture()
  {
    createTopology({
     {"1"}
    });
  }

  ~BasicFixture()
  {
  }
};

BOOST_FIXTURE_TEST_SUITE(Basic, BasicFixture)

BOOST_AUTO_TEST_CASE(AddBase)
{
  NetworkRegionTableHelper::AddRegionName(getNode("1"), Name("/ucla"));
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().count("/ucla"), 1);
}

BOOST_AUTO_TEST_CASE(RemoveBase){
  NetworkRegionTableHelper::AddRegionName(getNode("1"), Name("/ucla"));
  NetworkRegionTableHelper::RemoveRegionName(getNode("1"), Name("/ucla"));
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().count("/ucla"), 0);
}

BOOST_AUTO_TEST_CASE(AddSet)
{
  NetworkRegionTableHelper::AddRegionName(getNode("1"), { Name("/ucla"), Name("/att") });
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().count("/ucla"), 1);
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().count("/att"), 1);
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().size(), 2);
}

BOOST_AUTO_TEST_CASE(RemoveSet)
{
  NetworkRegionTableHelper::AddRegionName(getNode("1"), { "/ucla", "/att" });
  NetworkRegionTableHelper::RemoveRegionName(getNode("1"), { "/att", "/ucla" });
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().empty(), true);
}

BOOST_AUTO_TEST_CASE(Empty)
{
  NetworkRegionTableHelper::AddRegionName(getNode("1"), { Name("/ucla"), Name("/att") });
  NetworkRegionTableHelper::AddRegionName(getNode("1"), Name("/ndnSIM"));
  NetworkRegionTableHelper::EmptyNetworkRegionTable(getNode("1"));
  BOOST_CHECK_EQUAL(getNode("1")->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().empty(), true);
}

BOOST_AUTO_TEST_SUITE_END() // Basic

class MultiNodeWithAppFixture;
class TesterApp
{
public:
  TesterApp(const Interest& interest, MultiNodeWithAppFixture* fixture);

protected:
  ::ndn::Face m_face;
};

DelegationList
makeHint(const Name& delegation)
{
  Delegation del;
  del.name = Name(delegation);
  del.preference = 1;
  DelegationList list({del});
  return list;
}

class MultiNodeWithAppFixture : public ScenarioHelperWithCleanupFixture
{
public:
  MultiNodeWithAppFixture()
    : m_nData(0)
    , m_nTimeouts(0)
    , m_nNacks(0)
  {
    createTopology({
        {"1", "2"}
      });

    addApps({
        {"2", "ns3::ndn::Producer",
            {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
            "0s", "100s"}
      });

    addRoutes({
        {"1", "2", "/otherPrefix", 1},
      });
  }

public:
  size_t m_nData;
  size_t m_nTimeouts;
  size_t m_nNacks;
};

TesterApp::TesterApp(const Interest& interest, MultiNodeWithAppFixture* fixture)
{
  m_face.expressInterest(interest,
                         std::bind([fixture] {
                             ++fixture->m_nData;
                           }),
                         std::bind([fixture] {
                             ++fixture->m_nNacks;
                           }),
                         std::bind([fixture] {
                             ++fixture->m_nTimeouts;
                           }));
}


BOOST_FIXTURE_TEST_SUITE(MultiNode, MultiNodeWithAppFixture)

BOOST_AUTO_TEST_CASE(WithoutNetworkRegion)
{
  FactoryCallbackApp::Install(getNode("1"), [this] () -> shared_ptr<void> {
      Interest i("/prefix/someData");
      i.setForwardingHint(makeHint(Name("/otherPrefix")));
      return make_shared<TesterApp>(i, this);
    })
    .Start(Seconds(0.01));

  Simulator::Stop(Seconds(20.001));
  Simulator::Run();

  BOOST_CHECK_EQUAL(this->m_nData, 0);
  BOOST_CHECK_EQUAL(m_nTimeouts, 0);
  BOOST_CHECK_EQUAL(m_nNacks, 1);
}

BOOST_AUTO_TEST_CASE(WithNetworkRegion)
{
  NetworkRegionTableHelper::AddRegionName(getNode("2"), Name("/otherPrefix"));

  FactoryCallbackApp::Install(getNode("1"), [this] () -> shared_ptr<void> {
      Interest i("/prefix/someData");
      i.setForwardingHint(makeHint(Name("/otherPrefix")));
      return make_shared<TesterApp>(i, this);
    })
    .Start(Seconds(0.01));

  Simulator::Stop(Seconds(20.001));
  Simulator::Run();

  BOOST_CHECK_EQUAL(m_nData, 1);
  BOOST_CHECK_EQUAL(m_nTimeouts, 0);
  BOOST_CHECK_EQUAL(m_nNacks, 0);
}

BOOST_AUTO_TEST_CASE(WithMoreSpecificNetworkRegion)
{
  NetworkRegionTableHelper::AddRegionName(getNode("2"), Name("/otherPrefix/moreSpecific"));

  FactoryCallbackApp::Install(getNode("1"), [this] () -> shared_ptr<void> {
      Interest i("/prefix/someData");
      i.setForwardingHint(makeHint(Name("/otherPrefix")));
      return make_shared<TesterApp>(i, this);
    })
    .Start(Seconds(0.01));

  Simulator::Stop(Seconds(20.001));
  Simulator::Run();

  BOOST_CHECK_EQUAL(m_nData, 1);
  BOOST_CHECK_EQUAL(m_nTimeouts, 0);
  BOOST_CHECK_EQUAL(m_nNacks, 0);
}

BOOST_AUTO_TEST_CASE(WithLessSpecificLink)
{
  NetworkRegionTableHelper::AddRegionName(getNode("2"), Name("/otherPrefix"));

  FactoryCallbackApp::Install(getNode("1"), [this] () -> shared_ptr<void> {
      Interest i("/prefix/someData");
      i.setForwardingHint(makeHint(Name("/otherPrefix/moreSpecific")));
      return make_shared<TesterApp>(i, this);
    })
    .Start(Seconds(0.01));

  Simulator::Stop(Seconds(20.001));
  Simulator::Run();

  BOOST_CHECK_EQUAL(m_nData, 0);
  BOOST_CHECK_EQUAL(m_nTimeouts, 0);
  BOOST_CHECK_EQUAL(m_nNacks, 1);
}

BOOST_AUTO_TEST_SUITE_END() // MultiNode

BOOST_AUTO_TEST_SUITE_END() // HelperNdnNetworkRegionTableHelper

} // namespace ndn
} // namespace ns3
