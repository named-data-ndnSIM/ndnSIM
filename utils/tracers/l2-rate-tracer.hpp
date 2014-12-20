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

#ifndef L2_RATE_TRACER_H
#define L2_RATE_TRACER_H

#include "l2-tracer.h"

#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace ns3 {

/**
 * @ingroup ndn-tracers
 * @brief Tracer to collect link-layer rate information about links
 *
 * @todo Finish implementation
 */
class L2RateTracer : public L2Tracer
{
public:
  /**
   * @brief Network layer tracer constructor
   */
  L2RateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node);
  virtual ~L2RateTracer ();

  /**
   * @brief Helper method to install tracers on all simulation nodes
   *
   * @param file File to which traces will be written
   * @param averagingPeriod Defines averaging period for the rate calculation,
   *        as well as how often data will be written into the trace file (default, every half second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   *
   */
  static void
  InstallAll (const std::string &file, Time averagingPeriod = Seconds (0.5));

  /**
   * @brief Explicit request to remove all statically created tracers
   *
   * This method can be helpful if simulation scenario contains several independent run,
   * or if it is desired to do a postprocessing of the resulting data
   */
  static void
  Destroy ();

  void
  SetAveragingPeriod (const Time &period);

  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  Drop (Ptr<const Packet>);

private:
  void
  PeriodicPrinter ();

  void
  Reset ();

private:
  boost::shared_ptr<std::ostream> m_os;
  Time m_period;
  EventId m_printEvent;

  mutable boost::tuple<Stats, Stats, Stats, Stats> m_stats;
};

} // namespace ns3

#endif // L2_RATE_TRACER_H
