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
    Packet::EnableChecking();
    Packet::EnablePrinting();
    string input ("/Users/iliamo/ns3-abstract-ndn/ns-3.11/src/NDNabstraction/examples/simpletopology.txt");
    
    // Set up command line parameters used to control the experiment.
    //CommandLine cmd;
    //cmd.AddValue ("input", "Name of the input file.",
    //              input);
    //cmd.Parse (argc, argv);
    
    
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
    
    
    for(uint32_t j=0; j<nodes.GetN(); j++)
    {
        uint32_t name = j+1;
        std::stringstream ss;
        ss<<name;
        Names::Add (ss.str(), nodes.Get (j));
        NS_LOG_INFO("Name = " << ss.str());
    }
 
    // ------------------------------------------------------------
    // -- Create nodes and network stacks
    // --------------------------------------------
    NS_LOG_INFO ("creating internet stack");
    InternetStackHelper stack;
    
    
    //routing
    //Ipv4StaticRoutingHelper staticRouting;
    //Ipv4ListRoutingHelper listRH;
    //listRH.Add (staticRouting, 0);
    //stack.SetRoutingHelper (listRH);  // has effect on the next Install ()
    //stack.Install (nodes);
    
    Ipv4GlobalRoutingHelper ipv4RoutingHelper;
    // Ptr<Ipv4RoutingHelper> ipv4RoutingHelper = stack.GetRoutingHelper ();
    stack.SetRoutingHelper (ipv4RoutingHelper);
    stack.Install(nodes);
    
    NS_LOG_INFO ("creating ip4 addresses");
    Ipv4AddressHelper address;
    address.SetBase ("10.0.0.0", "255.255.255.252");
   
    // // Create router nodes, initialize routing database and set up the routing
    // // tables in the nodes.
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    
    /*grid.AssignIpv4Addresses (
                              Ipv4AddressHelper("10.1.0.0", "255.255.255.0"),
                              Ipv4AddressHelper("10.2.0.0", "255.255.255.0")
                              );
*/
    
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
    ///*** settings applied
    
    NS_LOG_INFO("installing ccnx stack");
    CcnxStackHelper ccnx(Ccnx::NDN_FLOODING);
    Ptr<CcnxFaceContainer> cf = ccnx.Install (nodes);
     
    NS_LOG_INFO ("Installing Applications");
    CcnxConsumerHelper helper ("/3");
    ApplicationContainer app = helper.Install (nodes.Get(1));
    app.Start (Seconds (1.0));
    app.Stop (Seconds (1000.05));
    
    /*CcnxConsumerHelper helper2 ("/4");
     ApplicationContainer app2 = helper2.Install(c.Get(5));
     app2.Start (Seconds (1.0));
     app2.Stop (Seconds (1000.05));
     */
    CcnxProducerHelper helper3 ("/3",120);
    ApplicationContainer app3 = helper3.Install(nodes.Get(6));
    app3.Start(Seconds(0.0));
    app3.Stop(Seconds(1500.0));
    /*
     CcnxProducerHelper helper4 ("/4",150);
     ApplicationContainer app4 = helper4.Install(c.Get(0));
     app4.Start(Seconds(0.0));
     app4.Stop(Seconds(1500.0));
     */

    NS_LOG_INFO("Routes");
    ccnx.AddRoute("1","/3",0,1);
    ccnx.AddRoute("3","/3",1,1);
    ccnx.AddRoute("3","/3",2,2);
    /*ccnx.AddRoute("4","/3",1,1);
    ccnx.AddRoute("5","/3",2,1);
*/
    
    // it creates little subnets, one for each couple of nodes.
    NS_LOG_INFO ("creating ipv4 interfaces");
    Ipv4InterfaceContainer* ipic = new Ipv4InterfaceContainer[totlinks];
    for (int i = 0; i < totlinks; i++)
    {
        ipic[i] = address.Assign (ndc[i]);
        address.NewNetwork ();
    }
    
    // ------------------------------------------------------------
    // -- Run the simulation
    // --------------------------------------------
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Stop (Seconds (20));
    Simulator::Run ();
    Simulator::Destroy ();
    
    delete[] ipic;
    delete[] ndc;
    delete[] nc;
    
    NS_LOG_INFO ("Done.");
    
    return 0;
}
