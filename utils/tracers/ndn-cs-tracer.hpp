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

#ifndef CCNX_CS_TRACER_H
#define CCNX_CS_TRACER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include <ns3/nstime.h>
#include <ns3/event-id.h>
#include <ns3/node-container.h>

#include <tuple>
#include <map>
#include <list>

namespace ns3 {

class Node;
class Packet;

namespace ndn {

namespace cs {

/// @cond include_hidden
struct Stats {
  inline void
  Reset()
  {
    m_cacheHits = 0;
    m_cacheMisses = 0;
  }
  double m_cacheHits;
  double m_cacheMisses;
};
/// @endcond
}

/**
 * @ingroup ndn-tracers
 * @brief NDN tracer for cache performance (hits and misses)
 */
class CsTracer : public SimpleRefCount<CsTracer> {
public:
  /**
   * @brief Helper method to install tracers on all simulation nodes
   *
   * @param file File to which traces will be written.  If filename is -, then std::out is used
   * @param averagingPeriod How often data will be written into the trace file (default, every half
   *second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This
   *tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   *
   */
  static void
  InstallAll(const std::string& file, Time averagingPeriod = Seconds(0.5));

  /**
   * @brief Helper method to install tracers on the selected simulation nodes
   *
   * @param nodes Nodes on which to install tracer
   * @param file File to which traces will be written.  If filename is -, then std::out is used
   * @param averagingPeriod How often data will be written into the trace file (default, every half
   *second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This
   *tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   *
   */
  static void
  Install(const NodeContainer& nodes, const std::string& file, Time averagingPeriod = Seconds(0.5));

  /**
   * @brief Helper method to install tracers on a specific simulation node
   *
   * @param nodes Nodes on which to install tracer
   * @param file File to which traces will be written.  If filename is -, then std::out is used
   * @param averagingPeriod How often data will be written into the trace file (default, every half
   *second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This
   *tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   *
   */
  static void
  Install(Ptr<Node> node, const std::string& file, Time averagingPeriod = Seconds(0.5));

  /**
   * @brief Helper method to install tracers on a specific simulation node
   *
   * @param nodes Nodes on which to install tracer
   * @param outputStream Smart pointer to a stream
   * @param averagingPeriod How often data will be written into the trace file (default, every half
   *second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This
   *tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   */
  static Ptr<CsTracer>
  Install(Ptr<Node> node, shared_ptr<std::ostream> outputStream,
          Time averagingPeriod = Seconds(0.5));

  /**
   * @brief Explicit request to remove all statically created tracers
   *
   * This method can be helpful if simulation scenario contains several independent run,
   * or if it is desired to do a postprocessing of the resulting data
   */
  static void
  Destroy();

  /**
   * @brief Trace constructor that attaches to the node using node pointer
   * @param os    reference to the output stream
   * @param node  pointer to the node
   */
  CsTracer(shared_ptr<std::ostream> os, Ptr<Node> node);

  /**
   * @brief Trace constructor that attaches to the node using node name
   * @param os        reference to the output stream
   * @param nodeName  name of the node registered using Names::Add
   */
  CsTracer(shared_ptr<std::ostream> os, const std::string& node);

  /**
   * @brief Destructor
   */
  ~CsTracer();

  /**
   * @brief Print head of the trace (e.g., for post-processing)
   *
   * @param os reference to output stream
   */
  void
  PrintHeader(std::ostream& os) const;

  /**
   * @brief Print current trace data
   *
   * @param os reference to output stream
   */
  void
  Print(std::ostream& os) const;

private:
  void
  Connect();

  void
  CacheHits(const Interest&, const Data&);

  void
  CacheMisses(const Interest&);

private:
  void
  SetAveragingPeriod(const Time& period);

  void
  Reset();

  void
  PeriodicPrinter();

private:
  std::string m_node;
  Ptr<Node> m_nodePtr;

  shared_ptr<std::ostream> m_os;

  Time m_period;
  EventId m_printEvent;
  cs::Stats m_stats;
};

/**
 * @brief Helper to dump the trace to an output stream
 */
inline std::ostream&
operator<<(std::ostream& os, const CsTracer& tracer)
{
  os << "# ";
  tracer.PrintHeader(os);
  os << "\n";
  tracer.Print(os);
  return os;
}

} // namespace ndn
} // namespace ns3

#endif // CCNX_CS_TRACER_H
