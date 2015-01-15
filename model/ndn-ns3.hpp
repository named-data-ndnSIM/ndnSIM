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

#ifndef NDN_NS3_HPP
#define NDN_NS3_HPP

#include "ns3/packet.h"
#include "ns3/ptr.h"
#include <memory>

namespace ns3 {
namespace ndn {

class Convert {
public:
  template<class T>
  static std::shared_ptr<const T>
  FromPacket(Ptr<Packet> packet);

  template<class T>
  static Ptr<Packet>
  ToPacket(const T& pkt);

  static uint32_t
  getPacketType(Ptr<const Packet> packet);
};

} // namespace ndn
} // namespace ns3

#endif
