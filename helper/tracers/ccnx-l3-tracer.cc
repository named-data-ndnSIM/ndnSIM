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

#include "ccnx-l3-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"
#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"

#include <boost/lexical_cast.hpp>

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

using namespace std;

namespace ns3 {
    
CcnxL3Tracer::CcnxL3Tracer (Ptr<Node> node)
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

CcnxL3Tracer::CcnxL3Tracer (const std::string &node)
: m_node (node)
{
  Connect ();
}

void
CcnxL3Tracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/ForwardingStrategy/OutInterests",
                   MakeCallback (&CcnxL3Tracer::OutInterests, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/InInterests",
                   MakeCallback (&CcnxL3Tracer::InInterests, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/DropInterests",
                   MakeCallback (&CcnxL3Tracer::DropInterests, this));

  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/OutNacks",
                   MakeCallback (&CcnxL3Tracer::OutNacks, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/InNacks",
                   MakeCallback (&CcnxL3Tracer::InNacks, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/DropNacks",
                   MakeCallback (&CcnxL3Tracer::DropNacks, this));

  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/OutData",
                   MakeCallback (&CcnxL3Tracer::OutData, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/InData",
                   MakeCallback (&CcnxL3Tracer::InData, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::CcnxL3Protocol/DropData",
                   MakeCallback (&CcnxL3Tracer::DropData, this));
}

} // namespace ns3
