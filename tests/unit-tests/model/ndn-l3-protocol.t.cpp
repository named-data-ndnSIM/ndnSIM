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

#include "helper/ndn-scenario-helper.hpp"
#include "helper/ndn-app-helper.hpp"

#include <ndn-cxx/face.hpp>

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

BOOST_FIXTURE_TEST_SUITE(ModelNdnL3Protocol, ScenarioHelperWithCleanupFixture)

class TesterApp
{
public:
  TesterApp(const std::function<void(::ndn::Face& face)>& func)
  {
    func(m_face);
  }

protected:
  ::ndn::Face m_face;
};

class ManagerCheckFixture : public ScenarioHelperWithCleanupFixture
{
public:
  void
  setupAndRun()
  {
    createTopology({
        {"1"},
          });

    requestedDatasets = {
      "/localhost/nfd/rib/list",
      "/localhost/nfd/faces/list",
      "/localhost/nfd/strategy-choice/list",
      "/localhost/nfd/status/general"
    };

    FactoryCallbackApp::Install(getNode("1"), [this] () -> shared_ptr<void> {
        return make_shared<TesterApp>([this] (::ndn::Face& face) {
            for (const Name& dataset : requestedDatasets) {
              Interest i(dataset);
              face.expressInterest(i, [&] (const Interest& i, const Data& data) {
                  BOOST_TEST_MESSAGE(data.getName());
                  receivedDatasets.insert(data.getName().getPrefix(-2));
                },
                std::bind([]{}),
                std::bind([]{}));
            }
          });
      })
      .Start(Seconds(0.01));

    Simulator::Stop(Seconds(1.0));
    Simulator::Run();
  }

public:
  std::set<Name> requestedDatasets;
  std::set<Name> receivedDatasets;
};

BOOST_FIXTURE_TEST_SUITE(ManagerCheck, ManagerCheckFixture)

BOOST_AUTO_TEST_CASE(AllEnabled)
{
  setupAndRun();

  BOOST_CHECK_EQUAL(requestedDatasets.size(), receivedDatasets.size());
  BOOST_CHECK_EQUAL_COLLECTIONS(requestedDatasets.begin(), requestedDatasets.end(),
                                receivedDatasets.begin(), receivedDatasets.end());
}

BOOST_AUTO_TEST_CASE(DisabledRibManager)
{
  // Disable RIB manager
  disableRibManager();

  setupAndRun();

  BOOST_CHECK_EQUAL(requestedDatasets.size(), receivedDatasets.size() + 1);

  requestedDatasets.erase("/localhost/nfd/rib/list");
  BOOST_CHECK_EQUAL_COLLECTIONS(requestedDatasets.begin(), requestedDatasets.end(),
                                receivedDatasets.begin(), receivedDatasets.end());
}

// BOOST_AUTO_TEST_CASE(DisabledFaceManager)
// {
//   // Disable Face manager
//   disableFaceManager();

//   setupAndRun();

//   BOOST_CHECK_EQUAL(requestedDatasets.size(), receivedDatasets.size() + 1);

//   requestedDatasets.erase("/localhost/nfd/faces/list");
//   BOOST_CHECK_EQUAL_COLLECTIONS(requestedDatasets.begin(), requestedDatasets.end(),
//                                 receivedDatasets.begin(), receivedDatasets.end());
// }

BOOST_AUTO_TEST_CASE(DisabledStrategyChoiceManager)
{
  // Disable Strategy Choice Manager manager
  disableStrategyChoiceManager();

  setupAndRun();

  BOOST_CHECK_EQUAL(requestedDatasets.size(), receivedDatasets.size() + 1);

  requestedDatasets.erase("/localhost/nfd/strategy-choice/list");
  BOOST_CHECK_EQUAL_COLLECTIONS(requestedDatasets.begin(), requestedDatasets.end(),
                                receivedDatasets.begin(), receivedDatasets.end());
}

BOOST_AUTO_TEST_CASE(DisabledForwarderStatusManager)
{
  // Disable Forwarder Status Manager
  disableForwarderStatusManager();

  setupAndRun();

  BOOST_CHECK_EQUAL(requestedDatasets.size(), receivedDatasets.size() + 1);

  requestedDatasets.erase("/localhost/nfd/status/general");
  BOOST_CHECK_EQUAL_COLLECTIONS(requestedDatasets.begin(), requestedDatasets.end(),
                                receivedDatasets.begin(), receivedDatasets.end());
}

BOOST_AUTO_TEST_SUITE_END() // ManagerCheck

BOOST_AUTO_TEST_SUITE_END() // ModelNdnL3Protocol

} // namespace ndn
} // namespace ns3
