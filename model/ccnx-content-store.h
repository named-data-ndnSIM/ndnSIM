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

#ifndef CCNX_CONTENT_STORE_H
#define	CCNX_CONTENT_STORE_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/packet.h"

#include <list>
#include <string>
#include <iostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "ccnx.h"
#include "hash-helper.h"
#include "ccnx-content-object-header.h"
#include "ccnx-interest-header.h"
#include "ccnx-name-components.h"

namespace  ns3
{
/**
 * \ingroup ccnx
 * \brief NDN content store entry
 *
 * Content store entry stores separately pseudo header and content of
 * ContentObject packet.  It is responsibility of the caller to
 * construct a fully formed CcnxPacket by calling Copy(), AddHeader(),
 * AddTail() on the packet received by GetPacket() method.
 *
 * GetFullyFormedCcnxPacket method provided as a convenience
 */
class CcnxContentStoreEntry
{
public:
  /**
   * \brief Construct content store entry
   *
   * \param header Parsed CcnxContentObject header
   * \param packet Original CCNx packet
   *
   * The constructor will make a copy of the supplied packet and calls
   * RemoveHeader and RemoveTail on the copy.
   */
  CcnxContentStoreEntry (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);

  /**
   * \brief Get prefix of the stored entry
   * \returns prefix of the stored entry
   */
  inline const CcnxNameComponents&
  GetName () const;

  /**
   * \brief Get CcnxContentObjectHeader of the stored entry
   * \returns CcnxContentObjectHeader of the stored entry
   */
  inline Ptr<const CcnxContentObjectHeader>
  GetHeader () const;

  /**
   * \brief Get content of the stored entry
   * \returns content of the stored entry
   */
  inline Ptr<const Packet>
  GetPacket () const;

  /**
   * \brief Convenience method to create a fully formed CCNx packet from stored header and content
   * \returns A read-write copy of the packet with CcnxContentObjectHeader and CcxnContentObjectTail
   */
  Ptr<Packet>
  GetFullyFormedCcnxPacket () const;

// Copy constructor is required by the container. Though, we're
// storing only two pointers, so shouldn't be a problem
// private:
//   CcnxContentStoreEntry (const CcnxContentStoreEntry &); ///< disabled copy constructor
//   CcnxContentStoreEntry& operator= (const CcnxContentStoreEntry&); ///< disabled copy operator
  
private:
  Ptr<CcnxContentObjectHeader> m_header; ///< \brief non-modifiable CcnxContentObjectHeader
  Ptr<Packet> m_packet; ///< \brief non-modifiable content of the ContentObject packet
};

/**
 * \ingroup ccnx
 * \brief Typedef for content store container implemented as a Boost.MultiIndex container
 *
 * - First index (tag<prefix>) is a unique hash index based on NDN prefix of the stored content.
 * - Second index (tag<mru>) is a sequential index used to maintain up to m_maxSize most recent used (MRU) entries in the content store
 * - Third index (tag<ordered>) is just a helper to provide stored prefixes in ordered way. Should be disabled in production build
 *
 * \see http://www.boost.org/doc/libs/1_46_1/libs/multi_index/doc/ for more information on Boost.MultiIndex library
 */
struct CcnxContentStoreContainer
{
  typedef
  boost::multi_index::multi_index_container<
    CcnxContentStoreEntry,
    boost::multi_index::indexed_by<
      boost::multi_index::hashed_unique<
        boost::multi_index::tag<__ccnx_private::i_prefix>,
        boost::multi_index::const_mem_fun<CcnxContentStoreEntry,
                                          const CcnxNameComponents&,
                                          &CcnxContentStoreEntry::GetName>,
        CcnxPrefixHash>,
      boost::multi_index::sequenced<boost::multi_index::tag<__ccnx_private::i_mru> >
#ifdef _DEBUG
      ,
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private::i_ordered>,
        boost::multi_index::const_mem_fun<CcnxContentStoreEntry,
                                          const CcnxNameComponents&,
                                          &CcnxContentStoreEntry::GetName>
          >
#endif
      >
    > type;
};

/**
 * \ingroup ccnx
 * \brief NDN content store entry
 */
class CcnxContentStore : public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * Default constructor
   */
  CcnxContentStore( );
  virtual ~CcnxContentStore( );
            
  /**
   * \brief Find corresponding CS entry for the given interest
   *
   * \param interest Interest for which matching content store entry
   * will be searched
   *
   * If an entry is found, it is promoted to the top of most recent
   * used entries index, \see m_contentStore
   */
  Ptr<Packet>
  Lookup (Ptr<const CcnxInterestHeader> interest);
            
  /**
   * \brief Add a new content to the content store.
   *
   * \param header Fully parsed CcnxContentObjectHeader
   * \param packet Fully formed CCNx packet to add to content store
   * (will be copied and stripped down of headers)
   *
   * If entry with the same prefix exists, the old entry will be
   * promoted to the top of the MRU hash
   */
  void
  Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);

  /**
   * \brief Set maximum size of content store
   *
   * \param size size in packets
   */
  inline void
  SetMaxSize (uint32_t maxSize);

  /**
   * \brief Get maximum size of content store
   *
   * \returns size in packets
   */
  inline uint32_t
  GetMaxSize () const;
  
  /**
   * \brief Print out content store entries
   *
   * Debug build provides dumping of content store entries in
   * lexicographical order of corresponding prefixes
   *
   * Release build dumps everything in MRU order
   */
  void Print () const;
            
protected:
  // /**
  //  * \brief Move the given CS entry to the head of the list
  //  *
  //  * \param entry Content Store entry
  //  */
  // void Promote( CsEntry &entry );

  /**
   * \todo Alex: DoDispose and NotifyNewAggregate are seem to be very
   * important, but I'm not yet sure what exactly they are supposed to
   * do
   */
  // virtual void DoDispose ();
  // virtual void NotifyNewAggregate ();

private:
  CcnxContentStore (const CcnxContentStore &o); ///< Disabled copy constructor
  CcnxContentStore& operator= (const CcnxContentStore &o); ///< Disabled copy operator
 
private:
  size_t m_maxSize; ///< \brief maximum number of entries in cache \internal
  // string_key_hash_t<CsEntry>  m_contentStore;     ///< \brief actual content store \internal

  /**
   * \brief Content store implemented as a Boost.MultiIndex container
   * \internal
   */
    CcnxContentStoreContainer::type m_contentStore;
};

inline std::ostream&
operator<< (std::ostream &os, const CcnxContentStore &cs)
{
  cs.Print ();
  return os;
}

const CcnxNameComponents&
CcnxContentStoreEntry::GetName () const
{
  return m_header->GetName ();
}

Ptr<const CcnxContentObjectHeader>
CcnxContentStoreEntry::GetHeader () const
{
  return m_header;
}

Ptr<const Packet>
CcnxContentStoreEntry::GetPacket () const
{
  return m_packet;
}


inline void
CcnxContentStore::SetMaxSize (uint32_t maxSize)
{
  m_maxSize = maxSize;
}

inline uint32_t
CcnxContentStore::GetMaxSize () const
{
  return m_maxSize;
}

} //namespace ns3

#endif // CCNX_CONTENT_STORE_H
