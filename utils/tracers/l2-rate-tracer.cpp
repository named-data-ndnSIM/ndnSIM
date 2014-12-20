/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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

#include "l2-rate-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/node.h"
#include "ns3/log.h"

#include <boost/lexical_cast.hpp>
#include <fstream>

using namespace boost;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("L2RateTracer");

namespace ns3 {

static std::list< boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<L2RateTracer> > > > g_tracers;

template<class T>
static inline void
NullDeleter (T *ptr)
{
}

void
L2RateTracer::Destroy ()
{
  g_tracers.clear ();
}

void
L2RateTracer::InstallAll (const std::string &file, Time averagingPeriod/* = Seconds (0.5)*/)
{
  std::list<Ptr<L2RateTracer> > tracers;
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
      NS_LOG_DEBUG ("Node: " << lexical_cast<string> ((*node)->GetId ()));

      Ptr<L2RateTracer> trace = Create<L2RateTracer> (outputStream, *node);
      trace->SetAveragingPeriod (averagingPeriod);
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


L2RateTracer::L2RateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node)
  : L2Tracer (node)
  , m_os (os)
{
  SetAveragingPeriod (Seconds (1.0));
}

L2RateTracer::~L2RateTracer ()
{
  m_printEvent.Cancel ();
}

void
L2RateTracer::SetAveragingPeriod (const Time &period)
{
  m_period = period;
  m_printEvent.Cancel ();
  m_printEvent = Simulator::Schedule (m_period, &L2RateTracer::PeriodicPrinter, this);
}

void
L2RateTracer::PeriodicPrinter ()
{
  Print (*m_os);
  Reset ();

  m_printEvent = Simulator::Schedule (m_period, &L2RateTracer::PeriodicPrinter, this);
}

void
L2RateTracer::PrintHeader (std::ostream &os) const
{
  os << "Time" << "\t"

     << "Node" << "\t"
     << "Interface" << "\t"

     << "Type" << "\t"
     << "Packets" << "\t"
     << "Kilobytes" << "\t"
     << "PacketsRaw" << "\t"
     << "KilobytesRaw";
}

void
L2RateTracer::Reset ()
{
  m_stats.get<0> ().Reset ();
  m_stats.get<1> ().Reset ();
}

const double alpha = 0.8;

#define STATS(INDEX) m_stats.get<INDEX> ()
#define RATE(INDEX, fieldName) STATS(INDEX).fieldName / m_period.ToDouble (Time::S)

#define PRINTER(printName, fieldName, interface)                        \
STATS(2).fieldName = /*new value*/alpha * RATE(0, fieldName) + /*old value*/(1-alpha) * STATS(2).fieldName; \
STATS(3).fieldName = /*new value*/alpha * RATE(1, fieldName) / 1024.0 + /*old value*/(1-alpha) * STATS(3).fieldName; \
                                                                        \
 os << time.ToDouble (Time::S) << "\t"                                  \
 << m_node << "\t"                                                      \
 << interface << "\t"                                                  \
 << printName << "\t"                                                   \
 << STATS(2).fieldName  << "\t"                                         \
 << STATS(3).fieldName << "\t"                                         \
 << STATS(0).fieldName << "\t"                                         \
 << STATS(1).fieldName / 1024.0 << "\n";

void
L2RateTracer::Print (std::ostream &os) const
{
  Time time = Simulator::Now ();

  PRINTER ("Drop", m_drop, "combined");
}

void
L2RateTracer::Drop (Ptr<const Packet> packet)
{
  // no interface information... this should be part of this L2Tracer object data

  m_stats.get<0> ().m_drop ++;
  m_stats.get<1> ().m_drop += packet->GetSize ();
}

} // namespace ns3
