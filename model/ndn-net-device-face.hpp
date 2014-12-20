/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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

#ifndef NDN_NET_DEVICE_FACE_H
#define NDN_NET_DEVICE_FACE_H

#include "ndn-face.h"
#include "ns3/net-device.h"

namespace ns3 {
namespace ndn {
  
/**
 * \ingroup ndn-face
 * \brief Implementation of layer-2 (Ethernet) Ndn face
 *
 * This class defines basic functionality of Ndn face. Face is core
 * component responsible for actual delivery of data packet to and
 * from Ndn stack
 *
 * NdnNetDevice face is permanently associated with one NetDevice
 * object and this object cannot be changed for the lifetime of the
 * face
 *
 * \see NdnAppFace, NdnNetDeviceFace, NdnIpv4Face, NdnUdpFace
 */
class NetDeviceFace  : public Face
{
public:
  static TypeId
  GetTypeId ();

  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   * @param netDevice a smart pointer to NetDevice object to which
   * this face will be associate
   */
  NetDeviceFace (Ptr<Node> node, const Ptr<NetDevice> &netDevice);
  virtual ~NetDeviceFace();

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from NdnFace
  virtual void
  RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler);

  virtual void
  UnRegisterProtocolHandlers ();
  
protected:
  virtual bool
  Send (Ptr<Packet> p);

public:
  /**
   * @brief Print out name of the NdnFace to the stream
   */
  virtual std::ostream&
  Print (std::ostream &os) const;
  ////////////////////////////////////////////////////////////////////

  /**
   * \brief Get NetDevice associated with the face
   *
   * \returns smart pointer to NetDevice associated with the face
   */
  Ptr<NetDevice> GetNetDevice () const;

private:
  NetDeviceFace (const NetDeviceFace &); ///< \brief Disabled copy constructor
  NetDeviceFace& operator= (const NetDeviceFace &); ///< \brief Disabled copy operator

  /// \brief callback from lower layers
  void ReceiveFromNetDevice (Ptr<NetDevice> device,
                             Ptr<const Packet> p,
                             uint16_t protocol,
                             const Address &from,
                             const Address &to,
                             NetDevice::PacketType packetType);

private:
  Ptr<NetDevice> m_netDevice; ///< \brief Smart pointer to NetDevice
};

} // namespace ndn
} // namespace ns3

#endif //NDN_NET_DEVICE_FACE_H
