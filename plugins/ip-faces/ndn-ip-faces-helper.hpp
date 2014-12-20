/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *                    Alexander Afanasyev
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_IP_FACES_HELPER_H
#define NDN_IP_FACES_HELPER_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

class Node;
class NodeContainer;
class Channel;

namespace ndn {

/**
 * @ingroup ndn-helpers
 * @brief Helper for NDN IP-based face creation
 */
class IpFacesHelper
{
public:
  /**
   * @brief Install IpFaceStack interface on a node
   * @param node Node to install IpFaceStack interface
   */
  static void
  Install (Ptr<Node> node);

  /**
   * @brief Install IpFaceStack interface on nodes
   * @param nodes NodeContainer to install IpFaceStack interface
   */
  static void
  Install (const NodeContainer &nodes);

  /**
   * @brief Install IpFaceStack interface on all nodes
   */
  static void
  InstallAll ();

  /**
   * @brief Create TCP face
   * @param when    Time when to create face (use `Seconds (0)' if face should be created right away)
   * @param node    Node to add TCP face (will initiate connection)
   * @param address IP address to connect (using standard 9695 port)
   * @param prefix  Prefix to associate with the face
   * @param metric  Metric that will be assigned to the face
   *
   * This call schedules connection initiation and after successful connection it will add new face
   * to NDN stack and add the requested route
   *
   * If face has been already created before (same IP address), then this call will simply
   * update FIB with requested prefix
   */
  static void
  CreateTcpFace (const Time &when, Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric = 1);

  /**
   * @brief Create TCP face
   */
  static void
  CreateUdpFace (const Time &when, Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric = 1);
};

} // namespace ndn
} // namespace ns3

#endif // NDN_IP_FACES_HELPER_H
