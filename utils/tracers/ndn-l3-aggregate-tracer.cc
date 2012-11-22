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

#include "ndn-l3-aggregate-tracer.h"

#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/ndn-app.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"

namespace ns3 {
namespace ndn {

L3AggregateTracer::L3AggregateTracer (Ptr<Node> node)
: L3Tracer (node)
{
  Reset ();
}

L3AggregateTracer::L3AggregateTracer (const std::string &node)
: L3Tracer (node)
{
  Reset ();
}

L3AggregateTracer::~L3AggregateTracer ()
{
};

void
L3AggregateTracer::Reset ()
{
  m_packets.Reset ();
  m_bytes.Reset ();
}


void
L3AggregateTracer::PrintHeader (std::ostream &os) const
{
  os << "Node" << "\t"
     << "InInterests" << "\t"
     << "OutInterests" << "\t"
     << "DropInterests" << "\t"
    
     << "InNacks" << "\t"
     << "OutNacks" << "\t"
     << "DropNacks" << "\t"
    
     << "InData" << "\t"
     << "OutData" << "\t"
     << "DropData" << "\t"
    
     << "InInterestsBytes" << "\t"
     << "OutInterestsBytes" << "\t"
     << "DropInterestsBytes" << "\t"
    
     << "InNacksBytes" << "\t"
     << "OutNacksBytes" << "\t"
     << "DropNacksBytes" << "\t"
    
     << "InDataBytes" << "\t"
     << "OutDataBytes" << "\t"
     << "DropDataBytes";
}

void
L3AggregateTracer::Print (std::ostream &os) const
{
  os << m_node << "\t"
     << m_packets.m_inInterests   << "\t"
     << m_packets.m_outInterests  << "\t"
     << m_packets.m_dropInterests << "\t"

     << m_packets.m_inNacks   << "\t"
     << m_packets.m_outNacks  << "\t"
     << m_packets.m_dropNacks << "\t"

     << m_packets.m_inData   << "\t"
     << m_packets.m_outData  << "\t"
     << m_packets.m_dropData << "\t"

     << m_bytes.m_inInterests   << "\t"
     << m_bytes.m_outInterests  << "\t"
     << m_bytes.m_dropInterests << "\t"

     << m_bytes.m_inNacks   << "\t"
     << m_bytes.m_outNacks  << "\t"
     << m_bytes.m_dropNacks << "\t"

     << m_bytes.m_inData   << "\t"
     << m_bytes.m_outData  << "\t"
     << m_bytes.m_dropData;
}

void
L3AggregateTracer::OutInterests  (std::string context,
                                  Ptr<const InterestHeader> header, Ptr<const Face>)
{
  m_packets.m_outInterests++;
  m_bytes.m_outInterests += header->GetSerializedSize ();
}

void
L3AggregateTracer::InInterests   (std::string context,
                                  Ptr<const InterestHeader> header, Ptr<const Face>)
{
  m_packets.m_inInterests++;
  m_bytes.m_inInterests += header->GetSerializedSize ();
}

void
L3AggregateTracer::DropInterests (std::string context,
                                  Ptr<const InterestHeader> header, Ptr<const Face>)
{
  m_packets.m_dropInterests++;
  m_bytes.m_dropInterests += header->GetSerializedSize ();
}

void
L3AggregateTracer::OutNacks  (std::string context,
                              Ptr<const InterestHeader> header, Ptr<const Face>)
{
  m_packets.m_outNacks++;
  m_bytes.m_outNacks += header->GetSerializedSize ();
}

void
L3AggregateTracer::InNacks   (std::string context,
                              Ptr<const InterestHeader> header, Ptr<const Face>)
{
  m_packets.m_inNacks++;
  m_bytes.m_inNacks += header->GetSerializedSize ();
}

void
L3AggregateTracer::DropNacks (std::string context,
                              Ptr<const InterestHeader> header, Ptr<const Face>)
{
  m_packets.m_dropNacks++;
  m_bytes.m_dropNacks += header->GetSerializedSize ();
}

void
L3AggregateTracer::OutData  (std::string context,
                             Ptr<const ContentObjectHeader> header, Ptr<const Packet> payload,
                             bool fromCache, Ptr<const Face>)
{
  m_packets.m_outData++;
  m_bytes.m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3AggregateTracer::InData   (std::string context,
                             Ptr<const ContentObjectHeader> header, Ptr<const Packet> payload,
                             Ptr<const Face>)
{
  m_packets.m_inData++;
  m_bytes.m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3AggregateTracer::DropData (std::string context,
                             Ptr<const ContentObjectHeader> header, Ptr<const Packet> payload,
                             Ptr<const Face>)
{
  m_packets.m_dropData++;
  m_bytes.m_dropData += header->GetSerializedSize () + payload->GetSize ();
}

} // namespace ndn
} // namespace ns3
