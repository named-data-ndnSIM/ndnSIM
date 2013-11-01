/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 *         Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/log.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/random-variable.h"

#include <boost/foreach.hpp>

#include "ns3/ndnSIM/plugins/topology/rocketfuel-map-reader.h"

using namespace ns3;
using namespace std;

int main (int argc, char**argv)
{
  string topology = "";
  string output_prefix = "";
  uint32_t run = 0;
  int clientNodeDegrees = -1;
  bool buildGraphvizGraph = false;
  bool keepLargestComponent = false;
  bool connectBackbones = false;

  CommandLine cmd;
  cmd.AddValue ("topology", "Topology filename (.ccn)", topology);
  cmd.AddValue ("output",   "Prefix for output files", output_prefix);
  cmd.AddValue ("run", "Run for ranged parameter randomization", run);
  cmd.AddValue ("clients", "Maximum degree of client nodes", clientNodeDegrees);
  cmd.AddValue ("buildGraph", "Whether or not build a graphviz graph (using neato)", buildGraphvizGraph);
  cmd.AddValue ("keepLargestComponent", "Keep only largest connected component of the network graph", keepLargestComponent);
  cmd.AddValue ("connectBackbones", "Make sure that ``backbone'' nodes are connected (adding extra links)", connectBackbones);
  cmd.Parse (argc, argv);

  /**
   * @todo Make range parameters as command line arguments
   */

  if (topology == "")
    {
      cerr << "ERROR: topology needs to be specified" << endl;
      cerr << endl;

      cmd.PrintHelp (cerr);
      return 1;
    }

  if (output_prefix == "")
    {
      cerr << "ERROR: output needs to be specified" << endl;
      cerr << endl;

      cmd.PrintHelp (cerr);
      return 1;
    }

  if (run == 0)
    {
      cerr << "ERROR: run needs to be specified" << endl;
      cerr << endl;

      cmd.PrintHelp (cerr);
      return 1;
    }

  if (clientNodeDegrees < 0)
    {
      cerr << "ERROR: clients needs to be specified" << endl;
      cerr << endl;

      cmd.PrintHelp (cerr);
      return 1;
    }

  Config::SetGlobal ("RngRun", IntegerValue (run));
  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::VisualSimulatorImpl"));

  string input = topology;
  string output = output_prefix+"-conv-annotated.txt";
  string graph  = output_prefix+"-conv-annotated.dot";
  string graph_pdf  = output_prefix+"-conv-annotated.pdf";

  RocketfuelParams params;
  params.clientNodeDegrees = clientNodeDegrees;
  params.averageRtt = 0.25; // 250ms
  //parameters for links Backbone<->Backbone
  params.minb2bBandwidth = "40Mbps";
  params.minb2bDelay = "5ms";

  params.maxb2bBandwidth = "100Mbps";
  params.maxb2bDelay = "10ms";

  //parameters for links Backbone<->Gateway and Gateway <-> Gateway
  params.minb2gBandwidth = "10Mbps";
  params.minb2gDelay = "5ms";

  params.maxb2gBandwidth = "20Mbps";
  params.maxb2gDelay = "10ms";

  //parameters for links Gateway <-> Customer
  params.ming2cBandwidth ="1Mbps";
  params.ming2cDelay = "70ms";

  params.maxg2cBandwidth ="3Mbps";
  params.maxg2cDelay = "10ms";

  RocketfuelMapReader topologyReader ("/", 1.0);
  topologyReader.SetFileName (input);
  NodeContainer nodes = topologyReader.Read (params, keepLargestComponent, connectBackbones);
  NodeContainer backboneRouters = topologyReader.GetBackboneRouters();
  NodeContainer gatewayRouters = topologyReader.GetGatewayRouters();
  NodeContainer customerRouters = topologyReader.GetCustomerRouters();
  list<TopologyReader::Link> links = topologyReader.GetLinks();

  topologyReader.SaveGraphviz (graph);
  if (buildGraphvizGraph)
    {
      int ret = system (("neato -Tpdf \"" + graph + "\" > \"" + graph_pdf + "\"").c_str ());
      if (ret != 0)
        {
          std::cerr << "WARN: failed to build a graph for the topology. Check if `neato' command is installed (part of graphviz package)" << std::endl;
        }
    }

  topologyReader.SaveTopology (output);

  // Names::Clear ();
  // Simulator::Destroy ();

  // // assign weights
  // AnnotatedTopologyReader topologyReader2 ("", 1.0);
  // topologyReader2.SetFileName (output);
  // topologyReader2.Read ();

  // char env[] = "NS_VIS_ASSIGN=1";
  // putenv (env);

  // Simulator::Stop (Seconds (0.0));
  // Simulator::Run ();

  // topologyReader2.SaveTopology (output);

  // Simulator::Destroy ();
  return 0;
}
