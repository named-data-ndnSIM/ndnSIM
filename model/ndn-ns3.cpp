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

#include "ndn-ns3.hpp"

#include <ndn-cxx/encoding/block.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/data.hpp>

#include "ndn-header.hpp"
#include "../utils/ndn-ns3-packet-tag.hpp"

namespace ns3 {
namespace ndn {

template<class T>
std::shared_ptr<const T>
Convert::FromPacket(Ptr<Packet> packet)
{
  PacketHeader<T> header;
  packet->RemoveHeader(header);

  auto pkt = header.getPacket();
  pkt->setTag(make_shared<Ns3PacketTag>(packet));

  return pkt;
}

template std::shared_ptr<const Interest>
Convert::FromPacket<Interest>(Ptr<Packet> packet);

template std::shared_ptr<const Data>
Convert::FromPacket<Data>(Ptr<Packet> packet);

template<class T>
Ptr<Packet>
Convert::ToPacket(const T& pkt)
{
  PacketHeader<T> header(pkt);

  Ptr<Packet> packet;

  auto tag = pkt.template getTag<Ns3PacketTag>();
  if (tag != nullptr) {
    packet = tag->getPacket()->Copy();
  }
  else {
    packet = Create<Packet>();
  }

  packet->AddHeader(header);
  return packet;
}

template Ptr<Packet>
Convert::ToPacket<Interest>(const Interest& packet);

template Ptr<Packet>
Convert::ToPacket<Data>(const Data& packet);

uint32_t
Convert::getPacketType(Ptr<const Packet> packet)
{
  uint8_t type;
  uint32_t nRead = packet->CopyData(&type, 1);
  if (nRead != 1) {
    throw ::ndn::tlv::Error("Unknown header");
  }

  if (type == ::ndn::tlv::Interest || type == ::ndn::tlv::Data) {
    return type;
  }
  else {
    throw ::ndn::tlv::Error("Unknown header");
  }
}

} // namespace ndn
} // namespace ns3
