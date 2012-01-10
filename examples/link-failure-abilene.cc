/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */
 


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/NDNabstraction-module.h"
#include "ns3/point-to-point-grid.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/animation-interface.h"
// #include "ns3/ccnx-l3-protocol.h"

#include <iostream>
#include <sstream>
#include "ns3/annotated-topology-reader.h"
#include "../utils/spring-mobility-helper.h"

#include "ns3/config-store.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("CcnxLinkFailureAbilene");


int 
main (int argc, char *argv[])
{
  // Packet::EnableChecking();
  // Packet::EnablePrinting();
  string input ("./src/NDNabstraction/examples/abilene-topology.txt");

  Time finishTime = Seconds (20.0);
  string animationFile;
  string strategy = "ns3::CcnxFloodingStrategy";
  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.AddValue ("netanim", "NetAnim filename", animationFile);
  cmd.AddValue ("strategy", "CCNx forwarding strategy", strategy);
  cmd.Parse (argc, argv);
    
  ConfigStore config;
  config.ConfigureDefaults ();
    
  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------
    
  AnnotatedTopologyReader reader ("/abilene");
  reader.SetMobilityModel ("ns3::SpringMobilityModel");
  reader.SetFileName (input);
    
  NodeContainer nodes = reader.Read ();
    
  if (reader.LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }

  SpringMobilityHelper::InstallSprings (reader.LinksBegin (), reader.LinksEnd ());

  // InternetStackHelper stack;
  Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  stack.SetRoutingHelper (ipv4RoutingHelper);
  stack.Install (nodes);

  // reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));

  NS_LOG_INFO("Nodes = " << nodes.GetN());
  NS_LOG_INFO("Links = " << reader.LinksSize ());
    
  // Install CCNx stack
  NS_LOG_INFO ("Installing CCNx stack");
  CcnxStackHelper ccnxHelper;
  ccnxHelper.SetForwardingStrategy (strategy);
  ccnxHelper.EnableLimits (false, Seconds(0.1));
  ccnxHelper.SetDefaultRoutes (true);
  ccnxHelper.InstallAll ();
    
  NS_LOG_INFO ("Installing Applications");
  CcnxConsumerHelper consumerHelper ("/5");
  ApplicationContainer consumers = consumerHelper.Install (Names::Find<Node> ("/abilene", "ATLAng"));
    
  // CcnxProducerHelper producerHelper ("/5",1024);
  // ApplicationContainer producers = producerHelper.Install (Names::Find<Node> ("/abilene", "IPLSng"));

  // // Populate FIB based on IPv4 global routing controller
  // ccnxHelper.InstallFakeGlobalRoutes ();
  // ccnxHelper.InstallRouteTo (Names::Find<Node> ("/abilene", "IPLSng"));

  // Simulator::Schedule (Seconds (1.0), PrintFIBs);
  // PrintFIBs ();

  // Simulator::Schedule (Seconds (10.0), PrintTime);

  Simulator::Stop (finishTime);

  AnimationInterface *anim = 0;
  if (animationFile != "")
    {
      anim = new AnimationInterface (animationFile);
      anim->SetMobilityPollInterval (Seconds (1));
    }

  // NS_LOG_INFO ("Configure Tracing.");
  /*AggregateTrace trace;
  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxConsumer/TransmittedInterests",
                   MakeCallback (&AggregateTrace::TransmittedInterests, &trace));

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxConsumer/ReceivedNacks",
                   MakeCallback (&AggregateTrace::ReceivedNacks, &trace));

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxProducer/ReceivedInterests",
                   MakeCallback (&AggregateTrace::ReceivedInterests, &trace));
  
  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxProducer/TransmittedContentObjects",
                   MakeCallback (&AggregateTrace::TransmittedData, &trace));

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxConsumer/ReceivedContentObjects",
                   MakeCallback (&AggregateTrace::ReceivedData, &trace));
  */
  // Config::Connect("/NodeList/*/ns3::CcnxL3Protocol/TransmittedInterestTrace",
  //                                MakeCallback (&OnTransmittedInterest));
  // Config::Connect ("/NodeList/*/ns3::CcnxL3Protocol/ReceivedInterestTrace",
  //                    MakeCallback (&OnReceivedInterest));
  // Config::Connect ("/NodeList/*/ns3::CcnxL3Protocol/DroppedInterestTrace",
  //                    MakeCallback (&OnDroppedInterest));
    
  // Config::Connect ("/NodeList/*/ns3::CcnxL3Protocol/ReceivedDataTrace",
  //                    MakeCallback (&OnReceivedData));
  // Config::Connect ("/NodeList/*/ns3::CcnxL3Protocol/TransmittedDataTrace",
  //                    MakeCallback (&OnTransmittedData));
  // Config::Connect ("/NodeList/*/ns3::CcnxL3Protocol/DroppedDataTrace",
  //                    MakeCallback (&OnDroppedData));
  
  config.ConfigureAttributes ();  
  
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  // NS_LOG_INFO("Total received interests = " << receivedInterests);
  // NS_LOG_INFO("Total transmitted interests = " << transmittedInterests);
  // NS_LOG_INFO("Total dropped interests = " << droppedInterests);
  // NS_LOG_INFO("Total received data = " << receivedData);
  // NS_LOG_INFO("Total transmitted data = " << transmittedData);
  // NS_LOG_INFO("Total dropped data = " << droppedData);
  NS_LOG_INFO (trace);
  return 0;
}
