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
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/log.h"

#include "ns3/tcp-l4-protocol.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include <boost/lexical_cast.hpp>
#include <fstream>

NS_LOG_COMPONENT_DEFINE ("Ipv4SeqsAppTracer");

using namespace boost;
using namespace std;

namespace ns3 {

Ipv4SeqsAppTracer::Ipv4SeqsAppTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
  : Ipv4AppTracer (node)
  , m_os (os)
{
}

boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<Ipv4SeqsAppTracer> > >
Ipv4SeqsAppTracer::InstallAll (const std::string &file)
{
  std::list<Ptr<Ipv4SeqsAppTracer> > tracers;
  boost::shared_ptr<std::ofstream> outputStream (new std::ofstream ());
  outputStream->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

  if (!outputStream->is_open ())
    return boost::make_tuple (outputStream, tracers);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));

      Ptr<Ipv4SeqsAppTracer> trace = Create<Ipv4SeqsAppTracer> (outputStream, *node);
      tracers.push_back (trace);
    }

  if (tracers.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      tracers.front ()->PrintHeader (*outputStream);
      *outputStream << "\n";
    }

  return boost::make_tuple (outputStream, tracers);
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
     << "Type\t"
     << "SeqNo";
}

void
Ipv4SeqsAppTracer::Print (std::ostream &os) const
{
}

#define PRINTER(type,size)                                         \
 *m_os                                                              \
 << Simulator::Now ().ToDouble (Time::S) << "\t"                   \
 << m_node << "\t"                                                 \
 << type << "\t"                                                   \
 << static_cast<uint32_t> (size / 1040.0) << std::endl;

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
      if (tcp.GetAckNumber ().GetValue () > 1000) // a little bit more cheating
        {
          PRINTER("InAck", tcp.GetAckNumber ().GetValue ());
        }
    }
}

} // namespace ns3
