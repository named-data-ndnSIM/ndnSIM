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
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_CONSUMER_WINDOW_H
#define NDN_CONSUMER_WINDOW_H

#include "ndn-consumer.h"
#include "ns3/traced-value.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief Ndn application for sending out Interest packets (window-based)
 *
 * !!! ATTENTION !!! This is highly experimental and relies on experimental features of the simulator.
 * Behavior may be unpredictable if used incorrectly.
 */
class ConsumerWindow: public Consumer
{
public:
  static TypeId GetTypeId ();

  /**
   * \brief Default constructor
   */
  ConsumerWindow ();

  // From App
  // virtual void
  // OnInterest (const Ptr<const Interest> &interest);

  virtual void
  OnNack (Ptr<const Interest> interest);

  virtual void
  OnData (Ptr<const Data> contentObject);

  virtual void
  OnTimeout (uint32_t sequenceNumber);

  virtual void
  WillSendOutInterest (uint32_t sequenceNumber);

protected:
  /**
   * \brief Constructs the Interest packet and sends it using a callback to the underlying NDN protocol
   */
  virtual void
  ScheduleNextPacket ();

private:
  virtual void
  SetWindow (uint32_t window);

  uint32_t
  GetWindow () const;

  virtual void
  SetPayloadSize (uint32_t payload);

  uint32_t
  GetPayloadSize () const;

  double
  GetMaxSize () const;

  void
  SetMaxSize (double size);

  uint32_t
  GetSeqMax () const;

  void
  SetSeqMax (uint32_t seqMax);

private:
  uint32_t m_payloadSize; // expected payload size
  double   m_maxSize; // max size to request

  uint32_t m_initialWindow;
  bool m_setInitialWindowOnTimeout;

  TracedValue<uint32_t> m_window;
  TracedValue<uint32_t> m_inFlight;
};

} // namespace ndn
} // namespace ns3

#endif
