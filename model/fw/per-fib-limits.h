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


#ifndef NDNSIM_PER_FIB_LIMITS_H
#define NDNSIM_PER_FIB_LIMITS_H

#include "ns3/event-id.h"

#include "fw-stats.h"

namespace ns3 {
namespace ndn {
namespace fw {

/**
 * \ingroup ndn
 * \brief Strategy implementing per-FIB entry limits
 */
class PerFibLimits :
    public FwStats
{
public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  PerFibLimits ();

  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

protected:
  virtual bool
  WillSendOutInterest (const Ptr<Face> &outgoingFace,
                       Ptr<InterestHeader> header,
                       Ptr<pit::Entry> pitEntry);
  
  virtual void
  WillSatisfyPendingInterest (const Ptr<Face> &incomingFace,
                              Ptr<pit::Entry> pitEntry);

  // from Object
  void
  DoDispose ();
  
private:
  void
  DecayLimits ();
  
private:
  EventId m_decayLimitsEvent;

  double m_threshold;
  double m_graceAcceptProbability;

  typedef FwStats super;
};


} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_PER_FIB_LIMITS_H
