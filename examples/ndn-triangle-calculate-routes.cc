/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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
// ndn-triangle-calculate-routes.cc
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

using namespace ns3;
using namespace std;

int 
main (int argc, char *argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
  Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse (argc, argv);

  ofstream file1 ("/tmp/topo1.txt");
  file1 << "router\n\n"
        << "#node	city	y	x	mpi-partition\n"
        << "A1	NA	1	1	1\n"
        << "B1	NA	80	-40	1\n"
        << "C1	NA	80	40	1\n"
        << "A2	NA	1	1	1\n"
        << "B2	NA	80	-40	1\n"
        << "C2	NA	80	40	1\n\n"
        << "link\n\n"
        << "# from  to  capacity	metric	delay	queue\n"
        << "A1	    B1	10Mbps		100	1ms	100\n"
        << "A1	    C1	10Mbps		50	1ms	100\n"
        << "B1	    C1	10Mbps		1	1ms	100\n"
        << "A2	    B2	10Mbps		50	1ms	100\n"
        << "A2	    C2	10Mbps		100	1ms	100\n"
        << "B2	    C2	10Mbps		1	1ms	100\n";
  file1.close ();

  AnnotatedTopologyReader topologyReader ("");
  topologyReader.SetFileName ("/tmp/topo1.txt");
  topologyReader.Read ();

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll ();

  topologyReader.ApplyOspfMetric ();

  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll ();

  ndnGlobalRoutingHelper.AddOrigins ("/test/prefix", Names::Find<Node> ("C1"));
  ndnGlobalRoutingHelper.AddOrigins ("/test/prefix", Names::Find<Node> ("C2"));
  ndn::GlobalRoutingHelper::CalculateRoutes ();

  cout << "FIB content on node A1" << endl;
  Ptr<ndn::Fib> fib = Names::Find<Node> ("A1")->GetObject<ndn::Fib> ();
  for (Ptr<ndn::fib::Entry> entry = fib->Begin (); entry != fib->End (); entry = fib->Next (entry))
    {
      cout << *entry << " (this is towards: ";
      cout << Names::FindName (DynamicCast<const ndn::NetDeviceFace> (entry->FindBestCandidate (0).GetFace ())->GetNetDevice ()->GetChannel ()->GetDevice (1)->GetNode ());
      cout << ")" << endl;
    }

  cout << "FIB content on node A2" << endl;
  fib = Names::Find<Node> ("A2")->GetObject<ndn::Fib> ();
  for (Ptr<ndn::fib::Entry> entry = fib->Begin (); entry != fib->End (); entry = fib->Next (entry))
    {
      cout << *entry << " (this is towards: ";
      cout << Names::FindName (DynamicCast<const ndn::NetDeviceFace> (entry->FindBestCandidate (0).GetFace ())->GetNetDevice ()->GetChannel ()->GetDevice (1)->GetNode ());
      cout << ")" << endl;
    }
    
  Simulator::Stop (Seconds (20.0));
  Simulator::Run ();
  Simulator::Destroy ();
  
  return 0;
}
