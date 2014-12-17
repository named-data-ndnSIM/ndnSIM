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

#ifndef NDN_LINK_CONTROL_HELPER_H
#define NDN_LINK_CONTROL_HELPER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ptr.h"
#include "ns3/node.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-helpers
 * @brief Helper class to control the up or down statuss of an NDN link connecting two specific
 *        nodes
 */
class LinkControlHelper {
public:
  /**
   * @brief Fail NDN link between two nodes
   *
   * The helper will attempt to find NDN link between node1 and
   * node2 and set NDN face to DOWN state
   *
   * Note that only PointToPointChannels are supported by this helper method
   *
   * @param node1 one node
   * @param node2 another node
   */
  static void
  FailLink(Ptr<Node> node1, Ptr<Node> node2);

  /**
   * @brief Fail NDN link between two nodes
   *
   * The helper will attempt to find NDN link between node1 and
   * node2 and set NDN face to DOWN state
   *
   * Note that only PointToPointChannels are supported by this helper method
   *
   * This variant uses node names registered by Names class
   *
   * @param node1 one node's name
   * @param node2 another node's name
   */
  static void
  FailLinkByName(const std::string& node1, const std::string& node2);

  /**
   * @brief Re-enable NDN link between two nodes
   *
   * The helper will attempt to find NDN link between node1 and
   * node2 and set NDN face to UP state
   *
   * Note that only PointToPointChannels are supported by this helper method
   *
   * @param node1 one node
   * @param node2 another node
   */
  static void
  UpLink(Ptr<Node> node1, Ptr<Node> node2);

  /**
   * @brief Re-enable NDN link between two nodes
   *
   * The helper will attempt to find NDN link between node1 and
   * node2 and set NDN face to UP state
   *
   * Note that only PointToPointChannels are supported by this helper method
   *
   * This variant uses node names registered by Names class
   *
   * @param node1 one node's name
   * @param node2 another node's name
   */
  static void
  UpLinkByName(const std::string& node1, const std::string& node2);

private:
  static void
  setErrorRate(Ptr<Node> node1, Ptr<Node> node2, double errorRate);
}; // LinkControlHelper

} // ndn
} // ns3

#endif // NDN_LINK_CONTROL_HELPER_H
