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
    
  CommandLine cmd;
  cmd.Parse (argc, argv);
    
  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
    
  Ptr<AnnotatedTopologyReader> reader = CreateObject<AnnotatedTopologyReader> ();
  reader->SetFileName (input);
    
  NodeContainer nodes;
  if (reader != 0)
    {
      nodes = reader->Read ();
    }
    
  if (reader->LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }

  NS_LOG_INFO("Nodes = " << nodes.GetN());
  NS_LOG_INFO("Links = " << reader->LinksSize ());
    
  int totlinks = reader->LinksSize ();
  ///*** applying settings
  NS_LOG_INFO ("creating node containers");
  NodeContainer* nc = new NodeContainer[totlinks];
  TopologyReader::ConstLinksIterator iter;
  int i = 0;
  for ( iter = reader->LinksBegin (); iter != reader->LinksEnd (); iter++, i++ )
    {
      nc[i] = NodeContainer (iter->GetFromNode (), iter->GetToNode ());
    }
    
  NetDeviceContainer* ndc = new NetDeviceContainer[totlinks];
  reader->ApplySettings(ndc,nc);
  reader->BoundingBox(nc, 100.0, 100.0, 400.0,400.0);
    
  // Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  // ccnxHelper.SetForwardingStrategy (strategy);
  ccnxHelper.EnableLimits (true, Seconds(0.1));
  ccnxHelper.InstallAll ();
    
  NS_LOG_INFO ("Installing Applications");
  CcnxConsumerHelper consumerHelper ("preved");
  ApplicationContainer consumers = consumerHelper.Install (nc[0]);
    
  consumers.Start (Seconds (0));
  consumers.Stop (Seconds (20));

  Simulator::Stop (Seconds (20));
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}
