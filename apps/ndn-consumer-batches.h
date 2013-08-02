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

#ifndef NDN_CONSUMER_BATCHES_H
#define NDN_CONSUMER_BATCHES_H

#include "ndn-consumer.h"
#include "ns3/traced-value.h"
#include "../utils/batches.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief Ndn application for sending out Interest packets in batches
 */
class ConsumerBatches: public Consumer
{
public: 
  static TypeId GetTypeId ();
        
  /**
   * \brief Default constructor 
   */
  ConsumerBatches ();

  // From App
  // virtual void
  // OnInterest (const Ptr<const Interest> &interest);

  // virtual void
  // OnNack (const Ptr<const Interest> &interest);

  // virtual void
  // OnData (const Ptr<const Data> &contentObject,
  //                  const Ptr<const Packet> &payload);

  // virtual void
  // OnTimeout (uint32_t sequenceNumber);

private:
  virtual void
  StartApplication ();    ///< @brief Called at time specified by Start
  
  void
  AddBatch (uint32_t amount);
protected:
  /**
   * \brief Constructs the Interest packet and sends it using a callback to the underlying NDN protocol
   */
  virtual void
  ScheduleNextPacket ();

private:
  bool m_initial;

  Batches m_batches;
};

} // namespace ndn
} // namespace ns3

#endif
