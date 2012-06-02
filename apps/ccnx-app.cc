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

#include "ccnx-app.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/packet.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx.h"
#include "ns3/ccnx-fib.h"
#include "../model/ccnx-local-face.h"

NS_LOG_COMPONENT_DEFINE ("CcnxApp");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxApp);
    
TypeId
CcnxApp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxApp")
    .SetGroupName ("Ccnx")
    .SetParent<Application> ()
    .AddConstructor<CcnxApp> ()

    .AddTraceSource ("ReceivedInterests", "ReceivedInterests",
                    MakeTraceSourceAccessor (&CcnxApp::m_receivedInterests))
    
    .AddTraceSource ("ReceivedNacks", "ReceivedNacks",
                    MakeTraceSourceAccessor (&CcnxApp::m_receivedNacks))
    
    .AddTraceSource ("ReceivedContentObjects", "ReceivedContentObjects",
                    MakeTraceSourceAccessor (&CcnxApp::m_receivedContentObjects))

    .AddTraceSource ("TransmittedInterests", "TransmittedInterests",
                    MakeTraceSourceAccessor (&CcnxApp::m_transmittedInterests))

    .AddTraceSource ("TransmittedContentObjects", "TransmittedContentObjects",
                    MakeTraceSourceAccessor (&CcnxApp::m_transmittedContentObjects))
    ;
  return tid;
}
    
CcnxApp::CcnxApp ()
  : m_protocolHandler (0)
  , m_active (false)
  , m_face (0)
{
}
    
CcnxApp::~CcnxApp ()
{
}

void
CcnxApp::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // Unfortunately, this causes SEGFAULT
  // The best reason I see is that apps are freed after ccnx stack is removed
  // StopApplication ();  
  Application::DoDispose ();
}

void
CcnxApp::RegisterProtocolHandler (ProtocolHandler handler)
{
  m_protocolHandler = handler;
}
    
void
CcnxApp::OnInterest (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << interest);
  m_receivedInterests (interest, this, m_face);
}

void
CcnxApp::OnNack (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << interest);
  m_receivedNacks (interest, this, m_face);
}

void
CcnxApp::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                          Ptr<Packet> payload)
{
  NS_LOG_FUNCTION (this << contentObject << payload);
  m_receivedContentObjects (contentObject, payload, this, m_face);
}

// Application Methods
void 
CcnxApp::StartApplication () // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS ();
  
  NS_ASSERT (m_active != true);
  m_active = true;

  NS_ASSERT_MSG (GetNode ()->GetObject<Ccnx> () != 0,
                 "Ccnx stack should be installed on the node " << GetNode ());

  // step 1. Create a face
  m_face = CreateObject<CcnxLocalFace> (/*Ptr<CcnxApp> (this)*/this);
    
  // step 2. Add face to the CCNx stack
  GetNode ()->GetObject<Ccnx> ()->AddFace (m_face);

  // step 3. Enable face
  m_face->SetUp (true);
}
    
void 
CcnxApp::StopApplication () // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS ();

  if (!m_active) return; //don't assert here, just return
 
  NS_ASSERT (GetNode ()->GetObject<Ccnx> () != 0);

  m_active = false;

  // step 1. Disable face
  m_face->SetUp (false);

  // step 2. Remove face from CCNx stack
  GetNode ()->GetObject<Ccnx> ()->RemoveFace (m_face);
  GetNode ()->GetObject<CcnxFib> ()->RemoveFromAll (m_face);

  // step 3. Destroy face
  NS_ASSERT_MSG (m_face->GetReferenceCount ()==1,
                 "At this point, nobody else should have referenced this face, but we have "
                 << m_face->GetReferenceCount () << " references");
  m_face = 0;
}

}
