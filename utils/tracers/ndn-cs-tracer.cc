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

#include "ndn-cs-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"

#include <boost/lexical_cast.hpp>

#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"

using namespace std;

namespace ns3 {
namespace ndn {
    
CsTracer::CsTracer (Ptr<Node> node)
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

CsTracer::CsTracer (const std::string &node)
: m_node (node)
{
  Connect ();
}

CsTracer::~CsTracer ()
{
};


void
CsTracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/$ns3::ndn::ContentStore/CacheHits",
                   MakeCallback (&CsTracer::CacheHits, this));
  Config::Connect ("/NodeList/"+m_node+"/$ns3::ndn::ContentStore/CacheMisses",
                   MakeCallback (&CsTracer::CacheMisses, this));
}

} // namespace ndn
} // namespace ns3
