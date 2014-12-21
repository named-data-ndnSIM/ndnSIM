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

#ifndef NDNSIM_NDN_HEADER_HPP
#define NDNSIM_NDN_HEADER_HPP

#include "ns3/header.h"

#include "ndn-common.hpp"

namespace ns3 {
namespace ndn {

template<class Pkt>
class PacketHeader : public Header {
public:
  static ns3::TypeId
  GetTypeId();

  virtual TypeId
  GetInstanceTypeId(void) const;

  PacketHeader();

  PacketHeader(const Pkt& packet);

  virtual uint32_t
  GetSerializedSize(void) const;

  virtual void
  Serialize(ns3::Buffer::Iterator start) const;

  virtual uint32_t
  Deserialize(ns3::Buffer::Iterator start);

  virtual void
  Print(std::ostream& os) const;

  shared_ptr<const Pkt>
  getPacket();

private:
  shared_ptr<const Pkt> m_packet;
};

} // namespace ndn
} // namespace ns3

#endif // NDNSIM_NDN_HEADER_HPP
