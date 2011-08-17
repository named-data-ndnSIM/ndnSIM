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
#include "ns3/ccnx-encoding-helper.h"
#include "ns3/ccnx-decoding-helper.h"

NS_LOG_COMPONENT_DEFINE ("CcnxInterestHeader");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (CcnxInterestHeader);

TypeId
CcnxInterestHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxInterestHeader")
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
  , m_interestLifetime (-1)
  , m_nonce (0)
{
}

void
CcnxInterestHeader::SetName (const Ptr<Name::Components> &name)
{
  m_name = name;
}

const Name::Components&
CcnxInterestHeader::GetName () const
{
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
CcnxInterestHeader::SetExclude (const Ptr<Name::Components> &exclude)
{
  m_exclude = exclude;
}

const Name::Components&
CcnxInterestHeader::GetExclude () const
{
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
  
uint32_t
CcnxInterestHeader::GetSerializedSize (void) const
{
  // unfortunately, 2 serialization required...
  Buffer tmp;
  
  return CcnxEncodingHelper::Serialize (tmp.Begin(), *this);
}
    
void
CcnxInterestHeader::Serialize (Buffer::Iterator start) const
{
  CcnxEncodingHelper::Serialize (start, *this);
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
  os << "<Interest><Name>" << *m_name << "</Name>";
  if (m_minSuffixComponents>=0)
    os << "<MinSuffixComponents>" << m_minSuffixComponents << "</MinSuffixComponents>";
  if (m_maxSuffixComponents>=0)
    os << "<MaxSuffixComponents>" << m_maxSuffixComponents << "</MaxSuffixComponents>";
  if (m_exclude->size()>0)
    os << "<Exclude>" << *m_exclude << "</Exclude>";
  if (m_childSelector)
    os << "<ChildSelector />";
  if (m_answerOriginKind)
    os << "<AnswerOriginKind />";
  if (m_scope>=0)
    os << "<Scope>" << m_scope << "</Scope>";
  if (!m_interestLifetime.IsZero())
    os << "<InterestLifetime>" << m_interestLifetime << "</InterestLifetime>";
  if (m_nonce>0)
    os << "<Nonce>" << m_nonce << "</Nonce>";
}

}
