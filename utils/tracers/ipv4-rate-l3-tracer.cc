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

#include "ipv4-rate-l3-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/log.h"

#include "ns3/ipv4-header.h"

#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("Ipv4RateL3Tracer");

namespace ns3 {

template<class T>
static inline void
NullDeleter (T *ptr)
{
}

boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<Ipv4RateL3Tracer> > >
Ipv4RateL3Tracer::InstallAll (const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  std::list<Ptr<Ipv4RateL3Tracer> > tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-")
    {
      boost::shared_ptr<std::ofstream> os (new std::ofstream ());
      os->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

      if (!os->is_open ())
        return boost::make_tuple (outputStream, tracers);

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
      Ptr<Ipv4RateL3Tracer> trace = Install (*node, outputStream, averagingPeriod);
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

boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<Ipv4RateL3Tracer> > >
Ipv4RateL3Tracer::Install (const NodeContainer &nodes, const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<Ipv4RateL3Tracer> > tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-")
    {
      boost::shared_ptr<std::ofstream> os (new std::ofstream ());
      os->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

      if (!os->is_open ())
        return boost::make_tuple (outputStream, tracers);

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
      Ptr<Ipv4RateL3Tracer> trace = Install (*node, outputStream, averagingPeriod);
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

boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<Ipv4RateL3Tracer> > >
Ipv4RateL3Tracer::Install (Ptr<Node> node, const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<Ipv4RateL3Tracer> > tracers;
  boost::shared_ptr<std::ostream> outputStream;
  if (file != "-")
    {
      boost::shared_ptr<std::ofstream> os (new std::ofstream ());
      os->open (file.c_str (), std::ios_base::out | std::ios_base::trunc);

      if (!os->is_open ())
        return boost::make_tuple (outputStream, tracers);

      outputStream = os;
    }
  else
    {
      outputStream = boost::shared_ptr<std::ostream> (&std::cout, NullDeleter<std::ostream>);
    }

  Ptr<Ipv4RateL3Tracer> trace = Install (node, outputStream, averagingPeriod);
  tracers.push_back (trace);

  if (tracers.size () > 0)
    {
      // *m_l3RateTrace << "# "; // not necessary for R's read.table
      tracers.front ()->PrintHeader (*outputStream);
      *outputStream << "\n";
    }

  return boost::make_tuple (outputStream, tracers);
}


Ptr<Ipv4RateL3Tracer>
Ipv4RateL3Tracer::Install (Ptr<Node> node,
                           boost::shared_ptr<std::ostream> outputStream,
                           Time averagingPeriod/* = Seconds (0.5)*/)
{
  NS_LOG_DEBUG ("Node: " << node->GetId ());

  Ptr<Ipv4RateL3Tracer> trace = Create<Ipv4RateL3Tracer> (outputStream, node);
  trace->SetAveragingPeriod (averagingPeriod);

  return trace;
}


Ipv4RateL3Tracer::Ipv4RateL3Tracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
  : Ipv4L3Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

Ipv4RateL3Tracer::~Ipv4RateL3Tracer ()
{
  m_printEvent.Cancel ();
}

void
Ipv4RateL3Tracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &Ipv4RateL3Tracer::PeriodicPrinter, this);
}

void
Ipv4RateL3Tracer::PeriodicPrinter ()
{
  Print (*m_os);
  Reset ();

  m_printEvent = Simulator::Schedule (m_period, &Ipv4RateL3Tracer::PeriodicPrinter, this);
}

void
Ipv4RateL3Tracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"

     << "Node" << "\t"
     << "Interface" << "\t"

     << "Type" << "\t"
     << "Packets" << "\t"
     << "Kilobytes";
}

void
Ipv4RateL3Tracer::Reset ()
{
  for (std::map<uint32_t, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
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
 << stats->first << "\t"                                      \
 << printName << "\t"                                                   \
 << STATS(2).fieldName  << "\t"                                        \
 << STATS(3).fieldName << "\n";

void
Ipv4RateL3Tracer::Print (std::ostream &os) const
{
  for (std::map<uint32_t, boost::tuple<Stats, Stats, Stats, Stats> >::iterator stats = m_stats.begin ();
       stats != m_stats.end ();
       stats++)
    {
      Time time = Simulator::Now ();

      PRINTER ("In",   m_in);
      PRINTER ("Out",  m_out);
      PRINTER ("Drop", m_drop);
    }
}

void
Ipv4RateL3Tracer::Rx  (Ptr<const Packet> packet, Ptr<Ipv4> ipv4,  uint32_t iface)
{
  m_stats[iface].get<0> ().m_in ++;
  m_stats[iface].get<1> ().m_in += packet->GetSize ();
}

void
Ipv4RateL3Tracer::Tx   (Ptr<const Packet> packet, Ptr<Ipv4> ipv4,  uint32_t iface)
{
  m_stats[iface].get<0> ().m_out ++;
  m_stats[iface].get<1> ().m_out += packet->GetSize ();
}

void
Ipv4RateL3Tracer::Drop (const Ipv4Header &header, Ptr<const Packet> packet, Ipv4L3Protocol::DropReason, Ptr<Ipv4> ipv4, uint32_t iface)
{
  m_stats[iface].get<0> ().m_drop ++;
  m_stats[iface].get<1> ().m_drop += packet->GetSize ();
}


} // namespace ns3
