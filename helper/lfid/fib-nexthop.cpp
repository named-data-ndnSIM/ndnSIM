/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2019 Klaus Schneider, The University of Arizona
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
 * Author: Klaus Schneider <klaus@cs.arizona.edu>
 */

#include "fib-nexthop.hpp"

#include <boost/functional/hash.hpp>
#include <ostream>

namespace ns3 {
namespace ndn {

constexpr int NODE_ID_LIMIT = 1000;

FibNextHop::FibNextHop(int cost, int nhId, int costDelta, NextHopType type)
{
  NS_ABORT_UNLESS(cost > 0 && cost <= MAX_COST);
  NS_ABORT_UNLESS(nhId >= 0 && nhId <= NODE_ID_LIMIT);

  this->m_nhId = nhId;
  this->m_cost = cost;
  this->m_type = type;
  this->m_costDelta = costDelta;
}

std::ostream&
operator<<(std::ostream& os, const NextHopType& type)
{
  switch (type) {
  case NextHopType::DOWNWARD:
    return os << "DOWNWARD";
  case NextHopType::UPWARD:
    return os << "UPWARD";
  case NextHopType::DISABLED:
    return os << "DISABLED";
  }
  return os << static_cast<int>(type);
}

std::ostream&
operator<<(std::ostream& os, const FibNextHop& a)
{
  return os << "Id: " << a.getNexthopId() << ", cost: " << a.m_cost << ", type: " << a.m_type;
}

} // namespace ndn
} // namespace ns-3

namespace std {

using ns3::ndn::FibNextHop;

template <>
struct hash<FibNextHop> {
  size_t
  operator()(const FibNextHop& k) const
  {
    // Combine hash via boost library
    std::size_t seed = 0;
    boost::hash_combine(seed, k.getNexthopId());
    boost::hash_combine(seed, k.getCost());

    return seed;
  }
};

}
