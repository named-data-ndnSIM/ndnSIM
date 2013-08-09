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

#include "ndn-data.h"

#include "ns3/log.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.Data");

namespace ns3 {
namespace ndn {

Data::Data (Ptr<Packet> payload/* = Create<Packet> ()*/)
  : m_name (Create<Name> ())
  , m_signature (0)
  , m_payload (payload)
  , m_keyLocator (0)
  , m_wire (0)
{
  if (m_payload == 0) // just in case
    {
      m_payload = Create<Packet> ();
    }
}

Data::Data (const Data &other)
  : m_name (Create<Name> (other.GetName ()))
  , m_freshness (other.GetFreshness ())
  , m_timestamp (other.GetTimestamp ())
  , m_signature (other.GetSignature ())
  , m_payload (other.GetPayload ()->Copy ())
  , m_wire (0)
{
  if (other.GetKeyLocator ())
    {
      m_keyLocator = Create<Name> (*other.GetKeyLocator ());
    }
}

void
Data::SetName (Ptr<Name> name)
{
  m_name = name;
  m_wire = 0;
}

void
Data::SetName (const Name &name)
{
  m_name = Create<Name> (name);
  m_wire = 0;
}

const Name&
Data::GetName () const
{
  if (m_name==0) throw DataException();
  return *m_name;
}

Ptr<const Name>
Data::GetNamePtr () const
{
  return m_name;
}


void
Data::SetTimestamp (const Time &timestamp)
{
  m_timestamp = timestamp;
  m_wire = 0;
}

Time
Data::GetTimestamp () const
{
  return m_timestamp;
}
    
void
Data::SetFreshness (const Time &freshness)
{
  m_freshness = freshness;
  m_wire = 0;
}


Time
Data::GetFreshness () const
{
  return m_freshness;
}

void
Data::SetSignature (uint32_t signature)
{
  m_signature = signature;
  m_wire = 0;
}

uint32_t
Data::GetSignature () const
{
  return m_signature;
}

void
Data::SetKeyLocator (Ptr<Name> keyLocator)
{
  m_keyLocator = keyLocator;
}

Ptr<const Name>
Data::GetKeyLocator () const
{
  return m_keyLocator;
}

void
Data::Print (std::ostream &os) const
{
  os << "D: " << GetName ();
  // os << "<Data><Name>" << GetName () << "</Name><Content>";
}

void
Data::SetPayload (Ptr<Packet> payload)
{
  m_payload = payload;
  m_wire = 0;
}

Ptr<const Packet>
Data::GetPayload () const
{
  return m_payload;
}

} // namespace ndn
} // namespace ns3
