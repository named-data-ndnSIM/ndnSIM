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

#ifndef LFID_ABS_FIB_H
#define LFID_ABS_FIB_H

#include <set>
#include <unordered_map>

#include "ns3/abort.h"
#include "ns3/ndnSIM/helper/lfid/fib-nexthop.hpp"
#include "ns3/ptr.h"

namespace ns3 {
namespace ndn {

class GlobalRouter;

/**
 * An abstract, lightweight representation of the FIB.
 */
class AbstractFib {
public:
  using AllNodeFib = std::unordered_map<int, AbstractFib>;

  /**
   * @param own The GlobalRouter object representing the current router
   * @param numNodes The total number of nodes in the network
   */
  AbstractFib(const Ptr<GlobalRouter> own, int numNodes);

public:
  // Getters:
  /**
   * @return Return set of nexthops per destination
   */
  const std::set<FibNextHop>&
  getNexthops(int dstId) const;

  /**
   * @return Return set of upward nexthops per destination
   */
  const std::set<FibNextHop>&
  getUpwardNexthops(int dstId) const;

  /**
   * Make sure that FIB is consistent (each destination has at least one downward nexthop)
   */
  void
  checkFib() const;

  /**
   * @return Return number nexthops per destination
   * @pre Also assure that the destination is not equal to the current nodeId.
   */
  int
  numEnabledNhPerDst(int dstId) const
  {
    NS_ABORT_UNLESS(dstId != nodeId);
    return static_cast<int>(getNexthops(dstId).size());
  }

  int
  getNodeId() const
  {
    return nodeId;
  }

  Ptr<GlobalRouter>
  getGR() const;

  std::string
  getName() const
  {
    return nodeName;
  }

  int
  getDegree() const
  {
    return nodeDegree;
  }

  int
  getNumDsts() const
  {
    return static_cast<int>(perDstFib.size());
  }

  bool
  contains(int dstId) const
  {
    return perDstFib.count(dstId) > 0;
  }

  // Functions for range-based loop:
  auto
  begin() const
  {
    return perDstFib.cbegin();
  }

  auto
  end() const
  {
    return perDstFib.cend();
  }

  // Setters:
  void
  insert(int dstId, const FibNextHop& nh);

  size_t
  erase(int dstId, int nhId);

private:
  void
  checkInputs();

  void
  createEmptyFib();

private:
  const int nodeId;           // Own node id
  const std::string nodeName; // Own node name
  const int numberOfNodes;
  const int nodeDegree;
  const Ptr<GlobalRouter> ownRouter;

  int upwardCounter;
  int totalNhCounter;

  // DstId -> set<FibNextHop>
  std::unordered_map<int, std::set<FibNextHop>> perDstFib;
  std::unordered_map<int, std::set<FibNextHop>> upwardPerDstFib;

  friend std::ostream&
  operator<<(std::ostream&, const AbstractFib& fib);
};

std::ostream&
operator<<(std::ostream& os, const AbstractFib& fib);

} // namespace ndn
} // namespace ns-3

#endif // LFID_ABS_FIB_H
