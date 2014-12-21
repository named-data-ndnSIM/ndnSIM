/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "l2-tracer.hpp"

#include "ns3/node.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"

#include "ns3/point-to-point-net-device.h"
#include "ns3/queue.h"
#include <boost/lexical_cast.hpp>

namespace ns3 {

L2Tracer::L2Tracer(Ptr<Node> node)
  : m_nodePtr(node)
{
  m_node = boost::lexical_cast<std::string>(m_nodePtr->GetId());

  Connect();

  std::string name = Names::FindName(node);
  if (!name.empty()) {
    m_node = name;
  }
}

void
L2Tracer::Connect()
{
  for (uint32_t devId = 0; devId < m_nodePtr->GetNDevices(); devId++) {
    Ptr<PointToPointNetDevice> p2pnd =
      DynamicCast<PointToPointNetDevice>(m_nodePtr->GetDevice(devId));
    if (p2pnd) {
      p2pnd->GetQueue()->TraceConnectWithoutContext("Drop", MakeCallback(&L2Tracer::Drop, this));
    }
  }
}

} // namespace ns3
