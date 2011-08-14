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

#ifndef CCNX_L3_PROTOCOL_H
#define CCNX_L3_PROTOCOL_H

#include <list>
#include <vector>
#include <stdint.h>
#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/ccnx.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class Packet;
class NetDevice;
class CcnxInterface;
// class CcnxAddress;
// class CcnxHeader;
// class CcnxRoutingTableEntry;
class CcnxRoute;
class Node;
class Socket;


/**
 * \brief Implement the Ccnx layer.
 * 
 * This is the actual implementation of IP.  It contains APIs to send and
 * receive packets at the IP layer, as well as APIs for IP routing.
 *
 * This class contains two distinct groups of trace sources.  The
 * trace sources 'Rx' and 'Tx' are called, respectively, immediately
 * after receiving from the NetDevice and immediately before sending
 * to a NetDevice for transmitting a packet.  These are low level
 * trace sources that include the CcnxHeader already serialized into
 * the packet.  In contrast, the Drop, SendOutgoing, UnicastForward,
 * and LocalDeliver trace sources are slightly higher-level and pass
 * around the CcnxHeader as an explicit parameter and not as part of
 * the packet.
 */
class CcnxL3Protocol : public Ccnx
{
public:
  static TypeId GetTypeId (void);
  static const uint16_t PROT_NUMBER;

  CcnxL3Protocol();
  virtual ~CcnxL3Protocol ();

  /**
   * \enum DropReason
   * \brief Reason why a packet has been dropped.
   */
  enum DropReason 
  {
    DROP_TTL_EXPIRED = 1,   /**< Packet TTL has expired */
    DROP_NO_ROUTE,   /**< No route to host */
    DROP_BAD_CHECKSUM,   /**< Bad checksum */
    DROP_INTERFACE_DOWN,   /**< Interface is down so can not send packet */
    DROP_ROUTE_ERROR,   /**< Route error */
  };

  void SetNode (Ptr<Node> node);

  // functions defined in base class Ccnx

  // void SetRoutingProtocol (Ptr<CcnxRoutingProtocol> routingProtocol);
  // Ptr<CcnxRoutingProtocol> GetRoutingProtocol (void) const;

  // Ptr<Socket> CreateRawSocket (void);
  // void DeleteRawSocket (Ptr<Socket> socket);

  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   * The ARP subclass needs to know from which NetDevice this
   * packet is coming to:
   *    - implement a per-NetDevice ARP cache
   *    - send back arp replies on the right device
   * \param device network device
   * \param p the packet
   * \param protocol lower layer protocol value
   * \param from lower layer address of the correspondant
   * \param to lower layer address of the destination
   * \param packetType type of the packet (broadcast/multicast/unicast/otherhost)
   */
  void Receive ( Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol,
                 const Address &from,
                 const Address &to,
                 NetDevice::PacketType packetType);

  /**
   * \param packet packet to send
   * \param route route entry
   *
   * Higher-level layers call this method to send a packet
   * down the stack to the MAC and PHY layers.
   */
  void Send (Ptr<Packet> packet, Ptr<CcnxRoute> route);

  uint32_t AddInterface (Ptr<NetDevice> device);
  Ptr<CcnxInterface> GetInterface (uint32_t i) const;
  uint32_t GetNInterfaces (void) const;

  int32_t GetInterfaceForDevice (Ptr<const NetDevice> device) const;
  // bool IsDestinationAddress (CcnxAddress address, uint32_t iif) const;

  void SetMetric (uint32_t i, uint16_t metric);
  uint16_t GetMetric (uint32_t i) const;
  uint16_t GetMtu (uint32_t i) const;
  bool IsUp (uint32_t i) const;
  void SetUp (uint32_t i);
  void SetDown (uint32_t i);

  Ptr<NetDevice> GetNetDevice (uint32_t i);

protected:

  virtual void DoDispose (void);
  /**
   * This function will notify other components connected to the node that a new stack member is now connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them together.
   */
  virtual void NotifyNewAggregate ();
private:
  // friend class CcnxL3ProtocolTestCase;
  CcnxL3Protocol(const CcnxL3Protocol &);
  CcnxL3Protocol &operator = (const CcnxL3Protocol &);

  void
  SendRealOut (Ptr<CcnxRoute> route,
               Ptr<Packet> packet,
               CcnxHeader const &ipHeader);

  // void 
  // IpForward (Ptr<CcnxRoute> rtentry, 
  //            Ptr<const Packet> p, 
  //            const CcnxHeader &header);

  // void
  // IpMulticastForward (Ptr<CcnxMulticastRoute> mrtentry, 
  //                     Ptr<const Packet> p, 
  //                     const CcnxHeader &header);

  void LocalDeliver (Ptr<const Packet> p, CcnxHeader const&ip, uint32_t iif);
  void RouteInputError (Ptr<const Packet> p, const CcnxHeader & ipHeader, Socket::SocketErrno sockErrno);

  uint32_t AddCcnxInterface (Ptr<CcnxInterface> interface);

  typedef std::vector<Ptr<CcnxInterface> > CcnxInterfaceList;

  CcnxInterfaceList m_interfaces;
  uint16_t m_identification;
  Ptr<Node> m_node;
  CcnxL4Protocol m_layer4;

  TracedCallback<const CcnxHeader &, Ptr<const Packet>, uint32_t> m_sendOutgoingTrace;
  TracedCallback<const CcnxHeader &, Ptr<const Packet>, uint32_t> m_unicastForwardTrace;
  TracedCallback<const CcnxHeader &, Ptr<const Packet>, uint32_t> m_localDeliverTrace;

  // The following two traces pass a packet with an IP header
  TracedCallback<Ptr<const Packet>, Ptr<Ccnx>,  uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, Ptr<Ccnx>, uint32_t> m_rxTrace;
  // <ip-header, payload, reason, ifindex> (ifindex not valid if reason is DROP_NO_ROUTE)
  TracedCallback<const CcnxHeader &, Ptr<const Packet>, DropReason, Ptr<Ccnx>, uint32_t> m_dropTrace;

  // Ptr<CcnxRoutingProtocol> m_routingProtocol;
};
  
} // Namespace ns3

#endif /* CCNX_L3_PROTOCOL_H */
