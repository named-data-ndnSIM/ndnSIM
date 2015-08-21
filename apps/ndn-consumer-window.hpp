/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef NDN_CONSUMER_WINDOW_H
#define NDN_CONSUMER_WINDOW_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-consumer.hpp"
#include "ns3/traced-value.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief Ndn application for sending out Interest packets (window-based)
 *
 * !!! ATTENTION !!! This is highly experimental and relies on experimental features of the
 *simulator.
 * Behavior may be unpredictable if used incorrectly.
 */
class ConsumerWindow : public Consumer {
public:
  static TypeId
  GetTypeId();

  /**
   * \brief Default constructor
   */
  ConsumerWindow();

  // From App
  virtual void
  OnData(shared_ptr<const Data> contentObject);

  virtual void
  OnTimeout(uint32_t sequenceNumber);

  virtual void
  WillSendOutInterest(uint32_t sequenceNumber);

public:
  typedef void (*WindowTraceCallback)(uint32_t);

protected:
  /**
   * \brief Constructs the Interest packet and sends it using a callback to the underlying NDN
   * protocol
   */
  virtual void
  ScheduleNextPacket();

private:
  virtual void
  SetWindow(uint32_t window);

  uint32_t
  GetWindow() const;

  virtual void
  SetPayloadSize(uint32_t payload);

  uint32_t
  GetPayloadSize() const;

  double
  GetMaxSize() const;

  void
  SetMaxSize(double size);

  uint32_t
  GetSeqMax() const;

  void
  SetSeqMax(uint32_t seqMax);

private:
  uint32_t m_payloadSize; // expected payload size
  double m_maxSize;       // max size to request

  uint32_t m_initialWindow;
  bool m_setInitialWindowOnTimeout;

  TracedValue<uint32_t> m_window;
  TracedValue<uint32_t> m_inFlight;
};

} // namespace ndn
} // namespace ns3

#endif
