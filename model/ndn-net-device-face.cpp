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

#include "ndn-net-device-face.h"
#include "ndn-l3-protocol.h"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

// #include "ns3/address.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/channel.h"
#include "ns3/ndn-name.h"

NS_LOG_COMPONENT_DEFINE ("ndn.NetDeviceFace");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (NetDeviceFace);

TypeId
NetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::NetDeviceFace")
    .SetParent<Face> ()
    .SetGroupName ("Ndn")
    ;
  return tid;
}

/**
 * By default, Ndn face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
NetDeviceFace::NetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : Face (node)
  , m_netDevice (netDevice)
{
  NS_LOG_FUNCTION (this << netDevice);

  SetMetric (1); // default metric

  NS_ASSERT_MSG (m_netDevice != 0, "NetDeviceFace needs to be assigned a valid NetDevice");
}

NetDeviceFace::~NetDeviceFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

NetDeviceFace& NetDeviceFace::operator= (const NetDeviceFace &)
{
  return *this;
}

Ptr<NetDevice>
NetDeviceFace::GetNetDevice () const
{
  return m_netDevice;
}

void
NetDeviceFace::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
  NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandlers (interestHandler, dataHandler);

  m_node->RegisterProtocolHandler (MakeCallback (&NetDeviceFace::ReceiveFromNetDevice, this),
                                   L3Protocol::ETHERNET_FRAME_TYPE, m_netDevice, true/*promiscuous mode*/);
}

void
NetDeviceFace:: UnRegisterProtocolHandlers ()
{
  m_node->UnregisterProtocolHandler (MakeCallback (&NetDeviceFace::ReceiveFromNetDevice, this));
  Face::UnRegisterProtocolHandlers ();
}

bool
NetDeviceFace::Send (Ptr<Packet> packet)
{
  if (!Face::Send (packet))
    {
      return false;
    }
  
  NS_LOG_FUNCTION (this << packet);

  NS_ASSERT_MSG (packet->GetSize () <= m_netDevice->GetMtu (),
                 "Packet size " << packet->GetSize () << " exceeds device MTU "
                 << m_netDevice->GetMtu ()
                 << " for Ndn; fragmentation not supported");

  bool ok = m_netDevice->Send (packet, m_netDevice->GetBroadcast (),
                               L3Protocol::ETHERNET_FRAME_TYPE);
  return ok;
}

// callback
void
NetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
                                     Ptr<const Packet> p,
                                     uint16_t protocol,
                                     const Address &from,
                                     const Address &to,
                                     NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (device << p << protocol << from << to << packetType);
  Receive (p);
}


std::ostream&
NetDeviceFace::Print (std::ostream& os) const
{
#ifdef NS3_LOG_ENABLE
  os << "dev[" << GetNode ()->GetId () << "]=net(" << GetId ();

  if (DynamicCast<PointToPointNetDevice> (m_netDevice))
    {
      // extra debugging information which available ONLY for PointToPointNetDevice's
      os << ",";
      os << DynamicCast<PointToPointNetDevice> (m_netDevice)->GetChannel ()->GetDevice (0)->GetNode ()->GetId ();
      os << "-";
      os << DynamicCast<PointToPointNetDevice> (m_netDevice)->GetChannel ()->GetDevice (1)->GetNode ()->GetId ();
    }
  os << ")";
#else
  os << "dev=net(" << GetId () << ")";
#endif
  return os;
}

} // namespace ndnsim
} // namespace ns3

