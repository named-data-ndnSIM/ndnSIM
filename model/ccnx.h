/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef CCNX_H
#define CCNX_H

#include <stdint.h>
#include "ns3/object.h"
#include "ns3/socket.h"
#include "ns3/callback.h"

#include "ccnx-route.h"

namespace ns3 {

class Node;
class NetDevice;
class Packet;
// class CcnxRoutingProtocol;

/**
 * \ingroup internet
 * \defgroup ccnx Ccnx
 */
/**
 * \ingroup ccnx
 * \brief Access to the Ccnx forwarding table, interfaces, and configuration
 *
 * This class defines the API to manipulate the following aspects of
 * the Ccnx implementation:
 * -# register a NetDevice for use by the Ccnx layer (basically, to
 * create Ccnx-related state such as addressing and neighbor cache that 
 * is associated with a NetDevice)
 * -# manipulate the status of the NetDevice from the Ccnx perspective, 
 * such as marking it as Up or Down, 
 // * -# adding, deleting, and getting addresses associated to the Ccnx 
 // * interfaces.
 * -# exporting Ccnx configuration attributes
 * 
 * Each NetDevice has conceptually a single Ccnx interface associated
 * with it.
 */
class Ccnx : public Object
{
public:
  static TypeId GetTypeId (void);
  Ccnx ();
  virtual ~Ccnx ();

  /**
   * \brief Register a new routing protocol to be used by this Ccnx stack
   *
   * This call will replace any routing protocol that has been previously 
   * registered.  If you want to add multiple routing protocols, you must
   * add them to a CcnxListRoutingProtocol directly.
   * 
   * \param routingProtocol smart pointer to CcnxRoutingProtocol object
   */
  // virtual void SetRoutingProtocol (Ptr<CcnxRoutingProtocol> routingProtocol) = 0;

  /**
   * \brief Get the routing protocol to be used by this Ccnx stack
   * 
   * \returns smart pointer to CcnxRoutingProtocol object, or null pointer if none
   */
  // virtual Ptr<CcnxRoutingProtocol> GetRoutingProtocol (void) const = 0;

  /**
   * \param device device to add to the list of Ccnx interfaces
   *        which can be used as output interfaces during packet forwarding.
   * \returns the index of the Ccnx interface added.
   *
   * Once a device has been added, it can never be removed: if you want
   * to disable it, you can invoke Ccnx::SetDown which will
   * make sure that it is never used during packet forwarding.
   */
  virtual uint32_t AddInterface (Ptr<NetDevice> device) = 0;

  /**
   * \returns the number of interfaces added by the user.
   */
  virtual uint32_t GetNInterfaces (void) const = 0;

  /**
   * \param packet packet to send
   * \param route route entry
   *
   * Higher-level layers call this method to send a packet
   * down the stack to the MAC and PHY layers.
   */
  virtual void Send (Ptr<Packet> packet, Ptr<CcnxRoute> route) = 0;

  /**
   * \param interface The interface number of an Ccnx interface.
   * \returns The NetDevice associated with the Ccnx interface number.
   */
  virtual Ptr<NetDevice> GetNetDevice (uint32_t interface) = 0;

  /**
   * \param device The NetDevice for an CcnxInterface
   * \returns The interface number of an Ccnx interface or -1 if not found.
   */
  virtual int32_t GetInterfaceForDevice (Ptr<const NetDevice> device) const = 0;

  /**
   * \param interface The interface number of an Ccnx interface
   * \param metric routing metric (cost) associated to the underlying 
   *          Ccnx interface
   */
  virtual void SetMetric (uint32_t interface, uint16_t metric) = 0;

  /**
   * \param interface The interface number of an Ccnx interface
   * \returns routing metric (cost) associated to the underlying 
   *          Ccnx interface
   */
  virtual uint16_t GetMetric (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ccnx interface
   * \returns the Maximum Transmission Unit (in bytes) associated
   *          to the underlying Ccnx interface
   */
  virtual uint16_t GetMtu (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ccnx interface
   * \returns true if the underlying interface is in the "up" state,
   *          false otherwise.
   */
  virtual bool IsUp (uint32_t interface) const = 0;

  /**
   * \param interface Interface number of Ccnx interface
   * 
   * Set the interface into the "up" state. In this state, it is
   * considered valid during Ccnx forwarding.
   */
  virtual void SetUp (uint32_t interface) = 0;

  /**
   * \param interface Interface number of Ccnx interface
   *
   * Set the interface into the "down" state. In this state, it is
   * ignored during Ccnx forwarding.
   */
  virtual void SetDown (uint32_t interface) = 0;
};

} // namespace ns3 

#endif /* CCNX_H */
