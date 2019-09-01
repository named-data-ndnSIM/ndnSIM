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

#ifndef LFID_FIB_NH_H
#define LFID_FIB_NH_H

#include <iosfwd>
#include <unordered_set>

#include "ns3/abort.h"

namespace ns3 {
namespace ndn {

enum class NextHopType { DOWNWARD,
                         UPWARD,
                         DISABLED };

class FibNextHop {
public:
  static constexpr int MAX_COST = 1 * 1000 * 1000;

  /**
   * @param nhId The nexthop id.
   * @param cost The link cost of a nexthop. has to be larger than 0!
   * @param costDelta The cost difference (relative to the shortest path nexthop)
   * @param type The nexthop type @sa NextHopType
   */
  FibNextHop(int cost, int nhId, int costDelta = -1, NextHopType type = NextHopType::DISABLED);

  // Getters
  int
  getNexthopId() const
  {
    return m_nhId;
  }

  int
  getCost() const
  {
    return m_cost;
  }

  int
  getCostDelta() const
  {
    return m_costDelta;
  }

  NextHopType
  getType() const
  {
    return m_type;
  }

  // Setters:
  void
  setType(const NextHopType& newType)
  {
    NS_ABORT_UNLESS(newType != NextHopType::DISABLED);
    this->m_type = newType;
  }

  // Only used in old fillFib:
  void
  setCost(int newCost, int newCostDelta)
  {
    NS_ABORT_UNLESS(newCost > 0);
    NS_ABORT_UNLESS(newCostDelta >= 0);
    this->m_cost = newCost;
    this->m_costDelta = newCostDelta;
  }

private:
  // Order of FibNexthop:
  friend bool
  operator<(const FibNextHop& own, const FibNextHop& other)
  {
    NS_ABORT_UNLESS(own.m_nhId != -1);

    return std::tie(own.m_costDelta, own.m_cost, own.m_nhId)
           < std::tie(other.m_costDelta, other.m_cost, other.m_nhId);
  }

  friend bool
  operator==(const FibNextHop& own, const FibNextHop& other)
  {
    if (other.m_nhId == own.m_nhId) {
      // Check that there are no FibNextHop with identical id, but different cost.
      NS_ABORT_UNLESS(other.m_cost == own.m_cost);
      NS_ABORT_UNLESS(other.m_costDelta == own.m_costDelta);
      return true;
    }
    else {
      return false;
    }
  }

  friend bool
  operator!=(const FibNextHop& own, const FibNextHop& other)
  {
    return !(own == other);
  }

  friend std::ostream&
  operator<<(std::ostream&, const FibNextHop& fib);

private:
  int m_cost;
  int m_nhId;
  NextHopType m_type;
  int m_costDelta;
};

std::ostream&
operator<<(std::ostream& os, const NextHopType& type);
std::ostream&
operator<<(std::ostream& os, const FibNextHop& a);

} // namespace ndn
} // namespace ns-3

namespace std {
template <>
struct hash<ns3::ndn::FibNextHop>;
}

#endif // LFID_FIB_NH_H
