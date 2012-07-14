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

#include "load-stats.h"

using namespace ns3;

const double EXP_1  = exp (-5.0/60.0);  /* 1/exp(5sec/1min) */
const double EXP_5  = exp (-5.0/300.0); /* 1/exp(5sec/5min) */
const double EXP_15 = exp (-5.0/900.0); /* 1/exp(5sec/15min) */

const double EPSILON = 0.000001;

namespace ndnSIM
{

LoadStats::LoadStats ()
  : counter_ (0)
  , avg1_ (0)
  , avg5_ (0)
  , avg15_ (0)
{
}

void
LoadStats::Step ()
{
  // do magic
  avg1_  = EXP_1 * avg1_  + (1 - EXP_1)  * counter_;
  avg5_  = EXP_1 * avg5_  + (1 - EXP_5)  * counter_;
  avg15_ = EXP_1 * avg15_ + (1 - EXP_15) * counter_;

  counter_ = 0;
}

LoadStats &
LoadStats::operator ++ (int)
{
  counter_ ++;
  return *this;
}

// void
// LoadStats::Increment (uint32_t amount)
// {
//   counter_ += amount;
// }

// uint32_t
// LoadStats::GetCounter () const
// {
//   return counter_;
// }

LoadStats &
LoadStats::operator += (const LoadStats &stats)
{
  counter_ += stats.counter_;
  return *this;
}

LoadStats::stats_tuple
LoadStats::GetStats () const
{
  return stats_tuple (avg1_, avg5_, avg15_);
}

bool
LoadStats::IsEmpty () const
{
  return (avg1_ < EPSILON &&
	  avg5_ < EPSILON &&
	  avg15_ < EPSILON);
}

std::ostream &
operator << (std::ostream &os, const LoadStats &stats)
{
  LoadStats::stats_tuple data = stats.GetStats ();
  os << data.get<0> () << ", " << data.get<1> () << ", " << data.get<2> ();// << std::endl;
  return os;
}

} // ndnSIM
