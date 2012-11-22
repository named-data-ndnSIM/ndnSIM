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

#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <list>

namespace ns3 {
namespace ndn {

/**
 * @ingroup ccnx
 * @brief CCNx network-layer rate tracer
 */
class L3RateTracer : public L3Tracer
{
public:
  /**
   * @brief Network layer tracer constructor
   */
  L3RateTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node);
  L3RateTracer (boost::shared_ptr<std::ostream> os, const std::string &node);
  virtual ~L3RateTracer ();

  static boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<L3RateTracer> > >
  InstallAll (const std::string &file, Time averagingPeriod = Seconds (0.5));
  
  void
  SetAveragingPeriod (const Time &period);
  
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

private:
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
