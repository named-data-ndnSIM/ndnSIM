/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005-2009 Old Dominion University [ARBABI]
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
 * Author: Hadi Arbabi <marbabi@cs.odu.edu>
 */

/*
  This the starting point of the simulation and experiments.
  The main function will parse the input and parameter settings.
  Creates a highway and set the highway parameters. then bind the events (callbacks)
  to the created controller and designed handlers. Sets the highway start and end time,
  and eventually runs the simulation which is basically running a highway with a controller.
  You can add your functions to controller to create various scenarios.
*/

#include <fstream>
#include <iostream>
#include <iomanip>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"

#include "math.h"

#include "ns3/model.h"
#include "ns3/highway.h"
#include "ns3/vehicle-generator.h"
#include "ns3/tinyxml.h"
#include "ns3/highway-project-xml.h"
#include "ns3/highway-xml.h"
#include "ns3/wifi-configuration-xml.h"
#include "ns3/highway-project.h"

#include "ns3/ccnx-stack-helper.h"
#include "ns3/ccnx-face-container.h"
#include "ns3/ccnx-app-helper.h"

NS_LOG_COMPONENT_DEFINE ("VanetCcnx");

using namespace ns3;
using namespace std;

void
EquipVehicle (Ptr<Vehicle> vehicle, Ptr<Highway> highway, double probability)
{
  NS_LOG_FUNCTION (vehicle->GetNode ()->GetId () << highway << probability);

  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetDefaultRoutes (true);
  Ptr<CcnxFaceContainer> faces = ccnxHelper.Install (vehicle->GetNode ());
  NS_LOG_DEBUG ("Install CCNx stack. " << faces->GetN () << " faces available");


  if (probability < 30)
    {
      CcnxAppHelper producerHelper ("ns3::CcnxProducer");
      producerHelper.SetPrefix ("/");
      producerHelper.Install (vehicle->GetNode ());
    }
  else
    {
      CcnxAppHelper consumerHelper ("ns3::CcnxConsumerCbr");
      consumerHelper.SetAttribute ("LifeTime", StringValue("100s"));
      consumerHelper.SetAttribute ("Frequency", StringValue("10"));
      consumerHelper.SetPrefix ("/");
      consumerHelper.Install (vehicle->GetNode ());
    }
}

int
main (int argc, char *argv[])
{
  string projectXmlFile = "";

  CommandLine cmd;
  cmd.AddValue ("project", "highway xml description", projectXmlFile);
  cmd.Parse (argc, argv);
  if (projectXmlFile == "")
    {
      std::cerr << "ERROR: --project option MUST be specified\n";
      return 1;
    }

  std::ifstream testProjectFile (projectXmlFile.c_str ());
  if (testProjectFile.bad ())
    {
      std::cerr << "ERROR: Cannot open project file\n";
      return 2;
    }
  testProjectFile.close ();
  
  TiXmlDocument doc (projectXmlFile.c_str ());
  doc.LoadFile ();
  TiXmlHandle hDoc (&doc);
  TiXmlElement* root = hDoc.FirstChildElement ().Element ();
  TiXmlHandle hroot = TiXmlHandle (root);
  HighwayProjectXml xml;
  xml.LoadFromXml (hroot);

  ns3::PacketMetadata::Enable ();
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  
  HighwayProject project (xml);
  // project.SetVehTraceFile (vehicleTraceFile);
  // project.SetNetTraceFile (networkTraceFile);
  
  for (std::list<ns3::Ptr<ns3::VehicleGenerator> >::iterator generator = project.GetVehicleGenerators ().begin ();
       generator != project.GetVehicleGenerators ().end ();
       generator++)
    {
      (*generator)->TraceConnectWithoutContext ("NewVehicle", MakeCallback (EquipVehicle));
    }
  
  project.Start ();

  Simulator::Run ();
  Simulator::Destroy ();

  // Ptr<Highway> highway = project.getHighways ()[0];
  // Simulator::Schedule (Seconds (10), &addCustomVehicle, highway);
  // project.SetVehicleControlCallback (MakeCallback (&controlVehicle));

  return 0;
}
