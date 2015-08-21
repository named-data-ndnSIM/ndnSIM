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

// Based on the code by Hajime Tazaki <tazaki@sfc.wide.ad.jp>

#include "rocketfuel-map-reader.hpp"

#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/fatal-error.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/net-device-container.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-address.h"
#include "ns3/node-list.h"

#include "ns3/mobility-model.h"

#include <regex.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/connected_components.hpp>

#include <iomanip>

using namespace std;
using namespace boost;

NS_LOG_COMPONENT_DEFINE("RocketfuelMapReader");

namespace ns3 {

RocketfuelMapReader::RocketfuelMapReader(const std::string& path /*=""*/, double scale /*=1.0*/,
                                         const std::string& referenceOspfRate)
  : AnnotatedTopologyReader(path, scale)
  , m_randVar(CreateObject<UniformRandomVariable>())
  , m_referenceOspfRate(boost::lexical_cast<DataRate>(referenceOspfRate))
{
}

RocketfuelMapReader::~RocketfuelMapReader()
{
}

NodeContainer
RocketfuelMapReader::Read()
{
  NS_FATAL_ERROR("Deprecated call. Use the other overloaded method instead");
  return NodeContainer();
}

/* uid @loc [+] [bb] (num_neigh) [&ext] -> <nuid-1> <nuid-2> ... {-euid} ... =name[!] rn */

#define REGMATCH_MAX 16

#define START "^"
#define END "$"
#define SPACE "[ \t]+"
#define MAYSPACE "[ \t]*"

#define ROCKETFUEL_MAPS_LINE                                                                       \
  START "(-*[0-9]+)" SPACE "(@[?A-Za-z0-9,+]+)" SPACE "(\\+)*" MAYSPACE "(bb)*" MAYSPACE           \
        "\\(([0-9]+)\\)" SPACE "(&[0-9]+)*" MAYSPACE "->" MAYSPACE "(<[0-9 \t<>]+>)*" MAYSPACE     \
        "(\\{-[0-9\\{\\} \t-]+\\})*" SPACE "=([A-Za-z0-9.!-]+)" SPACE "r([0-9])" MAYSPACE END

void
RocketfuelMapReader::CreateLink(string nodeName1, string nodeName2, double averageRtt,
                                const string& minBw, const string& maxBw, const string& minDelay,
                                const string& maxDelay)
{
  Ptr<Node> node1 = Names::Find<Node>(m_path, nodeName1);
  Ptr<Node> node2 = Names::Find<Node>(m_path, nodeName2);
  Link link(node1, nodeName1, node2, nodeName2);

  DataRate randBandwidth(
    m_randVar->GetInteger(static_cast<uint32_t>(lexical_cast<DataRate>(minBw).GetBitRate()),
                          static_cast<uint32_t>(lexical_cast<DataRate>(maxBw).GetBitRate())));

  int32_t metric = std::max(1, static_cast<int32_t>(1.0 * m_referenceOspfRate.GetBitRate()
                                                    / randBandwidth.GetBitRate()));

  Time randDelay =
    Time::FromDouble((m_randVar->GetValue(lexical_cast<Time>(minDelay).ToDouble(Time::US),
                                          lexical_cast<Time>(maxDelay).ToDouble(Time::US))),
                     Time::US);

  uint32_t queue = ceil(averageRtt * (randBandwidth.GetBitRate() / 8.0 / 1100.0));

  link.SetAttribute("DataRate", boost::lexical_cast<string>(randBandwidth));
  link.SetAttribute("OSPF", boost::lexical_cast<string>(metric));
  link.SetAttribute("Delay",
                    boost::lexical_cast<string>(ceil(randDelay.ToDouble(Time::US))) + "us");
  link.SetAttribute("MaxPackets", boost::lexical_cast<string>(queue));

  AddLink(link);
}

// NodeContainer
void
RocketfuelMapReader::GenerateFromMapsFile(int argc, char* argv[])
{
  string uid;
  string loc;
  string ptr;
  string name;
  string nuid;
  // bool dns = false;
  // bool bb = false;
  int num_neigh_s = 0;
  unsigned int num_neigh = 0;
  int radius = 0;
  vector<string> neigh_list;

  uid = argv[0];
  loc = argv[1];

  // if (argv[2])
  // {
  //   dns = true;
  // }

  // if (argv[3])
  // {
  //   bb = true;
  // }

  num_neigh_s = ::atoi(argv[4]);
  if (num_neigh_s < 0) {
    num_neigh = 0;
    NS_LOG_WARN("Negative number of neighbors given");
  }
  else {
    num_neigh = num_neigh_s;
  }

  /* neighbors */
  if (argv[6]) {
    char* nbr;
    char* stringp = argv[6];
    while ((nbr = strsep(&stringp, " \t")) != NULL) {
      nbr[strlen(nbr) - 1] = '\0';
      neigh_list.push_back(nbr + 1);
    }
  }

  if (num_neigh != neigh_list.size()) {
    NS_LOG_WARN("Given number of neighbors = " << num_neigh << " != size of neighbors list = "
                                               << neigh_list.size());
  }

  /* externs */
  if (argv[7]) {
    //      euid = argv[7];
  }

  /* name */
  if (argv[8]) {
    name = argv[8];
  }

  radius = ::atoi(&argv[9][1]);
  if (radius > 0) {
    return;
  }

  // Create node and link
  if (uid.empty())
    return;

  node_map_t::iterator node = m_graphNodes.find(uid);
  if (node == m_graphNodes.end()) {
    bool ok;
    tie(node, ok) = m_graphNodes.insert(make_pair(uid, add_vertex(nodeProperty(uid), m_graph)));
    NS_ASSERT(ok == true);

    put(vertex_index, m_graph, node->second, m_maxNodeId);
    m_maxNodeId++;
  }

  for (uint32_t i = 0; i < neigh_list.size(); ++i) {
    nuid = neigh_list[i];

    if (nuid.empty()) {
      continue;
    }

    node_map_t::iterator otherNode = m_graphNodes.find(nuid);
    if (otherNode == m_graphNodes.end()) {
      bool ok;
      tie(otherNode, ok) =
        m_graphNodes.insert(make_pair(nuid, add_vertex(nodeProperty(nuid), m_graph)));
      NS_ASSERT(ok == true);

      put(vertex_index, m_graph, otherNode->second, m_maxNodeId);
      m_maxNodeId++;
    }

    // cout << node->second << " <-> " << otherNode->second << endl;
    // parallel edges are disabled in the graph, so no need to worry
    add_edge(node->second, otherNode->second, m_graph);
  }
}

void
RocketfuelMapReader::assignGw(Traits::vertex_descriptor vertex, uint32_t degree,
                              node_type_t nodeType)
{
  graph_traits<Graph>::adjacency_iterator u, endu;
  for (tie(u, endu) = adjacent_vertices(vertex, m_graph); u != endu; u++) {
    if (get(vertex_rank, m_graph, *u) != UNKNOWN)
      continue;

    put(vertex_rank, m_graph, *u, nodeType);
    put(vertex_color, m_graph, *u, "green");

    uint32_t u_degree = out_degree(*u, m_graph);
    if (u_degree < degree)
      assignGw(*u, degree, BACKBONE);
  }
};

void
RocketfuelMapReader::AssignClients(uint32_t clientDegree, uint32_t gwDegree)
{
  graph_traits<Graph>::vertex_iterator v, endv;
  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    uint32_t degree = out_degree(*v, m_graph);
    if (degree == clientDegree) {
      put(vertex_rank, m_graph, *v, CLIENT);
      put(vertex_color, m_graph, *v, "red");

      assignGw(*v, gwDegree + 1, GATEWAY);
    }
  }
};

NodeContainer
RocketfuelMapReader::Read(RocketfuelParams params, bool keepOneComponent /*=true*/,
                          bool connectBackbones /*=true*/)
{
  m_maxNodeId = 0;

  ifstream topgen;
  topgen.open(GetFileName().c_str());
  // NodeContainer nodes;

  istringstream lineBuffer;
  string line;
  int lineNumber = 0;
  char errbuf[512];

  if (!topgen.is_open()) {
    NS_LOG_WARN("Couldn't open the file " << GetFileName());
    return m_nodes;
  }

  while (!topgen.eof()) {
    int ret;
    int argc;
    char* argv[REGMATCH_MAX];
    char* buf;

    lineNumber++;
    line.clear();
    lineBuffer.clear();

    getline(topgen, line);
    buf = (char*)line.c_str();

    regmatch_t regmatch[REGMATCH_MAX];
    regex_t regex;

    ret = regcomp(&regex, ROCKETFUEL_MAPS_LINE, REG_EXTENDED | REG_NEWLINE);
    if (ret != 0) {
      regerror(ret, &regex, errbuf, sizeof(errbuf));
      regfree(&regex);
      continue;
    }

    ret = regexec(&regex, buf, REGMATCH_MAX, regmatch, 0);
    if (ret == REG_NOMATCH) {
      NS_LOG_WARN("match failed (maps file): %s" << buf);
      regfree(&regex);
      continue;
    }

    line = buf;
    argc = 0;

    /* regmatch[0] is the entire strings that matched */
    for (int i = 1; i < REGMATCH_MAX; i++) {
      if (regmatch[i].rm_so == -1) {
        argv[i - 1] = NULL;
      }
      else {
        line[regmatch[i].rm_eo] = '\0';
        argv[i - 1] = &line[regmatch[i].rm_so];
        argc = i;
      }
    }

    GenerateFromMapsFile(argc, argv);
    regfree(&regex);
  }

  if (keepOneComponent) {
    NS_LOG_DEBUG("Before eliminating disconnected nodes: " << num_vertices(m_graph));
    KeepOnlyBiggestConnectedComponent();
    NS_LOG_DEBUG("After eliminating disconnected nodes:  " << num_vertices(m_graph));
  }

  for (int clientDegree = 1; clientDegree <= params.clientNodeDegrees; clientDegree++) {
    AssignClients(clientDegree, std::min(clientDegree, 3));
  }

  graph_traits<Graph>::vertex_iterator v, endv;
  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    node_type_t type = get(vertex_rank, m_graph, *v);
    if (type == UNKNOWN) {
      put(vertex_rank, m_graph, *v, BACKBONE);
      put(vertex_color, m_graph, *v, "blue");
    }
  }

  if (connectBackbones) {
    ConnectBackboneRouters();
  }

  graph_traits<Graph>::edge_iterator e, ende;
  for (tie(e, ende) = edges(m_graph); e != ende;) {
    Traits::vertex_descriptor u = source(*e, m_graph), v = target(*e, m_graph);

    node_type_t u_type = get(vertex_rank, m_graph, u), v_type = get(vertex_rank, m_graph, v);

    if (u_type == BACKBONE && v_type == BACKBONE) {
      // ok
    }
    else if ((u_type == GATEWAY && v_type == BACKBONE)
             || (u_type == BACKBONE && v_type == GATEWAY)) {
      // ok
    }
    else if (u_type == GATEWAY && v_type == GATEWAY) {
      // ok
    }
    else if ((u_type == GATEWAY && v_type == CLIENT) || (u_type == CLIENT && v_type == GATEWAY)) {
      // ok
    }
    else {
      // not ok
      NS_LOG_DEBUG("Wrong link type between nodes: " << u_type << " <-> " << v_type
                                                     << " (deleting the link)");

      graph_traits<Graph>::edge_iterator tmp = e;
      tmp++;

      remove_edge(*e, m_graph);
      e = tmp;
      continue;
    }
    e++;
  }

  if (keepOneComponent) {
    NS_LOG_DEBUG("Before 2 eliminating disconnected nodes: " << num_vertices(m_graph));
    KeepOnlyBiggestConnectedComponent();
    NS_LOG_DEBUG("After 2 eliminating disconnected nodes:  " << num_vertices(m_graph));
  }

  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    string nodeName = get(vertex_name, m_graph, *v);
    Ptr<Node> node = CreateNode(nodeName, 0);

    node_type_t type = get(vertex_rank, m_graph, *v);
    switch (type) {
    case BACKBONE:
      Names::Rename(nodeName, "bb-" + nodeName);
      put(vertex_name, m_graph, *v, "bb-" + nodeName);
      m_backboneRouters.Add(node);
      break;
    case CLIENT:
      Names::Rename(nodeName, "leaf-" + nodeName);
      put(vertex_name, m_graph, *v, "leaf-" + nodeName);
      m_customerRouters.Add(node);
      break;
    case GATEWAY:
      Names::Rename(nodeName, "gw-" + nodeName);
      put(vertex_name, m_graph, *v, "gw-" + nodeName);
      m_gatewayRouters.Add(node);
      break;
    case UNKNOWN:
      NS_FATAL_ERROR("Should not happen");
      break;
    }
  }

  for (tie(e, ende) = edges(m_graph); e != ende; e++) {
    Traits::vertex_descriptor u = source(*e, m_graph), v = target(*e, m_graph);

    node_type_t u_type = get(vertex_rank, m_graph, u), v_type = get(vertex_rank, m_graph, v);

    string u_name = get(vertex_name, m_graph, u), v_name = get(vertex_name, m_graph, v);

    if (u_type == BACKBONE && v_type == BACKBONE) {
      CreateLink(u_name, v_name, params.averageRtt, params.minb2bBandwidth, params.maxb2bBandwidth,
                 params.minb2bDelay, params.maxb2bDelay);
    }
    else if ((u_type == GATEWAY && v_type == BACKBONE)
             || (u_type == BACKBONE && v_type == GATEWAY)) {
      CreateLink(u_name, v_name, params.averageRtt, params.minb2gBandwidth, params.maxb2gBandwidth,
                 params.minb2gDelay, params.maxb2gDelay);
    }
    else if (u_type == GATEWAY && v_type == GATEWAY) {
      CreateLink(u_name, v_name, params.averageRtt, params.minb2gBandwidth, params.maxb2gBandwidth,
                 params.minb2gDelay, params.maxb2gDelay);
    }
    else if ((u_type == GATEWAY && v_type == CLIENT) || (u_type == CLIENT && v_type == GATEWAY)) {
      CreateLink(u_name, v_name, params.averageRtt, params.ming2cBandwidth, params.maxg2cBandwidth,
                 params.ming2cDelay, params.maxg2cDelay);
    }
    else {
      NS_FATAL_ERROR("Wrong link type between nodes: " << u_type << " <-> " << v_type);
    }
  }

  ApplySettings();

  NS_LOG_INFO("Clients:   " << m_customerRouters.GetN());
  NS_LOG_INFO("Gateways:  " << m_gatewayRouters.GetN());
  NS_LOG_INFO("Backbones: " << m_backboneRouters.GetN());
  NS_LOG_INFO("Links:     " << GetLinks().size());

  return m_nodes;
}

const NodeContainer&
RocketfuelMapReader::GetBackboneRouters() const
{
  return m_backboneRouters;
}

const NodeContainer&
RocketfuelMapReader::GetGatewayRouters() const
{
  return m_gatewayRouters;
}

const NodeContainer&
RocketfuelMapReader::GetCustomerRouters() const
{
  return m_customerRouters;
}

static void
nodeWriter(std::ostream& os, NodeContainer& m)
{
  for (NodeContainer::Iterator node = m.Begin(); node != m.End(); node++) {
    std::string name = Names::FindName(*node);

    os << name << "\t"
       << "NA"
       << "\t" << 0 << "\t" << 0 << "\n";
  }
};

void
RocketfuelMapReader::SaveTopology(const std::string& file)
{
  ofstream os(file.c_str(), ios::trunc);
  os << "# any empty lines and lines starting with '#' symbol is ignored\n"
     << "\n"
     << "# The file should contain exactly two sections: router and link, each starting with the "
        "corresponding keyword\n"
     << "\n"
     << "# router section defines topology nodes and their relative positions (e.g., to use in "
        "visualizer)\n"
     << "router\n"
     << "\n"
     << "# each line in this section represents one router and should have the following data\n"
     << "# node  comment     yPos    xPos\n";

  nodeWriter(os, m_backboneRouters);
  nodeWriter(os, m_gatewayRouters);
  nodeWriter(os, m_customerRouters);

  os << "# link section defines point-to-point links between nodes and characteristics of these "
        "links\n"
     << "\n"
     << "link\n"
     << "\n"
     << "# Each line should be in the following format (only first two are required, the rest can "
        "be omitted)\n"
     << "# srcNode   dstNode     bandwidth   metric  delay   queue\n"
     << "# bandwidth: link bandwidth\n"
     << "# metric: routing metric\n"
     << "# delay:  link delay\n"
     << "# queue:  MaxPackets for transmission queue on the link (both directions)\n";

  for (std::list<Link>::iterator link = m_linksList.begin(); link != m_linksList.end(); link++) {
    string src = Names::FindName(link->GetFromNode());
    string dst = Names::FindName(link->GetToNode());
    os << src << "\t";
    os << dst << "\t";

    string tmp;
    if (link->GetAttributeFailSafe("DataRate", tmp))
      os << link->GetAttribute("DataRate") << "\t";
    else
      NS_FATAL_ERROR("DataRate must be specified for the link");

    if (link->GetAttributeFailSafe("OSPF", tmp))
      os << link->GetAttribute("OSPF") << "\t";
    else {
      DataRate rate = boost::lexical_cast<DataRate>(link->GetAttribute("DataRate"));

      int32_t cost = std::max(1, static_cast<int32_t>(1.0 * m_referenceOspfRate.GetBitRate()
                                                      / rate.GetBitRate()));

      os << cost << "\t";
    }

    if (link->GetAttributeFailSafe("Delay", tmp)) {
      os << link->GetAttribute("Delay") << "\t";

      if (link->GetAttributeFailSafe("MaxPackets", tmp)) {
        os << link->GetAttribute("MaxPackets") << "\t";
      }
    }
    os << "\n";
  }
}

/// @cond include_hidden

template<class Names, class Colors>
class name_color_writer {
public:
  name_color_writer(Names _names, Colors _colors)
    : names(_names)
    , colors(_colors)
  {
  }

  template<class VertexOrEdge>
  void
  operator()(std::ostream& out, const VertexOrEdge& v) const
  {
    // out << "[label=\"" << names[v] << "\",style=filled,fillcolor=\"" << colors[v] << "\"]";
    out << "[shape=\"circle\",width=0.1,label=\"\",style=filled,fillcolor=\"" << colors[v] << "\"]";
  }

private:
  Names names;
  Colors colors;
};

template<class Names, class Colors>
inline name_color_writer<Names, Colors>
make_name_color_writer(Names n, Colors c)
{
  return name_color_writer<Names, Colors>(n, c);
}

/// @endcond

void
RocketfuelMapReader::SaveGraphviz(const std::string& file)
{
  ofstream of(file.c_str());
  property_map<Graph, vertex_name_t>::type names = get(vertex_name, m_graph);
  property_map<Graph, vertex_color_t>::type colors = get(vertex_color, m_graph);
  write_graphviz(of, m_graph, make_name_color_writer(names, colors));
}

void
RocketfuelMapReader::KeepOnlyBiggestConnectedComponent()
{
  std::map<graph_traits<Graph>::vertex_descriptor, int> temp;
  associative_property_map<std::map<graph_traits<Graph>::vertex_descriptor, int>> components(temp);

  // //check if topology has breaks in its structure and trim it if yes
  // property_map<Graph, vertex_index1_t>::type components = get (vertex_index1, m_graph);

  int num = connected_components(m_graph, components);
  NS_LOG_DEBUG("Topology has " << num << " components");

  vector<int> sizes(num, 0);

  graph_traits<Graph>::vertex_iterator v, endv;
  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    sizes[get(components, *v)]++;
  }
  int largestComponent = max_element(sizes.begin(), sizes.end()) - sizes.begin();
  // cout << "Largest: " << largestComponent << endl;

  // for (int i =0 ; i < num; i++) cout << sizes[i] << " ";
  // cout << endl;

  ////////////////////////////////////////////////////
  // remove nodes and edges from smaller components //
  ////////////////////////////////////////////////////
  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    if (get(components, *v) == largestComponent)
      continue;

    clear_vertex(*v, m_graph);
  }

  // this works only if vertices are organized in listS or setS (iterator is not invalidated on
  // remove)
  for (tie(v, endv) = vertices(m_graph); v != endv;) {
    if (get(components, *v) == largestComponent) {
      v++;
      continue;
    }

    graph_traits<Graph>::vertex_iterator tmp = v;
    tmp++;

    remove_vertex(*v, m_graph);
    v = tmp;
  }

  int index = 0;
  // renumber nodes
  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    put(vertex_index, m_graph, *v, index++);
  }
}

void
RocketfuelMapReader::ConnectBackboneRouters()
{
  // not the tricky part.  we want backbone to be a fully connected component,
  // so traffic doesn't bounce from backbone to gateway and back

  typedef adjacency_list<setS, setS, boost::undirectedS,
                         property<vertex_name_t, Traits::vertex_descriptor,
                                  property<vertex_index_t, int, property<vertex_index1_t, int>>>>
    BbGraph;
  BbGraph bbGraph;
  map<Traits::vertex_descriptor, graph_traits<BbGraph>::vertex_descriptor> nodeMap;

  int index = 0;

  graph_traits<Graph>::vertex_iterator v, endv;
  for (tie(v, endv) = vertices(m_graph); v != endv; v++) {
    node_type_t type = get(vertex_rank, m_graph, *v);
    if (type == BACKBONE) {
      graph_traits<BbGraph>::vertex_descriptor newv = add_vertex(*v, bbGraph);
      put(vertex_index, bbGraph, newv, index++);
      nodeMap[*v] = newv;
    }
  }

  graph_traits<BbGraph>::vertex_iterator bb, endBb;
  for (tie(bb, endBb) = vertices(bbGraph); bb != endBb; bb++) {
    Traits::vertex_descriptor actualVertex = get(vertex_name, bbGraph, *bb);

    graph_traits<Graph>::adjacency_iterator u, endu;
    for (tie(u, endu) = adjacent_vertices(actualVertex, m_graph); u != endu; u++) {
      if (nodeMap.find(*u) != nodeMap.end()) {
        add_edge(nodeMap[actualVertex], nodeMap[*u], bbGraph);
      }
    }
  }

  property_map<BbGraph, vertex_index1_t>::type components = get(vertex_index1, bbGraph);

  int num = connected_components(bbGraph, components);
  NS_LOG_DEBUG("Backbone has " << num << " components");
  if (num == 1)
    return; // nothing to do

  vector<vector<graph_traits<BbGraph>::vertex_descriptor>> subgraphs(num);
  for (tie(bb, endBb) = vertices(bbGraph); bb != endBb; bb++) {
    int component = get(vertex_index1, bbGraph, *bb);
    subgraphs[component].push_back(*bb);
  }

  Ptr<UniformRandomVariable> randVar = CreateObject<UniformRandomVariable>();

  for (int i = 1; i < num; i++) {
    int node1 = randVar->GetInteger(0, subgraphs[i - 1].size() - 1);
    int node2 = randVar->GetInteger(0, subgraphs[i].size() - 1);

    Traits::vertex_descriptor v1 = get(vertex_name, bbGraph, subgraphs[i - 1][node1]),
                              v2 = get(vertex_name, bbGraph, subgraphs[i][node2]);

    NS_LOG_DEBUG("Connecting " << get(vertex_name, m_graph, v1) << "[" << node1 << "] with "
                               << get(vertex_name, m_graph, v2) << "[" << node2 << "]");

    add_edge(v1, v2, m_graph);
  }
}

} /* namespace ns3 */
