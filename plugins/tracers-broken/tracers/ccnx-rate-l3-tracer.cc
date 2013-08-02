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

#include "ccnx-rate-l3-tracer.h"
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
    
CcnxRateL3Tracer::CcnxRateL3Tracer (std::ostream &os, Ptr<Node> node)
  : CcnxL3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

CcnxRateL3Tracer::CcnxRateL3Tracer (std::ostream &os, const std::string &node)
  : CcnxL3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

CcnxRateL3Tracer::~CcnxRateL3Tracer ()
{
  m_printEvent.Cancel ();
}

void
CcnxRateL3Tracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &CcnxRateL3Tracer::PeriodicPrinter, this);
}

void
CcnxRateL3Tracer::PeriodicPrinter ()
{
  Print (m_os);
  Reset ();
  
  m_printEvent = Simulator::Schedule (m_period, &CcnxRateL3Tracer::PeriodicPrinter, this);
}

void
CcnxRateL3Tracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"

     << "Node" << "\t"
     << "FaceId" << "\t"
     << "FaceDescr" << "\t"

     << "Type" << "\t"
     << "Packets" << "\t"
     << "Kilobytes";
}

void
CcnxRateL3Tracer::Reset ()
{
  for (std::map<Ptr<const CcnxFace>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      stats->second.get<0> ().Reset ();
      stats->second.get<1> ().Reset ();
    }
}

const double alpha = 0.8;

#define STATS(INDEX) stats->second.get<INDEX> ()
#define RATE(INDEX, fieldName) STATS(INDEX).fieldName / m_period.ToDouble (Time::S)

#define PRINTER(printName, fieldName) \
STATS(2).fieldName = /*new value*/alpha * RATE(0, fieldName) + /*old value*/(1-alpha) * STATS(2).fieldName; \
 STATS(3).fieldName = /*new value*/alpha * RATE(1, fieldName) / 1024.0 + /*old value*/(1-alpha) * STATS(3).fieldName; \
                                                                        \
os << time.ToDouble (Time::S) << "\t"                                   \
 << m_node << "\t"                                                      \
 << stats->first->GetId () << "\t"                                      \
 << *stats->first << "\t"                                               \
 << printName << "\t"                                                   \
 << STATS(2).fieldName  << "\t"                                        \
 << STATS(3).fieldName << "\n";

void
CcnxRateL3Tracer::Print (std::ostream &os) const
{
  for (std::map<Ptr<const CcnxFace>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      Time time = Simulator::Now ();

      PRINTER ("InInterests",   m_inInterests);
      PRINTER ("OutInterests",  m_outInterests);
      PRINTER ("DropInterests", m_dropInterests);
      
      PRINTER ("InNacks",   m_inNacks);
      PRINTER ("OutNacks",  m_outNacks);
      PRINTER ("DropNacks", m_dropNacks);

      PRINTER ("InData",   m_inData);
      PRINTER ("OutData",  m_outData);
      PRINTER ("DropData", m_dropData);
    }
}


void
CcnxRateL3Tracer::OutInterests  (std::string context,
                                      Ptr<const CcnxInterest> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_outInterests ++;
  m_stats[face].get<1> ().m_outInterests += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::InInterests   (std::string context,
                                 Ptr<const CcnxInterest> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_inInterests ++;
  m_stats[face].get<1> ().m_inInterests += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::DropInterests (std::string context,
                                      Ptr<const CcnxInterest> header, Ccnx::DropReason, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_dropInterests ++;
  m_stats[face].get<1> ().m_dropInterests += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::OutNacks  (std::string context,
                                  Ptr<const CcnxInterest> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_outNacks ++;
  m_stats[face].get<1> ().m_outNacks += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::InNacks   (std::string context,
                                  Ptr<const CcnxInterest> header, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_inNacks ++;
  m_stats[face].get<1> ().m_inNacks += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::DropNacks (std::string context,
                             Ptr<const CcnxInterest> header, Ccnx::DropReason, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_dropNacks ++;
  m_stats[face].get<1> ().m_dropNacks += header->GetSerializedSize ();
}

void
CcnxRateL3Tracer::OutData  (std::string context,
                            Ptr<const CcnxData> header, Ptr<const Packet> payload,
                            bool fromCache, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_inData ++;
  m_stats[face].get<1> ().m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxRateL3Tracer::InData   (std::string context,
                            Ptr<const CcnxData> header, Ptr<const Packet> payload,
                            Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_outData ++;
  m_stats[face].get<1> ().m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
CcnxRateL3Tracer::DropData (std::string context,
                            Ptr<const CcnxData> header, Ptr<const Packet> payload,
                            Ccnx::DropReason, Ptr<const CcnxFace> face)
{
  m_stats[face].get<0> ().m_dropData ++;
  m_stats[face].get<1> ().m_dropData += header->GetSerializedSize () + payload->GetSize ();
}

} // namespace ns3
