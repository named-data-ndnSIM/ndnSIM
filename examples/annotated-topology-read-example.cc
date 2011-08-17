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
#include "ns3/visualizer-module.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("AnnotatedTopologyReadingExample");

int main (int argc, char *argv[])
{
    //Packet::EnablePrinting();
    GlobalValue::Bind ("SimulatorImplementationType", StringValue
                       ("ns3::VisualSimulatorImpl"));
    
    string input ("/Users/iliamo/ns3-abstract-ndn/ns-3.11/src/NDNabstraction/examples/simpletopology.txt");
    
    // Set up command line parameters used to control the experiment.
    //CommandLine cmd;
    //cmd.AddValue ("input", "Name of the input file.",
    //              input);
    //cmd.Parse (argc, argv);
    
    
    // ------------------------------------------------------------
    // -- Read topology data.
    // --------------------------------------------
    NodeContainer nodes;
    
    Ptr<AnnotatedTopologyReader> inFile = CreateObject<AnnotatedTopologyReader> ();
    inFile->SetFileName (input);
    
    if (inFile != 0)
    {
        nodes = inFile->Read ();
    }
    
    if (inFile->LinksSize () == 0)
    {
        NS_LOG_ERROR ("Problems reading the topology file. Failing.");
        return -1;
    }
    
    // ------------------------------------------------------------
    // -- Create nodes and network stacks
    // --------------------------------------------
    NS_LOG_INFO ("creating internet stack");
    InternetStackHelper stack;
    
    
    //routing
    Ipv4StaticRoutingHelper staticRouting;
    Ipv4ListRoutingHelper listRH;
    listRH.Add (staticRouting, 0);
    stack.SetRoutingHelper (listRH);  // has effect on the next Install ()
    stack.Install (nodes);
    
    NS_LOG_INFO ("creating ip4 addresses");
    Ipv4AddressHelper address;
    address.SetBase ("10.0.0.0", "255.255.255.252");
    
    int totlinks = inFile->LinksSize ();
    
    NS_LOG_INFO ("creating node containers");
    NodeContainer* nc = new NodeContainer[totlinks];
    TopologyReader::ConstLinksIterator iter;
    int i = 0;
    for ( iter = inFile->LinksBegin (); iter != inFile->LinksEnd (); iter++, i++ )
    {
        nc[i] = NodeContainer (iter->GetFromNode (), iter->GetToNode ());
    }
    
    NS_LOG_INFO ("creating net device containers");
    ObjectFactory m_queueFactory;
    m_queueFactory.SetTypeId("ns3::DropTailQueue");
    //m_queueFactory.Set("Mode",
    NetDeviceContainer* ndc = new NetDeviceContainer[totlinks];
    PointToPointHelper p2p;
    TopologyReader::ConstLinksIterator iter2;
    i = 0;
    for ( iter2 = inFile->LinksBegin (); iter2 != inFile->LinksEnd (); iter2++, i++ )
    {
        std::string dataRate = iter2->GetAttribute("DataRate");
        NS_LOG_INFO("dataRate = "<<dataRate);
        dataRate += "Kbps";
        std::string delay = iter2->GetAttribute("Delay");
        NS_LOG_INFO("delay = "<<delay);
        delay += "ms";
        p2p.SetDeviceAttribute("DataRate", StringValue(dataRate));
        p2p.SetChannelAttribute("Delay", StringValue(delay));
        p2p.SetQueue("ns3::DropTailQueue","MaxPackets",StringValue("100"));
        ndc[i] = p2p.Install(nc[i]);
        
        NodeContainer twoNodes = nc[i];
        
        
        Ptr<Node> nd = twoNodes.Get(0);
        if(nd==NULL)
            NS_LOG_INFO("nd = null");
        Ptr<Node> nd2 = twoNodes.Get(1);
        if(nd2==NULL)
            NS_LOG_INFO("nd2 = null");
        //NS_LOG_INFO("1");
        NS_LOG_INFO("#netdevices = " << nd->GetNDevices());
        NS_LOG_INFO("#netdevices = " << nd2->GetNDevices());

        Ptr<PointToPointNetDevice> device = nd->GetDevice(nd->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
        if(device==NULL)
            NS_LOG_INFO("device = 0");
        
        //NS_LOG_INFO("2");
        
        Ptr<PointToPointNetDevice> device2 = nd2->GetDevice(nd2->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
`
        if(device2==NULL)
            NS_LOG_INFO("device2 = 0");
        
        PointerValue tmp1;
        device->GetAttribute ("TxQueue", tmp1);
        //NS_LOG_INFO("2.5");
        Ptr<Object> txQueue1 = tmp1.GetObject ();
        
        PointerValue tmp2;
        device2->GetAttribute ("TxQueue", tmp2);
        Ptr<Object> txQueue2 = tmp2.GetObject ();
        //NS_LOG_INFO("3");
        Ptr<DropTailQueue> dtq1 = txQueue1->GetObject <DropTailQueue> ();
        NS_ASSERT (dtq1 != 0);
        
        Ptr<DropTailQueue> dtq2 = txQueue2->GetObject <DropTailQueue> ();
        NS_ASSERT (dtq2 != 0);
        
        std::string queuesize1 = iter2->GetAttribute("QueueSizeNode1");
        std::string queuesize2 = iter2->GetAttribute("QueueSizeNode2");
        //NS_LOG_INFO("4");
        txQueue1->SetAttribute("MaxPackets", UintegerValue (atoi(queuesize1.c_str())));
        txQueue2->SetAttribute("MaxPackets", UintegerValue (atoi(queuesize2.c_str())));
        
        UintegerValue limit;
        txQueue1->GetAttribute ("MaxPackets", limit);
        NS_LOG_INFO ("txQueue1 limit changed: " << limit.Get () << " packets");
        
        txQueue2->GetAttribute ("MaxPackets", limit);
        NS_LOG_INFO ("txQueue2 limit changed: " << limit.Get () << " packets");
        
    
        /* //bauman way of doing things
        m_queueFactory.Set("MaxPackets",StringValue(iter2->GetAttribute("QueueSizeNode1")));
        NS_LOG_INFO("2.5");
        Ptr<DropTailQueue> queue = m_queueFactory.Create<DropTailQueue>();
        queue->SetMode(ns3::DropTailQueue::PACKETS);
        NS_LOG_INFO("2,8");
        device->SetQueue(queue);
        NS_LOG_INFO("3");
        m_queueFactory.Set("MaxPackets", StringValue(iter2->GetAttribute("QueueSizeNode2")));
        
        Ptr<DropTailQueue> queue2 = m_queueFactory.Create<DropTailQueue>();
        queue2->SetMode(ns3::DropTailQueue::PACKETS);
        device2->SetQueue(queue2);
        */
    }
    
    // it creates little subnets, one for each couple of nodes.
    NS_LOG_INFO ("creating ipv4 interfaces");
    Ipv4InterfaceContainer* ipic = new Ipv4InterfaceContainer[totlinks];
    for (int i = 0; i < totlinks; i++)
    {
        ipic[i] = address.Assign (ndc[i]);
        address.NewNetwork ();
    }
    
    
    
    
    
    
    /*
    NS_LOG_INFO ("Create Applications.");
    uint16_t port = 9;   // Discard port (RFC 863)
    
    std::string sendsizeattr = "SendSize";
    //flow2 7-->2
    BulkSendHelper bulksend0 ("ns3::TcpSocketFactory", InetSocketAddress (ipic[1].GetAddress (0), port));
    //bulksend0.SetAttribute(sendsizeattr, AttributeValue(ConstantVariable(2560)));
    bulksend0.SetAttribute("MaxBytes", UintegerValue(2560));
    ApplicationContainer apps = bulksend0.Install(nc[6]);
    apps.Start(Seconds (1.0));
    apps.Stop(Seconds (10.0));
    
    // Create a packet sink to receive these packets
    PacketSinkHelper sink0 ("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny (), port));
    apps = sink0.Install(nc[1]);
    apps.Start(Seconds(0.0));
    apps.Stop(Seconds(20.0));
    
    //flow1 1-->6
    BulkSendHelper bulksend ("ns3::TcpSocketFactory", InetSocketAddress (ipic[5].GetAddress (1), port));
    //bulksend.SetAttribute(sendsizeattr, AttributeValue( ConstantVariable(2560)));
    bulksend0.SetAttribute("MaxBytes", UintegerValue(2560));
    apps = bulksend.Install (nc[0]);
    apps.Start (Seconds (6.0));
    apps.Stop (Seconds (20.0));
    
    // Create a packet sink to receive these packets
    PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
    apps = sink.Install (nc[5]);
    apps.Start(Seconds(0.0));
    apps.Stop(Seconds(20.0));
    
    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll (ascii.CreateFileStream ("annotated-topology-read.tr"));
    p2p.EnablePcapAll ("annotated-topology-read");
    
    */
    
    
    
    
    
    
    
    
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
    
    // end main
}
