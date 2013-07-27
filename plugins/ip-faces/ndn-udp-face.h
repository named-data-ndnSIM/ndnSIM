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
   * \brief Constructor
   *
   * @param node Node associated with the face
   */
  UdpFace (Ptr<Node> node, Ptr<Socket> socket, Ipv4Address address);
  virtual ~UdpFace();

  Ipv4Address
  GetAddress () const;

  virtual bool
  ReceiveFromUdp (Ptr<const Packet> p);

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from ndn::Face
  virtual std::ostream&
  Print (std::ostream &os) const;

protected:
  // also from ndn::Face
  virtual bool
  Send (Ptr<Packet> p);

private:  
  UdpFace (const UdpFace &); ///< \brief Disabled copy constructor
  UdpFace& operator= (const UdpFace &); ///< \brief Disabled copy operator

private:
  Ptr<Socket> m_socket;
  Ipv4Address m_address;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_UDP_FACE_H
