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

#ifndef NDNSIM_HELPER_NDN_STRATEGY_CHOICE_HELPER_HPP
#define NDNSIM_HELPER_NDN_STRATEGY_CHOICE_HELPER_HPP

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/node.h"
#include "ns3/ptr.h"
#include "ns3/node-container.h"

#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/ndnSIM/NFD/daemon/mgmt/strategy-choice-manager.hpp"
#include "ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/strategy-choice.hpp"

namespace ndn {
namespace nfd {
class ControlParameters;
} // namespace nfd
} // namespace ndn

namespace nfd {
namespace fw {
class Strategy;
} // namespace fw
} // namespace nfd


namespace ns3 {
namespace ndn {

using ::ndn::nfd::ControlParameters;

/**
 * @ingroup ndn-helpers
 * @brief NFD Strategy Choice Helper (FIB) helper
 *
 * The Strategy Choice helper interacts with the Strategy Choice manager of NFD by sending
 * special Interest commands to the manager in order to specify the desired per-name
 * prefix forwarding strategy for one, more or all the nodes of a topology.
 */
class StrategyChoiceHelper
{
public:
  /**
   * @brief Install a built-in strategy @p strategy on @p node for @p namePrefix namespace
   */
  static void
  Install(Ptr<Node> node, const Name& namePrefix, const Name& strategy);

  /**
   * @brief Install a built-in strategy @p strategy on nodes in @p c container for
   *        @p namePrefix namespace
   */
  static void
  Install(const NodeContainer& c, const Name& namePrefix, const Name& strategy);

  /**
   * @brief Install a built-in strategy @p strategy on all nodes for @p namePrefix namespace
   */
  static void
  InstallAll(const Name& namePrefix, const Name& strategy);

  /**
   * @brief Install a custom strategy on @p node for @p namePrefix namespace
   * @tparam Strategy Class name of the custom strategy
   */
  template<class Strategy>
  static void
  Install(Ptr<Node> node, const Name& namePrefix);

  /**
   * @brief Install a custom strategy on nodes in @p c container for @p namePrefix namespace
   * @tparam Strategy Class name of the custom strategy
   */
  template<class Strategy>
  static void
  Install(const NodeContainer& c, const Name& namePrefix);

  /**
   * @brief Install a custom strategy on all nodes for @p namePrefix namespace
   * @tparam Strategy Class name of the custom strategy
   */
  template<class Strategy>
  static void
  InstallAll(const Name& namePrefix);

private:
  static void
  sendCommand(const ControlParameters& parameters, Ptr<Node> node);
};

template<class Strategy>
inline void
StrategyChoiceHelper::Install(Ptr<Node> node, const Name& namePrefix)
{
  Ptr<L3Protocol> l3Protocol = node->GetObject<L3Protocol>();
  NS_ASSERT(l3Protocol != nullptr);
  NS_ASSERT(l3Protocol->getForwarder() != nullptr);

  nfd::Forwarder& forwarder = *l3Protocol->getForwarder();
  nfd::StrategyChoice& strategyChoice = forwarder.getStrategyChoice();

  if (!strategyChoice.hasStrategy(Strategy::STRATEGY_NAME)) {
    strategyChoice.install(make_shared<Strategy>(ref(forwarder)));
  }

  Install(node, namePrefix, Strategy::STRATEGY_NAME);
}

template<class Strategy>
inline void
StrategyChoiceHelper::Install(const NodeContainer& c, const Name& namePrefix)
{
  for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i) {
    Install<Strategy>(*i, namePrefix);
  }
}

template<class Strategy>
inline void
StrategyChoiceHelper::InstallAll(const Name& namePrefix)
{
  Install<Strategy>(NodeContainer::GetGlobal(), namePrefix);
}

} // namespace ndn
} // namespace ns3

#endif // NDNSIM_HELPER_NDN_STRATEGY_CHOICE_HELPER_HPP
