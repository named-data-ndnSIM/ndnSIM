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

#ifndef NDN_NS3_PACKET_TAG_HPP
#define NDN_NS3_PACKET_TAG_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <ndn-cxx/tag.hpp>

namespace ns3 {
namespace ndn {

class Ns3PacketTag : public ::ndn::Tag {
public:
  static size_t
  getTypeId()
  {
    return 0xaee87802; // md5("Ns3PacketTag")[0:8]
  }

  Ns3PacketTag(Ptr<const Packet> packet)
    : m_packet(packet)
  {
  }

  Ptr<const Packet>
  getPacket() const
  {
    return m_packet;
  }

private:
  Ptr<const Packet> m_packet;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_NS3_PACKET_TAG_HPP
