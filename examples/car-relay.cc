#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/NDNabstraction-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CarRelay");

void
CourseChange (std::string context, Ptr<const MobilityModel> model)
{
  Vector position = model->GetPosition ();
  NS_LOG_INFO (context <<
		 " x = " << position.x << ", y = " << position.y);
}

void InData (std::string context, Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> packet,
	     Ptr<const CcnxFace> face){
  NS_LOG_INFO(face->GetNode()->GetId() << " has got data " << *(header->GetName()));
}

void SetupHighway(MobilityHelper mobility, WifiHelper& wifi, YansWifiPhyHelper& wifiPhy, NqosWifiMacHelper wifiMac)
{
  NodeContainer nodes;
  nodes.Create (10);

  // 1. Install Wifi
  wifi.Install (wifiPhy, wifiMac, nodes);

  // 2. Install Mobility model
  mobility.Install (nodes);

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange", MakeCallback (&CourseChange));


  /*NodeContainer::Iterator it = nodes.Begin ();
  for(; it != nodes.End (); it++){
    // set up location logging
    std::ostringstream location_oss;
    location_oss << "/NodeList/" << (*it)->GetId () <<
      "/$ns3::MobilityModel/CourseChange";
    Config::Connect (location_oss.str (), MakeCallback (&CourseChange));

    // set up data reception logging
    std::ostringstream data_oss;
    data_oss << "/NodeList/" << (*it)->GetId () <<
      "/$ns3::CcnxL3Protocol/InData";
    
      }*/

  // 3. Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes(true);
  ccnxHelper.Install(nodes);

  Config::Connect ("/NodeList/*/$ns3::CcnxL3Protocol/InData", MakeCallback (&InData));

  // 4. Set up applications
  NS_LOG_INFO ("Installing Applications");
  
  CcnxAppHelper consumerHelper ("ns3::CcnxConsumerBatches");
  consumerHelper.SetPrefix ("/very-long-prefix-requested-by-client/this-interest-hundred-bytes-long-");
  consumerHelper.SetAttribute ("Batches", StringValue ("2s 1"));
  // consumerHelper.SetAttribute ("Frequency", StringValue ("5"));
  // consumerHelper.SetAttribute ("Randomize", StringValue ("exponential"));

  CcnxAppHelper producerHelper ("ns3::CcnxProducer");
  producerHelper.SetPrefix ("/");
  producerHelper.SetAttribute ("PayloadSize", StringValue("300"));

  // install producer and consumer on the same node
  NS_LOG_INFO("Making node("<< nodes.Get(0)->GetId() <<") both a consumer and producer. "<<
	      "It will pull data from it self and then data will be propagated out.");
  consumerHelper.Install (nodes.Get (0));
  producerHelper.Install (nodes.Get (0));

  // gradient push mechanism should push data to the fartherest car... hopefully it will not explode
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
  Config::SetDefault ("ns3::CcnxBroadcastNetDeviceFace::MaxDelay", StringValue ("2ms"));
  Config::SetDefault ("ns3::CcnxBroadcastNetDeviceFace::MaxDelayLowPriority", StringValue ("5ms"));
  Config::SetDefault ("ns3::CcnxBroadcastNetDeviceFace::MaxDistance", StringValue ("40"));
  
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
				 "MinGap", DoubleValue(50.0),
				 "MaxGap", DoubleValue(50.0));
  
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(26.8224, 0, 0)));

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


  Simulator::Stop (Seconds (30.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
