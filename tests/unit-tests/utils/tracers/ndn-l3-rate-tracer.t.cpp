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

#include "utils/tracers/ndn-l3-rate-tracer.hpp"

#include <boost/filesystem.hpp>
#include <boost/test/output_test_stream.hpp>

#include "../../tests-common.hpp"

namespace ns3 {
namespace ndn {

const boost::filesystem::path TEST_TRACE = boost::filesystem::path(TEST_CONFIG_PATH) / "trace.txt";

class L3RateTracerFixture : public ScenarioHelperWithCleanupFixture
{
public:
  L3RateTracerFixture()
  {
    boost::filesystem::create_directories(TEST_CONFIG_PATH);

    // setting default parameters for PointToPoint links and channels
    Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10Mbps"));
    Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
    Config::SetDefault("ns3::QueueBase::MaxPackets", UintegerValue(20));

    createTopology({
        {"1"},
      });

    addApps({
        {"1", "ns3::ndn::ConsumerCbr",
            {{"Prefix", "/prefix"}, {"Frequency", "1"}},
            "0s", "0.9s"} // send just one packet
      });
  }

  ~L3RateTracerFixture()
  {
    boost::filesystem::remove(TEST_TRACE);
    L3RateTracer::Destroy(); // additional cleanup
  }
};

BOOST_FIXTURE_TEST_SUITE(UtilsTracersNdnL3RateTracer, L3RateTracerFixture)

BOOST_AUTO_TEST_CASE(NackTracing)
{
  NodeContainer nodes;
  nodes.Add(getNode("1"));

  L3RateTracer::Install(nodes, TEST_TRACE.string(), Seconds(1));

  Simulator::Stop(Seconds(1.5));
  Simulator::Run();

  L3RateTracer::Destroy(); // to force log to be written

  boost::test_tools::output_test_stream os(TEST_TRACE.string().c_str(), true);

  os << "Time	Node	FaceId	FaceDescr	Type	Packets	Kilobytes	PacketRaw	KilobytesRaw\n";
  BOOST_CHECK(os.match_pattern());

  os << "1	1	1	internal://	InInterests	0	0	0	0\n"
     << "1	1	1	internal://	OutInterests	0	0	0	0\n"
     << "1	1	1	internal://	InData	0	0	0	0\n"
     << "1	1	1	internal://	OutData	0	0	0	0\n"
     << "1	1	1	internal://	InNacks	0	0	0	0\n"
     << "1	1	1	internal://	OutNacks	0	0	0	0\n"
     << "1	1	1	internal://	InSatisfiedInterests	0	0	0	0\n"
     << "1	1	1	internal://	InTimedOutInterests	0	0	0	0\n"
     << "1	1	1	internal://	OutSatisfiedInterests	2.4	0	3	0\n"
     << "1	1	1	internal://	OutTimedOutInterests	0	0	0	0\n";
  BOOST_CHECK(os.match_pattern());

  os << "1	1	256	internal://	InInterests	0	0	0	0\n"
     << "1	1	256	internal://	OutInterests	0	0	0	0\n"
     << "1	1	256	internal://	InData	0	0	0	0\n"
     << "1	1	256	internal://	OutData	0	0	0	0\n"
     << "1	1	256	internal://	InNacks	0	0	0	0\n"
     << "1	1	256	internal://	OutNacks	0	0	0	0\n"
     << "1	1	256	internal://	InSatisfiedInterests	2.4	0	3	0\n"
     << "1	1	256	internal://	InTimedOutInterests	0	0	0	0\n"
     << "1	1	256	internal://	OutSatisfiedInterests	0	0	0	0\n"
     << "1	1	256	internal://	OutTimedOutInterests	0	0	0	0\n";
  BOOST_CHECK(os.match_pattern());

  os << "1	1	257	appFace://	InInterests	0.8	0	1	0\n"
     << "1	1	257	appFace://	OutInterests	0	0	0	0\n"
     << "1	1	257	appFace://	InData	0	0	0	0\n"
     << "1	1	257	appFace://	OutData	0	0	0	0\n"
     << "1	1	257	appFace://	InNacks	0	0	0	0\n"
     << "1	1	257	appFace://	OutNacks	0.8	0	1	0\n"
     << "1	1	257	appFace://	InSatisfiedInterests	0	0	0	0\n"
     << "1	1	257	appFace://	InTimedOutInterests	0	0	0	0\n"
     << "1	1	257	appFace://	OutSatisfiedInterests	0	0	0	0\n"
     << "1	1	257	appFace://	OutTimedOutInterests	0	0	0	0\n";
  BOOST_CHECK(os.match_pattern());

  os << "1	1	-1	all	SatisfiedInterests	2.4	0	3	0\n"
     << "1	1	-1	all	TimedOutInterests	0.8	0	1	0\n";
  BOOST_CHECK(os.match_pattern());
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace ndn
} // namespace ns3
