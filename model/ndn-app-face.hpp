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
 * Authors:     Alexander Afanasyev <alexander.afanasyev@ucla.edu>
                Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

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
   *
   * @note To send Interest from application, use `AppFace::onReceiveInterest(interest)`
   */
  virtual void
  sendInterest(const Interest& interest);

  /**
   * @brief Send Data towards application
   *
   * @note To send Data from application, use `AppFace::onReceiveData(data)`
   */
  virtual void
  sendData(const Data& data);

  virtual void
  close();

private:
  Ptr<Node> m_node;
  Ptr<App> m_app;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_FACE_H
