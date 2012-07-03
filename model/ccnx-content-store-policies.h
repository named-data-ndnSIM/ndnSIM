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

#ifndef CCNX_CONTENT_STORE_POLICIES_H
#define	CCNX_CONTENT_STORE_POLICIES_H

// #include "ns3/ccnx.h"

#include "ccnx-content-store-impl.h"

#include "../utils/trie.h"
#include "../utils/trie-with-policy.h"

#include "../utils/lru-policy.h"
#include "../utils/random-policy.h"
#include "../utils/fifo-policy.h"

namespace  ns3
{

/**
 * \ingroup ccnx
 * \brief Content Store with LRU replacement policy
 */
class CcnxContentStoreLru :
    public CcnxContentStoreImpl<
      trie_with_policy<CcnxNameComponents,
                       smart_pointer_payload_traits<CcnxContentStoreEntry>,
                       lru_policy_traits >
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
  CcnxContentStoreLru ();
  virtual ~CcnxContentStoreLru ();

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};


/**
 * \ingroup ccnx
 * \brief Content Store with RANDOM replacement policy
 */
class CcnxContentStoreRandom :
    public CcnxContentStoreImpl<
      trie_with_policy<CcnxNameComponents,
                       smart_pointer_payload_traits<CcnxContentStoreEntry>,
                       random_policy_traits >
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
  CcnxContentStoreRandom ();
  virtual ~CcnxContentStoreRandom ();

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};

/**
 * \ingroup ccnx
 * \brief Content Store with FIFO replacement policy
 */
class CcnxContentStoreFifo :
    public CcnxContentStoreImpl<
      trie_with_policy<CcnxNameComponents,
                       smart_pointer_payload_traits<CcnxContentStoreEntry>,
                       fifo_policy_traits >
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
  CcnxContentStoreFifo ();
  virtual ~CcnxContentStoreFifo ();

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};


} //namespace ns3

#endif // CCNX_CONTENT_STORE_POLICIES_H
