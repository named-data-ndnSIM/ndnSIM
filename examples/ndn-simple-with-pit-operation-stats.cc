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
// ndn-simple-with-pit-operation-stats.cc
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

#include "ns3/ndnSIM/model/pit/ndn-pit-impl.h"
#include "ns3/ndnSIM/utils/trie/persistent-policy.h"
// #include "ns3/ndnSIM/utils/trie/random-policy.h"
// #include "ns3/ndnSIM/utils/trie/lru-policy.h"
#include "ns3/ndnSIM/utils/trie/multi-policy.h"
#include "ns3/ndnSIM/utils/trie/aggregate-stats-policy.h"

using namespace ns3;

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     ./waf --run=ndn-simple-with-pit-operation-stats
 */

typedef ndn::ndnSIM::multi_policy_traits< boost::mpl::vector2< ndn::ndnSIM::persistent_policy_traits,
                                                               ndn::ndnSIM::aggregate_stats_policy_traits > > PersistentWithCountsTraits;
// typedef ndn::ndnSIM::multi_policy_traits< boost::mpl::vector2< ndn::ndnSIM::random_policy_traits,
//                                                                ndn::ndnSIM::aggregate_stats_policy_traits > > RandomWithCountsTraits;
// typedef ndn::ndnSIM::multi_policy_traits< boost::mpl::vector2< ndn::ndnSIM::lru_policy_traits,
//                                                                ndn::ndnSIM::aggregate_stats_policy_traits > > LruWithCountsTraits;

void
PeriodicStatsPrinter (Ptr<Node> node, Time next)
{
  if (DynamicCast<ndn::pit::PitImpl<PersistentWithCountsTraits> > (node->GetObject<ndn::Pit> ()) == 0)
    {
      std::cerr << "Invalid PIT class, please correct the scenario" << std::endl;
      return;
    }
  
  //  "ns3::ndn::pit::Persistent::AggregateStats"
  ndn::pit::PitImpl<PersistentWithCountsTraits>::super::policy_container &policy =
    DynamicCast<ndn::pit::PitImpl<PersistentWithCountsTraits> > (node->GetObject<ndn::Pit> ())->GetPolicy ();

  std::cout << Simulator::Now ().ToDouble (Time::S) << "\t"
            << node->GetId () << "\t"
            << policy.get<1> ().GetUpdates () << "\t"
            << policy.get<1> ().GetInserts () << "\t"
            << policy.get<1> ().GetLookups () << "\t"
            << policy.get<1> ().GetErases () << "\n";

  policy.get<1> ().ResetStats ();
  
  Simulator::Schedule (next, PeriodicStatsPrinter, node, next);
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
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes (true);
  ndnHelper.SetPit ("ns3::ndn::pit::Persistent::AggregateStats");
  ndnHelper.InstallAll ();

  // set up periodic PIT stats printer on node 1
  std::cout << "Time" << "\t"
            << "NodeId" << "\t"
            << "Updates" << "\t"
            << "Inserts" << "\t"
            << "Lookups" << "\t"
            << "Erases" << "\n";
  Simulator::Schedule (Seconds (1), PeriodicStatsPrinter, nodes.Get (1), Seconds (1));
  
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

  Simulator::Stop (Seconds (20.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
