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

NS_LOG_COMPONENT_DEFINE ("BlackholeSprint");

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
    m_linkRand = UniformVariable(0, m_reader.LinksSize());
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
    //Simulator::Schedule (Seconds (1.0), PrintTime);
    Simulator::Run ();
    Simulator::Destroy ();
    cout << "Done.\n";
  }

  //We are creating 10 pairs of producer-hijacker and everybody else is a consumer
  ApplicationContainer
  AddApplications(uint32_t numOfPairs)
  {
    NS_LOG_INFO("Adding applications");
    ApplicationContainer apps;
    
    list<uint32_t > usedNodes;
    list<uint32_t >::iterator listIterator;
    list<uint32_t >::iterator listIterator2;
    
    uint32_t producerNodeId;
    uint32_t hijackerNodeId;

    for(uint32_t pairCount = 0; pairCount < numOfPairs; pairCount++)
      {
        NS_LOG_INFO("pairCount = "<<pairCount);
        while(true)
          {
            producerNodeId = m_rand.GetValue ();
            hijackerNodeId = m_rand.GetValue ();
        
            bool unique = true; 
            for(listIterator=usedNodes.begin(); listIterator != usedNodes.end(); ++listIterator)
              {
                if((*listIterator == producerNodeId) || (*listIterator == hijackerNodeId))
                  {
                    NS_LOG_INFO("NonUnique");
                    unique = false;
                  }
              }
        
            if(unique == true)
              {
                usedNodes.push_back(producerNodeId);
                usedNodes.push_back(hijackerNodeId);
                break;
              }
          }
      
        NS_LOG_INFO("Producer #" << producerNodeId);
        Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (producerNodeId));
        CcnxAppHelper producerHelper ("ns3::CcnxProducer");
        producerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
        
        apps.Add(producerHelper.Install (node1));

        NS_LOG_INFO("Hijacker # "<<hijackerNodeId);
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (hijackerNodeId));
        CcnxAppHelper hijackerHelper ("ns3::CcnxHijacker");
        hijackerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
        
        apps.Add(hijackerHelper.Install (node1));
        
        /*NS_LOG_INFO("Consumers");
        for(uint32_t j = 0; j<m_reader.GetNodes().GetN();j++)
          {
            //NS_LOG_INFO("j="<<j);
            bool consumer = true;
            for(listIterator=usedNodes.begin(); listIterator != usedNodes.end(); ++listIterator)
              {
                if(*listIterator == j)
                {
                  consumer = false;
                  NS_LOG_INFO(j<<" CANNOT be CONSUMER");
                  break;
                }
              }
        
            if(consumer == true)
              {
                Ptr<Node> node3 = Names::Find<Node> ("/sprint", lexical_cast<string> (j));
        
                CcnxAppHelper consumerHelper ("ns3::CcnxConsumer");
                consumerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
                consumerHelper.SetAttribute ("MeanRate", StringValue ("1Kbps"));
                consumerHelper.SetAttribute ("Size", StringValue ("2"));  
        
                apps.Add(consumerHelper.Install (node3));
              }
          }*/
        }
      
      NS_LOG_INFO("Consumers");
      for(listIterator=usedNodes.begin(); listIterator != usedNodes.end(); ++listIterator,++listIterator)
        {
          for(uint32_t j = 0; j<m_reader.GetNodes().GetN();j++)
          {
            //NS_LOG_INFO("j="<<j);
            bool consumer = true;
            for(listIterator2=usedNodes.begin(); listIterator2 != usedNodes.end(); ++listIterator2)
              {
                if(*listIterator2 == j)
                {
                  consumer = false;
                  NS_LOG_INFO(j<<" CANNOT be a CONSUMER");
                  break;
                }
              }
        
            if(consumer == true)
              {
                Ptr<Node> node3 = Names::Find<Node> ("/sprint", lexical_cast<string> (j));
                Ptr<Node> node4 = Names::Find<Node> ("/sprint", lexical_cast<string> (*listIterator));

                CcnxAppHelper consumerHelper ("ns3::CcnxConsumer");
                NS_LOG_INFO("Node = " << *listIterator);
                consumerHelper.SetPrefix ("/" + lexical_cast<string> (node4->GetId ()));
                consumerHelper.SetAttribute ("MeanRate", StringValue ("1Kbps"));
                consumerHelper.SetAttribute ("Size", StringValue ("2"));  
        
                apps.Add(consumerHelper.Install (node3));
              }
          }
        }
      
      
    return apps;
  }

  UniformVariable m_rand;
  UniformVariable m_linkRand;
  
private:
  RocketfuelWeightsReader m_reader;
};

int 
main (int argc, char *argv[])
{
  cout << "Begin blackhole scenario\n";
  
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("2Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("100"));

  Time finishTime = Seconds (20.0);
  
   
  CommandLine cmd;
  cmd.AddValue ("finish", "Finish time", finishTime);
  cmd.Parse (argc, argv);

  Experiment experiment;

  for (uint32_t i = 0; i < 80; i++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (i));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

      Experiment experiment;
      cout << "Run " << i << endl;
      
      string prefix = "run-" + lexical_cast<string> (i) + "-";
  
      experiment.ConfigureTopology ();
      ApplicationContainer apps = experiment.AddApplications (10);
      experiment.ConfigureRouting ();
      
      /*ApplicationContainer apps = experiment.AddApplications ();

      for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
        {
          cout << "From " << apps.Get (i*2)->GetNode ()->GetId ()
               << " to "  << apps.Get (i*2 + 1)->GetNode ()->GetId ();
          cout << "\n";
        }
      */
      
      //tracing
      CcnxTraceHelper traceHelper;
      traceHelper.EnableRateL3All (prefix + "rate-trace.log");
      traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", prefix + "consumers-seqs.log");
      
      experiment.Run (finishTime);
    }

  cout << "Finish blackhole scenario\n";
  return 0;
}
