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


#ifndef NDNSIM_STATS_BASED_RANDOMIZED_INTEREST_ACCEPT_H
#define NDNSIM_STATS_BASED_RANDOMIZED_INTEREST_ACCEPT_H

#include "ns3/event-id.h"

#include "fw-stats.h"

namespace ns3 {
namespace ndn {
namespace fw {

/**
 * \ingroup ndn
 * \brief Strategy implementing stats-based randomized accept for interests
 *
 * (prerequisite) Window-based limits should be enabled
 *
 * This strategy has several limitation to accept interest for further processing:
 * - if per-fib-entry limit or per-face limit is exhausted, Interest will not be accepted
 * - if the number of interests received from the incoming face is less than threshold, then no special handling
 * - if this number is greater than threshold, an Interest will be accepted with probability equal to satisfaction ratio for this incoming face (overall per-face).
 * (probability is shifted to allow small rate of acceptance (1% by default) of Interests from faces with 0 satisfaction ratio.
 */
class StatsBasedRandomizedInterestAccept :
    public FwStats
{
public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  StatsBasedRandomizedInterestAccept ();

  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

protected:
  virtual bool
  WillSendOutInterest (Ptr<Face> outFace,
                       Ptr<const InterestHeader> header,
                       Ptr<pit::Entry> pitEntry);
  
  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

  // from Object
  void
  DoDispose ();
  
private:
  double m_threshold;
  double m_graceAcceptProbability;

  typedef FwStats super;
};


} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_STATS_BASED_RANDOMIZED_INTEREST_ACCEPT_H
