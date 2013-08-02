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

#include "ccnx-aggregate-app-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

namespace ns3 {
    
CcnxAggregateAppTracer::CcnxAggregateAppTracer (const std::string &app, Ptr<Node> node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_inInterests (0)
  , m_outInterests (0)
  , m_inNacks (0)
  , m_inData (0)
  , m_outData (0)

  , m_inInterestsBytes (0)
  , m_outInterestsBytes (0)
  , m_inNacksBytes (0)
  , m_inDataBytes (0)
  , m_outDataBytes (0)
{
}

CcnxAggregateAppTracer::CcnxAggregateAppTracer (const std::string &app, const std::string &node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_inInterests (0)
  , m_outInterests (0)
  , m_inNacks (0)
  , m_inData (0)
  , m_outData (0)

  , m_inInterestsBytes (0)
  , m_outInterestsBytes (0)
  , m_inNacksBytes (0)
  , m_inDataBytes (0)
  , m_outDataBytes (0)
{
}

void
CcnxAggregateAppTracer::Reset ()
{
  m_inInterests = 0;
  m_outInterests = 0;
  m_inNacks = 0;
  m_inData = 0;
  m_outData = 0;

  m_inInterestsBytes = 0;
  m_outInterestsBytes = 0;
  m_inNacksBytes = 0;
  m_inDataBytes = 0;
  m_outDataBytes = 0;
}

void
CcnxAggregateAppTracer::PrintHeader (std::ostream &os) const
{
  os << "NodeId" << "\t"
     << "App" << "\t"
     << "AppId" << "\t"
     << "InInterests" << "\t"
     << "OutInterests" << "\t"
    
     << "InNacks" << "\t"
    
     << "InData" << "\t"
     << "OutData" << "\t"

     << "InInterestsBytes" << "\t"
     << "OutInterestsBytes" << "\t"
    
     << "InNacksBytes" << "\t"
    
     << "InDataBytes" << "\t"
     << "OutDataBytes";
}

void
CcnxAggregateAppTracer::Print (std::ostream &os) const
{
  os << m_node << "\t"
     << m_app << "\t"
     << m_appId << "\t"

     << m_inInterests << "\t"
     << m_outInterests << "\t"
    
     << m_inNacks << "\t"
    
     << m_inData << "\t"
     << m_outData << "\t"

     << m_inInterestsBytes << "\t"
     << m_outInterestsBytes << "\t"
    
     << m_inNacksBytes << "\t"
    
     << m_inDataBytes << "\t"
     << m_outDataBytes;
}

void
CcnxAggregateAppTracer::OutInterests (std::string context,
                                      Ptr<const CcnxInterest> header, Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_outInterests++;
  m_outInterestsBytes += header->GetSerializedSize ();
}

void
CcnxAggregateAppTracer::OutData (std::string context,
                                 Ptr<const CcnxData> header, Ptr<const Packet> payload,
                                 Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_outData++;
  m_outDataBytes += header->GetSerializedSize () + payload->GetSerializedSize ();
}

void
CcnxAggregateAppTracer::InInterests (std::string context,
                                     Ptr<const CcnxInterest> header,
                                     Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inInterests++;
  m_inInterestsBytes += header->GetSerializedSize ();
}

void
CcnxAggregateAppTracer::InNacks (std::string context,
                                 Ptr<const CcnxInterest> header,
                                 Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inNacks++;
  m_inNacksBytes += header->GetSerializedSize ();
}
  
void
CcnxAggregateAppTracer::InData (std::string context,
                                Ptr<const CcnxData> header, Ptr<const Packet> payload,
                                Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  m_inData++;
  m_inDataBytes += header->GetSerializedSize () + payload->GetSerializedSize ();
}

} // namespace ns3
