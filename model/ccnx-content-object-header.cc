/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-content-object-header.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("CcnxContentObjectHeader");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (CcnxContentObjectHeader);

TypeId
CcnxContentObjectHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentObjectHeader")
    .SetParent<Header> ()
    .AddConstructor<CcnxContentObjectHeader> ()
    ;
  return tid;
}

CcnxContentObjectHeader::CcnxContentObjectHeader ()
{
}

void
CcnxContentObjectHeader::SetName (const Ptr<Name::Components> &name)
{
  m_name = name;
}

const Name::Components&
CcnxContentObjectHeader::GetName () const
{
  return *m_name;
}

uint32_t
CcnxContentObjectHeader::GetSerializedSize (void) const
{
  return 0;
}
    
void
CcnxContentObjectHeader::Serialize (Buffer::Iterator start) const
{
  return;
}

uint32_t
CcnxContentObjectHeader::Deserialize (Buffer::Iterator start)
{
  return 0;
}
  
TypeId
CcnxContentObjectHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
CcnxContentObjectHeader::Print (std::ostream &os) const
{
  os << "ContentObject: " << *m_name;
}

} // namespace ns3

