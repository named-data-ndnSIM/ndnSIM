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

#include "remove-loops.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <queue>

#include "ns3/abort.h"
#include "ns3/ndnSIM/helper/lfid/abstract-fib.hpp"

namespace ns3 {
namespace ndn {

using std::set;
using AllNodeFib = AbstractFib::AllNodeFib;

/**
 * Fill directed graph only with edges existing in the FIB.
 */
void
getDigraphFromFib(DiGraph& dg, const AllNodeFib& allNodeFIB, const int dstId)
{

  // 1. Erase All Arcs:
  dg.clear();

  // 2. Add Arcs from FIB
  for (const auto& node : allNodeFIB) {
    int nodeId = node.first;
    if (dstId == nodeId) {
      continue;
    }

    for (const auto& fibNh : node.second.getNexthops(dstId)) {
      NS_ABORT_UNLESS(fibNh.getType() <= NextHopType::UPWARD);
      boost::add_edge(static_cast<uint64_t>(nodeId), static_cast<uint64_t>(fibNh.getNexthopId()), 1, dg);
    }
  }
}

class NodePrio {
public:
  NodePrio(int nodeId, int remainingNh, set<FibNextHop> nhSet)
    : m_nodeId{nodeId}
    , m_remainingNh{remainingNh}
    , m_uwSet{nhSet}
  {
    NS_ABORT_UNLESS(remainingNh > 0 && m_uwSet.size() > 0);
    NS_ABORT_UNLESS(static_cast<int>(m_uwSet.size()) < remainingNh);
  }

  int
  getId() const
  {
    return m_nodeId;
  }

  int
  getRemainingUw() const
  {
    return static_cast<int>(m_uwSet.size());
  }

  /**
   * Order by Remamining UW NHs, Highest DeltaCost, and then id.
   */
  bool
  operator<(const NodePrio& other) const
  {
    return std::make_tuple(m_remainingNh, getHighestCostUw(), m_nodeId)
           < std::make_tuple(other.m_remainingNh, other.getHighestCostUw(), other.m_nodeId);
  }

  // Setters:
  FibNextHop
  popHighestCostUw()
  {
    const FibNextHop& tmp = getHighestCostUw();
    eraseUw(tmp);
    return tmp;
  }

  void
  reduceRemainingNh()
  {
    m_remainingNh--;
    // Check that remaining nexthops >= remaining uw nexthops.
    NS_ABORT_UNLESS(m_remainingNh > 0 && m_remainingNh > getRemainingUw());
  }

private:
  void
  eraseUw(FibNextHop nh)
  {
    NS_ABORT_UNLESS(m_uwSet.size() > 0);
    auto success = m_uwSet.erase(nh);
    NS_ABORT_UNLESS(success == 1);
  }

  FibNextHop
  getHighestCostUw() const
  {
    NS_ABORT_UNLESS(m_uwSet.size() > 0);
    NS_ABORT_UNLESS(std::prev(m_uwSet.end()) != m_uwSet.end());
    return *(std::prev(m_uwSet.end()));
  }

private:
  int m_nodeId;
  int m_remainingNh;
  set<FibNextHop> m_uwSet;

  friend std::ostream&
  operator<<(std::ostream&, const NodePrio& node);
};

std::ostream&
operator<<(std::ostream& os, const NodePrio& node)
{
  return os << "Id: " << node.m_nodeId << ", remaining NH: " << node.m_remainingNh
            << ", remaining UW: " << node.getRemainingUw() << " ";
}

int
removeLoops(AllNodeFib& allNodeFIB, bool printOutput)
{
  int removedLoopCounter = 0;
  int upwardCounter = 0;

  const int NUM_NODES{static_cast<int>(allNodeFIB.size())};

  // Build graph with boost graph library:
  DiGraph dg{};

  // Add all Arcs that fit into FIB. // O(n)
  for (int dstId = 0; dstId < NUM_NODES; dstId++) {
    // 1. Get DiGraph from Fib //
    getDigraphFromFib(dg, allNodeFIB, dstId);

    // NodeId -> set<UwNexthops>
    std::priority_queue<NodePrio> q;

    // 2. Put nodes in the queue, ordered by # remaining nexthops, then CostDelta // O(n^2)
    for (const auto& node : allNodeFIB) {
      int nodeId{node.first};
      const AbstractFib& fib{node.second};
      if (nodeId == dstId) {
        continue;
      }

      const auto& uwNhSet = fib.getUpwardNexthops(dstId);
      if (!uwNhSet.empty()) {
        upwardCounter += uwNhSet.size();

        int fibSize{fib.numEnabledNhPerDst(dstId)};
        // NodePrio tmpNode {nodeId, fibSize, uwNhSet};
        q.emplace(nodeId, fibSize, uwNhSet);
      }
    }

    // 3. Iterate PriorityQueue //
    while (!q.empty()) {
      NodePrio node = q.top();
      q.pop();

      int nodeId = node.getId();
      int nhId = node.popHighestCostUw().getNexthopId();

      // Remove opposite of Uphill link
      //      int arcId1 {getArcId(arcMap, nhId, nodeId)};
      auto res = boost::edge(static_cast<uint64_t>(nhId), static_cast<uint64_t>(nodeId), dg);

      auto arc = res.first;
      bool arcExists = res.second;

      if (arcExists) {
        boost::remove_edge(arc, dg);
      }

      // 2. Loop Check: Is the current node still reachable for the uphill nexthop?
      // Uses BFS:
      // bool willLoop = bfs(dg).run(dg.nodeFromId(nhId), dg.nodeFromId(nodeId)); // O(m^2n)

      std::vector<int> dists(num_vertices(dg));

      auto weightmap = get(boost::edge_weight, dg);

      const auto& x = boost::edges(dg);
      for (auto e = x.first; e != x.second; e++) {
        int weight = get(weightmap, *e);
        NS_ABORT_UNLESS(weight == 1); // Only use uniform weights.
      }

      // TODO: Could be replaced by BFS/DFS to improve speed.
      dijkstra_shortest_paths(dg, static_cast<uint64_t>(nhId),
                              distance_map(
                                boost::make_iterator_property_map(dists.begin(), get(boost::vertex_index, dg))));

      bool willLoop = (dists.at(static_cast<size_t>(nodeId)) < (std::numeric_limits<int>::max() - 1));

      // Uphill nexthop loops back to original node
      if (willLoop) {
        node.reduceRemainingNh();
        removedLoopCounter++;

        // Erase FIB entry
        allNodeFIB.at(node.getId()).erase(dstId, nhId);

        auto res2 = boost::edge(static_cast<uint64_t>(node.getId()), static_cast<uint64_t>(nhId), dg);
        auto arc2 = res2.first;
        NS_ABORT_UNLESS(res.second);

        boost::remove_edge(arc2, dg);
      }

      // Add opposite of UW link back:
      if (arcExists) {
        boost::add_edge(static_cast<uint64_t>(nhId), static_cast<uint64_t>(nodeId), 1, dg);
      }

      // If not has further UW nexthops: Requeue.
      if (node.getRemainingUw() > 0) {
        q.push(node);
      }
    }
  }

  if (printOutput) {
    std::cout << "Found " << upwardCounter << " UW nexthops, Removed " << removedLoopCounter
              << " Looping UwNhs, Remaining: " << upwardCounter - removedLoopCounter << " NHs\n";
  }
  NS_ABORT_UNLESS((upwardCounter - removedLoopCounter) >= 0);

  return removedLoopCounter;
}

int
removeDeadEnds(AllNodeFib& allNodeFIB, bool printOutput)
{
  int NUM_NODES{static_cast<int>(allNodeFIB.size())};
  int checkedUwCounter{0};
  int uwCounter{0};
  int totalCounter{0};
  int removedDeadendCounter{0};

  for (int dstId = 0; dstId < NUM_NODES; dstId++) {
    // NodeId -> FibNexthops (Order important)
    set<std::pair<int, FibNextHop>> nhSet;

    // 1. Put all uwNexthops in set<NodeId, FibNexhtop>:
    for (const auto& node : allNodeFIB) {
      int nodeId{node.first};
      if (nodeId == dstId) {
        continue;
      }

      totalCounter += node.second.getNexthops(dstId).size();

      const auto& uwNhSet = node.second.getUpwardNexthops(dstId);
      uwCounter += uwNhSet.size();
      for (const FibNextHop& fibNh : uwNhSet) {
        nhSet.emplace(nodeId, fibNh);
      }
    }

    // FibNexthops ordered by (costDelta, cost, nhId).
    // Start with nexthop with highest cost:
    while (!nhSet.empty()) {
      checkedUwCounter++;

      // Pop from queue:
      const auto& nhPair = nhSet.begin();
      NS_ABORT_UNLESS(nhPair != nhSet.end());
      nhSet.erase(nhPair);

      int nodeId = nhPair->first;
      const FibNextHop& nh = nhPair->second;
      AbstractFib& fib = allNodeFIB.at(nodeId);

      if (nh.getNexthopId() == dstId) {
        continue;
      }

      int reverseEntries{allNodeFIB.at(nh.getNexthopId()).numEnabledNhPerDst(dstId)};

      // Must have at least one FIB entry.
      NS_ABORT_UNLESS(reverseEntries > 0);

      // If it has exactly 1 entry -> Is downward back through the upward nexthop!
      // Higher O-Complexity below:
      if (reverseEntries <= 1) {
        removedDeadendCounter++;

        // Erase NhEntry from FIB:
        fib.erase(dstId, nh.getNexthopId());

        // Push into Queue: All NhEntries that lead to m_nodeId!
        const auto& nexthops = fib.getNexthops(dstId);

        for (const auto& ownNhs : nexthops) {
          if (ownNhs.getType() == NextHopType::DOWNWARD && ownNhs.getNexthopId() != dstId) {
            const auto& reverseNh = allNodeFIB.at(ownNhs.getNexthopId()).getNexthops(dstId);

            for (const auto& y : reverseNh) {
              if (y.getNexthopId() == nodeId) {
                NS_ABORT_UNLESS(y.getType() == NextHopType::UPWARD);
                nhSet.emplace(ownNhs.getNexthopId(), y);
                break;
              }
            }
          }
        }
      }
    }
  }

  if (printOutput) {
    std::cout << "Checked " << checkedUwCounter << " Upward NHs, Removed " << removedDeadendCounter
              << " Deadend UwNhs, Remaining: " << uwCounter - removedDeadendCounter << " UW NHs, "
              << totalCounter - removedDeadendCounter << " total nexthops\n";
  }

  return removedDeadendCounter;
}

} // namespace ndn
} // namespace ns3
