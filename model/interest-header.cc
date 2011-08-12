/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 */

///< #CCN_PR_SCOPE0 (0x20) local scope,
///< #CCN_PR_SCOPE1 (0x40) this host,
///< #CCN_PR_SCOPE2 (0x80) immediate neighborhood

#include "interest-header.h"

namespace ns3
{
namespace NDNabstraction
{

  InterestHeader::InterestHeader ()
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
  InterestHeader::SetName (const Ptr<Name::Components> &name)
  {
    m_name = name;
  }

  const Name::Components&
  InterestHeader::GetName () const
  {
    return *m_name;
  }

  void
  InterestHeader::SetMinSuffixComponents (int32_t value)
  {
    m_minSuffixComponents = value;
  }

  int32_t
  InterestHeader::GetMinSuffixComponents () const
  {
    return m_minSuffixComponents;
  }

  void
  InterestHeader::SetMaxSuffixComponents (int32_t value)
  {
    m_maxSuffixComponents = value;
  }

  int32_t
  InterestHeader::GetMaxSuffixComponents () const
  {
    return m_maxSuffixComponents;
  }

  void
  InterestHeader::SetExclude (const Ptr<Name::Components> &exclude)
  {
    m_exclude = exclude;
  }

  const Name::Components&
  InterestHeader::GetExclude () const
  {
    return *m_exclude;
  }

  void
  InterestHeader::EnableChildSelector ()
  {
    m_childSelector = true;
  }

  bool
  InterestHeader::IsEnabledChildSelector () const
  {
    return m_childSelector;
  }

  void
  InterestHeader::EnableAnswerOriginKind ()
  {
    m_answerOriginKind = true;
  }

  bool
  InterestHeader::IsEnabledAnswerOriginKind () const
  {
    return m_answerOriginKind;
  }

  void
  InterestHeader::SetScope (int8_t scope)
  {
    m_scope = scope;
  }

  int8_t
  InterestHeader::GetScope () const
  {
    return m_scope;
  }

  void
  InterestHeader::SetInterestLifetime (intmax_t lifetime)
  {
    m_interestLifetime = lifetime;
  }

  intmax_t
  InterestHeader::GetInterestLifetime () const
  {
    return m_interestLifetime;
  }

  void
  InterestHeader::SetNonce (uint32_t nonce)
  {
    m_nonce = nonce;
  }

  uint32_t
  InterestHeader::GetNonce () const
  {
    return m_nonce;
  }
  
  uint32_t
  InterestHeader::GetSerializedSize (void) const
  {
    return 0;
  }
    
  void
  InterestHeader::Serialize (Buffer::Iterator start) const
  {
    return;
  }

  uint32_t
  InterestHeader::Deserialize (Buffer::Iterator start)
  {
    return 0;
  }

  TypeId
  InterestHeader::GetTypeId (void)
  {
    static TypeId tid = TypeId ("ns3::NDNabstraction::InterestHeader")
      .SetParent<Header> ()
      .AddConstructor<InterestHeader> ()
      ;
    return tid;
  }
  
  TypeId
  InterestHeader::GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }
  
  void
  InterestHeader::Print (std::ostream &os) const
  {
    os << "Interest: " << *m_name;
  }
}
}
