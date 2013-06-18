/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
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
 */

#include "ndn-local-info-tag.h"
#include <ns3/ndn-face.h>

namespace ns3 {
namespace ndn {

LocalInfoTag::LocalInfoTag (Ptr<Face> inFace)
{
  if (inFace != 0)
    {
      m_faceId = inFace->GetId ();
    }
  else
    m_faceId = CACHE_FACE;
}

LocalInfoTag::~LocalInfoTag ()
{
}


TypeId
LocalInfoTag::GetTypeId ()
{
  static TypeId tid = TypeId("ns3::ndn::LocalInfo")
    .SetParent<Tag>()
    ;
  return tid;
}

TypeId
LocalInfoTag::GetInstanceTypeId () const
{
  return LocalInfoTag::GetTypeId ();
}

uint32_t
LocalInfoTag::GetSerializedSize () const
{
  return sizeof (uint32_t);
}

void
LocalInfoTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_faceId);
}
  
void
LocalInfoTag::Deserialize (TagBuffer i)
{
  m_faceId = i.ReadU32 ();
}

void
LocalInfoTag::Print (std::ostream &os) const
{
  os << m_faceId;
}

} // namespace ndn
} // namespace ns3
