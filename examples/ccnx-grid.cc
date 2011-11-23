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

#include <iostream>
#include <sstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CcnxGrid");

uint32_t nGrid = 3;
Time finishTime = Seconds (20.0); 

void PrintTime ()
{
  NS_LOG_INFO (Simulator::Now ());

  Simulator::Schedule (Seconds (10.0), PrintTime);
}

int 
main (int argc, char *argv[])
{
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("1ms"));
  Config::SetDefault ("ns3::CcnxConsumer::OffTime", StringValue ("1s"));
    
  Packet::EnableChecking();
  Packet::EnablePrinting();

  CommandLine cmd;
  cmd.AddValue ("nGrid", "Number of grid nodes", nGrid);
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.Parse (argc, argv);

  PointToPointHelper p2p;

  InternetStackHelper stack;
  Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  stack.SetRoutingHelper (ipv4RoutingHelper);
    
  PointToPointGridHelper grid (nGrid, nGrid, p2p);
  grid.BoundingBox(100,100,200,200);

  // Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper(Ccnx::NDN_FLOODING/*Ccnx::NDN_BESTROUTE*/);
  ccnxHelper.InstallAll ();

  // Install IP stack (necessary to populate FIB)
  NS_LOG_INFO ("Installing IP stack");
  grid.InstallStack (stack);
  grid.AssignIpv4Addresses (
                            Ipv4AddressHelper("10.1.0.0", "255.255.255.0"),
                            Ipv4AddressHelper("10.2.0.0", "255.255.255.0")
                            );

  Ptr<Node> producer = grid.GetNode (nGrid-1, nGrid-1);
  NodeContainer consumerNodes;
  consumerNodes.Add (grid.GetNode (0,0));
  
  // Populate FIB based on IPv4 global routing controller
  ccnxHelper.InstallFakeGlobalRoutes ();
  ccnxHelper.InstallRouteTo (producer);

  NS_LOG_INFO ("Installing Applications");
  std::ostringstream prefix;
  prefix << "/" << producer->GetId ();
  
  CcnxConsumerHelper consumerHelper (prefix.str ());
  ApplicationContainer consumers = consumerHelper.Install (consumerNodes);
  
  consumers.Start (Seconds (0.0));
  consumers.Stop (finishTime);
    
  CcnxProducerHelper producerHelper (prefix.str (),120);
  ApplicationContainer producers = producerHelper.Install (producer);
  
  producers.Start(Seconds(0.0));
  producers.Stop(finishTime);

  NS_LOG_INFO ("Outputing FIBs into [fibs.log]");
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("fibs.log", std::ios::out);
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      *routingStream->GetStream () << "Node " << (*node)->GetId () << "\n";

      Ptr<CcnxFib> fib = (*node)->GetObject<CcnxFib> ();
      NS_ASSERT_MSG (fib != 0, "Fire alarm");
      *routingStream->GetStream () << *fib << "\n\n";
    }

  Simulator::Schedule (Seconds (10.0), PrintTime);
  
  // NS_LOG_INFO ("FIB dump:\n" << *c.Get(0)->GetObject<CcnxFib> ());
  // NS_LOG_INFO ("FIB dump:\n" << *c.Get(1)->GetObject<CcnxFib> ());
    
  Simulator::Stop (finishTime);
    
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done!");
    
  return 0;
}
