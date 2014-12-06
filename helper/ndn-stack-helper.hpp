/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef NDN_STACK_HELPER_H
#define NDN_STACK_HELPER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ptr.h"
#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"

#include "ndn-face-container.hpp"
#include "ndn-fib-helper.hpp"
#include "ndn-strategy-choice-helper.hpp"

namespace ns3 {

class Node;

namespace ndn {

class NetDeviceFace;
class L3Protocol;

/**
 * \ingroup ndn
 * \defgroup ndn-helpers Helpers
 */
/**
 * \ingroup ndn-helpers
 * \brief Adding Ndn functionality to existing Nodes.
 *
 * This helper enables pcap and ascii tracing of events in the ndn stack
 * associated with a node.  This is substantially similar to the tracing that
 * happens in device helpers, but the important difference is that, well, there
 * is no device.  This means that the creation of output file names will change,
 * and also the user-visible methods will not reference devices and therefore
 * the number of trace enable methods is reduced.
 *
 * Normally we eschew multiple inheritance, however, the classes
 * PcapUserHelperForNdn and AsciiTraceUserHelperForNdn are treated as
 * "mixins".  A mixin is a self-contained class that encapsulates a general
 * attribute or a set of functionality that may be of interest to many other
 * classes.
 */
class StackHelper {
public:
  /**
   * \brief Create a new NdnStackHelper with a default NDN_FLOODING forwarding stategy
   */
  StackHelper();

  /**
   * \brief Destroy the NdnStackHelper
   */
  virtual ~StackHelper();

  /**
   * @brief Set parameters of NdnL3Protocol
   */
  void
  SetStackAttributes(const std::string& attr1 = "", const std::string& value1 = "",
                     const std::string& attr2 = "", const std::string& value2 = "",
                     const std::string& attr3 = "", const std::string& value3 = "",
                     const std::string& attr4 = "", const std::string& value4 = "");

  /**
   * @brief Set content store class and its attributes
   * @param contentStoreClass string, representing class of the content store
   */
  void
  SetContentStore(const std::string& contentStoreClass, const std::string& attr1 = "",
                  const std::string& value1 = "", const std::string& attr2 = "",
                  const std::string& value2 = "", const std::string& attr3 = "",
                  const std::string& value3 = "", const std::string& attr4 = "",
                  const std::string& value4 = "");

  /**
   * @brief Set to use native NFD's content store or ndnSIM provided CS implementations
   *
   * @note ndnSIM CS implementations have limited support for NDN selectors
   */
  void
  SetContentStoreChoice(const bool shouldUseNfdCs);

  /**
   * @brief Check if stack is using native NFD content store implementation
   */
  bool
  shouldUseNfdCs() const;

  typedef Callback<shared_ptr<NetDeviceFace>, Ptr<Node>, Ptr<L3Protocol>, Ptr<NetDevice>>
    NetDeviceFaceCreateCallback;

  /**
   * @brief Add callback to create and configure instance of the face, based on supplied Ptr<Node>
   *and Ptr<NetDevice>
   *
   * It is possible to set up several callbacks for different NetDevice types.
   *
   * If none of the callbacks fit the TypeId of NetDevice, a default callback is used
   *(DefaultNetDeviceCallback)
   */
  void
  AddNetDeviceFaceCreateCallback(TypeId netDeviceType, NetDeviceFaceCreateCallback callback);

  /**
   * @brief Update callback to create and configure instance of the face, based on supplied
   *Ptr<Node> and Ptr<NetDevice>
   *
   * It is possible to set up several callbacks for different NetDevice types.
   *
   * Using this method, it is possible to override Face creation for PointToPointNetDevices
   */
  void
  UpdateNetDeviceFaceCreateCallback(TypeId netDeviceType, NetDeviceFaceCreateCallback callback);

  /**
   * @brief Remove callback to create and configure instance of the face, based on supplied
   * Ptr<Node> and Ptr<NetDevice>
   */
  void
  RemoveNetDeviceFaceCreateCallback(TypeId netDeviceType, NetDeviceFaceCreateCallback callback);

  /**
  * \brief Install Ndn stack on the node
  *
  * This method will assert if called on a node that already has Ndn object
  * installed on it
  *
  * \param nodeName The name of the node on which to install the stack.
  *
  * \returns list of installed faces in the form of a smart pointer
  * to NdnFaceContainer object
  */
  Ptr<FaceContainer>
  Install(const std::string& nodeName) const;

  /**
   * \brief Install Ndn stack on the node
   *
   * This method will assert if called on a node that already has Ndn object
   * installed on it
   *
   * \param node The node on which to install the stack.
   *
   * \returns list of installed faces in the form of a smart pointer
   * to FaceContainer object
   */
  Ptr<FaceContainer>
  Install(Ptr<Node> node) const;

  /**
   * \brief Install Ndn stack on each node in the input container
   *
   * The program will assert if this method is called on a container with a node
   * that already has an ndn object aggregated to it.
   *
   * \param c NodeContainer that holds the set of nodes on which to install the
   * new stacks.
   *
   * \returns list of installed faces in the form of a smart pointer
   * to FaceContainer object
   */
  Ptr<FaceContainer>
  Install(const NodeContainer& c) const;

  /**
   * \brief Install Ndn stack on all nodes in the simulation
   *
   * \returns list of installed faces in the form of a smart pointer
   * to FaceContainer object
   */
  Ptr<FaceContainer>
  InstallAll() const;

  /**
   * \brief Set flag indicating necessity to install default routes in FIB
   */
  void
  SetDefaultRoutes(bool needSet);

  static KeyChain&
  getKeyChain();

private:
  shared_ptr<NetDeviceFace>
  DefaultNetDeviceCallback(Ptr<Node> node, Ptr<L3Protocol> ndn, Ptr<NetDevice> netDevice) const;

  shared_ptr<NetDeviceFace>
  PointToPointNetDeviceCallback(Ptr<Node> node, Ptr<L3Protocol> ndn,
                                Ptr<NetDevice> netDevice) const;

public:
  void
  setCustomNdnCxxClocks();

private:
  StackHelper(const StackHelper&);
  StackHelper&
  operator=(const StackHelper& o);

private:
  ObjectFactory m_ndnFactory;
  ObjectFactory m_contentStoreFactory;

  bool m_needSetDefaultRoutes;
  bool m_shouldUseNfdCs;

  typedef std::list<std::pair<TypeId, NetDeviceFaceCreateCallback>> NetDeviceCallbackList;
  NetDeviceCallbackList m_netDeviceCallbacks;
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_STACK_HELPER_H */
