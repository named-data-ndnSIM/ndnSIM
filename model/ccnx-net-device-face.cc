/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#include "ns3/ccnx-l3-protocol.h"
#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

NS_LOG_COMPONENT_DEFINE ("CcnxNetDeviceFace");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxNetDeviceFace);

TypeId 
CcnxNetDeviceFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxNetDeviceFace")
    .SetParent<CcnxFace> ()
  ;
  return tid;
}

/** 
 * By default, Ccnx face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
CcnxNetDeviceFace::CcnxNetDeviceFace (const Ptr<NetDevice> &netDevice) 
{
  NS_LOG_FUNCTION (this);

  m_netDevice = netDevice;
}

CcnxNetDeviceFace::~CcnxNetDeviceFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

CcnxNetDeviceFace::CcnxNetDeviceFace (const CcnxNetDeviceFace &)
{
}

CcnxNetDeviceFace& CcnxNetDeviceFace::operator= (const CcnxNetDeviceFace &)
{
  return *this;
}

  
void
CcnxNetDeviceFace::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_netDevice = 0;
  Object::DoDispose ();
}

Ptr<NetDevice>
CcnxNetDeviceFace::GetNetDevice () const
{
  return m_netDevice;
}

void
CcnxNetDeviceFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_ASSERT_MSG (m_netDevice != 0, "CcnxNetDeviceFace needs to be assigned NetDevice first");
  
  m_protocolHandler = handler;

  m_node->RegisterProtocolHandler (MakeCallback (&CcnxNetDeviceFace::ReceiveFromNetDevice, this),
                                   CcnxL3Protocol::ETHERNET_FRAME_TYPE, m_netDevice, true/*promiscuous mode*/);
}

void
CcnxNetDeviceFace::Send (Ptr<Packet> packet)
{
  NS_ASSERT_MSG (packet->GetSize () <= m_netDevice->GetMtu (), 
                 "Packet size " << packet->GetSize () << " exceeds device MTU "
                                << m_netDevice->GetMtu ()
                                << " for Ccnx; fragmentation not supported");

  NS_LOG_FUNCTION (*packet);
  if (!IsUp ())
    {
      return;
    }

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
  m_protocolHandler (Ptr<CcnxFace>(this), p);
}


std::ostream& operator<< (std::ostream& os, const CcnxNetDeviceFace &face)
{
  return operator<< (os, static_cast<const CcnxFace&> (face)); // just call parent class for now
  // os << "id=" << face.GetId ();
  // return os;
}

}; // namespace ns3

