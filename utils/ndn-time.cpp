/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012-2014 University of California, Los Angeles
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
 */

#include "ndn-time.hpp"

#include "ns3/simulator.h"

namespace ns3 {
namespace ndn {
namespace time {

system_clock::time_point
CustomSystemClock::getNow() const
{
  boost::chrono::nanoseconds ns(Simulator::Now().GetNanoSeconds());
  return system_clock::time_point(ns);
}

std::string
CustomSystemClock::getSince() const
{
  return " since start of simulation";
}

boost::posix_time::time_duration
CustomSystemClock::toPosixDuration(const system_clock::duration& duration) const
{
  BOOST_ASSERT_MSG(false, "Must not be called");
  return boost::posix_time::microseconds(1);
}

////////////////////////////////////

steady_clock::time_point
CustomSteadyClock::getNow() const
{
  boost::chrono::nanoseconds ns(Simulator::Now().GetNanoSeconds());
  return steady_clock::time_point(ns);
}

std::string
CustomSteadyClock::getSince() const
{
  return " since start of simulation";
}

boost::posix_time::time_duration
CustomSteadyClock::toPosixDuration(const steady_clock::duration& duration) const
{
  BOOST_ASSERT_MSG(false, "Must not be called");
  return boost::posix_time::microseconds(1);
}

} // namespace time
} // namespace ndn
} // namespace ns3
