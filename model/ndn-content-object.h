/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2013 University of California, Los Angeles
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

#include "ndn-name.h"

namespace ns3 {
namespace ndn {

/**
 * ContentObject header
 *
 * Only few important fields are actually implemented in the simulation
 *
 * @see http://ndnsim.net/new-packet-formats.html
 *
 * Optimized and simplified formatting of Interest packets
 *
 *	ContentObject ::= Signature
 *                	  Name
 *                   	  Content
 *
 *      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Length             |                               |
 *      |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               +
 *      ~                                                               ~
 *      ~                           Signature                           ~
 *      |							        |	
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Length             |                               |
 *      |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *      ~                                                               ~
 *      ~                             Name                              ~
 *      |							        |	
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *      |            Length             |                               |
 *      |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
 *      ~                                                               ~
 *      ~                           Content                             ~
 *      |							        |	
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 */
class ContentObject : public SimpleRefCount<ContentObject,Header>
{
public:
  /**
   * @brief Constructor
   *
   * Creates a null header
   **/
  ContentObject ();

  /**
   * @brief Copy constructor
   */
  ContentObject (const ContentObject &other);

  /**
   * \brief Set content object name
   *
   * Sets name of the content object
   **/
  void
  SetName (Ptr<Name> name);

  /**
   * @brief Another, less efficient, variant of setting content object name
   *
   * Sets name of the content object
   */
  void
  SetName (const Name &name);
  
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

  /**
   * @brief Set "fake" signature on the content object
   * @param signature  uint32_t number, simulating content object signature
   *
   * Values for the signature totally depend on the application
   */
  void
  SetSignature (uint32_t signature);

  /**
   * @brief Get "fake" signature of the content object
   *
   * Values for the signature totally depend on the application
   */
  uint32_t
  GetSignature () const;

  //////////////////////////////////////////////////////////////////
  /**
   * @brief Get payload of data packet
   *
   * This payload can also carry packet tags
   */
  void
  SetPayload (Ptr<Packet> payload);

  /**
   * @brief Set payload of data packet
   *
   * This payload can also carry packet tags
   */
  Ptr<const Payload>
  GetPayload () const;
  
  /**
   * @brief Get wire formatted packet
   *
   * If wire formatted packet has not been set before, 0 will be returned
   */
  inline Ptr<const Packet>
  GetWire () const;

  /**
   * @brief Set (cache) wire formatted packet
   */
  inline void
  SetWire (Ptr<const Packet> packet) const;

private:
  // NO_ASSIGN
  ContentObject &
  operator = (const ContentObject &other) { return *this; }
  
private:
  Ptr<Name> m_name;
  Time m_freshness;
  Time m_timestamp;
  uint32_t m_signature; // 0, means no signature, any other value application dependent (not a real signature)
  Ptr<Packet> m_payload;

  mutable Ptr<const Packet> m_wire;
};

/**
 * @ingroup ndn-exceptions
 * @brief Class for ContentObject parsing exception
 */
class ContentObjectException {};

} // namespace ndn
} // namespace ns3

#endif // _NDN_CONTENT_OBJECT_HEADER_H_
