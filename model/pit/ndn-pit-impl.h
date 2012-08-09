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

#include "../../utils/trie-with-policy.h"

#include "ndn-pit-entry-impl.h"

#include "ns3/ndn-name-components.h"

namespace ns3 {

/**
 * \ingroup ndn
 * \brief Class implementing Pending Interests Table
 */
template<class Policy>
class NdnPitImpl : public NdnPit
                  , protected ndnSIM::trie_with_policy<NdnNameComponents,
                                                       ndnSIM::smart_pointer_payload_traits<NdnPitEntryImpl< NdnPitImpl< Policy > > >,
                                                       // ndnSIM::persistent_policy_traits
                                                       Policy
                                                       >
{
public:
  typedef ndnSIM::trie_with_policy<NdnNameComponents,
                                   ndnSIM::smart_pointer_payload_traits<NdnPitEntryImpl< NdnPitImpl< Policy > > >,
                                   // ndnSIM::persistent_policy_traits
                                   Policy
                                   > super;
  typedef NdnPitEntryImpl< NdnPitImpl< Policy > > entry;

  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief PIT constructor
   */
  NdnPitImpl ();

  /**
   * \brief Destructor
   */
  virtual ~NdnPitImpl ();

  // inherited from NdnPit  
  virtual Ptr<NdnPitEntry>
  Lookup (const NdnContentObjectHeader &header);

  virtual Ptr<NdnPitEntry>
  Lookup (const NdnInterestHeader &header);

  virtual Ptr<NdnPitEntry>
  Create (Ptr<const NdnInterestHeader> header);
  
  virtual void
  MarkErased (Ptr<NdnPitEntry> entry);

  virtual void
  Print (std::ostream &os) const;

  virtual uint32_t
  GetSize () const;

  virtual Ptr<NdnPitEntry>
  Begin ();

  virtual Ptr<NdnPitEntry>
  End ();

  virtual Ptr<NdnPitEntry>
  Next (Ptr<NdnPitEntry>);
  
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
  Ptr<NdnFib> m_fib; ///< \brief Link to FIB table
  Ptr<NdnForwardingStrategy> m_forwardingStrategy;

  // indexes
  typedef
  boost::intrusive::multiset<entry,
                        boost::intrusive::compare < TimestampIndex< entry > >,
                        boost::intrusive::member_hook< entry,
                                                       boost::intrusive::set_member_hook<>,
                                                       &entry::time_hook_>
                        > time_index;
  time_index i_time; 
                        
  friend class NdnPitEntryImpl< NdnPitImpl >;
};

} // namespace ns3

#endif	/* NDN_PIT_IMPL_H */
