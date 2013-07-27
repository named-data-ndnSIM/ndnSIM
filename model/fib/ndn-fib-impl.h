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
#include "ns3/ndn-name.h"

#include "../../utils/trie/trie-with-policy.h"
#include "../../utils/trie/counting-policy.h"

namespace ns3 {
namespace ndn {
namespace fib {

/**
 * @ingroup ndn-fib
 * @brief FIB entry implementation with with additional references to the base container
 */
class EntryImpl : public Entry
{
public:
  typedef ndnSIM::trie_with_policy<
    Name,
    ndnSIM::smart_pointer_payload_traits<EntryImpl>,
    ndnSIM::counting_policy_traits
    > trie;

  EntryImpl (Ptr<Fib> fib, const Ptr<const Name> &prefix)
    : Entry (fib, prefix)
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

/**
 * @ingroup ndn-fib
 * \brief Class implementing FIB functionality
 */
class FibImpl : public Fib,
                protected ndnSIM::trie_with_policy< Name,
                                                    ndnSIM::smart_pointer_payload_traits< EntryImpl >,
                                                    ndnSIM::counting_policy_traits >
{
public:
  typedef ndnSIM::trie_with_policy< Name,
                                    ndnSIM::smart_pointer_payload_traits<EntryImpl>,
                                    ndnSIM::counting_policy_traits > super;
  
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief Constructor
   */
  FibImpl ();

  virtual Ptr<Entry>
  LongestPrefixMatch (const Interest &interest);

  virtual Ptr<fib::Entry>
  Find (const Name &prefix);
  
  virtual Ptr<Entry>
  Add (const Name &prefix, Ptr<Face> face, int32_t metric);

  virtual Ptr<Entry>
  Add (const Ptr<const Name> &prefix, Ptr<Face> face, int32_t metric);

  virtual void
  Remove (const Ptr<const Name> &prefix);

  virtual void
  InvalidateAll ();
  
  virtual void
  RemoveFromAll (Ptr<Face> face);

  virtual void
  Print (std::ostream &os) const;

  virtual uint32_t
  GetSize () const;

  virtual Ptr<const Entry>
  Begin () const;

  virtual Ptr<Entry>
  Begin ();

  virtual Ptr<const Entry>
  End () const;

  virtual Ptr<Entry>
  End ();

  virtual Ptr<const Entry>
  Next (Ptr<const Entry> item) const;
  
  virtual Ptr<Entry>
  Next (Ptr<Entry> item);
  
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
  RemoveFace (super::parent_trie &item, Ptr<Face> face);
};

} // namespace fib
} // namespace ndn
} // namespace ns3

#endif	/* _NDN_FIB_IMPL_H_ */
