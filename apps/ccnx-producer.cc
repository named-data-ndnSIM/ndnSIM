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

#include "ccnx-producer.h"
#include "ns3/log.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include "ns3/ccnx-app-face.h"
#include "ns3/ccnx-fib.h"
#include "../model/ccnx-fib-impl.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxProducer");

namespace ns3
{    

NS_OBJECT_ENSURE_REGISTERED (CcnxProducer);
    
TypeId
CcnxProducer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxProducer")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxApp> ()
    .AddConstructor<CcnxProducer> ()
    .AddAttribute ("Prefix","Prefix, for which producer has the data",
                   StringValue ("/"),
                   MakeCcnxNameComponentsAccessor (&CcnxProducer::m_prefix),
                   MakeCcnxNameComponentsChecker ())
    .AddAttribute ("PayloadSize", "Virtual payload size for Content packets",
                   UintegerValue (1024),
                   MakeUintegerAccessor(&CcnxProducer::m_virtualPayloadSize),
                   MakeUintegerChecker<uint32_t>())

    // optional attributes
    .AddAttribute ("SignatureBits", "SignatureBits field",
                   UintegerValue (0),
                   MakeUintegerAccessor(&CcnxProducer::m_signatureBits),
                   MakeUintegerChecker<uint32_t> ())
    ;
        
  return tid;
}
    
CcnxProducer::CcnxProducer ()
{
  // NS_LOG_FUNCTION_NOARGS ();
}

// inherited from Application base class.
void
CcnxProducer::StartApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (GetNode ()->GetObject<CcnxFib> () != 0);

  CcnxApp::StartApplication ();

  Ptr<CcnxFib> fib = GetNode ()->GetObject<CcnxFib> ();
  CcnxFib::iterator fibEntry = fib->Add (m_prefix, m_face, 0);

  // make face green, so it will be used primarily
  StaticCast<CcnxFibImpl> (fib)->modify (fibEntry,
                                         ll::bind (&CcnxFibEntry::UpdateStatus,
                                                   ll::_1, m_face, CcnxFibFaceMetric::NDN_FIB_GREEN));
}

void
CcnxProducer::StopApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT (GetNode ()->GetObject<CcnxFib> () != 0);

  CcnxApp::StopApplication ();
}


void
CcnxProducer::OnInterest (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> origPacket)
{
  CcnxApp::OnInterest (interest, origPacket); // tracing inside

  NS_LOG_FUNCTION (this << interest);

  if (!m_active) return;
    
  static CcnxContentObjectTail tail;
  Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
  header->SetName (Create<CcnxNameComponents> (interest->GetName ()));
  header->GetSignedInfo ().SetTimestamp (Simulator::Now ());
  header->GetSignature ().SetSignatureBits (m_signatureBits);

  NS_LOG_INFO ("node("<< GetNode()->GetId() <<") respodning with ContentObject:\n" << boost::cref(*header));
  
  Ptr<Packet> packet = Create<Packet> (m_virtualPayloadSize);
  // Ptr<const WeightsPathStretchTag> tag = origPacket->RemovePacketTag<WeightsPathStretchTag> ();
  // if (tag != 0)
  //   {
  //     // std::cout << Simulator::Now () << ", " << m_app->GetInstanceTypeId ().GetName () << "\n";

  //     // echo back WeightsPathStretchTag
  //     packet->AddPacketTag (CreateObject<WeightsPathStretchTag> (*tag));

  //     // \todo
  //     // packet->AddPacketTag should actually accept Ptr<const WeightsPathStretchTag> instead of
  //     // Ptr<WeightsPathStretchTag>.  Echoing will be simplified after change is done
  //   }
  
  packet->AddHeader (*header);
  packet->AddTrailer (tail);

  m_protocolHandler (packet);
  
  m_transmittedContentObjects (header, packet, this, m_face);
}

} // namespace ns3
