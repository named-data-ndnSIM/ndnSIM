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

#ifndef CCNX_CONSUMER_H
#define CCNX_CONSUMER_H

#include "ccnx-app.h"
#include "ns3/random-variable.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "../../internet/model/rtt-estimator.h"
//#include "ns3/internet-module.h"

#include <set>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/thread/mutex.hpp>

namespace ns3 
{

/**
 * @ingroup ccnx
 * \brief CCNx application for sending out Interest packets
 */
class CcnxConsumer: public CcnxApp
{
public: 
  static TypeId GetTypeId ();
        
  /**
   * \brief Default constructor 
   * Sets up randomizer function and packet sequence number
   */
  CcnxConsumer ();

  // From CcnxApp
  // virtual void
  // OnInterest (const Ptr<const CcnxInterestHeader> &interest);

  virtual void
  OnNack (const Ptr<const CcnxInterestHeader> &interest, Ptr<Packet> packet);

  virtual void
  OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                   Ptr<Packet> payload);

  virtual void
  OnTimeout (uint32_t sequenceNumber);

  // Simulator::Schedule doesn't work with protected members???
  void
  SendPacket ();
  
protected:
  // from CcnxApp
  virtual void
  StartApplication ();

  virtual void
  StopApplication ();
  
  /**
   * \brief Constructs the Interest packet and sends it using a callback to the underlying CCNx protocol
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
  UniformVariable m_rand; // nonce generator

  uint32_t        m_seq;
  uint32_t        m_seqMax;    // maximum number of sequence number
  EventId         m_sendEvent; // Eventid of pending "send packet" event
  Time            m_retxTimer;
  EventId         m_retxEvent; // Event to check whether or not retransmission should be performed

  Ptr<RttEstimator> m_rtt;
  
  Time               m_offTime;             ///< \brief Time interval between packets
  CcnxNameComponents m_interestName;        ///< \brief CcnxName of the Interest (use CcnxNameComponents)
  Time               m_interestLifeTime;    ///< \brief LifeTime for interest packet
  int32_t            m_minSuffixComponents; ///< \brief MinSuffixComponents. See CcnxInterestHeader for more information
  int32_t            m_maxSuffixComponents; ///< \brief MaxSuffixComponents. See CcnxInterestHeader for more information
  bool               m_childSelector;       ///< \brief ChildSelector. See CcnxInterestHeader for more information
  CcnxNameComponents m_exclude;             ///< \brief Exclude. See CcnxInterestHeader for more information

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

  class i_seq { };
  class i_timestamp { }; 
  
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
  boost::mutex m_seqTimeoutsGuard;          ///< \brief mutex for safe work with the m_seqTimeouts

  /**
   * \brief A trace that is called after each transmitted Interest packet
   */
  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_transmittedInterests;
};

} // namespace ns3

#endif
