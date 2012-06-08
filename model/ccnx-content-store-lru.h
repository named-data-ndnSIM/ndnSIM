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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/tuple/tuple.hpp>

#include "ccnx.h"
#include "ccnx-name-components-hash-helper.h"
// #include "ccnx-content-object-header.h"
// #include "ccnx-interest-header.h"
// #include "ccnx-name-components.h"

namespace  ns3
{
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
struct CcnxContentStoreLruContainer
{
  /// @cond include_hidden
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
  /// @endcond
};

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
  CcnxContentStoreLruContainer::type m_contentStore;
};

} //namespace ns3

#endif // CCNX_CONTENT_STORE_LRU_H
