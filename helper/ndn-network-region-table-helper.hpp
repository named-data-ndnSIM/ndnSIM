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

#ifndef NDNSIM_HELPER_NDN_NETWORK_REGION_TABLE_HELPER_HPP
#define NDNSIM_HELPER_NDN_NETWORK_REGION_TABLE_HELPER_HPP

#include "ns3/node-container.h"

#include "ns3/ndnSIM/model/ndn-common.hpp"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-helpers
 * @brief Network Region Table Helper
 *
 * The NRT Helper interacts directly with Network Region Table by using set-like API
 * in order to add/remove a prefix to/from the table or empty it in one node or
 * a node container
 */
class NetworkRegionTableHelper
{
public:
  static void
  AddRegionName(Ptr<Node> node, const Name& regionName);

  static void
  AddRegionName(NodeContainer& c, const Name& regionName);

  static void
  AddRegionName(Ptr<Node> node, std::initializer_list<Name> regionNames);

  static void
  AddRegionName(const NodeContainer& c, std::initializer_list<Name> regionNames);

  static void
  RemoveRegionName(Ptr<Node> node, const Name& regionName);

  static void
  RemoveRegionName(const NodeContainer& c, const Name& regionName);

  static void
  RemoveRegionName(Ptr<Node> node, std::initializer_list<Name> regionNames);

  static void
  RemoveRegionName(const NodeContainer& c, std::initializer_list<Name> regionNames);

  static void
  EmptyNetworkRegionTable(Ptr<Node> node);

  static void
  EmptyNetworkRegionTable(const NodeContainer& c);
};

} // namespace ndn
} // namespace ns3

#endif // NDNSIM_HELPER_NDN_NETWORK_REGION_TABLE_HELPER_HPP
