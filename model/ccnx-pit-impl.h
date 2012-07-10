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
#include "../utils/persistent-policy.h"
#include "ns3/ccnx-name-components.h"

namespace ns3 {

template<class Pit>
class CcnxPitEntryImpl : public CcnxPitEntry
{
public:
  CcnxPitEntryImpl (CcnxPit &pit,
                    Ptr<const CcnxInterestHeader> header,
                    Ptr<CcnxFibEntry> fibEntry)
  : CcnxPitEntry (pit, header, fibEntry)
  , item_ (0)
  {
    static_cast<Pit&> (m_container).i_time.insert (*this);    
  }
  
  virtual ~CcnxPitEntryImpl ()
  {
    static_cast<Pit&> (m_container).i_time.erase (*this);
  }

  // to make sure policies work
  void
  SetTrie (typename Pit::super::iterator item) { item_ = item; }

  typename Pit::super::iterator to_iterator () { return item_; }
  typename Pit::super::const_iterator to_iterator () const { return item_; }

public:
  boost::intrusive::set_member_hook<> time_hook_;
  
private:
  typename Pit::super::iterator item_;
};

template<class T>
struct TimestampIndex
{
  bool
  operator () (const T &a, const T &b) const
  {
    return a.GetExpireTime () < b.GetExpireTime ();
  }
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/**
 * \ingroup ccnx
 * \brief Class implementing Pending Interests Table
 */
class CcnxPitImpl : public CcnxPit
                  , protected ndnSIM::trie_with_policy<CcnxNameComponents,
                                                       ndnSIM::smart_pointer_payload_traits<CcnxPitEntryImpl< CcnxPitImpl > >,
                                                       ndnSIM::persistent_policy_traits
                                                       >
{
public:
  typedef ndnSIM::trie_with_policy<CcnxNameComponents,
                                   ndnSIM::smart_pointer_payload_traits<CcnxPitEntryImpl< CcnxPitImpl > >,
                                   ndnSIM::persistent_policy_traits
                                   > super;
  typedef CcnxPitEntryImpl< CcnxPitImpl > entry;

  // typedef CcnxPitEntryImpl::trie super;

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
  Lookup (const CcnxContentObjectHeader &header);

  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxInterestHeader &header);

  virtual Ptr<CcnxPitEntry>
  Create (Ptr<const CcnxInterestHeader> header);
  
  virtual void
  MarkErased (Ptr<CcnxPitEntry> entry);

  virtual void
  Print (std::ostream &os) const;

  virtual Ptr<CcnxPitEntry>
  Begin ();

  virtual Ptr<CcnxPitEntry>
  End ();

  virtual Ptr<CcnxPitEntry>
  Next (Ptr<CcnxPitEntry>);
  
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

  // indexes
  typedef
  boost::intrusive::set<entry,
                        boost::intrusive::compare < TimestampIndex< entry > >,
                        boost::intrusive::member_hook< entry,
                                                       boost::intrusive::set_member_hook<>,
                                                       &entry::time_hook_>
                        > expireTimeIndexType;
  expireTimeIndexType i_time; 
                        
  friend class CcnxPitEntryImpl< CcnxPitImpl >;
};

} // namespace ns3

#endif	/* CCNX_PIT_IMPL_H */
