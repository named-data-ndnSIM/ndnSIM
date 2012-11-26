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

#ifndef NDN_L3_AGGREGATE_TRACER_H
#define NDN_L3_AGGREGATE_TRACER_H

#include "ndn-l3-tracer.h"

#include <ns3/nstime.h>
#include <ns3/event-id.h>

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <list>

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn
 * @brief CCNx network-layer tracer for aggregate packet counts
 */
class L3AggregateTracer : public L3Tracer
{
public:
  /**
   * @brief Trace constructor that attaches to the node using node pointer
   * @param os    reference to the output stream
   * @param node  pointer to the node
   */
  L3AggregateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node);
  
  /**
   * @brief Trace constructor that attaches to the node using node name
   * @param os        reference to the output stream
   * @param nodeName  name of the node registered using Names::Add
   */
  L3AggregateTracer (boost::shared_ptr<std::ostream> os, const std::string &nodeName);

  /**
   * @brief Destructor
   */
  virtual ~L3AggregateTracer ();

  /**
   * @brief Helper method to install tracers on all simulation nodes
   *
   * @param file File to which traces will be written
   * @param averagingPeriod How often data will be written into the trace file (default, every half second)
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   * 
   */
  static boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<L3AggregateTracer> > >
  InstallAll (const std::string &file, Time averagingPeriod = Seconds (0.5));

protected: 
  // from L3Tracer
  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  OutInterests  (std::string context,
                 Ptr<const InterestHeader>, Ptr<const Face>);

  virtual void
  InInterests   (std::string context,
                 Ptr<const InterestHeader>, Ptr<const Face>);

  virtual void
  DropInterests (std::string context,
                 Ptr<const InterestHeader>, Ptr<const Face>);
  
  virtual void
  OutNacks  (std::string context,
             Ptr<const InterestHeader>, Ptr<const Face>);

  virtual void
  InNacks   (std::string context,
             Ptr<const InterestHeader>, Ptr<const Face>);

  virtual void
  DropNacks (std::string context,
             Ptr<const InterestHeader>, Ptr<const Face>);
  
  virtual void
  OutData  (std::string context,
            Ptr<const ContentObjectHeader>, Ptr<const Packet>, bool fromCache, Ptr<const Face>);

  virtual void
  InData   (std::string context,
            Ptr<const ContentObjectHeader>, Ptr<const Packet>, Ptr<const Face>);

  virtual void
  DropData (std::string context,
            Ptr<const ContentObjectHeader>, Ptr<const Packet>, Ptr<const Face>);

protected:
  void
  SetAveragingPeriod (const Time &period);

  void
  Reset ();

  void
  PeriodicPrinter ();
  
protected:
  boost::shared_ptr<std::ostream> m_os;

  Time m_period;
  EventId m_printEvent;
  
  mutable std::map<Ptr<const Face>, boost::tuple<Stats, Stats> > m_stats;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_L3_AGGREGATE_TRACER_H
