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

#include "ns3/ndn-header-helper.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"
#include "ns3/ndnSIM/model/wire/ndnsim.h"
#include "ns3/ndnSIM/model/wire/ccnb.h"

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

    .AddAttribute ("WireFormat",
                   "Default wire format for the face.  The face will be accepting packets "
                   "in any supported packet formats, but if encoding requested, it will "
                   "use default wire format to encode (0 for ndnSIM (default), 1 for CCNb)",
                   UintegerValue (WIRE_FORMAT_NDNSIM),
                   MakeUintegerAccessor (&Face::m_wireFormat),
                   MakeUintegerChecker<uint32_t> ())
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
  , m_upstreamInterestHandler (MakeNullCallback< void, Ptr<Face>, Ptr<Interest> > ())
  , m_upstreamDataHandler (MakeNullCallback< void, Ptr<Face>, Ptr<ContentObject> > ())
  , m_ifup (false)
  , m_id ((uint32_t)-1)
  , m_metric (0)
  , m_flags (0)
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
Face::RegisterProtocolHandlers (const InterestHandler &interestHandler, const DataHandler &dataHandler)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_upstreamInterestHandler = interestHandler;
  m_upstreamDataHandler = dataHandler;
}

void
Face::UnRegisterProtocolHandlers ()
{
  NS_LOG_FUNCTION_NOARGS ();

  m_upstreamInterestHandler = MakeNullCallback< void, Ptr<Face>, Ptr<Interest> > ();
  m_upstreamDataHandler = MakeNullCallback< void, Ptr<Face>, Ptr<ContentObject> > ();
}


bool
Face::SendInterest (Ptr<const Interest> interest)
{
  NS_LOG_FUNCTION (this << boost::cref (*this) << interest->GetName ());

  if (!IsUp ())
    {
      return false;
    }

  Ptr<Packet> packet;
  if (m_wireFormat == WIRE_FORMAT_NDNSIM)
    packet = wire::ndnSIM::Interest::ToWire (interest);
  else if (m_wireFormat == WIRE_FORMAT_CCNB)
    packet = wire::ccnb::Interest::ToWire (interest);
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
    }
  return Send (packet);
}

bool
Face::SendData (Ptr<const ContentObject> data)
{
  NS_LOG_FUNCTION (this << data);

  if (!IsUp ())
    {
      return false;
    }

  Ptr<Packet> packet;
  if (m_wireFormat == WIRE_FORMAT_NDNSIM)
    packet = wire::ndnSIM::Data::ToWire (data);
  else if (m_wireFormat == WIRE_FORMAT_CCNB)
    packet = wire::ccnb::Data::ToWire (data);
  else
    {
      NS_FATAL_ERROR ("Unsupported format requested");
    }
  return Send (packet);
}

bool
Face::Send (Ptr<Packet> packet)
{
  FwHopCountTag hopCount;
  bool tagExists = packet->RemovePacketTag (hopCount);
  if (tagExists)
    {
      hopCount.Increment ();
      packet->AddPacketTag (hopCount);
    }

  return true;
}

bool
Face::Receive (Ptr<const Packet> p)
{
  NS_LOG_FUNCTION (this << p << p->GetSize ());

  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  Ptr<Packet> packet = p->Copy (); // give upper layers a rw copy of the packet
  try
    {
      HeaderHelper::Type type = HeaderHelper::GetNdnHeaderType (packet);
      switch (type)
        {
        case HeaderHelper::INTEREST_NDNSIM:
          {
            Ptr<Interest> interest = wire::ndnSIM::Interest::FromWire (packet);
            return ReceiveInterest (interest);
          }
        case HeaderHelper::CONTENT_OBJECT_NDNSIM:
          {
            Ptr<ContentObject> data = wire::ndnSIM::Data::FromWire (packet);
            return ReceiveData (data);
          }
        case HeaderHelper::INTEREST_CCNB:
          {
            Ptr<Interest> interest = wire::ccnb::Interest::FromWire (packet);
            return ReceiveInterest (interest);
          }
        case HeaderHelper::CONTENT_OBJECT_CCNB:
          {
            Ptr<ContentObject> data = wire::ccnb::Data::FromWire (packet);
            return ReceiveData (data);
          }
        }

      // exception will be thrown if packet is not recognized
    }
  catch (UnknownHeaderException)
    {
      NS_ASSERT_MSG (false, "Unknown NDN header. Should not happen");
      NS_LOG_ERROR ("Unknown NDN header. Should not happen");
      return false;
    }
}

bool
Face::ReceiveInterest (Ptr<Interest> interest)
{
  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_upstreamInterestHandler (this, interest);
  return true;
}

bool
Face::ReceiveData (Ptr<ContentObject> data)
{
  if (!IsUp ())
    {
      // no tracing here. If we were off while receiving, we shouldn't even know that something was there
      return false;
    }

  m_upstreamDataHandler (this, data);
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
  return m_metric;
}

void
Face::SetFlags (uint32_t flags)
{
  m_flags = flags;
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

