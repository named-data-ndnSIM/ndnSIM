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

#include "ndn-interest-header.h"

#include "ns3/log.h"
#include "ns3/unused.h"
#include "../helper/ndn-encoding-helper.h"
#include "../helper/ndn-decoding-helper.h"

NS_LOG_COMPONENT_DEFINE ("NdnInterestHeader");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (NdnInterestHeader);

TypeId
NdnInterestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnInterestHeader")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<NdnInterestHeader> ()
    ;
  return tid;
}
  

NdnInterestHeader::NdnInterestHeader ()
  : m_minSuffixComponents (-1)
  , m_maxSuffixComponents (-1)
  , m_childSelector (false)
  , m_answerOriginKind (false)
  , m_scope (-1)
  , m_interestLifetime (Seconds (0))
  , m_nonce (0)
  , m_nackType (NORMAL_INTEREST)
{
}

void
NdnInterestHeader::SetName (const Ptr<NdnNameComponents> &name)
{
  m_name = name;
}

const NdnNameComponents&
NdnInterestHeader::GetName () const
{
  if (m_name==0) throw NdnInterestHeaderException();
  return *m_name;
}

Ptr<const NdnNameComponents>
NdnInterestHeader::GetNamePtr () const
{
  return m_name;
}

void
NdnInterestHeader::SetMinSuffixComponents (int32_t value)
{
  m_minSuffixComponents = value;
}

int32_t
NdnInterestHeader::GetMinSuffixComponents () const
{
  return m_minSuffixComponents;
}

void
NdnInterestHeader::SetMaxSuffixComponents (int32_t value)
{
  m_maxSuffixComponents = value;
}

int32_t
NdnInterestHeader::GetMaxSuffixComponents () const
{
  return m_maxSuffixComponents;
}

void
NdnInterestHeader::SetExclude (const Ptr<NdnNameComponents> &exclude)
{
  m_exclude = exclude;
}

bool
NdnInterestHeader::IsEnabledExclude () const
{
  return m_exclude!=0;
}

const NdnNameComponents&
NdnInterestHeader::GetExclude () const
{
  if (m_exclude==0) throw NdnInterestHeaderException();
  return *m_exclude;
}

void
NdnInterestHeader::SetChildSelector (bool value)
{
  m_childSelector = value;
}

bool
NdnInterestHeader::IsEnabledChildSelector () const
{
  return m_childSelector;
}

void
NdnInterestHeader::SetAnswerOriginKind (bool value)
{
  m_answerOriginKind = value;
}

bool
NdnInterestHeader::IsEnabledAnswerOriginKind () const
{
  return m_answerOriginKind;
}

void
NdnInterestHeader::SetScope (int8_t scope)
{
  m_scope = scope;
}

int8_t
NdnInterestHeader::GetScope () const
{
  return m_scope;
}

void
NdnInterestHeader::SetInterestLifetime (Time lifetime)
{
  m_interestLifetime = lifetime;
}

Time
NdnInterestHeader::GetInterestLifetime () const
{
  return m_interestLifetime;
}

void
NdnInterestHeader::SetNonce (uint32_t nonce)
{
  m_nonce = nonce;
}

uint32_t
NdnInterestHeader::GetNonce () const
{
  return m_nonce;
}

void
NdnInterestHeader::SetNack (uint32_t nackType)
{
  m_nackType = nackType;
}

uint32_t
NdnInterestHeader::GetNack () const
{
  return m_nackType;
}

uint32_t
NdnInterestHeader::GetSerializedSize (void) const
{
  // unfortunately, we don't know exact header size in advance
  return NdnEncodingHelper::GetSerializedSize (*this);
}
    
void
NdnInterestHeader::Serialize (Buffer::Iterator start) const
{
  size_t size = NdnEncodingHelper::Serialize (start, *this);
  NS_UNUSED (size);
  NS_LOG_INFO ("Serialize size = " << size);
}

uint32_t
NdnInterestHeader::Deserialize (Buffer::Iterator start)
{
  return NdnDecodingHelper::Deserialize (start, *this); // \todo Debugging is necessary
}

TypeId
NdnInterestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
NdnInterestHeader::Print (std::ostream &os) const
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

}
