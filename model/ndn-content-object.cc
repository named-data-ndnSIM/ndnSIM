/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2013 University of California, Los Angeles
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

#include "ndn-content-object.h"

#include "ns3/log.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.ContentObject");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (ContentObject);
NS_OBJECT_ENSURE_REGISTERED (ContentObjectTail);

TypeId
ContentObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::ContentObject")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<ContentObject> ()
    ;
  return tid;
}

ContentObject::ContentObject ()
  : m_signature (0)
{
}

void
ContentObject::SetName (Ptr<Name> name)
{
  m_name = name;
}

void
ContentObject::SetName (const Name &name)
{
  m_name = Create<Name> (name);
}

const Name&
ContentObject::GetName () const
{
  if (m_name==0) throw ContentObjectException();
  return *m_name;
}

Ptr<const Name>
ContentObject::GetNamePtr () const
{
  return m_name;
}


void
ContentObject::SetTimestamp (const Time &timestamp)
{
  m_timestamp = timestamp;
}

Time
ContentObject::GetTimestamp () const
{
  return m_timestamp;
}
    
void
ContentObject::SetFreshness (const Time &freshness)
{
  m_freshness = freshness;
}

Time
ContentObject::GetFreshness () const
{
  return m_freshness;
}

void
ContentObject::SetSignature (uint32_t signature)
{
  m_signature = signature;
}

uint32_t
ContentObject::GetSignature () const
{
  return m_signature;
}

uint32_t
ContentObject::GetSerializedSize () const
{
  uint32_t size = 2 + ((2 + 2) + (m_name->GetSerializedSize ()) + (2 + 2 + 4 + 2 + 2 + (2 + 0)));
  if (m_signature != 0)
    size += 4;
  
  NS_LOG_INFO ("Serialize size = " << size);
  return size;
}

void
ContentObject::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x80); // version
  start.WriteU8 (0x01); // packet type

  if (m_signature != 0)
    {
      start.WriteU16 (6); // signature length
      start.WriteU16 (0xFF00); // "fake" simulator signature
      start.WriteU32 (m_signature);
    }
  else
    {
      start.WriteU16 (2); // signature length
      start.WriteU16 (0); // empty signature
    }

  // name
  uint32_t offset = m_name->Serialize (start);
  NS_LOG_DEBUG ("Offset: " << offset);
  start.Next (offset);

  // content
  // for now assume that contentdata length is zero
  start.WriteU16 (2 + 4 + 2 + 2 + (2 + 0));
  start.WriteU16 (4 + 2 + 2 + (2 + 0));
  start.WriteU32 (static_cast<uint32_t> (m_timestamp.ToInteger (Time::S)));
  start.WriteU16 (static_cast<uint16_t> (m_freshness.ToInteger (Time::S)));
  start.WriteU16 (0); // reserved 
  start.WriteU16 (0); // Length (ContentInfoOptions)

  // that's it folks
}


uint32_t
ContentObject::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;

  if (i.ReadU8 () != 0x80)
    throw new ContentObjectException ();

  if (i.ReadU8 () != 0x01)
    throw new ContentObjectException ();

  uint32_t signatureLength = i.ReadU16 ();
  if (signatureLength == 6)
    {
      if (i.ReadU16 () != 0xFF00) // signature type
        throw new ContentObjectException ();
      m_signature = i.ReadU32 ();
    }
  else if (signatureLength == 2)
    {
      if (i.ReadU16 () != 0) // signature type
        throw new ContentObjectException ();
      m_signature = 0;
    }
  else
    throw new ContentObjectException ();

  m_name = Create<Name> ();
  uint32_t offset = m_name->Deserialize (i);
  i.Next (offset);

  if (i.ReadU16 () != (2 + 4 + 2 + 2 + (2 + 0))) // content length
    throw new ContentObjectException ();

  if (i.ReadU16 () != (4 + 2 + 2 + (2 + 0))) // Length (content Info)
    throw new ContentObjectException ();

  m_timestamp = Seconds (i.ReadU32 ());
  m_freshness = Seconds (i.ReadU16 ());

  if (i.ReadU16 () != 0) // Reserved
    throw new ContentObjectException ();
  if (i.ReadU16 () != 0) // Length (ContentInfoOptions)
    throw new ContentObjectException ();

  NS_ASSERT_MSG (i.GetDistanceFrom (start) == GetSerializedSize (),
                 "Something wrong with ContentObject::Deserialize");
  
  return i.GetDistanceFrom (start);
}
  
TypeId
ContentObject::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
ContentObject::Print (std::ostream &os) const
{
  os << "D: " << GetName ();
  // os << "<ContentObject><Name>" << GetName () << "</Name><Content>";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ContentObjectTail::ContentObjectTail ()
{
}

TypeId
ContentObjectTail::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::ContentObjectTail")
    .SetParent<Trailer> ()
    .AddConstructor<ContentObjectTail> ()
    ;
  return tid;
}

TypeId
ContentObjectTail::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
ContentObjectTail::Print (std::ostream &os) const
{
  // os << "</Content></ContentObject>";
}

uint32_t
ContentObjectTail::GetSerializedSize (void) const
{
  return 0;
}

void
ContentObjectTail::Serialize (Buffer::Iterator start) const
{
}

uint32_t
ContentObjectTail::Deserialize (Buffer::Iterator start)
{
  return 0;
}

} // namespace ndn
} // namespace ns3
