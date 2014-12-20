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

#include "ndn-l3-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"
#include "ns3/ndn-forwarding-strategy.h"

#include <boost/lexical_cast.hpp>

#include "ns3/ndn-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"
#include "ns3/ndn-pit-entry.h"

using namespace std;

namespace ns3 {
namespace ndn {

L3Tracer::L3Tracer (Ptr<Node> node)
: m_nodePtr (node)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

L3Tracer::L3Tracer (const std::string &node)
: m_node (node)
{
  Connect ();
}

L3Tracer::~L3Tracer ()
{
};


void
L3Tracer::Connect ()
{
  Ptr<ForwardingStrategy> fw = m_nodePtr->GetObject<ForwardingStrategy> ();
  
  fw->TraceConnectWithoutContext ("OutInterests",  MakeCallback (&L3Tracer::OutInterests, this));
  fw->TraceConnectWithoutContext ("InInterests",   MakeCallback (&L3Tracer::InInterests, this));
  fw->TraceConnectWithoutContext ("DropInterests", MakeCallback (&L3Tracer::DropInterests, this));
                                                                           
  fw->TraceConnectWithoutContext ("OutData",  MakeCallback (&L3Tracer::OutData, this));
  fw->TraceConnectWithoutContext ("InData",   MakeCallback (&L3Tracer::InData, this));
  fw->TraceConnectWithoutContext ("DropData", MakeCallback (&L3Tracer::DropData, this));

  // only for some strategies
  fw->TraceConnectWithoutContext ("OutNacks",  MakeCallback (&L3Tracer::OutNacks, this));
  fw->TraceConnectWithoutContext ("InNacks",   MakeCallback (&L3Tracer::InNacks, this));
  fw->TraceConnectWithoutContext ("DropNacks", MakeCallback (&L3Tracer::DropNacks, this));
  
  // satisfied/timed out PIs
  fw->TraceConnectWithoutContext ("SatisfiedInterests", MakeCallback (&L3Tracer::SatisfiedInterests, this));
  fw->TraceConnectWithoutContext ("TimedOutInterests", MakeCallback (&L3Tracer::TimedOutInterests, this));
}

} // namespace ndn
} // namespace ns3
