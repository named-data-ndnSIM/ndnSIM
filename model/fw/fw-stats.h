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
namespace ndn {
namespace fw {

/**
 * \ingroup ndn
 * \brief Strategy based on best route and adding statistics gathering capabilities
 */
class FwStats :
    public BestRoute
{
public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  FwStats ();

  inline
  const ndnSIM::StatsTree &
  GetStatsTree () const;  

  virtual void
  OnInterest (Ptr<Face> face,
              Ptr<const InterestHeader> header,
              Ptr<const Packet> origPacket);

  virtual void
  OnData (Ptr<Face> face,
          Ptr<const ContentObjectHeader> header,
          Ptr<Packet> payload,
          Ptr<const Packet> origPacket);

  virtual void
  RemoveFace (Ptr<Face> face);

protected:
  virtual void
  DidCreatePitEntry (Ptr<Face> inFace,
                     Ptr<const InterestHeader> header,
                     Ptr<const Packet> packet,
                     Ptr<pit::Entry> pitEntry);

  virtual void
  FailedToCreatePitEntry (Ptr<Face> inFace,
                          Ptr<const InterestHeader> header,
                          Ptr<const Packet> packet);

  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

  virtual void
  DidSendOutInterest (Ptr<Face> outFace,
                      Ptr<const InterestHeader> header,
                      Ptr<const Packet> origPacket,
                      Ptr<pit::Entry> pitEntry);

  virtual void
  DidSendOutData (Ptr<Face> outFace,
                  Ptr<const ContentObjectHeader> header,
                  Ptr<const Packet> payload,
                  Ptr<const Packet> origPacket,
                  Ptr<pit::Entry> pitEntry);

  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

  virtual void
  DidExhaustForwardingOptions (Ptr<Face> inFace,
                               Ptr<const InterestHeader> header,
                               Ptr<const Packet> origPacket,
                               Ptr<pit::Entry> pitEntry);
  
  // from Object
  void
  DoDispose ();
  
private:
  void
  RefreshStats ();

  void
  ScheduleRefreshingIfNecessary ();
  
private:
  ndnSIM::StatsTree m_stats;
  EventId m_statsRefreshEvent;

  TracedCallback< Ptr<ForwardingStrategy>,
                  const ndnSIM::StatsTree & > m_statsTrace;
  
  typedef BestRoute super;
};

const ndnSIM::StatsTree &
FwStats::GetStatsTree () const
{
  return m_stats;
}

} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_FW_STATS_H
