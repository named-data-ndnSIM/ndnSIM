/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *                    Alexander Afanasyev
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

#ifndef NDN_IP_FACE_STACK_H
#define NDN_IP_FACE_STACK_H

#include "ns3/application.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/ptr.h"
#include "ns3/ndn-name.h"

#include <map>

namespace ns3 {

class Packet;

namespace ndn {

class Face;
class TcpFace;
class UdpFace;

/**
 * @ingroup ndn
 * @brief Application that provides functionality of creating IP-based faces on NDN nodes
 *
 * The class implements virtual calls onInterest, onNack, and onData
 */
class IpFaceStack: public Object
{
public:
  static TypeId GetTypeId ();

  /**
   * @brief Default constructor
   */
  IpFaceStack ();
  virtual ~IpFaceStack ();

  /**
   * @brief Lookup TcpFace for a given address
   */
  Ptr<TcpFace>
  GetTcpFaceByAddress (const Ipv4Address &addr);

  /**
   * @brief Destroy TcpFace, e.g., after TCP connection got dropped
   */
  void
  DestroyTcpFace (Ptr<TcpFace> face);

  /**
   * @brief Lookup UdpFace for a given address
   */
  Ptr<UdpFace>
  GetUdpFaceByAddress (const Ipv4Address &addr);

  /**
   * @brief Method allowing creation and lookup of faces
   *
   * All created UDP faces are stored internally in the map, and if the same face is created, it will simply be looked up
   */
  Ptr<TcpFace>
  CreateOrGetTcpFace (Ipv4Address address,
                      Callback< void, Ptr<Face> > onCreate = NULL_CREATE_CALLBACK);

  /**
   * @brief Method allowing creation and lookup of faces
   *
   * All created TCP faces are stored internally in the map, and if the same face is created, it will simply be looked up
   */
  Ptr<UdpFace>
  CreateOrGetUdpFace (Ipv4Address address);

protected:
  void
  NotifyNewAggregate ();

private:
  void
  StartServer ();

  bool
  OnTcpConnectionRequest (Ptr< Socket > sock, const Address &addr);

  void
  OnTcpConnectionAccept (Ptr< Socket > sock, const Address &addr);

  void
  OnTcpConnectionClosed (Ptr< Socket > sock);

  void
  OnUdpPacket (Ptr< Socket > sock);

public:
  const static Callback< void, Ptr<Face> > NULL_CREATE_CALLBACK;

protected:
  Ptr<Node> m_node;

  bool m_enableTcp;
  bool m_enableUdp;

  Ptr<Socket> m_tcpServer;
  Ptr<Socket> m_udpServer;

  typedef std::map< Ipv4Address, Ptr<TcpFace> > TcpFaceMap;
  typedef std::map< Ipv4Address, Ptr<UdpFace> > UdpFaceMap;
  TcpFaceMap m_tcpFaceMap;
  UdpFaceMap m_udpFaceMap;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_IP_FACE_STACK_H
