/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Tsinghua University, P.R.China
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
 * Author: Xiaoke Jiang <shock.jiang@gmail.com>
 */


#ifndef NDN_CONSUMER_ZIPF_MANDELBROT_H_
#define NDN_CONSUMER_ZIPF_MANDELBROT_H_

#include "ndn-consumer.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ndn-consumer-cbr.h"
#include "ns3/random-variable.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn
 * @brief NDN app requesting contents following Zipf-Mandelbrot Distbituion
 *
 * The class implements an app which requests contents following Zipf-Mandelbrot Distribution
 * Here is the explaination of Zipf-Mandelbrot Distribution: http://en.wikipedia.org/wiki/Zipf%E2%80%93Mandelbrot_law
 */
class ConsumerZipfMandelbrot: public ConsumerCbr
{
public:
  static TypeId GetTypeId ();

  /**
   * \brief Default constructor
   * Sets up randomized Number Generator (RNG)
   * Note: m_seq of its parent class ConsumerCbr here is used to record the interest number
   */
  ConsumerZipfMandelbrot ();
  virtual ~ConsumerZipfMandelbrot ();

  virtual void SendPacket();
  uint32_t GetNextSeq();

protected:
  virtual void
  ScheduleNextPacket ();

private:
  void
  SetNumberOfContents (uint32_t numOfContents);

  uint32_t
  GetNumberOfContents () const;
  
private:
  uint32_t m_N;  //number of the contents
  double m_q;  //q in (k+q)^s
  double m_s;  //s in (k+q)^s
  std::vector<double> m_Pcum;  //cumulative probability

  UniformVariable m_SeqRng; //RNG
};

} /* namespace ndn */
} /* namespace ns3 */
#endif /* NDN_CONSUMER_ZIPF_MANDELBROT_H_ */
