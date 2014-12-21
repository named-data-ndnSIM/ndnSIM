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

#ifndef NDNSIM_UTILS_TIME_HPP
#define NDNSIM_UTILS_TIME_HPP

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include <ndn-cxx/util/time-custom-clock.hpp>

namespace ns3 {
namespace ndn {
namespace time {

class CustomSystemClock : public CustomClock<system_clock> {
public:
  system_clock::time_point
  getNow() const;

  std::string
  getSince() const;

  boost::posix_time::time_duration
  toPosixDuration(const system_clock::duration& duration) const;
};

class CustomSteadyClock : public CustomClock<steady_clock> {
public:
  steady_clock::time_point
  getNow() const;

  std::string
  getSince() const;

  boost::posix_time::time_duration
  toPosixDuration(const steady_clock::duration& duration) const;
};

} // namespace time
} // namespace ndn
} // namespace ns3

#endif // NDNSIM_UTILS_TIME_HPP
