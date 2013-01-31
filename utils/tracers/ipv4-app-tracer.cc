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

#include "ipv4-app-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/names.h"

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

namespace ns3 {

Ipv4AppTracer::Ipv4AppTracer (Ptr<Node> node)
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
Ipv4AppTracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/$ns3::Ipv4L3Protocol/SendOutgoing",
                   MakeCallback (&Ipv4AppTracer::Tx, this));

  Config::Connect ("/NodeList/"+m_node+"/$ns3::Ipv4L3Protocol/LocalDeliver",
                   MakeCallback (&Ipv4AppTracer::Rx, this));
}


} // namespace ns3
