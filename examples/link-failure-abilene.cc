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

#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <set>
#include "ns3/rocketfuel-topology-reader.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace ns3;
using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("LinkFailureSprint");

void PrintTime ()
{
  cout << "Progress: " << Simulator::Now ().ToDouble (Time::S) << "s" << endl;

  Simulator::Schedule (Seconds (1.0), PrintTime);
}


class Experiment
{
public:
  Experiment ()
  : m_reader ("/abilene", 2.0) { }

  void
  ConfigureTopology ()
  {
    string input ("./src/NDNabstraction/examples/abilene-topology.txt");
    string strategy ("ns3::CcnxBestRouteStrategy");
    // string strategy ("ns3::CcnxFloodingStrategy");
  
   
    m_reader.SetFileName (input);
    
    NodeContainer nodes = m_reader.Read ();
    
    if (m_reader.LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return;
    }

    NS_ASSERT_MSG (m_reader.LinksSize () != 0, "Problems reading the topology file. Failing.");    
    NS_LOG_INFO("Nodes = " << m_reader.GetNodes ().GetN());
    NS_LOG_INFO("Links = " << m_reader.LinksSize ());
  
    // ------------------------------------------------------------
    // -- Read topology data.
    // --------------------------------------------
        
    InternetStackHelper stack;
    Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
    stack.SetRoutingHelper (ipv4RoutingHelper);
    stack.Install (m_reader.GetNodes ());

    m_reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));

    // Install CCNx stack
    NS_LOG_INFO ("Installing CCNx stack");
    CcnxStackHelper ccnxHelper;
    ccnxHelper.SetForwardingStrategy (strategy);
    ccnxHelper.EnableLimits (true, Seconds(0.1));
    ccnxHelper.SetDefaultRoutes (false);
    ccnxHelper.InstallAll ();

    // // Populate FIB based on IPv4 global routing controller
    ccnxHelper.InstallFakeGlobalRoutes ();
    ccnxHelper.InstallRoutesToAll ();

    m_rand = UniformVariable (0, m_reader.GetNodes ().GetN());
    m_linkRand = UniformVariable(0, m_reader.LinksSize());
  }

public:  
  void
  Run (const Time &finishTime)
  {
    cout << "Run Simulation.\n";
    Simulator::Stop (finishTime);
    Simulator::Schedule (Seconds (1.0), PrintTime);
    Simulator::Run ();
    Simulator::Destroy ();
    cout << "Done.\n";
  }

  //We are creating "everybody-to-everybody" usage pattern
  ApplicationContainer
  AddApplications()
  {
    ApplicationContainer apps;
    for(uint32_t i =0; i<m_reader.GetNodes().GetN(); i++)
    {
      Ptr<Node> node1 = Names::Find<Node> ("/abilene", lexical_cast<string> (i));
      CcnxAppHelper producerHelper ("ns3::CcnxProducer");
      producerHelper.SetPrefix ("/" + lexical_cast<string> (i));
        
      apps.Add(producerHelper.Install (node1));
          
      for(uint32_t j = 0; j<m_reader.GetNodes().GetN();j++)
      {
        if(i==j)
          continue;
        
        Ptr<Node> node2 = Names::Find<Node> ("/abilene", lexical_cast<string> (j));
        
        CcnxAppHelper consumerHelper ("ns3::CcnxConsumer");
        consumerHelper.SetPrefix ("/" + lexical_cast<string> (i));
        consumerHelper.SetAttribute ("MeanRate", StringValue ("1Mbps"));
        consumerHelper.SetAttribute ("Size", StringValue ("2"));  
        
        apps.Add(consumerHelper.Install (node2));
      }
    }
    return apps;
  }
  
  UniformVariable m_rand;
  UniformVariable m_linkRand;
  
private:
  AnnotatedTopologyReader m_reader;
};

int 
main (int argc, char *argv[])
{
  cout << "Begin link failure scenario\n";
  
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("2Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("100"));

  Time finishTime = Seconds (20.0);

  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.Parse (argc, argv);

  Experiment experiment;

  for (uint32_t i = 0; i < 100; i++)
    {
  
      experiment.ConfigureTopology ();
      ApplicationContainer apps = experiment.AddApplications ();

      for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
        {
          cout << "From " << apps.Get (i*2)->GetNode ()->GetId ()
               << " to "  << apps.Get (i*2 + 1)->GetNode ()->GetId ();
          cout << "\n";
        }
  
      CcnxTraceHelper traceHelper;
      // traceHelper.EnableAggregateAppAll ("ns3::CcnxConsumer");
      // traceHelper.EnableAggregateAppAll ("ns3::CcnxProducer");
      // traceHelper.EnableAggregateL3All ();
      // traceHelper.SetL3TraceFile ("trace-l3.log");
      // traceHelper.SetAppTraceFile ("trace-app.log");
      // traceHelper.EnableRateL3All ("rate-trace.log");
      traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", "consumers-seqs.log");

      experiment.Run (finishTime);
    }

  cout << "Finish link failure scenario\n";
  return 0;
}
