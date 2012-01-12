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

#ifndef CCNX_CONSUMER_WINDOW_TRACER_H
#define CCNX_CONSUMER_WINDOW_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class Node;

class WindowTracer : public SimpleRefCount<WindowTracer>
{
public:
  WindowTracer (std::ostream &os, Ptr<Node> node, const std::string &appId = "*");
  virtual ~WindowTracer () { };
                
  static void
  PrintHeader (std::ostream &os);
  
  virtual void
  OnWindowChange (std::string context,
                  uint32_t oldValue, uint32_t newValue);

protected:
  std::string m_appId;
  std::string m_node;
  std::string m_nodeName;
  Ptr<Node> m_nodePtr;
  std::ostream& m_os;
};

class CcnxConsumerWindowTracer : public WindowTracer
{
public:
  CcnxConsumerWindowTracer (std::ostream &os, Ptr<Node> node, const std::string &appId = "*")
    : WindowTracer (os, node, appId)
  { Connect (); }

  void
  Connect ();
};

class TcpCongestionWindowTracer : public WindowTracer
{
public:
  TcpCongestionWindowTracer (std::ostream &os, Ptr<Node> node, const std::string &appId = "*")
    : WindowTracer (os, node, appId)
  { Connect (); }

  void
  Connect ();
};


} // namespace ns3

#endif // CCNX_CONSUMER_WINDOW_TRACER_H
