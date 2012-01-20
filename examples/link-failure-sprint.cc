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


#include "base-experiment.h"

class Experiment : public BaseExperiment
{
public:
  // hijacker is more than an application. just disable all faces
  static void
  FailLinks (uint32_t failId)
  {
    // Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
    // for (uint32_t i = 0; i < ccnx->GetNFaces (); i++)
    //   {
    //     Ptr<CcnxFace> face = ccnx->GetFace (i);
    //     face->SetUp (false);
    //   }
    // CcnxStackHelper::InstallRouteTo (prefix, node);
  }

  // void
  // FailLinks(double threshold)
  // {
  //   NS_LOG_INFO("Failing links");
  //   m_linkRand = UniformVariable(0, 1.0);
  //   double probability = 0.0;
    
  //   BOOST_FOREACH (const TopologyReader::Link &link, m_reader.GetLinks())
  //     {
  //       probability = m_linkRand.GetValue();
  //       NS_LOG_INFO ("Probability = " << probability);
        
  //       if(probability <= threshold)
  //         {
  //           Ptr<Node> fromNode = link.GetFromNode ();
  //           Ptr<Node> toNode = link.GetToNode ();
  //           NS_LOG_INFO("From node id = " << fromNode->GetId());
  //           NS_LOG_INFO("To node id = " << toNode->GetId());
            
  //           Ptr<CcnxL3Protocol> fromCcnx = fromNode->GetObject<CcnxL3Protocol> ();
  //           Ptr<CcnxL3Protocol> toCcnx = toNode->GetObject<CcnxL3Protocol> (); 
          
  //           Ptr<NetDevice> fromDevice = link.GetFromNetDevice ();
  //           Ptr<NetDevice> toDevice = link.GetToNetDevice ();
          
  //           Ptr<CcnxFace> fromFace = fromCcnx->GetFaceByNetDevice (fromDevice);
  //           Ptr<CcnxFace> toFace = toCcnx->GetFaceByNetDevice (toDevice);
          
  //           NS_LOG_INFO("From face id = " << fromFace->GetId());
  //           NS_LOG_INFO("To face id = " << toFace->GetId());
  //           fromFace->SetUp (false);
  //           toFace->SetUp (false);
            
  //           NS_LOG_INFO(fromFace->IsUp());
  //           NS_LOG_INFO(toFace->IsUp());
  //         }

  //     }
  //   /*
  //   uint32_t nodeId = m_rand.GetValue ();
  //   Ptr<Node> node = Names::Find<Node> ("/sprint", lexical_cast<string> (nodeId));
    
  //   Ptr<CcnxL3Protocol> ccnx = node->GetObject<CcnxL3Protocol> ();
  //   UniformVariable faceRandom = UniformVariable (0, ccnx->GetNFaces ());
  //   uint32_t faceId = faceRandom.GetValue();
  //   Ptr<CcnxFace> face = ccnx->GetFace (faceId);
  //   face->SetUp(false);
  //   */
  // }

  //We are creating "everybody-to-everybody" usage pattern
  ApplicationContainer
  AddApplications()
  {
    NS_LOG_INFO ("Adding applications");
    NS_LOG_INFO ("GetN = " << m_reader.GetNodes().GetN());
    
    ApplicationContainer apps;
    for (uint32_t i = 0; i<m_reader.GetNodes().GetN(); i++)
    {
      NS_LOG_INFO("i="<<i);
      Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (i));
        
      CcnxAppHelper producerHelper ("ns3::CcnxProducer");
      producerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
        
      apps.Add (producerHelper.Install (node1));
            
      CcnxAppHelper consumerHelper ("ns3::CcnxConsumerBatches");
      consumerHelper.SetAttribute ("LifeTime", StringValue("100s"));
      consumerHelper.SetAttribute ("Batches", StringValue("0s 10 6s 1 20s 1"));
      
      for(uint32_t j = 0; j<m_reader.GetNodes().GetN();j++)
      {
        NS_LOG_INFO("j="<<j);
        if(i==j)
          continue;
        
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (j));
          
        consumerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()) + "/" + lexical_cast<string> (node2->GetId ()));
        apps.Add (consumerHelper.Install (node2));
      }
    }
    
    return apps;
  }
};

int 
main (int argc, char *argv[])
{
  cout << "Begin link failure scenario\n";

  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("100Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("2000"));
  Config::SetDefault ("ns3::RttEstimator::InitialEstimation", StringValue ("0.5s"));

  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("attributes.xml"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));

  uint32_t maxRuns = 1;
  uint32_t startRun = 0;
  std::string failures = "";
  CommandLine cmd;
  cmd.AddValue ("start", "Initial run number", startRun);
  cmd.AddValue ("runs", "Number of runs", maxRuns);
  cmd.AddValue ("failures", "File with failures", failures);
  cmd.Parse (argc, argv);

  if (failures == "")
    {
      std::cerr << "--failures=<file> parameter has to be specified" << std::endl;
      return 1;
    }

  // ConfigStore config;
  // config.ConfigureDefaults ();

  Experiment experiment;
  for (uint32_t run = startRun; run < startRun + maxRuns; run++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (run));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

      Experiment experiment;
      // experiment.GenerateRandomPairs (1);
      experiment.FailLinks (run);
      cout << "Run " << run << endl;
      
      string prefix = "link-failure-" + lexical_cast<string> (run) + "-";
  
      experiment.ConfigureTopology ();
      experiment.InstallCcnxStack (false);
      ApplicationContainer apps = experiment.AddApplications ();
            
      //tracing
      CcnxTraceHelper traceHelper;
      // traceHelper.EnableRateL3All (prefix + "rate-trace.log");
      traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumerBatches", prefix + "consumers-seqs.log");

      // enable path weights some time from now (ensure that all faces are created)
      Simulator::Schedule (Seconds (4.5), &CcnxTraceHelper::EnablePathWeights, &traceHelper, prefix + "weights.log");
      std::cout << "Total " << apps.GetN () << " applications\n";

      experiment.Run (Seconds(40.0));
    }



  
//   Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("10Mbps"));
//   Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("100"));

//   Time finishTime1 = Seconds (5.0);
//   Time finishTime2 = Seconds (20.0);

//   CommandLine cmd;
//   cmd.AddValue ("finish", "Finish time", finishTime1);
//   cmd.Parse (argc, argv);

//   Experiment experiment;

//   for (uint32_t i = 0; i < 80; i++)
//     {
//       Config::SetGlobal ("RngRun", IntegerValue (i));
//       cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

//       Experiment experiment;
//       cout << "Run " << i << endl;
      
//       string prefix = "run-" + lexical_cast<string> (i) + "-";
  
//       //before link failure
//       experiment.ConfigureTopology ();
//       ApplicationContainer apps = experiment.AddApplications ();
//       experiment.ConfigureRouting ();
//       //tracing
//       //...
//       //experiment.Run (finishTime1);
      
//       //after link failure 
//       experiment.FailLinks(0.1);
      
//       //tracing
//       CcnxTraceHelper traceHelper;
//       traceHelper.EnableRateL3All (prefix + "rate-trace.log");
//       traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", prefix + "consumers-seqs.log");
//       //...
//       experiment.Run (finishTime2);

// /*
//       for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
//         {
//           cout << "From " << apps.Get (i*2)->GetNode ()->GetId ()
//                << " to "  << apps.Get (i*2 + 1)->GetNode ()->GetId ();
//           cout << "\n";
//         }*/
  
//       //CcnxTraceHelper traceHelper;
//       // traceHelper.EnableAggregateAppAll ("ns3::CcnxConsumer");
//       // traceHelper.EnableAggregateAppAll ("ns3::CcnxProducer");
//       // traceHelper.EnableAggregateL3All ();
//       // traceHelper.SetL3TraceFile ("trace-l3.log");
//       // traceHelper.SetAppTraceFile ("trace-app.log");
//       // traceHelper.EnableRateL3All ("rate-trace.log");
//       //traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumer", "consumers-seqs.log");

      
//     }

  cout << "Finish link failure scenario\n";
  return 0;
}
