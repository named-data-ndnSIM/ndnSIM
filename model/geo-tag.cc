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

#include "geo-tag.h"

namespace ns3
{

TypeId
GeoTag::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::GeoTag")
    .SetParent<Tag> ()
  ;
  return tid;
}

TypeId
GeoTransmissionTag::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::GeoTransmissionTag")
    .SetParent<GeoTag> ()
  ;
  return tid;
}

TypeId
GeoSrcTag::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::GeoSrcTag")
    .SetParent<GeoTag> ()
  ;
  return tid;
}

GeoTag::GeoTag (const Vector &position)
  : m_position (position)
{
}
  
void
GeoTag::SetPosition (const Vector &position)
{
  m_position = position;
}
  
const Vector &
GeoTag::GetPosition () const
{
  return m_position;
}

uint32_t
GeoTag::GetSerializedSize() const
{
  return 0;
}
  
void
GeoTag::Serialize(TagBuffer) const
{
  // do nothing
}
  
void
GeoTag::Deserialize(ns3::TagBuffer)
{
  // do nothing
}
  
void
GeoTag::Print(std::ostream&) const
{
  // do nothing
}


} // namespace ns3
