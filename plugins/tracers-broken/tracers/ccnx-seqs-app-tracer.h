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

#ifndef CCNX_SEQS_APP_TRACER_H
#define CCNX_SEQS_APP_TRACER_H

#include "ns3/ccnx-app-tracer.h"

namespace ns3 {

class CcnxSeqsAppTracer : public CcnxAppTracer
{
public:
  CcnxSeqsAppTracer (std::ostream &os, const std::string &app, Ptr<Node> node, const std::string &appId = "*");
  CcnxSeqsAppTracer (std::ostream &os, const std::string &app, const std::string &node, const std::string &appId = "*");
  virtual ~CcnxSeqsAppTracer () { };

  virtual void
  PrintHeader (std::ostream &os) const;

  virtual void
  Print (std::ostream &os) const;

  virtual void
  OutInterests (std::string context,
                Ptr<const CcnxInterest>, Ptr<CcnxApp>, Ptr<CcnxFace>);

  virtual void
  InInterests  (std::string context,
                Ptr<const CcnxInterest>, Ptr<CcnxApp>, Ptr<CcnxFace>);

  virtual void
  InNacks (std::string context,
           Ptr<const CcnxInterest>, Ptr<CcnxApp>, Ptr<CcnxFace>);

  virtual void
  OutData (std::string context,
           Ptr<const CcnxData>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>);
  
  virtual void
  InData  (std::string context,
           Ptr<const CcnxData>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>);

protected:
  void
  Reset ();

protected:
  std::ostream& m_os;
};

} // namespace ns3

#endif // CCNX_AGGREGATE_APP_TRACER_H
