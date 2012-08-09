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
namespace ndn {

class Face;
class InterestHeader;
class ContentObjectHeader;
class Pit;
namespace pit { class Entry; }
class FibFaceMetric;
class Fib;
class ContentStore;

/**
 * \ingroup ndn
 * \brief Abstract base class for Ndn forwarding strategies
 */
class ForwardingStrategy :
    public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * @brief Default constructor
   */
  ForwardingStrategy ();
  virtual ~ForwardingStrategy ();

  /**
   * \brief Actual processing of incoming Ndn interests. Note, interests do not have payload
   * 
   * Processing Interest packets
   * @param face    incoming face
   * @param header  deserialized Interest header
   * @param packet  original packet
   */
  virtual void
  OnInterest (const Ptr<Face> &face,
              Ptr<InterestHeader> &header,
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
  OnData (const Ptr<Face> &face,
          Ptr<ContentObjectHeader> &header,
          Ptr<Packet> &payload,
          const Ptr<const Packet> &packet);

  virtual void
  WillErasePendingInterest (Ptr<pit::Entry> pitEntry);

  virtual void
  RemoveFace (Ptr<Face> face);
  
protected:
  // events
  virtual void
  DidReceiveDuplicateInterest (const Ptr<Face> &face,
                               Ptr<InterestHeader> &header,
                               const Ptr<const Packet> &packet,
                               Ptr<pit::Entry> pitEntry);

  virtual void
  DidExhaustForwardingOptions (const Ptr<Face> &incomingFace,
                               Ptr<InterestHeader> header,
                               const Ptr<const Packet> &packet,
                               Ptr<pit::Entry> pitEntry);

  virtual void
  FailedToCreatePitEntry (const Ptr<Face> &incomingFace,
                          Ptr<InterestHeader> header,
                          const Ptr<const Packet> &packet);
  
  virtual void
  DidCreatePitEntry (const Ptr<Face> &incomingFace,
                     Ptr<InterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<pit::Entry> pitEntry);

  virtual bool
  DetectRetransmittedInterest (const Ptr<Face> &incomingFace,
                               Ptr<pit::Entry> pitEntry);

  // makes sense only for data received from network
  // When Interest is satisfied from the cache, incoming face is 0
  virtual void
  WillSatisfyPendingInterest (const Ptr<Face> &incomingFace,
                              Ptr<pit::Entry> pitEntry);

  // for data received both from network and cache
  virtual void
  SatisfyPendingInterest (const Ptr<Face> &incomingFace, // 0 allowed (from cache)
                          Ptr<const ContentObjectHeader> header,
                          Ptr<const Packet> payload,
                          const Ptr<const Packet> &packet,
                          Ptr<pit::Entry> pitEntry);

  virtual void
  DidSendOutData (const Ptr<Face> &face,
                  Ptr<const ContentObjectHeader> header,
                  Ptr<const Packet> payload,
                  const Ptr<const Packet> &packet);
  
  virtual void
  DidReceiveUnsolicitedData (const Ptr<Face> &incomingFace,
                             Ptr<const ContentObjectHeader> header,
                             Ptr<const Packet> payload);
  
  virtual bool
  ShouldSuppressIncomingInterest (const Ptr<Face> &incomingFace,
                                  Ptr<pit::Entry> pitEntry);

  /**
   * @brief Event fired before actually sending out an interest
   *
   * If event returns false, then there is some kind of a problem (e.g., per-face limit reached)
   */
  virtual bool
  WillSendOutInterest (const Ptr<Face> &outgoingFace,
                       Ptr<InterestHeader> header,
                       Ptr<pit::Entry> pitEntry);

  /**
   * @brief Event fired just after sending out an interest
   */
  virtual void
  DidSendOutInterest (const Ptr<Face> &outgoingFace,
                      Ptr<InterestHeader> header,
                      const Ptr<const Packet> &packet,
                      Ptr<pit::Entry> pitEntry);

  virtual void
  PropagateInterest (const Ptr<Face> &incomingFace,
                     Ptr<InterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<pit::Entry> pitEntry);
  
  /**
   * @brief Base method to propagate the interest according to the forwarding strategy
   *
   * @param pitEntry      Reference to PIT entry (reference to corresponding FIB entry inside)
   * @param incomingFace  Incoming face
   * @param header        InterestHeader
   * @param packet        Original Interest packet
   * @param sendCallback  Send callback
   *
   * @return true if interest was successfully propagated, false if all options have failed
   */
  virtual bool
  DoPropagateInterest (const Ptr<Face> &incomingFace,
                       Ptr<InterestHeader> header,
                       const Ptr<const Packet> &packet,
                       Ptr<pit::Entry> pitEntry) = 0;
  
protected:
  // inherited from Object class                                                                                                                                                        
  virtual void NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object
  virtual void DoDispose (); ///< @brief Do cleanup
  
protected:  
  Ptr<Pit> m_pit; ///< \brief Reference to PIT to which this forwarding strategy is associated
  Ptr<Fib> m_fib; ///< \brief FIB  
  Ptr<ContentStore> m_contentStore; ///< \brief Content store (for caching purposes only)

  bool m_cacheUnsolicitedData;
  bool m_detectRetransmissions;
  
  TracedCallback<Ptr<const InterestHeader>,
                 Ptr<const Face> > m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<Ptr<const InterestHeader>,
                 Ptr<const Face> > m_inInterests; ///< @brief trace of incoming Interests

  TracedCallback<Ptr<const InterestHeader>,
                 Ptr<const Face> > m_dropInterests; ///< @brief trace of dropped Interests
  
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback<Ptr<const ContentObjectHeader>, Ptr<const Packet>,
                 bool /*from cache*/,
                 Ptr<const Face> > m_outData; ///< @brief trace of outgoing Data

  TracedCallback<Ptr<const ContentObjectHeader>, Ptr<const Packet>,
                 Ptr<const Face> > m_inData; ///< @brief trace of incoming Data

  TracedCallback<Ptr<const ContentObjectHeader>, Ptr<const Packet>,
                  Ptr<const Face> > m_dropData;  ///< @brief trace of dropped Data
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_FORWARDING_STRATEGY_H */
