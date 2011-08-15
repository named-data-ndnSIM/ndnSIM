/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */
#ifndef CCNX_FORWARDING_PROTOCOL_H
#define CCNX_FORWARDING_PROTOCOL_H

#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/socket.h"
#include "ns3/output-stream-wrapper.h"

#include "ccnx.h"

namespace ns3 {

class CcnxRoute;
class CcnxFace;

/**
 * \ingroup internet 
 * \defgroup ccnxForwarding CcnxForwardingProtocol 
 */
/**
 * \ingroup ccnxForwarding
 * \brief Abstract base class for Ccnx forwarding protocols. 
 * 
 * Defines two virtual functions for packet forwarding and forwarding.  The first, 
 * RouteOutput(), is used for locally originated packets, and the second,
 * RouteInput(), is used for forwarding and/or delivering received packets. 
 * Also defines the signatures of four callbacks used in RouteInput().
 *
 */
class CcnxForwardingProtocol : public Object
{
public:
  static TypeId GetTypeId (void);

  typedef Callback<void, Ptr<Packet>, Ptr<CcnxRoute> > SendCallback;
  typedef Callback<void, Ptr<Packet>/*, Socket::SocketErrno*/ > ErrorCallback;

  /**
   * \brief Query forwarding cache for an existing route, for an outbound packet
  //  *
  //  * This lookup is used by transport protocols.  It does not cause any
  //  * packet to be forwarded, and is synchronous.  Can be used for
  //  * multicast or unicast.  The Linux equivalent is ip_route_output()
  //  *
  //  * \param p packet to be routed.  Note that this method may modify the packet.
  //  *          Callers may also pass in a null pointer. 
  //  * \param header input parameter (used to form key to search for the route)
  //  * \param oif Output interface Netdevice.  May be zero, or may be bound via
  //  *            socket options to a particular output interface.
  //  * \param sockerr Output parameter; socket errno 
  //  *
  //  * \returns a code that indicates what happened in the lookup
  //  */
  // virtual Ptr<CcnxRoute> RouteOutput (Ptr<Packet> p, const CcnxHeader &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr) = 0;

  /**
   * \brief Route an input packet (to be forwarded or locally delivered)
   *
   * This lookup is used in the forwarding process.  The packet is
   * handed over to the CcnxForwardingProtocol, and will get forwarded onward
   * by one of the callbacks.  The Linux equivalent is ip_route_input().
   * There are four valid outcomes, and a matching callbacks to handle each.
   *
   * \param p received packet
   * \param header input parameter used to form a search key for a route
   * \param iface Pointer to ingress face
   * \param ucb Callback for the case in which the packet is to be forwarded
   * \param ecb Callback to call if there is an error in forwarding
   * \returns true if the CcnxForwardingProtocol takes responsibility for 
   *          forwarding or delivering the packet, false otherwise
   */ 
  virtual bool RouteInput  (Ptr<Packet> p, Ptr<CcnxFace> iface, 
                            SendCallback ucb, ErrorCallback ecb) = 0;

  /**
   * \param interface the index of the interface we are being notified about
   *
   * Protocols are expected to implement this method to be notified of the state change of
   * an interface in a node.
   */
  virtual void NotifyInterfaceUp (uint32_t interface) = 0;
  /**
   * \param interface the index of the interface we are being notified about
   *
   * Protocols are expected to implement this method to be notified of the state change of
   * an interface in a node.
   */
  virtual void NotifyInterfaceDown (uint32_t interface) = 0;


  // Should be modified to notify about new prefixes ?
  
  /**
   * \param interface the index of the interface we are being notified about
   * \param address a new address being added to an interface
   *
   * Protocols are expected to implement this method to be notified whenever
   * a new address is added to an interface. Typically used to add a 'network route' on an
   * interface. Can be invoked on an up or down interface.
   */
  // virtual void NotifyAddAddress (uint32_t interface, CcnxInterfaceAddress address) = 0;

  /**
   * \param interface the index of the interface we are being notified about
   * \param address a new address being added to an interface
   *
   * Protocols are expected to implement this method to be notified whenever
   * a new address is removed from an interface. Typically used to remove the 'network route' of an
   * interface. Can be invoked on an up or down interface.
   */
  // virtual void NotifyRemoveAddress (uint32_t interface, CcnxInterfaceAddress address) = 0;

  /**
   * \param ccnx the ccnx object this forwarding protocol is being associated with
   * 
   * Typically, invoked directly or indirectly from ns3::Ccnx::SetForwardingProtocol
   */
  virtual void SetCcnx (Ptr<Ccnx> ccnx);

  virtual void PrintForwardingTable (Ptr<OutputStreamWrapper> stream) const = 0;

protected:
  Ptr<Ccnx> m_ccnx;
};

} //namespace ns3

#endif /* CCNX_FORWARDING_PROTOCOL_H */
