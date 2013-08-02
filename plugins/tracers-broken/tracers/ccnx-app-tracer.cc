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

#include "ccnx-app-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/names.h"

#include "ns3/ccnx-app.h"
#include "ns3/ccnx-face.h"

#include <boost/lexical_cast.hpp>

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

using namespace std;
using namespace boost;

namespace ns3 {
    
CcnxAppTracer::CcnxAppTracer (const std::string &app, Ptr<Node> node, const std::string &appId)
  : m_app (app)
  , m_appId (appId)
  , m_nodePtr (node)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  Connect ();

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_node = name;
    }
}

CcnxAppTracer::CcnxAppTracer (const std::string &app, const std::string &node, const std::string &appId)
  : m_app (app)
  , m_appId (appId)
  , m_node (node)
{
  Connect ();
}

void
CcnxAppTracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/TransmittedInterests",
                   MakeCallback (&CcnxAppTracer::OutInterests, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/ReceivedNacks",
                   MakeCallback (&CcnxAppTracer::InNacks, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/ReceivedInterests",
                   MakeCallback (&CcnxAppTracer::InInterests, this));
  
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/TransmittedDatas",
                   MakeCallback (&CcnxAppTracer::OutData, this));

  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$"+m_app+"/ReceivedDatas",
                   MakeCallback (&CcnxAppTracer::InData, this));
}

} // namespace ns3
