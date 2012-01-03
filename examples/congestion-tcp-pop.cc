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
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store.h"

#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <set>
#include "ns3/rocketfuel-topology-reader.h"

#include "../helper/tracers/ipv4-seqs-app-tracer.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace ns3;
using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("Scenario");

void PrintTime ()
{
  cout << "Progress: " << Simulator::Now ().ToDouble (Time::S) << "s" << endl;

  Simulator::Schedule (Seconds (1.0), PrintTime);
}

class Experiment
{
public:
  Experiment ()
    : m_rand (0,52)
    , reader ("/sprint")
  { }

  void
  ConfigureIpv4Topology ()
  {
    Names::Clear ();
    
    string weights   ("./src/NDNabstraction/examples/sprint-pops.weights");
    string latencies ("./src/NDNabstraction/examples/sprint-pops.latencies");
    string positions ("./src/NDNabstraction/examples/sprint-pops.positions");
  
    reader.SetFileName (positions);
    reader.SetFileType (RocketfuelWeightsReader::POSITIONS);
    reader.Read ();
  
    reader.SetFileName (weights);
    reader.SetFileType (RocketfuelWeightsReader::WEIGHTS);    
    reader.Read ();

    reader.SetFileName (latencies);
    reader.SetFileType (RocketfuelWeightsReader::LATENCIES);    
    reader.Read ();
    
    reader.Commit ();
    NS_ASSERT_MSG (reader.LinksSize () != 0, "Problems reading the topology file. Failing.");
    
    NS_LOG_INFO("Nodes = " << reader.GetNodes ().GetN());
    NS_LOG_INFO("Links = " << reader.LinksSize ());

    InternetStackHelper stack;
    stack.Install (reader.GetNodes ());
    reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));
  }

  void
  ConfigureRouting ()
  {
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    // m_rand = UniformVariable (0, reader.GetNodes ().GetN());
  }
  
  ApplicationContainer
  AddTcpRandomApplications (uint16_t numStreams)
  {
    map<uint32_t, set<uint32_t> > streams;
    ApplicationContainer apps;

    const static uint32_t base_port = 10;
    uint16_t createdStreams = 0;
    uint16_t guard = 0;
    while (createdStreams < numStreams && guard < (numeric_limits<uint16_t>::max ()-1))
      {
        guard ++;
        
        uint32_t node1_num = m_rand.GetValue ();
        uint32_t node2_num = m_rand.GetValue ();

        if (node1_num == node2_num)
          continue;

        if (streams[node1_num].count (node2_num) > 0) // don't create duplicate streams
          continue;
        
        streams[node1_num].insert (node2_num);

        Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (node1_num));
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (node2_num));

        Ptr<Ipv4> ipv4 = node1->GetObject<Ipv4> ();
        // ipv4->GetAddress (0, 0);

        // to make sure we don't reuse the same port numbers for different flows, just make all port numbers unique
        PacketSinkHelper consumerHelper ("ns3::TcpSocketFactory",
                                         InetSocketAddress (Ipv4Address::GetAny (), base_port + createdStreams));

        BulkSendHelper producerHelper ("ns3::TcpSocketFactory",
                                       InetSocketAddress (ipv4->GetAddress (1, 0).GetLocal (), base_port + createdStreams));
        // cout << "SendTo: " <<  ipv4->GetAddress (1, 0).GetLocal () << endl;
        producerHelper.SetAttribute ("MaxBytes", UintegerValue (2081040)); // equal to 2001 ccnx packets
        
        apps.Add
          (consumerHelper.Install (node1));

        apps.Add
          (producerHelper.Install (node2));

        createdStreams ++;
      }

    return apps;
  }

  void
  Run (const Time &finishTime)
  {
    cout << "Run Simulation.\n";
    Simulator::Stop (finishTime);
    // Simulator::Schedule (Seconds (1.0), PrintTime);
    Simulator::Run ();
    Simulator::Destroy ();
    cout << "Done.\n";
  }

  UniformVariable m_rand;
  RocketfuelWeightsReader reader;
};

int 
main (int argc, char *argv[])
{
  cout << "Begin congestion-pop scenario\n";
  Packet::EnableChecking();
  Packet::EnablePrinting();
  
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", StringValue ("1040"));
  
  Config::SetDefault ("ns3::BulkSendApplication::SendSize", StringValue ("1040"));

  // Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("attributes.xml"));
  // Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  // Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));
  
  uint32_t maxRuns = 1;
  uint32_t startRun = 0;
  CommandLine cmd;
  cmd.AddValue ("start", "Initial run number", startRun);
  cmd.AddValue ("runs", "Number of runs", maxRuns);
  cmd.Parse (argc, argv);

  // ConfigStore config;
  // config.ConfigureDefaults ();

  for (uint32_t run = startRun; run < startRun + maxRuns; run++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (run));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;
      
      Experiment experiment;
      cout << "Run " << run << endl;
      
      experiment.ConfigureIpv4Topology ();
      ApplicationContainer apps = experiment.AddTcpRandomApplications (20);
      experiment.ConfigureRouting ();

      string prefix = "run-tcp-" + lexical_cast<string> (run) + "-";      

      ofstream of_nodes ((prefix + "apps.log").c_str ());
      for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
        {
          of_nodes << "From " << apps.Get (i*2)->GetNode ()->GetId ()
                   << " to "  << apps.Get (i*2 + 1)->GetNode ()->GetId ();
          of_nodes << "\n";
        }

      CcnxTraceHelper traceHelper;
      traceHelper.EnableIpv4SeqsAppAll (prefix + "consumers-seqs.log");

      // config.ConfigureAttributes ();  

      experiment.Run (Seconds (200.0));
    }

  // cout << "Finish congestion-pop scenario\n";
  return 0;
}
