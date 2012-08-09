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
#ifndef NDN_FORWARDING_STRATEGY_H
#define NDN_FORWARDING_STRATEGY_H

#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"

namespace ns3 {

class NdnFace;
class NdnInterestHeader;
class NdnContentObjectHeader;
class NdnPit;
class NdnPitEntry;
class NdnFibFaceMetric;
class NdnFib;
class NdnContentStore;

/**
 * \ingroup ndn
 * \brief Abstract base class for Ndn forwarding strategies
 */
class NdnForwardingStrategy :
    public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Default constructor
   */
  NdnForwardingStrategy ();
  virtual ~NdnForwardingStrategy ();

  /**
   * \brief Actual processing of incoming Ndn interests. Note, interests do not have payload
   * 
   * Processing Interest packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  virtual void
  OnInterest (const Ptr<NdnFace> &face,
              Ptr<NdnInterestHeader> &header,
              const Ptr<const Packet> &p);

  /**
   * \brief Actual processing of incoming Ndn content objects
   * 
   * Processing ContentObject packets
   * @param face    incoming face
   * @param header  deserialized ContentObject header
   * @param payload data packet payload
   * @param packet  original packet
   */
  virtual void
  OnData (const Ptr<NdnFace> &face,
          Ptr<NdnContentObjectHeader> &header,
          Ptr<Packet> &payload,
          const Ptr<const Packet> &packet);

  virtual void
  WillErasePendingInterest (Ptr<NdnPitEntry> pitEntry);

  virtual void
  RemoveFace (Ptr<NdnFace> face);
  
protected:
  // events
  virtual void
  DidReceiveDuplicateInterest (const Ptr<NdnFace> &face,
                               Ptr<NdnInterestHeader> &header,
                               const Ptr<const Packet> &packet,
                               Ptr<NdnPitEntry> pitEntry);

  virtual void
  DidExhaustForwardingOptions (const Ptr<NdnFace> &incomingFace,
                               Ptr<NdnInterestHeader> header,
                               const Ptr<const Packet> &packet,
                               Ptr<NdnPitEntry> pitEntry);

  virtual void
  FailedToCreatePitEntry (const Ptr<NdnFace> &incomingFace,
                          Ptr<NdnInterestHeader> header,
                          const Ptr<const Packet> &packet);
  
  virtual void
  DidCreatePitEntry (const Ptr<NdnFace> &incomingFace,
                     Ptr<NdnInterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<NdnPitEntry> pitEntry);

  virtual bool
  DetectRetransmittedInterest (const Ptr<NdnFace> &incomingFace,
                               Ptr<NdnPitEntry> pitEntry);

  // makes sense only for data received from network
  // When Interest is satisfied from the cache, incoming face is 0
  virtual void
  WillSatisfyPendingInterest (const Ptr<NdnFace> &incomingFace,
                              Ptr<NdnPitEntry> pitEntry);

  // for data received both from network and cache
  virtual void
  SatisfyPendingInterest (const Ptr<NdnFace> &incomingFace, // 0 allowed (from cache)
                          Ptr<const NdnContentObjectHeader> header,
                          Ptr<const Packet> payload,
                          const Ptr<const Packet> &packet,
                          Ptr<NdnPitEntry> pitEntry);

  virtual void
  DidSendOutData (const Ptr<NdnFace> &face,
                  Ptr<const NdnContentObjectHeader> header,
                  Ptr<const Packet> payload,
                  const Ptr<const Packet> &packet);
  
  virtual void
  DidReceiveUnsolicitedData (const Ptr<NdnFace> &incomingFace,
                             Ptr<const NdnContentObjectHeader> header,
                             Ptr<const Packet> payload);
  
  virtual bool
  ShouldSuppressIncomingInterest (const Ptr<NdnFace> &incomingFace,
                                  Ptr<NdnPitEntry> pitEntry);

  /**
   * @brief Event fired before actually sending out an interest
   *
   * If event returns false, then there is some kind of a problem (e.g., per-face limit reached)
   */
  virtual bool
  WillSendOutInterest (const Ptr<NdnFace> &outgoingFace,
                       Ptr<NdnInterestHeader> header,
                       Ptr<NdnPitEntry> pitEntry);

  /**
   * @brief Event fired just after sending out an interest
   */
  virtual void
  DidSendOutInterest (const Ptr<NdnFace> &outgoingFace,
                      Ptr<NdnInterestHeader> header,
                      const Ptr<const Packet> &packet,
                      Ptr<NdnPitEntry> pitEntry);

  virtual void
  PropagateInterest (const Ptr<NdnFace> &incomingFace,
                     Ptr<NdnInterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<NdnPitEntry> pitEntry);
  
  /**
   * @brief Base method to propagate the interest according to the forwarding strategy
   *
   * @param pitEntry      Reference to PIT entry (reference to corresponding FIB entry inside)
   * @param incomingFace  Incoming face
   * @param header        NdnInterestHeader
   * @param packet        Original Interest packet
   * @param sendCallback  Send callback
   *
   * @return true if interest was successfully propagated, false if all options have failed
   */
  virtual bool
  DoPropagateInterest (const Ptr<NdnFace> &incomingFace,
                       Ptr<NdnInterestHeader> header,
                       const Ptr<const Packet> &packet,
                       Ptr<NdnPitEntry> pitEntry) = 0;


  // virtual void
  // OnDataDelayed (Ptr<const NdnContentObjectHeader> header,
  //                Ptr<const Packet> payload,
  //                const Ptr<const Packet> &packet);
  
protected:
  // inherited from Object class                                                                                                                                                        
  virtual void NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object
  virtual void DoDispose (); ///< @brief Do cleanup
  
protected:  
  Ptr<NdnPit> m_pit; ///< \brief Reference to PIT to which this forwarding strategy is associated
  Ptr<NdnFib> m_fib; ///< \brief FIB  
  Ptr<NdnContentStore> m_contentStore; ///< \brief Content store (for caching purposes only)

  bool m_cacheUnsolicitedData;
  bool m_detectRetransmissions;
  
  TracedCallback<Ptr<const NdnInterestHeader>,
                 Ptr<const NdnFace> > m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<Ptr<const NdnInterestHeader>,
                 Ptr<const NdnFace> > m_inInterests; ///< @brief trace of incoming Interests

  TracedCallback<Ptr<const NdnInterestHeader>,
                 Ptr<const NdnFace> > m_dropInterests; ///< @brief trace of dropped Interests
  
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback<Ptr<const NdnContentObjectHeader>, Ptr<const Packet>,
                 bool /*from cache*/,
                 Ptr<const NdnFace> > m_outData; ///< @brief trace of outgoing Data

  TracedCallback<Ptr<const NdnContentObjectHeader>, Ptr<const Packet>,
                 Ptr<const NdnFace> > m_inData; ///< @brief trace of incoming Data

  TracedCallback<Ptr<const NdnContentObjectHeader>, Ptr<const Packet>,
                  Ptr<const NdnFace> > m_dropData;  ///< @brief trace of dropped Data
};

} //namespace ns3

#endif /* NDN_FORWARDING_STRATEGY_H */
