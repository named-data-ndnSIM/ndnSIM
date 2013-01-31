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

#ifndef IPV4_SEQS_APP_TRACER_H
#define IPV4_SEQS_APP_TRACER_H

#include "ipv4-app-tracer.h"

namespace ns3 {

class Ipv4SeqsAppTracer : public Ipv4AppTracer
{
public:
  Ipv4SeqsAppTracer (std::ostream &os, Ptr<Node> node, const std::string &appId = "*");
  virtual ~Ipv4SeqsAppTracer () { };

  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  Rx (std::string context,
      const Ipv4Header &, Ptr<const Packet>, uint32_t);

  virtual void
  Tx (std::string context,
      const Ipv4Header &, Ptr<const Packet>, uint32_t);

protected:
  void
  Reset ();

protected:
  std::ostream& m_os;
};

} // namespace ns3

#endif // IPV4_AGGREGATE_APP_TRACER_H
