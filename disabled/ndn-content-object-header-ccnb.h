/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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

#ifndef _NDN_CONTENT_OBJECT_HEADER_CCNB_H_
#define _NDN_CONTENT_OBJECT_HEADER_CCNB_H_

#include "ns3/integer.h"
#include "ns3/header.h"
#include "ns3/simple-ref-count.h"
#include "ns3/trailer.h"
#include "ns3/nstime.h"

#include <string>
#include <vector>
#include <list>

#include "ndn-name.h"

namespace ns3 {
namespace ndn {

/**
 * Ndn XML definition of ContentObject
 * 
 * Only few important fields are actually implemented in the simulation
 *
 *
 * ContentObject serializes/deserializes header up-to and including <Content> tags
 * Necessary closing tags should be added using ContentObjectTail
 *
 * This hacks are necessary to optimize memory use (i.e., virtual payload)
 *
 * "<ContentObject><Signature>..</Signature><Name>...</Name><SignedInfo>...</SignedInfo><Content>"
 * 
 */
class ContentObject : public SimpleRefCount<ContentObject,Header>
{
public:
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  /**
   * @brief Class representing Signature section of the content object
   */
  class Signature
  {
  public:
    /**
     * @brief Default constructor. Creates a fake-type signature
     */
    inline Signature ();

    /**
     * @brief Get digest algorithm
     */
    inline const std::string &
    GetDigestAlgorithm () const;

    /**
     * @brief Set digest algorithm
     */
    inline void
    SetDigestAlgorithm (const std::string &digestAlgorithm);

    /**
     * @brief Get signature bits
     */
    inline uint32_t
    GetSignatureBits () const;

    /**
     * @brief Set signature bits
     */
    inline void
    SetSignatureBits (uint32_t signatureBits);

    /**
     * @brief Default digest algorithm ("2.16.840.1.101.3.4.2.1")
     */
    static const std::string DefaultDigestAlgorithm; // = "2.16.840.1.101.3.4.2.1";
    
  private:
    std::string m_digestAlgorithm; // if value is `2.16.840.1.101.3.4.2.1`, then SHA-256 (not supported)
                                   // in NS-3 value `99.0` represents a fake digest
    // Witness // not used in NS-3
    uint32_t m_signatureBits; // in NS-3 a fake signature is a just 32-bits
  };
  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  

  /**
   * @brief Options for the data packet Type
   */
  enum ContentType
    {
      DATA = 0x0C04C0, // default value. If ContentObject is type of DATA, then ContentType tag will be omitted
      ENCR = 0x10D091,
      GONE = 0x18E344,
      KEY  = 0x28463F,
      LINK = 0x2C834A,
      NACK = 0x34008A
    };

  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  

  /**
   * @brief Class representing SignedInfo section of the content object
   */
  class SignedInfo
  {
  public:
    /**
     * @brief Default constructor
     */
    SignedInfo (); 
    
    /**
     * @brief Set PublisherPublicKey digest
     * @param digest a fake 32-bit digest is supported
     */
    void
    SetPublisherPublicKeyDigest (uint32_t digest);

    /**
     * @brief Get PublisherPublicKey digest
     */
    uint32_t
    GetPublisherPublicKeyDigest () const;

    /**
     * @brief Set content object timestamp
     * @param timestamp timestamp
     */
    void
    SetTimestamp (const Time &timestamp);

    /**
     * @brief Get timestamp of the content object
     */
    Time
    GetTimestamp () const;

    /**
     * @brief Set ContentObject type
     * @param type type of the content object
     */
    void
    SetContentType (ContentType type);

    /**
     * @brief Get ContentObject type
     */
    ContentType
    GetContentType () const;
    
    /**
     * @brief Set freshness of the content object
     * @param freshness Freshness, 0s means infinity
     */
    void
    SetFreshness (const Time &freshness);

    /**
     * @brief Get freshness of the content object
     */
    Time
    GetFreshness () const;

    /**
     * @brief Set key locator
     * @param keyLocator name of the key
     *
     * Note that only <KeyName> option for the key locator is supported
     */
    void
    SetKeyLocator (Ptr<const Name> keyLocator);

    /**
     * @brief Get key locator
     *
     * Note that only <KeyName> option for the key locator is supported
     */
    Ptr<const Name>
    GetKeyLocator () const;
    
  private:
    uint32_t m_publisherPublicKeyDigest; // fake publisher key digest
    Time m_timestamp;
    ContentType m_type;
    Time m_freshness;
    // FinalBlockID
    Ptr<const Name> m_keyLocator; // support only <KeyName> option for KeyLocator
  };

  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  
  ////////////////////////////////////////////////////////////////////////////  

  /**
   * Constructor
   *
   * Creates a null header
   **/
  ContentObject ();

  /**
   * \brief Set content object name
   *
   * Sets name of the content object. For example, SetName( Name("prefix")("postfix") );
   **/
  void
  SetName (const Ptr<Name> &name);

  /**
   * @brief Get name of the content object
   */
  const Name&
  GetName () const;

  /**
   * @brief Get smart pointer to the interest name (to avoid extra memory usage)
   */
  Ptr<const Name>
  GetNamePtr () const;

  /**
   * @brief Get editable reference to content object's Signature
   */
  inline Signature &
  GetSignature ();

  /**
   * @brief Get read-only reference to content object's Signature
   */
  inline const Signature &
  GetSignature () const;

  /**
   * @brief Get editable reference to content object's SignedInfo
   */
  inline SignedInfo &
  GetSignedInfo ();

  /**
   * @brief Get read-only reference to content object's SignedInfo
   */
  inline const SignedInfo &
  GetSignedInfo () const;
  
  //////////////////////////////////////////////////////////////////
  
  static TypeId GetTypeId (void); ///< @brief Get TypeId
  virtual TypeId GetInstanceTypeId (void) const; ///< @brief Get TypeId of the instance
  virtual void Print (std::ostream &os) const; ///< @brief Print out information about the Header into the stream
  virtual uint32_t GetSerializedSize (void) const; ///< @brief Get size necessary to serialize the Header
  virtual void Serialize (Buffer::Iterator start) const; ///< @brief Serialize the Header
  virtual uint32_t Deserialize (Buffer::Iterator start); ///< @brief Deserialize the Header
  
private:
  Signature  m_signature;
  Ptr<Name> m_name;
  SignedInfo m_signedInfo;
};

/**
 * ContentObjectTail should always be 2 bytes, representing two closing tags:
 * "</Content><ContentObject>"
 */
class ContentObjectTail : public Trailer
{
public:
  ContentObjectTail ();
  //////////////////////////////////////////////////////////////////
  
  static TypeId GetTypeId (void); ///< @brief Get TypeId
  virtual TypeId GetInstanceTypeId (void) const; ///< @brief Get TypeId of the instance
  virtual void Print (std::ostream &os) const; ///< @brief Print out information about Tail into the stream
  virtual uint32_t GetSerializedSize (void) const; ///< @brief Get size necessary to serialize the Tail
  virtual void Serialize (Buffer::Iterator start) const; ///< @brief Serialize the Tail
  virtual uint32_t Deserialize (Buffer::Iterator start); ///< @brief Deserialize the Tail
};


ContentObject::Signature::Signature ()
  : m_digestAlgorithm ("99.0")
  , m_signatureBits (0)
{
}

const std::string &
ContentObject::Signature::GetDigestAlgorithm () const
{
  return m_digestAlgorithm;
}

void
ContentObject::Signature::SetDigestAlgorithm (const std::string &digestAlgorithm)
{
  m_digestAlgorithm = digestAlgorithm;
}

uint32_t
ContentObject::Signature::GetSignatureBits () const
{
  return m_signatureBits;
}

inline void
ContentObject::Signature::SetSignatureBits (uint32_t signature)
{
  m_signatureBits = signature;
}


ContentObject::Signature &
ContentObject::GetSignature ()
{
  return m_signature;
}

const ContentObject::Signature &
ContentObject::GetSignature () const
{
  return m_signature;
}

ContentObject::SignedInfo &
ContentObject::GetSignedInfo ()
{
  return m_signedInfo;
}

const ContentObject::SignedInfo &
ContentObject::GetSignedInfo () const
{
  return m_signedInfo;
}

/**
 * @ingroup ndn-exceptions
 * @brief Class for ContentObject parsing exception 
 */
class ContentObjectException {};

} // namespace ndn
} // namespace ns3

#endif // _NDN_CONTENT_OBJECT_HEADER_CCNB_H_
