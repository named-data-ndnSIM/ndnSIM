/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 */

#include "ndn-interest.h"

#include "ns3/log.h"
#include "ns3/unused.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE ("ndn.Interest");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (Interest);

TypeId
Interest::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::Interest")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<Interest> ()
    ;
  return tid;
}
  

Interest::Interest ()
  : m_name ()
  , m_scope (0xFF)
  , m_interestLifetime (Seconds (0))
  , m_nonce (0)
  , m_nackType (NORMAL_INTEREST)
{
}

Interest::Interest (const Interest &interest)
  : m_name                (Create<Name> (interest.GetName ()))
  , m_scope               (interest.m_scope)
  , m_interestLifetime    (interest.m_interestLifetime)
  , m_nonce               (interest.m_nonce)
  , m_nackType            (interest.m_nackType)
{
}

Ptr<Interest>
Interest::GetInterest (Ptr<Packet> packet)
{
  Ptr<Interest> interest = Create<Interest> ();
  packet->RemoveHeader (*interest);

  return interest;
}

void
Interest::SetName (Ptr<Name> name)
{
  m_name = name;
}

void
Interest::SetName (const Name &name)
{
  m_name = Create<Name> (name);
}

const Name&
Interest::GetName () const
{
  if (m_name==0) throw InterestException();
  return *m_name;
}

Ptr<const Name>
Interest::GetNamePtr () const
{
  return m_name;
}

void
Interest::SetScope (int8_t scope)
{
  m_scope = scope;
}

int8_t
Interest::GetScope () const
{
  return m_scope;
}

void
Interest::SetInterestLifetime (Time lifetime)
{
  m_interestLifetime = lifetime;
}

Time
Interest::GetInterestLifetime () const
{
  return m_interestLifetime;
}

void
Interest::SetNonce (uint32_t nonce)
{
  m_nonce = nonce;
}

uint32_t
Interest::GetNonce () const
{
  return m_nonce;
}

void
Interest::SetNack (uint8_t nackType)
{
  m_nackType = nackType;
}

uint8_t
Interest::GetNack () const
{
  return m_nackType;
}

uint32_t
Interest::GetSerializedSize (void) const
{
  size_t size = 2 + (1 + 4 + 2 + 1 + (m_name->GetSerializedSize ()) + (2 + 0) + (2 + 0));
  NS_LOG_INFO ("Serialize size = " << size);

  return size;
}
    
void
Interest::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (0x80); // version
  start.WriteU8 (0x00); // packet type

  start.WriteU32 (m_nonce);
  start.WriteU8 (m_scope);
  start.WriteU8 (m_nackType);

  NS_ASSERT_MSG (0 <= m_interestLifetime.ToInteger (Time::S) && m_interestLifetime.ToInteger (Time::S) < 65535,
                 "Incorrect InterestLifetime (should not be smaller than 0 and larger than 65535");
  
  // rounding timestamp value to seconds
  start.WriteU16 (static_cast<uint16_t> (m_interestLifetime.ToInteger (Time::S)));

  uint32_t offset = m_name->Serialize (start);
  start.Next (offset);
  
  start.WriteU16 (0); // no selectors
  start.WriteU16 (0); // no options
}

uint32_t
Interest::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  
  if (i.ReadU8 () != 0x80)
    throw new InterestException ();

  if (i.ReadU8 () != 0x00)
    throw new InterestException ();

  m_nonce = i.ReadU32 ();
  m_scope = i.ReadU8 ();
  m_nackType = i.ReadU8 ();
  
  m_interestLifetime = Seconds (i.ReadU16 ());

  m_name = Create<Name> ();
  uint32_t offset = m_name->Deserialize (i);
  i.Next (offset);
  
  i.ReadU16 ();
  i.ReadU16 ();

  NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

  return i.GetDistanceFrom (start);
}

TypeId
Interest::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
Interest::Print (std::ostream &os) const
{
  os << "I: " << GetName ();
  
  return;
  os << "<Interest>\n  <Name>" << GetName () << "</Name>\n";
  if (GetNack ()>0)
    {
      os << "  <NACK>";
      switch (GetNack ())
        {
        case NACK_LOOP:
          os << "loop";
          break;
        case NACK_CONGESTION:
          os << "congestion";
          break;
        default:
          os << "unknown";
          break;
        }
      os << "</NACK>\n";
    }
  os << "  <Scope>" << GetScope () << "</Scope>\n";
  if ( !GetInterestLifetime ().IsZero() )
    os << "  <InterestLifetime>" << GetInterestLifetime () << "</InterestLifetime>\n";
  if (GetNonce ()>0)
    os << "  <Nonce>" << GetNonce () << "</Nonce>\n";
  os << "</Interest>";
}

} // namespace ndn
} // namespace ns3

