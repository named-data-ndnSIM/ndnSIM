/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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

#ifndef CCNX_DATA_QUEUE_H
#define CCNX_DATA_QUEUE_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/object.h"
#include "ns3/random-variable.h"

namespace ns3
{

class Packet;
class CcnxFace;

class CcnxDataQueue : public Object
{
public:
  static TypeId
  GetTypeId ();
  
  CcnxDataQueue ();
  
  void
  Enqueue (Ptr<Packet> packet, Ptr<CcnxFace> face);

private:
  void
  SetMaxDelay (const Time &value);

  Time
  GetMaxDelay () const;

  /**
   * \brief Forward packet from the front of the queue to the lower layers
   */
  void
  Send ();
  
private:
  EventId m_scheduledSend;
  Time m_totalWaitPeriod;
  UniformVariable m_randomPeriod;
  Time m_maxWaitPeriod;

  struct Item
  {
    Item (const Time &_gap, const Ptr<Packet> &_packet, const Ptr<CcnxFace> &face)
      : gap (_gap), packet (_packet)
    { }
    
    Time gap;
    Ptr<Packet> packet;
    Ptr<CcnxFace> face;
  };

  std::list<Item> m_queue;
};

} // namespace ns3

#endif // CCNX_DATA_QUEUE_H
