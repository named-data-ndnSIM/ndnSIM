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
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

namespace ns3 {

/**
 * @ingroup ndn-tracers
 * @brief Helper to track application-level sequence numbers (approximated from TCP ACKs)
 */
class Ipv4SeqsAppTracer : public Ipv4AppTracer
{
public:
  Ipv4SeqsAppTracer (boost::shared_ptr<std::ostream> os, Ptr<Node> node);
  virtual ~Ipv4SeqsAppTracer () { };

  /**
   * @brief Helper method to install tracers on all simulation nodes
   *
   * @param file File to which traces will be written
   *
   * @returns a tuple of reference to output stream and list of tracers. !!! Attention !!! This tuple needs to be preserved
   *          for the lifetime of simulation, otherwise SEGFAULTs are inevitable
   *
   */
  static boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<Ipv4SeqsAppTracer> > >
  InstallAll (const std::string &file);


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
  boost::shared_ptr<std::ostream> m_os;
};

} // namespace ns3

#endif // IPV4_AGGREGATE_APP_TRACER_H
