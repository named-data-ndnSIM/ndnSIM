/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "l2-rate-tracer.hpp"

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

NS_LOG_COMPONENT_DEFINE("L2RateTracer");

namespace ns3 {

static std::list<std::tuple<std::shared_ptr<std::ostream>, std::list<Ptr<L2RateTracer>>>>
  g_tracers;

void
L2RateTracer::Destroy()
{
  g_tracers.clear();
}

void
L2RateTracer::InstallAll(const std::string& file, Time averagingPeriod /* = Seconds (0.5)*/)
{
  std::list<Ptr<L2RateTracer>> tracers;
  std::shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    std::shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = std::shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    NS_LOG_DEBUG("Node: " << boost::lexical_cast<std::string>((*node)->GetId()));

    Ptr<L2RateTracer> trace = Create<L2RateTracer>(outputStream, *node);
    trace->SetAveragingPeriod(averagingPeriod);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

L2RateTracer::L2RateTracer(std::shared_ptr<std::ostream> os, Ptr<Node> node)
  : L2Tracer(node)
  , m_os(os)
{
  SetAveragingPeriod(Seconds(1.0));
}

L2RateTracer::~L2RateTracer()
{
  m_printEvent.Cancel();
}

void
L2RateTracer::SetAveragingPeriod(const Time& period)
{
  m_period = period;
  m_printEvent.Cancel();
  m_printEvent = Simulator::Schedule(m_period, &L2RateTracer::PeriodicPrinter, this);
}

void
L2RateTracer::PeriodicPrinter()
{
  Print(*m_os);
  Reset();

  m_printEvent = Simulator::Schedule(m_period, &L2RateTracer::PeriodicPrinter, this);
}

void
L2RateTracer::PrintHeader(std::ostream& os) const
{
  os << "Time"
     << "\t"

     << "Node"
     << "\t"
     << "Interface"
     << "\t"

     << "Type"
     << "\t"
     << "Packets"
     << "\t"
     << "Kilobytes"
     << "\t"
     << "PacketsRaw"
     << "\t"
     << "KilobytesRaw";
}

void
L2RateTracer::Reset()
{
  std::get<0>(m_stats).Reset();
  std::get<1>(m_stats).Reset();
}

const double alpha = 0.8;

#define STATS(INDEX) std::get<INDEX>(m_stats)
#define RATE(INDEX, fieldName) STATS(INDEX).fieldName / m_period.ToDouble(Time::S)

#define PRINTER(printName, fieldName, interface)                                                   \
  STATS(2).fieldName =                                                                             \
    /*new value*/ alpha * RATE(0, fieldName) + /*old value*/ (1 - alpha) * STATS(2).fieldName;     \
  STATS(3).fieldName = /*new value*/ alpha * RATE(1, fieldName) / 1024.0                           \
                       + /*old value*/ (1 - alpha) * STATS(3).fieldName;                           \
                                                                                                   \
  os << time.ToDouble(Time::S) << "\t" << m_node << "\t" << interface << "\t" << printName << "\t" \
     << STATS(2).fieldName << "\t" << STATS(3).fieldName << "\t" << STATS(0).fieldName << "\t"     \
     << STATS(1).fieldName / 1024.0 << "\n";

void
L2RateTracer::Print(std::ostream& os) const
{
  Time time = Simulator::Now();

  PRINTER("Drop", m_drop, "combined");
}

void
L2RateTracer::Drop(Ptr<const Packet> packet)
{
  // no interface information... this should be part of this L2Tracer object data

  std::get<0>(m_stats).m_drop++;
  std::get<1>(m_stats).m_drop += packet->GetSize();
}

} // namespace ns3
