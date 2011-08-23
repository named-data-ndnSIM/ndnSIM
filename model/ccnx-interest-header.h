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

#ifndef _CCNX_INTEREST_HEADER_H_
#define _CCNX_INTEREST_HEADER_H_

#include "ns3/integer.h"
#include "ns3/header.h"
#include "ns3/nstime.h"

#include <string>
#include <vector>
#include <list>

#include "name-components.h"

namespace ns3
{
  
/**
 * CCNx XML definition of Interest
 * 
 * Only few important fields are actually implemented in the simulation
 *
 * <xs:element name="Interest" type="InterestType"/>
 * <xs:complexType name="InterestType">
 *   <xs:sequence>
 *     <xs:element name="Name" type="NameType"/>
 *     <xs:element name="MinSuffixComponents" type="xs:nonNegativeInteger"
 *                         minOccurs="0" maxOccurs="1"/>
 *     <xs:element name="MaxSuffixComponents" type="xs:nonNegativeInteger"
 *                         minOccurs="0" maxOccurs="1"/>
 *     <xs:choice minOccurs="0" maxOccurs="1">
 *         <xs:element name="PublisherPublicKeyDigest" type="DigestType"/>
 *         <xs:element name="PublisherCertificateDigest" type="DigestType"/>
 *         <xs:element name="PublisherIssuerKeyDigest" type="DigestType"/>
 *         <xs:element name="PublisherIssuerCertificateDigest" type="DigestType"/>
 *     </xs:choice>
 *     <xs:element name="Exclude" type="ExcludeType"
 *                         minOccurs="0" maxOccurs="1"/>
 *     <xs:element name="ChildSelector" type="xs:nonNegativeInteger"
 *                         minOccurs="0" maxOccurs="1"/>
 *     <xs:element name="AnswerOriginKind" type="xs:nonNegativeInteger"
 *                         minOccurs="0" maxOccurs="1"/>
 *     <xs:element name="Scope" type="xs:nonNegativeInteger"
 * 			minOccurs="0" maxOccurs="1"/>
 *     <xs:element name="InterestLifetime" type="FinegrainLifetimeType"
 * 			minOccurs="0" maxOccurs="1"/>
 *     <xs:element name="Nonce" type="Base64BinaryType"
 * 			minOccurs="0" maxOccurs="1"/>
 *   </xs:sequence>
 * </xs:complexType>
 *
 * <xs:complexType name="NameType">
 *   <xs:sequence>
 *     <xs:element name="Component" type="Base64BinaryType"
 *                 minOccurs="0" maxOccurs="unbounded"/>
 *   </xs:sequence>
 * </xs:complexType>
 * 
 * <xs:complexType name="ExcludeType">
 *   <xs:sequence>
 *     <xs:choice minOccurs="0" maxOccurs="1">
 *         <xs:element name="Any" type="EmptyType"/>
 *         <xs:element name="Bloom" type="Base64BinaryType"/> <!-- Bloom is deprecated --!>
 *     </xs:choice>
 *      <xs:sequence minOccurs="0" maxOccurs="unbounded">
 *         <xs:element name="Component" type="Base64BinaryType"/>
 *         <xs:choice minOccurs="0" maxOccurs="1">
 *             <xs:element name="Any" type="EmptyType"/>
 *             <xs:element name="Bloom" type="Base64BinaryType"/> <!-- Bloom is deprecated --!>
 *         </xs:choice>
 *      </xs:sequence>
 *   </xs:sequence>
 * </xs:complexType>
 * 
 * <!-- Binary representation of time, Unix time epoch, units 2**-12 sec (0.000244140625 sec) -->
 * <!-- The length limit limit of 6 bytes is not actually to be enforced, but
 *      it will be a loooooooong time before anyone cares. --> 
 * 
 * <!-- Binary representation of relative time, relative to "now" -->
 * <xs:complexType name="FinegrainLifetimeType">
 *   <xs:simpleContent>
 *     <xs:extension base="BinaryTime12">
 *       <xs:attribute name="ccnbencoding" type="xs:string" fixed="base64Binary"/>
 *     </xs:extension>
 *   </xs:simpleContent>
 * </xs:complexType>
 *
 * <xs:simpleType name="BinaryTime12">
 *     <xs:restriction base="xs:base64Binary">
 *       <xs:length value="6" fixed="true"/>
 *     </xs:restriction>
 * </xs:simpleType>
 *
 **/

/**
   NDN InterestHeader and routines to serialize/deserialize

   Simplifications:
   - Name:  binary name components are not supported
   - MinSuffixComponents and MasSuffixComponents: if value is negative (default), will not be serialized
   - ChildSelector, AnswerOriginKind: 0 - false, 1 - true, -1 not set
   - Publisher* elements are not supported
   - Exclude: only simple name matching is supported (Bloom support has been deprecated in CCNx)
   - InterestLifetime: ?
   - Nonce: 32 bit random integer.  If value is 0, will not be serialized
 */
class CcnxInterestHeader : public Header
{
public:
  /**
   * Constructor
   *
   * Creates a null header
   **/
  CcnxInterestHeader ();

  /**
   * \brief Set interest name
   *
   * Sets name of the interest. For example, SetName( CcnxNameComponents("prefix")("postfix") );
   **/
  void
  SetName (const Ptr<CcnxNameComponents> &name);

  const CcnxNameComponents&
  GetName () const;

  void
  SetMinSuffixComponents (int32_t value);

  int32_t
  GetMinSuffixComponents () const;

  void
  SetMaxSuffixComponents (int32_t value);

  int32_t
  GetMaxSuffixComponents () const;

  /**
   * \brief Set exclude filer
   *
   * For example, SetExclude (CcnxNameComponents("exclude1")("exclude2")("exclude3"))
   **/
  void
  SetExclude (const Ptr<CcnxNameComponents> &exclude);

  const CcnxNameComponents&
  GetExclude () const;

  void
  SetChildSelector (bool value);

  bool
  IsEnabledChildSelector () const;

  void
  SetAnswerOriginKind (bool value);

  bool
  IsEnabledAnswerOriginKind () const;

  void
  SetScope (int8_t scope);

  int8_t
  GetScope () const;

  void
  SetInterestLifetime (Time time);

  Time
  GetInterestLifetime () const;

  void
  SetNonce (uint32_t nonce);

  uint32_t
  GetNonce () const;

  //////////////////////////////////////////////////////////////////
  
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

private:
  Ptr<CcnxNameComponents> m_name;
  int32_t m_minSuffixComponents; ///< minimum suffix components. not used if negative
  int32_t m_maxSuffixComponents; ///< maximum suffix components. not used if negative
  Ptr<CcnxNameComponents> m_exclude; ///< exclude filter
  bool m_childSelector;    
  bool m_answerOriginKind; 
  int8_t m_scope;            ///< -1 not set, 0 local scope, 1 this host, 2 immediate neighborhood
  Time  m_interestLifetime;
  uint32_t m_nonce; ///< Nonce. not used if zero
};

} // namespace ns3

#endif // _CCNX_INTEREST_HEADER_H_
