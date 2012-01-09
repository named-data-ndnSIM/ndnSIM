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
#include "ns3/rocketfuel-topology-reader.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("CcnxSprintTopology");

void PrintTime ()
{
  NS_LOG_INFO (Simulator::Now ());

  Simulator::Schedule (Seconds (1.0), PrintTime);
}

int 
main (int argc, char *argv[])
{
  // Packet::EnableChecking();
  // Packet::EnablePrinting();
  string weights ("./src/NDNabstraction/examples/sprint.weights");
  string latencies ("./src/NDNabstraction/examples/sprint.latencies");
  string positions;
    
  Time finishTime = Seconds (2.0);
  string animationFile;
  string strategy = "ns3::CcnxFloodingStrategy";
  string save;
  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.AddValue ("netanim", "NetAnim filename", animationFile);
  cmd.AddValue ("strategy", "CCNx forwarding strategy", strategy);
  cmd.AddValue ("weights", "Weights file", weights);
  cmd.AddValue ("latencies", "Latencies file", latencies);
  cmd.AddValue ("positions", "Positions files", positions);
  cmd.AddValue ("save", "Save positions to a file", save);
  cmd.Parse (argc, argv);

  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
    
  RocketfuelWeightsReader reader ("/sprint");
  reader.SetBoundingBox (0, 0, 400, 250);

  if (!positions.empty())
    {
      reader.SetFileName (positions);
      reader.SetFileType (RocketfuelWeightsReader::POSITIONS);
      reader.Read ();
    }
  
  reader.SetFileName (weights);
  reader.SetFileType (RocketfuelWeightsReader::WEIGHTS);    
  reader.Read ();

  reader.SetFileName (latencies);
  reader.SetFileType (RocketfuelWeightsReader::LATENCIES);    
  reader.Read ();
    
  reader.Commit ();
  if (reader.LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }
    
  NS_LOG_INFO("Nodes = " << reader.GetNodes ().GetN());
  NS_LOG_INFO("Links = " << reader.LinksSize ());
    
  InternetStackHelper stack;
  Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  stack.SetRoutingHelper (ipv4RoutingHelper);
  stack.Install (reader.GetNodes ());

  reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));

  // Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetForwardingStrategy (strategy);
  ccnxHelper.EnableLimits (false, Seconds(0.1));
  ccnxHelper.SetDefaultRoutes (true);
  ccnxHelper.InstallAll ();

  // // Populate FIB based on IPv4 global routing controller
  // ccnxHelper.InstallFakeGlobalRoutes ();
  // ccnxHelper.InstallRouteTo (Names::Find<Node> ("/sprint", "San+Jose,+CA4062"));

  Simulator::Stop (finishTime);
  Simulator::Schedule (Seconds (1.0), PrintTime);

  // reader.SavePositions (save+".debug");

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  if (!save.empty ())
    {
      NS_LOG_INFO ("Saving positions.");
      reader.SavePositions (save);
    }
  
  return 0;
}
