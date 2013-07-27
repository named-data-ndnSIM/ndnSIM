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

#ifndef L2_TRACER_H
#define L2_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/packet.h"

namespace ns3 {

class Node;

/**
 * @ingroup ndn-tracers
 * @brief Link-layer tracer
 *
 * @todo Finish implementation
 */
class L2Tracer : public SimpleRefCount<L2Tracer>
{
public:
  L2Tracer (Ptr<Node> node);
  virtual ~L2Tracer () { };

  void
  Connect ();

  virtual void
  PrintHeader (std::ostream &os) const = 0;

  virtual void
  Print (std::ostream &os) const = 0;

  virtual void
  Drop (Ptr<const Packet>) = 0;

  // Rx/Tx is NetDevice specific
  // please refer to pyviz.cc in order to extend this tracer

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
operator << (std::ostream &os, const L2Tracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ns3

#endif // L2_TRACER_H
