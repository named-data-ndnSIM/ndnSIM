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
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-grid.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ndn.Grid");

/**
 * This scenario simulates a grid topology (using PointToPointGrid module)
 *
 * (consumer) -- ( ) ----- ( )
 *     |          |         |
 *    ( ) ------ ( ) ----- ( )
 *     |          |         |
 *    ( ) ------ ( ) -- (producer)
 *
 * Grid size could be specified using --nGrid parameter (default 3)
 *
 * All links are 1Mbps with propagation 10ms delay. 
 *
 * FIB is populated using NdnGlobalRoutingHelper.
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * Simulation time is 20 seconds, unless --finish parameter is specified
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=NdnSimple:NdnConsumer ./waf --run=ndn-grid
 */


int 
main (int argc, char *argv[])
{
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));
  
  uint32_t nGrid = 3;
  Time finishTime = Seconds (20.0); 

  CommandLine cmd;
  cmd.AddValue ("nGrid", "Number of grid nodes", nGrid);
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.Parse (argc, argv);

  PointToPointHelper p2p;

  PointToPointGridHelper grid (nGrid, nGrid, p2p);
  grid.BoundingBox(100,100,200,200);

  // Install NDN stack on all nodes
  NS_LOG_INFO ("Installing Ndn stack on all nodes");
  ndn::StackHelper ndnHelper;
  ndnHelper.SetContentStore ("ns3::ndn::cs::Lru", "MaxSize", "10");
  // ndnHelper.SetContentStore ("ns3::ndn::cs::Random", "MaxSize", "10");
  ndnHelper.InstallAll ();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll ();
  
  // Getting containers for the consumer/producer
  Ptr<Node> producer = grid.GetNode (nGrid-1, nGrid-1);
  NodeContainer consumerNodes;
  consumerNodes.Add (grid.GetNode (0,0));
  
  // Install Ndn applications
  NS_LOG_INFO ("Installing Applications");
  std::string prefix = "/prefix";
  
  ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
  consumerHelper.SetPrefix (prefix);
  consumerHelper.SetAttribute ("Frequency", StringValue ("100")); // 10 interests a second
  ApplicationContainer consumers = consumerHelper.Install (consumerNodes);
  
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  producerHelper.SetPrefix (prefix);
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  ApplicationContainer producers = producerHelper.Install (producer);

  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins (prefix, producer);

  // Calculate and install FIBs
  ndnGlobalRoutingHelper.CalculateRoutes ();
  
  Simulator::Stop (finishTime);
    
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      std::cout << "Node #" << (*node)->GetId () << std::endl;
      (*node)->GetObject<ndn::ContentStore> ()->Print (std::cout);
      std::cout << std::endl;
    }
  
  Simulator::Destroy ();
  NS_LOG_INFO ("Done!");
    
  return 0;
}
