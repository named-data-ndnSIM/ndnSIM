/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: 
//

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


NS_LOG_COMPONENT_DEFINE ("CcnxL3Protocol");

namespace ns3 {

const uint16_t CcnxL3Protocol::PROT_NUMBER = 0x7777;

NS_OBJECT_ENSURE_REGISTERED (CcnxL3Protocol);

TypeId 
CcnxL3Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxL3Protocol")
    .SetParent<Ccnx> ()
    .AddConstructor<CcnxL3Protocol> ()
    .AddTraceSource ("Tx", "Send ccnx packet to outgoing interface.",
                     MakeTraceSourceAccessor (&CcnxL3Protocol::m_txTrace))
    .AddTraceSource ("Rx", "Receive ccnx packet from incoming interface.",
                     MakeTraceSourceAccessor (&CcnxL3Protocol::m_rxTrace))
    .AddTraceSource ("Drop", "Drop ccnx packet",
                     MakeTraceSourceAccessor (&CcnxL3Protocol::m_dropTrace))
    .AddAttribute ("InterfaceList", "The set of Ccnx interfaces associated to this Ccnx stack.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&CcnxL3Protocol::m_faces),
                   MakeObjectVectorChecker<CcnxFace> ())

    .AddTraceSource ("SendOutgoing", "A newly-generated packet by this node is about to be queued for transmission",
                     MakeTraceSourceAccessor (&CcnxL3Protocol::m_sendOutgoingTrace))

  ;
  return tid;
}

CcnxL3Protocol::CcnxL3Protocol()
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

uint32_t 
CcnxL3Protocol::AddFace (Ptr<CcnxFace> face)
{
  NS_LOG_FUNCTION (this << *face);

  // Ptr<Node> node = GetObject<Node> (); ///< \todo not sure why this thing should be called...
  face->SetNode (m_node);

  if (face->GetDevice() != 0)
    {
      m_node->RegisterProtocolHandler (MakeCallback (&CcnxL3Protocol::ReceiveFromLower, this), 
                                       CcnxL3Protocol::PROT_NUMBER, face->GetDevice(), true/*promiscuous mode*/);
    }

  uint32_t index = m_faces.size ();
  m_faces.push_back (face);
  return index;
}


Ptr<CcnxFace>
CcnxL3Protocol::GetFace (uint32_t index) const
{
  if (index < m_faces.size ())
    {
      return m_faces[index];
    }
  return 0;
}

uint32_t 
CcnxL3Protocol::GetNFaces (void) const
{
  return m_faces.size ();
}

Ptr<CcnxFace>
CcnxL3Protocol::GetFaceForDevice (Ptr<const NetDevice> device) const
{
  for (CcnxFaceList::const_iterator i = m_faces.begin (); 
       i != m_faces.end (); 
       i++)
    {
      if ((*i)->GetDevice () == device)
        {
          return *i;
        }
    }

  NS_ASSERT_MSG (false, "Should never get to this place" );
  return 0;
}

// Callback from lower layer
void 
CcnxL3Protocol::ReceiveFromLower ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                          const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol <<  from);

  NS_LOG_LOGIC ("Packet from " << from << " received on node " <<  m_node->GetId ());

  Ptr<CcnxFace> ccnxFace = GetFaceForDevice (device);

  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      Ptr<Header> header = CcnxHeaderHelper::CreateCorrectCcnxHeader (p);
      ReceiveAndProcess (ccnxFace, header, packet);  // header should serve as overloaded method selector... not sure whether it works with this "smart" pointers...
    }
  catch (CcnxUnknownHeaderException)
    {
      NS_ASSERT_MSG (false, "Unknown CCNx header. Should not happen");
    }
}

// Processing Interests
void CcnxL3Protocol::ReceiveAndProcess (Ptr<CcnxFace> incomingFace, Ptr<CcnxInterestHeader> header, Ptr<Packet> packet)
{
  if (incomingFace->IsUp ())
    {
      NS_LOG_LOGIC ("Dropping received packet -- interface is down");
      m_dropTrace (packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), incomingFace);
      return;
    }

  NS_LOG_LOGIC ("Receiving interest from " << incomingFace);
  // m_rxTrace (packet, m_node->GetObject<Ccnx> (), incomingFace);

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
void CcnxL3Protocol::ReceiveAndProcess (Ptr<CcnxFace> incomingFace, Ptr<CcnxContentObjectHeader> header, Ptr<Packet> packet)
{
  if (incomingFace->IsUp ())
    {
      NS_LOG_LOGIC ("Dropping received packet -- interface is down");
      m_dropTrace (packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), incomingFace);
      return;
    }

  NS_LOG_LOGIC ("Receiving contentObject from " << incomingFace);
}

// fake method
void
CcnxL3Protocol::ReceiveAndProcess (Ptr<CcnxFace> face, Ptr<Header> header, Ptr<Packet> p)
{
  NS_ASSERT_MSG (false, "This function should never be called");
}


void
CcnxL3Protocol::Send (Ptr<Packet> packet, Ptr<CcnxRoute> route)
{
  NS_LOG_FUNCTION (this << "packet: " << packet << ", route: "<< route);
  
  if (route == 0)
    {
      NS_LOG_WARN ("No route to host.  Drop.");
      m_dropTrace (packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), 0);
      return;
    }
  Ptr<CcnxFace> outFace = route->GetOutputFace ();

  if (outFace->IsUp ())
    {
      NS_LOG_LOGIC ("Sending via face " << *outFace);
      m_txTrace (packet, m_node->GetObject<Ccnx> (), outFace);
      outFace->Send (packet);
    }
  else
    {
      NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << *outFace);
      m_dropTrace (packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), outFace);
    }
}


void 
CcnxL3Protocol::SetMetric (uint32_t i, uint16_t metric)
{
  NS_LOG_FUNCTION (this << i << metric);
  Ptr<CcnxFace> face = GetFace (i);
  face->SetMetric (metric);
}

uint16_t
CcnxL3Protocol::GetMetric (uint32_t i) const
{
  Ptr<const CcnxFace> face = GetFace (i);
  return face->GetMetric ();
}

uint16_t 
CcnxL3Protocol::GetMtu (uint32_t i) const
{
  Ptr<CcnxFace> face = GetFace (i);
  return face->GetDevice ()->GetMtu ();
}

bool 
CcnxL3Protocol::IsUp (uint32_t i) const
{
  Ptr<CcnxFace> interface = GetFace (i);
  return interface->IsUp ();
}

void 
CcnxL3Protocol::SetUp (uint32_t i)
{
  NS_LOG_FUNCTION (this << i);
  Ptr<CcnxFace> interface = GetFace (i);
  interface->SetUp ();

  if (m_forwardingStrategy != 0)
    {
      m_forwardingStrategy->NotifyInterfaceUp (i);
    }
}

void 
CcnxL3Protocol::SetDown (uint32_t ifaceIndex)
{
  NS_LOG_FUNCTION (this << ifaceIndex);
  Ptr<CcnxFace> interface = GetFace (ifaceIndex);
  interface->SetDown ();

  if (m_forwardingStrategy != 0)
    {
      m_forwardingStrategy->NotifyInterfaceDown (ifaceIndex);
    }
}

void
CcnxL3Protocol::RouteInputError (Ptr<Packet> p)//, Socket::SocketErrno sockErrno)
{
  // NS_LOG_FUNCTION (this << p << ipHeader << sockErrno);
  // NS_LOG_LOGIC ("Route input failure-- dropping packet to " << ipHeader << " with errno " << sockErrno); 
  m_dropTrace (p, DROP_ROUTE_ERROR, m_node->GetObject<Ccnx> (), 0);
}

} //namespace ns3
