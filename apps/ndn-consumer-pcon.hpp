/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2018  Regents of the University of California.
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

#ifndef NDN_CONSUMER_PCON_H
#define NDN_CONSUMER_PCON_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-consumer-window.hpp"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * \brief NDN consumer application (uses a PCON congestion window)
 *
 * Please refer to ConsumerWindow for further documentation on how to use this consumer.
 */
class ConsumerPcon : public ConsumerWindow {
public:
  static TypeId
  GetTypeId();

  ConsumerPcon();

  virtual void
  OnData(shared_ptr<const Data> data) override;

  virtual void
  OnTimeout(uint32_t sequenceNum) override;

private:
  void
  WindowIncrease();

  void
  WindowDecrease();

private:
  double m_beta;
  double m_addRttSupress;
  bool m_shouldReactToCongestionMarks;
  bool m_shouldUseCwa;

  double m_ssthresh;
  uint32_t m_highData;
  double m_recPoint;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_CONSUMER_PCON_H
