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
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/log.h"

#include <fstream>

NS_LOG_COMPONENT_DEFINE ("ndn.L3AggregateTracer");

namespace ns3 {
namespace ndn {

boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<L3AggregateTracer> > >
L3AggregateTracer::InstallAll (const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;
  
  std::list<Ptr<L3AggregateTracer> > tracers;
  boost::shared_ptr<std::ofstream> outputStream (new std::ofstream ());
  outputStream->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

  if (!outputStream->is_open ())
    return boost::make_tuple (outputStream, tracers);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << (*node)->GetId ());

      Ptr<L3AggregateTracer> trace = Create<L3AggregateTracer> (outputStream, *node);
      trace->SetAveragingPeriod (averagingPeriod);
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



L3AggregateTracer::L3AggregateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
  : L3Tracer (node)
  , m_os (os)
{
  Reset ();
}

L3AggregateTracer::L3AggregateTracer (boost::shared_ptr<std::ostream> os, const std::string &node)
  : L3Tracer (node)
  , m_os (os)
{
  Reset ();
}

L3AggregateTracer::~L3AggregateTracer ()
{
};

void
L3AggregateTracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &L3AggregateTracer::PeriodicPrinter, this);
}

void
L3AggregateTracer::PeriodicPrinter ()
{
  Print (*m_os);
  Reset ();
  
  m_printEvent = Simulator::Schedule (m_period, &L3AggregateTracer::PeriodicPrinter, this);
}

void
L3AggregateTracer::PrintHeader (std::ostream &os) const
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
L3AggregateTracer::Reset ()
{
  for (std::map<Ptr<const Face>, boost::tuple<Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      stats->second.get<0> ().Reset ();
      stats->second.get<1> ().Reset ();
    }
}


#define STATS(INDEX) stats->second.get<INDEX> ()

#define PRINTER(printName, fieldName) \
  os << time.ToDouble (Time::S) << "\t"                                 \
  << m_node << "\t"                                                     \
  << stats->first->GetId () << "\t"                                     \
  << *stats->first << "\t"                                              \
  << printName << "\t"                                                  \
  << STATS(0).fieldName << "\t"                                         \
  << STATS(1).fieldName / 1024.0 << "\n";


void
L3AggregateTracer::Print (std::ostream &os) const
{
  for (std::map<Ptr<const Face>, boost::tuple<Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      Time time = Simulator::Now ();

      PRINTER ("InInterests",   m_inInterests);
      PRINTER ("OutInterests",  m_outInterests);
      // PRINTER ("DropInterests", m_dropInterests);
      
      PRINTER ("InNacks",   m_inNacks);
      PRINTER ("OutNacks",  m_outNacks);
      // PRINTER ("DropNacks", m_dropNacks);

      PRINTER ("InData",   m_inData);
      PRINTER ("OutData",  m_outData);
      // PRINTER ("DropData", m_dropData);
    }
}

void
L3AggregateTracer::OutInterests  (std::string context,
                                  Ptr<const InterestHeader> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outInterests ++;
  m_stats[face].get<1> ().m_outInterests += header->GetSerializedSize ();
}

void
L3AggregateTracer::InInterests   (std::string context,
                                  Ptr<const InterestHeader> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inInterests ++;
  m_stats[face].get<1> ().m_inInterests += header->GetSerializedSize ();
}

void
L3AggregateTracer::DropInterests (std::string context,
                                  Ptr<const InterestHeader> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropInterests ++;
  m_stats[face].get<1> ().m_dropInterests += header->GetSerializedSize ();
}

void
L3AggregateTracer::OutNacks  (std::string context,
                              Ptr<const InterestHeader> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outNacks ++;
  m_stats[face].get<1> ().m_outNacks += header->GetSerializedSize ();
}

void
L3AggregateTracer::InNacks   (std::string context,
                              Ptr<const InterestHeader> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inNacks ++;
  m_stats[face].get<1> ().m_inNacks += header->GetSerializedSize ();
}

void
L3AggregateTracer::DropNacks (std::string context,
                              Ptr<const InterestHeader> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropNacks ++;
  m_stats[face].get<1> ().m_dropNacks += header->GetSerializedSize ();
}

void
L3AggregateTracer::OutData  (std::string context,
                             Ptr<const ContentObjectHeader> header, Ptr<const Packet> payload,
                             bool fromCache, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outData ++;
  m_stats[face].get<1> ().m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3AggregateTracer::InData   (std::string context,
                             Ptr<const ContentObjectHeader> header, Ptr<const Packet> payload,
                             Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inData ++;
  m_stats[face].get<1> ().m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3AggregateTracer::DropData (std::string context,
                             Ptr<const ContentObjectHeader> header, Ptr<const Packet> payload,
                             Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropData ++;
  m_stats[face].get<1> ().m_dropData += header->GetSerializedSize () + payload->GetSize ();
}

} // namespace ndn
} // namespace ns3
