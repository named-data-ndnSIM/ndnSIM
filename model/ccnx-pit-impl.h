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

#ifndef _CCNX_PIT_IMPL_H_
#define	_CCNX_PIT_IMPL_H_

#include "ccnx-pit.h"
#include "../utils/trie-with-policy.h"
#include "../utils/empty-policy.h"
#include "ns3/ccnx-name-components.h"

namespace ns3 {

class CcnxPitEntryImpl : public CcnxPitEntry
{
public:
  typedef ndnSIM::trie_with_policy<
    CcnxNameComponents,
    ndnSIM::smart_pointer_payload_traits<CcnxPitEntryImpl>,
    ndnSIM::empty_policy_traits
    > trie;

  CcnxPitEntryImpl (const Ptr<const CcnxNameComponents> &prefix, const Time &offsetTime, Ptr<CcnxFibEntry> fibEntry)
    : CcnxPitEntry (prefix, offsetTime, fibEntry)
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

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/**
 * \ingroup ccnx
 * \brief Class implementing Pending Interests Table
 */
class CcnxPitImpl : public CcnxPit
                  , protected CcnxPitEntryImpl::trie
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief PIT constructor
   */
  CcnxPitImpl ();

  /**
   * \brief Destructor
   */
  virtual ~CcnxPitImpl ();

  // inherited from CcnxPit  
  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxContentObjectHeader &header) const;

  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxInterestHeader &header);

  virtual bool
  CheckIfDuplicate (Ptr<CcnxPitEntry> entry, const CcnxInterestHeader &header);
  
  virtual Ptr<CcnxPitEntry>
  Create (const CcnxInterestHeader &header);
  
  virtual void
  MarkErased (Ptr<CcnxPitEntry> entry);

  virtual void
  Print (std::ostream &os) const;
  
protected:
  // inherited from CcnxPit
  virtual void DoCleanExpired ();
  
  // inherited from Object class                                                                                                                                                        
  virtual void NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object
  virtual void DoDispose (); ///< @brief Do cleanup

private:
  uint32_t
  GetMaxSize () const;

  void
  SetMaxSize (uint32_t maxSize);
  
private:
  Ptr<CcnxFib> m_fib; ///< \brief Link to FIB table
};

} // namespace ns3

#endif	/* CCNX_PIT_IMPL_H */
