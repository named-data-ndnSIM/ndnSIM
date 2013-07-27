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

#ifndef _NDN_PIT_ENTRY_H_
#define _NDN_PIT_ENTRY_H_

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

#include "ns3/ndn-fib.h"

#include "ns3/ndn-pit-entry-incoming-face.h"
#include "ns3/ndn-pit-entry-outgoing-face.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
// #include <boost/multi_index/composite_key.hpp>
// #include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
// #include <boost/multi_index/mem_fun.hpp>
#include <set>
#include <boost/shared_ptr.hpp>

namespace ns3 {
namespace ndn {

class Pit;

namespace fw { class Tag; }

namespace pit {

/**
 * @ingroup ndn-pit
 * @brief structure for PIT entry
 *
 * All set-methods are virtual, in case index rearrangement is necessary in the derived classes
 */
class Entry : public SimpleRefCount<Entry>
{
public:
  typedef std::set< IncomingFace > in_container; ///< @brief incoming faces container type
  typedef in_container::iterator in_iterator;                ///< @brief iterator to incoming faces

  // typedef OutgoingFaceContainer::type out_container; ///< @brief outgoing faces container type
  typedef std::set< OutgoingFace > out_container; ///< @brief outgoing faces container type
  typedef out_container::iterator out_iterator;              ///< @brief iterator to outgoing faces

  typedef std::set< uint32_t > nonce_container;  ///< @brief nonce container type

  /**
   * \brief PIT entry constructor
   * \param prefix Prefix of the PIT entry
   * \param offsetTime Relative time to the current moment, representing PIT entry lifetime
   * \param fibEntry A FIB entry associated with the PIT entry
   */
  Entry (Pit &container, Ptr<const Interest> header, Ptr<fib::Entry> fibEntry);

  /**
   * @brief Virtual destructor
   */
  virtual ~Entry ();

  /**
   * @brief Update lifetime of PIT entry
   *
   * @param lifetime desired lifetime of the pit entry (relative to the Simulator::Now ())
   *
   * This function will update PIT entry lifetime to the maximum of the current lifetime and
   * the lifetime Simulator::Now () + lifetime
   */
  virtual void
  UpdateLifetime (const Time &lifetime);

  /**
   * @brief Offset the currently set PIT lifetime (allowed both negative and positive offsets)
   *
   * @param offsetTime positive or negative offset for the PIT lifetime.
   *
   * If PIT expire time becomes less than Simulator::Now, then it is adjusted to Simulator::Now.
   */
  virtual void
  OffsetLifetime (const Time &offsetTime);

  /**
   * @brief Get prefix of the PIT entry
   */
  const Name &
  GetPrefix () const;

  /**
   * @brief Get current expiration time of the record
   *
   * @returns current expiration time of the record
   */
  const Time &
  GetExpireTime () const;

  /**
   * @brief Check if nonce `nonce` for the same prefix has already been seen
   *
   * @param nonce Nonce to check
   */
  bool
  IsNonceSeen (uint32_t nonce) const;

  /**
   * @brief Add `nonce` to the list of seen nonces
   *
   * @param nonce nonce to add to the list of seen nonces
   *
   * All nonces are stored for the lifetime of the PIT entry
   */
  virtual void
  AddSeenNonce (uint32_t nonce);

  /**
   * @brief Add `face` to the list of incoming faces
   *
   * @param face Face to add to the list of incoming faces
   * @returns iterator to the added entry
   */
  virtual in_iterator
  AddIncoming (Ptr<Face> face);

  /**
   * @brief Remove incoming entry for face `face`
   */
  virtual void
  RemoveIncoming (Ptr<Face> face);

  /**
   * @brief Clear all incoming faces either after all of them were satisfied or NACKed
   */
  virtual void
  ClearIncoming ();

  /**
   * @brief Add `face` to the list of outgoing faces
   *
   * @param face Face to add to the list of outgoing faces
   * @returns iterator to the added entry
   */
  virtual out_iterator
  AddOutgoing (Ptr<Face> face);

  /**
   * @brief Clear all incoming faces either after all of them were satisfied or NACKed
   */
  virtual void
  ClearOutgoing ();

  /**
   * @brief Remove all references to face.
   *
   * This method should be called before face is completely removed from the stack.
   * Face is removed from the lists of incoming and outgoing faces
   */
  virtual void
  RemoveAllReferencesToFace (Ptr<Face> face);

  /**
   * @brief Flag outgoing face as hopeless
   */
  // virtual void
  // SetWaitingInVain (out_iterator face);
  virtual void
  SetWaitingInVain (Ptr<Face> face);

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
  AreTherePromisingOutgoingFacesExcept (Ptr<Face> face) const;

  /**
   * @brief Increase maximum limit of allowed retransmission per outgoing face
   */
  virtual void
  IncreaseAllowedRetxCount ();

  /**
   * @brief Get maximum allowed number of retransmissions via outgoing faces
   */
  uint32_t
  GetMaxRetxCount () const;

  /**
   * @brief Get associated FIB entry
   */
  Ptr<fib::Entry>
  GetFibEntry ();

  /**
   * @brief Get associated list (const reference) of incoming faces
   */
  const in_container &
  GetIncoming () const;

  /**
   * @brief Get associated list (const reference) of outgoing faces
   */
  const out_container &
  GetOutgoing () const;

  /**
   * @brief Get number of outgoing faces (needed for python bindings)
   */
  uint32_t
  GetOutgoingCount () const;

  /**
   * @brief Add new forwarding strategy tag
   */
  inline void
  AddFwTag (boost::shared_ptr< fw::Tag > tag);

  /**
   * @brief Get forwarding strategy tag (tag is not removed)
   */
  template<class T>
  inline boost::shared_ptr< T >
  GetFwTag ();

  /**
   * @brief Remove the forwarding strategy tag
   */
  template<class T>
  inline void
  RemoveFwTag ();

  /**
   * @brief Get Interest (if several interests are received, then nonce is from the first Interest)
   */
  Ptr<const Interest>
  GetInterest () const;

private:
  friend std::ostream& operator<< (std::ostream& os, const Entry &entry);

protected:
  Pit &m_container; ///< @brief Reference to the container (to rearrange indexes, if necessary)

  Ptr<const Interest> m_interest; ///< \brief Interest of the PIT entry (if several interests are received, then nonce is from the first Interest)
  Ptr<fib::Entry> m_fibEntry;     ///< \brief FIB entry related to this prefix

  nonce_container m_seenNonces;  ///< \brief map of nonces that were seen for this prefix
  in_container  m_incoming;      ///< \brief container for incoming interests
  out_container m_outgoing;      ///< \brief container for outgoing interests

  Time m_expireTime;         ///< \brief Time when PIT entry will be removed

  Time m_lastRetransmission; ///< @brief Last time when number of retransmissions were increased
  uint32_t m_maxRetxCount;   ///< @brief Maximum allowed number of retransmissions via outgoing faces

  std::list< boost::shared_ptr<fw::Tag> > m_fwTags; ///< @brief Forwarding strategy tags
};

/// @cond include_hidden
struct EntryIsNotEmpty
{
  bool
  operator () (Ptr<Entry> entry)
  {
    return !entry->GetIncoming ().empty ();
  }
};
/// @endcond

std::ostream& operator<< (std::ostream& os, const Entry &entry);

inline void
Entry::AddFwTag (boost::shared_ptr< fw::Tag > tag)
{
  m_fwTags.push_back (tag);
}

/**
 * @brief Get and remove forwarding strategy tag
 */
template<class T>
inline boost::shared_ptr< T >
Entry::GetFwTag ()
{
  for (std::list< boost::shared_ptr<fw::Tag> >::iterator item = m_fwTags.begin ();
       item != m_fwTags.end ();
       item ++)
    {
      boost::shared_ptr< T > retPtr = boost::dynamic_pointer_cast<T> (*item);
      if (retPtr != boost::shared_ptr< T > ())
        {
          return retPtr;
        }
    }

  return boost::shared_ptr< T > ();
}

// /**
//  * @brief Peek the forwarding strategy tag
//  */
// template<class T>
// inline boost::shared_ptr< const T >
// Entry::PeekFwTag () const
// {
//   for (std::list< boost::shared_ptr<fw::Tag> >::const_iterator item = m_fwTags.begin ();
//        item != m_fwTags.end ();
//        item ++)
//     {
//       boost::shared_ptr< const T > retPtr = boost::dynamic_pointer_cast<const T> (*item);
//       if (retPtr != 0)
//         {
//           return retPtr;
//         }
//     }

//   return 0;
// }

template<class T>
inline void
Entry::RemoveFwTag ()
{
  for (std::list< boost::shared_ptr<fw::Tag> >::iterator item = m_fwTags.begin ();
       item != m_fwTags.end ();
       item ++)
    {
      boost::shared_ptr< T > retPtr = boost::dynamic_pointer_cast< T > (*item);
      if (retPtr != boost::shared_ptr< T > ())
        {
          m_fwTags.erase (item);
          return;
        }
    }
}


} // namespace pit
} // namespace ndn
} // namespace ns3

#endif // _NDN_PIT_ENTRY_H_
