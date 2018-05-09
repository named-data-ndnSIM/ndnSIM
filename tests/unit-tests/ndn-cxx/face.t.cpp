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

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/util/scheduler.hpp>
#include <ndn-cxx/util/scheduler-scoped-event-id.hpp>
#include <ndn-cxx/lp/tags.hpp>

#include "ns3/ndnSIM/helper/ndn-app-helper.hpp"
#include "ns3/error-model.h"

#include "../tests-common.hpp"

namespace ns3 {
namespace ndn {

class NndCxxFaceFixture : public ScenarioHelperWithCleanupFixture
{
public:
  NndCxxFaceFixture()
    : hasFired(false)
  {
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
    Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
    Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

    createTopology({{"A", "B"}});
    addRoutes({{"A", "B", "/test", 1}});
  }

protected:
  bool hasFired;
};

BOOST_FIXTURE_TEST_SUITE(NdnCxxFace, NndCxxFaceFixture)

class BaseTesterApp
{
public:
  typedef std::function<void(const Name&)> NameCallback;
  typedef std::function<void()> VoidCallback;

protected:
  ::ndn::Face m_face;
};

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class BasicProducer : public BaseTesterApp
{
public:
  BasicProducer(const Name& name, const NameCallback& onInterest, const VoidCallback& onFail)
  {
    m_face.setInterestFilter(name,
                             [this, onInterest] (const ::ndn::InterestFilter& filter, const Interest& interest) {
                               auto data = make_shared<Data>(Name(interest.getName()));
                               StackHelper::getKeyChain().sign(*data);
                               m_face.put(*data);
                               onInterest(interest.getName());
                             },
                             std::bind(onFail));
  }
};

BOOST_AUTO_TEST_CASE(SetInterestFilter)
{
  FactoryCallbackApp::Install(getNode("B"), [this] () -> shared_ptr<void> {
      return make_shared<BasicProducer>("/test", [this] (const Name& interest) {
          BOOST_CHECK_EQUAL(interest, "/test/prefix/%FE%00");
          this->hasFired = true;
        },
        [] {
          BOOST_ERROR("Unexpected failure to set interest filter");
        });
    })
    .Start(Seconds(0.01));

  addApps({{"A", "ns3::ndn::ConsumerBatches",
            {{"Prefix", "/test/prefix"}, {"Batches", "0s 1"}}, "1s", "5.1s"}});

  Simulator::Stop(Seconds(20));
  Simulator::Run();

  BOOST_CHECK(hasFired);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class SingleInterest : public BaseTesterApp
{
public:
  SingleInterest(const Name& name, const std::function<void(const Data&)>& onData,
                 const VoidCallback& onNack, const VoidCallback& onTimeout)
  {
    m_face.expressInterest(Interest(name), std::bind([onData] (const Data& data) {
          onData(data);
        }, _2),
      std::bind(onNack),
      std::bind(onTimeout));
  }
};

BOOST_AUTO_TEST_CASE(ExpressInterestLocalhost)
{
  // Retrieve data from remote
  FactoryCallbackApp::Install(getNode("A"), [this] () -> shared_ptr<void> {
      return make_shared<SingleInterest>("/localhost", [this] (const Data& data) {
          BOOST_CHECK(Name("/localhost").isPrefixOf(data.getName()));
          this->hasFired = true;
          BOOST_CHECK_LE(Simulator::Now().ToDouble(Time::S), 1.01);
        },
        [] {
          BOOST_ERROR("Unexpected NACK");
        },
        [] {
          BOOST_ERROR("Unexpected timeout");
        });
    })
    .Start(Seconds(1.01));

  Simulator::Stop(Seconds(20));
  Simulator::Run();

  BOOST_CHECK(hasFired);
}

BOOST_AUTO_TEST_CASE(ExpressInterestRemote)
{
  addApps({{"B", "ns3::ndn::Producer", {{"Prefix", "/test"}}, "0s", "100s"}});

  // Retrieve data from remote
  FactoryCallbackApp::Install(getNode("A"), [this] () -> shared_ptr<void> {
      return make_shared<SingleInterest>("/test/prefix", [this] (const Data& data) {
          BOOST_CHECK_EQUAL(data.getName(), "/test/prefix");
          BOOST_REQUIRE(data.getTag<lp::HopCountTag>() != nullptr);
          BOOST_CHECK_EQUAL(*data.getTag<lp::HopCountTag>(), 2);
          this->hasFired = true;
          BOOST_CHECK_LE(Simulator::Now().ToDouble(Time::S), 2.0);
        },
        [] {
          BOOST_ERROR("Unexpected NACK");
        },
        [] {
          BOOST_ERROR("Unexpected timeout");
        });
    })
    .Start(Seconds(1.01));

  Simulator::Stop(Seconds(20));
  Simulator::Run();

  BOOST_CHECK(hasFired);
}

BOOST_AUTO_TEST_CASE(ExpressInterestTimeout)
{
  FactoryCallbackApp::Install(getNode("A"), [this] () -> shared_ptr<void> {
      return make_shared<SingleInterest>(Name("/test/prefix"), [] (const Data&) {
          BOOST_ERROR("Unexpected data");
        },
        [] {
          BOOST_ERROR("Unexpected NACK");
        },
        [this] {
          BOOST_CHECK_GT(Simulator::Now().ToDouble(Time::S), 6.0);
          this->hasFired = true;
        });
    })
    .Start(Seconds(2.01));

  // Make sure NACKs are never received
  Ptr<ns3::RateErrorModel> model = CreateObject<ns3::RateErrorModel>();
  model->SetRate(std::numeric_limits<double>::max());
  Config::Set("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/ReceiveErrorModel", PointerValue(model));

  Simulator::Stop(Seconds(20));
  Simulator::Run();

  BOOST_CHECK(hasFired);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class MultipleInterest : public BaseTesterApp
{
public:
  MultipleInterest(const Name& name, const NameCallback& onData, const VoidCallback& onTimeout,
                   const VoidCallback& onNack)
    : m_scheduler(m_face.getIoService())
    , m_event(m_scheduler)
  {
    expressNextInterest(name, 0, onData, onTimeout, onNack);
  }

private:
  void
  expressNextInterest(const Name& name, uint32_t seqNo, const NameCallback& onData,
                      const VoidCallback& onTimeout, const VoidCallback& onNack)
  {
    m_face.expressInterest(Interest(Name(name).appendSegment(seqNo)), std::bind([=] (const Data& data) {
          onData(data.getName());

          m_event = m_scheduler.scheduleEvent(time::seconds(1),
                                              std::bind(&MultipleInterest::expressNextInterest, this,
                                                        name, seqNo + 1, onData, onTimeout, onNack));
        }, _2),
      std::bind(onNack),
      std::bind(onTimeout));
  }

private:
  ::ndn::Scheduler m_scheduler;
  ::ndn::util::scheduler::ScopedEventId m_event;
};

BOOST_AUTO_TEST_CASE(ExpressMultipleInterests)
{
  addApps({{"B", "ns3::ndn::Producer", {{"Prefix", "/test"}}, "0s", "100s"}});

  size_t recvCount = 0;

  // Retrieve data from remote
  FactoryCallbackApp::Install(getNode("A"), [&recvCount] () -> shared_ptr<void> {
      return make_shared<MultipleInterest>("/test/prefix", [&recvCount] (const Name& data) {
          BOOST_CHECK_EQUAL(data, Name("/test/prefix").appendSegment(recvCount));
          ++recvCount;
        },
        [] {
          BOOST_ERROR("Unexpected timeout");
        },
        [] {
          BOOST_ERROR("Unexpected NACK");
        });
    })
    .Start(Seconds(1.01));

  Simulator::Stop(Seconds(10.9)); // this test case also checks that apps stops properly
  Simulator::Run();

  BOOST_CHECK_EQUAL(recvCount, 10);
}

class SingleInterestWithFaceShutdown : public BaseTesterApp
{
public:
  SingleInterestWithFaceShutdown()
  {
    m_face.expressInterest(Interest(Name("/interest/to/timeout")),
                           std::bind([] {
                               BOOST_ERROR("Unexpected response");
                             }),
                           std::bind([this] {
                               m_face.shutdown();
                             }),
                           std::bind([this] {
                               m_face.shutdown();
                             }));
  }
};

BOOST_AUTO_TEST_CASE(FaceShutdownFromTimeoutCallback)
{
  // This test case to check if Face.shutdown from an onTimeout callback doesn't cause segfaults
  FactoryCallbackApp::Install(getNode("A"), [] () -> shared_ptr<void> {
      return make_shared<SingleInterestWithFaceShutdown>();
    })
    .Start(Seconds(1.01));

  Simulator::Stop(Seconds(20));
  Simulator::Run();
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
