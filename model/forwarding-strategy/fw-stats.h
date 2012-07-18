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


#ifndef NDNSIM_FW_STATS_H
#define NDNSIM_FW_STATS_H

#include "best-route.h"
#include "../../utils/stats-tree.h"

namespace ns3 {
namespace ndnSIM {

/**
 * \ingroup ccnx
 * \brief Best route strategy
 */
class FwStats :
    public BestRoute
{
public:
  static TypeId
  GetTypeId (void);

  /**
   * @brief Default constructor
   */
  FwStats ();

protected:
  virtual void
  DidCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                     Ptr<CcnxInterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<CcnxPitEntry> pitEntry);

  virtual void
  WillSatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                              Ptr<CcnxPitEntry> pitEntry);

  virtual void
  DidSendOutInterest (const Ptr<CcnxFace> &outgoingFace,
                      Ptr<CcnxInterestHeader> header,
                      Ptr<CcnxPitEntry> pitEntry);

  virtual void
  WillErasePendingInterest (Ptr<CcnxPitEntry> pitEntry);
    
private:
  ::ndnSIM::StatsTree m_stats;
  
  typedef BestRoute super;
};

} // namespace ndnSIM
} // namespace ns3

#endif // NDNSIM_FW_STATS_H
