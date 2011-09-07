/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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

#ifndef CCNX_NET_DEVICE_FACE_H
#define CCNX_NET_DEVICE_FACE_H

#include "ccnx-face.h"
#include "ns3/net-device.h"

namespace ns3 {

class Address;
  
/**
 * \ingroup ccnx-face
 * \brief Implementation of layer-2 (Ethernet) CCNx face
 *
 * This class defines basic functionality of CCNx face. Face is core
 * component responsible for actual delivery of data packet to and
 * from CCNx stack
 *
 * CcnxNetDevice face is permanently associated with one NetDevice
 * object and this object cannot be changed for the lifetime of the
 * face
 *
 * \see CcnxLocalFace, CcnxNetDeviceFace, CcnxIpv4Face, CcnxUdpFace
 */
class CcnxNetDeviceFace  : public CcnxFace
{
public:
  // /**
  //  * \brief Interface ID
  //  *
  //  * \return interface ID
  //  */
  // static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   *
   * \param netDevice a smart pointer to NetDevice object to which
   * this face will be associate
   */
  CcnxNetDeviceFace (const Ptr<NetDevice> &netDevice);
  virtual ~CcnxNetDeviceFace();

  ////////////////////////////////////////////////////////////////////
  // methods overloaded from CcnxFace
  
  virtual void
  RegisterProtocolHandler (ProtocolHandler handler);

  virtual void
  Send (Ptr<Packet> p);

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
  CcnxNetDeviceFace (const CcnxNetDeviceFace &); ///< \brief Disabled copy constructor
  CcnxNetDeviceFace& operator= (const CcnxNetDeviceFace &); ///< \brief Disabled copy operator

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

} // namespace ns3

#endif //CCNX_NET_DEVICE_FACE_H
