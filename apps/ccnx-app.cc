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
#include "ns3/packet.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxApp");

namespace ns3
{    
    
NS_OBJECT_ENSURE_REGISTERED (CcnxApp);
    
TypeId
CcnxConsumer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxApp")
    .SetParent<Application> ()
    .AddConstructor<CcnxApp> ()
    ;
  return tid;
}
    
CcnxApp::CcnxApp ()
  : m_protocolHandler (0)
  , m_active (true)
  , m_face (0)
{
}
    
CcnxApp::~CcnxApp ()
{
  StopApplication ();
}

void
CcnxProducer::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  StopApplication ();
  CcnxApp::DoDispose ();
}

void
CcnxApp::RegisterProtocolHandler (ProtocolHandler handler)
{
  m_protocolHandler = handler;
}
    
void
CcnxApp::OnInterest (const Ptr<const CcnxInterestHeader> &interest)
{
  NS_LOG_FUNCTION (this << interest);
}

void
CcnxApp::OnNack (const Ptr<const CcnxInterestHeader> &interest)
{
  NS_LOG_FUNCTION (this << interest);
}

void
CcnxApp::OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                          const Ptr<const Packet> &payload)
{
  NS_LOG_FUNCTION (this << contentObject << payload);
}

// Application Methods
void 
CcnxApp::StartApplication () // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS ();
  
  NS_ASSERT (m_active != true);
  m_active = true;

  NS_ASSERT_MSG (GetObject<Ccnx> () != 0,
                 "Ccnx stack should be installed on the node " << GetNode ());
  NS_ASSERT_MSG (GetObject<CcnxFib> () != 0);

  // step 1. Create a face
  m_face = Create<CcnxLocalFace> (/*Ptr<CcnxApp> (this)*/this);
    
  // step 2. Add face to the CCNx stack
  GetObject<CcnxFib> ()->Add (m_prefix, m_face, 0);
  GetObject<Ccnx> ()->AddFace (m_face);

  // step 3. Enable face
  m_face->SetUp (true);
}
    
void 
CcnxApp::StopApplication () // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS ();

  if (!m_active) return; //don't assert here, just return
 
  NS_ASSERT (GetObject<Ccnx> () != 0);
  NS_ASSERT (GetObject<CcnxFib> () != 0);

  m_active = false;

  // step 1. Disable face
  m_face->SetUp (false);

  // step 2. Remove face from CCNx stack
  GetObject<Ccnx> ()->RemoveFace (m_face);
  GetObject<CcnxFib> ()->Add (m_prefix, m_face, 0);

  // step 3. Destroy face
  NS_ASSERT_MSG (m_face->GetReferenceCount ()==1,
                 "At this point, nobody else should have referenced this face");
  m_face = 0;
}


}
