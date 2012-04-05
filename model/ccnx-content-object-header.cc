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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-content-object-header.h"

#include "ns3/log.h"
#include "../helper/ccnx-encoding-helper.h"
#include "../helper/ccnx-decoding-helper.h"

NS_LOG_COMPONENT_DEFINE ("CcnxContentObjectHeader");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (CcnxContentObjectHeader);
NS_OBJECT_ENSURE_REGISTERED (CcnxContentObjectTail);

TypeId
CcnxContentObjectHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentObjectHeader")
    .SetGroupName ("Ccnx")
    .SetParent<Header> ()
    .AddConstructor<CcnxContentObjectHeader> ()
    ;
  return tid;
}

CcnxContentObjectHeader::CcnxContentObjectHeader ()
{
}

void
CcnxContentObjectHeader::SetName (const Ptr<CcnxNameComponents> &name)
{
  m_name = name;
}

const CcnxNameComponents&
CcnxContentObjectHeader::GetName () const
{
  if (m_name==0) throw CcnxContentObjectHeaderException();
  return *m_name;
}

void
CcnxContentObjectHeader::SetTimestamp (const Time &timestamp)
{
  m_signedInfo.m_timestamp = timestamp;
}

Time
CcnxContentObjectHeader::GetTimestamp () const
{
  return m_signedInfo.m_timestamp;
}

void
CcnxContentObjectHeader::SetFreshness (const Time &freshness)
{
  m_signedInfo.m_freshness = freshness;
}

Time
CcnxContentObjectHeader::GetFreshness () const
{
  return m_signedInfo.m_freshness;
}


uint32_t
CcnxContentObjectHeader::GetSerializedSize (void) const
{
  // unfortunately, we don't know exact header size in advance
  return CcnxEncodingHelper::GetSerializedSize (*this);
}
    
void
CcnxContentObjectHeader::Serialize (Buffer::Iterator start) const
{
  CcnxEncodingHelper::Serialize (start, *this);
}

uint32_t
CcnxContentObjectHeader::Deserialize (Buffer::Iterator start)
{
  return CcnxDecodingHelper::Deserialize (start, *this); // \todo Debugging is necessary
}
  
TypeId
CcnxContentObjectHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
CcnxContentObjectHeader::Print (std::ostream &os) const
{
  os << "<ContentObject><Name>" << GetName () << "</Name><Content>";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

CcnxContentObjectTail::CcnxContentObjectTail ()
{
}

TypeId
CcnxContentObjectTail::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentObjectTail")
    .SetParent<Trailer> ()
    .AddConstructor<CcnxContentObjectTail> ()
    ;
  return tid;
}

TypeId
CcnxContentObjectTail::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
CcnxContentObjectTail::Print (std::ostream &os) const
{
  os << "</Content></ContentObject>";
}

uint32_t
CcnxContentObjectTail::GetSerializedSize (void) const
{
  return 2;
}

void
CcnxContentObjectTail::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.Prev (2); // Trailer interface requires us to go backwards
  
  i.WriteU8 (0x00); // </Content>
  i.WriteU8 (0x00); // </ContentObject>
}

uint32_t
CcnxContentObjectTail::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i.Prev (2); // Trailer interface requires us to go backwards

  uint8_t __attribute__ ((unused)) closing_tag_content = i.ReadU8 ();
  NS_ASSERT_MSG (closing_tag_content==0, "Should be a closing tag </Content> (0x00)");

  uint8_t __attribute__ ((unused)) closing_tag_content_object = i.ReadU8 ();
  NS_ASSERT_MSG (closing_tag_content_object==0, "Should be a closing tag </ContentObject> (0x00)");

  return 2;
}
  
} // namespace ns3
