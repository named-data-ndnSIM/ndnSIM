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
 * Author:
 *
 */

#include "ccnx-interface.h"

#include "ns3/ccnx-address.h"
#include "ns3/ccnx-l3-protocol.h"
#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"

NS_LOG_COMPONENT_DEFINE ("CcnxInterface");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxInterface);

TypeId 
CcnxInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxInterface")
    .SetParent<Object> ()
  ;
  return tid;
}

/** 
 * By default, Ccnx interface are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must 
 * invoke SetUp on them once an Ccnx address and mask have been set.
 */
CcnxInterface::CcnxInterface () 
  : m_ifup (false),
    m_metric (1),
    m_node (0), 
    m_device (0),
{
  NS_LOG_FUNCTION (this);
}

CcnxInterface::~CcnxInterface ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CcnxInterface::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = 0;
  m_device = 0;
  Object::DoDispose ();
}

void 
CcnxInterface::SetNode (Ptr<Node> node)
{
  m_node = node;
}

void 
CcnxInterface::SetDevice (Ptr<NetDevice> device)
{
  m_device = device;
}

Ptr<NetDevice>
CcnxInterface::GetDevice (void) const
{
  return m_device;
}

void
CcnxInterface::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION (metric);
  m_metric = metric;
}

uint16_t
CcnxInterface::GetMetric (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_metric;
}

/**
 * These are interface states and may be distinct from 
 * NetDevice states, such as found in real implementations
 * (where the device may be down but interface state is still up).
 */
bool 
CcnxInterface::IsUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifup;
}

bool 
CcnxInterface::IsDown (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return !m_ifup;
}

void 
CcnxInterface::SetUp (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifup = true;
}

void 
CcnxInterface::SetDown (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifup = false;
}

void
CcnxInterface::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (*p);
  if (!IsUp ())
    {
      return;
    }

  // Check if Local Delivery
  // if (DynamicCast<LoopbackNetDevice> (m_device))
  //   {
  //     // XXX additional checks needed here (such as whether multicast
  //     // goes to loopback)?
  //     m_device->Send (p, m_device->GetBroadcast (), 
  //                     CcnxL3Protocol::PROT_NUMBER);
  //     return;
  //   }

  NS_LOG_LOGIC ("Doesn't need ARP");
  m_device->Send (p, m_device->GetBroadcast (), 
                  CcnxL3Protocol::PROT_NUMBER);
}

}; // namespace ns3

