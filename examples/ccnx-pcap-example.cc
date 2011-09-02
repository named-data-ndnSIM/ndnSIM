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
//#include "ns3/visualizer-module.h"
#include "ns3/ccnx-consumer-helper.h"
#include "ns3/ccnx-stack-helper.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("CcnxPcapExample");

int main (int argc, char *argv[])
{
    Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("10Mbps"));
    Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("1ms"));
    
    NodeContainer c;
    c.Create (2);
    
    NodeContainer n1 = NodeContainer (c.Get (0), c.Get (1));
    
    PointToPointHelper p2p;
    NetDeviceContainer ndc1 = p2p.Install (n1);
    
//    CcnxStackHelper ccnx;
//    Ptr<CcnxFaceContainer> cf = ccnx.Install (c);

    //Ipv4AddressHelper ipv4;
    //ipv4.SetBase ("192.168.1.0", "255.255.255.0");
    //Ipv4InterfaceContainer i1 = ipv4.Assign (ndc1);
    
    CcnxInterestHeader header;
    Ptr<CcnxNameComponents> testname = Create<CcnxNameComponents> ();
    (*testname) ("test") ("test2");
    header.SetName (testname);
    header.SetNonce (1);

    Packet packet (0);
    packet.AddHeader (header);
    
    Ptr<Node> nd = c.Get(0);
    Ptr<PointToPointNetDevice> device = nd->GetDevice(nd->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
    
   
    
    Ptr<CcnxNameComponents> testname2 = Create<CcnxNameComponents> ();
    (*testname2) ("test3") ("test4");
    header.SetName (testname2);
    header.SetNonce (2);
    Packet packet2 (0);
    packet2.AddHeader (header);
    
    
    p2p.EnablePcapAll("pcap-example.pcap");
    
    Simulator::Stop (Seconds (20));
    
    //NS_LOG_INFO ("Run Simulation.");
    NS_LOG_INFO("Size1 = " << packet.GetSize());
    NS_LOG_INFO("Size2 = " << packet2.GetSize());
    device->Send(packet.Copy(),device->GetBroadcast(),0x7777);
    NS_LOG_INFO(device->IsLinkUp());
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    NS_LOG_INFO(device->IsLinkUp());
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    NS_LOG_INFO(device->IsLinkUp());
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    device->Send(packet2.Copy(),device->GetBroadcast(),0x7777);
    NS_LOG_INFO ("Run Simulation.");
    Simulator::Run ();
    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");
}
