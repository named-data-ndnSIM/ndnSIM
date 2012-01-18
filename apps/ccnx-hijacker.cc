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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-hijacker.h"
#include "ns3/log.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"

#include "../model/ccnx-local-face.h"
#include "ns3/ccnx-fib.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxHijacker");

namespace ns3
{    

NS_OBJECT_ENSURE_REGISTERED (CcnxHijacker);
    
TypeId
CcnxHijacker::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxHijacker")
    .SetParent<CcnxApp> ()
    .AddConstructor<CcnxHijacker> ()
    .AddAttribute ("Prefix","Prefix, which hijacker wants to attack",
                   StringValue ("/"),
                   MakeCcnxNameComponentsAccessor (&CcnxHijacker::m_prefix),
                   MakeCcnxNameComponentsChecker ())
    .AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
                   UintegerValue (1024),
                   MakeUintegerAccessor(&CcnxHijacker::m_virtualPayloadSize),
                   MakeUintegerChecker<uint32_t>())
    
    .AddTraceSource ("TransmittedContentObjects", "TransmittedContentObjects",
                    MakeTraceSourceAccessor (&CcnxHijacker::m_transmittedContentObjects))
    ;
        
  return tid;
}
    
CcnxHijacker::CcnxHijacker ()
{
  // NS_LOG_FUNCTION_NOARGS ();
}

// inherited from Application base class.
void
CcnxHijacker::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (GetNode ()->GetObject<CcnxFib> () != 0);

  CcnxApp::StartApplication ();

  Ptr<CcnxFib> fib = GetNode ()->GetObject<CcnxFib> ();
  CcnxFibEntryContainer::type::iterator fibEntry = fib->Add (m_prefix, m_face, 0);

  // make face green, so it will be used primarily
  fib->m_fib.modify (fibEntry,
                     ll::bind (&CcnxFibEntry::UpdateStatus,
                               ll::_1, m_face, CcnxFibFaceMetric::NDN_FIB_GREEN));
}

void
CcnxHijacker::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (GetNode ()->GetObject<CcnxFib> () != 0);

  CcnxApp::StopApplication ();
}


void
CcnxHijacker::OnInterest (const Ptr<const CcnxInterestHeader> &interest)
{
  CcnxApp::OnInterest (interest); // tracing inside

  NS_LOG_FUNCTION (this << interest);

  /*
  if (!m_active) return;
    
  static CcnxContentObjectTail tail;
  Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
  header->SetName (Create<CcnxNameComponents> (interest->GetName ()));

  NS_LOG_INFO ("Respodning with ContentObject:\n" << boost::cref(*header));
  
  Ptr<Packet> packet = Create<Packet> (m_virtualPayloadSize);

  m_transmittedContentObjects (header, packet, this, m_face);
  
  packet->AddHeader (*header);
  packet->AddTrailer (tail);

  m_protocolHandler (packet);*/
}

} // namespace ns3
