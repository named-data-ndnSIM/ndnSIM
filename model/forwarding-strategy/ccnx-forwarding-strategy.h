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
class CcnxContentObjectHeader;
class CcnxPit;
class CcnxPitEntry;
class CcnxFibFaceMetric;
class CcnxFib;
class CcnxContentStore;

/**
 * \ingroup ccnx
 * \brief Abstract base class for CCNx forwarding strategies
 */
class CcnxForwardingStrategy :
    public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Default constructor
   */
  CcnxForwardingStrategy ();
  virtual ~CcnxForwardingStrategy ();

  /**
   * \brief Actual processing of incoming CCNx interests. Note, interests do not have payload
   * 
   * Processing Interest packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  virtual void
  OnInterest (const Ptr<CcnxFace> &face,
              Ptr<CcnxInterestHeader> &header,
              const Ptr<const Packet> &p);

  /**
   * \brief Actual processing of incoming CCNx content objects
   * 
   * Processing ContentObject packets
   * @param face    incoming face
   * @param header  deserialized ContentObject header
   * @param payload data packet payload
   * @param packet  original packet
   */
  virtual void
  OnData (const Ptr<CcnxFace> &face,
          Ptr<CcnxContentObjectHeader> &header,
          Ptr<Packet> &payload,
          const Ptr<const Packet> &packet);
    
protected:
  // events
  virtual void
  DidReceiveDuplicateInterest (const Ptr<CcnxFace> &face,
                               Ptr<CcnxInterestHeader> &header,
                               const Ptr<const Packet> &packet,
                               Ptr<CcnxPitEntry> pitEntry);

  virtual void
  DidExhaustForwardingOptions (const Ptr<CcnxFace> &incomingFace,
                               Ptr<CcnxInterestHeader> header,
                               const Ptr<const Packet> &packet,
                               Ptr<CcnxPitEntry> pitEntry);

  virtual void
  FailedToCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                          Ptr<CcnxInterestHeader> header,
                          const Ptr<const Packet> &packet);
  
  virtual void
  DidCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                     Ptr<CcnxInterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<CcnxPitEntry> pitEntry);

  virtual bool
  DetectRetransmittedInterest (const Ptr<CcnxFace> &incomingFace,
                               Ptr<CcnxPitEntry> pitEntry);

  // only for data received from network
  virtual void
  WillSatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                              Ptr<CcnxPitEntry> pitEntry);

  // for data received both from network and cache
  virtual void
  SatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace, // 0 allowed (from cache)
                          Ptr<const CcnxContentObjectHeader> header,
                          Ptr<const Packet> payload,
                          const Ptr<const Packet> &packet,
                          Ptr<CcnxPitEntry> pitEntry);

  virtual void
  DidReceiveUnsolicitedData (const Ptr<CcnxFace> &incomingFace,
                             Ptr<const CcnxContentObjectHeader> header,
                             Ptr<const Packet> payload);
  
  virtual bool
  ShouldSuppressIncomingInterest (const Ptr<CcnxFace> &incomingFace,
                                  Ptr<CcnxPitEntry> pitEntry);


  virtual void
  PropagateInterest (const Ptr<CcnxFace> &incomingFace,
                     Ptr<CcnxInterestHeader> &header,
                     const Ptr<const Packet> &packet,
                     Ptr<CcnxPitEntry> pitEntry);
  
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
  DoPropagateInterest (const Ptr<CcnxFace> &incomingFace,
                       Ptr<CcnxInterestHeader> &header,
                       const Ptr<const Packet> &packet,
                       Ptr<CcnxPitEntry> pitEntry) = 0;


  // virtual void
  // OnDataDelayed (Ptr<const CcnxContentObjectHeader> header,
  //                Ptr<const Packet> payload,
  //                const Ptr<const Packet> &packet);
  
protected:
  // inherited from Object class                                                                                                                                                        
  virtual void NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object
  virtual void DoDispose (); ///< @brief Do cleanup
  
protected:  
  Ptr<CcnxPit> m_pit; ///< \brief Reference to PIT to which this forwarding strategy is associated
  Ptr<CcnxFib> m_fib; ///< \brief FIB  
  Ptr<CcnxContentStore> m_contentStore; ///< \brief Content store (for caching purposes only)

  bool m_cacheUnsolicitedData;
  bool m_detectRetransmissions;
  
  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxFace> > m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxFace> > m_inInterests; ///< @brief trace of incoming Interests

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxFace> > m_dropInterests; ///< @brief trace of dropped Interests
  
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                 bool /*from cache*/,
                 Ptr<const CcnxFace> > m_outData; ///< @brief trace of outgoing Data

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                 Ptr<const CcnxFace> > m_inData; ///< @brief trace of incoming Data

  TracedCallback<Ptr<const CcnxContentObjectHeader>, Ptr<const Packet>,
                  Ptr<const CcnxFace> > m_dropData;  ///< @brief trace of dropped Data
};

} //namespace ns3

#endif /* CCNX_FORWARDING_STRATEGY_H */
