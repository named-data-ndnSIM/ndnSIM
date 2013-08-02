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

#include "ccnx-seqs-app-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"

#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

namespace ns3 {
    
CcnxSeqsAppTracer::CcnxSeqsAppTracer (std::ostream &os, const std::string &app, Ptr<Node> node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_os (os)
{
}

CcnxSeqsAppTracer::CcnxSeqsAppTracer (std::ostream &os, const std::string &app, const std::string &node, const std::string &appId)
  : CcnxAppTracer (app, node, appId)
  , m_os (os)
{
}

void
CcnxSeqsAppTracer::Reset ()
{
}

void
CcnxSeqsAppTracer::PrintHeader (std::ostream &os) const
{
  os << "Time\t"
     << "Node\t"
     << "AppName\t"
     << "AppId\t"
     << "Type\t"
     << "SeqNo";
}

void
CcnxSeqsAppTracer::Print (std::ostream &os) const
{
}

#define PRINTER(type)                                              \
 m_os                                                              \
 << Simulator::Now ().ToDouble (Time::S) << "\t"                   \
 << m_node << "\t"                                                 \
 << m_app << "\t"                                                  \
 << m_appId << "\t"                                                \
 << type << "\t"                                                   \
 << header->GetName ().GetLastComponent () << std::endl;

void
CcnxSeqsAppTracer::OutInterests (std::string context,
                                 Ptr<const CcnxInterest> header, Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  PRINTER ("OutInterest");
}

void
CcnxSeqsAppTracer::OutData (std::string context,
                            Ptr<const CcnxData> header, Ptr<const Packet>,
                            Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  PRINTER ("OutData");
}

void
CcnxSeqsAppTracer::InInterests (std::string context,
                                Ptr<const CcnxInterest> header,
                                Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  PRINTER ("InInterest");
}

void
CcnxSeqsAppTracer::InNacks (std::string context,
                            Ptr<const CcnxInterest> header,
                            Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  PRINTER ("InNacks");
}
  
void
CcnxSeqsAppTracer::InData (std::string context,
                           Ptr<const CcnxData> header, Ptr<const Packet>,
                           Ptr<CcnxApp>, Ptr<CcnxFace>)
{
  PRINTER ("InData");
}

} // namespace ns3
