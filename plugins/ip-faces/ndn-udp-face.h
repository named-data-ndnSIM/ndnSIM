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

#ifndef NDN_UDP_FACE_H
#define NDN_UDP_FACE_H

#include "ns3/ndn-face.h"
#include "ns3/socket.h"
#include "ns3/ptr.h"

#include <map>

namespace ns3 {
namespace ndn {
  
/**
 * \ingroup ndn-face
 * \brief Implementation of UDP/IP NDN face
 *
 * \see ndn::AppFace, ndn::NetDeviceFace, ndn::Ipv4Face, ndn::TcpFace
 */
class UdpFace : public Face
{
public:
  static TypeId
  GetTypeId ();

  /**
   * @brief A singleton method allowing creation and lookup of faces
   *
   * All created UDP faces are stored internally in the map, and if the same face is created, it will simply be looked up
   */
  static Ptr<UdpFace>
  CreateOrGetFace (Ptr<Node> node, Ipv4Address address);
  
  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   */
  UdpFace (Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address);
  virtual ~UdpFace();

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from NdnFace
  virtual void
  RegisterProtocolHandler (ProtocolHandler handler);

  Ipv4Address
  GetAddress () const;

  static Ptr<UdpFace>
  GetFaceByAddress (const Ipv4Address &addr);

protected:
  // also from NdnFace
  virtual bool
  SendImpl (Ptr<Packet> p);

public:
  /**
   * @brief Print out name of the NdnFace to the stream
   */
  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////

private:  
  UdpFace (const UdpFace &); ///< \brief Disabled copy constructor
  UdpFace& operator= (const UdpFace &); ///< \brief Disabled copy operator

  void
  ReceiveFromUdp (Ptr< Socket > clientSocket);

private:
  Ptr<Socket> m_socket;
  Ipv4Address m_address;

  typedef std::map<Ipv4Address, Ptr<UdpFace> > FaceMap;
  static FaceMap s_map;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_UDP_FACE_H
