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

// ndn-test.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include <sys/time.h>
#include "ns3/ndnSIM/utils/mem-usage.hpp"
#include "ns3/ndnSIM/model/cs/ndn-content-store.hpp"
#include "ns3/ndnSIM/utils/mem-usage.hpp"

namespace ns3 {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     10000Mbps   +----------+
 *      | consumer |  <------------> | producer |
 *      +----------+       10ms      +----------+
 *
 *
 *     NS_LOG=ndn.Consumer ./waf --run ndn-test
 */

class Tester {
public:
  Tester()
    : m_csSize(100)
    , m_interestRate(1000)
    , m_shouldEvaluatePit(false)
    , m_simulationTime(Seconds(2000) / m_interestRate)
  {
  }

  int
  run(int argc, char* argv[]);

  void
  printHeader(std::ostream& os);

  void
  printStats(std::ostream& os, Time nextPrintTime, double beginRealTime);

private:
  std::string m_oldContentStore;
  size_t m_csSize;
  double m_interestRate;
  bool m_shouldEvaluatePit;
  std::string m_strategy;
  double m_initialOverhead;
  Time m_simulationTime;
};

void
Tester::printHeader(std::ostream& os)
{
  m_initialOverhead = MemUsage::Get() / 1024.0 / 1024.0;
  os << "SimulationTime"
     << "\t"
     << "RealTime"
     << "\t"
     << "NumberOfInterests (total)"
     << "\t"
     << "NumberOfInterests (per real time)"
     << "\n";
}

void
Tester::printStats(std::ostream& os, Time nextPrintTime, double beginRealTime)
{
  ::timeval t;
  gettimeofday(&t, NULL);
  double realTime = t.tv_sec + (0.000001 * (unsigned)t.tv_usec) - beginRealTime;
  Time simTime = Simulator::Now();

  os << simTime << "\t";
  os << realTime << "\t";

  double interestCount = m_interestRate * simTime.ToDouble(Time::S);
  double nInterestsPerSec = interestCount / realTime;

  os << interestCount << "\t" << nInterestsPerSec << "\t";

  uint64_t pitCount = 0;
  uint64_t csCount = 0;
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {

    auto pitSize = (*node)->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size();
    if (pitSize != 0)
      pitCount += pitSize;

    if (true != true) {
      Ptr<ndn::ContentStore> cs = (*node)->GetObject<ndn::ContentStore>();
      if (cs != 0)
        csCount += cs->GetSize();
    }
    else {
      auto csSize = (*node)->GetObject<ndn::L3Protocol>()->getForwarder()->getCs().size();
      if (csSize != 0)
        csCount += csSize;
    }
  }

  os << "pit:" << pitCount << "\t";
  os << "cs:" << csCount << "\t";

  os << MemUsage::Get() / 1024.0 / 1024.0 << "MiB\n";

  if ((simTime + nextPrintTime) >= m_simulationTime) {
    double finalOverhead = MemUsage::Get() / 1024.0 / 1024.0;
    if (m_shouldEvaluatePit) {
      if (pitCount != 0) {
        os << "Approximate memory overhead per PIT entry:"
           <<  1000 * (finalOverhead - m_initialOverhead) / pitCount << "KiB\n";
      }
      else {
        os << "`The number of PIT entries is equal to zero\n";
      }
    }
    else {
      if (csCount != 0) {
        os << "Approximate memory overhead per CS entry:"
           <<  1000 * (finalOverhead - m_initialOverhead) / csCount << "KiB\n";
      }
      else {
        os << "The number of CS entries is equal to zero\n";
      }
    }
  }

  Simulator::Schedule(nextPrintTime, &Tester::printStats, this, ref(os), nextPrintTime,
                      beginRealTime);
}

int
Tester::run(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("10000Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.AddValue("old-cs", "Old content store to use "
                         "(e.g., ns3::ndn::cs::Lru, ns3::ndn::cs::Lfu, ...)",
               m_oldContentStore);
  cmd.AddValue("cs-size", "Maximum number of cached packets per node", m_csSize);
  cmd.AddValue("rate", "Interest rate", m_interestRate);
  cmd.AddValue("pit", "Perform PIT evaluation if this parameter is true",
               m_shouldEvaluatePit);
  cmd.AddValue("strategy", "Choose forwarding strategy "
                           "(e.g., /localhost/nfd/strategy/multicast, "
                           "/localhost/nfd/strategy/best-route, ...) ",
               m_strategy);
  cmd.AddValue("sim-time", "Simulation time", m_simulationTime);
  cmd.Parse(argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create(2);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.setCsSize(m_csSize);

  if (!m_oldContentStore.empty()) {
    ndnHelper.SetOldContentStore(m_oldContentStore, "MaxSize", std::to_string(m_csSize));
  }

  ndnHelper.InstallAll();

  ndn::FibHelper::AddRoute(nodes.Get(0), "/", nodes.Get(1), 10);
  if (!m_strategy.empty()) {
    ndn::StrategyChoiceHelper::InstallAll("/", m_strategy);
  }

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix("/prefix");
  consumerHelper.SetAttribute("Frequency", DoubleValue(m_interestRate));
  consumerHelper.Install(nodes.Get(0)); // first node

  if (!m_shouldEvaluatePit) {
    // Producer
    ndn::AppHelper producerHelper("ns3::ndn::Producer");
    // Producer will reply to all requests starting with /prefix
    producerHelper.SetPrefix("/prefix");
    producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
    producerHelper.Install(nodes.Get(1)); // last node
  }

  Simulator::Stop(m_simulationTime);

  struct ::timeval t;
  gettimeofday(&t, NULL);
  double beginRealTime = t.tv_sec + (0.000001 * (unsigned)t.tv_usec);
  Simulator::Schedule(Seconds(0), &Tester::printHeader, this, ref(std::cout));
  Simulator::Schedule(m_simulationTime / 200, &Tester::printStats, this, ref(std::cout),
                      m_simulationTime / 200, beginRealTime);

  L2RateTracer::InstallAll("drop-trace2.txt", Seconds(0.5));
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  ns3::Tester tester;
  return tester.run(argc, argv);
}
