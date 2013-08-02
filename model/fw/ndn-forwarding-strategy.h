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
 */

#ifndef NDN_FORWARDING_STRATEGY_H
#define NDN_FORWARDING_STRATEGY_H

#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/object.h"
#include "ns3/traced-callback.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn
 * @defgroup ndn-fw NDN forwarding strategies
 */


/**
 * @ingroup ndn-fw
 * @brief Namespace for Forwarding Strategy operations
 */
namespace fw {
}

class Face;

class Interest;
class Data;

class Pit;
namespace pit { class Entry; }
class FibFaceMetric;
class Fib;
namespace fib { class Entry; }
class ContentStore;

/**
 * @ingroup ndn-fw
 * @brief Abstract base class for Ndn forwarding strategies
 */
class ForwardingStrategy :
    public Object
{
public:
  static TypeId GetTypeId ();

  /**
   * @brief Helper function to retrieve logging name for the forwarding strategy
   */
  static std::string GetLogName ();

  /**
   * @brief Default constructor
   */
  ForwardingStrategy ();
  virtual ~ForwardingStrategy ();

  /**
   * \brief Actual processing of incoming Ndn interests. Note, interests do not have payload
   *
   * Processing Interest packets
   * @param face     incoming face
   * @param interest Interest packet
   */
  virtual void
  OnInterest (Ptr<Face> face,
              Ptr<Interest> interest);

  /**
   * \brief Actual processing of incoming Ndn content objects
   *
   * Processing Data packets
   * @param face    incoming face
   * @param data    Data packet
   */
  virtual void
  OnData (Ptr<Face> face,
          Ptr<Data> data);

  /**
   * @brief Event fired just before PIT entry is removed by timeout
   * @param pitEntry PIT entry to be removed
   */
  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

  /**
   * @brief Event fired every time face is added to NDN stack
   * @param face face to be removed
   */
  virtual void
  AddFace (Ptr<Face> face);

  /**
   * @brief Event fired every time face is removed from NDN stack
   * @param face face to be removed
   *
   * For example, when an application terminates, AppFace is removed and this method called by NDN stack.
   */
  virtual void
  RemoveFace (Ptr<Face> face);

  /**
   * @brief Event fired every time a FIB entry is added to FIB
   * @param fibEntry FIB entry that was added
   */
  virtual void
  DidAddFibEntry (Ptr<fib::Entry> fibEntry);

  /**
   * @brief Fired just before FIB entry will be removed from FIB
   * @param fibEntry FIB entry that will be removed
   */
  virtual void
  WillRemoveFibEntry (Ptr<fib::Entry> fibEntry);

protected:
  /**
   * @brief An event that is fired every time a new PIT entry is created
   *
   * Note that if NDN node is receiving a similar interest (interest for the same name),
   * then either DidReceiveDuplicateInterest, DidSuppressSimilarInterest, or DidForwardSimilarInterest
   * will be called
   *
   * Suppression of similar Interests is controlled using ShouldSuppressIncomingInterest virtual method
   *
   * @param inFace  incoming face
   * @param header  deserialized Interest header
   * @param pitEntry created PIT entry (incoming and outgoing face sets are empty)
   *
   * @see DidReceiveDuplicateInterest, DidSuppressSimilarInterest, DidForwardSimilarInterest, ShouldSuppressIncomingInterest
   */
  virtual void
  DidCreatePitEntry (Ptr<Face> inFace,
                     Ptr<const Interest> interest,
                     Ptr<pit::Entry> pitEntry);

  /**
   * @brief An event that is fired every time a new PIT entry cannot be created (e.g., PIT container imposes a limit)
   *
   * Note that this call can be called only for non-similar Interest (i.e., there is an attempt to create a new PIT entry).
   * For any non-similar Interests, either FailedToCreatePitEntry or DidCreatePitEntry is called.
   *
   * @param inFace   incoming face
   * @param interest Interest packet
   */
  virtual void
  FailedToCreatePitEntry (Ptr<Face> inFace,
                          Ptr<const Interest> interest);

  /**
   * @brief An event that is fired every time a duplicated Interest is received
   *
   * This even is the last action that is performed before the Interest processing is halted
   *
   * @param inFace  incoming face
   * @param interest Interest packet
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   *
   * @see DidReceiveDuplicateInterest, DidSuppressSimilarInterest, DidForwardSimilarInterest, ShouldSuppressIncomingInterest
   */
  virtual void
  DidReceiveDuplicateInterest (Ptr<Face> inFace,
                               Ptr<const Interest> interest,
                               Ptr<pit::Entry> pitEntry);

  /**
   * @brief An event that is fired every time when a similar Interest is received and suppressed (collapsed)
   *
   * This even is the last action that is performed before the Interest processing is halted
   *
   * @param inFace  incoming face
   * @param interest Interest packet
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   *
   * @see DidReceiveDuplicateInterest, DidForwardSimilarInterest, ShouldSuppressIncomingInterest
   */
  virtual void
  DidSuppressSimilarInterest (Ptr<Face> inFace,
                              Ptr<const Interest> interest,
                              Ptr<pit::Entry> pitEntry);

  /**
   * @brief An event that is fired every time when a similar Interest is received and further forwarded (not suppressed/collapsed)
   *
   * This even is fired just before handling the Interest to PropagateInterest method
   *
   * @param inFace  incoming face
   * @param interest Interest packet
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   *
   * @see DidReceiveDuplicateInterest, DidSuppressSimilarInterest, ShouldSuppressIncomingInterest
   */
  virtual void
  DidForwardSimilarInterest (Ptr<Face> inFace,
                             Ptr<const Interest> interest,
                             Ptr<pit::Entry> pitEntry);

  /**
   * @brief An even that is fired when Interest cannot be forwarded
   *
   * Note that the event will not fire if  retransmission detection is enabled (by default)
   * and retransmitted Interest cannot by forwarded.  For more details, refer to the implementation.
   *
   * @param inFace  incoming face
   * @param interest Interest header
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   *
   * @see DetectRetransmittedInterest
   */
  virtual void
  DidExhaustForwardingOptions (Ptr<Face> inFace,
                               Ptr<const Interest> interest,
                               Ptr<pit::Entry> pitEntry);

  /**
   * @brief Method that implements logic to distinguish between new and retransmitted interest
   *
   * This method is called only when DetectRetransmissions attribute is set true (by default).
   *
   * Currently, the retransmission detection logic relies on the fact that list of incoming faces
   * already has inFace (i.e., a similar interest is received on the same face more than once).
   *
   * @param inFace  incoming face
   * @param interest Interest header
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   * @return true if Interest should be considered as retransmitted
   */
  virtual bool
  DetectRetransmittedInterest (Ptr<Face> inFace,
                               Ptr<const Interest> interest,
                               Ptr<pit::Entry> pitEntry);

  /**
   * @brief Even fired just before Interest will be satisfied
   *
   * Note that when Interest is satisfied from the cache, incoming face will be 0
   *
   * @param inFace  incoming face
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   */
  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

  /**
   * @brief Actual procedure to satisfy Interest
   *
   * Note that when Interest is satisfied from the cache, incoming face will be 0
   *
   * @param inFace  incoming face
   * @param data    Data packet
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   */
  virtual void
  SatisfyPendingInterest (Ptr<Face> inFace, // 0 allowed (from cache)
                          Ptr<const Data> data,
                          Ptr<pit::Entry> pitEntry);

  /**
   * @brief Event which is fired just after data was send out on the face
   *
   * @param inFace   incoming face of the Data
   * @param outFace  outgoing face
   * @param data     Data packet
   * @param pitEntry an existing PIT entry, corresponding to the duplicated Interest
   */
  virtual void
  DidSendOutData (Ptr<Face> inFace,
                  Ptr<Face> outFace,
                  Ptr<const Data> data,
                  Ptr<pit::Entry> pitEntry);

  /**
   * @brief Event which is fired every time a requested (solicited) DATA packet (there is an active PIT entry) is received
   *
   * @param inFace  incoming face
   * @param data    Data packet
   * @param didCreateCacheEntry flag indicating whether a cache entry was added for this data packet or not (e.g., packet already exists in cache)
   */
  virtual void
  DidReceiveSolicitedData (Ptr<Face> inFace,
                           Ptr<const Data> data,
                           bool didCreateCacheEntry);

  /**
   * @brief Event which is fired every time an unsolicited DATA packet (no active PIT entry) is received
   *
   * The current implementation allows ignoring unsolicited DATA (by default), or cache it by setting
   * attribute CacheUnsolicitedData true
   *
   * @param inFace  incoming face
   * @param data    Data packet
   * @param didCreateCacheEntry flag indicating whether a cache entry was added for this data packet or not (e.g., packet already exists in cache)
   */
  virtual void
  DidReceiveUnsolicitedData (Ptr<Face> inFace,
                             Ptr<const Data> data,
                             bool didCreateCacheEntry);

  /**
   * @brief Method implementing logic to suppress (collapse) similar Interests
   *
   * In the base class implementation this method checks list of incoming/outgoing faces of the PIT entry
   * (for new Intersets, both lists are empty before this call)
   *
   * For more details, refer to the source code.
   *
   * @param inFace  incoming face
   * @param interest Interest packet
   * @param payload Data payload
   */
  virtual bool
  ShouldSuppressIncomingInterest (Ptr<Face> inFace,
                                  Ptr<const Interest> interest,
                                  Ptr<pit::Entry> pitEntry);

  /**
   * @brief Method to check whether Interest can be send out on the particular face or not
   *
   * In the base class, this method perfoms two checks:
   * 1. If inFace is equal to outFace (when equal, Interest forwarding is prohibited)
   * 2. Whether Interest should be suppressed (list of outgoing faces include outFace),
   * considering (if enabled) retransmission logic
   *
   * @param inFace     incoming face of the Interest
   * @param outFace    proposed outgoing face of the Interest
   * @param interest   Interest packet
   * @param pitEntry   reference to PIT entry (reference to corresponding FIB entry inside)
   *
   * @see DetectRetransmittedInterest
   */
  virtual bool
  CanSendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const Interest> interest,
                      Ptr<pit::Entry> pitEntry);

  /**
   * @brief Method implementing actual interest forwarding, taking into account CanSendOutInterest decision
   *
   * If event returns false, then there is some kind of a problem exists
   *
   * @param inFace     incoming face of the Interest
   * @param outFace    proposed outgoing face of the Interest
   * @param interest Interest packet
   * @param pitEntry   reference to PIT entry (reference to corresponding FIB entry inside)
   *
   * @see CanSendOutInterest
   */
  virtual bool
  TrySendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const Interest> interest,
                      Ptr<pit::Entry> pitEntry);

  /**
   * @brief Event fired just after forwarding the Interest
   *
   * @param inFace     incoming face of the Interest
   * @param outFace    outgoing face of the Interest
   * @param interest Interest packet
   * @param pitEntry   reference to PIT entry (reference to corresponding FIB entry inside)
   */
  virtual void
  DidSendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const Interest> interest,
                      Ptr<pit::Entry> pitEntry);

  /**
   * @brief Wrapper method, which performs general tasks and calls DoPropagateInterest method
   *
   * General tasks so far are adding face to the list of incoming face, updating
   * PIT entry lifetime, calling DoPropagateInterest, and retransmissions (enabled by default).
   *
   * @param inFace     incoming face
   * @param interest   Interest packet
   * @param pitEntry   reference to PIT entry (reference to corresponding FIB entry inside)
   *
   * @see DoPropagateInterest
   */
  virtual void
  PropagateInterest (Ptr<Face> inFace,
                     Ptr<const Interest> interest,
                     Ptr<pit::Entry> pitEntry);

  /**
   * @brief Virtual method to perform Interest propagation according to the forwarding strategy logic
   *
   * In most cases, this is the call that needs to be implemented/re-implemented in order
   * to perform forwarding of Interests according to the desired logic.
   *
   * There is also PropagateInterest method (generally, do not require to be overriden)
   * which performs general tasks (adding face to the list of incoming face, updating
   * PIT entry lifetime, calling DoPropagateInterest, as well as perform retransmissions (enabled by default).
   *
   * @param inFace     incoming face
   * @param interest   Interest packet
   * @param pitEntry   reference to PIT entry (reference to corresponding FIB entry inside)
   *
   * @return true if interest was successfully propagated, false if all options have failed
   *
   * @see PropagateInterest
   */
  virtual bool
  DoPropagateInterest (Ptr<Face> inFace,
                       Ptr<const Interest> interest,
                       Ptr<pit::Entry> pitEntry) = 0;

protected:
  // inherited from Object class
  virtual void NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object
  virtual void DoDispose (); ///< @brief Do cleanup

protected:
  Ptr<Pit> m_pit; ///< \brief Reference to PIT to which this forwarding strategy is associated
  Ptr<Fib> m_fib; ///< \brief FIB
  Ptr<ContentStore> m_contentStore; ///< \brief Content store (for caching purposes only)

  bool m_cacheUnsolicitedDataFromApps;
  bool m_cacheUnsolicitedData;
  bool m_detectRetransmissions;

  TracedCallback<Ptr<const Interest>,
                 Ptr<const Face> > m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<Ptr<const Interest>,
                 Ptr<const Face> > m_inInterests; ///< @brief trace of incoming Interests

  TracedCallback<Ptr<const Interest>,
                 Ptr<const Face> > m_dropInterests; ///< @brief trace of dropped Interests

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback<Ptr<const Data>,
                 bool /*from cache*/,
                 Ptr<const Face> > m_outData; ///< @brief trace of outgoing Data

  TracedCallback<Ptr<const Data>,
                 Ptr<const Face> > m_inData; ///< @brief trace of incoming Data

  TracedCallback<Ptr<const Data>,
                  Ptr<const Face> > m_dropData;  ///< @brief trace of dropped Data

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  TracedCallback< Ptr<const pit::Entry> > m_satisfiedInterests;
  TracedCallback< Ptr<const pit::Entry> > m_timedOutInterests;
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_FORWARDING_STRATEGY_H */
