/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef BASE_EXPERIMENT_H
#define BASE_EXPERIMENT_H

#include "ns3/rocketfuel-topology-reader.h"

class BaseExperiment
{
public:
  BaseExperiment ()
    : m_rand (0,52)
    , reader (0)
    , m_numNodes (52)
  { }

  ~BaseExperiment ()
  {
    if (reader != 0) delete reader;
  }
    
  void
  ConfigureTopology ()
  {
    Names::Clear ();
    cout << "Configure Topology\n";
    if (reader != 0) delete reader;
    reader = new RocketfuelWeightsReader ("/sprint");
    
    string weights   ("./src/NDNabstraction/examples/sprint-pops.weights");
    string latencies ("./src/NDNabstraction/examples/sprint-pops.latencies");
    string positions ("./src/NDNabstraction/examples/sprint-pops.positions");
  
    reader->SetFileName (positions);
    reader->SetFileType (RocketfuelWeightsReader::POSITIONS);
    reader->Read ();
  
    reader->SetFileName (weights);
    reader->SetFileType (RocketfuelWeightsReader::WEIGHTS);    
    reader->Read ();

    reader->SetFileName (latencies);
    reader->SetFileType (RocketfuelWeightsReader::LATENCIES);    
    reader->Read ();
    
    reader->Commit ();
  }

  void InstallCcnxStack (bool installFIBs = true)
  {
    InternetStackHelper stack;
    Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
    stack.SetRoutingHelper (ipv4RoutingHelper);
    stack.Install (reader->GetNodes ());

    reader->AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));
    
    // Install CCNx stack
    cout << "Installing CCNx stack\n";
    CcnxStackHelper ccnxHelper;
    ccnxHelper.SetForwardingStrategy ("ns3::CcnxBestRouteStrategy");
    ccnxHelper.EnableLimits (true, Seconds(0.1));
    ccnxHelper.SetDefaultRoutes (false);
    ccnxHelper.InstallAll ();

    ccnxHelper.InstallFakeGlobalRoutes ();
    if (installFIBs)
      {
        // // Populate FIB based on IPv4 global routing controller
        ccnxHelper.InstallRoutesToAll ();
      }
  }
  
  void InstallIpStack ()
  {
    InternetStackHelper stack;
    stack.Install (reader->GetNodes ());
    reader->AssignIpv4Addresses (Ipv4Address ("10.0.0.0"));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  }

  void
  GenerateRandomPairs (uint16_t numStreams)
  {
    m_pairs.clear ();
    // map<uint32_t, set<uint32_t> > streams;
    m_usedNodes.clear ();
    
    uint16_t createdStreams = 0;
    uint16_t guard = 0;
    while (createdStreams < numStreams && guard < (numeric_limits<uint16_t>::max ()-1))
      {
        guard ++;

        uint32_t node1_num = m_rand.GetValue (); //43;//
        uint32_t node2_num = m_rand.GetValue (); //38;//

        if (node1_num == node2_num)
          continue;

        if (m_usedNodes.count (node1_num) > 0 ||
            m_usedNodes.count (node2_num) > 0 )
          {
            continue; // don't reuse nodes
          }

        m_usedNodes.insert (node1_num);
        m_usedNodes.insert (node2_num);

        m_pairs.push_back (make_tuple (node1_num, node2_num));
        createdStreams ++;
      }
  }

  void
  Run (const Time &finishTime)
  {
    cout << "Run Simulation.\n";
    Simulator::Stop (finishTime);
    // Simulator::Schedule (Seconds (1.0), PrintTime);
    Simulator::Run ();
    Simulator::Destroy ();
    cout << "Done.\n";
  }

  UniformVariable m_rand;
  RocketfuelWeightsReader *reader;

  list<tuple<uint32_t,uint32_t> > m_pairs;
  set<uint32_t> m_usedNodes;
  const int m_numNodes;
};

#endif
