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

#ifndef NDN_NULL_TRANSPORT_HPP
#define NDN_NULL_TRANSPORT_HPP

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/transport.hpp"

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-face
 * \brief Null transport (does nothing, just fulfills requirements of the interface)
 */
class NullTransport : public nfd::face::Transport
{
public:
  NullTransport(const std::string& localUri, const std::string& remoteUri,
                ::ndn::nfd::FaceScope scope = ::ndn::nfd::FACE_SCOPE_NON_LOCAL,
                ::ndn::nfd::FacePersistency persistency = ::ndn::nfd::FACE_PERSISTENCY_PERSISTENT,
                ::ndn::nfd::LinkType linkType = ::ndn::nfd::LINK_TYPE_POINT_TO_POINT)
  {
    this->setLocalUri(FaceUri(localUri));
    this->setRemoteUri(FaceUri(remoteUri));
    this->setScope(scope);
    this->setPersistency(persistency);
    this->setLinkType(linkType);
    // this->setMtu(udp::computeMtu(m_socket.local_endpoint())); // not sure what should be here
  }

private:
  virtual void
  beforeChangePersistency(::ndn::nfd::FacePersistency newPersistency)
  {
  }

  virtual void
  doClose()
  {
    this->setState(nfd::face::TransportState::CLOSED);
  }

  virtual void
  doSend(Packet&& packet)
  {
  }
};

} // namespace ndn
} // namespace ns3

#endif // NDN_NULL_TRANSPORT_HPP
