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
// Author: George F. Riley<riley@ece.gatech.edu>
//

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/ccnx-address.h"
#include "ns3/ccnx-route.h"
#include "ns3/node.h"
#include "ns3/socket.h"
#include "ns3/net-device.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/ccnx-header.h"
#include "ns3/boolean.h"
//#include "ns3/ccnx-routing-table-entry.h"

#include "arp-l3-protocol.h"
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
  : m_identification (0)
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
  // Add a LoopbackNetDevice if needed, and an CcnxInterface on top of it
  SetupLoopback ();
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

// void 
// CcnxL3Protocol::SetRoutingProtocol (Ptr<CcnxRoutingProtocol> routingProtocol)
// {
//   NS_LOG_FUNCTION (this);
//   m_routingProtocol = routingProtocol;
//   m_routingProtocol->SetCcnx (this);
// }


// Ptr<CcnxRoutingProtocol> 
// CcnxL3Protocol::GetRoutingProtocol (void) const
// {
//   return m_routingProtocol;
// }

void 
CcnxL3Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  for (CcnxInterfaceList::iterator i = m_interfaces.begin (); i != m_interfaces.end (); ++i)
    {
      *i = 0;
    }
  m_interfaces.clear ();
  m_node = 0;
  // m_routingProtocol = 0;
  Object::DoDispose ();
}

uint32_t 
CcnxL3Protocol::AddInterface (Ptr<NetDevice> device)
{
  NS_LOG_FUNCTION (this << &device);

  Ptr<Node> node = GetObject<Node> ();
  node->RegisterProtocolHandler (MakeCallback (&CcnxL3Protocol::Receive, this), 
                                 CcnxL3Protocol::PROT_NUMBER, device, true);

  // ccnx doesn't need arp protocol to run. Everything is broadcast!
  // node->RegisterProtocolHandler (MakeCallback (&ArpL3Protocol::Receive, PeekPointer (GetObject<ArpL3Protocol> ())),
  //                                ArpL3Protocol::PROT_NUMBER, device, true);

  Ptr<CcnxInterface> interface = CreateObject<CcnxInterface> ();
  interface->SetNode (m_node);
  interface->SetDevice (device);
  // interface->SetForwarding (m_ipForward);
  return AddCcnxInterface (interface);
}

uint32_t 
CcnxL3Protocol::AddCcnxInterface (Ptr<CcnxInterface>interface)
{
  NS_LOG_FUNCTION (this << interface);
  uint32_t index = m_interfaces.size ();
  m_interfaces.push_back (interface);
  return index;
}

Ptr<CcnxInterface>
CcnxL3Protocol::GetInterface (uint32_t index) const
{
  if (index < m_interfaces.size ())
    {
      return m_interfaces[index];
    }
  return 0;
}

uint32_t 
CcnxL3Protocol::GetNInterfaces (void) const
{
  return m_interfaces.size ();
}

int32_t 
CcnxL3Protocol::GetInterfaceForDevice (
  Ptr<const NetDevice> device) const
{
  int32_t interface = 0;
  for (CcnxInterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      if ((*i)->GetDevice () == device)
        {
          return interface;
        }
    }

  return -1;
}

void 
CcnxL3Protocol::Receive ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address &from,
                          const Address &to, NetDevice::PacketType packetType)
{
  NS_LOG_FUNCTION (this << &device << p << protocol <<  from);

  NS_LOG_LOGIC ("Packet from " << from << " received on node " << 
                m_node->GetId ());

  uint32_t interface = 0;
  Ptr<Packet> packet = p->Copy ();

  Ptr<CcnxInterface> ccnxInterface;
  for (CcnxInterfaceList::const_iterator i = m_interfaces.begin (); 
       i != m_interfaces.end (); 
       i++, interface++)
    {
      ccnxInterface = *i;
      if (ccnxInterface->GetDevice () == device)
        {
          if (ccnxInterface->IsUp ())
            {
              m_rxTrace (packet, m_node->GetObject<Ccnx> (), interface);
              break;
            }
          else
            {
              NS_LOG_LOGIC ("Dropping received packet -- interface is down");
              CcnxHeader ipHeader;
              packet->RemoveHeader (ipHeader);
              m_dropTrace (ipHeader, packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), interface);
              return;
            }
        }
    }

  CcnxHeader ipHeader;
  if (Node::ChecksumEnabled ())
    {
      ipHeader.EnableChecksum ();
    }
  packet->RemoveHeader (ipHeader);

  // Trim any residual frame padding from underlying devices
  if (ipHeader.GetPayloadSize () < packet->GetSize ())
    {
      packet->RemoveAtEnd (packet->GetSize () - ipHeader.GetPayloadSize ());
    }

  if (!ipHeader.IsChecksumOk ()) 
    {
      NS_LOG_LOGIC ("Dropping received packet -- checksum not ok");
      m_dropTrace (ipHeader, packet, DROP_BAD_CHECKSUM, m_node->GetObject<Ccnx> (), interface);
      return;
    }

  for (SocketList::iterator i = m_sockets.begin (); i != m_sockets.end (); ++i)
    {
      NS_LOG_LOGIC ("Forwarding to raw socket"); 
      Ptr<CcnxRawSocketImpl> socket = *i;
      socket->ForwardUp (packet, ipHeader, ccnxInterface);
    }

  NS_ASSERT_MSG (m_routingProtocol != 0, "Need a routing protocol object to process packets");
  if (!m_routingProtocol->RouteInput (packet, ipHeader, device,
                                      MakeCallback (&CcnxL3Protocol::IpForward, this),
                                      MakeCallback (&CcnxL3Protocol::IpMulticastForward, this),
                                      MakeCallback (&CcnxL3Protocol::LocalDeliver, this),
                                      MakeCallback (&CcnxL3Protocol::RouteInputError, this)
                                      ))
    {
      NS_LOG_WARN ("No route found for forwarding packet.  Drop.");
      m_dropTrace (ipHeader, packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), interface);
    }


}

// void 
// CcnxL3Protocol::SendWithHeader (Ptr<Packet> packet, 
//                                 CcnxHeader ipHeader,
//                                 Ptr<CcnxRoute> route)
// {
//   NS_LOG_FUNCTION (this << packet << ipHeader << route);
//   SendRealOut (route, packet, ipHeader);
// }

void 
CcnxL3Protocol::Send (Ptr<Packet> packet, 
                      Ptr<CcnxRoute> route)
{
  NS_LOG_FUNCTION (this << packet << route);

  // CcnxHeader ipHeader;
  // bool mayFragment = true;
  // uint8_t ttl = m_defaultTtl;
  // SocketIpTtlTag tag;
  // bool found = packet->RemovePacketTag (tag);
  // if (found)
  //   {
  //     ttl = tag.GetTtl ();
  //   }

  // // Handle a few cases:
  // // 1) packet is destined to limited broadcast address
  // // 2) packet is destined to a subnet-directed broadcast address
  // // 3) packet is not broadcast, and is passed in with a route entry
  // // 4) packet is not broadcast, and is passed in with a route entry but route->GetGateway is not set (e.g., on-demand)
  // // 5) packet is not broadcast, and route is NULL (e.g., a raw socket call, or ICMP)

  // // 1) packet is destined to limited broadcast address or link-local multicast address
  // if (destination.IsBroadcast () || destination.IsLocalMulticast ())
  //   {
  //     NS_LOG_LOGIC ("CcnxL3Protocol::Send case 1:  limited broadcast");
  //     ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
  //     uint32_t ifaceIndex = 0;
  //     for (CcnxInterfaceList::iterator ifaceIter = m_interfaces.begin ();
  //          ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
  //       {
  //         Ptr<CcnxInterface> outInterface = *ifaceIter;
  //         Ptr<Packet> packetCopy = packet->Copy ();

  //         NS_ASSERT (packetCopy->GetSize () <= outInterface->GetDevice ()->GetMtu ());

  //         m_sendOutgoingTrace (ipHeader, packetCopy, ifaceIndex);
  //         packetCopy->AddHeader (ipHeader);
  //         m_txTrace (packetCopy, m_node->GetObject<Ccnx> (), ifaceIndex);
  //         outInterface->Send (packetCopy, destination);
  //       }
  //     return;
  //   }

  // // 2) check: packet is destined to a subnet-directed broadcast address
  // uint32_t ifaceIndex = 0;
  // for (CcnxInterfaceList::iterator ifaceIter = m_interfaces.begin ();
  //      ifaceIter != m_interfaces.end (); ifaceIter++, ifaceIndex++)
  //   {
  //     Ptr<CcnxInterface> outInterface = *ifaceIter;
  //     for (uint32_t j = 0; j < GetNAddresses (ifaceIndex); j++)
  //       {
  //         CcnxInterfaceAddress ifAddr = GetAddress (ifaceIndex, j);
  //         NS_LOG_LOGIC ("Testing address " << ifAddr.GetLocal () << " with mask " << ifAddr.GetMask ());
  //         if (destination.IsSubnetDirectedBroadcast (ifAddr.GetMask ()) && 
  //             destination.CombineMask (ifAddr.GetMask ()) == ifAddr.GetLocal ().CombineMask (ifAddr.GetMask ())   )
  //           {
  //             NS_LOG_LOGIC ("CcnxL3Protocol::Send case 2:  subnet directed bcast to " << ifAddr.GetLocal ());
  //             ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
  //             Ptr<Packet> packetCopy = packet->Copy ();
  //             m_sendOutgoingTrace (ipHeader, packetCopy, ifaceIndex);
  //             packetCopy->AddHeader (ipHeader);
  //             m_txTrace (packetCopy, m_node->GetObject<Ccnx> (), ifaceIndex);
  //             outInterface->Send (packetCopy, destination);
  //             return;
  //           }
  //       }
  //   }

  // // 3) packet is not broadcast, and is passed in with a route entry
  // //    with a valid CcnxAddress as the gateway
  // if (route && route->GetGateway () != CcnxAddress ())
  //   {
  //     NS_LOG_LOGIC ("CcnxL3Protocol::Send case 3:  passed in with route");
  //     ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
  //     int32_t interface = GetInterfaceForDevice (route->GetOutputDevice ());
  //     m_sendOutgoingTrace (ipHeader, packet, interface);
  //     SendRealOut (route, packet->Copy (), ipHeader);
  //     return; 
  //   } 
  // // 4) packet is not broadcast, and is passed in with a route entry but route->GetGateway is not set (e.g., on-demand)
  // if (route && route->GetGateway () == CcnxAddress ())
  //   {
  //     // This could arise because the synchronous RouteOutput() call
  //     // returned to the transport protocol with a source address but
  //     // there was no next hop available yet (since a route may need
  //     // to be queried).
  //     NS_FATAL_ERROR ("CcnxL3Protocol::Send case 4: This case not yet implemented");
  //   }
  // // 5) packet is not broadcast, and route is NULL (e.g., a raw socket call)
  // NS_LOG_LOGIC ("CcnxL3Protocol::Send case 5:  passed in with no route " << destination);
  // Socket::SocketErrno errno_; 
  // Ptr<NetDevice> oif (0); // unused for now
  // ipHeader = BuildHeader (source, destination, protocol, packet->GetSize (), ttl, mayFragment);
  // Ptr<CcnxRoute> newRoute;
  // if (m_routingProtocol != 0)
  //   {
  //     newRoute = m_routingProtocol->RouteOutput (packet, ipHeader, oif, errno_);
  //   }
  // else
  //   {
  //     NS_LOG_ERROR ("CcnxL3Protocol::Send: m_routingProtocol == 0");
  //   }
  // if (newRoute)
  //   {
  //     int32_t interface = GetInterfaceForDevice (newRoute->GetOutputDevice ());
  //     m_sendOutgoingTrace (ipHeader, packet, interface);
  //     SendRealOut (newRoute, packet->Copy (), ipHeader);
  //   }
  // else
  //   {
  //     NS_LOG_WARN ("No route to host.  Drop.");
  //     m_dropTrace (ipHeader, packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), 0);
  //   }
}


void
CcnxL3Protocol::SendRealOut (Ptr<CcnxRoute> route,
                             Ptr<Packet> packet,
                             CcnxHeader const &ipHeader)
{
  NS_LOG_FUNCTION (this << packet << &ipHeader);

  // if (route == 0)
  //   {
  //     NS_LOG_WARN ("No route to host.  Drop.");
  //     m_dropTrace (ipHeader, packet, DROP_NO_ROUTE, m_node->GetObject<Ccnx> (), 0);
  //     return;
  //   }
  // packet->AddHeader (ipHeader);
  // Ptr<NetDevice> outDev = route->GetOutputDevice ();
  // int32_t interface = GetInterfaceForDevice (outDev);
  // NS_ASSERT (interface >= 0);
  // Ptr<CcnxInterface> outInterface = GetInterface (interface);
  // NS_LOG_LOGIC ("Send via NetDevice ifIndex " << outDev->GetIfIndex () << " ccnxInterfaceIndex " << interface);

  // NS_ASSERT_MSG (packet->GetSize () <= outInterface->GetDevice ()->GetMtu (), 
  //                "Packet size " << packet->GetSize () << " exceeds device MTU "
  //                               << outInterface->GetDevice ()->GetMtu ()
  //                               << " for Ccnx; fragmentation not supported");
  // if (!route->GetGateway ().IsEqual (CcnxAddress ("0.0.0.0"))) 
  //   {
  //     if (outInterface->IsUp ())
  //       {
  //         NS_LOG_LOGIC ("Send to gateway " << route->GetGateway ());
  //         m_txTrace (packet, m_node->GetObject<Ccnx> (), interface);
  //         outInterface->Send (packet, route->GetGateway ());
  //       }
  //     else
  //       {
  //         NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << route->GetGateway ());
  //         CcnxHeader ipHeader;
  //         packet->RemoveHeader (ipHeader);
  //         m_dropTrace (ipHeader, packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), interface);
  //       }
  //   } 
  // else 
  //   {
  //     if (outInterface->IsUp ())
  //       {
  //         NS_LOG_LOGIC ("Send to destination " << ipHeader.GetDestination ());
  //         m_txTrace (packet, m_node->GetObject<Ccnx> (), interface);
  //         outInterface->Send (packet, ipHeader.GetDestination ());
  //       }
  //     else
  //       {
  //         NS_LOG_LOGIC ("Dropping -- outgoing interface is down: " << ipHeader.GetDestination ());
  //         CcnxHeader ipHeader;
  //         packet->RemoveHeader (ipHeader);
  //         m_dropTrace (ipHeader, packet, DROP_INTERFACE_DOWN, m_node->GetObject<Ccnx> (), interface);
  //       }
  //   }
}


// This function analogous to Linux ip_forward()
// void
// CcnxL3Protocol::IpForward (Ptr<CcnxRoute> rtentry, Ptr<const Packet> p, const CcnxHeader &header)
// {
//   NS_LOG_FUNCTION (this << rtentry << p << header);
//   NS_LOG_LOGIC ("Forwarding logic for node: " << m_node->GetId ());
//   // Forwarding
//   CcnxHeader ipHeader = header;
//   Ptr<Packet> packet = p->Copy ();
//   int32_t interface = GetInterfaceForDevice (rtentry->GetOutputDevice ());
//   ipHeader.SetTtl (ipHeader.GetTtl () - 1);
//   if (ipHeader.GetTtl () == 0)
//     {
//       // Do not reply to ICMP or to multicast/broadcast IP address 
//       if (ipHeader.GetProtocol () != Icmpv4L4Protocol::PROT_NUMBER && 
//           ipHeader.GetDestination ().IsBroadcast () == false &&
//           ipHeader.GetDestination ().IsMulticast () == false)
//         {
//           Ptr<Icmpv4L4Protocol> icmp = GetIcmp ();
//           icmp->SendTimeExceededTtl (ipHeader, packet);
//         }
//       NS_LOG_WARN ("TTL exceeded.  Drop.");
//       m_dropTrace (header, packet, DROP_TTL_EXPIRED, m_node->GetObject<Ccnx> (), interface);
//       return;
//     }
//   m_unicastForwardTrace (ipHeader, packet, interface);
//   SendRealOut (rtentry, packet, ipHeader);
// }

// Will be called from CcnxRoutingProtocol if prefix is locally registered
// Local interest will be satisfied inside CcnxInterface::Send call

void
CcnxL3Protocol::LocalDeliver (Ptr<const Packet> packet, CcnxHeader const&ip, uint32_t iif)
{
  NS_LOG_FUNCTION (this << packet << &ip);
  Ptr<Packet> p = packet->Copy (); // need to pass a non-const packet up

//   m_localDeliverTrace (ip, packet, iif);

//   Ptr<CcnxL4Protocol> protocol = GetProtocol (ip.GetProtocol ());
//   if (protocol != 0)
//     {
//       // we need to make a copy in the unlikely event we hit the
//       // RX_ENDPOINT_UNREACH codepath
//       Ptr<Packet> copy = p->Copy ();
//       enum CcnxL4Protocol::RxStatus status = 
//         protocol->Receive (p, ip, GetInterface (iif));
//       switch (status) {
//         case CcnxL4Protocol::RX_OK:
//         // fall through
//         case CcnxL4Protocol::RX_ENDPOINT_CLOSED:
//         // fall through
//         case CcnxL4Protocol::RX_CSUM_FAILED:
//           break;
//         case CcnxL4Protocol::RX_ENDPOINT_UNREACH:
//           if (ip.GetDestination ().IsBroadcast () == true ||
//               ip.GetDestination ().IsMulticast () == true)
//             {
//               break; // Do not reply to broadcast or multicast
//             }
//           // Another case to suppress ICMP is a subnet-directed broadcast
//           bool subnetDirected = false;
//           for (uint32_t i = 0; i < GetNAddresses (iif); i++)
//             {
//               CcnxInterfaceAddress addr = GetAddress (iif, i);
//               if (addr.GetLocal ().CombineMask (addr.GetMask ()) == ip.GetDestination ().CombineMask (addr.GetMask ()) &&
//                   ip.GetDestination ().IsSubnetDirectedBroadcast (addr.GetMask ()))
//                 {
//                   subnetDirected = true;
//                 }
s//             }
//           if (subnetDirected == false)
//             {
//               GetIcmp ()->SendDestUnreachPort (ip, copy);
//             }
//         }
//     }
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

Ptr<NetDevice>
CcnxL3Protocol::GetNetDevice (uint32_t i)
{
  return GetInterface (i)->GetDevice ();
}

void
CcnxL3Protocol::RouteInputError (Ptr<const Packet> p, const CcnxHeader & ipHeader, Socket::SocketErrno sockErrno)
{
  NS_LOG_FUNCTION (this << p << ipHeader << sockErrno);
  NS_LOG_LOGIC ("Route input failure-- dropping packet to " << ipHeader << " with errno " << sockErrno); 
  m_dropTrace (ipHeader, p, DROP_ROUTE_ERROR, m_node->GetObject<Ccnx> (), 0);
}

} //namespace ns3
