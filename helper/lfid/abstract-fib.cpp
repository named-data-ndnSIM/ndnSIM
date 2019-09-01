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

#include "abstract-fib.hpp"

#include <algorithm>

#include "ns3/names.h"
#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/model/ndn-global-router.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/node.h"

namespace ns3 {
namespace ndn {

using std::set;

AbstractFib::AbstractFib(const Ptr<GlobalRouter> own, int numNodes)
  : nodeId{static_cast<int>(own->GetObject<ns3::Node>()->GetId())}
  , nodeName{ns3::Names::FindName(own->GetObject<ns3::Node>())}
  , numberOfNodes{numNodes}
  , nodeDegree{static_cast<int>(own->GetIncidencies().size())}
  , ownRouter{own}
  , upwardCounter{0}
  , totalNhCounter{0}
{
  checkInputs();

  createEmptyFib();
}

void
AbstractFib::checkInputs()
{
  if (nodeDegree <= 0) {
    std::cerr << nodeName << " has a degree of " << nodeDegree << "!\n\n";
  }

  const auto MAX_SIZE{1e5};
  NS_ABORT_UNLESS(nodeId >= 0 && nodeId <= MAX_SIZE);
  NS_ABORT_UNLESS(nodeName.size() > 0 && nodeName.size() <= MAX_SIZE);
  NS_ABORT_UNLESS(nodeDegree >= 0 && nodeDegree <= MAX_SIZE);
  NS_ABORT_UNLESS(numberOfNodes > 1 && numberOfNodes <= MAX_SIZE);
}

void
AbstractFib::createEmptyFib()
{
  // Create empty FIB:
  for (int dstId = 0; dstId < numberOfNodes; dstId++) {
    if (dstId == nodeId) {
      continue;
    }
    perDstFib.insert({dstId, {}});
    upwardPerDstFib.insert({dstId, {}});
  }
}

Ptr<GlobalRouter>
AbstractFib::getGR() const
{
  return ownRouter;
}

// Setters:
void
AbstractFib::insert(int dstId, const FibNextHop& nh)
{
  NS_ABORT_UNLESS(nh.getType() == NextHopType::DOWNWARD || nh.getType() == NextHopType::UPWARD);
  NS_ABORT_UNLESS(nh.getNexthopId() != nodeId);

  bool inserted1 = perDstFib.at(dstId).insert(nh).second;
  BOOST_VERIFY(inserted1); // Check if it didn't exist yet.
  totalNhCounter++;

  if (nh.getType() == NextHopType::UPWARD) {
    bool inserted2 = upwardPerDstFib.at(dstId).insert(nh).second;
    BOOST_VERIFY(inserted2);
    upwardCounter++;
  }
}

size_t
AbstractFib::erase(int dstId, int nhId)
{
  auto& fib{perDstFib.at(dstId)};

  auto fibNh = std::find_if(fib.begin(), fib.end(),
                            [&](const FibNextHop& item) { return item.getNexthopId() == nhId; });

  // Element doesn't exist:
  if (fibNh == fib.end()) {

    // TODO: Figure out why this happens.
    return 0;
  }

  NS_ABORT_UNLESS(fibNh != perDstFib.at(dstId).end());
  NS_ABORT_UNLESS(fibNh->getType() == NextHopType::UPWARD);
  totalNhCounter--;

  fib.erase(fibNh);
  auto numErased2 = upwardPerDstFib.at(dstId).erase(*fibNh);
  NS_ABORT_UNLESS(numErased2 == 1);
  upwardCounter--;

  return numErased2;
}

// O(1)
const set<FibNextHop>&
AbstractFib::getNexthops(int dstId) const
{
  NS_ABORT_MSG_IF(dstId == nodeId, "Requested destination id is the same as current nodeId!");
  NS_ABORT_MSG_IF(perDstFib.count(dstId) == 0,
                  "Node " << nodeId << " No nexthops for dst: " << dstId << "!");
  NS_ABORT_UNLESS(perDstFib.count(dstId) != 0);
  return perDstFib.at(dstId);
}

const set<FibNextHop>&
AbstractFib::getUpwardNexthops(int dstId) const
{
  NS_ABORT_MSG_IF(dstId == nodeId, "Requested destination id is the same as current nodeId!");
  NS_ABORT_MSG_IF(perDstFib.count(dstId) == 0,
                  "Node " << nodeId << " No nexthops for dst: " << dstId << "!");
  return upwardPerDstFib.at(dstId);
}

void
AbstractFib::checkFib() const
{
  BOOST_VERIFY(perDstFib.size() > 0);

  for (const auto& fibSet : perDstFib) {
    const size_t numNhs = fibSet.second.size();

    bool hasDownward{false};
    std::unordered_set<int> nextHopSet{};

    for (const FibNextHop& nextHop : fibSet.second) {
      BOOST_VERIFY(nextHop.getCost() > 0 && nextHop.getCost() < FibNextHop::MAX_COST);
      if (nextHop.getType() == NextHopType::DOWNWARD) {
        hasDownward = true;
      }

      // Only one FIB entry per nexthop allowed!
      BOOST_VERIFY(nextHopSet.count(nextHop.getNexthopId()) == 0);
      nextHopSet.emplace(nextHop.getNexthopId());
    }
    BOOST_VERIFY(hasDownward || numNhs == 0);
    BOOST_VERIFY(nextHopSet.size() == fibSet.second.size());
  }
}

std::ostream&
operator<<(std::ostream& os, const AbstractFib& fib)
{
  for (const auto& entry : fib.perDstFib) {
    os << "\nFIB node: " << fib.nodeName << fib.nodeId << "\n";
    os << "Dst: " << entry.first << "\n";
    for (const auto& nh : entry.second) {
      os << nh << "\n";
    }
  }
  return os;
}

} // namespace ndn
} // namespace ns-3
