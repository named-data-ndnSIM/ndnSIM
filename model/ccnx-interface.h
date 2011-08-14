/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Authors: 
 */
#ifndef CCNX_INTERFACE_H
#define CCNX_INTERFACE_H

#include <list>
#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

class NetDevice;
class Packet;
class Node;

/**
 * \brief The Ccnx representation of a network interface
 *
 * This class roughly corresponds to the struct in_device
 * of Linux; the main purpose is to provide address-family
 * specific information (addresses) about an interface.
 *
 * By default, Ccnx interface are created in the "down" state
 * no IP addresses.  Before becoming useable, the user must 
 * add an address of some type and invoke Setup on them.
 */
class CcnxInterface  : public Object
{
public:
  static TypeId GetTypeId (void);

  CcnxInterface ();
  virtual ~CcnxInterface();

  void SetNode (Ptr<Node> node); 
  void SetDevice (Ptr<NetDevice> device);

  /**
   * \returns the underlying NetDevice. This method cannot return zero.
   */
  Ptr<NetDevice> GetDevice (void) const;

  /**
   * \param metric configured routing metric (cost) of this interface
   *
   * Note:  This is synonymous to the Metric value that ifconfig prints
   * out.  It is used by ns-3 global routing, but other routing daemons
   * choose to ignore it. 
   */
  void SetMetric (uint16_t metric);

  /**
   * \returns configured routing metric (cost) of this interface
   *
   * Note:  This is synonymous to the Metric value that ifconfig prints
   * out.  It is used by ns-3 global routing, but other routing daemons 
   * may choose to ignore it. 
   */
  uint16_t GetMetric (void) const;

  /**
   * These are IP interface states and may be distinct from 
   * NetDevice states, such as found in real implementations
   * (where the device may be down but IP interface state is still up).
   */
  /**
   * \returns true if this interface is enabled, false otherwise.
   */
  bool IsUp (void) const;

  /**
   * \returns true if this interface is disabled, false otherwise.
   */
  bool IsDown (void) const;

  /**
   * Enable this interface
   */
  void SetUp (void);

  /**
   * Disable this interface
   */
  void SetDown (void);

  /**
   * \param p packet to send
   */ 
  void Send (Ptr<Packet> p);

protected:
  virtual void DoDispose (void);

private:
  bool m_ifup;
  uint16_t m_metric;
  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
};

} // namespace ns3

#endif
