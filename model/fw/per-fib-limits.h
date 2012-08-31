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
#include "../../utils/ndn-pit-queue.h"

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
private:
  typedef FwStats super;

public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  PerFibLimits ();

  virtual void
  OnInterest (Ptr<Face> face,
              Ptr<const InterestHeader> header,
              Ptr<const Packet> origPacket);
  
  virtual void
  WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry);

  virtual void
  RemoveFace (Ptr<Face> face);
  
protected:
  virtual bool
  TrySendOutInterest (Ptr<Face> inFace,
                      Ptr<Face> outFace,
                      Ptr<const InterestHeader> header,
                      Ptr<const Packet> origPacket,
                      Ptr<pit::Entry> pitEntry);
  
  virtual void
  WillSatisfyPendingInterest (Ptr<Face> inFace,
                              Ptr<pit::Entry> pitEntry);

  virtual void
  DidReceiveValidNack (Ptr<Face> inFace,
                       uint32_t nackCode,
                       Ptr<pit::Entry> pitEntry);
  
private:
  void
  ProcessFromQueue ();

  void
  AnnounceLimits ();

  void
  ApplyAnnouncedLimit (Ptr<Face> inFace,
                       Ptr<const InterestHeader> header);
  
  // from Object
  virtual void
  NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object

  virtual void
  DoDispose ();
    
private:
  typedef std::map< Ptr<Face>, PitQueue > PitQueueMap;
  PitQueueMap m_pitQueues; // per-outgoing face pit queue

  bool    m_announceLimits;

  EventId m_announceEvent;
};


} // namespace fw
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_PER_FIB_LIMITS_H
