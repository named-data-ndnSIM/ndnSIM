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

#ifndef CCNX_RATE_L3_TRACER_H
#define CCNX_RATE_L3_TRACER_H

#include "ndn-l3-tracer.h"

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include <ns3/node-container.h>

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <list>

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-tracers
 * @brief NDN network-layer rate tracer
 */
class L3RateTracer : public L3Tracer
{
public:
  /**
   * @brief Helper method to install tracers on all simulation nodes
   *
   * @param file File to which traces will be written.  If filename is -, then std::out is used
   * @param averagingPeriod Defines averaging period for the rate calculation,
   *        as well as how often data will be written into the trace file (default, every half second)
   */
  static void
  InstallAll (const std::string &file, Time averagingPeriod = Seconds (0.5));

  /**
   * @brief Helper method to install tracers on the selected simulation nodes
   *
   * @param nodes Nodes on which to install tracer
   * @param file File to which traces will be written.  If filename is -, then std::out is used
   * @param averagingPeriod How often data will be written into the trace file (default, every half second)
   */
  static void
  Install (const NodeContainer &nodes, const std::string &file, Time averagingPeriod = Seconds (0.5));

  /**
   * @brief Helper method to install tracers on a specific simulation node
   *
   * @param nodes Nodes on which to install tracer
   * @param file File to which traces will be written.  If filename is -, then std::out is used
   * @param averagingPeriod How often data will be written into the trace file (default, every half second)
   */
  static void
  Install (Ptr<Node> node, const std::string &file, Time averagingPeriod = Seconds (0.5));

  /**
   * @brief Explicit request to remove all statically created tracers
   *
   * This method can be helpful if simulation scenario contains several independent run,
   * or if it is desired to do a postprocessing of the resulting data
   */
  static void
  Destroy ();
  
  /**
   * @brief Trace constructor that attaches to the node using node pointer
   * @param os    reference to the output stream
   * @param node  pointer to the node
   */
  L3RateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node);

  /**
   * @brief Trace constructor that attaches to the node using node name
   * @param os        reference to the output stream
   * @param nodeName  name of the node registered using Names::Add
   */
  L3RateTracer (boost::shared_ptr<std::ostream> os, const std::string &node);

  /**
   * @brief Destructor
   */
  virtual ~L3RateTracer ();

  /**
   * @brief Helper method to install tracers on a specific simulation node
   *
   * @param nodes Nodes on which to install tracer
   * @param outputStream Smart pointer to a stream
   * @param averagingPeriod How often data will be written into the trace file (default, every half second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   */
  static Ptr<L3RateTracer>
  Install (Ptr<Node> node, boost::shared_ptr<std::ostream> outputStream, Time averagingPeriod = Seconds (0.5));
  
  // from L3Tracer
  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

protected:
  // from L3Tracer
  virtual void
  OutInterests  (Ptr<const Interest>, Ptr<const Face>);

  virtual void
  InInterests   (Ptr<const Interest>, Ptr<const Face>);

  virtual void
  DropInterests (Ptr<const Interest>, Ptr<const Face>);

  virtual void
  OutNacks  (Ptr<const Interest>, Ptr<const Face>);

  virtual void
  InNacks   (Ptr<const Interest>, Ptr<const Face>);

  virtual void
  DropNacks (Ptr<const Interest>, Ptr<const Face>);

  virtual void
  OutData  (Ptr<const Data>, bool fromCache, Ptr<const Face>);

  virtual void
  InData   (Ptr<const Data>, Ptr<const Face>);

  virtual void
  DropData (Ptr<const Data>, Ptr<const Face>);

  virtual void
  SatisfiedInterests (Ptr<const pit::Entry>);

  virtual void
  TimedOutInterests (Ptr<const pit::Entry>);

private:
  void
  SetAveragingPeriod (const Time &period);

  void
  PeriodicPrinter ();

  void
  Reset ();

private:
  boost::shared_ptr<std::ostream> m_os;
  Time m_period;
  EventId m_printEvent;

  mutable std::map<Ptr<const Face>, boost::tuple<Stats, Stats, Stats, Stats> > m_stats;
};

} // namespace ndn
} // namespace ns3

#endif // CCNX_RATE_L3_TRACER_H
