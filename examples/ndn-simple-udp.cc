/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */
// ndn-simple-tcp.cc
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *           \                                                   /
 *            -------------------- udp face --------------------
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple-udp
 */

int 
main (int argc, char *argv[])
{
  Packet::EnablePrinting ();
  
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create (3);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  NetDeviceContainer link1 = p2p.Install (nodes.Get (0), nodes.Get (1));
  NetDeviceContainer link2 = p2p.Install (nodes.Get (1), nodes.Get (2));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
  ndnHelper.SetDefaultRoutes (false);
  ndnHelper.InstallAll ();

  InternetStackHelper ipStack;
  ipStack.SetIpv6StackInstall (false);
  ipStack.InstallAll ();

  Ipv4AddressHelper ipAddressHelper;
  ipAddressHelper.SetBase (Ipv4Address ("10.1.1.0"), Ipv4Mask ("255.255.255.0"));
  ipAddressHelper.Assign (link1);
  
  ipAddressHelper.SetBase (Ipv4Address ("10.1.2.0"), Ipv4Mask ("255.255.255.0"));
  ipAddressHelper.Assign (link2);

  Ipv4StaticRoutingHelper ipStaticRouting;
  ipStaticRouting.GetStaticRouting (nodes.Get (0)->GetObject<Ipv4> ())->
    AddNetworkRouteTo (Ipv4Address ("10.1.2.0"), Ipv4Mask ("255.255.255.0"),
                       Ipv4Address ("10.1.1.2"),
                       1, 1);

  ipStaticRouting.GetStaticRouting (nodes.Get (2)->GetObject<Ipv4> ())->
    AddNetworkRouteTo (Ipv4Address ("10.1.1.0"), Ipv4Mask ("255.255.255.0"),
                       Ipv4Address ("10.1.2.1"),
                       1, 1);
  
  ndn::IpFacesHelper::InstallAll ();
  ndn::IpFacesHelper::CreateUdpFace (Seconds (1.0), nodes.Get (0), Ipv4Address ("10.1.2.2"), "/udp-route1");
  ndn::IpFacesHelper::CreateUdpFace (Seconds (1.0), nodes.Get (0), Ipv4Address ("10.1.1.2"), "/udp-route2");
  
  // Installing applications

  // Consumer
  ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
  // consumerHelper.SetAttribute ("Randomize", StringValue ("uniform"));
  // Consumer will request /udp-route1/0, /udp-route1/1, ...
  consumerHelper.SetPrefix ("/udp-route1");
  consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
  consumerHelper.Install (nodes.Get (0)).
    Start (Seconds (3)); // first node

  // Consumer will request /udp-route1/0, /udp-route1/1, ...
  consumerHelper.SetPrefix ("/udp-route2");
  consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
  consumerHelper.Install (nodes.Get (0)).
    Start (Seconds (3)); // first node
  
  // Producer
  ndn::AppHelper producerHelper ("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerHelper.SetPrefix ("/udp-route1");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (nodes.Get (2)); // last node

  producerHelper.SetPrefix ("/udp-route2");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  producerHelper.Install (nodes.Get (1)); // last node

  Simulator::Stop (Seconds (20.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
