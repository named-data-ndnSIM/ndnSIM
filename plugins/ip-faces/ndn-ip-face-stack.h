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

/**
 * @ingroup ndn
 * @brief Application that provides functionality of creating IP-based faces on NDN nodes
 * 
 * The class implements virtual calls onInterest, onNack, and onContentObject
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

protected:
  Ptr<Node> m_node;
  
  bool m_enableTcp;
  bool m_enableUdp;
  
  Ptr<Socket> m_tcpServer;
  Ptr<Socket> m_udpServer;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_IP_FACE_STACK_H
