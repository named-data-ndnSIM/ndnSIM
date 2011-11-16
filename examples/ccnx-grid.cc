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
#include <ns3/point-to-point-grid.h>
#include "ns3/ipv4-global-routing-helper.h"

#include <iostream>
#include <sstream>

#include "ns3/visualizer-module.h"
#include "ns3/ccnx.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CcnxGrid");

int 
main (int argc, char *argv[])
{
    GlobalValue::Bind ("SimulatorImplementationType", StringValue
                       ("ns3::VisualSimulatorImpl"));
    
    uint32_t n = 3;
    
    Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
    Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("1ms"));
    
    Packet::EnableChecking();
    Packet::EnablePrinting();
    CommandLine cmd;
    cmd.Parse (argc, argv);

    PointToPointHelper p2p;
    InternetStackHelper stack;
    
    Ipv4GlobalRoutingHelper ipv4RoutingHelper;
    // Ptr<Ipv4RoutingHelper> ipv4RoutingHelper = stack.GetRoutingHelper ();
    stack.SetRoutingHelper (ipv4RoutingHelper);
    
    PointToPointGridHelper grid (n, n, p2p);
    grid.BoundingBox(100,100,200,200);
    grid.InstallStack (stack);
    
    // // Create router nodes, initialize routing database and set up the routing
    // // tables in the nodes.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    
    grid.AssignIpv4Addresses (
                              Ipv4AddressHelper("10.1.0.0", "255.255.255.0"),
                              Ipv4AddressHelper("10.2.0.0", "255.255.255.0")
                              );
    
    NS_LOG_INFO ("Installing NDN stack");
    NodeContainer c;
    
    for(uint32_t i=0; i<n; i++)
    {
        for(uint32_t j=0; j<n; j++)
        {
            NS_LOG_INFO ("Adding node i="<<i << " j=" << j);
            c.Add(grid.GetNode(i,j));
            
            int nodeCount = i*n+j+1;
            std::stringstream ss;
            ss<<nodeCount;
            Names::Add (ss.str(), c.Get (c.GetN()-1));
            NS_LOG_INFO("Eventual name is " << ss.str());
        }
    }
    CcnxStackHelper ccnx(Ccnx::NDN_FLOODING/*Ccnx::NDN_BESTROUTE*/);
    Ptr<CcnxFaceContainer> cf = ccnx.Install (c);

    NS_LOG_INFO ("Installing Applications");
    CcnxConsumerHelper helper ("/3");
    ApplicationContainer app = helper.Install (grid.GetNode (0,0));
    app.Start (Seconds (1.0));
    app.Stop (Seconds (1000.05));
    
    CcnxProducerHelper helper2 ("/3",120);
    ApplicationContainer app2 = helper2.Install(grid.GetNode (2,2));
    app2.Start(Seconds(0.0));
    app2.Stop(Seconds(1500.0));
    
    /**
     * \brief Add forwarding entry in FIB
     *
     * \param node Node
     * \param prefix Routing prefix
     * \param face Face index
     * \param metric Routing metric
     */
    
    //2x2 works
    /*ccnx.AddRoute ("1", "/3", 0, 1);
    ccnx.AddRoute ("1", "/3", 1, 1);
    
    ccnx.AddRoute ("2", "/3", 1, 1);
    ccnx.AddRoute ("3", "/3", 1, 1);
    */
    
    //3x3
    
    ccnx.AddRoute ("1", "/3", 0, 1);
    ccnx.AddRoute ("1", "/3", 1, 1);
    
    ccnx.AddRoute ("2", "/3", 1, 1);
    
    ccnx.AddRoute ("3", "/3", 1, 1);
    
    ccnx.AddRoute ("4", "/3", 2, 1);
    
    ccnx.AddRoute ("6", "/3", 2, 1);
    
    ccnx.AddRoute ("7", "/3", 1, 1);
    
    ccnx.AddRoute ("8", "/3", 1, 1);
    
    
    NS_LOG_INFO ("FIB dump:\n" << *c.Get(0)->GetObject<CcnxFib> ());
    NS_LOG_INFO ("FIB dump:\n" << *c.Get(1)->GetObject<CcnxFib> ());
    
    
    
    Simulator::Stop (Seconds (2000));
    
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done!");
    
    return 0;

}