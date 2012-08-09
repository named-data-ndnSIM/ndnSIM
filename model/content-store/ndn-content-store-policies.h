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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef NDN_CONTENT_STORE_POLICIES_H
#define	NDN_CONTENT_STORE_POLICIES_H

// #include "ns3/ndn.h"

#include "ndn-content-store-impl.h"

#include "../../utils/trie.h"
#include "../../utils/trie-with-policy.h"

#include "../../utils/lru-policy.h"
#include "../../utils/random-policy.h"
#include "../../utils/fifo-policy.h"

namespace  ns3
{

/**
 * \ingroup ndn
 * \brief Content Store with LRU replacement policy
 */
class NdnContentStoreLru :
    public NdnContentStoreImpl<
       ndnSIM::trie_with_policy<NdnNameComponents,
                                ndnSIM::smart_pointer_payload_traits<NdnContentStoreEntry>,
                                ndnSIM::lru_policy_traits >
      >
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
  NdnContentStoreLru ();
  virtual ~NdnContentStoreLru ();

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};


/**
 * \ingroup ndn
 * \brief Content Store with RANDOM replacement policy
 */
class NdnContentStoreRandom :
    public NdnContentStoreImpl<
      ndnSIM::trie_with_policy<NdnNameComponents,
                       ndnSIM::smart_pointer_payload_traits<NdnContentStoreEntry>,
                       ndnSIM::random_policy_traits >
      >
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
  NdnContentStoreRandom ();
  virtual ~NdnContentStoreRandom ();

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};

/**
 * \ingroup ndn
 * \brief Content Store with FIFO replacement policy
 */
class NdnContentStoreFifo :
    public NdnContentStoreImpl<
      ndnSIM::trie_with_policy<NdnNameComponents,
                       ndnSIM::smart_pointer_payload_traits<NdnContentStoreEntry>,
                       ndnSIM::fifo_policy_traits >
      >
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
  NdnContentStoreFifo ();
  virtual ~NdnContentStoreFifo ();

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};


} //namespace ns3

#endif // NDN_CONTENT_STORE_POLICIES_H
