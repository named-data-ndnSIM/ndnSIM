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
#include "ns3/tcp-socket-factory.h"

#include "ns3/ndn-name.h"

using namespace std;

NS_LOG_COMPONENT_DEFINE ("ndn.UdpFace");

namespace ns3 {
namespace ndn {

UdpFace::FaceMap UdpFace::s_map;

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

void
UdpFace::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandler (handler);

  m_socket->SetRecvCallback (MakeCallback (&UdpFace::ReceiveFromUdp, this));
}

bool
UdpFace::SendImpl (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  Ptr<Packet> boundary = Create<Packet> ();
  TcpBoundaryHeader hdr (packet);
  boundary->AddHeader (hdr);
  
  m_socket->Send (boundary);
  m_socket->Send (packet);

  return true;
}

void
UdpFace::ReceiveFromUdp (Ptr< Socket > clientSocket)
{
  NS_LOG_FUNCTION (this << clientSocket);

  Ptr<Packet> packet;
  Address from;
  while ((packet = socket->RecvFrom (from)))
    {
      Receive (realPacket);
    }
  }
}

Ipv4Address
UdpFace::GetAddress () const
{
  return m_address;
}

Ptr<UdpFace>
UdpFace::GetFaceByAddress (const Ipv4Address &address)
{
  FaceMap::iterator i = s_map.find (address);
  if (i != s_map.end ())
    return i->second;
  else
    return 0;
}

Ptr<UdpFace>
UdpFace::CreateOrGetFace (Ptr<Node> node, Ipv4Address address)
{
  NS_LOG_FUNCTION (address);

  FaceMap::iterator i = s_map.find (address);
  if (i != s_map.end ())
    return i->second;
  
  Ptr<Socket> socket = Socket::CreateSocket (node, UdpSocketFactory::GetTypeId ());
  Ptr<UdpFace> face = CreateObject<UdpFace> (node, socket, address);

  Ptr<L3Protocol> ndn = GetNode ()->GetObject<L3Protocol> ();
  
  ndn->AddFace (this);
  this->SetUp (true);

  s_map.insert (std::make_pair (address, face));

  return face;
}
    
std::ostream&
UdpFace::Print (std::ostream& os) const
{
  os << "dev=udp(" << GetId () << ")";
  return os;
}

} // namespace ndn
} // namespace ns3

