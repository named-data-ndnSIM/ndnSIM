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
    
    .AddAttribute ("Prefix","Name of the Interest",
                   StringValue ("/"),
                   MakeNameAccessor (&ApiApp::m_name),
                   MakeNameChecker ())
    .AddAttribute ("LifeTime", "LifeTime for interest packet",
                   StringValue ("2s"),
                   MakeTimeAccessor (&ApiApp::m_interestLifetime),
                   MakeTimeChecker ())
    ;

  return tid;
}

ApiApp::ApiApp ()
  : m_face (0)
{
}

void
ApiApp::RequestData ()
{
  NS_LOG_FUNCTION (this);
  
  Ptr<Interest> interest = Create<Interest> ();
  interest->SetName (m_name);
  interest->SetInterestLifetime (m_interestLifetime);

  Ptr<Exclude> exclude = Create<Exclude> ();
  exclude->excludeOne (name::Component ("unique"));
  interest->SetExclude (exclude);
  
  m_face->ExpressInterest (interest,
                           MakeCallback (&ApiApp::GotData, this),
                           MakeNullCallback< void, Ptr<const Interest> > ());
}

void
ApiApp::GotData (Ptr<const Interest> origInterest, Ptr<const Data> data)
{
  NS_LOG_FUNCTION (this << origInterest->GetName () << data->GetName ());
  // do nothing else
}
    
void
ApiApp::StartApplication ()
{
  m_face = CreateObject<ApiFace> (GetNode ());
  
  Simulator::Schedule (Seconds (1), &::ns3::ndn::ApiApp::RequestData, this);
  Simulator::Schedule (Seconds (10), &::ns3::ndn::ApiApp::RequestData, this);
}

void
ApiApp::StopApplication ()
{
  NS_LOG_FUNCTION (this);
  m_face->Shutdown ();
  m_face = 0;
}

} // namespace ndn
} // namespace ns3

