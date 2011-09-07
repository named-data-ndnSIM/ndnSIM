/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *
 */

#include "ccnx-face.h"

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/assert.h"

NS_LOG_COMPONENT_DEFINE ("CcnxFace");

namespace ns3 {

// NS_OBJECT_ENSURE_REGISTERED (CcnxFace);

// TypeId 
// CcnxFace::GetTypeId (void)
// {
//   static TypeId tid = TypeId ("ns3::CcnxFace")
//     .SetGroupName ("Ccnx")
//     .SetParent<Object> ()
//   ;
//   return tid;
// }

/** 
 * By default, Ccnx face are created in the "down" state
 *  with no IP addresses.  Before becoming useable, the user must 
 * invoke SetUp on them once an Ccnx address and mask have been set.
 */
CcnxFace::CcnxFace () 
  // : m_metric (1)
  : m_node (0)
  , m_ifup (false)
  , m_id ((uint32_t)-1)
{
  NS_LOG_FUNCTION (this);
}

CcnxFace::~CcnxFace ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

CcnxFace::CcnxFace (const CcnxFace &)
{
}

CcnxFace& CcnxFace::operator= (const CcnxFace &)
{
  return *this;
}

  
// void
// CcnxFace::DoDispose (void)
// {
//   NS_LOG_FUNCTION_NOARGS ();
//   m_node = 0;
//   Object::DoDispose ();
// }

void 
CcnxFace::SetNode (Ptr<Node> node)
{
  m_node = node;
}

// void
// CcnxFace::SetMetric (uint16_t metric)
// {
//   NS_LOG_FUNCTION (metric);
//   m_metric = metric;
// }

// uint16_t
// CcnxFace::GetMetric (void) const
// {
//   NS_LOG_FUNCTION_NOARGS ();
//   return m_metric;
// }

/**
 * These are face states and may be distinct from 
 * NetDevice states, such as found in real implementations
 * (where the device may be down but face state is still up).
 */
bool 
CcnxFace::IsUp (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifup;
}

bool 
CcnxFace::IsDown (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return !m_ifup;
}

void 
CcnxFace::SetUp (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifup = true;
}

void 
CcnxFace::SetDown (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifup = false;
}

bool
CcnxFace::operator== (const CcnxFace &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (), "Faces of different nodes should not be compared to each other");

  return (m_id == face.m_id);
}

bool
CcnxFace::operator< (const CcnxFace &face) const
{
  NS_ASSERT_MSG (m_node->GetId () == face.m_node->GetId (), "Faces of different nodes should not be compared to each other");

  return (m_id < face.m_id);
}

std::ostream&
CcnxFace::Print (std::ostream &os) const
{
  os << "id=" << GetId ();
  return os;
}

std::ostream& operator<< (std::ostream& os, const CcnxFace &face)
{
  face.Print (os);
  return os;
}

}; // namespace ns3

