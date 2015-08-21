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

#ifndef NDN_APP_H
#define NDN_APP_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-app-face.hpp"

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class Packet;

namespace ndn {

/**
 * \ingroup ndn
 * \defgroup ndn-apps NDN applications
 */
/**
 * @ingroup ndn-apps
 * @brief Base class that all NDN applications should be derived from.
 *
 * The class implements virtual calls onInterest, onNack, and onData
 */
class App : public Application {
public:
  static TypeId
  GetTypeId();

  /**
   * @brief Default constructor
   */
  App();
  virtual ~App();

  /**
   * @brief Get application ID (ID of applications face)
   */
  uint32_t
  GetId() const;

  /**
   * @brief Method that will be called every time new Interest arrives
   * @param interest Interest header
   * @param packet   "Payload" of the interests packet. The actual payload should be zero, but
   * packet itself
   *                 may be useful to get packet tags
   */
  virtual void
  OnInterest(shared_ptr<const Interest> interest);

  /**
   * @brief Method that will be called every time new Data arrives
   * @param contentObject Data header
   * @param payload payload (potentially virtual) of the Data packet (may include packet tags of
   * original packet)
   */
  virtual void
  OnData(shared_ptr<const Data> data);

public:
  typedef void (*InterestTraceCallback)(shared_ptr<const Interest>, Ptr<App>, shared_ptr<Face>);
  typedef void (*DataTraceCallback)(shared_ptr<const Data>, Ptr<App>, shared_ptr<Face>);

protected:
  virtual void
  DoInitialize();

  virtual void
  DoDispose();

  // inherited from Application base class. Originally they were private
  virtual void
  StartApplication(); ///< @brief Called at time specified by Start

  virtual void
  StopApplication(); ///< @brief Called at time specified by Stop

protected:
  bool m_active; ///< @brief Flag to indicate that application is active (set by StartApplication and StopApplication)
  shared_ptr<AppFace> m_face; ///< @brief automatically created application face through which application communicates
  uint32_t m_appId;

  TracedCallback<shared_ptr<const Interest>, Ptr<App>, shared_ptr<Face>>
    m_receivedInterests; ///< @brief App-level trace of received Interests

  TracedCallback<shared_ptr<const Data>, Ptr<App>, shared_ptr<Face>>
    m_receivedDatas; ///< @brief App-level trace of received Data

  TracedCallback<shared_ptr<const Interest>, Ptr<App>, shared_ptr<Face>>
    m_transmittedInterests; ///< @brief App-level trace of transmitted Interests

  TracedCallback<shared_ptr<const Data>, Ptr<App>, shared_ptr<Face>>
    m_transmittedDatas; ///< @brief App-level trace of transmitted Data
};

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_H
