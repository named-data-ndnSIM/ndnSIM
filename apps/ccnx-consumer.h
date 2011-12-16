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
 */

#ifndef CCNX_CONSUMER_H
#define CCNX_CONSUMER_H

#include "ccnx-app.h"
#include "ns3/random-variable.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/nstime.h"

#include <set>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/thread/mutex.hpp>

namespace ns3 
{

class CcnxConsumer: public CcnxApp
{
public: 
  static TypeId GetTypeId ();
        
  CcnxConsumer ();

  // From CcnxApp
  // virtual void
  // OnInterest (const Ptr<const CcnxInterestHeader> &interest);

  virtual void
  OnNack (const Ptr<const CcnxInterestHeader> &interest);

  virtual void
  OnContentObject (const Ptr<const CcnxContentObjectHeader> &contentObject,
                   const Ptr<const Packet> &payload);

protected:
  // from CcnxApp
  virtual void
  StartApplication ();

  virtual void
  StopApplication ();
  
private:
  //helpers
  void
  SendPacket ();

  void
  CheckRetxTimeout ();
  
  void
  SetRetxTimer (Time retxTimer);

  Time
  GetRetxTimer () const;
  
protected:
  UniformVariable m_rand;
  uint32_t        m_seq;
  EventId         m_sendEvent; // Eventid of pending "send packet" event
  Time            m_retxTimer;
  EventId         m_retxEvent; // Event to check whether or not retransmission should be performed

  Time            m_rto; // Retransmission timeout
  Time            m_rttVar; // RTT variance
  Time            m_sRtt; // smoothed RTT
  
  Time               m_offTime;
  CcnxNameComponents m_interestName;
  Time               m_interestLifeTime;
  int32_t            m_minSuffixComponents;
  int32_t            m_maxSuffixComponents;
  bool               m_childSelector;
  CcnxNameComponents m_exclude;

  struct RetxSeqsContainer :
    public std::set<uint32_t> { };
  
  RetxSeqsContainer m_retxSeqs; // ordered set of sequence numbers to be retransmitted

  struct SeqTimeout
  {
    SeqTimeout (uint32_t _seq, Time _time) : seq (_seq), time (_time) { }
    
    uint32_t seq;
    Time time;
  };

  class i_seq { };
  class i_timestamp { }; 
  
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

  SeqTimeoutsContainer m_seqTimeouts;
  boost::mutex m_seqTimeoutsGuard;

  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<CcnxApp>, Ptr<CcnxFace> > m_transmittedInterests;
};

} // namespace ns3

#endif
