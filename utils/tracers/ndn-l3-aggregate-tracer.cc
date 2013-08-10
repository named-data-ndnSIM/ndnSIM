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
#include "ns3/ndn-data.h"
#include "ns3/ndn-pit-entry.h"

#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/log.h"

#include <fstream>

NS_LOG_COMPONENT_DEFINE ("ndn.L3AggregateTracer");

namespace ns3 {
namespace ndn {

static std::list< boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<L3AggregateTracer> > > > g_tracers;

template<class T>
static inline void
NullDeleter (T *ptr)
{
}

void
L3AggregateTracer::Destroy ()
{
  g_tracers.clear ();
}

void
L3AggregateTracer::InstallAll (const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3AggregateTracer> > tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-")
    {
      boost::shared_ptr<std::ofstream> os (new std::ofstream ());
      os->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

      if (!os->is_open ())
        {
          NS_LOG_ERROR ("File " << file << " cannot be opened for writing. Tracing disabled");
          return;
        }

      outputStream = os;
    }
  else
    {
      outputStream = boost::shared_ptr<std::ostream> (&std::cout, NullDeleter<std::ostream>);
    }

  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node++)
    {
      Ptr<L3AggregateTracer> trace = Install (*node, outputStream, averagingPeriod);
      tracers.push_back (trace);
    }

  if (tracers.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      tracers.front ()->PrintHeader (*outputStream);
      *outputStream << "\n";
    }

  g_tracers.push_back (boost::make_tuple (outputStream, tracers));
}

void
L3AggregateTracer::Install (const NodeContainer &nodes, const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3AggregateTracer> > tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-")
    {
      boost::shared_ptr<std::ofstream> os (new std::ofstream ());
      os->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

      if (!os->is_open ())
        {
          NS_LOG_ERROR ("File " << file << " cannot be opened for writing. Tracing disabled");
          return;
        }

      outputStream = os;
    }
  else
    {
      outputStream = boost::shared_ptr<std::ostream> (&std::cout, NullDeleter<std::ostream>);
    }

  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node++)
    {
      Ptr<L3AggregateTracer> trace = Install (*node, outputStream, averagingPeriod);
      tracers.push_back (trace);
    }

  if (tracers.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      tracers.front ()->PrintHeader (*outputStream);
      *outputStream << "\n";
    }

  g_tracers.push_back (boost::make_tuple (outputStream, tracers));
}

void
L3AggregateTracer::Install (Ptr<Node> node, const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3AggregateTracer> > tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-")
    {
      boost::shared_ptr<std::ofstream> os (new std::ofstream ());
      os->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

      if (!os->is_open ())
        {
          NS_LOG_ERROR ("File " << file << " cannot be opened for writing. Tracing disabled");
          return;
        }

      outputStream = os;
    }
  else
    {
      outputStream = boost::shared_ptr<std::ostream> (&std::cout, NullDeleter<std::ostream>);
    }

  Ptr<L3AggregateTracer> trace = Install (node, outputStream, averagingPeriod);
  tracers.push_back (trace);

  if (tracers.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      tracers.front ()->PrintHeader (*outputStream);
      *outputStream << "\n";
    }

  g_tracers.push_back (boost::make_tuple (outputStream, tracers));
}


Ptr<L3AggregateTracer>
L3AggregateTracer::Install (Ptr<Node> node,
                            boost::shared_ptr<std::ostream> outputStream,
                            Time averagingPeriod/* = Seconds (0.5)*/)
{
  NS_LOG_DEBUG ("Node: " << node->GetId ());

  Ptr<L3AggregateTracer> trace = Create<L3AggregateTracer> (outputStream, node);
  trace->SetAveragingPeriod (averagingPeriod);

  return trace;
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
  << STATS(0).fieldName << "\t"                                         \
  << STATS(1).fieldName / 1024.0 << "\n";


void
L3AggregateTracer::Print (std::ostream &os) const
{
  Time time = Simulator::Now ();

  for (std::map<Ptr<const Face>, boost::tuple<Stats, Stats> >::iterator stats = m_stats.begin ();
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
    std::map<Ptr<const Face>, boost::tuple<Stats, Stats> >::iterator stats = m_stats.find (Ptr<const Face> (0));
    if (stats != m_stats.end ())
      {
        PRINTER ("SatisfiedInterests", m_satisfiedInterests);
        PRINTER ("TimedOutInterests", m_timedOutInterests);
      }
  }
}

void
L3AggregateTracer::OutInterests  (Ptr<const Interest> interest, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outInterests ++;
  if (interest->GetWire ())
    {
      m_stats[face].get<1> ().m_outInterests += interest->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::InInterests   (Ptr<const Interest> interest, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inInterests ++;
  if (interest->GetWire ())
    {
      m_stats[face].get<1> ().m_inInterests += interest->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::DropInterests (Ptr<const Interest> interest, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropInterests ++;
  if (interest->GetWire ())
    {
      m_stats[face].get<1> ().m_dropInterests += interest->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::OutNacks  (Ptr<const Interest> nack, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outNacks ++;
  if (nack->GetWire ())
    {
      m_stats[face].get<1> ().m_outNacks += nack->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::InNacks   (Ptr<const Interest> nack, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inNacks ++;
  if (nack->GetWire ())
    {
      m_stats[face].get<1> ().m_inNacks += nack->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::DropNacks (Ptr<const Interest> nack, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropNacks ++;
  if (nack->GetWire ())
    {
      m_stats[face].get<1> ().m_dropNacks += nack->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::OutData  (Ptr<const Data> data, 
                             bool fromCache, Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_outData ++;
  if (data->GetWire ())
    {
      m_stats[face].get<1> ().m_outData += data->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::InData   (Ptr<const Data> data, 
                             Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_inData ++;
  if (data->GetWire ())
    {
      m_stats[face].get<1> ().m_inData += data->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::DropData (Ptr<const Data> data, 
                             Ptr<const Face> face)
{
  m_stats[face].get<0> ().m_dropData ++;
  if (data->GetWire ())
    {
      m_stats[face].get<1> ().m_dropData += data->GetWire ()->GetSize ();
    }
}

void
L3AggregateTracer::SatisfiedInterests (Ptr<const pit::Entry>)
{
  m_stats[0].get<0> ().m_satisfiedInterests ++;
  // no "size" stats
}

void
L3AggregateTracer::TimedOutInterests (Ptr<const pit::Entry>)
{
  m_stats[0].get<0> ().m_timedOutInterests ++;
  // no "size" stats
}

} // namespace ndn
} // namespace ns3
