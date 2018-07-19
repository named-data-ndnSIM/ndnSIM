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

enum CcAlgorithm {
  AIMD,
  BIC,
  CUBIC
};

/**
 * @ingroup ndn-apps
 * \brief NDN consumer application with more advanced congestion control options
 *
 * This app uses the algorithms from "A Practical Congestion Control Scheme for Named
 * Data Networking" (https://dl.acm.org/citation.cfm?id=2984369).
 *
 * It implements slow start, conservative window adaptation (RFC 6675),
 * and 3 different TCP algorithms: AIMD, BIC, and CUBIC (RFC 8312).
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

  void
  CubicIncrease();

  void
  CubicDecrease();

  void
  BicIncrease();

  void
  BicDecrease();

private:
  CcAlgorithm m_ccAlgorithm;
  double m_beta;
  double m_addRttSuppress;
  bool m_reactToCongestionMarks;
  bool m_useCwa;

  double m_ssthresh;
  uint32_t m_highData;
  double m_recPoint;

  // TCP CUBIC Parameters //
  static constexpr double CUBIC_C = 0.4;
  bool m_useCubicFastConv;
  double m_cubicBeta;

  double m_cubicWmax;
  double m_cubicLastWmax;
  time::steady_clock::TimePoint m_cubicLastDecrease;

  // TCP BIC Parameters //
  //! Regular TCP behavior (including slow start) until this window size
  static constexpr uint32_t BIC_LOW_WINDOW = 14;

  //! Sets the maximum (linear) increase of TCP BIC. Should be between 8 and 64.
  static constexpr uint32_t BIC_MAX_INCREMENT = 16;

  // BIC variables:
  double m_bicMinWin; //!< last minimum cwnd
  double m_bicMaxWin; //!< last maximum cwnd
  double m_bicTargetWin;
  double m_bicSsCwnd;
  double m_bicSsTarget;
  bool m_isBicSs; //!< whether we are currently in the BIC slow start phase
};

} // namespace ndn
} // namespace ns3

#endif // NDN_CONSUMER_PCON_H
