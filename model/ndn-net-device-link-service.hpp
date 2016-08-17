/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef NDN_NET_DEVICE_LINK_SERVICE_HPP
#define NDN_NET_DEVICE_LINK_SERVICE_HPP

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/link-service.hpp"

#include "ns3/net-device.h"

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-face
 * \brief Implementation of layer-2 (Ethernet) LinkService (current hack, to be changed eventually)
 *
 * NetDeviceLinkService is permanently associated with one NetDevice
 * object and this object cannot be changed for the lifetime of the
 * face
 *
 * \see AppLinkService
 */
class NetDeviceLinkService : public nfd::face::LinkService
{

public:
  /**
   * \brief Constructor
   *
   * @param node Node associated with the face
   * @param netDevice a smart pointer to NetDevice object to which this NetDeviceLinkService will be associate
   */
  NetDeviceLinkService(Ptr<Node> node, const Ptr<NetDevice>& netDevice);

  virtual
  ~NetDeviceLinkService();

public:
  /**
   * \brief Get Node associated with the LinkService
   */
  Ptr<Node>
  GetNode() const;

  /**
   * \brief Get NetDevice associated with the LinkService
   */
  Ptr<NetDevice>
  GetNetDevice() const;

private:
  virtual void
  doSendInterest(const ::ndn::Interest& interest) override;

  virtual void
  doSendData(const ::ndn::Data& data) override;

  virtual void
  doSendNack(const ::ndn::lp::Nack& nack) override;

  virtual void
  doReceivePacket(nfd::face::Transport::Packet&& packet) override
  {
    // not used now
    BOOST_ASSERT(false);
  }

private:
  void
  send(Ptr<Packet> packet);

  /// \brief callback from lower layers
  void
  receiveFromNetDevice(Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol,
                       const Address& from, const Address& to, NetDevice::PacketType packetType);

private:
  Ptr<Node> m_node;
  Ptr<NetDevice> m_netDevice; ///< \brief Smart pointer to NetDevice
};

} // namespace ndn
} // namespace ns3

#endif // NDN_NET_DEVICE_LINK_SERVICE_HPP
