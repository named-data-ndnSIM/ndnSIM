/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_APP_H
#define NDN_APP_H

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class Packet;

namespace ndn {

class Interest;
class Data;

class Face;

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
class App: public Application
{
public:
  static TypeId GetTypeId ();

  /**
   * @brief Default constructor
   */
  App ();
  virtual ~App ();

  /**
   * @brief Get application ID (ID of applications face)
   */
  uint32_t
  GetId () const;
   
  /**
   * @brief Method that will be called every time new Interest arrives
   * @param interest Interest header
   * @param packet   "Payload" of the interests packet. The actual payload should be zero, but packet itself
   *                 may be useful to get packet tags
   */
  virtual void
  OnInterest (Ptr<const Interest> interest);

  /**
   * @brief Method that will be called every time new NACK arrives
   * @param interest Interest header
   */
  virtual void
  OnNack (Ptr<const Interest> interest);
  
  /**
   * @brief Method that will be called every time new Data arrives
   * @param contentObject Data header
   * @param payload payload (potentially virtual) of the Data packet (may include packet tags of original packet)
   */
  virtual void
  OnData (Ptr<const Data> contentObject);
  
protected:
  /**
   * @brief Do cleanup when application is destroyed
   */
  virtual void
  DoDispose ();

  // inherited from Application base class. Originally they were private
  virtual void
  StartApplication ();    ///< @brief Called at time specified by Start

  virtual void
  StopApplication ();     ///< @brief Called at time specified by Stop

protected:
  bool m_active;  ///< @brief Flag to indicate that application is active (set by StartApplication and StopApplication)
  Ptr<Face> m_face;   ///< @brief automatically created application face through which application communicates

  TracedCallback<Ptr<const Interest>,
                 Ptr<App>, Ptr<Face> > m_receivedInterests; ///< @brief App-level trace of received Interests

  TracedCallback<Ptr<const Interest>,
                 Ptr<App>, Ptr<Face> > m_receivedNacks; ///< @brief App-level trace of received NACKs

  TracedCallback<Ptr<const Data>,
                 Ptr<App>, Ptr<Face> > m_receivedDatas; ///< @brief App-level trace of received Data


  TracedCallback<Ptr<const Interest>,
                 Ptr<App>, Ptr<Face> > m_transmittedInterests; ///< @brief App-level trace of transmitted Interests

  TracedCallback<Ptr<const Data>,
                 Ptr<App>, Ptr<Face> > m_transmittedDatas; ///< @brief App-level trace of transmitted Data
};

} // namespace ndn
} // namespace ns3

#endif // NDN_APP_H
