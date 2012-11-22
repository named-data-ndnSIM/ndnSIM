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

namespace ns3 {
namespace ndn {

class L3AggregateTracer : public L3Tracer
{
public:
  L3AggregateTracer (Ptr<Node> node);
  L3AggregateTracer (const std::string &node);
  virtual ~L3AggregateTracer ();
  
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
  Reset ();

  void
  PeriodicPrinter ();
  
protected:
  Stats m_packets;
  Stats m_bytes;

  Time m_period;
  EventId m_printEvent;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_L3_AGGREGATE_TRACER_H
