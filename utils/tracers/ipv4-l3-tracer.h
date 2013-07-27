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

#ifndef IPV4_L3_TRACER_H
#define IPV4_L3_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ipv4-l3-protocol.h"

namespace ns3 {

class Node;

/**
 * @ingroup ndn-tracers
 * @brief Base class for IPv4 network-layer tracers
 */
class Ipv4L3Tracer : public SimpleRefCount<Ipv4L3Tracer>
{
public:
  Ipv4L3Tracer (Ptr<Node> node);
  virtual ~Ipv4L3Tracer () { };

  void
  Connect ();
  
  virtual void
  PrintHeader (std::ostream &os) const = 0;

  virtual void
  Print (std::ostream &os) const = 0;
  
  virtual void
  Rx  (Ptr<const Packet>, Ptr<Ipv4>,  uint32_t) = 0;

  virtual void
  Tx   (Ptr<const Packet>, Ptr<Ipv4>,  uint32_t) = 0;

  virtual void
  Drop (const Ipv4Header &, Ptr<const Packet>, Ipv4L3Protocol::DropReason, Ptr<Ipv4>, uint32_t) = 0;

protected:
  std::string m_node;
  Ptr<Node> m_nodePtr;

  struct Stats
  {
    void Reset ()
    {
      m_in = 0;
      m_out = 0;
      m_drop = 0;
    }
    
    uint64_t m_in;
    uint64_t m_out;
    uint64_t m_drop;
  };
};

inline std::ostream&
operator << (std::ostream &os, const Ipv4L3Tracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ns3

#endif // IPV4_L3_TRACER_H
