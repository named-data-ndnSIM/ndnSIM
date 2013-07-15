/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 */

#include "ndn-ip-face-stack.h"
#include "ndn-tcp-face.h"

#include "ns3/ndn-l3-protocol.h"

#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"

#include "ns3/socket.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("ndn.IpFaceStack");

namespace ns3 {
namespace ndn {
    
NS_OBJECT_ENSURE_REGISTERED (IpFaceStack);

TypeId
IpFaceStack::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::IpFaceStack")
    .SetGroupName ("Ndn")
    .SetParent<Object> ()
    .AddConstructor<IpFaceStack> ()

    .AddAttribute ("EnableTCP", "Enable ability to create TCP faces",
                   BooleanValue (true),
                   MakeBooleanAccessor (&IpFaceStack::m_enableTcp),
                   MakeBooleanChecker ())

    .AddAttribute ("EnableUDP", "Enable ability to create UDP faces",
                   BooleanValue (true),
                   MakeBooleanAccessor (&IpFaceStack::m_enableUdp),
                   MakeBooleanChecker ())
    ;
  return tid;
}
    
IpFaceStack::IpFaceStack ()
{
}
    
IpFaceStack::~IpFaceStack ()
{
}

void
IpFaceStack::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      m_node = GetObject<Node> ();
      if (m_node != 0)
        {
          Simulator::ScheduleWithContext (m_node->GetId (), Seconds (0.1), &IpFaceStack::StartServer, this);
        }
    }
}

// Application Methods
void 
IpFaceStack::StartServer () // Called at time specified by Start
{
  NS_LOG_FUNCTION (this);
  
  if (m_enableTcp)
    {
      m_tcpServer = Socket::CreateSocket (m_node, TcpSocketFactory::GetTypeId ());
  
      m_tcpServer->Bind (InetSocketAddress (Ipv4Address::GetAny (), L3Protocol::IP_STACK_PORT));
      m_tcpServer->Listen ();

      m_tcpServer->SetAcceptCallback (MakeCallback (&IpFaceStack::OnTcpConnectionRequest, this),
                                      MakeCallback (&IpFaceStack::OnTcpConnectionAccept, this));
    }

  if (m_enableUdp)
    {
      // m_udpServer = Socket::CreateSocket (m_node, UdpSocketFactory::GetTypeId ());  
      // m_udpServer->Bind (InetSocketAddress (Ipv4Address::GetAny (), L3Protocol::IP_STACK_PORT));
      
      // m_udpServer->SetRecvCallback (MakeCallback (&IpFaceStack::HandleRead, this));
      // #error "Broken"
    }
}

bool
IpFaceStack::OnTcpConnectionRequest (Ptr< Socket > sock, const Address &addr)
{
  NS_LOG_FUNCTION (this << sock << InetSocketAddress::ConvertFrom (addr));
  return true; // accept all connections from anybody
}

void
IpFaceStack::OnTcpConnectionAccept (Ptr<Socket> socket, const Address &addr)
{
  NS_LOG_FUNCTION (this << socket << InetSocketAddress::ConvertFrom (addr));
  
  Ptr<L3Protocol> ndn = m_node->GetObject<L3Protocol> ();
  Ptr<TcpFace> face = CreateObject<TcpFace> (m_node, socket, InetSocketAddress::ConvertFrom (addr).GetIpv4 ());

  ndn->AddFace (face);
  face->SetUp (true);

  socket->SetCloseCallbacks (MakeCallback (&TcpFace::OnTcpConnectionClosed, face),
                             MakeCallback (&TcpFace::OnTcpConnectionClosed, face));
}

} // namespace ndn
} // namespace ns3
