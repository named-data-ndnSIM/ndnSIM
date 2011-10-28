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
  // LogComponentEnable ("CcnxTest", LOG_ALL);
  // LogComponentEnable ("CcnxStackHelper", LOG_ALL);
  // LogComponentEnable ("CcnxRit", LOG_ALL);
  
  // Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (210));
  // Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("448kb/s"));

  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("10Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("1ms"));

  Packet::EnableChecking();
  Packet::EnablePrinting();
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  // Here, we will explicitly create seven nodes. 
  NodeContainer c;
  c.Create (3);
  Names::Add ("1", c.Get (0));
  Names::Add ("2", c.Get (1));
  Names::Add ("3", c.Get (2));
  
  // NodeContainer n1 = NodeContainer ("1") ("2");
  // NodeContainer n2 = NodeContainer ("2") ("3");
  
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  p2p.Install ("1", "2");
  p2p.Install ("2", "3");

  NS_LOG_INFO ("Installing NDN stack");
  CcnxStackHelper ccnx;
  Ptr<CcnxFaceContainer> cf = ccnx.Install (c);

  CcnxConsumerHelper helper ("/3");
  ApplicationContainer app = helper.Install ("1");
  app.Start (Seconds (1.0));
  app.Stop (Seconds (10.05));
    
  CcnxProducerHelper helper2 ("/3",120);
  ApplicationContainer app2 = helper2.Install("3");
  app2.Start(Seconds(0.0));
  app2.Stop(Seconds(15.0));
  
  /**
   * \brief Add forwarding entry in FIB
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param face Face index
   * \param metric Routing metric
   */
  ccnx.AddRoute ("1", "/3", 0, 1);
  ccnx.AddRoute ("2", "/3", 1, 1);
  ccnx.AddRoute ("2", "/3", 0, 10000);
  NS_LOG_INFO ("FIB dump:\n" << *c.Get(0)->GetObject<CcnxFib> ());
  NS_LOG_INFO ("FIB dump:\n" << *c.Get(1)->GetObject<CcnxFib> ());

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
