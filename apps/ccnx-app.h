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

#ifndef CCNX_APP_H
#define CCNX_APP_H

#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/callback.h"
#include "ns3/traced-callback.h"

namespace ns3 
{

class Packet;
class CcnxInterestHeader;
class CcnxContentObjectHeader;
class CcnxFace;

/**
 * @ingroup ccnx
 * @brief Base class that all CCNx applications should be derived from.
 * 
 * The class implements virtual calls onInterest, onNack, and onContentObject
 */
class CcnxApp: public Application
{
public:
  /**
   * \typedef A callback to pass packets to underlying CCNx protocol
   */
  typedef Callback<bool, const Ptr<const Packet>&> ProtocolHandler;
  
  static TypeId GetTypeId ();

  /**
   * @brief Default constructor
   */
  CcnxApp ();
  virtual ~CcnxApp ();

  /**
   * @brief Register lower layer callback (to send interests from the application)
   */
  void
  RegisterProtocolHandler (ProtocolHandler handler);
  
  /**
   * @brief Method that will be called every time new Interest arrives
   * @param interest Interest header
   * @param packet   "Payload" of the interests packet. The actual payload should be zero, but packet itself
   *                 may be useful to get packet tags
   */
  virtual void
  OnInterest (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> packet);

  /**
   * @brief Method that will be called every time new NACK arrives
   * @param interest Interest header
   */
  virtual void
  OnNack (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> packet);
  
  /**
   * @brief Method that will be called every time new ContentObject arrives
   * @param contentObject ContentObject header
   * @param payload payload (potentially virtual) of the ContentObject packet (may include packet tags of original packet)
   */
  virtual void
  OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                   Ptr<Packet> payload);
        
protected:
  virtual void
  DoDispose ();

  // inherited from Application base class.
  virtual void
  StartApplication ();    // Called at time specified by Start

  virtual void
  StopApplication ();     // Called at time specified by Stop

protected:
  ProtocolHandler m_protocolHandler;      ///< \brief A callback to pass packets to underlying CCNx protocol
  bool m_active; 
  Ptr<CcnxFace> m_face;   // local face that is created 

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_receivedInterests;

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_receivedNacks;

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_receivedContentObjects;


  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_transmittedInterests;

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_transmittedContentObjects;
};

} // namespace ns3

#endif // CCNX_APP_H
