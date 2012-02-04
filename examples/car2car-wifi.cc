#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/NDNabstraction-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int 
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc,argv);
  NodeContainer producerNode;
  producerNode.Create (1);
  NodeContainer consumerNodes;
  consumerNodes.Create(2);
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());
  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();
  /*Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));*/
  mac.SetType("ns3::AdhocWifiMac");
  NetDeviceContainer producerDevices;
  producerDevices = wifi.Install (phy, mac, producerNode);
  /*mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));
  */
  NetDeviceContainer consumerDevices;
  consumerDevices = wifi.Install (phy, mac, consumerNodes);

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::HighwayPositionAllocator",
				 "Start", VectorValue(Vector(0.0, 0.0, 0.0)),
				 "Direction", DoubleValue(0.0),
				 "Length", DoubleValue(1000.0));
  
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(26.8224, 0, 0)));
  mobility.Install (producerNode);

  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(26.8224, 0, 0)));
  mobility.Install (consumerNodes);


  // 1. Set RoutingHelper to support prefix
  //InternetStackHelper stack;
  //Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  //stack.SetRoutingHelper (ipv4RoutingHelper);

  // 2. Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  std::string strategy = "ns3::CcnxFloodingStrategy";
  ccnxHelper.SetForwardingStrategy (strategy);
  ccnxHelper.SetDefaultRoutes(true);
  //ccnxHelper.EnableLimits (false, Seconds(0.1));
  ccnxHelper.InstallAll ();

  // 3. Install IP stack
  //stack.Install (consumerNode);
  //stack.Install (producerNode);

  // 4. Assign Addresses
  //Ipv4AddressHelper address;
  //address.SetBase ("10.1.3.0", "255.255.255.0");
  //address.Assign (consumerDevices);
  //Ipv4InterfaceContainer producerInterface = address.Assign (producerDevices);

  // 5. Populate FIB based on IPv4 global routing controller
  //ccnxHelper.InstallFakeGlobalRoutes ();
  //ccnxHelper.InstallRouteTo (producerNode.Get(0));

  // 6. Set up applications
  NS_LOG_INFO ("Installing Applications");
  std::ostringstream prefix;
  prefix << "/" << producerNode.Get(0)->GetId ();
  
  CcnxAppHelper consumerHelper ("ns3::CcnxConsumer");
  consumerHelper.SetPrefix (prefix.str ());
  consumerHelper.SetAttribute ("MeanRate", StringValue ("1Mbps"));
  ApplicationContainer consumers = consumerHelper.Install (consumerNodes);
  
  // consumers.Start (Seconds (0.0));
  // consumers.Stop (finishTime);
    
  CcnxAppHelper producerHelper ("ns3::CcnxProducer");
  producerHelper.SetPrefix (prefix.str ());
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  ApplicationContainer producers = producerHelper.Install (producerNode);

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
