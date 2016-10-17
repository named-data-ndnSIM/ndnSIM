/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2016  Regents of the University of California.
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

#include "ndn-network-region-table-helper.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"

namespace ns3 {
namespace ndn {

NS_LOG_COMPONENT_DEFINE("ndn.NetworkRegionTableHelper");

void
NetworkRegionTableHelper::AddRegionName(Ptr<Node> node, const Name& region)
{
  NS_LOG_LOGIC("Node [" << node->GetId() << "]$ RegionName " << region << " is added into NetworkRegionTable ");

  Ptr<L3Protocol> l3protocol = node->GetObject<L3Protocol>();
  NS_ASSERT_MSG(l3protocol != 0, "Ndn stack should be installed on the node");

  node->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().insert(region);
}

void
NetworkRegionTableHelper::AddRegionName(NodeContainer& nodes, const ndn::Name& region)
{
  for (auto nodeIt = nodes.Begin(); nodeIt != nodes.End(); ++nodeIt) {
    AddRegionName(*nodeIt, region);
  }
}

void
NetworkRegionTableHelper::AddRegionName(Ptr<Node> node, std::initializer_list<Name> regions)
{
  for (const auto& region : regions) {
    AddRegionName(node, region);
  }
}

void
NetworkRegionTableHelper::AddRegionName(const NodeContainer& nodes, std::initializer_list<Name> regions)
{
  for (auto nodeIt = nodes.begin(); nodeIt != nodes.End(); ++nodeIt) {
    AddRegionName(*nodeIt, regions);
  }
}

void
NetworkRegionTableHelper::RemoveRegionName(Ptr<Node> node, const Name& region)
{
  NS_LOG_LOGIC("Node [" << node->GetId() << "]$ RegionName " << region << " is removed from NetworkRegionTable ");

  Ptr<L3Protocol> l3protocol = node->GetObject<L3Protocol>();
  NS_ASSERT_MSG(l3protocol != 0, "Ndn stack should be installed on the node");

  node->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().erase(region);
}

void
NetworkRegionTableHelper::RemoveRegionName(const NodeContainer& nodes, const ndn::Name &region)
{
  for (auto nodeIt = nodes.begin(); nodeIt != nodes.End(); ++nodeIt) {
    RemoveRegionName(*nodeIt, region);
  }
}

void
NetworkRegionTableHelper::RemoveRegionName(Ptr<Node> node, std::initializer_list<Name> regions)
{
  for (const auto& region : regions) {
    RemoveRegionName(node, region);
  }
}

void
NetworkRegionTableHelper::RemoveRegionName(const NodeContainer& nodes, std::initializer_list<Name> regions)
{
  for (auto nodeIt = nodes.begin(); nodeIt != nodes.End(); ++nodeIt) {
    RemoveRegionName(*nodeIt, regions);
  }
}

void
NetworkRegionTableHelper::EmptyNetworkRegionTable(Ptr<Node> node)
{
  NS_LOG_LOGIC("Node [" << node->GetId() << "]$ NetworkRegionTable is cleared");

  Ptr<L3Protocol> l3protocol = node->GetObject<L3Protocol>();
  NS_ASSERT_MSG(l3protocol != 0, "Ndn stack should be installed on the node");

  node->GetObject<L3Protocol>()->getForwarder()->getNetworkRegionTable().clear();
}

void
NetworkRegionTableHelper::EmptyNetworkRegionTable(const NodeContainer& nodes)
{
  for (auto nodeIt = nodes.begin(); nodeIt != nodes.End(); ++nodeIt) {
    EmptyNetworkRegionTable(*nodeIt);
  }
}

} // namespace ndn
} // namespace ns3
