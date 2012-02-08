/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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
 * Authors: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-name-components-tag.h"

namespace ns3
{

TypeId
CcnxNameComponentsTag::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxNameComponentsTag")
    .SetParent<Tag> ()
  ;
  return tid;
}

CcnxNameComponentsTag::CcnxNameComponentsTag (const Ptr<const CcnxNameComponents> &name)
  : m_name (name)
{
}
  
void
CcnxNameComponentsTag::SetName (const Ptr<const CcnxNameComponents> &name)
{
  m_name = name;
}

const Ptr<const CcnxNameComponents> &
CcnxNameComponentsTag::GetName () const
{
  return m_name;
}

uint32_t
CcnxNameComponentsTag::GetSerializedSize() const
{
  return 0;
}
  
void
CcnxNameComponentsTag::Serialize(TagBuffer) const
{
  // do nothing
}
  
void
CcnxNameComponentsTag::Deserialize(ns3::TagBuffer)
{
  // do nothing
}
  
void
CcnxNameComponentsTag::Print(std::ostream&) const
{
  // do nothing
}


} // namespace ns3
