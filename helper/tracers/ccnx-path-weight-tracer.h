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

#ifndef CCNX_PATH_WEIGHT_TRACER_H
#define CCNX_PATH_WEIGHT_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/ccnx-path-stretch-tag.h"
#include <list>

namespace ns3 {

class Node;
class Packet;
class CcnxApp;

class CcnxPathWeightTracer : public SimpleRefCount<CcnxPathWeightTracer>
{
public:
  CcnxPathWeightTracer (std::ostream &os, Ptr<Node> node, std::string appId);
  virtual ~CcnxPathWeightTracer () { };

  void
  Connect ();
  
  virtual void
  PrintHeader (std::ostream &os) const;

  /**
   * \brief Process packet weight upon reception of packet on a local face
   */
  virtual void
  InLocalFace (std::string context,
               uint32_t weight, Ptr<Node> src, Ptr<Node> dst);

protected:
  std::ostream &m_os;
  std::string m_node;
  Ptr<Node> m_nodePtr;
  std::string m_appId;
};

} // namespace ns3

#endif // CCNX_PATH_WEIGHT_TRACER_H
