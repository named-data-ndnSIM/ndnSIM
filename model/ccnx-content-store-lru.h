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

#ifndef CCNX_CONTENT_STORE_LRU_H
#define	CCNX_CONTENT_STORE_LRU_H

#include "ccnx-content-store.h"

#include "ccnx.h"
#include "ccnx-name-components-hash-helper.h"

#include "../utils/trie.h"

namespace  ns3
{
/**
 * \ingroup ccnx
 * \brief Typedef for content store container implemented as trie with configurable replacement policy
 */
typedef indexed_trie<CcnxNameComponents,
                     CcnxContentStoreEntry, smart_pointer_payload_traits<CcnxContentStoreEntry>,
                     lru_policy_traits<CcnxNameComponents,
                                       CcnxContentStoreEntry,
                                       smart_pointer_payload_traits<CcnxContentStoreEntry> > > CcnxContentStoreLruContainer;

/**
 * \ingroup ccnx
 * \brief NDN content store entry
 */
class CcnxContentStoreLru : public CcnxContentStore
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
  CcnxContentStoreLru ();
  virtual ~CcnxContentStoreLru ();

  /**
   * \brief Set maximum size of content store
   *
   * \param size size in packets
   */
  void
  SetMaxSize (uint32_t maxSize);

  /**
   * \brief Get maximum size of content store
   *
   * \returns size in packets
   */
  uint32_t
  GetMaxSize () const;
  
  // from CcnxContentStore
  virtual boost::tuple<Ptr<Packet>, Ptr<const CcnxContentObjectHeader>, Ptr<const Packet> >
  Lookup (Ptr<const CcnxInterestHeader> interest);
            
  virtual bool
  Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);

  virtual void
  Print () const;
  
private:
  size_t m_maxSize; ///< \brief maximum number of entries in cache

  /**
   * \brief Content store implemented as a Boost.MultiIndex container
   * \internal
   */
  CcnxContentStoreLruContainer m_contentStore;
};

} //namespace ns3

#endif // CCNX_CONTENT_STORE_LRU_H
