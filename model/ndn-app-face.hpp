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

#ifndef NDN_APP_FACE_H
#define NDN_APP_FACE_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/NFD/daemon/face/local-face.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"

namespace ns3 {

class Packet;
class Node;

namespace ndn {

class App;

/**
 * \ingroup ndn-face
 * \brief Implementation of application Ndn face
 *
 * This class defines basic functionality of Ndn face. Face is core
 * component responsible for actual delivery of data packet to and
 * from Ndn stack
 *
 * \see AppFace, NdnNetDeviceFace
 */
class AppFace : public nfd::LocalFace {
public:
  /**
   * \brief Default constructor
   */
  AppFace(Ptr<App> app);

  virtual ~AppFace();

public: // from nfd::Face
  /**
   * @brief Send Interest towards application
   */
  virtual void
  sendInterest(const Interest& interest);

  /**
   * @brief Send Data towards application
   */
  virtual void
  sendData(const Data& data);

  /**
   * @brief Send Interest towards NFD
   */
  void
  onReceiveInterest(const Interest& interest);

  /**
   * @brief Send Data towards NFD
   */
  void
  onReceiveData(const Data& data);

  virtual void
  close();

private:
  Ptr<Node> m_node;
  Ptr<App> m_app;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_FACE_H
