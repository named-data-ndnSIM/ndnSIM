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
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace ns3;
using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("Scenario");

// void PrintTime ()
// {
//   cout << "Progress: " << Simulator::Now ().ToDouble (Time::S) << "s" << endl;

//   Simulator::Schedule (Seconds (1.0), PrintTime);
// }

#include "base-experiment.h"

class Experiment : public BaseExperiment
{
public:
  ApplicationContainer
  AddCcnxApplications ()
  {
    ApplicationContainer apps;

    for (list<tuple<uint32_t,uint32_t> >::iterator i = m_pairs.begin (); i != m_pairs.end (); i++)
      {
        uint32_t node1_num = i->get<0> ();
        uint32_t node2_num = i->get<1> ();

        Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (node1_num));
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (node2_num));

        CcnxAppHelper consumerHelper ("ns3::CcnxConsumerWindow");
        consumerHelper.SetPrefix ("/" + lexical_cast<string> (node2->GetId ()));
        // consumerHelper.SetAttribute ("MeanRate", StringValue ("2Mbps"));
        consumerHelper.SetAttribute ("Size", StringValue ("1.983642578125")); //to make sure max seq # is 2000

        CcnxAppHelper producerHelper ("ns3::CcnxProducer");
        producerHelper.SetPrefix ("/" + lexical_cast<string> (node2->GetId ()));
        
        apps.Add
          (consumerHelper.Install (node1));

        apps.Add
          (producerHelper.Install (node2));
      }

    return apps;
  }

  ApplicationContainer
  AddTcpApplications ()
  {
    ApplicationContainer apps;

    uint32_t streamId = 0;
    const static uint32_t base_port = 10;
    for (list<tuple<uint32_t,uint32_t> >::iterator i = m_pairs.begin (); i != m_pairs.end (); i++)
      {
        uint32_t node1_num = i->get<0> ();
        uint32_t node2_num = i->get<1> ();

        Ptr<Node> node1 = Names::Find<Node> ("/sprint", lexical_cast<string> (node2_num));
        Ptr<Node> node2 = Names::Find<Node> ("/sprint", lexical_cast<string> (node1_num));

        Ptr<Ipv4> ipv4 = node1->GetObject<Ipv4> ();
        // ipv4->GetAddress (0, 0);

        // to make sure we don't reuse the same port numbers for different flows, just make all port numbers unique
        PacketSinkHelper consumerHelper ("ns3::TcpSocketFactory",
                                         InetSocketAddress (Ipv4Address::GetAny (), base_port + streamId));

        BulkSendHelper producerHelper ("ns3::TcpSocketFactory",
                                       InetSocketAddress (ipv4->GetAddress (1, 0).GetLocal (), base_port + streamId));
        // cout << "SendTo: " <<  ipv4->GetAddress (1, 0).GetLocal () << endl;
        producerHelper.SetAttribute ("MaxBytes", UintegerValue (2081040)); // equal to 2001 ccnx packets
        
        apps.Add
          (consumerHelper.Install (node1));

        apps.Add
          (producerHelper.Install (node2));

        streamId++;
      }

    return apps;
  }
};


int 
main (int argc, char *argv[])
{
  cout << "Begin congestion-pop scenario\n";
  
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("60"));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", StringValue ("1040"));
  
  Config::SetDefault ("ns3::BulkSendApplication::SendSize", StringValue ("1040"));

  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("attributes.xml"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("Xml"));
  
  uint32_t maxRuns = 1;
  uint32_t startRun = 0;
  CommandLine cmd;
  cmd.AddValue ("start", "Initial run number", startRun);
  cmd.AddValue ("runs", "Number of runs", maxRuns);
  cmd.Parse (argc, argv);

  // ConfigStore config;
  // config.ConfigureDefaults ();
  
  for (uint32_t run = startRun; run < startRun + maxRuns; run++)
    {
      Config::SetGlobal ("RngRun", IntegerValue (run));
      cout << "seed = " << SeedManager::GetSeed () << ", run = " << SeedManager::GetRun () << endl;

      Experiment experiment;
      cout << "Run " << run << endl;
      string prefix = "run-" + lexical_cast<string> (run) + "-";

      experiment.GenerateRandomPairs (20);
      ofstream of_nodes ((prefix + "apps.log").c_str ());
      for (list<tuple<uint32_t,uint32_t> >::iterator i = experiment.m_pairs.begin (); i != experiment.m_pairs.end (); i++)
        {
          of_nodes << "From " << i->get<0> ()
                   << " to "  << i->get<1> ();
          of_nodes << "\n";
        }
      of_nodes.close ();

      cout << "NDN experiment\n";
      // NDN
      {
        experiment.ConfigureTopology ();
        experiment.InstallCcnxStack ();
        ApplicationContainer apps = experiment.AddCcnxApplications ();

        for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
          {
            apps.Get (i*2)->SetStartTime (Seconds (1+i));
            apps.Get (i*2 + 1)->SetStartTime (Seconds (1+i));
          }

        CcnxTraceHelper traceHelper;
        // traceHelper.EnableRateL3All (prefix + "rate-trace.log");
        // traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumerCbr", prefix + "consumers-seqs.log");
        traceHelper.EnableSeqsAppAll ("ns3::CcnxConsumerWindow", prefix + "consumers-seqs.log");
        traceHelper.EnableWindowsAll (prefix + "windows.log");

        // config.ConfigureAttributes ();
        experiment.Run (Seconds (200.0));
      }

      cout << "TCP experiment\n";
      // TCP
      {
        experiment.ConfigureTopology ();
        experiment.InstallIpStack ();
        ApplicationContainer apps = experiment.AddTcpApplications ();

        CcnxTraceHelper traceHelper;
        traceHelper.EnableIpv4SeqsAppAll (prefix + "tcp-consumers-seqs.log");
        traceHelper.EnableWindowsTcpAll (prefix + "tcp-windows.log");

        for (uint32_t i = 0; i < apps.GetN () / 2; i++) 
          {
            apps.Get (i*2)->SetStartTime (Seconds (1+i));

            apps.Get (i*2 + 1)->SetStartTime (Seconds (1+i));

            // cout << "Node: " << apps.Get (i*2 + 1)->GetNode ()->GetId () << "\n";
            // care only about BulkSender
            Simulator::Schedule (Seconds (1+i+0.01),
                                 &CcnxTraceHelper::TcpConnect, &traceHelper, apps.Get (i*2)->GetNode ());

            Simulator::Schedule (Seconds (1+i+0.01),
                                 &CcnxTraceHelper::TcpConnect, &traceHelper, apps.Get (i*2 + 1)->GetNode ());
          }

        experiment.Run (Seconds (200.0));
      }
    }

  // cout << "Finish congestion-pop scenario\n";
  return 0;
}
