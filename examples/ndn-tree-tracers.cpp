/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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

// ndn-tree-tracers.cc

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;

/**
 * This scenario simulates a tree topology (using topology reader module)
 *
 *    /------\      /------\      /------\      /------\
 *    |leaf-1|      |leaf-2|      |leaf-3|      |leaf-4|
 *    \------/      \------/      \------/      \------/
 *         ^          ^                ^           ^	
 *         |          |                |           |
 *    	    \        /                  \         / 
 *           \      /  			 \  	 /    10Mbps / 1ms
 *            \    /  			  \ 	/
 *             |  |  			   |   | 
 *    	       v  v                        v   v     
 *          /-------\                    /-------\
 *          | rtr-1 |                    | rtr-2 |
 *          \-------/                    \-------/
 *                ^                        ^                      
 *      	  |	 		   |
 *      	   \			  /  10 Mpbs / 1ms 
 *      	    +--------+  +--------+ 
 *      		     |  |      
 *                           v  v
 *      		  /--------\
 *      		  |  root  |
 *                        \--------/
 *
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     ./waf --run=ndn-tree-tracers
 */

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  AnnotatedTopologyReader topologyReader ("", 1);
  topologyReader.SetFileName ("src/ndnSIM/examples/topologies/topo-tree.txt");
  topologyReader.Read ();

  // Install CCNx stack on all nodes
  ndn::StackHelper ccnxHelper;
  ccnxHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
  ccnxHelper.InstallAll ();

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ccnxGlobalRoutingHelper;
  ccnxGlobalRoutingHelper.InstallAll ();

  // Getting containers for the consumer/producer
  Ptr<Node> consumers[4] = { Names::Find<Node> ("leaf-1"), Names::Find<Node> ("leaf-2"),
                             Names::Find<Node> ("leaf-3"), Names::Find<Node> ("leaf-4") };
  Ptr<Node> producer = Names::Find<Node> ("root");

  for (int i = 0; i < 4; i++)
    {
      ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
      consumerHelper.SetAttribute ("Frequency", StringValue ("100")); // 100 interests a second

      // Each consumer will express unique interests /root/<leaf-name>/<seq-no>
      consumerHelper.SetPrefix ("/root/" + Names::FindName (consumers[i]));
      consumerHelper.Install (consumers[i]);
    }
    
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));  

  // Register /root prefix with global routing controller and
  // install producer that will satisfy Interests in /root namespace
  ccnxGlobalRoutingHelper.AddOrigins ("/root", producer);
  producerHelper.SetPrefix ("/root");
  producerHelper.Install (producer);

  // Calculate and install FIBs
  ccnxGlobalRoutingHelper.CalculateRoutes ();

  Simulator::Stop (Seconds (20.0));

  ndn::L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (0.5));
  ndn::L3RateTracer::InstallAll ("rate-trace.txt", Seconds (0.5));
  
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
