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
