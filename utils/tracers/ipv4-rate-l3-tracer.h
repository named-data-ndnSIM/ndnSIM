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

#ifndef IPV4_RATE_L3_TRACER_H
#define IPV4_RATE_L3_TRACER_H

#include "ipv4-l3-tracer.h"

#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <map>

namespace ns3 {

/**
 * @ingroup ccnx
 * @brief CCNx network-layer rate tracer
 */
class Ipv4RateL3Tracer : public Ipv4L3Tracer
{
public:
  /**
   * @brief Network layer tracer constructor
   */
  Ipv4RateL3Tracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node);
  virtual ~Ipv4RateL3Tracer ();

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
  static boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<Ipv4RateL3Tracer> > >
  InstallAll (const std::string &file, Time averagingPeriod = Seconds (0.5));

  void
  SetAveragingPeriod (const Time &period);

  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  Rx  (std::string context,
       Ptr<const Packet>, Ptr<Ipv4>,  uint32_t);

  virtual void
  Tx   (std::string context,
        Ptr<const Packet>, Ptr<Ipv4>,  uint32_t);

  virtual void
  Drop (std::string context,
        const Ipv4Header &, Ptr<const Packet>, Ipv4L3Protocol::DropReason, Ptr<Ipv4>, uint32_t);

private:
  void
  PeriodicPrinter ();

  void
  Reset ();

private:
  boost::shared_ptr<std::ostream> m_os;
  Time m_period;
  EventId m_printEvent;

  mutable std::map<uint32_t, boost::tuple<Stats, Stats, Stats, Stats> > m_stats;
};

} // namespace ns3

#endif // IPV4_RATE_L3_TRACER_H
