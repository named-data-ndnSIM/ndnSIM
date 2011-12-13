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
#include "ns3/annotated-topology-reader.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("CcnxAbileneTopology");

int 
main (int argc, char *argv[])
{
  // Packet::EnableChecking();
  // Packet::EnablePrinting();
  string input ("./src/NDNabstraction/examples/abilene-topology.txt");

  Time finishTime = Seconds (20.0);
  string animationFile;
  string strategy = "ns3::CcnxFloodingStrategy";
  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.AddValue ("netanim", "NetAnim filename", animationFile);
  cmd.AddValue ("strategy", "CCNx forwarding strategy", strategy);
  cmd.Parse (argc, argv);
    
  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
    
  AnnotatedTopologyReader reader;
  reader.SetFileName (input);
  reader.SetBoundingBox (100.0, 100.0, 400.0, 400.0);
  
  NodeContainer nodes = reader.Read ();
    
  if (reader.LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }

  NS_LOG_INFO("Nodes = " << nodes.GetN());
  NS_LOG_INFO("Links = " << reader.LinksSize ());
    
  // Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  // ccnxHelper.SetForwardingStrategy (strategy);
  ccnxHelper.EnableLimits (true, Seconds(0.1));
  ccnxHelper.InstallAll ();
    
  // NS_LOG_INFO ("Installing Applications");
  // CcnxConsumerHelper consumerHelper ("preved");
  // ApplicationContainer consumers = consumerHelper.Install (nc[0]);
    
  Simulator::Stop (finishTime);
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}
