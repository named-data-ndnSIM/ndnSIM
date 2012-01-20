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

void PrintTime ()
{
  cout << "Progress: " << Simulator::Now ().ToDouble (Time::S) << "s" << endl;

  Simulator::Schedule (Seconds (1.0), PrintTime);
}

#include "base-experiment.h"

class Experiment : public BaseExperiment
{
public:
  enum nodes {N0,N1,N2,N3,N4,N5,N6,N7,N8,N9,N10,N11,N12,N13,N14,N15,N16,N17,N18,N19,N20,N21,N22,N23,N24,N25,N26,N27,N28,N29,N30,N31,N32,N33,N34,N35,N36,N37,N38,N39,N40,N41,N42,N43,N44,N45,N46,N47,N48,N49,N50,N51};
  
  typedef std::pair<int, int> Edge;
  const Edge edge_array[84];
  
  Experiment ()
    : edge_array((Edge[84]){ 
    Edge(N0, N1), 
    Edge(N2, N3), 
    Edge(N1, N4), 
    Edge(N1, N5),
    Edge(N1, N6),
    Edge(N1, N7),
    Edge(N1, N8),
    Edge(N1, N9),
    Edge(N1, N10),
    Edge(N1, N11),
    Edge(N12, N13),
    Edge(N14, N15),
    Edge(N14, N16),
    Edge(N14, N17),
    Edge(N15, N18),
    Edge(N15, N19),
    Edge(N20, N21),
    Edge(N20, N22),
    Edge(N23, N24),
    Edge(N25, N26),
    Edge(N27, N28),
    Edge(N8, N20),
    Edge(N8, N16),
    Edge(N8, N28),
    Edge(N8, N24),
    Edge(N8, N26),
    Edge(N21, N31),
    Edge(N13, N32),
    Edge(N13, N33),
    Edge(N17, N35),
    Edge(N17, N20),
    Edge(N17, N18),
    Edge(N17, N28),
    Edge(N17, N24),
    Edge(N17, N25),
    Edge(N17, N26),
    Edge(N17, N29),
    Edge(N17, N36),
    Edge(N11, N24),
    Edge(N11, N38),
    Edge(N11, N17),
    Edge(N33, N39),
    Edge(N33, N40),
    Edge(N6, N7),
    Edge(N6, N28),
    Edge(N6, N25),
    Edge(N6, N8),
    Edge(N6, N17),
    Edge(N6, N34),
    Edge(N6, N11),
    Edge(N39, N42),
    Edge(N16, N24),
    Edge(N16, N29),
    Edge(N16, N17),
    Edge(N31, N45),
    Edge(N31, N46),
    Edge(N31, N37),
    Edge(N31, N47),
    Edge(N32, N48),
    Edge(N32, N44),
    Edge(N32, N42),
    Edge(N48, N49),
    Edge(N7, N13),
    Edge(N38, N43),
    Edge(N9, N15),
    Edge(N9, N20),
    Edge(N9, N31),
    Edge(N9, N30),
    Edge(N9, N17),
    Edge(N9, N19),
    Edge(N5, N32),
    Edge(N5, N8),
    Edge(N5, N7),
    Edge(N18, N23),
    Edge(N40, N48),
    Edge(N40, N49),
    Edge(N24, N38),
    Edge(N24, N31),
    Edge(N24, N45),
    Edge(N24, N50),
    Edge(N3, N9),
    Edge(N19, N41),
    Edge(N19, N20),
    Edge(N19, N51) 
    })
  { }

  
  int
  ComputeShortestWeightsPath(uint32_t sourceNode, uint32_t destinationNode)
  {
    typedef adjacency_list < listS, vecS, undirectedS,
    no_property, property < edge_weight_t, int > > graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
    
    //string name[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51"};
  
    
    int weights[] = { 312, 10786, 222, 2500, 4000, 2500, 3860, 11769, 352, 3500, 2622, 500, 14192, 8909, 11747, 44530, 19775, 345, 5337, 2184, 645, 11409, 8282, 3000, 7735, 5500, 20741, 7552, 1500, 2091,  14409, 4337, 4000, 5735, 1315, 2500, 3282, 2478, 5096, 3235, 4360, 2000, 3000, 2500, 6860, 5176, 5860, 3860, 802, 5500, 699, 1547, 3000, 5282, 500, 268, 3375, 2708, 1712, 2329, 3352, 3201, 30890, 1643, 5500, 2500, 4735, 124, 13909, 42030, 28338, 2360, 2000, 5735, 2340, 2529, 2860, 9909, 10409, 92, 59812, 26190, 39530, 14125 };
    
    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    
    graph_t g(edge_array, edge_array + num_arcs, weights, m_numNodes);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
    
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<int> d(num_vertices(g));
    
    vertex_descriptor s = vertex(static_cast<nodes>(sourceNode), g);
    
    dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
    
    NS_LOG_INFO("Shortest distance (weights) from Node"<<sourceNode << " to Node"<<destinationNode<<" equals  " << d[destinationNode]);
    
    /*
    //PRINTING
    NS_LOG_INFO ("distances and parents:");
    graph_traits < graph_t >::vertex_iterator vi, vend;
    
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) 
      {
        NS_LOG_INFO("distance(" << name[*vi] << ") = " << d[*vi] << ", ");
        NS_LOG_INFO("parent(" << name[*vi] << ") = " << name[p[*vi]] << std::endl);
      }
    */
      
    return d[destinationNode];
  }
  
  double 
  ComputeShortestDelayPath(uint32_t sourceNode, uint32_t destinationNode)
  {
    typedef adjacency_list < listS, vecS, undirectedS,
    no_property, property < edge_weight_t, double > > graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
    
    //string name[] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51"};
    
    double weights[] = {0.312, 10.786, 0.222, 1.035, 1.414, 1.24, 0.814, 19.532, 0.352, 4.593, 2.622, 0.207, 12.098, 13.941, 7.791, 38.946, 19.775, 0.345, 5.337, 0.276, 0.645, 19.787, 8.352, 1.578,  10.459, 5.005, 20.741, 4.737, 1.424, 2.091, 14.409, 7.13, 6.214, 6.437, 1.315, 1.176, 3.282, 2.478, 5.751, 3.235, 4.718, 1.817, 2.035, 0.327, 0.97, 5.176, 0.612, 5.725, 0.802, 6.007, 0.699, 3.655, 0.135, 3.286, 0.268, 0.268, 3.375, 2.708, 1.712, 2.329, 1.595, 3.201, 31.13, 1.643, 5.513, 0.437, 2.648, 0.124, 14.774, 42.03, 28.338, 0.359, 0.316, 0.779, 2.34, 2.529, 7.706, 9.827, 10.045, 0.092, 59.812, 26.19, 42.057, 14.125};

    int num_arcs = sizeof(edge_array) / sizeof(Edge);
    
    graph_t g(edge_array, edge_array + num_arcs, weights, m_numNodes);
    property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
    
    std::vector<vertex_descriptor> p(num_vertices(g));
    std::vector<double> d(num_vertices(g));
    
    vertex_descriptor s = vertex(static_cast<nodes>(sourceNode), g);
    
    dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
    
    NS_LOG_INFO("Shortest distance (delay) from Node"<<sourceNode << " to Node"<<destinationNode<<" equals  " << d[destinationNode]);
    
    /*
    //PRINTING
    NS_LOG_INFO ("distances and parents:");
    graph_traits < graph_t >::vertex_iterator vi, vend;
    
    for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) 
      {
        NS_LOG_INFO("distance(" << name[*vi] << ") = " << d[*vi] << ", ");
        NS_LOG_INFO("parent(" << name[*vi] << ") = " << name[p[*vi]] << std::endl);
      }
    */
    
    return d[destinationNode];
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
        
        CcnxAppHelper fakeProducerHelper ("ns3::CcnxHijacker");
        fakeProducerHelper.SetPrefix (prefix);
        ApplicationContainer hijacker = fakeProducerHelper.Install (node2);
        apps.Add (hijacker);
        hijacker.Start (Seconds(1.0));
        
        // one more trick. Need to install route to hijacker (aka "hijacker announces itself as a legitimate producer")
        CcnxStackHelper::InstallRouteTo (prefix, node1);
        Simulator::Schedule (Seconds(1.0), CcnxStackHelper::InstallRouteTo, prefix, node2);
        // CcnxStackHelper::InstallRouteTo (prefix, node2);

        prefixes.push_back (prefix); // remember prefixes that consumers will be requesting
      }

    // All consumers request exactly 10 packets, to convert number interests packets to requested size:
    // size = 1040 * (max_number_of_packets-1) / 1024 / 1024
    // double requestSize = 1040.0 * (10 - 1) / 1024.0 / 1024.0;
    
    // Create Consumers
    NodeContainer nodes = reader->GetNodes ();
    for (NodeContainer::Iterator node = nodes.Begin (); node != nodes.End (); node++)
      {
        uint32_t namedId = lexical_cast<uint32_t> (Names::FindName (*node));
        if (m_usedNodes.count (namedId) > 0)
          continue;

        CcnxAppHelper consumerHelper ("ns3::CcnxConsumerBatches");
        consumerHelper.SetAttribute ("LifeTime", StringValue("100s"));
        consumerHelper.SetAttribute ("Batches", StringValue("0s 10 2s 1"));
        BOOST_FOREACH (const string &prefix, prefixes)
          {
            consumerHelper.SetPrefix (prefix);

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
      experiment.GenerateRandomPairs (1);
      experiment.ComputeShortestWeightsPath(1,12);
      experiment.ComputeShortestDelayPath(1,12);
      cout << "Run " << run << endl;
      
      string prefix = "blackhole-" + lexical_cast<string> (run) + "-";
  
      experiment.ConfigureTopology ();
      experiment.InstallCcnxStack (false);
      ApplicationContainer apps = experiment.AddApplications ();
            
      //tracing
      CcnxTraceHelper traceHelper;
      // traceHelper.EnableRateL3All (prefix + "rate-trace.log");
      traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumerBatches", prefix + "consumers-seqs.log");

      traceHelper.EnablePathWeights (prefix + "weights.log");
      std::cout << "Total " << apps.GetN () << " applications\n";
      for (ApplicationContainer::Iterator i = apps.Begin (); i != apps.End (); i++)
        {
          Simulator::Schedule (Seconds (1.999999), &CcnxTraceHelper::WeightsConnect, &traceHelper, (*i)->GetNode (), *i);
        }

      experiment.Run (Seconds(40.0));
    }

  cout << "Finish blackhole scenario\n";
  return 0;
}
