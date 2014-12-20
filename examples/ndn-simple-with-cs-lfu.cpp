/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012-2013 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */
// ndn-simple-with-cs-lfu.cc
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include <sys/time.h>
#include "ns3/ndnSIM/utils/mem-usage.h"

using namespace ns3;

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 * This scenario demonstrates how to use content store that responds to Freshness parameter set in Datas.
 * That is, if producer set Freshness field to 2 seconds, the corresponding content object will not be cached
 * more than 2 seconds (can be cached for a shorter time, if entry is evicted earlier)
 *
 *     NS_LOG=ndn.Consumer ./waf --run ndn-simple-with-cs-lfu
 */

void PrintCsMemStatsHeader (std::ostream &os)
{
  os << "SimulationTime" << "\t"
     << "RealTime" << "\t"
     // << "NumberOfProcessedData" << "\t"
     // << "NumberOfProcessedInterests" << "\t"
     << "NumberPitEntries" << "\t"
     << "NumberCsEntries" << "\t"
     << "MemUsage" << "\n";
}

void
PrintCsMemStats (std::ostream &os, Time nextPrintTime, double beginRealTime)
{
  ::timeval t;
  gettimeofday(&t, NULL);
  double realTime = t.tv_sec + (0.000001 * (unsigned)t.tv_usec) - beginRealTime;

  os << Simulator::Now ().ToDouble (Time::S) << "\t";
  os << realTime << "\t";

  // os << ndn::L3Protocol::GetDataCounter () << "\t";
  // os << ndn::L3Protocol::GetInterestCounter () << "\t";

  uint64_t pitCount = 0;
  uint64_t csCount = 0;
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      Ptr<ndn::Pit> pit = (*node)->GetObject<ndn::Pit> ();
      Ptr<ndn::ContentStore> cs = (*node)->GetObject<ndn::ContentStore> ();

      if (pit != 0)
        pitCount += pit->GetSize ();

      if (cs != 0)
        csCount += cs->GetSize ();
    }

  os << pitCount << "\t";
  os << csCount << "\t";
  os << MemUsage::Get () / 1024.0 / 1024.0 << "\n";

  Simulator::Schedule (nextPrintTime, PrintCsMemStats, boost::ref (os), nextPrintTime, beginRealTime);
}

int
main (int argc, char *argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create (3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install (nodes.Get (0), nodes.Get (1));
  p2p.Install (nodes.Get (1), nodes.Get (2));

  // Install CCNx stack on all nodes
  ndn::StackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes (true);

  // node 0: disable cache completely
  ccnxHelper.SetContentStore ("ns3::ndn::cs::Nocache"); // disable cache
  ccnxHelper.Install (nodes.Get (0));

  // node 1 and 2: set cache with Lfu policy
  ccnxHelper.SetContentStore ("ns3::ndn::cs::Freshness::Lfu", "MaxSize", "2"); // can set cache size this way
  ccnxHelper.Install (nodes.Get (1));
  ccnxHelper.Install (nodes.Get (2));

  // alternative way to configure cache size
  // [number after nodeList is global ID of the node (= node->GetId ())]
  Config::Set ("/NodeList/2/$ns3::ndn::ContentStore/MaxSize", UintegerValue (100000));

  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
  // Consumer will request /prefix/0, /prefix/1, ...
  consumerHelper.SetPrefix ("/prefix");
  consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
  consumerHelper.Install (nodes.Get (0)); // first node

  // Producer
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix ("/prefix");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (nodes.Get (2)); // last node

  Simulator::Stop (Seconds (200000.0));

  struct ::timeval t;
  gettimeofday(&t, NULL);
  double beginRealTime = t.tv_sec + (0.000001 * (unsigned)t.tv_usec);
  Simulator::Schedule (Seconds (0), PrintCsMemStatsHeader, boost::ref (std::cout));
  Simulator::Schedule (Seconds (100), PrintCsMemStats, boost::ref (std::cout), Seconds (100), beginRealTime);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
