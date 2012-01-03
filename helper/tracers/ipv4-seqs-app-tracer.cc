/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ipv4-seqs-app-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"

#include "ns3/tcp-l4-protocol.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

namespace ns3 {
    
Ipv4SeqsAppTracer::Ipv4SeqsAppTracer (std::ostream &os, Ptr<Node> node, const std::string &appId)
  : Ipv4AppTracer (node, appId)
  , m_os (os)
{
}

void
Ipv4SeqsAppTracer::Reset ()
{
}

void
Ipv4SeqsAppTracer::PrintHeader (std::ostream &os) const
{
  os << "Time\t"
     << "Node\t"
     << "AppName\t"
     << "AppId\t"
     << "Type\t"
     << "SeqNo";
}

void
Ipv4SeqsAppTracer::Print (std::ostream &os) const
{
}

#define PRINTER(type,size)                                         \
 m_os                                                              \
 << Simulator::Now ().ToDouble (Time::S) << "\t"                   \
 << m_node << "\t"                                                 \
 << m_app << "\t"                                                  \
 << m_appId << "\t"                                                \
 << type << "\t"                                                   \
 << size / 1040.0 << std::endl;

void
Ipv4SeqsAppTracer::Tx (std::string context,
    const Ipv4Header &ip, Ptr<const Packet>, uint32_t)
{
  if (ip.GetProtocol () != TcpL4Protocol::PROT_NUMBER) return;
}

void
Ipv4SeqsAppTracer::Rx (std::string context,
                       const Ipv4Header &ip, Ptr<const Packet> pktOrig, uint32_t)
{
  if (ip.GetProtocol () != TcpL4Protocol::PROT_NUMBER) return;

  TcpHeader tcp;
  Ptr<Packet> packet = pktOrig->Copy ();
  packet->RemoveHeader (tcp);

  if (tcp.GetFlags () | TcpHeader::ACK)
    {
      PRINTER("InAck", tcp.GetAckNumber ().GetValue ());
    }
}
  

// void
// Ipv4SeqsAppTracer::InData (std::string context,
//                            Ptr<const Packet> packet, const Address &address)
// {
//   PRINTER ("InData", m_inSeq);
//   m_inSeq += packet->GetSize ();
// }
  
// void
// Ipv4SeqsAppTracer::OutData (std::string context,
//                             Ptr<const Packet> packet)
// {
//   PRINTER ("OutData", m_outSeq);
//   m_outSeq += packet->GetSize ();
// }

} // namespace ns3
