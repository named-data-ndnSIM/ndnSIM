/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_TCP_FACE_H
#define NDN_TCP_FACE_H

#include "ns3/ndn-face.h"
#include "ns3/socket.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"

#include <map>

namespace ns3 {
namespace ndn {
  
/**
 * \ingroup ndn-face
 * \brief Implementation of TCP/IP NDN face
 *
 * \see NdnAppFace, NdnNetDeviceFace, NdnIpv4Face, NdnUdpFace
 */
class TcpFace : public Face
{
public:
  static TypeId
  GetTypeId ();
  
  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   */
  TcpFace (Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address);
  virtual ~TcpFace();

  void
  OnTcpConnectionClosed (Ptr<Socket> socket);

  Ipv4Address
  GetAddress () const;

  static Ptr<TcpFace>
  GetFaceByAddress (const Ipv4Address &addr);

  void
  SetCreateCallback (Callback< void, Ptr<Face> > callback);

  void
  OnConnect (Ptr<Socket> socket);

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ndn::Face
  virtual void
  RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler);

  virtual void
  UnRegisterProtocolHandlers ();

  virtual std::ostream&
  Print (std::ostream &os) const;
  
protected:
  // also from ndn::Face
  virtual bool
  Send (Ptr<Packet> p);

private:  
  TcpFace (const TcpFace &); ///< \brief Disabled copy constructor
  TcpFace& operator= (const TcpFace &); ///< \brief Disabled copy operator

  void
  ReceiveFromTcp (Ptr< Socket > clientSocket);

private:
  Ptr<Socket> m_socket;
  Ipv4Address m_address;
  uint32_t m_pendingPacketLength;
  Callback< void, Ptr<Face> > m_onCreateCallback;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_TCP_FACE_H
