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

#include "utils/tracers/ndn-app-delay-tracer.hpp"

#include <boost/filesystem.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../../tests-common.hpp"

namespace ns3 {
namespace ndn {

const boost::filesystem::path TEST_TRACE = boost::filesystem::path(TEST_CONFIG_PATH) / "trace.txt";

class AppDelayTracerFixture : public ScenarioHelperWithCleanupFixture
{
public:
  AppDelayTracerFixture()
  {
    boost::filesystem::create_directories(TEST_CONFIG_PATH);

    // setting default parameters for PointToPoint links and channels
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
    Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
    Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

    createTopology({
        {"1", "2"},
        {"2", "3"}
      });

    addRoutes({
        {"1", "2", "/prefix", 1},
        {"2", "3", "/prefix", 1}
      });

    addApps({
        {"1", "ns3::ndn::ConsumerCbr",
            {{"Prefix", "/prefix"}, {"Frequency", "1"}},
            "0s", "0.9s"}, // send just one packet
        // Second consumer will capture effect of the bug #2764
        {"2", "ns3::ndn::ConsumerCbr",
            {{"Prefix", "/prefix"}, {"Frequency", "1"}},
            "2s", "100s"},
        {"3", "ns3::ndn::Producer",
            {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
            "0s", "100s"}
      });
  }

  ~AppDelayTracerFixture()
  {
    boost::filesystem::remove(TEST_TRACE);
    AppDelayTracer::Destroy(); // additional cleanup
  }
};

BOOST_FIXTURE_TEST_SUITE(UtilsTracersNdnAppDelayTracer, AppDelayTracerFixture)

BOOST_AUTO_TEST_CASE(InstallAll)
{
  AppDelayTracer::InstallAll(TEST_TRACE.string());

  Simulator::Stop(Seconds(4));
  Simulator::Run();

  AppDelayTracer::Destroy(); // to force log to be written

  std::ifstream t(TEST_TRACE.string().c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();

  BOOST_CHECK_EQUAL(buffer.str(),
                    R"STR(Time	Node	AppId	SeqNo	Type	DelayS	DelayUS	RetxCount	HopCount
0.0417888	1	0	0	LastDelay	0.0417888	41788.8	1	2
0.0417888	1	0	0	FullDelay	0.0417888	41788.8	1	2
2	2	0	0	LastDelay	0	0	1	0
2	2	0	0	FullDelay	0	0	1	0
3.02089	2	0	1	LastDelay	0.0208944	20894.4	1	1
3.02089	2	0	1	FullDelay	0.0208944	20894.4	1	1
)STR");
}

BOOST_AUTO_TEST_CASE(InstallNodeContainer)
{
  NodeContainer nodes;
  nodes.Add(getNode("1"));

  AppDelayTracer::Install(nodes, TEST_TRACE.string());

  Simulator::Stop(Seconds(4));
  Simulator::Run();

  AppDelayTracer::Destroy(); // to force log to be written

  std::ifstream t(TEST_TRACE.string().c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();

  BOOST_CHECK_EQUAL(buffer.str(),
    R"STR(Time	Node	AppId	SeqNo	Type	DelayS	DelayUS	RetxCount	HopCount
0.0417888	1	0	0	LastDelay	0.0417888	41788.8	1	2
0.0417888	1	0	0	FullDelay	0.0417888	41788.8	1	2
)STR");
}

BOOST_AUTO_TEST_CASE(InstallNode)
{
  AppDelayTracer::Install(getNode("2"), TEST_TRACE.string());

  Simulator::Stop(Seconds(4));
  Simulator::Run();

  AppDelayTracer::Destroy(); // to force log to be written

  std::ifstream t(TEST_TRACE.string().c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();

  BOOST_CHECK_EQUAL(buffer.str(),
    R"STR(Time	Node	AppId	SeqNo	Type	DelayS	DelayUS	RetxCount	HopCount
2	2	0	0	LastDelay	0	0	1	0
2	2	0	0	FullDelay	0	0	1	0
3.02089	2	0	1	LastDelay	0.0208944	20894.4	1	1
3.02089	2	0	1	FullDelay	0.0208944	20894.4	1	1
)STR");
}

BOOST_AUTO_TEST_CASE(InstallNodeDumpStream)
{
  auto output = make_shared<boost::test_tools::output_test_stream>();
  Ptr<AppDelayTracer> tracer = AppDelayTracer::Install(getNode("2"), output);

  Simulator::Stop(Seconds(4));
  Simulator::Run();

  tracer = nullptr; // destroy tracer

  BOOST_CHECK(output->is_equal(
    R"STR(2	2	0	0	LastDelay	0	0	1	0
2	2	0	0	FullDelay	0	0	1	0
3.02089	2	0	1	LastDelay	0.0208944	20894.4	1	1
3.02089	2	0	1	FullDelay	0.0208944	20894.4	1	1
)STR"));
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
