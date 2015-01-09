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

#include "ndn-l3-rate-tracer.hpp"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/node-list.h"

#include "daemon/table/pit-entry.hpp"

#include <fstream>
#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE("ndn.L3RateTracer");

namespace ns3 {
namespace ndn {

static std::list<std::tuple<shared_ptr<std::ostream>, std::list<Ptr<L3RateTracer>>>>
  g_tracers;

void
L3RateTracer::Destroy()
{
  g_tracers.clear();
}

void
L3RateTracer::InstallAll(const std::string& file, Time averagingPeriod /* = Seconds (0.5)*/)
{
  std::list<Ptr<L3RateTracer>> tracers;
  shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    Ptr<L3RateTracer> trace = Install(*node, outputStream, averagingPeriod);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

void
L3RateTracer::Install(const NodeContainer& nodes, const std::string& file,
                      Time averagingPeriod /* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3RateTracer>> tracers;
  shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  for (NodeContainer::Iterator node = nodes.Begin(); node != nodes.End(); node++) {
    Ptr<L3RateTracer> trace = Install(*node, outputStream, averagingPeriod);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

void
L3RateTracer::Install(Ptr<Node> node, const std::string& file,
                      Time averagingPeriod /* = Seconds (0.5)*/)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<L3RateTracer>> tracers;
  shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  Ptr<L3RateTracer> trace = Install(node, outputStream, averagingPeriod);
  tracers.push_back(trace);

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

Ptr<L3RateTracer>
L3RateTracer::Install(Ptr<Node> node, shared_ptr<std::ostream> outputStream,
                      Time averagingPeriod /* = Seconds (0.5)*/)
{
  NS_LOG_DEBUG("Node: " << node->GetId());

  Ptr<L3RateTracer> trace = Create<L3RateTracer>(outputStream, node);
  trace->SetAveragingPeriod(averagingPeriod);

  return trace;
}

L3RateTracer::L3RateTracer(shared_ptr<std::ostream> os, Ptr<Node> node)
  : L3Tracer(node)
  , m_os(os)
{
  SetAveragingPeriod(Seconds(1.0));
}

L3RateTracer::L3RateTracer(shared_ptr<std::ostream> os, const std::string& node)
  : L3Tracer(node)
  , m_os(os)
{
  SetAveragingPeriod(Seconds(1.0));
}

L3RateTracer::~L3RateTracer()
{
  m_printEvent.Cancel();
}

void
L3RateTracer::SetAveragingPeriod(const Time& period)
{
  m_period = period;
  m_printEvent.Cancel();
  m_printEvent = Simulator::Schedule(m_period, &L3RateTracer::PeriodicPrinter, this);
}

void
L3RateTracer::PeriodicPrinter()
{
  Print(*m_os);
  Reset();

  m_printEvent = Simulator::Schedule(m_period, &L3RateTracer::PeriodicPrinter, this);
}

void
L3RateTracer::PrintHeader(std::ostream& os) const
{
  os << "Time"
     << "\t"

     << "Node"
     << "\t"
     << "FaceId"
     << "\t"
     << "FaceDescr"
     << "\t"

     << "Type"
     << "\t"
     << "Packets"
     << "\t"
     << "Kilobytes"
     << "\t"
     << "PacketRaw"
     << "\t"
     << "KilobytesRaw";
}

void
L3RateTracer::Reset()
{
  for (auto& stats : m_stats) {
    std::get<0>(stats.second).Reset();
    std::get<1>(stats.second).Reset();
  }
}

const double alpha = 0.8;

#define STATS(INDEX) std::get<INDEX>(stats.second)
#define RATE(INDEX, fieldName) STATS(INDEX).fieldName / m_period.ToDouble(Time::S)

#define PRINTER(printName, fieldName)                                                              \
  STATS(2).fieldName =                                                                             \
    /*new value*/ alpha * RATE(0, fieldName) + /*old value*/ (1 - alpha) * STATS(2).fieldName;     \
  STATS(3).fieldName = /*new value*/ alpha * RATE(1, fieldName) / 1024.0                           \
                       + /*old value*/ (1 - alpha) * STATS(3).fieldName;                           \
                                                                                                   \
  os << time.ToDouble(Time::S) << "\t" << m_node << "\t";                                          \
  if (stats.first != nullptr) {                                                                    \
    os << stats.first->getId() << "\t" << stats.first->getLocalUri() << "\t";                      \
  }                                                                                                \
  else {                                                                                           \
    os << "-1\tall\t";                                                                             \
  }                                                                                                \
  os << printName << "\t" << STATS(2).fieldName << "\t" << STATS(3).fieldName << "\t"              \
     << STATS(0).fieldName << "\t" << STATS(1).fieldName / 1024.0 << "\n";

void
L3RateTracer::Print(std::ostream& os) const
{
  Time time = Simulator::Now();

  for (auto& stats : m_stats) {
    if (stats.first == nullptr)
      continue;

    PRINTER("InInterests", m_inInterests);
    PRINTER("OutInterests", m_outInterests);

    PRINTER("InData", m_inData);
    PRINTER("OutData", m_outData);

    PRINTER("InSatisfiedInterests", m_satisfiedInterests);
    PRINTER("InTimedOutInterests", m_timedOutInterests);

    PRINTER("OutSatisfiedInterests", m_outSatisfiedInterests);
    PRINTER("OutTimedOutInterests", m_outTimedOutInterests);
  }

  {
    auto i = m_stats.find(nullptr);
    if (i != m_stats.end()) {
      auto& stats = *i;
      PRINTER("SatisfiedInterests", m_satisfiedInterests);
      PRINTER("TimedOutInterests", m_timedOutInterests);
    }
  }
}

void
L3RateTracer::OutInterests(const Interest& interest, const Face& face)
{
  std::get<0>(m_stats[face.shared_from_this()]).m_outInterests++;
  if (interest.hasWire()) {
    std::get<1>(m_stats[face.shared_from_this()]).m_outInterests +=
      interest.wireEncode().size();
  }
}

void
L3RateTracer::InInterests(const Interest& interest, const Face& face)
{
  std::get<0>(m_stats[face.shared_from_this()]).m_inInterests++;
  if (interest.hasWire()) {
    std::get<1>(m_stats[face.shared_from_this()]).m_inInterests +=
      interest.wireEncode().size();
  }
}

void
L3RateTracer::OutData(const Data& data, const Face& face)
{
  std::get<0>(m_stats[face.shared_from_this()]).m_outData++;
  if (data.hasWire()) {
    std::get<1>(m_stats[face.shared_from_this()]).m_outData +=
      data.wireEncode().size();
  }
}

void
L3RateTracer::InData(const Data& data, const Face& face)
{
  std::get<0>(m_stats[face.shared_from_this()]).m_inData++;
  if (data.hasWire()) {
    std::get<1>(m_stats[face.shared_from_this()]).m_inData +=
      data.wireEncode().size();
  }
}

void
L3RateTracer::SatisfiedInterests(const nfd::pit::Entry& entry, const Face&, const Data&)
{
  std::get<0>(m_stats[nullptr]).m_satisfiedInterests++;
  // no "size" stats

  for (const auto& in : entry.getInRecords()) {
    std::get<0>(m_stats[in.getFace()]).m_satisfiedInterests ++;
  }

  for (const auto& out : entry.getOutRecords()) {
    std::get<0>(m_stats[out.getFace()]).m_outSatisfiedInterests ++;
  }
}

void
L3RateTracer::TimedOutInterests(const nfd::pit::Entry& entry)
{
  std::get<0>(m_stats[nullptr]).m_timedOutInterests++;
  // no "size" stats

  for (const auto& in : entry.getInRecords()) {
    std::get<0>(m_stats[in.getFace()]).m_timedOutInterests++;
  }

  for (const auto& out : entry.getOutRecords()) {
    std::get<0>(m_stats[out.getFace()]).m_outTimedOutInterests++;
  }
}

} // namespace ndn
} // namespace ns3
