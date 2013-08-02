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

#ifndef CCNX_AGGREGATE_L3_TRACER_H
#define CCNX_AGGREGATE_L3_TRACER_H

#include "ns3/ccnx-l3-tracer.h"

namespace ns3 {

class CcnxAggregateL3Tracer : public CcnxL3Tracer
{
public:
  CcnxAggregateL3Tracer (Ptr<Node> node);
  CcnxAggregateL3Tracer (const std::string &node);
  virtual ~CcnxAggregateL3Tracer () { };
  
  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  OutInterests  (std::string context,
                 Ptr<const CcnxInterest>, Ptr<const CcnxFace>);

  virtual void
  InInterests   (std::string context,
                 Ptr<const CcnxInterest>, Ptr<const CcnxFace>);

  virtual void
  DropInterests (std::string context,
                 Ptr<const CcnxInterest>, Ccnx::DropReason, Ptr<const CcnxFace>);
  
  virtual void
  OutNacks  (std::string context,
             Ptr<const CcnxInterest>, Ptr<const CcnxFace>);

  virtual void
  InNacks   (std::string context,
             Ptr<const CcnxInterest>, Ptr<const CcnxFace>);

  virtual void
  DropNacks (std::string context,
             Ptr<const CcnxInterest>, Ccnx::DropReason, Ptr<const CcnxFace>);
  
  virtual void
  OutData  (std::string context,
            Ptr<const CcnxData>, Ptr<const Packet>, bool fromCache, Ptr<const CcnxFace>);

  virtual void
  InData   (std::string context,
            Ptr<const CcnxData>, Ptr<const Packet>, Ptr<const CcnxFace>);

  virtual void
  DropData (std::string context,
            Ptr<const CcnxData>, Ptr<const Packet>, Ccnx::DropReason, Ptr<const CcnxFace>);

protected:
  void
  Reset ();
  
protected:
  Stats m_packets;
  Stats m_bytes;
};

} // namespace ns3

#endif // CCNX_AGGREGATE_L3_TRACER_H
