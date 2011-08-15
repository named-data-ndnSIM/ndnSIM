/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef CCNX_FACE_H
#define CCNX_FACE_H

#include <list>
#include <ostream>

#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

class NetDevice;
class Packet;
class Node;

/**
 * \brief The Ccnx representation of a network face
 *
 * This class roughly corresponds to the struct in_device
 * of Linux; the main purpose is to provide address-family
 * specific information (addresses) about an face.
 *
 * By default, Ccnx face are created in the "down" state
 * no IP addresses.  Before becoming useable, the user must 
 * add an address of some type and invoke Setup on them.
 */
class CcnxFace  : public Object
{
public:
  static TypeId GetTypeId (void);

  CcnxFace ();
  virtual ~CcnxFace();

  virtual void SetNode (Ptr<Node> node); 
  virtual void SetDevice (Ptr<NetDevice> device);

  /**
   * \returns the underlying NetDevice. This method cannot return zero.
   */
  virtual Ptr<NetDevice> GetDevice (void) const;

  /**
   * \param metric configured routing metric (cost) of this face
   *
   * Note:  This is synonymous to the Metric value that ifconfig prints
   * out.  It is used by ns-3 global routing, but other routing daemons
   * choose to ignore it. 
   */
  virtual void SetMetric (uint16_t metric);

  /**
   * \returns configured routing metric (cost) of this face
   *
   * Note:  This is synonymous to the Metric value that ifconfig prints
   * out.  It is used by ns-3 global routing, but other routing daemons 
   * may choose to ignore it. 
   */
  virtual uint16_t GetMetric (void) const;

  /**
   * These are IP face states and may be distinct from 
   * NetDevice states, such as found in real implementations
   * (where the device may be down but IP face state is still up).
   */
  /**
   * \returns true if this face is enabled, false otherwise.
   */
  virtual bool IsUp (void) const;

  /**
   * \returns true if this face is disabled, false otherwise.
   */
  virtual bool IsDown (void) const;

  /**
   * Enable this face
   */
  virtual void SetUp (void);

  /**
   * Disable this face
   */
  virtual void SetDown (void);

  /**
   * \param p packet to send
   */ 
  virtual void Send (Ptr<Packet> p);

protected:
  virtual void DoDispose (void);

private:
  bool m_ifup;
  uint16_t m_metric;
  Ptr<Node> m_node;
  Ptr<NetDevice> m_device;
};

std::ostream& operator<< (std::ostream& os, CcnxFace const& face);

} // namespace ns3

#endif //CCNX_FACE_H
