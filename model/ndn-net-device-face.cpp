/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 */

#include "ndn-net-device-face.hpp"
#include "ndn-l3-protocol.hpp"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

// #include "ns3/address.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/channel.h"

NS_LOG_COMPONENT_DEFINE("ndn.NetDeviceFace");

namespace ns3 {
namespace ndn {

/**
 * By default, Ndn face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
NetDeviceFace::NetDeviceFace(Ptr<Node> node, const Ptr<NetDevice>& netDevice)
  // : Face(node)
  : m_netDevice(netDevice)
{
  NS_LOG_FUNCTION(this << netDevice);

  // SetMetric(1); // default metric

  NS_ASSERT_MSG(m_netDevice != 0, "NetDeviceFace needs to be assigned a valid NetDevice");
}

NetDeviceFace::~NetDeviceFace()
{
  NS_LOG_FUNCTION_NOARGS();
}

Ptr<NetDevice>
NetDeviceFace::GetNetDevice() const
{
  return m_netDevice;
}

bool
NetDeviceFace::Send(Ptr<Packet> packet)
{
  return false;
}

// callback
void
NetDeviceFace::ReceiveFromNetDevice(Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol,
                                    const Address& from, const Address& to,
                                    NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION(device << p << protocol << from << to << packetType);
  // Receive(p);
}

} // namespace ndnsim
} // namespace ns3
