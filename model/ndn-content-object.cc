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

ContentObject::ContentObject ()
  : m_signature (0)
{
}

ContentObject::ContentObject (const ContentObject &other)
  : m_name (Create<Name> (other.GetName ()))
  , m_freshness (other->GetFreshness ())
  , m_timestamp (other->GetTimestamp ())
  , m_signature (other->GetSignature ())
  , m_payload (other->GetPayload ()->Copy ())
  , m_wire (0)
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

void
ContentObject::Print (std::ostream &os) const
{
  os << "D: " << GetName ();
  // os << "<ContentObject><Name>" << GetName () << "</Name><Content>";
}

} // namespace ndn
} // namespace ns3
