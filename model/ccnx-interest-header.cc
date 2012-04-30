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

#include "ccnx-interest-header.h"

#include "ns3/log.h"
#include "../helper/ccnx-encoding-helper.h"
#include "../helper/ccnx-decoding-helper.h"

NS_LOG_COMPONENT_DEFINE ("CcnxInterestHeader");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (CcnxInterestHeader);

TypeId
CcnxInterestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxInterestHeader")
    .SetGroupName ("Ccnx")
    .SetParent<Header> ()
    .AddConstructor<CcnxInterestHeader> ()
    ;
  return tid;
}
  

CcnxInterestHeader::CcnxInterestHeader ()
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
CcnxInterestHeader::SetName (const Ptr<CcnxNameComponents> &name)
{
  m_name = name;
}

const CcnxNameComponents&
CcnxInterestHeader::GetName () const
{
  if (m_name==0) throw CcnxInterestHeaderException();
  return *m_name;
}

void
CcnxInterestHeader::SetMinSuffixComponents (int32_t value)
{
  m_minSuffixComponents = value;
}

int32_t
CcnxInterestHeader::GetMinSuffixComponents () const
{
  return m_minSuffixComponents;
}

void
CcnxInterestHeader::SetMaxSuffixComponents (int32_t value)
{
  m_maxSuffixComponents = value;
}

int32_t
CcnxInterestHeader::GetMaxSuffixComponents () const
{
  return m_maxSuffixComponents;
}

void
CcnxInterestHeader::SetExclude (const Ptr<CcnxNameComponents> &exclude)
{
  m_exclude = exclude;
}

bool
CcnxInterestHeader::IsEnabledExclude () const
{
  return m_exclude!=0;
}

const CcnxNameComponents&
CcnxInterestHeader::GetExclude () const
{
  if (m_exclude==0) throw CcnxInterestHeaderException();
  return *m_exclude;
}

void
CcnxInterestHeader::SetChildSelector (bool value)
{
  m_childSelector = value;
}

bool
CcnxInterestHeader::IsEnabledChildSelector () const
{
  return m_childSelector;
}

void
CcnxInterestHeader::SetAnswerOriginKind (bool value)
{
  m_answerOriginKind = value;
}

bool
CcnxInterestHeader::IsEnabledAnswerOriginKind () const
{
  return m_answerOriginKind;
}

void
CcnxInterestHeader::SetScope (int8_t scope)
{
  m_scope = scope;
}

int8_t
CcnxInterestHeader::GetScope () const
{
  return m_scope;
}

void
CcnxInterestHeader::SetInterestLifetime (Time lifetime)
{
  m_interestLifetime = lifetime;
}

Time
CcnxInterestHeader::GetInterestLifetime () const
{
  return m_interestLifetime;
}

void
CcnxInterestHeader::SetNonce (uint32_t nonce)
{
  m_nonce = nonce;
}

uint32_t
CcnxInterestHeader::GetNonce () const
{
  return m_nonce;
}

void
CcnxInterestHeader::SetNack (uint32_t nackType)
{
  m_nackType = nackType;
}

uint32_t
CcnxInterestHeader::GetNack () const
{
  return m_nackType;
}

uint32_t
CcnxInterestHeader::GetSerializedSize (void) const
{
  // unfortunately, we don't know exact header size in advance
  return CcnxEncodingHelper::GetSerializedSize (*this);
}
    
void
CcnxInterestHeader::Serialize (Buffer::Iterator start) const
{
  size_t size __attribute__ ((__unused__)) = CcnxEncodingHelper::Serialize (start, *this);
  
  NS_LOG_INFO ("Serialize size = " << size);
}

uint32_t
CcnxInterestHeader::Deserialize (Buffer::Iterator start)
{
  return CcnxDecodingHelper::Deserialize (start, *this); // \todo Debugging is necessary
}

TypeId
CcnxInterestHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
CcnxInterestHeader::Print (std::ostream &os) const
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
