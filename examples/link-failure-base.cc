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
#include "ns3/ccnx.h"
#include "ns3/topology-reader.h"
#include "../model/ccnx-net-device-face.h"

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

#include "base-experiment.h"

class Experiment : public BaseExperiment
{
public:
  typedef tuple<string, string> failure_t;
  typedef list<failure_t> failures_t;
  
  Experiment (const string &file)
  {
    ifstream failures (("./src/NDNabstraction/examples/failures/failures-"+file).c_str ());
    for(std::string line; std::getline(failures, line); )
      {
        if (line == "")
          {
            m_failures.push_back (failures_t ());
            continue;
          }

        failures_t failures;
        istringstream run (line);
        while (!run.eof () && !run.bad ())
          {
            int32_t link1 = -1;
            int32_t link2 = -1;
            run >> link1;
            run.get ();
            run >> link2;
            run.get ();
            if (link1 < 0 || link2 < 0) continue;

            // cout << link1 << " <-> " << link2 << "   ";
            failures.push_back (failure_t (lexical_cast<string> (link1), lexical_cast<string> (link2)));
          }
        m_failures.push_back (failures);
      }
  }
  
  // hijacker is more than an application. just disable all faces
  void
  FailLinks (uint32_t failId)
  {
    failures_t failures = m_failures [failId];
    BOOST_FOREACH (failure_t failure, failures)
      {
        Ptr<Node> node1 = Names::Find<Node> ("/sprint", failure.get<0> ());
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", failure.get<1> ());
        // cout << failure.get<0> () << " <-> " << failure.get<1> () << "   ";
        // cout << node1 << ", " << node2 << "\n";

        Ptr<Ccnx> ccnx1 = node1->GetObject<Ccnx> ();
        Ptr<Ccnx> ccnx2 = node2->GetObject<Ccnx> ();
        for (uint32_t faceId = 0; faceId < ccnx1->GetNFaces (); faceId++)
          {
            Ptr<CcnxFace> face = ccnx1->GetFace (faceId);
            Ptr<CcnxNetDeviceFace> ndFace = face->GetObject<CcnxNetDeviceFace> ();
            if (ndFace == 0) continue;

            Ptr<PointToPointNetDevice> nd1 = ndFace->GetNetDevice ()->GetObject<PointToPointNetDevice> ();
            if (nd1 == 0) continue;

            Ptr<Channel> channel = nd1->GetChannel ();
            if (channel == 0) continue;

            Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel> (channel);

            Ptr<NetDevice> nd2 = ppChannel->GetDevice (0);
            if (nd2->GetNode () == node1)
              nd2 = ppChannel->GetDevice (1);

            if (Names::FindName (nd2->GetNode ()) == failure.get<1> ())
              {
                cout << "Failing " << failure.get<0> () << " <-> " << failure.get<1> () << " link\n";

                Ptr<CcnxFace> face1 = ccnx1->GetFaceByNetDevice (nd1);
                Ptr<CcnxFace> face2 = ccnx2->GetFaceByNetDevice (nd2);

                face1->SetUp (false);
                face2->SetUp (false);

                // set metric to max (for GlobalRouter to know what we want)
                Ptr<Ipv4> ipv4_1 = ccnx1->GetObject<Ipv4> ();
                Ptr<Ipv4> ipv4_2 = ccnx2->GetObject<Ipv4> ();

                uint32_t if1 = ipv4_1->GetInterfaceForDevice (nd1);
                uint32_t if2 = ipv4_2->GetInterfaceForDevice (nd2);

                ipv4_1->SetMetric (if1, std::numeric_limits<uint16_t>::max ());
                ipv4_2->SetMetric (if2, std::numeric_limits<uint16_t>::max ());
                break;
              }
          }
      }
  }

  //We are creating "everybody-to-everybody" usage pattern
  ApplicationContainer
  AddApplications()
  {
    NS_LOG_INFO ("Adding applications");
    NS_LOG_INFO ("GetN = " << reader->GetNodes().GetN());

    double delay = 0;
    
    ApplicationContainer apps;
    for (uint32_t i = 0; i<reader->GetNodes().GetN(); i++)
    {
      NS_LOG_INFO("i="<<i);
      Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (i));
        
      CcnxAppHelper producerHelper ("ns3::CcnxProducer");
      producerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()));
        
      apps.Add (producerHelper.Install (node1));
            
      CcnxAppHelper consumerHelper ("ns3::CcnxConsumerBatches");
      consumerHelper.SetAttribute ("LifeTime", StringValue("100s"));
      consumerHelper.SetAttribute ("Batches", StringValue("2s 1"));
      
      for(uint32_t j = 0; j<reader->GetNodes().GetN();j++)
      {
        NS_LOG_INFO("j="<<j);
        if(i==j)
          continue;
        
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (j));
          
        consumerHelper.SetPrefix ("/" + lexical_cast<string> (node1->GetId ()) + "/" + lexical_cast<string> (node2->GetId ()));
        ApplicationContainer consumer = consumerHelper.Install (node2);
        consumer.Start (Seconds (delay));
        apps.Add (consumer);

        delay += 0.0001;
      }
    }
    
    return apps;
  }

private:
  vector<failures_t> m_failures;
};

int 
main (int argc, char *argv[])
{
  cout << "Begin link failure scenario\n";

  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("100Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("2000"));
  Config::SetDefault ("ns3::RttEstimator::InitialEstimation", StringValue ("0.5s"));
  // Config::SetDefault ("ns3::RttEstimator::MaxMultiplier", StringValue ("16.0")); // original default is 64.0

  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("attributes.xml"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));

  // Config::SetDefault ("ns3::CcnxConsumer::LifeTime", StringValue ("100s"));
  
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

  Experiment experiment (failures);
  for (uint32_t run = startRun; run < startRun + maxRuns; run++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (run));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

      cout << "Run " << run << endl;
      string prefix = "link-failure-"+ failures +"-base-" + lexical_cast<string> (run) + "-";

      experiment.ConfigureTopology ();
      experiment.InstallCcnxStackImpl ();

      CcnxStackHelper ccnxHelper;
      ccnxHelper.InstallFakeGlobalRoutesImpl ();

      experiment.FailLinks (run);

      Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
      ccnxHelper.InstallRoutesToAll ();
      
      ApplicationContainer apps = experiment.AddApplications ();
      cout << "Total number of applications: " << apps.GetN () << "\n";

      //tracing
      CcnxTraceHelper traceHelper;
      // Simulator::Schedule (Seconds (4.5), &CcnxTraceHelper::EnableSeqsAppAll, &traceHelper,
      //                      "ns3::CcnxConsumerBatches", prefix + "consumers-seqs.log");
      Simulator::Schedule (Seconds (0.1), &CcnxTraceHelper::EnablePathWeights, &traceHelper,
                           prefix + "weights.log");

      experiment.Run (Seconds(10.0));
    }

  cout << "Finish link failure scenario\n";
  return 0;
}
