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

#include "ndn-net-device-face.hpp"
#include "ndn-l3-protocol.hpp"

#include "ndn-ns3.hpp"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

// #include "ns3/address.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/channel.h"

#include "../utils/ndn-fw-hop-count-tag.hpp"

NS_LOG_COMPONENT_DEFINE("ndn.NetDeviceFace");

namespace ns3 {
namespace ndn {

NetDeviceFace::NetDeviceFace(Ptr<Node> node, const Ptr<NetDevice>& netDevice)
  : Face(FaceUri("netDeviceFace://"), FaceUri("netDeviceFace://"))
  , m_node(node)
  , m_netDevice(netDevice)
{
  NS_LOG_FUNCTION(this << netDevice);

  setMetric(1); // default metric

  NS_ASSERT_MSG(m_netDevice != 0, "NetDeviceFace needs to be assigned a valid NetDevice");

  m_node->RegisterProtocolHandler(MakeCallback(&NetDeviceFace::receiveFromNetDevice, this),
                                  L3Protocol::ETHERNET_FRAME_TYPE, m_netDevice,
                                  true /*promiscuous mode*/);
}

NetDeviceFace::~NetDeviceFace()
{
  NS_LOG_FUNCTION_NOARGS();
  close();
}

void
NetDeviceFace::close()
{
  m_node->UnregisterProtocolHandler(MakeCallback(&NetDeviceFace::receiveFromNetDevice, this));
  this->fail("Close connection");
}

Ptr<NetDevice>
NetDeviceFace::GetNetDevice() const
{
  return m_netDevice;
}

void
NetDeviceFace::send(Ptr<Packet> packet)
{
  NS_ASSERT_MSG(packet->GetSize() <= m_netDevice->GetMtu(),
                "Packet size " << packet->GetSize() << " exceeds device MTU "
                               << m_netDevice->GetMtu());

  FwHopCountTag tag;
  packet->RemovePacketTag(tag);
  tag.Increment();
  packet->AddPacketTag(tag);

  m_netDevice->Send(packet, m_netDevice->GetBroadcast(), L3Protocol::ETHERNET_FRAME_TYPE);
}

void
NetDeviceFace::sendInterest(const Interest& interest)
{
  NS_LOG_FUNCTION(this << &interest);

  this->emitSignal(onSendInterest, interest);

  Ptr<Packet> packet = Convert::ToPacket(interest);
  send(packet);
}

void
NetDeviceFace::sendData(const Data& data)
{
  NS_LOG_FUNCTION(this << &data);

  this->emitSignal(onSendData, data);

  Ptr<Packet> packet = Convert::ToPacket(data);
  send(packet);
}

// callback
void
NetDeviceFace::receiveFromNetDevice(Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol,
                                    const Address& from, const Address& to,
                                    NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION(device << p << protocol << from << to << packetType);

  Ptr<Packet> packet = p->Copy();
  try {
    uint32_t type = Convert::getPacketType(p);
    if (type == ::ndn::tlv::Interest) {
      shared_ptr<const Interest> i = Convert::FromPacket<Interest>(packet);
      this->emitSignal(onReceiveInterest, *i);
    }
    else if (type == ::ndn::tlv::Data) {
      shared_ptr<const Data> d = Convert::FromPacket<Data>(packet);
      this->emitSignal(onReceiveData, *d);
    }
    else {
      NS_LOG_ERROR("Unsupported TLV packet");
    }
  }
  catch (::ndn::tlv::Error&) {
    NS_LOG_ERROR("Unrecognized TLV packet");
  }
}

} // namespace ndn
} // namespace ns3
