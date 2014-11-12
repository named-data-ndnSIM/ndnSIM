/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_L3_PROTOCOL_H
#define NDN_L3_PROTOCOL_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"

#include <list>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/traced-callback.h"

namespace nfd {
class Forwarder;
class FibManager;
class StrategyChoiceManager;
} // namespace nfd

namespace ns3 {

class Packet;
class Node;
class Header;

namespace ndn {

/**
 * \defgroup ndn ndnSIM: NDN simulation module
 *
 * This is a modular implementation of NDN protocol for NS-3
 */
/**
 * \ingroup ndn
 * \brief Implementation network-layer of NDN stack
 *
 * This class defines the API to manipulate the following aspects of
 * the NDN stack implementation:
 * -# register a face (Face-derived object) for use by the NDN
 *    layer
 *
 * Each Face-derived object has conceptually a single NDN
 * interface associated with it.
 *
 * In addition, this class defines NDN packet coding constants
 *
 * \see Face, ForwardingStrategy
 */
class L3Protocol : boost::noncopyable, public Object {
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId
  GetTypeId();

  static const uint16_t ETHERNET_FRAME_TYPE; ///< @brief Ethernet Frame Type of Ndn
  static const uint16_t IP_STACK_PORT;       ///< @brief TCP/UDP port for NDN stack
  // static const uint16_t IP_PROTOCOL_TYPE;    ///< \brief IP protocol type of NDN

  /**
   * \brief Default constructor. Creates an empty stack without forwarding strategy set
   */
  L3Protocol();
  virtual ~L3Protocol();

  /**
   * \brief Initialize NFD instance
   */
  void
  initialize();

  /**
   * \brief Get smart pointer to nfd::Forwarder installed on the node
   */
  shared_ptr<nfd::Forwarder>
  getForwarder();

  /**
   * \brief Get smart pointer to nfd::FibManager, used by node's NFD
   */
  shared_ptr<nfd::FibManager>
  getFibManager();

  /**
   * \brief Get smart pointer to nfd::StrategyChoiceManager, used by node's NFD
   */
  shared_ptr<nfd::StrategyChoiceManager>
  getStrategyChoiceManager();


  /**
   * \brief Add face to NDN stack
   *
   * \param face smart pointer to Face-derived object (AppFace, NetDeviceFace)
   * \return nfd::FaceId
   *
   * \see AppFace, NetDeviceFace
   */
  nfd::FaceId
  addFace(shared_ptr<Face> face);

  /**
   * \brief Get face by face ID
   * \param face The face ID number
   * \returns The Face associated with the Ndn face number.
   */
  shared_ptr<Face>
  getFaceById(nfd::FaceId face) const;

  // /**
  //  * \brief Remove face from ndn stack (remove callbacks)
  //  */
  // virtual void
  // removeFace(shared_ptr<Face> face);

  /**
   * \brief Get face for NetDevice
   */
  shared_ptr<Face>
  getFaceByNetDevice(Ptr<NetDevice> netDevice) const;

protected:
  virtual void
  DoDispose(void); ///< @brief Do cleanup

  /**
   * This function will notify other components connected to the node that a new stack member is now
   * connected
   * This will be used to notify Layer 3 protocol of layer 4 protocol stack to connect them
   * together.
   */
  virtual void
  NotifyNewAggregate();

private:
  void
  initializeManagement();

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;

  // These objects are aggregated, but for optimization, get them here
  Ptr<Node> m_node; ///< \brief node on which ndn stack is installed

  TracedCallback<const Interest&, const Face&>
    m_inInterests; ///< @brief trace of incoming Interests
  TracedCallback<const Interest&, const Face&>
    m_outInterests; ///< @brief Transmitted interests trace

  TracedCallback<const Data&, const Face&> m_outData; ///< @brief trace of outgoing Data
  TracedCallback<const Data&, const Face&> m_inData;  ///< @brief trace of incoming Data
};

} // namespace ndn
} // namespace ns3

#endif /* NDN_L3_PROTOCOL_H */
