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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_CONSUMER_H
#define NDN_CONSUMER_H

#include "ndn-app.h"
#include "ns3/random-variable.h"
#include "ns3/ndn-name.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ndn-rtt-estimator.h"

#include <set>
#include <map>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn
 * \brief NDN application for sending out Interest packets
 */
class Consumer: public App
{
public:
  static TypeId GetTypeId ();

  /**
   * \brief Default constructor
   * Sets up randomizer function and packet sequence number
   */
  Consumer ();
  virtual ~Consumer () {};

  // From App
  // virtual void
  // OnInterest (const Ptr<const Interest> &interest);

  virtual void
  OnNack (const Ptr<const Interest> &interest, Ptr<Packet> packet);

  virtual void
  OnContentObject (const Ptr<const ContentObject> &contentObject,
                   Ptr<Packet> payload);

  /**
   * @brief Timeout event
   * @param sequenceNumber time outed sequence number
   */
  virtual void
  OnTimeout (uint32_t sequenceNumber);

  /**
   * @brief Actually send packet
   */
  void
  SendPacket ();

  /**
   * @brief An event that is fired just before an Interest packet is actually send out (send is inevitable)
   *
   * The reason for "before" even is that in certain cases (when it is possible to satisfy from the local cache),
   * the send call will immediately return data, and if "after" even was used, this after would be called after
   * all processing of incoming data, potentially producing unexpected results.
   */
  virtual void
  WillSendOutInterest (uint32_t sequenceNumber);
  
protected:
  // from App
  virtual void
  StartApplication ();

  virtual void
  StopApplication ();

  /**
   * \brief Constructs the Interest packet and sends it using a callback to the underlying NDN protocol
   */
  virtual void
  ScheduleNextPacket () = 0;

  /**
   * \brief Checks if the packet need to be retransmitted becuase of retransmission timer expiration
   */
  void
  CheckRetxTimeout ();

  /**
   * \brief Modifies the frequency of checking the retransmission timeouts
   * \param retxTimer Timeout defining how frequent retransmission timeouts should be checked
   */
  void
  SetRetxTimer (Time retxTimer);

  /**
   * \brief Returns the frequency of checking the retransmission timeouts
   * \return Timeout defining how frequent retransmission timeouts should be checked
   */
  Time
  GetRetxTimer () const;

protected:
  UniformVariable m_rand; ///< @brief nonce generator

  uint32_t        m_seq;  ///< @brief currently requested sequence number
  uint32_t        m_seqMax;    ///< @brief maximum number of sequence number
  EventId         m_sendEvent; ///< @brief EventId of pending "send packet" event
  Time            m_retxTimer; ///< @brief Currently estimated retransmission timer
  EventId         m_retxEvent; ///< @brief Event to check whether or not retransmission should be performed

  Ptr<RttEstimator> m_rtt; ///< @brief RTT estimator

  Time               m_offTime;             ///< \brief Time interval between packets
  Name     m_interestName;        ///< \brief NDN Name of the Interest (use Name)
  Time               m_interestLifeTime;    ///< \brief LifeTime for interest packet

/// @cond include_hidden
  /**
   * \struct This struct contains sequence numbers of packets to be retransmitted
   */
  struct RetxSeqsContainer :
    public std::set<uint32_t> { };

  RetxSeqsContainer m_retxSeqs;             ///< \brief ordered set of sequence numbers to be retransmitted

  /**
   * \struct This struct contains a pair of packet sequence number and its timeout
   */
  struct SeqTimeout
  {
    SeqTimeout (uint32_t _seq, Time _time) : seq (_seq), time (_time) { }

    uint32_t seq;
    Time time;
  };
/// @endcond

/// @cond include_hidden
  class i_seq { };
  class i_timestamp { };
/// @endcond

/// @cond include_hidden
  /**
   * \struct This struct contains a multi-index for the set of SeqTimeout structs
   */
  struct SeqTimeoutsContainer :
    public boost::multi_index::multi_index_container<
    SeqTimeout,
    boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<i_seq>,
        boost::multi_index::member<SeqTimeout, uint32_t, &SeqTimeout::seq>
        >,
      boost::multi_index::ordered_non_unique<
        boost::multi_index::tag<i_timestamp>,
        boost::multi_index::member<SeqTimeout, Time, &SeqTimeout::time>
        >
      >
    > { } ;

  SeqTimeoutsContainer m_seqTimeouts;       ///< \brief multi-index for the set of SeqTimeout structs

  SeqTimeoutsContainer m_seqLastDelay;
  SeqTimeoutsContainer m_seqFullDelay;
  std::map<uint32_t, uint32_t> m_seqRetxCounts;

  TracedCallback<Ptr<App> /* app */, uint32_t /* seqno */,
                 Time /* delay */, int32_t /*hop count*/> m_lastRetransmittedInterestDataDelay;
  TracedCallback<Ptr<App> /* app */, uint32_t /* seqno */,
                 Time /* delay */, uint32_t /*retx count*/,
                 int32_t /*hop count*/> m_firstInterestDataDelay;

/// @endcond
};

} // namespace ndn
} // namespace ns3

#endif
