/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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

#include "ndn-app.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"

#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"
#include "ns3/ndn.h"
#include "ns3/ndn-fib.h"
#include "ns3/ndn-app-face.h"
#include "ns3/ndn-forwarding-strategy.h"

NS_LOG_COMPONENT_DEFINE ("NdnApp");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (NdnApp);
    
TypeId
NdnApp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnApp")
    .SetGroupName ("Ndn")
    .SetParent<Application> ()
    .AddConstructor<NdnApp> ()

    .AddTraceSource ("ReceivedInterests", "ReceivedInterests",
                    MakeTraceSourceAccessor (&NdnApp::m_receivedInterests))
    
    .AddTraceSource ("ReceivedNacks", "ReceivedNacks",
                    MakeTraceSourceAccessor (&NdnApp::m_receivedNacks))
    
    .AddTraceSource ("ReceivedContentObjects", "ReceivedContentObjects",
                    MakeTraceSourceAccessor (&NdnApp::m_receivedContentObjects))

    .AddTraceSource ("TransmittedInterests", "TransmittedInterests",
                    MakeTraceSourceAccessor (&NdnApp::m_transmittedInterests))

    .AddTraceSource ("TransmittedContentObjects", "TransmittedContentObjects",
                    MakeTraceSourceAccessor (&NdnApp::m_transmittedContentObjects))
    ;
  return tid;
}
    
NdnApp::NdnApp ()
  : m_protocolHandler (0)
  , m_active (false)
  , m_face (0)
{
}
    
NdnApp::~NdnApp ()
{
}

void
NdnApp::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // Unfortunately, this causes SEGFAULT
  // The best reason I see is that apps are freed after ndn stack is removed
  // StopApplication ();  
  Application::DoDispose ();
}

void
NdnApp::RegisterProtocolHandler (ProtocolHandler handler)
{
  m_protocolHandler = handler;
}
    
void
NdnApp::OnInterest (const Ptr<const NdnInterestHeader> &interest, Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << interest);
  m_receivedInterests (interest, this, m_face);
}

void
NdnApp::OnNack (const Ptr<const NdnInterestHeader> &interest, Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << interest);
  m_receivedNacks (interest, this, m_face);
}

void
NdnApp::OnContentObject (const Ptr<const NdnContentObjectHeader> &contentObject,
                          Ptr<Packet> payload)
{
  NS_LOG_FUNCTION (this << contentObject << payload);
  m_receivedContentObjects (contentObject, payload, this, m_face);
}

// Application Methods
void 
NdnApp::StartApplication () // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS ();
  
  NS_ASSERT (m_active != true);
  m_active = true;

  NS_ASSERT_MSG (GetNode ()->GetObject<Ndn> () != 0,
                 "Ndn stack should be installed on the node " << GetNode ());

  // step 1. Create a face
  m_face = CreateObject<NdnAppFace> (/*Ptr<NdnApp> (this)*/this);
    
  // step 2. Add face to the Ndn stack
  GetNode ()->GetObject<Ndn> ()->AddFace (m_face);

  // step 3. Enable face
  m_face->SetUp (true);
}
    
void 
NdnApp::StopApplication () // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS ();

  if (!m_active) return; //don't assert here, just return
 
  NS_ASSERT (GetNode ()->GetObject<Ndn> () != 0);

  m_active = false;

  // step 1. Disable face
  m_face->SetUp (false);

  // step 2. Remove face from Ndn stack
  GetNode ()->GetObject<Ndn> ()->RemoveFace (m_face);
  GetNode ()->GetObject<NdnFib> ()->RemoveFromAll (m_face);
  GetNode ()->GetObject<NdnForwardingStrategy> ()->RemoveFace (m_face); // notify that face is removed

  // step 3. Destroy face
  NS_ASSERT_MSG (m_face->GetReferenceCount ()==1,
                 "At this point, nobody else should have referenced this face, but we have "
                 << m_face->GetReferenceCount () << " references");
  m_face = 0;
}

}
