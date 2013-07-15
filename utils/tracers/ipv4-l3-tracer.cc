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

#include "ipv4-l3-tracer.h"
#include "ns3/node.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"
#include "ns3/ipv4-l3-protocol.h"

#include <boost/lexical_cast.hpp>

using namespace std;

namespace ns3 {

Ipv4L3Tracer::Ipv4L3Tracer (Ptr<Node> node)
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

void
Ipv4L3Tracer::Connect ()
{
  Ptr<Ipv4L3Protocol> ip = m_nodePtr->GetObject<Ipv4L3Protocol> ();
  ip->TraceConnectWithoutContext ("Tx",   MakeCallback (&Ipv4L3Tracer::Tx, this));
  ip->TraceConnectWithoutContext ("Rx",   MakeCallback (&Ipv4L3Tracer::Rx, this));
  ip->TraceConnectWithoutContext ("Drop", MakeCallback (&Ipv4L3Tracer::Drop, this));
}

} // namespace ns3
