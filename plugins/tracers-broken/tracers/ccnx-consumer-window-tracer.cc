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

#include "ccnx-consumer-window-tracer.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/names.h"
#include "ns3/simulator.h"

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

namespace ns3 {

void
CcnxConsumerWindowTracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/ApplicationList/"+m_appId+"/$ns3::CcnxConsumerWindow/WindowTrace",
                   MakeCallback (&WindowTracer::OnWindowChange, this));
}

void
TcpCongestionWindowTracer::Connect ()
{
  Config::Connect ("/NodeList/"+m_node+"/$ns3::TcpL4Protocol/SocketList/*/CongestionWindow",
                   MakeCallback (&WindowTracer::OnWindowChange, this));
}


WindowTracer::WindowTracer (std::ostream &os, Ptr<Node> node, const std::string &appId)
  : m_appId (appId)
  , m_nodePtr (node)
  , m_os (os)
{
  m_node = boost::lexical_cast<string> (m_nodePtr->GetId ());

  string name = Names::FindName (node);
  if (!name.empty ())
    {
      m_nodeName = name;
    }
  else
    m_nodeName = m_node;
}


void
WindowTracer::PrintHeader (std::ostream &os)
{
  os << "Time\t"
     << "Node\t"
     << "AppId\t"
     << "Window";
}

void
WindowTracer::OnWindowChange (std::string context,
                              uint32_t oldValue, uint32_t newValue)
{
  m_os                                                             
    << Simulator::Now ().ToDouble (Time::S) << "\t"                   
    << m_nodeName << "\t"                                                 
    << m_appId << "\t"
    << newValue << endl;
}

} // namespace ns3
