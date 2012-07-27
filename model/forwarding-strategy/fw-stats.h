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

#include "ns3/event-id.h"

#include "best-route.h"
#include "../../utils/stats-tree.h"

namespace ns3 {
namespace ndnSIM {

/**
 * \ingroup ccnx
 * \brief Strategy based on best route and adding statistics gathering capabilities
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

  virtual void
  OnInterest (const Ptr<CcnxFace> &face,
              Ptr<CcnxInterestHeader> &header,
              const Ptr<const Packet> &p);

  virtual void
  OnData (const Ptr<CcnxFace> &face,
          Ptr<CcnxContentObjectHeader> &header,
          Ptr<Packet> &payload,
          const Ptr<const Packet> &packet);

  virtual void
  RemoveFace (Ptr<CcnxFace> face);

protected:
  virtual void
  DidCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                     Ptr<CcnxInterestHeader> header,
                     const Ptr<const Packet> &packet,
                     Ptr<CcnxPitEntry> pitEntry);

  virtual void
  FailedToCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                          Ptr<CcnxInterestHeader> header,
                          const Ptr<const Packet> &packet);

  virtual void
  WillSatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                              Ptr<CcnxPitEntry> pitEntry);

  virtual void
  DidSendOutInterest (const Ptr<CcnxFace> &outgoingFace,
                      Ptr<CcnxInterestHeader> header,
                      const Ptr<const Packet> &packet,
                      Ptr<CcnxPitEntry> pitEntry);

  virtual void
  DidSendOutData (const Ptr<CcnxFace> &face,
                  Ptr<const CcnxContentObjectHeader> header,
                  Ptr<const Packet> payload,
                  const Ptr<const Packet> &packet);

  virtual void
  WillErasePendingInterest (Ptr<CcnxPitEntry> pitEntry);

  // from Object
  void
  DoDispose ();
  
private:
  void
  RefreshStats ();

  void
  ScheduleRefreshingIfNecessary ();
  
private:
  ::ndnSIM::StatsTree m_stats;
  EventId m_statsRefreshEvent;

  TracedCallback< Ptr<CcnxForwardingStrategy>,
                  const ::ndnSIM::StatsTree & > m_statsTrace;
  
  typedef BestRoute super;
};

} // namespace ndnSIM
} // namespace ns3

#endif // NDNSIM_FW_STATS_H
