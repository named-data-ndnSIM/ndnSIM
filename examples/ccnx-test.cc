/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/NDNabstraction-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CcnxTest");

int 
main (int argc, char *argv[])
{
  LogComponentEnable ("CcnxTest", LOG_ALL);
  LogComponentEnable ("CcnxStackHelper", LOG_ALL);
  LogComponentEnable ("CcnxRit", LOG_ALL);
  
  // Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
  // Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));

  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("10Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("1ms"));
  
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  // Here, we will explicitly create seven nodes. 
  NodeContainer c;
  c.Create (3);
  
  NodeContainer n1 = NodeContainer (c.Get (0), c.Get (1));
  NodeContainer n2 = NodeContainer (c.Get (1), c.Get (2));
  
  // Ipv4StaticRoutingHelper staticRouting;
  
  // Ipv4ListRoutingHelper list;
  // list.Add (staticRouting, 1);

  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  // NetDeviceContainer nd =
  p2p.Install (n1);
  p2p.Install (n2);

  NS_LOG_INFO ("Installing NDN stack");
  CcnxStackHelper ccnx;
  Ptr<CcnxFaceContainer> cf = ccnx.Install (c);

  // test RIT
  NS_LOG_INFO ("Creating RIT");
  Ptr<CcnxRit> rit = CreateObject<CcnxRit> ();

  CcnxInterestHeader header;
  Ptr<Name::Components> testname = Create<Name::Components> ();
  (*testname) ("test") ("test2");
  header.SetName (testname);
  header.SetNonce (1);

  rit->SetRecentlySatisfied (header);

  NS_LOG_INFO (rit->WasRecentlySatisfied (header));

  (*testname) ("test3"); // should have a side effect of changing name in the packet
  rit->SetRecentlySatisfied (header);

  NS_LOG_INFO (rit->WasRecentlySatisfied (header));

  header.SetNonce (2);
  NS_LOG_INFO (rit->WasRecentlySatisfied (header));

  // rit->SetRecentlySatisfied (header);
  // ? set up forwarding
  
  
  //Add static routing
  // InternetStackHelper internet;
  // internet.SetRoutingHelper (list); // has effect on the next Install ()
  // internet.Install (c);
  
  // Create the OnOff application to send UDP datagrams of size
  // 210 bytes at a rate of 448 Kb/s from n0 to n4
  // NS_LOG_INFO ("Create Applications.");
  
  // std::string sendsizeattr = "SendSize";
  // //flow2 7-->2
  // BulkSendHelper bulksend0 ("ns3::CcnxLocalFaceFactory", InetSocketAddress (i23.GetAddress (0), port));
  // //bulksend0.SetAttribute(sendsizeattr, AttributeValue(ConstantVariable(2560)));
  // bulksend0.SetAttribute("MaxBytes", UintegerValue(2560));
  // ApplicationContainer apps = bulksend0.Install(c.Get(6));
  // apps.Start(Seconds (1.0));
  // apps.Stop(Seconds (10.0));

  // AsciiTraceHelper ascii;
  // p2p.EnableAsciiAll (ascii.CreateFileStream ("ccnx-test.tr"));
  // p2p.EnablePcapAll ("ccnx-test");
  
  Simulator::Stop (Seconds (20));
  
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  
  return 0;
}
