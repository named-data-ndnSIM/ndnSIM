// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
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

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/ccnx-address.h"
#include "ns3/ccnx-route.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/boolean.h"
//#include "ns3/ccnx-routing-table-entry.h"

#include "ccnx-l3-protocol.h"
#include "ccnx-interface.h"

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
                   MakeObjectVectorAccessor (&CcnxL3Protocol::m_interfaces),
                   MakeObjectVectorChecker<CcnxInterface> ())

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
CcnxL3Protocol::SetForwardingProtocol (Ptr<CcnxForwardingProtocol> forwardingProtocol)
{
  NS_LOG_FUNCTION (this);
  m_forwardingProtocol = forwardingProtocol;
  m_forwardingProtocol->SetCcnx (this);
}

Ptr<CcnxForwardingProtocol>
CcnxL3Protocol::GetForwardingProtocol (void) const
{
  return m_forwardingProtocol;
}

void 
CcnxL3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  for (CcnxInterfaceList::iterator i = m_faces.begin (); i != m_faces.end (); ++i)
    {
      *i = 0;
    }
  m_faces.clear ();
  m_node = 0;
  // m_forwardingProtocol = 0;
  Object::DoDispose ();
}

uint32_t 
CcnxL3Protocol::AddFace (Ptr<CcnxFace> face)
{
  NS_LOG_FUNCTION (this << *face);

  // Ptr<Node> node = GetObject<Node> (); ///< \todo not sure why this thing should be called...
  face->setNode (m_node);

  if (face->GetDevice() != 0)
    {
      m_node->RegisterProtocolHandler (MakeCallback (&CcnxL3Protocol::Receive, this), 
                                       CcnxL3Protocol::PROT_NUMBER, face->GetDevice(), true/*promiscuous mode*/);
    }

  uint32_t index = m_faces.size ();
  m_faces.push_back (interface);
  return index;
}


Ptr<CcnxInterface>
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
GetFaceForDevice (Ptr<const NetDevice> device) const
{
  for (CcnxInterfaceList::const_iterator i = m_faces.begin (); 
       i != m_faces.end (); 
       i++, face++)
    {
      if ((*i)->GetDevice () == device)
        {
          return *i;
        }
    }

  NS_ASSERT_MSG (false "Should never get to this place" );
  return 0;
}

// Callback from lower layer
void 
CcnxL3Protocol::Receive ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                          const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol <<  from);

  NS_LOG_LOGIC ("Packet from " << from << " received on node " <<  m_node->GetId ());

  uint32_t interface = 0;
  Ptr<Packet> packet = p->Copy ();

  Ptr<CcnxFace> ccnxFace = GetFaceFromDevice (device);

  Receive (ccnxFace, p);
}

// Callback from higher level
void Receive (Ptr<CcnxFace> incomingFace, Ptr<const Packet> p)
{
  if ( incomingFace->IsUp ())
    {
      NS_LOG_LOGIC ("Dropping received packet -- interface is down");
      m_dropTrace (packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), incomingFace);
      return;
    }
  
  m_rxTrace (packet, m_node->GetObject<Ccnx> (), incomingFace);

  NS_ASSERT_MSG (m_routingProtocol != 0, "Need a routing protocol object to process packets");
  if (!m_routingProtocol->RouteInput (packet, incomingFace,
                                      MakeCallback (&CcnxL3Protocol::Send, this),
                                      MakeCallback (&CcnxL3Protocol::RouteInputError, this)
                                      ))
    {
      NS_LOG_WARN ("No route found for forwarding packet.  Drop.");
      m_dropTrace (packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), incomingFace);
    }
}


void
CcnxL3Protocol::Send (Ptr<Packet> packet, Ptr<CcnxRoute> route)
{
  NS_LOG_FUNCTION (this << "packet: " << packet << ", route: "<< route);

  if (route == 0)
    {
      NS_LOG_WARN ("No route to host.  Drop.");
      m_dropTrace (ipHeader, packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), 0);
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
  Ptr<CcnxInterface> interface = GetInterface (i);
  interface->SetMetric (metric);
}

uint16_t
CcnxL3Protocol::GetMetric (uint32_t i) const
{
  Ptr<CcnxInterface> interface = GetInterface (i);
  return interface->GetMetric ();
}

uint16_t 
CcnxL3Protocol::GetMtu (uint32_t i) const
{
  Ptr<CcnxInterface> interface = GetInterface (i);
  return interface->GetDevice ()->GetMtu ();
}

bool 
CcnxL3Protocol::IsUp (uint32_t i) const
{
  Ptr<CcnxInterface> interface = GetInterface (i);
  return interface->IsUp ();
}

void 
CcnxL3Protocol::SetUp (uint32_t i)
{
  NS_LOG_FUNCTION (this << i);
  Ptr<CcnxInterface> interface = GetInterface (i);
  interface->SetUp ();

  if (m_routingProtocol != 0)
    {
      m_routingProtocol->NotifyInterfaceUp (i);
    }
}

void 
CcnxL3Protocol::SetDown (uint32_t ifaceIndex)
{
  NS_LOG_FUNCTION (this << ifaceIndex);
  Ptr<CcnxInterface> interface = GetInterface (ifaceIndex);
  interface->SetDown ();

  if (m_routingProtocol != 0)
    {
      m_routingProtocol->NotifyInterfaceDown (ifaceIndex);
    }
}

void
CcnxL3Protocol::RouteInputError (Ptr<const Packet> p, const CcnxHeader & ipHeader, Socket::SocketErrno sockErrno)
{
  NS_LOG_FUNCTION (this << p << ipHeader << sockErrno);
  NS_LOG_LOGIC ("Route input failure-- dropping packet to " << ipHeader << " with errno " << sockErrno); 
  m_dropTrace (ipHeader, p, DROP_ROUTE_ERROR, m_node->GetObject<Ccnx> (), 0);
}

} //namespace ns3
