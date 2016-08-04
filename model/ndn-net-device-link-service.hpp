/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2015,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NDN_NET_DEVICE_LINK_SERVICE_HPP
#define NDN_NET_DEVICE_LINK_SERVICE_HPP

#include "core/counter.hpp"
#include "transport.hpp"
#include "face-log.hpp"

namespace ns3 {
namespace ndn {

class Face;

/** \brief the upper part of a Face
 *  \sa Face
 */
class NetDeviceLinkService : protected virtual LinkService
{

public:
  NetDeviceLinkService();

  virtual
  ~NetDeviceLinkService();

private: // upper interface to be overridden in subclass (send path entrypoint)
  /** \brief performs LinkService specific operations to send an Interest
   */
  virtual void
  doSendInterest(const Interest& interest) = 0;

  /** \brief performs LinkService specific operations to send a Data
   */
  virtual void
  doSendData(const Data& data) = 0;

  /** \brief performs LinkService specific operations to send a Nack
   */
  virtual void
  doSendNack(const lp::Nack& nack) = 0;

private: // lower interface to be overridden in subclass
  virtual void
  doReceivePacket(Transport::Packet&& packet) = 0;

private:
  Face* m_face;
  Transport* m_transport;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_NET_DEVICE_LINK_SERVICE_HPP
