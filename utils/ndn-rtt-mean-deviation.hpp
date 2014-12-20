/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//           (c) 2013 University of Arizona
//           (c) 2013 University of California, Los Angeles
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//         Cheng Yi <yic@email.arizona.edu>
//         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
//

// Georgia Tech Network Simulator - Round Trip Time Estimation Class
// George F. Riley.  Georgia Tech, Spring 2002


#ifndef NDN_RTT_MEAN_DEVIATION_H
#define NDN_RTT_MEAN_DEVIATION_H

#include <ns3/ndnSIM/utils/ndn-rtt-estimator.h>

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn-apps
 *
 * \brief The modified version of "Mean--Deviation" RTT estimator, as discussed by Van Jacobson that better suits NDN communication model
 *
 * This class implements the "Mean--Deviation" RTT estimator, as discussed
 * by Van Jacobson and Michael J. Karels, in
 * "Congestion Avoidance and Control", SIGCOMM 88, Appendix A
 *
 */
class RttMeanDeviation : public RttEstimator {
public:
  static TypeId GetTypeId (void);

  RttMeanDeviation ();
  RttMeanDeviation (const RttMeanDeviation&);

  virtual TypeId GetInstanceTypeId (void) const;

  void SentSeq (SequenceNumber32 seq, uint32_t size);
  Time AckSeq (SequenceNumber32 ackSeq);
  void Measurement (Time measure);
  Time RetransmitTimeout ();
  Ptr<RttEstimator> Copy () const;
  void Reset ();
  void Gain (double g);

private:
  double       m_gain;       // Filter gain
  double       m_gain2;      // Filter gain
  Time         m_variance;   // Current variance
};

} // namespace ndn
} // namespace ns3

#endif // NDN_RTT_MEAN_DEVIATION
