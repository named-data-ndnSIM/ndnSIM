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
#include "ns3/simple-ref-count.h"

#include "ns3/ccnx-fib.h"
#include "ccnx-pit-entry-incoming-face.h"
#include "ccnx-pit-entry-outgoing-face.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
// #include <boost/multi_index/composite_key.hpp>
// #include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
// #include <boost/multi_index/mem_fun.hpp>
#include <set>

namespace ns3 {

class CcnxFace;
class CcnxNameComponents;
class CcnxPit;

/// @cond include_hidden
namespace __ccnx_private
{
class i_retx {};
}
/// @endcond

/**
 * \ingroup ccnx
 * \brief Typedef for indexed face container of CcnxPitEntryOutgoingFace
 *
 * Indexes:
 * - by face (may be it will be possible to replace with just the std::map)
 */
struct CcnxPitEntryOutgoingFaceContainer
{
  /// @cond include_hidden
  typedef boost::multi_index::multi_index_container<
    CcnxPitEntryOutgoingFace,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private::i_face>,
        boost::multi_index::member<CcnxPitEntryOutgoingFace, Ptr<CcnxFace>, &CcnxPitEntryOutgoingFace::m_face>
      >,
      boost::multi_index::ordered_non_unique<
        boost::multi_index::tag<__ccnx_private::i_retx>,
        boost::multi_index::member<CcnxPitEntryOutgoingFace, uint32_t, &CcnxPitEntryOutgoingFace::m_retxCount>
      >    
    >
   > type;
  /// @endcond
};


/**
 * \ingroup ccnx
 * \brief structure for PIT entry
 *
 * All set-methods are virtual, in case index rearrangement is necessary in the derived classes
 */
class CcnxPitEntry : public SimpleRefCount<CcnxPitEntry>
{
public:
  typedef std::set< CcnxPitEntryIncomingFace > in_container; ///< @brief incoming faces container type
  typedef in_container::iterator in_iterator;                ///< @brief iterator to incoming faces

  typedef CcnxPitEntryOutgoingFaceContainer::type out_container; ///< @brief outgoing faces container type
  typedef out_container::iterator out_iterator;              ///< @brief iterator to outgoing faces

  typedef std::set< uint32_t > nonce_container;  ///< @brief nonce container type
  
  /**
   * \brief PIT entry constructor
   * \param prefix Prefix of the PIT entry
   * \param offsetTime Relative time to the current moment, representing PIT entry lifetime
   * \param fibEntry A FIB entry associated with the PIT entry
   */
  CcnxPitEntry (CcnxPit &container, Ptr<const CcnxInterestHeader> header, Ptr<CcnxFibEntry> fibEntry);

  /**
   * @brief Virtual destructor
   */
  virtual ~CcnxPitEntry () {}
  
  /**
   * @brief Update lifetime of PIT entry
   *
   * This function will update PIT entry lifetime to the maximum of the current lifetime and
   * the lifetime Simulator::Now () + offsetTime
   *
   * @param offsetTime Relative time to the current moment, representing PIT entry lifetime
   */
  virtual void
  UpdateLifetime (const Time &offsetTime);

  /**
   * @brief Get prefix of the PIT entry
   */
  const CcnxNameComponents &
  GetPrefix () const
  { return *m_prefix; }

  /**
   * @brief Get current expiration time of the record
   *
   * @returns current expiration time of the record
   */
  const Time &
  GetExpireTime () const
  { return m_expireTime; }

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
  virtual void
  AddSeenNonce (uint32_t nonce)
  { m_seenNonces.insert (nonce); }

  /**
   * @brief Add `face` to the list of incoming faces
   *
   * @param face Face to add to the list of incoming faces
   * @returns iterator to the added entry
   */
  virtual in_iterator
  AddIncoming (Ptr<CcnxFace> face);

  /**
   * @brief Remove incoming entry for face `face`
   */
  virtual void
  RemoveIncoming (Ptr<CcnxFace> face);

  /**
   * @brief Clear all incoming faces either after all of them were satisfied or NACKed
   */
  virtual void
  ClearIncoming ()
  { m_incoming.clear (); }

  /**
   * @brief Add `face` to the list of outgoing faces
   *
   * @param face Face to add to the list of outgoing faces
   * @returns iterator to the added entry
   */
  virtual out_iterator
  AddOutgoing (Ptr<CcnxFace> face);

  /**
   * @brief Clear all incoming faces either after all of them were satisfied or NACKed
   */
  virtual void
  ClearOutgoing ()
  { m_outgoing.clear (); }  
  
  /**
   * @brief Remove all references to face.
   * 
   * This method should be called before face is completely removed from the stack.
   * Face is removed from the lists of incoming and outgoing faces
   */
  virtual void
  RemoveAllReferencesToFace (Ptr<CcnxFace> face);

  /**
   * @brief Flag outgoing face as hopeless
   */
  virtual void
  SetWaitingInVain (out_iterator face);
  
  /**
   * @brief Check if all outgoing faces are NACKed
   */
  bool
  AreAllOutgoingInVain () const;

  /**
   * @brief Similar to AreAllOutgoingInVain, but ignores `face`
   * \see AreAllOutgoingInVain
   **/
  bool
  AreTherePromisingOutgoingFacesExcept (Ptr<CcnxFace> face) const;

  /**
   * @brief Increase maximum limit of allowed retransmission per outgoing face
   */
  virtual void
  IncreaseAllowedRetxCount ();

  Ptr<CcnxFibEntry>
  GetFibEntry () { return m_fibEntry; };

  const in_container &
  GetIncoming () const { return m_incoming; }

  const out_container &
  GetOutgoing () const { return m_outgoing; }

  uint32_t
  GetMaxRetxCount () const { return m_maxRetxCount; }

private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxPitEntry &entry);
  
protected:
  CcnxPit &m_container; ///< @brief Reference to the container (to rearrange indexes, if necessary)
  
  Ptr<const CcnxNameComponents> m_prefix; ///< \brief Prefix of the PIT entry
  Ptr<CcnxFibEntry> m_fibEntry;     ///< \brief FIB entry related to this prefix
  
  nonce_container m_seenNonces;  ///< \brief map of nonces that were seen for this prefix  
  in_container  m_incoming;      ///< \brief container for incoming interests
  out_container m_outgoing;      ///< \brief container for outgoing interests

  Time m_expireTime;         ///< \brief Time when PIT entry will be removed

  Time m_lastRetransmission; ///< @brief Last time when number of retransmissions were increased
  uint32_t m_maxRetxCount;   ///< @brief Maximum allowed number of retransmissions via outgoing faces
};

std::ostream& operator<< (std::ostream& os, const CcnxPitEntry &entry);

} // namespace ns3

#endif // _CCNX_PIT_ENTRY_H_
