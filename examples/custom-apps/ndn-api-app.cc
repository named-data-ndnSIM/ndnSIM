/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-api-app.h"

NS_LOG_COMPONENT_DEFINE ("ndn.ApiApp");

namespace ns3 {
namespace ndn {

// Necessary if you are planning to use ndn::AppHelper
NS_OBJECT_ENSURE_REGISTERED (ApiApp);

TypeId
ApiApp::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::ApiApp")
    .SetParent<Application> ()
    .AddConstructor<ApiApp> ()
    ;

  return tid;
}

ApiApp::ApiApp ()
{
  // m_handler = CreateObject<Handler> ();
}

void
ApiApp::RequestData ()
{
  // m_handler->sendInterest ("/test/prefix", boost::bind (&ApiApp::OnData
}

void
ApiApp::StartApplication ()
{
  // m_handler->SetNode (GetNode ());
  // m_handler->StartApplication ();
  
  // Simulator::Schedule (Seconds (1), &::ns3::ndn::ApiApp::RequestData, this);
}

void
ApiApp::StopApplication ()
{
  // m_handler->StopApplication ();
}

} // namespace ndn
} // namespace ns3

