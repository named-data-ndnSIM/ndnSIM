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

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#endif

#include "ndn-global-routing-helper.hpp"

#include "model/ndn-l3-protocol.hpp"
#include "helper/ndn-fib-helper.hpp"
#include "model/ndn-net-device-face.hpp"
#include "model/ndn-global-router.hpp"

#include "daemon/table/fib.hpp"
#include "daemon/fw/forwarder.hpp"
#include "daemon/table/fib-entry.hpp"
#include "daemon/table/fib-nexthop.hpp"

#include "ns3/object.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/node-list.h"
#include "ns3/channel-list.h"
#include "ns3/object-factory.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/concept/assert.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <unordered_map>

#include "boost-graph-ndn-global-routing-helper.hpp"

#include <math.h>

NS_LOG_COMPONENT_DEFINE("ndn.GlobalRoutingHelper");

namespace ns3 {
namespace ndn {

void
GlobalRoutingHelper::Install(Ptr<Node> node)
{
  NS_LOG_LOGIC("Node: " << node->GetId());

  Ptr<L3Protocol> ndn = node->GetObject<L3Protocol>();
  NS_ASSERT_MSG(ndn != 0, "Cannot install GlobalRoutingHelper before Ndn is installed on a node");

  Ptr<GlobalRouter> gr = node->GetObject<GlobalRouter>();
  if (gr != 0) {
    NS_LOG_DEBUG("GlobalRouter is already installed: " << gr);
    return; // already installed
  }

  gr = CreateObject<GlobalRouter>();
  node->AggregateObject(gr);

  for (auto& i : ndn->getForwarder()->getFaceTable()) {
    shared_ptr<NetDeviceFace> face = std::dynamic_pointer_cast<NetDeviceFace>(i);
    if (face == 0) {
      NS_LOG_DEBUG("Skipping non-netdevice face");
      continue;
    }

    Ptr<NetDevice> nd = face->GetNetDevice();
    if (nd == 0) {
      NS_LOG_DEBUG("Not a NetDevice associated with NetDeviceFace");
      continue;
    }

    Ptr<Channel> ch = nd->GetChannel();

    if (ch == 0) {
      NS_LOG_DEBUG("Channel is not associated with NetDevice");
      continue;
    }

    if (ch->GetNDevices() == 2) // e.g., point-to-point channel
    {
      for (uint32_t deviceId = 0; deviceId < ch->GetNDevices(); deviceId++) {
        Ptr<NetDevice> otherSide = ch->GetDevice(deviceId);
        if (nd == otherSide)
          continue;

        Ptr<Node> otherNode = otherSide->GetNode();
        NS_ASSERT(otherNode != 0);

        Ptr<GlobalRouter> otherGr = otherNode->GetObject<GlobalRouter>();
        if (otherGr == 0) {
          Install(otherNode);
        }
        otherGr = otherNode->GetObject<GlobalRouter>();
        NS_ASSERT(otherGr != 0);
        gr->AddIncidency(face, otherGr);
      }
    }
    else {
      Ptr<GlobalRouter> grChannel = ch->GetObject<GlobalRouter>();
      if (grChannel == 0) {
        Install(ch);
      }
      grChannel = ch->GetObject<GlobalRouter>();

      gr->AddIncidency(face, grChannel);
    }
  }
}

void
GlobalRoutingHelper::Install(Ptr<Channel> channel)
{
  NS_LOG_LOGIC("Channel: " << channel->GetId());

  Ptr<GlobalRouter> gr = channel->GetObject<GlobalRouter>();
  if (gr != 0)
    return;

  gr = CreateObject<GlobalRouter>();
  channel->AggregateObject(gr);

  for (uint32_t deviceId = 0; deviceId < channel->GetNDevices(); deviceId++) {
    Ptr<NetDevice> dev = channel->GetDevice(deviceId);

    Ptr<Node> node = dev->GetNode();
    NS_ASSERT(node != 0);

    Ptr<GlobalRouter> grOther = node->GetObject<GlobalRouter>();
    if (grOther == 0) {
      Install(node);
    }
    grOther = node->GetObject<GlobalRouter>();
    NS_ASSERT(grOther != 0);

    gr->AddIncidency(0, grOther);
  }
}

void
GlobalRoutingHelper::Install(const NodeContainer& nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin(); node != nodes.End(); node++) {
    Install(*node);
  }
}

void
GlobalRoutingHelper::InstallAll()
{
  Install(NodeContainer::GetGlobal());
}

void
GlobalRoutingHelper::AddOrigin(const std::string& prefix, Ptr<Node> node)
{
  Ptr<GlobalRouter> gr = node->GetObject<GlobalRouter>();
  NS_ASSERT_MSG(gr != 0, "GlobalRouter is not installed on the node");

  auto name = make_shared<Name>(prefix);
  gr->AddLocalPrefix(name);
}

void
GlobalRoutingHelper::AddOrigins(const std::string& prefix, const NodeContainer& nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin(); node != nodes.End(); node++) {
    AddOrigin(prefix, *node);
  }
}

void
GlobalRoutingHelper::AddOrigin(const std::string& prefix, const std::string& nodeName)
{
  Ptr<Node> node = Names::Find<Node>(nodeName);
  NS_ASSERT_MSG(node != 0, nodeName << "is not a Node");

  AddOrigin(prefix, node);
}

void
GlobalRoutingHelper::AddOriginsForAll()
{
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    Ptr<GlobalRouter> gr = (*node)->GetObject<GlobalRouter>();
    std::string name = Names::FindName(*node);

    if (gr != 0 && !name.empty()) {
      AddOrigin("/" + name, *node);
    }
  }
}

void
GlobalRoutingHelper::CalculateRoutes()
{
  /**
   * Implementation of route calculation is heavily based on Boost Graph Library
   * See http://www.boost.org/doc/libs/1_49_0/libs/graph/doc/table_of_contents.html for more details
   */

  BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<boost::NdnGlobalRouterGraph>));
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<boost::NdnGlobalRouterGraph>));

  boost::NdnGlobalRouterGraph graph;
  // typedef graph_traits < NdnGlobalRouterGraph >::vertex_descriptor vertex_descriptor;

  // For now we doing Dijkstra for every node.  Can be replaced with Bellman-Ford or Floyd-Warshall.
  // Other algorithms should be faster, but they need additional EdgeListGraph concept provided by
  // the graph, which
  // is not obviously how implement in an efficient manner
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    Ptr<GlobalRouter> source = (*node)->GetObject<GlobalRouter>();
    if (source == 0) {
      NS_LOG_DEBUG("Node " << (*node)->GetId() << " does not export GlobalRouter interface");
      continue;
    }

    boost::DistancesMap distances;

    dijkstra_shortest_paths(graph, source,
                            // predecessor_map (boost::ref(predecessors))
                            // .
                            distance_map(boost::ref(distances))
                              .distance_inf(boost::WeightInf)
                              .distance_zero(boost::WeightZero)
                              .distance_compare(boost::WeightCompare())
                              .distance_combine(boost::WeightCombine()));

    // NS_LOG_DEBUG (predecessors.size () << ", " << distances.size ());

    Ptr<L3Protocol> L3protocol = (*node)->GetObject<L3Protocol>();
    shared_ptr<nfd::Forwarder> forwarder = L3protocol->getForwarder();

    NS_LOG_DEBUG("Reachability from Node: " << source->GetObject<Node>()->GetId());
    for (const auto& dist : distances) {
      if (dist.first == source)
        continue;
      else {
        // cout << "  Node " << dist.first->GetObject<Node> ()->GetId ();
        if (std::get<0>(dist.second) == 0) {
          // cout << " is unreachable" << endl;
        }
        else {
          for (const auto& prefix : dist.first->GetLocalPrefixes()) {
            NS_LOG_DEBUG(" prefix " << prefix << " reachable via face " << *std::get<0>(dist.second)
                         << " with distance " << std::get<1>(dist.second) << " with delay "
                         << std::get<2>(dist.second));

            FibHelper::AddRoute(*node, *prefix, std::get<0>(dist.second),
                                std::get<1>(dist.second));
          }
        }
      }
    }
  }
}

void
GlobalRoutingHelper::CalculateAllPossibleRoutes()
{
  /**
   * Implementation of route calculation is heavily based on Boost Graph Library
   * See http://www.boost.org/doc/libs/1_49_0/libs/graph/doc/table_of_contents.html for more details
   */

  BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<boost::NdnGlobalRouterGraph>));
  BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<boost::NdnGlobalRouterGraph>));

  boost::NdnGlobalRouterGraph graph;
  // typedef graph_traits < NdnGlobalRouterGraph >::vertex_descriptor vertex_descriptor;

  // For now we doing Dijkstra for every node.  Can be replaced with Bellman-Ford or Floyd-Warshall.
  // Other algorithms should be faster, but they need additional EdgeListGraph concept provided by
  // the graph, which
  // is not obviously how implement in an efficient manner
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    Ptr<GlobalRouter> source = (*node)->GetObject<GlobalRouter>();
    if (source == 0) {
      NS_LOG_DEBUG("Node " << (*node)->GetId() << " does not export GlobalRouter interface");
      continue;
    }

    Ptr<L3Protocol> L3protocol = (*node)->GetObject<L3Protocol>();
    shared_ptr<nfd::Forwarder> forwarder = L3protocol->getForwarder();

    NS_LOG_DEBUG("Reachability from Node: " << source->GetObject<Node>()->GetId() << " ("
                                            << Names::FindName(source->GetObject<Node>()) << ")");

    Ptr<L3Protocol> l3 = source->GetObject<L3Protocol>();
    NS_ASSERT(l3 != 0);

    // remember interface statuses
    std::list<nfd::FaceId> faceIds;
    std::unordered_map<nfd::FaceId, uint16_t> originalMetrics;
    for (auto& i : l3->getForwarder()->getFaceTable()) {
      shared_ptr<Face> nfdFace = std::dynamic_pointer_cast<Face>(i);
      faceIds.push_back(nfdFace->getId());
      originalMetrics[nfdFace->getId()] = nfdFace->getMetric();
      nfdFace->setMetric(std::numeric_limits<uint16_t>::max() - 1);
      // value std::numeric_limits<uint16_t>::max () MUST NOT be used (reserved)
    }

    for (auto& faceId : faceIds) {
      shared_ptr<Face> k = l3->getForwarder()->getFaceTable().get(faceId);
      shared_ptr<NetDeviceFace> face = std::dynamic_pointer_cast<NetDeviceFace>(k);
      if (face == 0) {
        NS_LOG_DEBUG("Skipping non-netdevice face");
        continue;
      }

      // enabling only faceId
      face->setMetric(originalMetrics[faceId]);

      boost::DistancesMap distances;

      NS_LOG_DEBUG("-----------");

      dijkstra_shortest_paths(graph, source,
                              // predecessor_map (boost::ref(predecessors))
                              // .
                              distance_map(boost::ref(distances))
                                .distance_inf(boost::WeightInf)
                                .distance_zero(boost::WeightZero)
                                .distance_compare(boost::WeightCompare())
                                .distance_combine(boost::WeightCombine()));

      // NS_LOG_DEBUG (predecessors.size () << ", " << distances.size ());

      for (const auto& dist : distances) {
        if (dist.first == source)
          continue;
        else {
          // cout << "  Node " << dist.first->GetObject<Node> ()->GetId ();
          if (std::get<0>(dist.second) == 0) {
            // cout << " is unreachable" << endl;
          }
          else {
            for (const auto& prefix : dist.first->GetLocalPrefixes()) {
              NS_LOG_DEBUG(" prefix " << *prefix << " reachable via face "
                           << *std::get<0>(dist.second)
                           << " with distance " << std::get<1>(dist.second)
                           << " with delay " << std::get<2>(dist.second));

              if (std::get<0>(dist.second)->getMetric() == std::numeric_limits<uint16_t>::max() - 1)
                continue;

              FibHelper::AddRoute(*node, *prefix, std::get<0>(dist.second),
                                  std::get<1>(dist.second));
            }
          }
        }
      }

      // disabling the face again
      face->setMetric(std::numeric_limits<uint16_t>::max() - 1);
    }

    // recover original interface statuses
    for (auto& i : originalMetrics) {
      l3->getForwarder()->getFaceTable().get(i.first)->setMetric(i.second);
    }
  }
}

} // namespace ndn
} // namespace ns3
