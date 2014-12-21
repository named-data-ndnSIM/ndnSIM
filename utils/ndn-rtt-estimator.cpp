/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//

// THIS IS A COPY OF rtt-estimator.cc from internet module with minor modifications

// Ported from:
// Georgia Tech Network Simulator - Round Trip Time Estimation Class
// George F. Riley.  Georgia Tech, Spring 2002

// Implements several variations of round trip time estimators

#include <iostream>

#include "ndn-rtt-estimator.hpp"
#include "ns3/simulator.h"
#include "ns3/double.h"
#include "ns3/integer.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE("ndn.RttEstimator");

namespace ns3 {

namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(RttEstimator);

TypeId
RttEstimator::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::RttEstimator")
      .SetParent<Object>()
      .AddAttribute("MaxMultiplier", "Maximum RTO Multiplier", UintegerValue(64),
                    MakeUintegerAccessor(&RttEstimator::m_maxMultiplier),
                    MakeUintegerChecker<uint16_t>())
      .AddAttribute("InitialEstimation", "Initial RTT estimation", TimeValue(Seconds(1.0)),
                    MakeTimeAccessor(&RttEstimator::m_initialEstimatedRtt), MakeTimeChecker())
      .AddAttribute("MinRTO", "Minimum retransmit timeout value",
                    TimeValue(
                      Seconds(0.2)), // RFC2988 says min RTO=1 sec, but Linux uses 200ms. See
                    // http://www.postel.org/pipermail/end2end-interest/2004-November/004402.html
                    MakeTimeAccessor(&RttEstimator::SetMinRto, &RttEstimator::GetMinRto),
                    MakeTimeChecker())
      .AddAttribute("MaxRTO", "Maximum retransmit timeout value", TimeValue(Seconds(200.0)),
                    MakeTimeAccessor(&RttEstimator::SetMaxRto, &RttEstimator::GetMaxRto),
                    MakeTimeChecker());
  return tid;
}

void
RttEstimator::SetMinRto(Time minRto)
{
  NS_LOG_FUNCTION(this << minRto);
  m_minRto = minRto;
}
Time
RttEstimator::GetMinRto(void) const
{
  return m_minRto;
}

void
RttEstimator::SetMaxRto(Time maxRto)
{
  NS_LOG_FUNCTION(this << maxRto);
  m_maxRto = maxRto;
}
Time
RttEstimator::GetMaxRto(void) const
{
  return m_maxRto;
}

void
RttEstimator::SetCurrentEstimate(Time estimate)
{
  NS_LOG_FUNCTION(this << estimate);
  m_currentEstimatedRtt = estimate;
}
Time
RttEstimator::GetCurrentEstimate(void) const
{
  return m_currentEstimatedRtt;
}

// RttHistory methods
RttHistory::RttHistory(SequenceNumber32 s, uint32_t c, Time t)
  : seq(s)
  , count(c)
  , time(t)
  , retx(false)
{
  NS_LOG_FUNCTION(this);
}

RttHistory::RttHistory(const RttHistory& h)
  : seq(h.seq)
  , count(h.count)
  , time(h.time)
  , retx(h.retx)
{
  NS_LOG_FUNCTION(this);
}

// Base class methods

RttEstimator::RttEstimator()
  : m_next(1)
  , m_nSamples(0)
  , m_multiplier(1)
  , m_history()
{
  NS_LOG_FUNCTION(this);
  // note next=1 everywhere since first segment will have sequence 1

  // We need attributes initialized here, not later, so use the
  // ConstructSelf() technique documented in the manual
  ObjectBase::ConstructSelf(AttributeConstructionList());
  m_currentEstimatedRtt = m_initialEstimatedRtt;
  NS_LOG_DEBUG("Initialize m_currentEstimatedRtt to " << m_currentEstimatedRtt.GetSeconds()
                                                      << " sec.");
}

RttEstimator::RttEstimator(const RttEstimator& c)
  : Object(c)
  , m_next(c.m_next)
  , m_maxMultiplier(c.m_maxMultiplier)
  , m_initialEstimatedRtt(c.m_initialEstimatedRtt)
  , m_currentEstimatedRtt(c.m_currentEstimatedRtt)
  , m_minRto(c.m_minRto)
  , m_maxRto(c.m_maxRto)
  , m_nSamples(c.m_nSamples)
  , m_multiplier(c.m_multiplier)
  , m_history(c.m_history)
{
  NS_LOG_FUNCTION(this);
}

RttEstimator::~RttEstimator()
{
  NS_LOG_FUNCTION(this);
}

TypeId
RttEstimator::GetInstanceTypeId(void) const
{
  return GetTypeId();
}

void
RttEstimator::SentSeq(SequenceNumber32 seq, uint32_t size)
{
  NS_LOG_FUNCTION(this << seq << size);
  // Note that a particular sequence has been sent
  if (seq == m_next) { // This is the next expected one, just log at end
    m_history.push_back(RttHistory(seq, size, Simulator::Now()));
    m_next = seq + SequenceNumber32(size); // Update next expected
  }
  else { // This is a retransmit, find in list and mark as re-tx
    for (RttHistory_t::iterator i = m_history.begin(); i != m_history.end(); ++i) {
      if ((seq >= i->seq) && (seq < (i->seq + SequenceNumber32(i->count)))) { // Found it
        i->retx = true;
        // One final test..be sure this re-tx does not extend "next"
        if ((seq + SequenceNumber32(size)) > m_next) {
          m_next = seq + SequenceNumber32(size);
          i->count = ((seq + SequenceNumber32(size)) - i->seq); // And update count in hist
        }
        break;
      }
    }
  }
}

Time
RttEstimator::AckSeq(SequenceNumber32 ackSeq)
{
  NS_LOG_FUNCTION(this << ackSeq);
  // An ack has been received, calculate rtt and log this measurement
  // Note we use a linear search (O(n)) for this since for the common
  // case the ack'ed packet will be at the head of the list
  Time m = Seconds(0.0);
  if (m_history.size() == 0)
    return (m); // No pending history, just exit
  RttHistory& h = m_history.front();
  if (!h.retx && ackSeq >= (h.seq + SequenceNumber32(h.count))) { // Ok to use this sample
    m = Simulator::Now() - h.time;                                // Elapsed time
    Measurement(m);                                               // Log the measurement
    ResetMultiplier(); // Reset multiplier on valid measurement
  }
  // Now delete all ack history with seq <= ack
  while (m_history.size() > 0) {
    RttHistory& h = m_history.front();
    if ((h.seq + SequenceNumber32(h.count)) > ackSeq)
      break;               // Done removing
    m_history.pop_front(); // Remove
  }
  return m;
}

void
RttEstimator::ClearSent()
{
  NS_LOG_FUNCTION(this);
  // Clear all history entries
  m_next = 1;
  m_history.clear();
}

void
RttEstimator::IncreaseMultiplier()
{
  NS_LOG_FUNCTION(this);
  m_multiplier = (m_multiplier * 2 < m_maxMultiplier) ? m_multiplier * 2 : m_maxMultiplier;
  NS_LOG_DEBUG("Multiplier increased to " << m_multiplier);
}

void
RttEstimator::ResetMultiplier()
{
  NS_LOG_FUNCTION(this);
  m_multiplier = 1;
}

void
RttEstimator::Reset()
{
  NS_LOG_FUNCTION(this);
  // Reset to initial state
  m_next = 1;
  m_currentEstimatedRtt = m_initialEstimatedRtt;
  m_history.clear(); // Remove all info from the history
  m_nSamples = 0;
  ResetMultiplier();
}

} // namespace ndn
} // namespace ns3
