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
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ndn-l3-protocol.hpp"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/uinteger.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/object-vector.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"

#include "ndn-face.hpp"
#include "ndn-net-device-face.hpp"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE("ndn.L3Protocol");

namespace ns3 {
namespace ndn {

const uint16_t L3Protocol::ETHERNET_FRAME_TYPE = 0x7777;
const uint16_t L3Protocol::IP_STACK_PORT = 9695;

NS_OBJECT_ENSURE_REGISTERED(L3Protocol);

TypeId
L3Protocol::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::L3Protocol")
      .SetGroupName("ndn")
      .SetParent<Object>()
      .AddConstructor<L3Protocol>()
    ;
      // .AddAttribute("FaceList", "List of faces associated with ndn stack", ObjectVectorValue(),
      //               MakeObjectVectorAccessor(&L3Protocol::m_faces),
      //               MakeObjectVectorChecker<Face>());
  return tid;
}

L3Protocol::L3Protocol()
{
  NS_LOG_FUNCTION(this);
}

L3Protocol::~L3Protocol()
{
  NS_LOG_FUNCTION(this);
}

/*
 * This method is called by AddAgregate and completes the aggregation
 * by setting the node in the ndn stack
 */
void
L3Protocol::NotifyNewAggregate()
{
  // not really efficient, but this will work only once
  if (m_node == 0) {
    m_node = GetObject<Node>();
    if (m_node != 0) {
    }
  }

  Object::NotifyNewAggregate ();
}

void
L3Protocol::DoDispose(void)
{
  NS_LOG_FUNCTION(this);

  m_node = 0;

  Object::DoDispose();
}

uint32_t
L3Protocol::AddFace(const shared_ptr<Face>& face)
{
  NS_LOG_FUNCTION(this << &face);

  return 0;
}

void
L3Protocol::RemoveFace(shared_ptr<Face> face)
{
  NS_LOG_FUNCTION(this << std::cref(*face));
}

shared_ptr<Face>
L3Protocol::GetFace(uint32_t index) const
{
  return nullptr;
}

shared_ptr<Face>
L3Protocol::GetFaceById(uint32_t index) const
{
  return nullptr;
}

shared_ptr<Face>
L3Protocol::GetFaceByNetDevice(Ptr<NetDevice> netDevice) const
{
  return nullptr;
}

uint32_t
L3Protocol::GetNFaces(void) const
{
  return 0;
}

} // namespace ndn
} // namespace ns3
