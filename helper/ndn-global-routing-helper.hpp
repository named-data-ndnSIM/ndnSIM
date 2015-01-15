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

#ifndef NDN_GLOBAL_ROUTING_HELPER_H
#define NDN_GLOBAL_ROUTING_HELPER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ptr.h"

namespace ns3 {

class Node;
class NodeContainer;
class Channel;

namespace ndn {

/**
 * @ingroup ndn-helpers
 * @brief Helper for GlobalRouter interface
 */
class GlobalRoutingHelper {
public:
  /**
   * @brief Install GlobalRouter interface on a node
   *
   * Note that GlobalRouter will also be installed on all connected nodes and channels
   *
   * @param node Node to install GlobalRouter interface
   */
  void
  Install(Ptr<Node> node);

  /**
   * @brief Install GlobalRouter interface on nodes
   *
   * Note that GlobalRouter will also be installed on all connected nodes and channels
   *
   * @param nodes NodeContainer to install GlobalRouter interface
   */
  void
  Install(const NodeContainer& nodes);

  /**
   * @brief Install GlobalRouter interface on all nodes
   */
  void
  InstallAll();

  /**
   * @brief Add `prefix' as origin on `node'
   * @param prefix Prefix that is originated by node, e.g., node is a producer for this prefix
   * @param node   Pointer to a node
   */
  void
  AddOrigin(const std::string& prefix, Ptr<Node> node);

  /**
   * @brief Add `prefix' as origin on all `nodes'
   * @param prefix Prefix that is originated by nodes
   * @param nodes NodeContainer
   */
  void
  AddOrigins(const std::string& prefix, const NodeContainer& nodes);

  /**
   * @brief Add `prefix' as origin on node `nodeName'
   * @param prefix     Prefix that is originated by node, e.g., node is a producer for this prefix
   * @param nodeName   Name of the node that is associated with Ptr<Node> using ns3::Names
   */
  void
  AddOrigin(const std::string& prefix, const std::string& nodeName);

  /**
   * @brief Add origin to each node based on the node's name (using Names class)
   */
  void
  AddOriginsForAll();

  /**
   * @brief Calculate for every node shortest path trees and install routes to all prefix origins
   */
  static void
  CalculateRoutes();

  /**
   * @brief Calculate all possible next-hop independent alternative routes
   *
   * Refer to the implementation for more details.
   *
   * Note that this method is highly experimental and should be used with caution (very time
   *consuming).
   */
  static void
  CalculateAllPossibleRoutes();

private:
  void
  Install(Ptr<Channel> channel);
};

} // namespace ndn
} // namespace ns3

#endif // NDN_GLOBAL_ROUTING_HELPER_H
