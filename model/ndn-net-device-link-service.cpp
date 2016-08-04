/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2015,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ndn-net-device-link-service.hpp"

namespace ns3 {
namespace ndn {

void
NetDeviceLinkService::doSendInterest(const Interest& interest)
{
  lp::Packet lpPacket(interest.wireEncode());

  if (m_options.allowLocalFields) {
    encodeLocalFields(interest, lpPacket);
  }

  this->sendNetPacket(std::move(lpPacket));
}

void
NetDeviceLinkService::doSendData(const Data& data)
{
  lp::Packet lpPacket(data.wireEncode());

  if (m_options.allowLocalFields) {
    encodeLocalFields(data, lpPacket);
  }

  this->sendNetPacket(std::move(lpPacket));
}

void
NetDeviceLinkService::doSendNack(const lp::Nack& nack)
{
  lp::Packet lpPacket(nack.getInterest().wireEncode());
  lpPacket.add<lp::NackField>(nack.getHeader());

  if (m_options.allowLocalFields) {
    encodeLocalFields(nack, lpPacket);
  }

  this->sendNetPacket(std::move(lpPacket));
}

void
NetDeviceLinkService::doReceivePacket(Transport::Packet&& packet)
{
  try {
    lp::Packet pkt(packet.packet);

    if (!pkt.has<lp::FragmentField>()) {
      NFD_LOG_FACE_TRACE("received IDLE packet: DROP");
      return;
    }

    if ((pkt.has<lp::FragIndexField>() || pkt.has<lp::FragCountField>()) &&
        !m_options.allowReassembly) {
      NFD_LOG_FACE_WARN("received fragment, but reassembly disabled: DROP");
      return;
    }

    bool isReassembled = false;
    Block netPkt;
    lp::Packet firstPkt;
    std::tie(isReassembled, netPkt, firstPkt) = m_reassembler.receiveFragment(packet.remoteEndpoint,
                                                                              pkt);
    if (isReassembled) {
      this->decodeNetPacket(netPkt, firstPkt);
    }
  }
  catch (const tlv::Error& e) {
    ++this->nInLpInvalid;
    NFD_LOG_FACE_WARN("packet parse error (" << e.what() << "): DROP");
  }
}

} // namespace face
} // namespace nfd
