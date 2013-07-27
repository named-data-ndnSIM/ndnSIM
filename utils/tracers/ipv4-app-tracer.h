/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012 UCLA
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

#ifndef IPV4_APP_TRACER_H
#define IPV4_APP_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ipv4.h"

namespace ns3 {

class Ipv4Header;

/**
 * @ingroup ndn-tracers
 * @brief Base class for IPv4/TCP based applications
 */
class Ipv4AppTracer : public SimpleRefCount<Ipv4AppTracer>
{
public:
  Ipv4AppTracer (Ptr<Node> node);
  virtual ~Ipv4AppTracer ()  { };

  void
  Connect ();

  virtual void
  PrintHeader (std::ostream &os) const = 0;

  virtual void
  Print (std::ostream &os) const = 0;

  virtual void
  Rx (std::string context,
      const Ipv4Header &, Ptr<const Packet>, uint32_t) = 0;

  virtual void
  Tx (std::string context,
      const Ipv4Header &, Ptr<const Packet>, uint32_t) = 0;

protected:
  std::string m_node;
  Ptr<Node> m_nodePtr;
};

inline std::ostream&
operator << (std::ostream &os, const Ipv4AppTracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ns3

#endif // IPV4_APP_TRACER_H
