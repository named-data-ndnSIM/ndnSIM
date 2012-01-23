/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */
 
 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/NDNabstraction-module.h"
#include "ns3/point-to-point-grid.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/random-variable.h"

#include <sstream>
#include <map>
#include <list>
#include <set>
#include "ns3/rocketfuel-topology-reader.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace ns3;
using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("BlackholeSprint");

#include "base-experiment.h"

class Experiment : public BaseExperiment
{
public:
  // hijacker is more than an application. just disable all faces
  static void
  InstallHijacker (std::string prefix, Ptr<Node> node)
  {
    Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
    for (uint32_t i = 0; i < ccnx->GetNFaces (); i++)
      {
        Ptr<CcnxFace> face = ccnx->GetFace (i);
        face->SetUp (false);
      }
    CcnxStackHelper::InstallRouteTo (prefix, node);
  }

  //We are creating 10 pairs of producer-hijacker and everybody else is a consumer
  ApplicationContainer
  AddApplications ()
  {
    ApplicationContainer apps;

    list<string> prefixes;

    // Create Producers/Hijackers
    uint32_t pair = 0;
    for (list<tuple<uint32_t,uint32_t> >::iterator i = m_pairs.begin (); i != m_pairs.end (); i++)
      {
        uint32_t node1_num = i->get<0> ();
        uint32_t node2_num = i->get<1> ();

        cout << "Good: " << node1_num << ", bad: " << node2_num << "\n";
        
        Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (node1_num));
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (node2_num));

        // node1 legitimate producer
        // node2 "fake" producer

        string prefix = "/bh/" + lexical_cast<string> (pair);
        pair ++;

        CcnxAppHelper legitimateProducerHelper ("ns3::CcnxProducer");
        legitimateProducerHelper.SetPrefix (prefix);
        apps.Add
          (legitimateProducerHelper.Install (node1));
        
        // one more trick. Need to install route to hijacker (aka "hijacker announces itself as a legitimate producer")
        CcnxStackHelper::InstallRouteTo (prefix, node1);
        Simulator::Schedule (Seconds(10.0), Experiment::InstallHijacker, prefix, node2);

        prefixes.push_back (prefix); // remember prefixes that consumers will be requesting
      }
    
    // Create Consumers
    NodeContainer nodes = reader->GetNodes ();
    for (NodeContainer::Iterator node = nodes.Begin (); node != nodes.End (); node++)
      {
        uint32_t namedId = lexical_cast<uint32_t> (Names::FindName (*node));
        if (m_usedNodes.count (namedId) > 0)
          continue;

        CcnxAppHelper consumerHelper ("ns3::CcnxConsumerBatches");
        consumerHelper.SetAttribute ("LifeTime", StringValue("100s"));
        consumerHelper.SetAttribute ("Batches", StringValue("0s 10 6s 1 20s 1"));
        BOOST_FOREACH (const string &prefix, prefixes)
          {
            consumerHelper.SetPrefix (prefix + "/" + lexical_cast<string> (namedId)); // make sure we're requesting unique prefixes... this was a huge bug before                 

            ApplicationContainer consumer = consumerHelper.Install (*node);
            apps.Add (consumer);
          }

        // break;
      }
    return apps;
  }
};

int 
main (int argc, char *argv[])
{
  cout << "Begin blackhole scenario\n";
  
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("100Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("2000"));
  Config::SetDefault ("ns3::RttEstimator::InitialEstimation", StringValue ("0.5s"));

  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("attributes.xml"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));

  uint32_t maxRuns = 1;
  uint32_t startRun = 0;
  CommandLine cmd;
  cmd.AddValue ("start", "Initial run number", startRun);
  cmd.AddValue ("runs", "Number of runs", maxRuns);
  cmd.Parse (argc, argv);

  // ConfigStore config;
  // config.ConfigureDefaults ();

  Experiment experiment;
  for (uint32_t run = startRun; run < startRun + maxRuns; run++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (run));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

      Experiment experiment;
      // experiment.GenerateRandomPairs (1);
      experiment.SetPair (run);
      cout << "Run " << run << endl;
      
      string prefix = "blackhole-" + lexical_cast<string> (run) + "-";
  
      experiment.ConfigureTopology ();
      experiment.InstallCcnxStack (false);
      ApplicationContainer apps = experiment.AddApplications ();
            
      //tracing
      CcnxTraceHelper traceHelper;
      // traceHelper.EnableRateL3All (prefix + "rate-trace.log");
      traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumerBatches", prefix + "consumers-seqs.log");

      // enable path weights some time from now (ensure that all faces are created)
      Simulator::Schedule (Seconds (4.5), &CcnxTraceHelper::EnablePathWeights, &traceHelper, prefix + "weights.log");
      std::cout << "Total " << apps.GetN () << " applications\n";

      experiment.Run (Seconds(40.0));
    }

  cout << "Finish blackhole scenario\n";
  return 0;
}
