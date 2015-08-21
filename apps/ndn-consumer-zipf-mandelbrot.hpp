/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Tsinghua University, P.R.China.
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
 *
 * @author Xiaoke Jiang <shock.jiang@gmail.com>
 **/

#ifndef NDN_CONSUMER_ZIPF_MANDELBROT_H_
#define NDN_CONSUMER_ZIPF_MANDELBROT_H_

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ndn-consumer.hpp"
#include "ndn-consumer-cbr.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/random-variable-stream.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-apps
 * @brief NDN app requesting contents following Zipf-Mandelbrot Distbituion
 *
 * The class implements an app which requests contents following Zipf-Mandelbrot Distribution
 * Here is the explaination of Zipf-Mandelbrot Distribution:
 *http://en.wikipedia.org/wiki/Zipf%E2%80%93Mandelbrot_law
 */
class ConsumerZipfMandelbrot : public ConsumerCbr {
public:
  static TypeId
  GetTypeId();

  /**
   * \brief Default constructor
   * Sets up randomized Number Generator (RNG)
   * Note: m_seq of its parent class ConsumerCbr here is used to record the interest number
   */
  ConsumerZipfMandelbrot();
  virtual ~ConsumerZipfMandelbrot();

  virtual void
  SendPacket();
  uint32_t
  GetNextSeq();

protected:
  virtual void
  ScheduleNextPacket();

private:
  void
  SetNumberOfContents(uint32_t numOfContents);

  uint32_t
  GetNumberOfContents() const;

  void
  SetQ(double q);

  double
  GetQ() const;

  void
  SetS(double s);

  double
  GetS() const;

private:
  uint32_t m_N;               // number of the contents
  double m_q;                 // q in (k+q)^s
  double m_s;                 // s in (k+q)^s
  std::vector<double> m_Pcum; // cumulative probability

  Ptr<UniformRandomVariable> m_seqRng; // RNG
};

} /* namespace ndn */
} /* namespace ns3 */
#endif /* NDN_CONSUMER_ZIPF_MANDELBROT_H_ */
