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

#include "ndn-scenario-helper.hpp"
#include "ndn-fib-helper.hpp"
#include "ndn-app-helper.hpp"

#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"

#include "ns3/names.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/string.h"

namespace ns3 {
namespace ndn {

ScenarioHelper::ScenarioHelper()
  : m_isTopologyInitialized(false)
{
}

void
ScenarioHelper::createTopology(std::initializer_list<std::initializer_list<std::string>/*node clique*/> topology)
{
  if (m_isTopologyInitialized) {
    throw std::logic_error("Topology cannot be created twice");
  }

  PointToPointHelper p2p;

  for (auto&& clique : topology) {
    for (auto i = clique.begin(); i != clique.end(); ++i) {
      auto node1 = getOrCreateNode(*i);
      for (auto j = i + 1; j != clique.end(); ++j) {
        auto node2 = getOrCreateNode(*j);

        auto link = p2p.Install(node1, node2);
        links[*i][*j] = link.Get(0);
        links[*j][*i] = link.Get(1);
      }
    }
  }

  ndnHelper.InstallAll();
  m_isTopologyInitialized = true;
}

void
ScenarioHelper::addRoutes(std::initializer_list<ScenarioHelper::RouteInfo> routes)
{
  for (auto&& route : routes) {
    FibHelper::AddRoute(getNode(route.node1), route.prefix,
                        getFace(route.node1, route.node2), route.metric);
  }
}

void
ScenarioHelper::addApps(std::initializer_list<ScenarioHelper::AppInfo> apps)
{
  for (auto&& app : apps) {
    AppHelper appHelper(app.name);
    for (auto&& param : app.params) {
      appHelper.SetAttribute(param.first, StringValue(param.second));
    }
    auto installedApp = appHelper.Install(getNode(app.node));
    installedApp.Start(Time(app.start));
    installedApp.Stop(Time(app.end));
  }
}

Ptr<Node>
ScenarioHelper::getOrCreateNode(const std::string& nodeName)
{
  auto node = nodes.find(nodeName);
  if (node == nodes.end()) {
    std::tie(node, std::ignore) = nodes.insert(std::make_pair(nodeName, CreateObject<Node>()));
    Names::Add(nodeName, node->second);
  }
  return node->second;
}

Ptr<Node>
ScenarioHelper::getNode(const std::string& nodeName)
{
  auto node = nodes.find(nodeName);
  if (node != nodes.end()) {
    return node->second;
  }

  throw std::invalid_argument("Node " + nodeName + " does not exist");
}

shared_ptr<Face>
ScenarioHelper::getFace(const std::string& node1, const std::string& node2)
{
  auto i = links.find(node1);
  if (i != links.end()) {
    auto j = i->second.find(node2);
    if (j != i->second.end()) {
      return j->second->GetNode()->GetObject<L3Protocol>()->getFaceByNetDevice(j->second);
    }
  }

  throw std::invalid_argument("Link between " + node1 + " and " + node2 + " does not exist");
}

} // namespace ndn
} // namespace ns3
