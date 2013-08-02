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

#ifndef CCNX_APP_TRACER_H
#define CCNX_APP_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ccnx.h"

namespace ns3 {

class CcnxApp;

class CcnxAppTracer : public SimpleRefCount<CcnxAppTracer>
{
public:
  CcnxAppTracer (const std::string &app, Ptr<Node> node, const std::string &appId = "*");
  CcnxAppTracer (const std::string &app, const std::string &node, const std::string &appId = "*");
  virtual ~CcnxAppTracer ()  { };

  void
  Connect ();

  virtual void
  PrintHeader (std::ostream &os) const = 0;
  
  virtual void
  Print (std::ostream &os) const = 0;

  virtual void
  OutInterests (std::string context,
                Ptr<const CcnxInterest>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

  virtual void
  InInterests  (std::string context,
                Ptr<const CcnxInterest>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

  virtual void
  InNacks (std::string context,
           Ptr<const CcnxInterest>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

  virtual void
  OutData (std::string context,
           Ptr<const CcnxData>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;
  
  virtual void
  InData  (std::string context,
           Ptr<const CcnxData>, Ptr<const Packet>, Ptr<CcnxApp>, Ptr<CcnxFace>) = 0;

protected:
  std::string m_app;
  std::string m_appId;
  std::string m_node;
  Ptr<Node> m_nodePtr;
};

inline std::ostream&
operator << (std::ostream &os, const CcnxAppTracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ns3

#endif // CCNX_APP_TRACER_H
