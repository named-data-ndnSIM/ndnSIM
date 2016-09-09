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

#ifndef NDNSIM_HELPER_NDN_SCENARIO_HELPER_HPP
#define NDNSIM_HELPER_NDN_SCENARIO_HELPER_HPP

#include "ndn-stack-helper.hpp"

#include "ns3/net-device.h"
#include "ns3/node.h"

#include <ndn-cxx/name.hpp>
#include <map>

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn-helpers
 * @brief Helper class to simplify writing basic simulation scenarios
 *
 * The following code with scenario helper creates a 3-node topology,
 * with manual routes between nodes and 2 applications, installed on first and last node
 * of the topology:
 *
 *     ScenarioHelper helper;
 *     helper.createTopology({
 *         {"1", "2"},
 *         {"2", "3"}
 *       });
 *
 *     helper.addRoutes({
 *         {"1", "2", "/prefix", 1},
 *         {"2", "3", "/prefix", 1}
 *       });
 *
 *     helper.addApps({
 *         {"1", "ns3::ndn::ConsumerCbr",
 *             {{"Prefix", "/prefix"}, {"Frequency", "1"}},
 *             "0s", "100s"},
 *         {"3", "ns3::ndn::Producer",
 *             {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
 *             "0s", "100s"}
 *       });
 *
 */
class ScenarioHelper
{
public:
  /**
   * @brief Route information for addRoutes method
   *
   * It is preferred to use initializer list to indirectly pass RouteInfo's to addRoutes
   * method.
   */
  struct RouteInfo
  {
    std::string node1;
    std::string node2;
    Name prefix;
    int32_t metric;
  };

  /*
   * @brief Application information for addApps method
   *
   * It is preferred to use initializer list to indirectly pass AppInfo's to addApps
   * method.
   */
  struct AppInfo
  {
    std::string node;
    std::string name;
    std::initializer_list<std::pair<std::string, std::string>> params;
    std::string start;
    std::string end;
  };

public:
  ScenarioHelper();

  /**
   * @brief Create topology
   * @throw std::logic_error if createTopology is called more than once
   *
   * Example:
   *
   *     ScenarioHelper helper;
   *     helper.createTopology({
   *         {"1", "2"},
   *         {"2", "3"}
   *       });
   */
  void
  createTopology(std::initializer_list<std::initializer_list<std::string>/*node clique*/> topology,
                 bool shouldInstallNdnStack = true);

  /**
   * @brief Create routes between topology nodes
   * @throw std::invalid_argument if the nodes or links between nodes do not exist
   *
   * Example:
   *
   *     helper.addRoutes({
   *         {"1", "2", "/prefix", 1},
   *         {"2", "3", "/prefix", 1}
   *       });
   */
  void
  addRoutes(std::initializer_list<RouteInfo> routes);

  /**
   * @brief Create and install application on nodes
   * @throw std::invalid_argument if the nodes or links between nodes do not exist
   *
   * Example:
   *
   *     helper.addApps({
   *         {"1", "ns3::ndn::ConsumerCbr",
   *             {{"Prefix", "/prefix"}, {"Frequency", "1"}},
   *             "0s", "100s"},
   *         {"3", "ns3::ndn::Producer",
   *             {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
   *             "0s", "100s"}
   *       });
   */
  void
  addApps(std::initializer_list<AppInfo> apps);

public: // topology accessors
  /**
   * @brief Get node
   * @throw std::invalid_argument if the node does not exist
   */
  Ptr<Node>
  getNode(const std::string& nodeName);

  /**
   * @brief Get face on the @p node1 pointing towards @p node2
   * @throw std::invalid_argument if the link does not exist
   */
  shared_ptr<Face>
  getFace(const std::string& node1, const std::string& node2);

  /**
   * @brief Get NetDevice on the @p node1 pointing towards @p node2
   * @throw std::invalid_argument if the link does not exist
   */
  Ptr<NetDevice>
  getNetDevice(const std::string& node1, const std::string& node2);

  /**
   * \brief Disable RIB Manager
   */
  void
  disableRibManager();

  // Cannot be disabled for now
  // /**
  //  * \brief Disable Face Manager
  //  */
  // void
  // disableFaceManager();

  /**
   * \brief Disable Strategy Choice Manager
   */
  void
  disableStrategyChoiceManager();

  /**
   * \brief Disable Forwarder Status Manager
   */
  void
  disableForwarderStatusManager();

  /**
   * \brief Get NDN stack helper, e.g., to adjust its parameters
   */
  StackHelper&
  getStackHelper();

private:
  Ptr<Node>
  getOrCreateNode(const std::string& nodeName);

private:
  bool m_isTopologyInitialized;
  StackHelper ndnHelper;
  std::map<std::string, std::map<std::string, Ptr<NetDevice>>> links;
  std::map<std::string, Ptr<Node>> nodes;
};

} // namespace ndn
} // namespace ns3

#endif // NDNSIM_HELPER_NDN_SCENARIO_HELPER_HPP
