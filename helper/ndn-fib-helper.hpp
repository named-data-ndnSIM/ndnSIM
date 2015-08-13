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

#ifndef NDN_FIB_HELPER_H
#define NDN_FIB_HELPER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"

#include "ns3/node.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"

#include <ndn-cxx/management/nfd-control-parameters.hpp>

namespace ns3 {
namespace ndn {

using ::ndn::nfd::ControlParameters;

/**
 * @ingroup ndn-helpers
 * @brief Forwarding Information Base (FIB) helper
 *
 * The FIB helper interacts with the FIB manager of NFD by sending special Interest
 * commands to the manager in order to add/remove a next hop from FIB entries or add
 * routes to the FIB manually (manual configuration of FIB).
 */
class FibHelper {
public:
  /**
   * \brief Add forwarding entry to FIB
   *
   * \param node   Node
   * \param prefix Routing prefix
   * \param face   Face
   * \param metric Routing metric
   */
  static void
  AddRoute(Ptr<Node> node, const Name& prefix, shared_ptr<Face> face, int32_t metric);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param nodeName Node name
   * \param prefix Routing prefix
   * \param faceId Face index
   * \param metric Routing metric
   */
  static void
  AddRoute(const std::string& nodeName, const Name& prefix, uint32_t faceId, int32_t metric);

  /**
   * \brief Add forwarding entry to FIB
   *
   * \param nodeName Node
   * \param prefix Routing prefix
   * \param faceId Face index
   * \param metric Routing metric
   */
  static void
  AddRoute(Ptr<Node> node, const Name& prefix, uint32_t faceId, int32_t metric);

  /**
   * @brief Add forwarding entry to FIB (work only with point-to-point links)
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param otherNode The other node, to which interests (will be used to infer face id
   * \param metric Routing metric
   */
  static void
  AddRoute(Ptr<Node> node, const Name& prefix, Ptr<Node> otherNode, int32_t metric);

  /**
   * @brief Add forwarding entry to FIB (work only with point-to-point links)
   *
   * \param nodeName Node name (refer to ns3::Names)
   * \param prefix Routing prefix
   * \param otherNode The other node name, to which interests (will be
   *                  used to infer face id (refer to ns3::Names)
   * \param metric Routing metric
   */
  static void
  AddRoute(const std::string& nodeName, const Name& prefix, const std::string& otherNodeName,
           int32_t metric);

  /**
   * \brief remove forwarding entry in FIB
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param face Face
   */
  static void
  RemoveRoute(Ptr<Node> node, const Name& prefix, shared_ptr<Face> face);

  /**
   * \brief remove forwarding entry in FIB
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param faceId Face index
   */
  static void
  RemoveRoute(Ptr<Node> node, const Name& prefix, uint32_t faceId);

  /**
   * \brief remove forwarding entry in FIB
   *
   * \param nodeName Node name
   * \param prefix Routing prefix
   * \param faceId Face index
   */
  static void
  RemoveRoute(const std::string& nodeName, const Name& prefix, uint32_t faceId);

  /**
   * @brief remove forwarding entry in FIB (work only with point-to-point links)
   *
   * \param node Node
   * \param prefix Routing prefix
   * \param otherNode The other node, to which interests (will be used to infer face id
   */
  static void
  RemoveRoute(Ptr<Node> node, const Name& prefix, Ptr<Node> otherNode);

  /**
   * @brief remove forwarding entry in FIB (work only with point-to-point links)
   *
   * \param nodeName Node name
   * \param prefix Routing prefix
   * \param otherNode The other node name, to which interests (will be used to infer face id
   */
  static void
  RemoveRoute(const std::string& nodeName, const Name& prefix, const std::string& otherNodeName);

private:
  static void
  GenerateCommand(Interest& interest);

  static void
  AddNextHop(const ControlParameters& parameters, Ptr<Node> node);

  static void
  RemoveNextHop(const ControlParameters& parameters, Ptr<Node> node);
};

} // namespace ndn

} // namespace ns3

#endif // NDN_FIB_HELPER_H
