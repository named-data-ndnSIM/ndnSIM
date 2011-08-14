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

#include "ccnx-local-face.h"

#include "ns3/ccnx-address.h"
#include "ns3/ccnx-l3-protocol.h"
#include "ns3/net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/pointer.h"
#include "ns3/assert.h" 

NS_LOG_COMPONENT_DEFINE ("CcnxLocalFace");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxLocalFace);

TypeId 
CcnxLocalFace::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxLocalFace")
    .SetParent<Object> ()
  ;
  return tid;
}

/** 
 * By default, Ccnx interface are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must 
 * invoke SetUp on them once an Ccnx address and mask have been set.
 */
CcnxLocalFace::CcnxLocalFace () 
{
  NS_LOG_FUNCTION (this);
}

CcnxLocalFace::~CcnxLocalFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
CcnxLocalFace::DoDispose (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  CcnxFace::DoDispose ();
}

void 
CcnxLocalFace::SetDevice (Ptr<NetDevice> device)
{
  assert (false);
}

Ptr<NetDevice>
CcnxLocalFace::GetDevice (void) const
{
  assert (false)
}



void
CcnxLocalFace::Send (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (*p);
  if (!IsUp ())
    {
      return;
    }

  // m_device->Send (p, m_device->GetBroadcast (), 
  //                 CcnxL3Protocol::PROT_NUMBER);
}

std::ostream& operator<< (std::ostream& os, CcnxLocalFace const& localFace)
{
  os << "dev=local";
  return os;
}

}; // namespace ns3

