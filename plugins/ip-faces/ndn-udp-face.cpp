/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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

#include "ndn-udp-face.h"
#include "ns3/ndn-l3-protocol.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/udp-socket-factory.h"

#include "ns3/ndn-name.h"

using namespace std;

NS_LOG_COMPONENT_DEFINE ("ndn.UdpFace");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (UdpFace);

TypeId
UdpFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::UdpFace")
    .SetParent<Face> ()
    .SetGroupName ("Ndn")
    ;
  return tid;
}

/**
 * By default, Ndn face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
UdpFace::UdpFace (Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address)
  : Face (node)
  , m_socket (socket)
  , m_address (address)
{
  SetMetric (1); // default metric
}

UdpFace::~UdpFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

UdpFace& UdpFace::operator= (const UdpFace &)
{
  return *this;
}

bool
UdpFace::ReceiveFromUdp (Ptr<const Packet> p)
{
  return Face::Receive (p);
}

bool
UdpFace::Send (Ptr<Packet> packet)
{
  if (!Face::Send (packet))
    {
      return false;
    }
  
  NS_LOG_FUNCTION (this << packet);
  m_socket->Send (packet);

  return true;
}

Ipv4Address
UdpFace::GetAddress () const
{
  return m_address;
}

std::ostream&
UdpFace::Print (std::ostream& os) const
{
  os << "dev=udp(" << GetId () << "," << GetAddress () << ")";
  return os;
}

} // namespace ndn
} // namespace ns3
