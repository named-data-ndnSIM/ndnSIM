/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author:  Ilya Moiseenko <iliamo@cs.ucla.edu>
 *          Hajime Tazaki (tazaki@sfc.wide.ad.jp)
 *          Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef ROCKETFUEL_MAP_READER_H
#define ROCKETFUEL_MAP_READER_H

#include "ns3/annotated-topology-reader.h"
#include "ns3/net-device-container.h"
#include "ns3/random-variable.h"
#include <set>
#include "ns3/data-rate.h"

#include <boost/graph/adjacency_list.hpp>

using namespace std;

namespace ns3 {

struct RocketfuelParams
{
  double averageRtt;
  int    clientNodeDegrees;

  //parameters for links Backbone <->Backbone
  string minb2bBandwidth;
  string minb2bDelay;

  string maxb2bBandwidth;
  string maxb2bDelay;

  //parameters for links Backbone<->Gateway and Gateway <-> Gateway
  string minb2gBandwidth;
  string minb2gDelay;

  string maxb2gBandwidth;
  string maxb2gDelay;

  //parameters for links Gateway <-> Customer
  string ming2cBandwidth;
  string ming2cDelay;

  string maxg2cBandwidth;
  string maxg2cDelay;
};

/**
 * \brief Topology file reader and topology estimator (extension of Rocketfuel-format type).
 *
 * http://www.cs.washington.edu/research/networking/rocketfuel/
 *
 * Only map file (.cch) is supported
 *
 * In addition to reading specified topology from the .cch file, this class divides nodes into three categories:
 * - client nodes (nodes with degrees less or equal to RocketfuelParams.clientNodeDegrees
 * - gateway nodes (nodes that directly connected to client nodes)
 * - backbone nodes (all the rest)
 *
 * As some of the .cch files do not give a connected network graph, this reader also allows to keep only the largest connected
 * network graph component.
 */
class RocketfuelMapReader : public AnnotatedTopologyReader
{
public:
  RocketfuelMapReader (const std::string &path="", double scale=1.0, const string &referenceOspfRate="100Mbps");
  virtual ~RocketfuelMapReader ();

  /**
   * @brief Deprecated call. Read (RocketfuelParams params, bool keepOneComponent=true, bool connectBackbones=true) should be used instead
   */
  virtual
  NodeContainer
  Read ();

  /**
   * \brief Main topology reading function.
   *
   * @param params parameters specifying range from which link bandwidths and delays should be assigned
   * @param keepOneComponent if true, then only the largest connected component will be kept
   * @param connectBackbones if true, then extra links will be added to ensure connectivity of estimated backbone
   *
   * This method opens an input stream and reads the Rocketfuel-format file.
   * Every row represents a topology link (the ids of a couple of nodes),
   * so the input file is read line by line to figure out how many links
   * and nodes are in the topology.
   *
   * \return the container of the nodes created (or empty container if there was an error)
   */
  virtual NodeContainer
  Read (RocketfuelParams params, bool keepOneComponent=true, bool connectBackbones=true);

  const NodeContainer&
  GetBackboneRouters() const;

  const NodeContainer&
  GetGatewayRouters() const;

  const NodeContainer&
  GetCustomerRouters() const;

  virtual void
  SaveTopology (const std::string &file);

  virtual void
  SaveGraphviz (const std::string &file);

private:
  RocketfuelMapReader (const RocketfuelMapReader&);
  RocketfuelMapReader& operator= (const RocketfuelMapReader&);

  // NodeContainer
  void
  GenerateFromMapsFile (int argc, char *argv[]);

  void
  CreateLink (string nodeName1, string nodeName2,
              double averageRtt,
              const string &minBw, const string &maxBw,
              const string &minDelay, const string &maxDelay);
  void
  KeepOnlyBiggestConnectedComponent ();

  void AssignClients(uint32_t clientDegree, uint32_t gwDegree);

  void
  ConnectBackboneRouters ();

private:
  UniformVariable m_randVar;

  NodeContainer m_backboneRouters;
  NodeContainer m_gatewayRouters;
  NodeContainer m_customerRouters;

  typedef boost::adjacency_list_traits<boost::setS, boost::setS, boost::undirectedS> Traits;

  enum node_type_t {UNKNOWN = 0, CLIENT = 1, GATEWAY = 2, BACKBONE = 3};

  typedef boost::property< boost::vertex_name_t, std::string, boost::property
                           < boost::vertex_index_t, uint32_t, boost::property
                             < boost::vertex_rank_t, node_type_t, boost::property
                               < boost::vertex_color_t, std::string > > > > nodeProperty;

  typedef boost::no_property edgeProperty;

  typedef boost::adjacency_list< boost::setS, boost::setS, boost::undirectedS,
                                 nodeProperty, edgeProperty > Graph;

  typedef map<string, Traits::vertex_descriptor> node_map_t;
  node_map_t m_graphNodes;

  Graph      m_graph;
  uint32_t     m_maxNodeId;

  const DataRate m_referenceOspfRate; // reference rate of OSPF metric calculation


private:
  void
  assignGw (Traits::vertex_descriptor vertex, uint32_t degree, node_type_t nodeType);
}; // end class RocketfuelMapReader

}; // end namespace ns3


#endif /* ROCKETFUEL_MAP_READER_H */
