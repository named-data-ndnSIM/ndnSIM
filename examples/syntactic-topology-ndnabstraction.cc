/*
 *  sync-topology-ndnabstraction.cpp
 *  XcodeProject
 *
 *  Created by Ilya on 7/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SyncTopologyNDNabstraction");

int 
main (int argc, char *argv[])
{
  // Set up some default values for the simulation.  Use the 
  
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));
  
  // Allow the user to override any of the defaults and the above
  // DefaultValue::Bind ()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  // Here, we will explicitly create seven nodes. 
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (7);
  
  
  NodeContainer n13 = NodeContainer (c.Get (0), c.Get (2));
  NodeContainer n23 = NodeContainer (c.Get (1), c.Get (3));
  NodeContainer n35 = NodeContainer (c.Get (2), c.Get (4));
  NodeContainer n34 = NodeContainer (c.Get (2), c.Get (3));
  NodeContainer n45 = NodeContainer (c.Get (3), c.Get (4));
  NodeContainer n56 = NodeContainer (c.Get (4), c.Get (5));
  NodeContainer n57 = NodeContainer (c.Get (4), c.Get (6));
  
  Ipv4StaticRoutingHelper staticRouting;
  
  Ipv4ListRoutingHelper list;
  list.Add (staticRouting, 1);
  
  //Add static routing
  InternetStackHelper internet;
  internet.SetRoutingHelper (list); // has effect on the next Install ()
  internet.Install (c);
  
  // We create the channels first without any IP addressing information
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  NetDeviceContainer nd13 = p2p.Install (n13);
  NetDeviceContainer nd23 = p2p.Install (n23);
  NetDeviceContainer nd56 = p2p.Install (n56);
  
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("50ms"));
  NetDeviceContainer nd57 = p2p.Install (n57);
  
  p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  NetDeviceContainer nd34 = p2p.Install (n34);
  NetDeviceContainer nd45 = p2p.Install (n45);
  
  p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("50ms"));
  NetDeviceContainer nd35 = p2p.Install (n35);
  
  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i13 = ipv4.Assign (nd13);
  Ipv4InterfaceContainer i23 = ipv4.Assign (nd23);
  Ipv4InterfaceContainer i35 = ipv4.Assign (nd35);
  Ipv4InterfaceContainer i34 = ipv4.Assign (nd34);
  Ipv4InterfaceContainer i45 = ipv4.Assign (nd45);
  Ipv4InterfaceContainer i56 = ipv4.Assign (nd56);
  Ipv4InterfaceContainer i57 = ipv4.Assign (nd57);
  
  
  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s from n0 to n4
  NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)
  
  std::string sendsizeattr = "SendSize";
  //flow2 7-->2
  BulkSendHelper bulksend0 ("ns3::UdpSocketFactory", InetSocketAddress (i23.GetAddress (0), port));
  //bulksend0.SetAttribute(sendsizeattr, AttributeValue(ConstantVariable(2560)));
  bulksend0.SetAttribute("MaxBytes", UintegerValue(2560));
  ApplicationContainer apps = bulksend0.Install(c.Get(6));
  apps.Start(Seconds (1.0));
  apps.Stop(Seconds (10.0));
  
  // Create a packet sink to receive these packets
  PacketSinkHelper sink0 ("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny (), port));
  apps = sink0.Install(c.Get(1));
  apps.Start(Seconds(0.0));
  apps.Stop(Seconds(20.0));
  
  //flow1 1-->6
  BulkSendHelper bulksend ("ns3::UdpSocketFactory", InetSocketAddress (i56.GetAddress (1), port));
  //bulksend.SetAttribute(sendsizeattr, AttributeValue( ConstantVariable(2560)));
  bulksend0.SetAttribute("MaxBytes", UintegerValue(2560));
  apps = bulksend.Install (c.Get (0));
  apps.Start (Seconds (6.0));
  apps.Stop (Seconds (20.0));
  
  // Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  apps = sink.Install (c.Get (5));
  apps.Start(Seconds(0.0));
  apps.Stop(Seconds(20.0));
  
  AsciiTraceHelper ascii;
  p2p.EnableAsciiAll (ascii.CreateFileStream ("sync-topology-ndnabstraction.tr"));
  p2p.EnablePcapAll ("sync-topology-ndnabstraction");
  
  Simulator::Stop (Seconds (20));
  
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  
	return 0;
}
