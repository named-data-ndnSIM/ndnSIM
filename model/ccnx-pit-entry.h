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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _CCNX_PIT_ENTRY_H_
#define _CCNX_PIT_ENTRY_H_

#include "ns3/ptr.h"

#include "ccnx-pit-entry-incoming-face.h"
#include "ccnx-pit-entry-outgoing-face.h"
#include "ccnx-fib.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <set>

#include <iostream>

namespace ns3 {

class CcnxFace;
class CcnxNameComponents;

namespace __ccnx_private
{
// class i_face {};
}

/**
 * \ingroup ccnx
 * \brief Typedef for indexed face container of CcnxPitEntryIncomingFace
 *
 * Indexes:
 * - by face (may be it will be possible to replace with just the std::map)
 */
struct CcnxPitEntryIncomingFaceContainer
{
  typedef boost::multi_index::multi_index_container<
    CcnxPitEntryIncomingFace,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private::i_face>,
        boost::multi_index::member<CcnxPitEntryIncomingFace, Ptr<CcnxFace>, &CcnxPitEntryIncomingFace::m_face>
      >
    >
   > type;
};

/**
 * \ingroup ccnx
 * \brief Typedef for indexed face container of CcnxPitEntryOutgoingFace
 *
 * Indexes:
 * - by face (may be it will be possible to replace with just the std::map)
 */
struct CcnxPitEntryOutgoingFaceContainer
{
  typedef boost::multi_index::multi_index_container<
    CcnxPitEntryOutgoingFace,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private::i_face>,
        boost::multi_index::member<CcnxPitEntryOutgoingFace, Ptr<CcnxFace>, &CcnxPitEntryOutgoingFace::m_face>
      >
    >
   > type;
};


/**
 * \ingroup ccnx
 * \brief structure for PIT entry
 */
struct CcnxPitEntry
{
public:
  /**
   * \brief PIT entry constructor
   * \param prefix Prefix of the PIT entry
   * \param fibEntry A FIB entry associated with the PIT entry
   */
  CcnxPitEntry (Ptr<CcnxNameComponents> prefix, const Time &expireTime, const CcnxFibEntry &fibEntry);
  
  // // Get number of outgoing interests that we're expecting data from
  // inline size_t numberOfPromisingInterests( ) const; 

  const CcnxNameComponents &
  GetPrefix () const
  {
    return *m_prefix;
  }

  /**
   * @brief Get current expiration time of the record
   *
   * @returns current expiration time of the record
   */
  const Time &
  GetExpireTime () const
  { return m_expireTime; }

  /**
   * @brief Set expiration time on record as `expireTime` (absolute time)
   *
   * @param expireTime absolute simulation time of when the record should expire
   */
  void
  SetExpireTime (const Time &expireTime)
  {
    m_expireTime = expireTime;
  }
  
  /**
   * @brief Check if nonce `nonce` for the same prefix has already been seen
   *
   * @param nonce Nonce to check
   */
  bool
  IsNonceSeen (uint32_t nonce) const
  { return m_seenNonces.find (nonce) != m_seenNonces.end (); }

  /**
   * @brief Add `nonce` to the list of seen nonces
   *
   * @param nonce nonce to add to the list of seen nonces
   *
   * All nonces are stored for the lifetime of the PIT entry
   */
  void
  AddSeenNonce (uint32_t nonce)
  { m_seenNonces.insert (nonce); }

  /**
   * @brief Add `face` to the list of incoming faces
   *
   * @param face Face to add to the list of incoming faces
   * @returns iterator to the added entry
   */
  CcnxPitEntryIncomingFaceContainer::type::iterator
  AddIncoming (Ptr<CcnxFace> face);

  /**
   * @brief Remove incoming entry for face `face`
   */
  void
  RemoveIncoming (Ptr<CcnxFace> face);

  /**
   * @brief Clear all incoming faces either after all of them were satisfied or NACKed
   */
  void
  ClearIncoming ()
  { m_incoming.clear (); }

  /**
   * @brief Add `face` to the list of outgoing faces
   *
   * @param face Face to add to the list of outgoing faces
   * @returns iterator to the added entry
   */
  CcnxPitEntryOutgoingFaceContainer::type::iterator
  AddOutgoing (Ptr<CcnxFace> face);

  /**
   * @brief Remove all references to face.
   * 
   * This method should be called before face is completely removed from the stack.
   * Face is removed from the lists of incoming and outgoing faces
   */
  void
  RemoveAllReferencesToFace (Ptr<CcnxFace> face);

protected:
  
private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxPitEntry &entry);
  
public:
  Ptr<CcnxNameComponents> m_prefix; ///< \brief Prefix of the PIT entry
  const CcnxFibEntry &m_fibEntry; ///< \brief FIB entry related to this prefix
  std::set<uint32_t> m_seenNonces; ///< \brief map of nonces that were seen for this prefix
  
  CcnxPitEntryIncomingFaceContainer::type m_incoming; ///< \brief container for incoming interests
  CcnxPitEntryOutgoingFaceContainer::type m_outgoing; ///< \brief container for outgoing interests

  Time m_expireTime;         ///< \brief Time when PIT entry will be removed
  bool m_timerExpired;       ///< \brief flag indicating that PIT timer has expired
  int  m_counterExpirations; ///< \brief whether timer is expired (+ number of times timer expired)
};

} // namespace ns3

#endif // _CCNX_PIT_ENTRY_H_
