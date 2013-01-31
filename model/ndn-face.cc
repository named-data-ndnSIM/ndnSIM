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
 *
 */

#include "ndn-face.h"

#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/pointer.h"

#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

#include <boost/ref.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.Face");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (Face);

TypeId
Face::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Face")
    .SetParent<Object> ()
    .SetGroupName ("Ndn")
    .AddAttribute ("Id", "Face id (unique integer for the Ndn stack on this node)",
                   TypeId::ATTR_GET, // allow only getting it.
                   UintegerValue (0),
                   MakeUintegerAccessor (&Face::m_id),
                   MakeUintegerChecker<uint32_t> ())

    .AddTraceSource ("NdnTx", "Transmitted packet trace",
                     MakeTraceSourceAccessor (&Face::m_txTrace))
    .AddTraceSource ("NdnRx", "Received packet trace",
                     MakeTraceSourceAccessor (&Face::m_rxTrace))
    .AddTraceSource ("NdnDrop", "Dropped packet trace",
                     MakeTraceSourceAccessor (&Face::m_dropTrace))
    ;
  return tid;
}

/**
 * By default, Ndn face are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must
 * invoke SetUp on them once an Ndn address and mask have been set.
 */
Face::Face (Ptr<Node> node)
  : m_node (node)
  , m_protocolHandler (MakeNullCallback<void,const Ptr<Face>&,const Ptr<const Packet>&> ())
  , m_ifup (false)
  , m_id ((uint32_t)-1)
  , m_metric (0)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT_MSG (node != 0, "node cannot be NULL. Check the code");
}

Face::~Face ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Face::Face (const Face &)
{
}

Face& Face::operator= (const Face &)
{
  return *this;
}

Ptr<Node>
Face::GetNode () const
{
  return m_node;
}

void
Face::RegisterProtocolHandler (ProtocolHandler handler)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_protocolHandler = handler;
}

bool
Face::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (boost::cref (*this) << packet << packet->GetSize ());
  NS_LOG_DEBUG (*packet);

  if (!IsUp ())
    {
      m_dropTrace (packet);
      return false;
    }

  FwHopCountTag hopCount;
  bool tagExists = packet->RemovePacketTag (hopCount);
  if (tagExists)
    {
      hopCount.Increment ();
      packet->AddPacketTag (hopCount);
    }

  bool ok = SendImpl (packet);
  if (ok)
    {
      m_txTrace (packet);
      return true;
    }
  else
    {
      m_dropTrace (packet);
      return false;
    }
}

bool
Face::Receive (const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (boost::cref (*this) << packet << packet->GetSize ());

  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_rxTrace (packet);
  m_protocolHandler (this, packet);

  return true;
}

void
Face::SetMetric (uint16_t metric)
{
  NS_LOG_FUNCTION (metric);
  m_metric = metric;
}

uint16_t
Face::GetMetric (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_metric;
}

/**
 * These are face states and may be distinct from
 * NetDevice states, such as found in real implementations
 * (where the device may be down but face state is still up).
 */

bool
Face::IsUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifup;
}

void
Face::SetUp (bool up/* = true*/)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifup = up;
}

bool
Face::operator== (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other: " << *this << " == " << face);

  return (m_id == face.m_id);
}

bool
Face::operator< (const Face &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (),
                 "Faces of different nodes should not be compared to each other: " << *this << " == " << face);

  return (m_id < face.m_id);
}

std::ostream&
Face::Print (std::ostream &os) const
{
  os << "id=" << GetId ();
  return os;
}

std::ostream&
operator<< (std::ostream& os, const Face &face)
{
  face.Print (os);
  return os;
}

} // namespace ndn
} // namespace ns3

