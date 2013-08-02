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

#include "ccnx-aggregate-l3-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

namespace ns3 {
    
CcnxAggregateL3Tracer::CcnxAggregateL3Tracer (Ptr<Node> node)
  : CcnxL3Tracer (node)
{
  Reset ();
}

CcnxAggregateL3Tracer::CcnxAggregateL3Tracer (const std::string &node)
  : CcnxL3Tracer (node)
{
  Reset ();
}

void
CcnxAggregateL3Tracer::Stats::Reset ()
{
  m_inInterests = 0;
  m_outInterests = 0;
  m_dropInterests = 0;
  m_inNacks = 0;
  m_outNacks = 0;
  m_dropNacks = 0;
  m_inData = 0;
  m_outData = 0;
  m_dropData = 0;
}


void
CcnxAggregateL3Tracer::Reset ()
{
  m_packets.Reset ();
  m_bytes.Reset ();
}


void
CcnxAggregateL3Tracer::PrintHeader (std::ostream &os) const
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
CcnxAggregateL3Tracer::Print (std::ostream &os) const
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
CcnxAggregateL3Tracer::OutInterests  (std::string context,
                                      Ptr<const CcnxInterest> header, Ptr<const CcnxFace>)
{
  m_packets.m_outInterests++;
  m_bytes.m_outInterests += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::InInterests   (std::string context,
                                      Ptr<const CcnxInterest> header, Ptr<const CcnxFace>)
{
  m_packets.m_inInterests++;
  m_bytes.m_inInterests += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::DropInterests (std::string context,
                                      Ptr<const CcnxInterest> header, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_packets.m_dropInterests++;
  m_bytes.m_dropInterests += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::OutNacks  (std::string context,
                                  Ptr<const CcnxInterest> header, Ptr<const CcnxFace>)
{
  m_packets.m_outNacks++;
  m_bytes.m_outNacks += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::InNacks   (std::string context,
                                  Ptr<const CcnxInterest> header, Ptr<const CcnxFace>)
{
  m_packets.m_inNacks++;
  m_bytes.m_inNacks += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::DropNacks (std::string context,
                                  Ptr<const CcnxInterest> header, Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_packets.m_dropNacks++;
  m_bytes.m_dropNacks += header->GetSerializedSize ();
}

void
CcnxAggregateL3Tracer::OutData  (std::string context,
                                 Ptr<const CcnxData> header, Ptr<const Packet> payload,
                                 bool fromCache, Ptr<const CcnxFace>)
{
  m_packets.m_outData++;
  m_bytes.m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxAggregateL3Tracer::InData   (std::string context,
                                 Ptr<const CcnxData> header, Ptr<const Packet> payload,
                                 Ptr<const CcnxFace>)
{
  m_packets.m_inData++;
  m_bytes.m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxAggregateL3Tracer::DropData (std::string context,
                                 Ptr<const CcnxData> header, Ptr<const Packet> payload,
                                 Ccnx::DropReason, Ptr<const CcnxFace>)
{
  m_packets.m_dropData++;
  m_bytes.m_dropData += header->GetSerializedSize () + payload->GetSize ();
}

} // namespace ns3
