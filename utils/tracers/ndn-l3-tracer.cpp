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

#include "ndn-l3-tracer.hpp"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"

#include <boost/lexical_cast.hpp>

#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"

namespace ns3 {
namespace ndn {

L3Tracer::L3Tracer(Ptr<Node> node)
  : m_nodePtr(node)
{
  m_node = boost::lexical_cast<std::string>(m_nodePtr->GetId());

  Connect();

  std::string name = Names::FindName(node);
  if (!name.empty()) {
    m_node = name;
  }
}

L3Tracer::L3Tracer(const std::string& node)
  : m_node(node)
{
  Connect();
}

L3Tracer::~L3Tracer(){};

void
L3Tracer::Connect()
{
  Ptr<L3Protocol> l3 = m_nodePtr->GetObject<L3Protocol>();

  l3->TraceConnectWithoutContext("OutInterests", MakeCallback(&L3Tracer::OutInterests, this));
  l3->TraceConnectWithoutContext("InInterests", MakeCallback(&L3Tracer::InInterests, this));
  l3->TraceConnectWithoutContext("OutData", MakeCallback(&L3Tracer::OutData, this));
  l3->TraceConnectWithoutContext("InData", MakeCallback(&L3Tracer::InData, this));

  // satisfied/timed out PIs
  l3->TraceConnectWithoutContext("SatisfiedInterests",
                                 MakeCallback(&L3Tracer::SatisfiedInterests, this));

  l3->TraceConnectWithoutContext("TimedOutInterests",
                                 MakeCallback(&L3Tracer::TimedOutInterests, this));
}

} // namespace ndn
} // namespace ns3
