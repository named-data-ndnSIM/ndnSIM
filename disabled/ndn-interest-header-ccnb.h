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

#ifndef _NDN_INTEREST_HEADER_CCNB_H_
#define _NDN_INTEREST_HEADER_CCNB_H_

#include "ns3/integer.h"
#include "ns3/header.h"
#include "ns3/simple-ref-count.h"
#include "ns3/nstime.h"

#include <string>
#include <vector>
#include <list>

#include "ndn-name.h"

namespace ns3 {

class Packet;

namespace ndn {
  
/**
 * Ndn XML definition of Interest
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
  * @brief NDN Interest and routines to serialize/deserialize
  *
  * Simplifications:
  * - Name:  binary name components are not supported
  * - MinSuffixComponents and MasSuffixComponents: if value is negative (default), will not be serialized
  * - ChildSelector, AnswerOriginKind: 0 - false, 1 - true, -1 not set
  * - Publisher* elements are not supported
  * - Exclude: only simple name matching is supported (Bloom support has been deprecated in Ndn)
  * - InterestLifetime: ?
  * - Nonce: 32 bit random integer.  If value is 0, will not be serialized
  **/
class Interest : public SimpleRefCount<Interest, Header>
{
public:
  /**
   * \brief Constructor
   *
   * Creates a null header
   **/
  Interest ();

  /**
   * @brief Copy constructor
   */
  Interest (const Interest &interest);

  /**
   * \brief Set interest name
   *
   * Sets name of the interest. For example, SetName( ndnName("prefix")("postfix") );
   * @param[in] name const pointer to ndnName object that contains an interest name
   **/
  void
  SetName (Ptr<Name> name);


  /**
   * \brief Get interest name
   *
   * Gets name of the interest.
   **/
  const Name&
  GetName () const;

  /**
   * @brief Get smart pointer to the interest name (to avoid extra memory usage)
   */
  Ptr<const Name>
  GetNamePtr () const;

  /**
   * \brief Set interest MinSuffixComponents
   *
   * MinSuffixComponents refer to the number of name components beyond those in the prefix, 
   * and counting the implicit digest, that may occur in the matching ContentObject.
   * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
   * @param[in] value minimum length of suffix components
   **/
  void
  SetMinSuffixComponents (int32_t value);

  /**
   * \brief Get interest MinSuffixComponents
   *
   * MinSuffixComponents refer to the number of name components beyond those in the prefix, 
   * and counting the implicit digest, that may occur in the matching ContentObject.
   * For more information, see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html
   **/
  int32_t
  GetMinSuffixComponents () const;


  /**
   * \brief Set interest MaxSuffixComponents
   *
   * MaxSuffixComponents refer to the number of name components beyond those in the prefix, 
   * and counting the implicit digest, that may occur in the matching ContentObject.
   * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
   * @param[in] value maximum length of suffix components
   **/
  void
  SetMaxSuffixComponents (int32_t value);

  /**
   * \brief Get interest MaxSuffixComponents
   *
   * MaxSuffixComponents refer to the number of name components beyond those in the prefix, 
   * and counting the implicit digest, that may occur in the matching ContentObject.
   * For more information, see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html
   **/
  int32_t
  GetMaxSuffixComponents () const;

  /**
   * \brief Set exclude filer
   *
   * For example, SetExclude (ndnName("exclude1")("exclude2")("exclude3"))
   * @param[in] exclude const pointer to ndnName to be excluded 
   **/
  void
  SetExclude (Ptr<Name> exclude);

  /**
   * \brief Check if interest conatins exclude filter
   *
   */ 
  bool
  IsEnabledExclude () const;
  
  /**
   * \brief Get exclude filter 
   */
  const Name&
  GetExclude () const;

  /**
   * \brief Set ChildSelector
   * Often a given interest will match more than one ContentObject within a given content store. 
   * The ChildSelector provides a way of expressing a preference for which of these should be returned. 
   * If the value is false, the leftmost child is preferred. If true, the rightmost child is preferred.
   * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information. 
   * @param[in] value boolean ChildSelector value
   */
  void
  SetChildSelector (bool value);

  /**
   * \brief Return ChildSelector value
   * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
   *
   */
  bool
  IsEnabledChildSelector () const;

  /**
   * \brief Set AnswerOriginKind
   * Default value for AnswerOriginKind is false.
   * @param[in] value boolean AnswerOriginKind value
   */
  void
  SetAnswerOriginKind (bool value);

  /**
   * \brief Check the value of AnswerOriginKind
   *
   */
  bool
  IsEnabledAnswerOriginKind () const;

  /**
   * \brief Set Scope
   * Scope limits where the Interest may propagate. 
   * Scope 0 prevents propagation beyond the local ccnd (even to other applications on the same host).
   * Scope 1 limits propagation to the applications on the originating host. 
   * Scope 2 limits propagation to no further than the next host. 
   * Other values are not defined, and will cause the Interest message to be dropped.
   * Note that this is not a hop count - the value is not decremented as the interest is forwarded.
   * @param[in] scope interest scope
   */
  void
  SetScope (int8_t scope);

  /**
   * \brief Get Scope value
   * Scope limits where the Interest may propagate. 
   * Scope 0 prevents propagation beyond the local ccnd (even to other applications on the same host).
   * Scope 1 limits propagation to the applications on the originating host. 
   * Scope 2 limits propagation to no further than the next host. 
   * Other values are not defined, and will cause the Interest message to be dropped.
   * Note that this is not a hop count - the value is not decremented as the interest is forwarded.
   */
  int8_t
  GetScope () const;

  /**
   * \brief Set InterestLifetime
   * InterestLifetime indicates the (approximate) time remaining before the interest times out.
   * The timeout is relative to the arrival time of the interest at the current node.
   * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
   * @param[in] time interest lifetime  
   */ 
  void
  SetInterestLifetime (Time time);

  /**
   * \brief Get InterestLifetime value
   * InterestLifetime indicates the (approximate) time remaining before the interest times out.
   * The timeout is relative to the arrival time of the interest at the current node.
   * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
   */ 
  Time
  GetInterestLifetime () const;

  /**
   * \brief Set Nonce
   * Nonce carries a randomly-genenerated bytestring that is used to detect and discard duplicate Interest messages.
   * @param[in] nonce Unique packet identification number
   */
  void
  SetNonce (uint32_t nonce);

  /**
   * \brief Get Nonce value
   * Nonce carries a randomly-genenerated bytestring that is used to detect and discard duplicate Interest messages.
   *
   */
  uint32_t
  GetNonce () const;
  
  /**
   * @brief NACK Type
   * Specifies the type of Interest packet
   */
  enum
    {
      NORMAL_INTEREST = 0,
      NACK_LOOP = 10,
      NACK_CONGESTION = 11,
      NACK_GIVEUP_PIT = 12,
    };

  /**
   * \brief Mark the Interest as a Negative Acknowledgement
   * Three types of NACKs are supported
   * 1. NACK_LOOP 
   * 2. NACK_CONGESTION
   * 3. NACK_GIVEUP_PIT
   * @param[in] nackType  NACK_LOOP or NACK_CONGESTION or NACK_GIVEUP_PIT or NORMAL_INTEREST
   */
  void
  SetNack (uint32_t nackType);
    
  /**
  * \brief Get NACK type
  * Returns NACK_LOOP, NACK_CONGESTION or NACK_GIVEUP_PIT. 
  * Otherwise, in case of normal interest packet, returns NORMAL_INTEREST (equals 0).
  */
  uint32_t
  GetNack () const;

  //////////////////////////////////////////////////////////////////

  static TypeId GetTypeId (void); ///< @brief Get TypeId of the class
  virtual TypeId GetInstanceTypeId (void) const; ///< @brief Get TypeId of the instance
  
  /**
   * \brief Print Interest packet 
   */
  virtual void Print (std::ostream &os) const;
  
  /**
   * \brief Get the size of Interest packet
   * Returns the Interest packet size after serialization
   */
  virtual uint32_t GetSerializedSize (void) const;
  
  /**
   * \brief Serialize Interest packet
   * Serializes Interest packet into Buffer::Iterator
   * @param[in] start buffer to contain serialized Interest packet
   */
  virtual void Serialize (Buffer::Iterator start) const;
  
  /**
   * \brief Deserialize Interest packet
   * Deserializes Buffer::Iterator into Interest packet
   * @param[in] start buffer that contains serialized Interest packet
   */ 
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * @brief Cheat for python bindings
   */
  static Ptr<Interest>
  GetInterest (Ptr<Packet> packet);
  
private:
  Ptr<Name> m_name;    ///< Interest name
  int32_t m_minSuffixComponents; ///< Minimum suffix components. not used if negative
  int32_t m_maxSuffixComponents; ///< Maximum suffix components. not used if negative
  Ptr<Name> m_exclude; ///< Exclude filter
  bool m_childSelector;          ///< Default value for ChildSelector is false
  bool m_answerOriginKind;       ///< Default value for AnswerOriginKind is false
  int8_t m_scope;                ///< -1 not set, 0 local scope, 1 this host, 2 immediate neighborhood
  Time  m_interestLifetime;      ///< InterestLifetime
  uint32_t m_nonce;              ///< Nonce. not used if zero
  uint32_t m_nackType;           ///< Negative Acknowledgement type
};

/**
 * @ingroup ndn-exceptions
 * @brief Class for Interest parsing exception 
 */
class InterestException {};

} // namespace ndn
} // namespace ns3

#endif // _NDN_INTEREST_HEADER_CCNB_H_
