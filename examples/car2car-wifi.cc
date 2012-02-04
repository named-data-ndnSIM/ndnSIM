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


  
  // 2. Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes(true);
  ccnxHelper.InstallAll ();

  // 6. Set up applications
  NS_LOG_INFO ("Installing Applications");
  
  CcnxAppHelper consumerHelper ("ns3::CcnxConsumerCbr");
  consumerHelper.SetPrefix ("/");
  consumerHelper.SetAttribute ("Frequency", StringValue ("1"));
  ApplicationContainer consumers = consumerHelper.Install (consumerNodes);
  
  // consumers.Start (Seconds (0.0));
  // consumers.Stop (finishTime);
    
  CcnxAppHelper producerHelper ("ns3::CcnxProducer");
  producerHelper.SetPrefix ("/");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  ApplicationContainer producers = producerHelper.Install (producerNode);

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
