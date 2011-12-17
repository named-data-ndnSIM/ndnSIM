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

NS_LOG_COMPONENT_DEFINE ("CcnxAbileneTopology");

// int transmittedInterests = 0;
// int receivedInterests = 0;
// int droppedInterests = 0;

// int transmittedData = 0;
// int receivedData = 0;
// int droppedData = 0;

void PrintTime ()
{
  NS_LOG_INFO (Simulator::Now ());

  Simulator::Schedule (Seconds (10.0), PrintTime);
}

void PrintFIBs ()
{
  NS_LOG_INFO ("Outputing FIBs into [fibs.log]");
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("fibs.log", std::ios::out);
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      // *routingStream->GetStream () << "Node " << (*node)->GetId () << "\n";

      Ptr<CcnxFib> fib = (*node)->GetObject<CcnxFib> ();
      NS_ASSERT_MSG (fib != 0, "Fire alarm");
      *routingStream->GetStream () << *fib << "\n\n";
    }
}


struct AggregateAppTrace
{
  AggregateAppTrace ()
    : m_transmittedInterests (0)
    , m_transmittedData (0)
    , m_receivedInterests (0)
    , m_receivedNacks (0)
    , m_receivedData (0)
  {
    Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxConsumer/TransmittedInterests",
                     MakeCallback (&AggregateAppTrace::TransmittedInterests, this));

    Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxConsumer/ReceivedNacks",
                     MakeCallback (&AggregateAppTrace::ReceivedNacks, this));

    Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxProducer/ReceivedInterests",
                     MakeCallback (&AggregateAppTrace::ReceivedInterests, this));
  
    Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxProducer/TransmittedContentObjects",
                     MakeCallback (&AggregateAppTrace::TransmittedData, this));

    Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::CcnxConsumer/ReceivedContentObjects",
                     MakeCallback (&AggregateAppTrace::ReceivedData, this));
  }
  
  void
  TransmittedInterests (std::string context,
                        Ptr<const CcnxInterestHeader>, Ptr<CcnxApp>, Ptr<CcnxFace>)
  {
    m_transmittedInterests++;
  }

  void
  TransmittedData (std::string context,
                   Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                   Ptr<CcnxApp>, Ptr<CcnxFace>)
  {
    m_transmittedData++;
  }

  void
  ReceivedInterests (std::string context,
                     Ptr<const CcnxInterestHeader>,
                     Ptr<CcnxApp>, Ptr<CcnxFace>)
  {
    m_receivedInterests++;
  }

  void
  ReceivedNacks (std::string context,
                 Ptr<const CcnxInterestHeader>,
                 Ptr<CcnxApp>, Ptr<CcnxFace>)
  {
    m_receivedNacks++;
  }
  
  void
  ReceivedData (std::string context,
                Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                Ptr<CcnxApp>, Ptr<CcnxFace>)
  {
    m_receivedData++;
  }

  uint64_t m_transmittedInterests;
  uint64_t m_transmittedData;
  uint64_t m_receivedInterests;
  uint64_t m_receivedNacks;
  uint64_t m_receivedData;
};

ostream&
operator << (ostream &os, const AggregateAppTrace &trace)
{
  os << ">> (i): " << trace.m_transmittedInterests << "\n";
  os << ">> (d): " << trace.m_transmittedData << "\n";
  os << "<< (i): " << trace.m_receivedInterests << "\n";
  os << "<< (d): " << trace.m_receivedData << "\n";
  os << "<< (n): " << trace.m_receivedNacks << "\n";
  return os;
}

struct AggregateCcnxTrace
{
  AggregateCcnxTrace (const std::string &node = "*")
    : m_inInterests (0)
    , m_outInterests (0)
    , m_dropInterests (0)
    , m_inNacks (0)
    , m_outNacks (0)
    , m_dropNacks (0)
    , m_inData (0)
    , m_outData (0)
    , m_dropData (0)
  {
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/ForwardingStrategy/OutInterests",
                     MakeCallback (&AggregateCcnxTrace::OutInterests, this));
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/InInterests",
                     MakeCallback (&AggregateCcnxTrace::InInterests, this));
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/DropInterests",
                     MakeCallback (&AggregateCcnxTrace::DropInterests, this));

    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/OutNacks",
                     MakeCallback (&AggregateCcnxTrace::OutNacks, this));
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/InNacks",
                     MakeCallback (&AggregateCcnxTrace::InNacks, this));
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/DropNacks",
                     MakeCallback (&AggregateCcnxTrace::DropNacks, this));

    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/OutData",
                     MakeCallback (&AggregateCcnxTrace::OutData, this));
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/InData",
                     MakeCallback (&AggregateCcnxTrace::InData, this));
    Config::Connect ("/NodeList/"+node+"/$ns3::CcnxL3Protocol/DropData",
                     MakeCallback (&AggregateCcnxTrace::DropData, this));
  }

  void OutInterests  (std::string context,
                      Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
  { m_outInterests++; }
  void InInterests   (std::string context,
                      Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
  { m_inInterests++; }
  void DropInterests (std::string context,
                      Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>)
  { m_dropInterests++; }

  void OutNacks  (std::string context,
                  Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
  { m_outNacks++; }
  void InNacks   (std::string context,
                  Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace>)
  { m_inNacks++; }
  void DropNacks (std::string context,
                  Ptr<const CcnxInterestHeader>, Ccnx::DropReason, Ptr<const CcnxFace>)
  { m_dropNacks++; }

  void OutData  (std::string context,
                 Ptr<const CcnxContentObjectHeader>, bool fromCache, Ptr<const CcnxFace>)
  { m_inData++; }
  void InData   (std::string context,
                 Ptr<const CcnxContentObjectHeader>, Ptr<const CcnxFace>)
  { m_outData++; }
  void DropData (std::string context,
                 Ptr<const CcnxContentObjectHeader>, Ccnx::DropReason, Ptr<const CcnxFace>)
  { m_dropData++; }

  uint64_t m_inInterests;
  uint64_t m_outInterests;
  uint64_t m_dropInterests;
  uint64_t m_inNacks;
  uint64_t m_outNacks;
  uint64_t m_dropNacks;
  uint64_t m_inData;
  uint64_t m_outData;
  uint64_t m_dropData;
};

ostream&
operator << (ostream &os, const AggregateCcnxTrace &trace)
{
  os << "(Interests)\n";
  os << ">> (in):   " << trace.m_inInterests   << "\n";
  os << ">> (out):  " << trace.m_outInterests  << "\n";
  os << "<< (drop): " << trace.m_dropInterests << "\n";
  os << "(Nacks)\n";
  os << "<< (in):   " << trace.m_inNacks   << "\n";
  os << "<< (out):  " << trace.m_outNacks  << "\n";
  os << "<< (drop): " << trace.m_dropNacks << "\n";
  os << "(Data)\n";
  os << ">> (in):   " << trace.m_inData   << "\n";
  os << ">> (out):  " << trace.m_outData  << "\n";
  os << "<< (drop): " << trace.m_dropData << "\n";
  return os;
}

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
  // Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
  // stack.SetRoutingHelper (ipv4RoutingHelper);
  // stack.Install (nodes);

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
    
  CcnxProducerHelper producerHelper ("/5",1024);
  ApplicationContainer producers = producerHelper.Install (Names::Find<Node> ("/abilene", "IPLSng"));

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
  AggregateAppTrace appTrace;
  AggregateCcnxTrace ccnxTrace ("1");
    
  config.ConfigureAttributes ();  
  
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  NS_LOG_INFO ("AppTrace: \n" << appTrace);
  NS_LOG_INFO ("CcnxTrace: \n" << ccnxTrace);
  return 0;
}
