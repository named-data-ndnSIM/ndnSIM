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

#include "ndn-tcp-face.h"
#include "ndn-ip-face-stack.h"

#include "ns3/ndn-l3-protocol.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/tcp-socket-factory.h"

#include "ns3/ndn-name.h"

using namespace std;

NS_LOG_COMPONENT_DEFINE ("ndn.TcpFace");

namespace ns3 {
namespace ndn {

class TcpBoundaryHeader : public Header
{
public:
  static TypeId GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::ndn::TcpFace::BoundaryHeader")
      .SetGroupName ("Ndn")
      .SetParent<Header> ()
      ;
    return tid;
  }

  TcpBoundaryHeader ()
    : m_length (0)
  {
  }
  
  TcpBoundaryHeader (Ptr<Packet> packet)
    : m_length (packet->GetSize ())
  {
    
  }

  TcpBoundaryHeader (uint32_t length)
    : m_length (length)
  {
  }

  uint32_t
  GetLength () const
  {
    return m_length;
  }
  
  virtual TypeId
  GetInstanceTypeId (void) const
  {
    return TcpBoundaryHeader::GetTypeId ();
  }
  
  virtual void
  Print (std::ostream &os) const
  {
    os << "[" << m_length << "]";
  }
  
  virtual uint32_t
  GetSerializedSize (void) const
  {
    return 4;
  }
  
  virtual void
  Serialize (Buffer::Iterator start) const
  {
    start.WriteU32 (m_length);
  }
  
  virtual uint32_t
  Deserialize (Buffer::Iterator start)
  {
    m_length = start.ReadU32 ();
    return 4;
  }

private:
  uint32_t m_length;
};

NS_OBJECT_ENSURE_REGISTERED (TcpFace);

TypeId
TcpFace::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::TcpFace")
    .SetParent<Face> ()
    .SetGroupName ("Ndn")
    ;
  return tid;
}

/**
 * By default, Ndn face are created in the "down" state.  Before
 * becoming useable, the user must invoke SetUp on the face
 */
TcpFace::TcpFace (Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address)
  : Face (node)
  , m_socket (socket)
  , m_address (address)
  , m_pendingPacketLength (0)
{
  SetMetric (1); // default metric
}

TcpFace::~TcpFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TcpFace& TcpFace::operator= (const TcpFace &)
{
  return *this;
}

void
TcpFace::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
  NS_LOG_FUNCTION (this);

  Face::RegisterProtocolHandlers (interestHandler, dataHandler);
  m_socket->SetRecvCallback (MakeCallback (&TcpFace::ReceiveFromTcp, this));
}

void
TcpFace::UnRegisterProtocolHandlers ()
{
  m_socket->SetRecvCallback (MakeNullCallback< void, Ptr<Socket> > ());
  Face::UnRegisterProtocolHandlers ();
}

bool
TcpFace::Send (Ptr<Packet> packet)
{
  if (!Face::Send (packet))
    {
      return false;
    }
  
  NS_LOG_FUNCTION (this << packet);

  Ptr<Packet> boundary = Create<Packet> ();
  TcpBoundaryHeader hdr (packet);
  boundary->AddHeader (hdr);
  
  m_socket->Send (boundary);
  m_socket->Send (packet);

  return true;
}

void
TcpFace::ReceiveFromTcp (Ptr< Socket > clientSocket)
{
  NS_LOG_FUNCTION (this << clientSocket);
  TcpBoundaryHeader hdr;

  if (m_pendingPacketLength > 0)
    {
      if (clientSocket->GetRxAvailable () >= m_pendingPacketLength)
        {
          Ptr<Packet> realPacket = clientSocket->Recv (m_pendingPacketLength, 0);
          NS_LOG_DEBUG ("+++ Expected " << m_pendingPacketLength << " bytes, got " << realPacket->GetSize () << " bytes");
          if (realPacket == 0)
            return;
          
          Receive (realPacket);
        }
      else
        return; // still not ready
    }
  
  m_pendingPacketLength = 0;
  
  while (clientSocket->GetRxAvailable () >= hdr.GetSerializedSize ())
    {
      Ptr<Packet> boundary = clientSocket->Recv (hdr.GetSerializedSize (), 0);
      if (boundary == 0)
        return; // no idea why it would happen...

      NS_LOG_DEBUG ("Expected 4 bytes, got " << boundary->GetSize () << " bytes");
      
      boundary->RemoveHeader (hdr);
      NS_LOG_DEBUG ("Header specifies length: " << hdr.GetLength ());
      m_pendingPacketLength = hdr.GetLength ();
      
      if (clientSocket->GetRxAvailable () >= hdr.GetLength ())
        {
          Ptr<Packet> realPacket = clientSocket->Recv (hdr.GetLength (), 0);
          if (realPacket == 0)
            {
              NS_LOG_DEBUG ("Got nothing, but requested at least " << hdr.GetLength ());
              return;
            }
          
          NS_LOG_DEBUG ("Receiving data " << hdr.GetLength () << " bytes, got " << realPacket->GetSize () << " bytes");

          Receive (realPacket);
          m_pendingPacketLength = 0;
        }
      else
        {
          return;
        }
    }
}

void
TcpFace::OnTcpConnectionClosed (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  GetNode ()->GetObject<IpFaceStack> ()->DestroyTcpFace (this);
}

Ipv4Address
TcpFace::GetAddress () const
{
  return m_address;
}

void
TcpFace::SetCreateCallback (Callback< void, Ptr<Face> > callback)
{
  m_onCreateCallback = callback;
}

void
TcpFace::OnConnect (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);

  Ptr<L3Protocol> ndn = GetNode ()->GetObject<L3Protocol> ();
  
  ndn->AddFace (this);
  this->SetUp (true);

  socket->SetCloseCallbacks (MakeCallback (&TcpFace::OnTcpConnectionClosed, this),
                             MakeCallback (&TcpFace::OnTcpConnectionClosed, this));

  if (!m_onCreateCallback.IsNull ())
    {
      m_onCreateCallback (this);
      m_onCreateCallback = IpFaceStack::NULL_CREATE_CALLBACK;
    }
}
    
std::ostream&
TcpFace::Print (std::ostream& os) const
{
  os << "dev=tcp(" << GetId () << ", " << m_address << ")";
  return os;
}

} // namespace ndn
} // namespace ns3

