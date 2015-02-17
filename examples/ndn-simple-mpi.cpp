/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// ndn-simple-mpi.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/mpi-interface.h"

#ifdef NS3_MPI
#include <mpi.h>
#else
#error "ndn-simple-mpi scenario can be compiled only if NS3_MPI is enabled"
#endif

namespace ns3 {

/**
 * This scenario simulates a very simple network topology using mpi:
 *
 *
 *      +----------+     1 Mbps     +----------+
 *      | consumer | <------------> | producer |
 *      +----------+      10ms      +----------+
 *
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     NS_LOG=ndn.Consumer:ndn.Producer mpirun -np 2 ./waf --run=ndn-simple-mpi
 *
 * The default parallel synchronization strategy implemented in the
 * DistributedSimulatorImpl class is based on a globally synchronized algorithm
 * using an MPI collective operation to synchronize simulation time across all LPs.
 * A second synchronization strategy based on local communication and null messages
 * is implemented in the NullMessageSimulatorImpl class, For the null message strategy
 * the global all to all gather is not required; LPs only need to communication with
 * LPs that have shared point-to-point links. The algorithm to use is controlled by
 * which the ns-3 global value SimulatorImplementationType.
 *
 * The strategy can be selected according to the value of nullmsg. If nullmsg is true,
 * then the local communication strategy is selected. If nullmsg is false, then the
 * globally synchronized strategy is selected. This parameter can be passed either
 * as a command line argument or by directly modifying the simulation scenario.
 *
 */

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Gbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("1ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("10"));

  bool nullmsg = false;

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.AddValue("nullmsg", "Enable the use of null-message synchronization", nullmsg);
  cmd.Parse(argc, argv);

  // Distributed simulation setup; by default use granted time window algorithm.
  if (nullmsg) {
    GlobalValue::Bind("SimulatorImplementationType",
                      StringValue("ns3::NullMessageSimulatorImpl"));
  }
  else {
    GlobalValue::Bind("SimulatorImplementationType",
                      StringValue("ns3::DistributedSimulatorImpl"));
  }

  // Enable parallel simulator with the command line arguments
  MpiInterface::Enable(&argc, &argv);

  uint32_t systemId = MpiInterface::GetSystemId();
  uint32_t systemCount = MpiInterface::GetSize();

  if (systemCount != 2)  {
    std::cout << "Simulation will run on a single processor only" << std::endl
              << "To run using MPI, run" << std::endl
              << "  mpirun -np 2 ./waf --run=ndn-simple-mpi" << std::endl;
  }

  // Creating nodes

  // consumer node is associated with system id 0
  Ptr<Node> node1 = CreateObject<Node>(0);

  // producer node is associated with system id 1 (or 0 when running on single CPU)
  Ptr<Node> node2 = CreateObject<Node>(systemCount == 2 ? 1 : 0);

  // Connecting nodes using a link
  PointToPointHelper p2p;
  p2p.Install(node1, node2);

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.InstallAll();

  ndn::FibHelper::AddRoute(node1, "/prefix/1", node2, 1);
  ndn::FibHelper::AddRoute(node2, "/prefix/2", node1, 1);

  // Installing applications
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
  consumerHelper.SetAttribute("Frequency", StringValue("100")); // 10 interests a second

  ndn::AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

  // Run consumer application on the first processor only (if running on 2 CPUs)
  if (systemCount != 2 || systemId == 0) {
    consumerHelper.SetPrefix("/prefix/1"); // request /prefix/1/*
    consumerHelper.Install(node1);

    producerHelper.SetPrefix("/prefix/2"); // serve /prefix/2/*
    producerHelper.Install(node1);

    ndn::L3RateTracer::Install(node1, "node1.txt", Seconds(0.5));
  }

  // Run consumer application on the second processor only (if running on 2 CPUs)
  if (systemCount != 2 || systemId == 1) {
    // Producer
    consumerHelper.SetPrefix("/prefix/2"); // request /prefix/2/*
    consumerHelper.Install(node2);

    producerHelper.SetPrefix("/prefix/1"); // serve /prefix/1/*
    producerHelper.Install(node2);

    ndn::L3RateTracer::Install(node2, "node2.txt", Seconds(0.5));
  }

  Simulator::Stop(Seconds(400.0));

  Simulator::Run();
  Simulator::Destroy();

  MpiInterface::Disable();
  return 0;
}

} // namespace ns3


int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
