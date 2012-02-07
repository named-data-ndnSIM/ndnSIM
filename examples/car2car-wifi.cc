#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/NDNabstraction-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

static void
ExactTimePrinter (std::ostream &os)
{
  os << Simulator::Now ();
}


int 
main (int argc, char *argv[])
{
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue ("OfdmRate24Mbps"));

  // vanet hacks to CcnxL3Protocol
  Config::SetDefault ("ns3::CcnxL3Protocol::EnableNACKs", StringValue ("false"));
  Config::SetDefault ("ns3::CcnxL3Protocol::CacheUnsolicitedData", StringValue ("true"));

  CommandLine cmd;
  cmd.Parse (argc,argv);

  //////////////////////
  //////////////////////
  //////////////////////
  
  WifiHelper wifi = WifiHelper::Default ();
  // wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate24Mbps"));

  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();

  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType("ns3::AdhocWifiMac");

  //////////////////////
  //////////////////////
  //////////////////////

  
  NodeContainer producerNode;
  producerNode.Create (1);
  NodeContainer consumerNodes;
  consumerNodes.Create(2);

  wifi.Install (wifiPhy, wifiMac, producerNode);
  wifi.Install (wifiPhy, wifiMac, consumerNodes);

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::HighwayPositionAllocator",
				 "Start", VectorValue(Vector(0.0, 0.0, 0.0)),
				 "Direction", DoubleValue(0.0),
				 "Length", DoubleValue(1000.0));
  
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(0/*26.8224*/, 0, 0)));
  mobility.Install (producerNode);

  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(0/*26.8224*/, 0, 0)));
  mobility.Install (consumerNodes);

  
  // 2. Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes(true);
  ccnxHelper.InstallAll ();

  // 6. Set up applications
  NS_LOG_INFO ("Installing Applications");
  
  CcnxAppHelper consumerHelper ("ns3::CcnxConsumerCbr");
  consumerHelper.SetPrefix ("/very-long-prefix-requested-by-client/this-interest-hundred-bytes-long-");
  consumerHelper.SetAttribute ("Frequency", StringValue ("1"));
  consumerHelper.SetAttribute ("Randomize", StringValue ("exponential"));
  consumerHelper.Install (consumerNodes.Get (0));
  
  consumerHelper.Install (consumerNodes.Get (1));
  
  // consumers.Start (Seconds (0.0));
  // consumers.Stop (finishTime);
    
  CcnxAppHelper producerHelper ("ns3::CcnxProducer");
  producerHelper.SetPrefix ("/");
  producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
  ApplicationContainer producers = producerHelper.Install (producerNode);

  Simulator::Schedule (Seconds(0.0), LogSetTimePrinter, ExactTimePrinter);

  
  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
