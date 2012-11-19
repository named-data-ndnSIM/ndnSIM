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

#ifndef _NDN_CONTENT_OBJECT_HEADER_H_
#define _NDN_CONTENT_OBJECT_HEADER_H_

#include "ns3/integer.h"
#include "ns3/header.h"
#include "ns3/simple-ref-count.h"
#include "ns3/trailer.h"
#include "ns3/nstime.h"

#include <string>
#include <vector>
#include <list>

#include "ndn-name-components.h"

namespace ns3 {
namespace ndn {

/**
 * ContentObject header
 * 
 * Only few important fields are actually implemented in the simulation
 *
 * ContentObjectHeader serializes/deserializes header up-to and including <Content> tags
 * Necessary closing tags should be added using ContentObjectTail
 *
 * Optimized and simplified formatting of Interest packets 
 *
 *	ContentObject ::= Signature
 *                	  Name
 *                   	  Content
 *
 * This hacks are necessary to optimize memory use (i.e., virtual payload)
 *
 * "<ContentObject><Signature>..</Signature><Name>...</Name><SignedInfo>...</SignedInfo><Content>"
 * 
 */
class ContentObjectHeader : public SimpleRefCount<ContentObjectHeader,Header>
{
public:
  /**
   * Constructor
   *
   * Creates a null header
   **/
  ContentObjectHeader ();

  /**
   * \brief Set content object name
   *
   * Sets name of the content object. For example, SetName( NameComponents("prefix")("postfix") );
   **/
  void
  SetName (const Ptr<NameComponents> &name);

  /**
   * @brief Get name of the content object
   */
  const NameComponents&
  GetName () const;

  /**
   * @brief Get smart pointer to the interest name (to avoid extra memory usage)
   */
  Ptr<const NameComponents>
  GetNamePtr () const;

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
  
  //////////////////////////////////////////////////////////////////
  
  static TypeId GetTypeId (void); ///< @brief Get TypeId
  virtual TypeId GetInstanceTypeId (void) const; ///< @brief Get TypeId of the instance
  virtual void Print (std::ostream &os) const; ///< @brief Print out information about the Header into the stream
  virtual uint32_t GetSerializedSize (void) const; ///< @brief Get size necessary to serialize the Header
  virtual void Serialize (Buffer::Iterator start) const; ///< @brief Serialize the Header
  virtual uint32_t Deserialize (Buffer::Iterator start); ///< @brief Deserialize the Header
  
private:
  Ptr<NameComponents> m_name;
  Time m_freshness;
  Time m_timestamp;
};

/**
 * ContentObjectTail for compatibility with other packet formats
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


/**
 * @ingroup ndn-exceptions
 * @brief Class for ContentObject parsing exception 
 */
class ContentObjectHeaderException {};

} // namespace ndn
} // namespace ns3

#endif // _NDN_CONTENT_OBJECT_HEADER_H_
