#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/NDNabstraction-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

void SetupHighway(MobilityHelper mobility, WifiHelper& wifi, YansWifiPhyHelper& wifiPhy, NqosWifiMacHelper wifiMac){

  //////////////////////
  //////////////////////
  //////////////////////
  static int highway_run = 1;
  
  NodeContainer producerNode;
  producerNode.Create (1);
  
  NodeContainer consumerNodes;
  //if(highway_run == 2)
  consumerNodes.Create(3);

  wifi.Install (wifiPhy, wifiMac, producerNode);
  wifi.Install (wifiPhy, wifiMac, consumerNodes);

  mobility.Install (producerNode);
  mobility.Install (consumerNodes);

  // 2. Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes(true);
  ccnxHelper.Install(producerNode);
  ccnxHelper.Install(consumerNodes);

  // 6. Set up applications
  NS_LOG_INFO ("Installing Applications");
  
  CcnxAppHelper consumerHelper ("ns3::CcnxConsumerCbr");
  consumerHelper.SetPrefix ("/very-long-prefix-requested-by-client/this-interest-hundred-bytes-long-");
  consumerHelper.SetAttribute ("Frequency", StringValue ("1"));
  consumerHelper.SetAttribute ("Randomize", StringValue ("exponential"));
  ApplicationContainer consumers = consumerHelper.Install (consumerNodes);
  

  if(highway_run == 2){ // there is only one producer in the scenario on the second highway 
    CcnxAppHelper producerHelper ("ns3::CcnxProducer");
    producerHelper.SetPrefix ("/");
    producerHelper.SetAttribute ("PayloadSize", StringValue("300"));
    ApplicationContainer producers = producerHelper.Install (producerNode);
  }
  highway_run++;
}

// static void
// ExactTimePrinter (std::ostream &os)
// {
//   os << Simulator::Now ();
// }

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


  // Setup the first highway going in the direction of left -> right
  //bool static_cars = true;
  
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::HighwayPositionAllocator",
				 "Start", VectorValue(Vector(0.0, 0.0, 0.0)),
				 "Direction", DoubleValue(0.0),
				 "Length", DoubleValue(1000.0),
				 "MinGap", DoubleValue(80.0),
				 "MaxGap", DoubleValue(80.0));
  
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(0/*26.8224*/, 0, 0)));

  SetupHighway(mobility, wifi, wifiPhy, wifiMac);
  /*26.8224*/
  // Set up the second highway going in the direction of left <- right
  /*mobility.SetPositionAllocator ("ns3::HighwayPositionAllocator",
				 "Start", VectorValue(Vector(1000.0, 5.0, 0.0)),
				 "Direction", DoubleValue(0.0),
				 "Length", DoubleValue(1000.0));
  
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(-26.8224, 0, 0)));

  SetupHighway(mobility, wifi, wifiPhy, wifiMac);
  */
  

  // Simulator::Schedule (Seconds(0.0), LogSetTimePrinter, ExactTimePrinter);

  
  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
