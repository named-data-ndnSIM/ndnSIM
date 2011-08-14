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
class CcnxRoute;
class Node;
class Socket;
class CcnxForwardingProtocol;


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
    /** \todo  Fill reasons from QualNet code */
    DROP_DUPLICATE_INTEREST=1,  /**< Duplicate Interest */
    DROP_CONGESTION, /**< Congestion detected */
    DROP_NO_ROUTE,   /**< No route to host */
    DROP_INTERFACE_DOWN,   /**< Interface is down so can not send packet */
    DROP_ROUTE_ERROR,   /**< Route error */
  };

  void SetNode (Ptr<Node> node);

  // functions defined in base class Ccnx
  
  void SetForwardingProtocol (Ptr<CcnxForwardingProtocol> forwardingProtocol);
  Ptr<CcnxForwardingProtocol> GetForwardingProtocol (void) const;

  /**
   * Lower layer calls this method after calling L3Demux::Lookup
   *
   * \param device network device
   * \param p the packet
   * \param protocol lower layer protocol value
   * \param from lower layer address of the correspondant
   * \param to lower layer address of the destination
   * \param packetType type of the packet (broadcast/multicast/unicast/otherhost)
   */
  void Receive (Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol,
                 const Address &from,
                 const Address &to,
                 NetDevice::PacketType packetType);

  /**
   * Actual processing of incoming CCNx packets. Also processing packets coming from local apps
   */
  void Receive (Ptr<CcnxFace> face, Ptr<const Packet> p);
  
  /**
   * \param packet packet to send
   * \param route route entry
   *
   * Higher-level layers call this method to send a packet
   * down the stack to the MAC and PHY layers.
   */
  virtual void Send (Ptr<Packet> packet, Ptr<CcnxRoute> route);

  virtual uint32_t AddFace (Ptr<CcnxFace> face);
  virtual uint32_t GetNFaces (void) const;
  virtual Ptr<CcnxFace> GetFace (uint32_t face);

  virtual void SetMetric (uint32_t i, uint16_t metric);
  virtual uint16_t GetMetric (uint32_t i) const;
  virtual uint16_t GetMtu (uint32_t i) const;
  virtual bool IsUp (uint32_t i) const;
  virtual void SetUp (uint32_t i);
  virtual void SetDown (uint32_t i);

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

  /**
   * Helper function to get CcnxFace from NetDevice
   */
  Ptr<CcnxFace> GetFaceForDevice (Ptr<const NetDevice> device) const;
  
  void RouteInputError (Ptr<const Packet> p, const CcnxHeader & ipHeader, Socket::SocketErrno sockErrno);

private:
  typedef std::vector<Ptr<CcnxFace> > CcnxFaceList;
  CcnxFaceList m_faces;

  Ptr<Node> m_node;
  // Ptr<CcnxForwardingProtocol> m_forwardingProtocol;

  TracedCallback<const CcnxHeader &, Ptr<const Packet>, uint32_t> m_sendOutgoingTrace;
  TracedCallback<const CcnxHeader &, Ptr<const Packet>, uint32_t> m_unicastForwardTrace;
  TracedCallback<const CcnxHeader &, Ptr<const Packet>, uint32_t> m_localDeliverTrace;

  // The following two traces pass a packet with an IP header
  TracedCallback<Ptr<const Packet>, Ptr<Ccnx>,  uint32_t> m_txTrace;
  TracedCallback<Ptr<const Packet>, Ptr<Ccnx>, uint32_t> m_rxTrace;
  // <ip-header, payload, reason, ifindex> (ifindex not valid if reason is DROP_NO_ROUTE)
  TracedCallback<const CcnxHeader &, Ptr<const Packet>, DropReason, Ptr<Ccnx>, uint32_t> m_dropTrace;
};
  
} // Namespace ns3

#endif /* CCNX_L3_PROTOCOL_H */
