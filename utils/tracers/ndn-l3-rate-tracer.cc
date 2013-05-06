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

#include "ndn-l3-rate-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/node-list.h"

#include "ns3/ndn-app.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"
#include "ns3/ndn-pit-entry.h"

#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("ndn.L3RateTracer");

namespace ns3 {
namespace ndn {


boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<L3RateTracer> > >
L3RateTracer::InstallAll (const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  std::list<Ptr<L3RateTracer> > tracers;
  boost::shared_ptr<std::ofstream> outputStream (new std::ofstream ());
  outputStream->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

  if (!outputStream->is_open ())
    return boost::make_tuple (outputStream, tracers);

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));

      Ptr<L3RateTracer> trace = Create<L3RateTracer> (outputStream, *node);
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


L3RateTracer::L3RateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
  : L3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

L3RateTracer::L3RateTracer (boost::shared_ptr<std::ostream> os, const std::string &node)
  : L3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

L3RateTracer::~L3RateTracer ()
{
  m_printEvent.Cancel ();
}

void
L3RateTracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &L3RateTracer::PeriodicPrinter, this);
}

void
L3RateTracer::PeriodicPrinter ()
{
  Print (*m_os);
  Reset ();

  m_printEvent = Simulator::Schedule (m_period, &L3RateTracer::PeriodicPrinter, this);
}

void
L3RateTracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"

     << "Node" << "\t"
     << "FaceId" << "\t"
     << "FaceDescr" << "\t"

     << "Type" << "\t"
     << "Packets" << "\t"
     << "Kilobytes" << "\t"
     << "PacketRaw" << "\t"
     << "KilobytesRaw";
}

void
L3RateTracer::Reset ()
{
  for (std::map<Ptr<const Face>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
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
  os << time.ToDouble (Time::S) << "\t"                                 \
  << m_node << "\t";                                                    \
  if (stats->first)                                                     \
    {                                                                   \
      os                                                                \
        << stats->first->GetId () << "\t"                               \
        << *stats->first << "\t";                                       \
    }                                                                   \
  else                                                                  \
    {                                                                   \
      os << "-1\tall\t";                                                \
    }                                                                   \
  os                                                                    \
  << printName << "\t"                                                  \
  << STATS(2).fieldName << "\t"                                         \
  << STATS(3).fieldName << "\t"                                         \
  << STATS(0).fieldName << "\t"                                         \
  << STATS(1).fieldName / 1024.0 << "\n";

void
L3RateTracer::Print (std::ostream &os) const
{
  Time time = Simulator::Now ();

  for (std::map<Ptr<const Face>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      if (!stats->first)
        continue;

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

  {
    std::map<Ptr<const Face>, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.find (Ptr<const Face> (0));
    if (stats != m_stats.end ())
      {
        PRINTER ("SatisfiedInterests", m_satisfiedInterests);
        PRINTER ("TimedOutInterests", m_timedOutInterests);
      }
  }
}


void
L3RateTracer::OutInterests  (std::string context,
                             Ptr<const Interest> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outInterests ++;
  m_stats[face].get<1> ().m_outInterests += header->GetSerializedSize ();
}

void
L3RateTracer::InInterests   (std::string context,
                             Ptr<const Interest> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inInterests ++;
  m_stats[face].get<1> ().m_inInterests += header->GetSerializedSize ();
}

void
L3RateTracer::DropInterests (std::string context,
                             Ptr<const Interest> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropInterests ++;
  m_stats[face].get<1> ().m_dropInterests += header->GetSerializedSize ();
}

void
L3RateTracer::OutNacks  (std::string context,
                         Ptr<const Interest> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outNacks ++;
  m_stats[face].get<1> ().m_outNacks += header->GetSerializedSize ();
}

void
L3RateTracer::InNacks   (std::string context,
                         Ptr<const Interest> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inNacks ++;
  m_stats[face].get<1> ().m_inNacks += header->GetSerializedSize ();
}

void
L3RateTracer::DropNacks (std::string context,
                         Ptr<const Interest> header, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropNacks ++;
  m_stats[face].get<1> ().m_dropNacks += header->GetSerializedSize ();
}

void
L3RateTracer::OutData  (std::string context,
                        Ptr<const ContentObject> header, Ptr<const Packet> payload,
                        bool fromCache, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outData ++;
  m_stats[face].get<1> ().m_outData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3RateTracer::InData   (std::string context,
                        Ptr<const ContentObject> header, Ptr<const Packet> payload,
                        Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inData ++;
  m_stats[face].get<1> ().m_inData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3RateTracer::DropData (std::string context,
                        Ptr<const ContentObject> header, Ptr<const Packet> payload,
                        Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropData ++;
  m_stats[face].get<1> ().m_dropData += header->GetSerializedSize () + payload->GetSize ();
}

void
L3RateTracer::SatisfiedInterests (Ptr<const pit::Entry>)
{
  m_stats[0].get<0> ().m_satisfiedInterests ++;
  // no "size" stats
}

void
L3RateTracer::TimedOutInterests (Ptr<const pit::Entry>)
{
  m_stats[0].get<0> ().m_timedOutInterests ++;
  // no "size" stats
}


} // namespace ndn
} // namespace ns3
