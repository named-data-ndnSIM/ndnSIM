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

#ifndef LOAD_STATS_H
#define LOAD_STATS_H

#include "ns3/nstime.h"
#include <boost/tuple/tuple.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

class LoadStats
{
public:
  typedef boost::tuple<double, double, double> stats_tuple;

  static const double PRECISION;

  LoadStats ();
  
  void
  Step ();

  // void
  // Increment (uint32_t amount);

  LoadStats &
  operator ++ (int);
  
  // uint32_t
  // GetCounter () const;

  LoadStats &
  operator += (const LoadStats &stats);

  LoadStats &
  operator += (uint32_t amount);

  stats_tuple
  GetStats () const;

  bool
  IsZero () const;
  
private:
  uint32_t counter_;

  double avg1_;
  double avg2_;
  double avg3_;

  friend std::ostream &
  operator << (std::ostream &os, const LoadStats &stats);
};

std::ostream &
operator << (std::ostream &os, const LoadStats &stats);

} // namespace ndnSIM
} // namespace ndn
} // namespace ns3

#endif // LOAD_STATS_H
