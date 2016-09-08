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

#ifndef NDN_APP_LINK_SERVICE_HPP
#define NDN_APP_LINK_SERVICE_HPP

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/link-service.hpp"

namespace ns3 {

class Packet;
class Node;

namespace ndn {

class App;

/**
 * \ingroup ndn-face
 * \brief Implementation of LinkService for ndnSIM application
 *
 * \see NetDeviceLinkService
 */
class AppLinkService : public nfd::face::LinkService
{
public:
  /**
   * \brief Default constructor
   */
  AppLinkService(Ptr<App> app);

  virtual ~AppLinkService();

public:
  void
  onReceiveInterest(const Interest& interest);

  void
  onReceiveData(const Data& data);

  void
  onReceiveNack(const lp::Nack& nack);

private:
  virtual void
  doSendInterest(const Interest& interest) override;

  virtual void
  doSendData(const Data& data) override;

  virtual void
  doSendNack(const lp::Nack& nack) override;

  virtual void
  doReceivePacket(nfd::face::Transport::Packet&& packet) override
  {
    // does nothing (all operations for now handled by LinkService)
    BOOST_ASSERT(false);
  }

private:
  Ptr<Node> m_node;
  Ptr<App> m_app;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_LINK_SERVICE_HPP
