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

#ifndef _NDN_FIB_IMPL_H_
#define	_NDN_FIB_IMPL_H_

#include "ns3/ndn-fib.h"
#include "ns3/ndn-name-components.h"

#include "../../utils/trie-with-policy.h"
#include "../../utils/counting-policy.h"

namespace ns3 {

class NdnFibEntryImpl : public NdnFibEntry
{
public:
  typedef ndnSIM::trie_with_policy<
    NdnNameComponents,
    ndnSIM::smart_pointer_payload_traits<NdnFibEntryImpl>,
    ndnSIM::counting_policy_traits
    > trie;

  NdnFibEntryImpl (const Ptr<const NdnNameComponents> &prefix)
    : NdnFibEntry (prefix)
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

struct NdnFibEntryContainer
{
  typedef ndnSIM::trie_with_policy<
    NdnNameComponents,
    ndnSIM::smart_pointer_payload_traits<NdnFibEntryImpl>,
    ndnSIM::counting_policy_traits
    > type;
};

/**
 * \ingroup ndn
 * \brief Class implementing FIB functionality
 */
class NdnFibImpl : public NdnFib,
                    private NdnFibEntryContainer::type
{
public:
  typedef NdnFibEntryContainer::type super;
  
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief Constructor
   */
  NdnFibImpl ();

  virtual Ptr<NdnFibEntry>
  LongestPrefixMatch (const NdnInterestHeader &interest);
  
  virtual Ptr<NdnFibEntry>
  Add (const NdnNameComponents &prefix, Ptr<NdnFace> face, int32_t metric);

  virtual Ptr<NdnFibEntry>
  Add (const Ptr<const NdnNameComponents> &prefix, Ptr<NdnFace> face, int32_t metric);

  virtual void
  Remove (const Ptr<const NdnNameComponents> &prefix);

  virtual void
  InvalidateAll ();
  
  virtual void
  RemoveFromAll (Ptr<NdnFace> face);

  virtual void
  Print (std::ostream &os) const;

  virtual uint32_t
  GetSize () const;

  virtual Ptr<const NdnFibEntry>
  Begin ();

  virtual Ptr<const NdnFibEntry>
  End ();

  virtual Ptr<const NdnFibEntry>
  Next (Ptr<const NdnFibEntry> item);
  
  // /**
  //  * @brief Modify element in container
  //  */
  // template<typename Modifier>
  // bool
  // modify (Ptr<NdnFibEntry> item, Modifier mod)
  // {
  //   return super::modify (StaticCast<NdnFibEntryImpl> (item)->to_iterator (), mod);
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
  RemoveFace (super::parent_trie &item, Ptr<NdnFace> face);
  
private:
  Ptr<Node> m_node;
};
 
} // namespace ns3

#endif	/* _NDN_FIB_IMPL_H_ */
