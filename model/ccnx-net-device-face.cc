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

#include "ccnx-net-device-face.h"
#include "ccnx-l3-protocol.h"

#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

NS_LOG_COMPONENT_DEFINE ("CcnxNetDeviceFace");

namespace ns3 {

TypeId
CcnxNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxNetDeviceFace")
    .SetParent<CcnxFace> ()
    .SetGroupName ("Ccnx")
    ;
  return tid;
}

/** 
 * By default, Ccnx face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
CcnxNetDeviceFace::CcnxNetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice)
  : CcnxFace (node)
  , m_netDevice (netDevice)
{
  NS_LOG_FUNCTION (this << netDevice);

  NS_ASSERT_MSG (m_netDevice != 0, "CcnxNetDeviceFace needs to be assigned a valid NetDevice");
}

CcnxNetDeviceFace::~CcnxNetDeviceFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

CcnxNetDeviceFace& CcnxNetDeviceFace::operator= (const CcnxNetDeviceFace &)
{
  return *this;
}

Ptr<NetDevice>
CcnxNetDeviceFace::GetNetDevice () const
{
  return m_netDevice;
}

void
CcnxNetDeviceFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  CcnxFace::RegisterProtocolHandler (handler);
  
  m_node->RegisterProtocolHandler (MakeCallback (&CcnxNetDeviceFace::ReceiveFromNetDevice, this),
                                   CcnxL3Protocol::ETHERNET_FRAME_TYPE, m_netDevice, true/*promiscuous mode*/);
}

void
CcnxNetDeviceFace::SendImpl (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  
  NS_ASSERT_MSG (packet->GetSize () <= m_netDevice->GetMtu (), 
                 "Packet size " << packet->GetSize () << " exceeds device MTU "
                 << m_netDevice->GetMtu ()
                 << " for Ccnx; fragmentation not supported");

  m_netDevice->Send (packet, m_netDevice->GetBroadcast (), 
                     CcnxL3Protocol::ETHERNET_FRAME_TYPE);
}

// callback
void
CcnxNetDeviceFace::ReceiveFromNetDevice (Ptr<NetDevice> device,
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
CcnxNetDeviceFace::Print (std::ostream& os) const
{
  os << "dev=net(" << GetId () << ")";
  return os;
}

}; // namespace ns3

