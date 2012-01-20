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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 */
#ifndef CCNX_FORWARDING_STRATEGY_H
#define CCNX_FORWARDING_STRATEGY_H

#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class CcnxFace;
class CcnxInterestHeader;
class CcnxPit;
class CcnxPitEntry;
class CcnxFibFaceMetric;

/**
 * \ingroup ccnx
 * \brief Abstract base class for CCNx forwarding strategies
 */
class CcnxForwardingStrategy : public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Default constructor
   */
  CcnxForwardingStrategy ();
  virtual ~CcnxForwardingStrategy ();

  /**
   * @brief Base method to propagate the interest according to the forwarding strategy
   *
   * @param pitEntry      Reference to PIT entry (reference to corresponding FIB entry inside)
   * @param incomingFace  Incoming face
   * @param header        CcnxInterestHeader
   * @param packet        Original Interest packet
   * @param sendCallback  Send callback
   *
   * @return true if interest was successfully propagated, false if all options have failed
   */
  virtual bool
  PropagateInterest (const CcnxPitEntry  &pitEntry, 
                     const Ptr<CcnxFace> &incomingFace,
                     Ptr<CcnxInterestHeader> &header,
                     const Ptr<const Packet> &packet) = 0;
    
  /**
   * @brief Set link to PIT for the forwarding strategy
   *
   * @param pit pointer to PIT
   */
  void
  SetPit (Ptr<CcnxPit> pit);
  
protected:
  /**
   * @brief Propagate interest via a green interface. Fail, if no green interfaces available
   *
   * @param pitEntry      Reference to PIT entry (reference to corresponding FIB entry inside)
   * @param incomingFace  Incoming face
   * @param header        CcnxInterestHeader
   * @param packet        Original Interest packet
   * @param sendCallback  Send callback
   * @return true if interest was successfully propagated, false if all options have failed
   *
   * \see PropagateInterest
   */
  bool
  PropagateInterestViaGreen (const CcnxPitEntry  &pitEntry, 
                             const Ptr<CcnxFace> &incomingFace,
                             Ptr<CcnxInterestHeader> &header,
                             const Ptr<const Packet> &packet);

  TracedCallback<Ptr<const CcnxInterestHeader>, Ptr<const CcnxFace> > m_transmittedInterestsTrace;
  
protected:  
  Ptr<CcnxPit> m_pit;
};

} //namespace ns3

#endif /* CCNX_FORWARDING_STRATEGY_H */
