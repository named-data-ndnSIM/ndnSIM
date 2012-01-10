/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include <ctime>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/annotated-topology-reader.h"
#include <list>
#include "ns3/ccnx.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/NDNabstraction-module.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("AnnotatedTopologyExample");

int main (int argc, char *argv[])
{
  // Packet::EnableChecking();
  // Packet::EnablePrinting();
  string input ("./src/NDNabstraction/examples/simpletopology.txt");
    
  // Time finishTime;
  // string animationFile;
  string strategy = "ns3::CcnxFloodingStrategy";
  CommandLine cmd;
  // cmd.AddValue ("finish", "Finish time", finishTime);
  // cmd.AddValue ("netanim", "NetAnim filename", animationFile);
  cmd.AddValue ("strategy", "CCNx forwarding strategy", strategy);
  cmd.Parse (argc, argv);
    
    
  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
  AnnotatedTopologyReader reader;
  reader.SetFileName (input);

  NodeContainer nodes = reader.Read ();

  if (reader.LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }

  // ------------------------------------------------------------
  // -- Create nodes and network stacks
  // --------------------------------------------
  NS_LOG_INFO ("creating internet stack");
  InternetStackHelper stack;
    
  Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  stack.SetRoutingHelper (ipv4RoutingHelper);
  stack.Install(nodes);
    
  NS_LOG_INFO ("Assigning IPv4 addresses");
  reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0")); // will assign metrics
     
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  
  NS_LOG_INFO("installing ccnx stack");
  CcnxStackHelper ccnx;
  ccnx.SetForwardingStrategy (strategy);
  ccnx.EnableLimits (false);
    
  Ptr<CcnxFaceContainer> cf = ccnx.Install (nodes);
     
  NS_LOG_INFO ("Installing Applications");
  CcnxAppHelper helper ("ns3::CcnxConsumerCbr");
  helper.SetPrefix ("/3");
  ApplicationContainer app = helper.Install ("1");
  app.Start (Seconds (1.0));
  app.Stop (Seconds (1000.05));

  CcnxAppHelper helper2 ("ns3::CcnxProducer");
  helper2.SetPrefix ("/3");
  helper2.SetAttribute ("PayloadSize", StringValue("1024"));
  ApplicationContainer app2 = helper2.Install("3");

  app2.Start(Seconds(0.0));
  app2.Stop(Seconds(1500.0));
    
  // ------------------------------------------------------------
  // -- Run the simulation
  // --------------------------------------------
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (20));
  Simulator::Run ();
  Simulator::Destroy ();
    
  NS_LOG_INFO ("Done.");
    
  return 0;
}
