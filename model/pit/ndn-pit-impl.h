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

#ifndef _NDN_PIT_IMPL_H_
#define	_NDN_PIT_IMPL_H_

#include "ndn-pit.h"

#include "../../utils/trie/trie-with-policy.h"

#include "ndn-pit-entry-impl.h"

#include "ns3/ndn-name-components.h"

namespace ns3 {
namespace ndn {

class ForwardingStrategy;

namespace pit {

/**
 * \ingroup ndn
 * \brief Class implementing Pending Interests Table
 */
template<class Policy>
class PitImpl : public Pit
              , protected ndnSIM::trie_with_policy<NameComponents,
                                                   ndnSIM::smart_pointer_payload_traits< EntryImpl< PitImpl< Policy > > >,
                                                   // ndnSIM::persistent_policy_traits
                                                   Policy
                                                   >
{
public:
  typedef ndnSIM::trie_with_policy<NameComponents,
                                   ndnSIM::smart_pointer_payload_traits< EntryImpl< PitImpl< Policy > > >,
                                   // ndnSIM::persistent_policy_traits
                                   Policy
                                   > super;
  typedef EntryImpl< PitImpl< Policy > > entry;

  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief PIT constructor
   */
  PitImpl ();

  /**
   * \brief Destructor
   */
  virtual ~PitImpl ();

  // inherited from Pit  
  virtual Ptr<Entry>
  Lookup (const ContentObjectHeader &header);

  virtual Ptr<Entry>
  Lookup (const InterestHeader &header);

  virtual Ptr<Entry>
  Create (Ptr<const InterestHeader> header);
  
  virtual void
  MarkErased (Ptr<Entry> entry);

  virtual void
  Print (std::ostream &os) const;

  virtual uint32_t
  GetSize () const;

  virtual Ptr<Entry>
  Begin ();

  virtual Ptr<Entry>
  End ();

  virtual Ptr<Entry>
  Next (Ptr<Entry>);
  
protected:
  void RescheduleCleaning ();
  void CleanExpired ();
  
  // inherited from Object class                                                                                                                                                        
  virtual void NotifyNewAggregate (); ///< @brief Even when object is aggregated to another Object
  virtual void DoDispose (); ///< @brief Do cleanup

private:
  uint32_t
  GetMaxSize () const;

  void
  SetMaxSize (uint32_t maxSize);
  
private:
  EventId m_cleanEvent;
  Ptr<Fib> m_fib; ///< \brief Link to FIB table
  Ptr<ForwardingStrategy> m_forwardingStrategy;

  // indexes
  typedef
  boost::intrusive::multiset<entry,
                        boost::intrusive::compare < TimestampIndex< entry > >,
                        boost::intrusive::member_hook< entry,
                                                       boost::intrusive::set_member_hook<>,
                                                       &entry::time_hook_>
                        > time_index;
  time_index i_time; 
                        
  friend class EntryImpl< PitImpl >;
};

} // namespace pit
} // namespace ndn
} // namespace ns3

#endif	/* NDN_PIT_IMPL_H */
