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
#include "ns3/random-variable.h"
#include "ns3/ccnx-l3-protocol.h"
#include "ns3/topology-reader.h"

#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <set>
#include "ns3/rocketfuel-topology-reader.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace ns3;
using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("LinkFailureSprint");

void PrintTime ()
{
  cout << "Progress: " << Simulator::Now ().ToDouble (Time::S) << "s" << endl;

  Simulator::Schedule (Seconds (1.0), PrintTime);
}


class Experiment
{
public:
  Experiment ()
  : m_reader ("/sprint") { }

  void
  ConfigureTopology ()
  {
    Names::Clear ();
    
    string weights   ("./src/NDNabstraction/examples/sprint-pops.weights");
    string latencies ("./src/NDNabstraction/examples/sprint-pops.latencies");
    string positions ("./src/NDNabstraction/examples/sprint-pops.positions");
  
    //RocketfuelWeightsReader reader ("/sprint");

    m_reader.SetFileName (positions);
    m_reader.SetFileType (RocketfuelWeightsReader::POSITIONS);
    m_reader.Read ();
  
    m_reader.SetFileName (weights);
    m_reader.SetFileType (RocketfuelWeightsReader::WEIGHTS);    
    m_reader.Read ();

    m_reader.SetFileName (latencies);
    m_reader.SetFileType (RocketfuelWeightsReader::LATENCIES);    
    m_reader.Read ();
    
    m_reader.Commit ();
    NS_ASSERT_MSG (m_reader.LinksSize () != 0, "Problems reading the topology file. Failing.");
    
    NS_LOG_INFO("Nodes = " << m_reader.GetNodes ().GetN());
    NS_LOG_INFO("Links = " << m_reader.LinksSize ());
  
    // ------------------------------------------------------------
    // -- Read topology data.
    // --------------------------------------------
        
    InternetStackHelper stack;
    Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
    stack.SetRoutingHelper (ipv4RoutingHelper);
    stack.Install (m_reader.GetNodes ());

    m_reader.AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));
    
    // Install CCNx stack
    NS_LOG_INFO ("Installing CCNx stack");
    CcnxStackHelper ccnxHelper;
    ccnxHelper.SetForwardingStrategy ("ns3::CcnxBestRouteStrategy");
    ccnxHelper.EnableLimits (true, Seconds(0.1));
    ccnxHelper.SetDefaultRoutes (false);
    ccnxHelper.InstallAll ();
    
    m_rand = UniformVariable (0, m_reader.GetNodes ().GetN());
    //m_linkRand = UniformVariable(0, m_reader.LinksSize());
  }

  void
  ConfigureRouting ()
  {
    CcnxStackHelper ccnxHelper;
    // // Populate FIB based on IPv4 global routing controller
    ccnxHelper.InstallFakeGlobalRoutes ();
    ccnxHelper.InstallRoutesToAll ();
  }

public:  
  void
  Run (const Time &finishTime)
  {
    cout << "Run Simulation.\n";
    Simulator::Stop (finishTime);
    Simulator::Schedule (Seconds (1.0), PrintTime);
    Simulator::Run ();
    Simulator::Destroy ();
    cout << "Done.\n";
  }

  void
  FailLinks(double threshold)
  {
    NS_LOG_INFO("Failing links");
    m_linkRand = UniformVariable(0, 1.0);
    double probability = 0.0;
    
    BOOST_FOREACH (const TopologyReader::Link &link, m_reader.GetLinks())
      {
        probability = m_linkRand.GetValue();
        NS_LOG_INFO ("Probability = " << probability);
        
        if(probability <= threshold)
          {
            Ptr<Node> fromNode = link.GetFromNode ();
            Ptr<Node> toNode = link.GetToNode ();
            NS_LOG_INFO("From node id = " << fromNode->GetId());
            NS_LOG_INFO("To node id = " << toNode->GetId());
            
            Ptr<CcnxL3Protocol> fromCcnx = fromNode->GetObject<CcnxL3Protocol> ();
            Ptr<CcnxL3Protocol> toCcnx = toNode->GetObject<CcnxL3Protocol> (); 
          
            Ptr<NetDevice> fromDevice = link.GetFromNetDevice ();
            Ptr<NetDevice> toDevice = link.GetToNetDevice ();
          
            Ptr<CcnxFace> fromFace = fromCcnx->GetFaceByNetDevice (fromDevice);
            Ptr<CcnxFace> toFace = toCcnx->GetFaceByNetDevice (toDevice);
          
            NS_LOG_INFO("From face id = " << fromFace->GetId());
            NS_LOG_INFO("To face id = " << toFace->GetId());
            fromFace->SetUp (false);
            toFace->SetUp (false);
            
            NS_LOG_INFO(fromFace->IsUp());
            NS_LOG_INFO(toFace->IsUp());
          }

      }
    /*
    uint32_t nodeId = m_rand.GetValue ();
    Ptr<Node> node = Names::Find<Node> ("/sprint", lexical_cast<string> (nodeId));
    
    Ptr<CcnxL3Protocol> ccnx = node->GetObject<CcnxL3Protocol> ();
    UniformVariable faceRandom = UniformVariable (0, ccnx->GetNFaces ());
    uint32_t faceId = faceRandom.GetValue();
    Ptr<CcnxFace> face = ccnx->GetFace (faceId);
    face->SetUp(false);
    */
  }

  //We are creating "everybody-to-everybody" usage pattern
  ApplicationContainer
  AddApplications()
  {
    NS_LOG_INFO("Adding applications");
    NS_LOG_INFO("GetN = " << m_reader.GetNodes().GetN());
    
    ApplicationContainer apps;
    for(uint32_t i = 0; i<m_reader.GetNodes().GetN(); i++)
    {
      NS_LOG_INFO("i="<<i);
      Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (i));
        
      CcnxAppHelper producerHelper ("ns3::CcnxProducer");
      producerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
        
      apps.Add(producerHelper.Install (node1));
            
      for(uint32_t j = 0; j<m_reader.GetNodes().GetN();j++)
      {
        NS_LOG_INFO("j="<<j);
        if(i==j)
          continue;
        
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (j));
          
        CcnxAppHelper consumerHelper ("ns3::CcnxConsumer");
        consumerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
        consumerHelper.SetAttribute ("MeanRate", StringValue ("1Kbps"));
        consumerHelper.SetAttribute ("Size", StringValue ("2"));  
        
        apps.Add(consumerHelper.Install (node2));
      }
    }
    
    return apps;
  }

  UniformVariable m_rand;
  UniformVariable m_linkRand;
  RocketfuelWeightsReader m_reader;
};

int 
main (int argc, char *argv[])
{
  cout << "Begin link failure scenario\n";
  
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("10Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("100"));

  Time finishTime1 = Seconds (5.0);
  Time finishTime2 = Seconds (20.0);

  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime1);
  cmd.Parse (argc, argv);

  Experiment experiment;

  for (uint32_t i = 0; i < 80; i++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (i));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

      Experiment experiment;
      cout << "Run " << i << endl;
      
      string prefix = "run-" + lexical_cast<string> (i) + "-";
  
      //before link failure
      experiment.ConfigureTopology ();
      ApplicationContainer apps = experiment.AddApplications ();
      experiment.ConfigureRouting ();
      //tracing
      //...
      //experiment.Run (finishTime1);
      
      //after link failure 
      experiment.FailLinks(0.1);
      
      //tracing
      CcnxTraceHelper traceHelper;
      traceHelper.EnableRateL3All (prefix + "rate-trace.log");
      traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", prefix + "consumers-seqs.log");
      //...
      experiment.Run (finishTime2);

/*
      for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
        {
          cout << "From " << apps.Get (i*2)->GetNode ()->GetId ()
               << " to "  << apps.Get (i*2 + 1)->GetNode ()->GetId ();
          cout << "\n";
        }*/
  
      //CcnxTraceHelper traceHelper;
      // traceHelper.EnableAggregateAppAll ("ns3::CcnxConsumer");
      // traceHelper.EnableAggregateAppAll ("ns3::CcnxProducer");
      // traceHelper.EnableAggregateL3All ();
      // traceHelper.SetL3TraceFile ("trace-l3.log");
      // traceHelper.SetAppTraceFile ("trace-app.log");
      // traceHelper.EnableRateL3All ("rate-trace.log");
      //traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", "consumers-seqs.log");

      
    }

  cout << "Finish link failure scenario\n";
  return 0;
}
