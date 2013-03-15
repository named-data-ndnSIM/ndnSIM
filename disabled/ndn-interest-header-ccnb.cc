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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

///< #CCN_PR_SCOPE0 (0x20) local scope,
///< #CCN_PR_SCOPE1 (0x40) this host,
///< #CCN_PR_SCOPE2 (0x80) immediate neighborhood

#include "ndn-interest-header-ccnb.h"

#include "ns3/log.h"
#include "ns3/unused.h"
#include "ns3/packet.h"
#include "../helper/ndn-encoding-helper.h"
#include "../helper/ndn-decoding-helper.h"

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
  , m_minSuffixComponents (-1)
  , m_maxSuffixComponents (-1)
  , m_exclude ()
  , m_childSelector (false)
  , m_answerOriginKind (false)
  , m_scope (-1)
  , m_interestLifetime (Seconds (0))
  , m_nonce (0)
  , m_nackType (NORMAL_INTEREST)
{
}

Interest::Interest (const Interest &interest)
  : m_name                (Create<Name> (interest.GetName ()))
  , m_minSuffixComponents (interest.m_minSuffixComponents)
  , m_maxSuffixComponents (interest.m_maxSuffixComponents)
  , m_exclude             (interest.IsEnabledExclude () ? Create<Name> (interest.GetExclude ()) : 0)
  , m_childSelector       (interest.m_childSelector)
  , m_answerOriginKind    (interest.m_answerOriginKind)
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
Interest::SetMinSuffixComponents (int32_t value)
{
  m_minSuffixComponents = value;
}

int32_t
Interest::GetMinSuffixComponents () const
{
  return m_minSuffixComponents;
}

void
Interest::SetMaxSuffixComponents (int32_t value)
{
  m_maxSuffixComponents = value;
}

int32_t
Interest::GetMaxSuffixComponents () const
{
  return m_maxSuffixComponents;
}

void
Interest::SetExclude (Ptr<Name> exclude)
{
  m_exclude = exclude;
}

bool
Interest::IsEnabledExclude () const
{
  return m_exclude!=0;
}

const Name&
Interest::GetExclude () const
{
  if (m_exclude==0) throw InterestException();
  return *m_exclude;
}

void
Interest::SetChildSelector (bool value)
{
  m_childSelector = value;
}

bool
Interest::IsEnabledChildSelector () const
{
  return m_childSelector;
}

void
Interest::SetAnswerOriginKind (bool value)
{
  m_answerOriginKind = value;
}

bool
Interest::IsEnabledAnswerOriginKind () const
{
  return m_answerOriginKind;
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
Interest::SetNack (uint32_t nackType)
{
  m_nackType = nackType;
}

uint32_t
Interest::GetNack () const
{
  return m_nackType;
}

uint32_t
Interest::GetSerializedSize (void) const
{
  // unfortunately, we don't know exact header size in advance
  return EncodingHelper::GetSerializedSize (*this);
}
    
void
Interest::Serialize (Buffer::Iterator start) const
{
  size_t size = EncodingHelper::Serialize (start, *this);
  NS_UNUSED (size);
  NS_LOG_INFO ("Serialize size = " << size);
}

uint32_t
Interest::Deserialize (Buffer::Iterator start)
{
  return DecodingHelper::Deserialize (start, *this); // \todo Debugging is necessary
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
  if (GetMinSuffixComponents () >= 0)
    os << "  <MinSuffixComponents>" << GetMinSuffixComponents () << "</MinSuffixComponents>\n";
  if (GetMaxSuffixComponents () >= 0)
    os << "  <MaxSuffixComponents>" << m_maxSuffixComponents << "</MaxSuffixComponents>\n";
  if (IsEnabledExclude () && GetExclude ().size()>0)
    os << "  <Exclude>" << GetExclude () << "</Exclude>\n";
  if (IsEnabledChildSelector ())
    os << "  <ChildSelector />\n";
  if (IsEnabledAnswerOriginKind ())
    os << "  <AnswerOriginKind />\n";
  if (GetScope () >= 0)
    os << "  <Scope>" << GetScope () << "</Scope>\n";
  if ( !GetInterestLifetime ().IsZero() )
    os << "  <InterestLifetime>" << GetInterestLifetime () << "</InterestLifetime>\n";
  if (GetNonce ()>0)
    os << "  <Nonce>" << GetNonce () << "</Nonce>\n";
  os << "</Interest>";
}

} // namespace ndn
} // namespace ns3

