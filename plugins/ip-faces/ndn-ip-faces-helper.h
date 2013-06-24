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
#include "ns3/ipv4-address.h"

namespace ns3 {

class Node;
class NodeContainer;
class Channel;

namespace ndn {

/**
 * @ingroup ndn
 * @brief Helper for NDN IP-based face creation
 */
class IpFacesHelper
{
public:
  /**
   * @brief Install IpFaceStack interface on a node
   * @param node Node to install IpFaceStack interface
   */
  void
  Install (Ptr<Node> node);

  /**
   * @brief Install IpFaceStack interface on nodes
   * @param nodes NodeContainer to install IpFaceStack interface
   */
  void
  Install (const NodeContainer &nodes);

  /**
   * @brief Install IpFaceStack interface on all nodes
   */
  void
  InstallAll ();

  /**
   * @brief Create TCP face
   */
  void
  CreateTcpFace (Ptr<Node> node, Ipv4Address address, const std::string &prefix);
};

} // namespace ndn
} // namespace ns3

#endif // NDN_IP_FACES_HELPER_H
