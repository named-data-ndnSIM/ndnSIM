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

#ifndef _CCNX_FIB_IMPL_H_
#define	_CCNX_FIB_IMPL_H_

#include "ns3/ccnx-fib.h"
#include "ns3/ccnx-name-components.h"

#include "../utils/trie-with-policy.h"
#include "../utils/empty-policy.h"

namespace ns3 {

class CcnxFibEntryImpl : public CcnxFibEntry
{
public:
  typedef ndnSIM::trie_with_policy<
    CcnxNameComponents,
    ndnSIM::smart_pointer_payload_traits<CcnxFibEntryImpl>,
    ndnSIM::empty_policy_traits
    > trie;

  CcnxFibEntryImpl (const Ptr<const CcnxNameComponents> &prefix)
    : CcnxFibEntry (prefix)
    , item_ (0)
  {
  }

  void
  SetTrie (trie::iterator item)
  {
    item_ = item;
  }

  trie::iterator to_iterator () { return item_; }
  trie::const_iterator to_iterator () const { return item_; }
  
private:
  trie::iterator item_;
};

struct CcnxFibEntryContainer
{
  typedef ndnSIM::trie_with_policy<
    CcnxNameComponents,
    ndnSIM::smart_pointer_payload_traits<CcnxFibEntryImpl>,
    ndnSIM::empty_policy_traits
    > type;
};

/**
 * \ingroup ccnx
 * \brief Class implementing FIB functionality
 */
class CcnxFibImpl : public CcnxFib,
                    private CcnxFibEntryContainer::type
{
public:
  typedef CcnxFibEntryContainer::type super;
  
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief Constructor
   */
  CcnxFibImpl ();

  virtual Ptr<CcnxFibEntry>
  LongestPrefixMatch (const CcnxInterestHeader &interest);
  
  virtual Ptr<CcnxFibEntry>
  Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric);

  virtual Ptr<CcnxFibEntry>
  Add (const Ptr<const CcnxNameComponents> &prefix, Ptr<CcnxFace> face, int32_t metric);

  virtual void
  Remove (const Ptr<const CcnxNameComponents> &prefix);

  virtual void
  InvalidateAll ();
  
  virtual void
  RemoveFromAll (Ptr<CcnxFace> face);

  virtual void
  Print (std::ostream &os) const;

  virtual Ptr<const CcnxFibEntry>
  Begin ();

  virtual Ptr<const CcnxFibEntry>
  End ();

  virtual Ptr<const CcnxFibEntry>
  Next (Ptr<const CcnxFibEntry> item);
  
  // /**
  //  * @brief Modify element in container
  //  */
  // template<typename Modifier>
  // bool
  // modify (Ptr<CcnxFibEntry> item, Modifier mod)
  // {
  //   return super::modify (StaticCast<CcnxFibEntryImpl> (item)->to_iterator (), mod);
  // }
  
protected:
  // inherited from Object class
  virtual void NotifyNewAggregate (); ///< @brief Notify when object is aggregated
  virtual void DoDispose (); ///< @brief Perform cleanup

private:
  /**
   * @brief Remove reference to a face from the entry. If entry had only this face, the whole
   * entry will be removed
   */
  void
  Remove (super::parent_trie &item, Ptr<CcnxFace> face);
  
private:
  Ptr<Node> m_node;
};
 
} // namespace ns3

#endif	/* _CCNX_FIB_IMPL_H_ */
