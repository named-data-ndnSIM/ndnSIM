/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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

#include "ccnx-l3-protocol.h"

#include "ns3/packet.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/boolean.h"

#include "ns3/ccnx-header-helper.h"

#include "ccnx-face.h"
#include "ccnx-route.h"
#include "ccnx-forwarding-strategy.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxL3Protocol");

namespace ns3 {

const uint16_t CcnxL3Protocol::ETHERNET_FRAME_TYPE = 0x7777;

NS_OBJECT_ENSURE_REGISTERED (CcnxL3Protocol);

TypeId 
CcnxL3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxL3Protocol")
    .SetParent<Ccnx> ()
    .SetGroupName ("Ccnx")
    .AddConstructor<CcnxL3Protocol> ()
    // .AddTraceSource ("Tx", "Send ccnx packet to outgoing interface.",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_txTrace))
    // .AddTraceSource ("Rx", "Receive ccnx packet from incoming interface.",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_rxTrace))
    // .AddTraceSource ("Drop", "Drop ccnx packet",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_dropTrace))
    .AddAttribute ("InterfaceList", "The set of Ccnx interfaces associated to this Ccnx stack.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&CcnxL3Protocol::m_faces),
                   MakeObjectVectorChecker<CcnxFace> ())

    // .AddTraceSource ("SendOutgoing", "A newly-generated packet by this node is about to be queued for transmission",
    //                  MakeTraceSourceAccessor (&CcnxL3Protocol::m_sendOutgoingTrace))

  ;
  return tid;
}

CcnxL3Protocol::CcnxL3Protocol()
: m_faceCounter (0)
{
  NS_LOG_FUNCTION (this);
}

CcnxL3Protocol::~CcnxL3Protocol ()
{
  NS_LOG_FUNCTION (this);
}

void
CcnxL3Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ccnx stack
 */
void
CcnxL3Protocol::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node>();
      // verify that it's a valid node and that
      // the node has not been set before
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}

void 
CcnxL3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  for (CcnxFaceList::iterator i = m_faces.begin (); i != m_faces.end (); ++i)
    {
      *i = 0;
    }
  m_faces.clear ();
  m_node = 0;
  // m_forwardingStrategy = 0;
  Object::DoDispose ();
}

void
CcnxL3Protocol::SetForwardingStrategy (Ptr<CcnxForwardingStrategy> forwardingStrategy)
{
  NS_LOG_FUNCTION (this);
  m_forwardingStrategy = forwardingStrategy;
  m_forwardingStrategy->SetCcnx (this);
}

Ptr<CcnxForwardingStrategy>
CcnxL3Protocol::GetForwardingStrategy (void) const
{
  return m_forwardingStrategy;
}

uint32_t 
CcnxL3Protocol::AddFace (const Ptr<CcnxFace> &face)
{
  NS_LOG_FUNCTION (this << &face);

  face->SetNode (m_node);
  face->SetId (m_faceCounter); // sets a unique ID of the face. This ID serves only informational purposes

  // ask face to register in lower-layer stack
  face->RegisterProtocolHandler (MakeCallback (&CcnxL3Protocol::Receive, this));

  m_faces.push_back (face);
  m_faceCounter ++;
  return face->GetId ();
}

Ptr<CcnxFace>
CcnxL3Protocol::GetFace (uint32_t index) const
{
  BOOST_FOREACH (const Ptr<CcnxFace> &face, m_faces) // this function is not supposed to be called often, so linear search is fine
    {
      if (face->GetId () == index)
        return face;
    }
  return 0;
}

uint32_t 
CcnxL3Protocol::GetNFaces (void) const
{
  return m_faces.size ();
}

// Callback from lower layer
void 
CcnxL3Protocol::Receive (const Ptr<CcnxFace> &face, const Ptr<const Packet> &p)
{
  if (face->IsUp ())
    {
      NS_LOG_LOGIC ("Dropping received packet -- interface is down");
      m_dropTrace (p, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> ()/*this*/, face);
      return;
    }
  NS_LOG_LOGIC ("Packet from face " << &face << " received on node " <<  m_node->GetId ());

  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      CcnxHeaderHelper::Type type = CcnxHeaderHelper::CreateCorrectCcnxHeader (p);
      switch (type)
        {
        case CcnxHeaderHelper::INTEREST:
          {
            Ptr<CcnxInterestHeader> header = Create<CcnxInterestHeader> ();
            OnInterest (face, header, packet);  
            break;
          }
        case CcnxHeaderHelper::CONTENT_OBJECT:
          {
            Ptr<CcnxContentObjectHeader> header = Create<CcnxContentObjectHeader> ();
            OnData (face, header, packet);  
            break;
          }
        }
      
      // exception will be thrown if packet is not recognized
    }
  catch (CcnxUnknownHeaderException)
    {
      NS_ASSERT_MSG (false, "Unknown CCNx header. Should not happen");
    }
}

// Processing Interests
void CcnxL3Protocol::OnInterest (const Ptr<CcnxFace> &incomingFace,
                                 Ptr<CcnxInterestHeader> &header,
                                 Ptr<Packet> &packet)
{
  NS_LOG_LOGIC ("Receiving interest from " << &incomingFace);
  m_receivedInterestsTrace (packet, m_node->GetObject<Ccnx> ()/*this*/, incomingFace);
  
  // dangerous place. Trying to deserialize header
  packet->RemoveHeader (*header);

  NS_ASSERT_MSG (packet->GetSize () == 0, "Payload of Interests should be zero");

  /// \todo Processing of Interest packets
  
  // NS_ASSERT_MSG (m_forwardingStrategy != 0, "Need a forwarding protocol object to process packets");
  // if (!m_forwardingStrategy->RouteInput (packet, incomingFace,
  //                                     MakeCallback (&CcnxL3Protocol::Send, this),
  //                                     MakeCallback (&CcnxL3Protocol::RouteInputError, this)
  //                                     ))
  //   {
  //     NS_LOG_WARN ("No route found for forwarding packet.  Drop.");
  //     m_dropTrace (packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), incomingFace);
  //   }
}

// Processing ContentObjects
void CcnxL3Protocol::OnData (const Ptr<CcnxFace> &incomingFace,
                             Ptr<CcnxContentObjectHeader> &header,
                             Ptr<Packet> &packet)
{
  static CcnxContentObjectTail contentObjectTrailer; //there is no data in this object
  
  NS_LOG_LOGIC ("Receiving contentObject from " << &incomingFace);
  m_receivedDataTrace (packet, m_node->GetObject<Ccnx> ()/*this*/, incomingFace);

  // dangerous place. Trying to deserialize header
  packet->RemoveHeader (*header);
  packet->RemoveTrailer (contentObjectTrailer);
  
  /// \todo Processing of ContentObject packets
}


void
CcnxL3Protocol::Send (const Ptr<CcnxFace> &face, const Ptr<Packet> &packet)
{
  NS_LOG_FUNCTION (this << "packet: " << &packet << ", face: "<< &face); //

  NS_ASSERT_MSG (face != 0, "Face should never be NULL");

  if (face->IsUp ())
    {
      NS_LOG_LOGIC ("Sending via face " << &face); //
      // m_txTrace (packet, m_node->GetObject<Ccnx> (), face);
      face->Send (packet);
    }
  else
    {
      NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << &face);
      m_dropTrace (packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), face);
    }
}


} //namespace ns3
