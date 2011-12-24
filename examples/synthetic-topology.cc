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
#include "../utils/spring-mobility-helper.h"
#include "../helper/ccnx-trace-helper.h"

#include <sstream>
#include "ns3/annotated-topology-reader.h"

#include "ns3/config-store.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("CcnxSyntheticTopology");

void PrintTime ()
{
  NS_LOG_INFO (Simulator::Now ());
    
  Simulator::Schedule (Seconds (10.0), PrintTime);
}

int
main (int argc, char *argv[])
{
  string input ("./src/NDNabstraction/examples/synthetic-topology.txt");
    
  Time finishTime = Seconds (20.0);
  string strategy = "ns3::CcnxFloodingStrategy";
  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.AddValue ("strategy", "CCNx forwarding strategy", strategy);
  cmd.Parse (argc, argv);
    
  ConfigStore config;
  config.ConfigureDefaults ();
    
  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
    
  AnnotatedTopologyReader reader ("/synthetic");
  // reader.SetMobilityModel ("ns3::SpringMobilityModel");
  reader.SetFileName (input);
  NodeContainer nodes = reader.Read ();
  // SpringMobilityHelper::InstallSprings (reader.LinksBegin (), reader.LinksEnd ());
    
  InternetStackHelper stack;
  Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  stack.SetRoutingHelper (ipv4RoutingHelper);
  stack.Install (nodes);

  reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));

  NS_LOG_INFO("Nodes = " << nodes.GetN());
  NS_LOG_INFO("Links = " << reader.LinksSize ());
    
  // Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetForwardingStrategy (strategy);
  ccnxHelper.EnableLimits (true, Seconds(0.1));
  ccnxHelper.SetDefaultRoutes (false);
  ccnxHelper.InstallAll ();
    
  NS_LOG_INFO ("Installing Applications");
  CcnxAppHelper consumerHelper ("ns3::CcnxConsumer");

  consumerHelper.SetPrefix ("/6");
  consumerHelper.SetAttribute ("MeanRate", StringValue ("2Mbps"));
  consumerHelper.SetAttribute ("Size", StringValue ("1.0"));
  ApplicationContainer consumers = consumerHelper.Install (Names::Find<Node> ("/synthetic", "c1"));

  consumerHelper.SetPrefix ("/7");
  consumerHelper.SetAttribute ("MeanRate", StringValue ("2Mbps"));
  ApplicationContainer consumers2 = consumerHelper.Install(Names::Find<Node> ("/synthetic", "c2"));

  consumerHelper.SetPrefix ("/8");
  consumerHelper.SetAttribute ("MeanRate", StringValue ("2Mbps"));
  ApplicationContainer consumers3 = consumerHelper.Install(Names::Find<Node> ("/synthetic", "c3"));
  
  consumerHelper.SetPrefix ("/10");
  consumerHelper.SetAttribute ("MeanRate", StringValue ("2Mbps"));
  ApplicationContainer consumers4 = consumerHelper.Install(Names::Find<Node> ("/synthetic", "c4"));

  consumers.Start (Seconds (0));
  consumers2.Start (Seconds (2.5));
  consumers3.Start (Seconds (5));
  consumers4.Start (Seconds (7.5));

  /////////////////////////////////////////////
  
  CcnxAppHelper producerHelper ("ns3::CcnxProducer");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
 
  producerHelper.SetPrefix ("/6");  
  producerHelper.Install (Names::Find<Node> ("/synthetic", "p1"));
        
  producerHelper.SetPrefix ("/7");
  producerHelper.Install (Names::Find<Node> ("/synthetic", "p2"));

  producerHelper.SetPrefix ("/8");
  producerHelper.Install (Names::Find<Node> ("/synthetic", "p3"));
  
  producerHelper.SetPrefix ("/10");
  producerHelper.Install (Names::Find<Node> ("/synthetic", "p4"));

  // Populate FIB based on IPv4 global routing controller
  ccnxHelper.InstallFakeGlobalRoutes ();
  ccnxHelper.InstallRouteTo (Names::Find<Node> ("/synthetic", "p1"));
  ccnxHelper.InstallRouteTo (Names::Find<Node> ("/synthetic", "p2"));
  ccnxHelper.InstallRouteTo (Names::Find<Node> ("/synthetic", "p3"));
  ccnxHelper.InstallRouteTo (Names::Find<Node> ("/synthetic", "p4"));

  Simulator::Schedule (Seconds (10.0), PrintTime);

  Simulator::Stop (finishTime);

  CcnxTraceHelper traceHelper;
  // traceHelper.EnableAggregateAppAll ("ns3::CcnxConsumer");
  // traceHelper.EnableAggregateAppAll ("ns3::CcnxProducer");
  // traceHelper.EnableAggregateL3All ();
  // traceHelper.SetL3TraceFile ("trace-l3.log");
  // traceHelper.SetAppTraceFile ("trace-app.log");
  // traceHelper.EnableRateL3All ("rate-trace.log");
  traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", "consumers-seqs.log");

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
