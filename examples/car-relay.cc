#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/NDNabstraction-module.h"
#include <boost/regex.hpp>
#include "stdlib.h"
#include <iostream>
#include <sstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CarRelay");

typedef bool LINE_CALLBACK(string line);

class ScenarioConf{
public:
  ScenarioConf();
  int scenario_id; // this value is not used for now
  long car_number;
  double car_distance;
};

ScenarioConf::ScenarioConf(){
  scenario_id = -1;
  car_number = 0;
  car_distance = 0;
}

int global_scenario_id = -1;
ScenarioConf globalConf;

/*
 * @Return whether to terminate the loop process
 **/
bool process_conf_line(string line){
  std::stringstream ss_pattern;
  ss_pattern << global_scenario_id << "\\s+car_distance\\((\\d+)\\):\\s*(\\d+)";
  boost::regex re;
  try{
    re.assign(ss_pattern.str(), boost::regex_constants::icase);
  }catch ( boost::regex_error& e){
    std::cout<< ss_pattern.str() << " is not a valid pattern" << endl;
  }
  boost::cmatch matches;      
  if(boost::regex_search(line.c_str(), matches, re)){
    NS_LOG_INFO("Cars(" << matches[1] << "), Distance(" << matches[2] << ")");
    globalConf.car_number = strtol(matches[1].str().c_str(), NULL, 10);
    globalConf.car_distance = strtod(matches[2].str().c_str(), NULL);
    return true;
  }else{
    std::cout << "Cannot match line " << line << endl;
    return false;
  }
}

void go_through(string fileName, LINE_CALLBACK cb){
  string line;
  ifstream myfile (fileName.c_str());
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      getline (myfile,line);
      if(cb(line)){
	break;
      }
    }
    myfile.close();
  }
}


void
CourseChange (std::string context, Ptr<const MobilityModel> model)
{
  Vector position = model->GetPosition ();
  NS_LOG_INFO (context <<
		 " x = " << position.x << ", y = " << position.y);
}

void InData (std::string context, Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> packet,
	     Ptr<const CcnxFace> face){
  NS_LOG_INFO(face->GetNode()->GetId() << " has got data seq#" << header->GetName()->GetLastComponent ());
}

void OutData (std::string context, Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> packet,
	     Ptr<const CcnxFace> face){
  NS_LOG_INFO(face->GetNode()->GetId() << " sends out data seq#" << header->GetName()->GetLastComponent ());
}

void PhyRxInterferenceDrop(string context, Ptr<const Packet> packet){
  std::cout << "Dropping packet" << endl;
  //NS_LOG_INFO(" Dropping packet ");
}

void InCache (std::string context, Ptr<Ccnx> ccnx, Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
  std::cout << Simulator::Now ().ToDouble (Time::S) << " sec \tNode #" << ccnx->GetObject<Node> ()->GetId () << " cached seq#" << header->GetName ()->GetLastComponent () << "\n";
}

void SetupHighway(MobilityHelper mobility, WifiHelper& wifi, YansWifiPhyHelper& wifiPhy, NqosWifiMacHelper wifiMac)
{
  NodeContainer nodes;
  nodes.Create (globalConf.car_number);

  // 1. Install Wifi
  wifi.Install (wifiPhy, wifiMac, nodes);

  // 2. Install Mobility model
  mobility.Install (nodes);

  Config::Connect ("/NodeList/*/DeviceList/*/Phy/PhyRxInterferenceDrop", MakeCallback (&PhyRxInterferenceDrop));

  // 3. Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes(true);
  ccnxHelper.Install(nodes);

  Config::Connect ("/NodeList/*/$ns3::CcnxL3Protocol/InData", MakeCallback (&InData));
  Config::Connect ("/NodeList/*/$ns3::CcnxL3Protocol/OutData", MakeCallback (&OutData));
  Config::Connect ("/NodeList/*/$ns3::CcnxL3Protocol/ContentStore/InCache", MakeCallback (&InCache));
  
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


void init(int argc, char** argv){
  CommandLine cmd;
  string confFile;
  if(argc != 3){
	  std::cerr << "Usage: car-relay --conf=conf_file --s_id=id." << endl;
	  exit(3);
  }
  cmd.AddValue ("conf", "XML file for scenario parameter configuration", confFile);
  cmd.AddValue ("s_id", "The scenario id to be used", global_scenario_id);
  assert(confFile != "");
  assert(global_scenario_id != -1);

  cmd.Parse (argc,argv);
  go_through(confFile, process_conf_line);
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
  
  init(argc, argv);
  assert(globalConf.car_number != 0);
  assert(globalConf.car_distance != 0);

  //////////////////////
  //////////////////////
  //////////////////////
  WifiHelper wifi = WifiHelper::Default ();
  // wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode", StringValue ("OfdmRate24Mbps"));

  YansWifiChannelHelper wifiChannel;// = YansWifiChannelHelper::Default ();
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::NakagamiPropagationLossModel");


  //YansWifiPhy wifiPhy = YansWifiPhy::Default();
  YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default ();
  wifiPhyHelper.SetChannel (wifiChannel.Create ());

  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType("ns3::AdhocWifiMac");
  

  // Setup the first highway going in the direction of left -> right
  //bool static_cars = true;

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::HighwayPositionAllocator",
				 "Start", VectorValue(Vector(0.0, 0.0, 0.0)),
				 "Direction", DoubleValue(0.0),
				 "Length", DoubleValue(1000.0),
				 "MinGap", DoubleValue(globalConf.car_distance),
				 "MaxGap", DoubleValue(globalConf.car_distance));
  
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel",
			    "ConstantVelocity", VectorValue(Vector(26.8224, 0, 0)));

  SetupHighway(mobility, wifi, wifiPhyHelper, wifiMac);
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
