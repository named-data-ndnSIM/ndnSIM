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

#ifndef _NDN_INTEREST_HEADER_H_
#define _NDN_INTEREST_HEADER_H_

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
  * @brief NDN Interest and routines to serialize/deserialize
  *
  * Optimized and simplified formatting of Interest packets 
  *
  *	Interest ::= Nonce 
  *	     	     Scope 
  *		     InterestLifetime 
  *	     	     Name 
  *	     	     Selectors 
  *	     	     Options
  *
  * Minumum size of the Interest packet: 1 + 4 + 2 + 1 + (2 + 0) + (2 + 0) + (2 + 0) = 14
  *
  * Maximum size of the Interest packet: 1 + 4 + 2 + 1 + (2 + 65535) + (2 + 65535) + (2 + 65535) = 196619
  *
  * ::
  *
  *        0                   1                   2                   3
  *        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  *        |                          Nonce                                |
  *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  *        |     Scope     |   Reserved    |      InterestLifetime         |
  *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  *        |            Length             |                               |
  *	   |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
  *        ~                                                               ~
  *        ~                            Name                               ~
  *        |							           |	
  *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  *        |            Length             |                               |
  *        |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
  *        ~                                                               ~
  *        ~                          Selectors                            ~
  *        |							            |	
  *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  *        |            Length             |                               |
  *	   |-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               |
  *        ~                                                               ~
  *        ~                          Options                              ~
  *        |							           |	
  *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  * **/
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
   * @param name smart pointer to Name
   *
   **/
  void
  SetName (Ptr<Name> name);

  /**
   * \brief Another variant to set interest name
   *
   * @param name const reference to Name object
   *
   **/
  void
  SetName (const Name &name);
  
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
  SetNack (uint8_t nackType); //using reserved field
    
  /**
  * \brief Get NACK type
  * Returns NACK_LOOP, NACK_CONGESTION or NACK_GIVEUP_PIT. 
  * Otherwise, in case of normal interest packet, returns NORMAL_INTEREST (equals 0).
  */
  uint8_t
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
  uint8_t m_scope;                ///< 0xFF not set, 0 local scope, 1 this host, 2 immediate neighborhood
  Time  m_interestLifetime;      ///< InterestLifetime
  uint32_t m_nonce;              ///< Nonce. not used if zero
  uint8_t  m_nackType;           ///< Negative Acknowledgement type
};

typedef Interest InterestHeader;

/**
 * @ingroup ndn-exceptions
 * @brief Class for Interest parsing exception 
 */
class InterestException {};

} // namespace ndn
} // namespace ns3

#endif // _NDN_INTEREST_HEADER_H_
