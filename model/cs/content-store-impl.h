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
 */

#ifndef NDN_CONTENT_STORE_IMPL_H_
#define NDN_CONTENT_STORE_IMPL_H_

#include "ndn-content-store.h"
#include "ns3/packet.h"
#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"
#include <boost/foreach.hpp>

#include "../../utils/trie-with-policy.h"

namespace ns3 {
namespace ndn {
namespace cs {

template<class CS>
class EntryImpl : public Entry
{
public:
  EntryImpl (Ptr<const ContentObjectHeader> header, Ptr<const Packet> packet)
    : Entry (header, packet)
    , item_ (0)
  {
  }

  void
  SetTrie (typename CS::super::iterator item)
  {
    item_ = item;
  }

  typename CS::super::iterator to_iterator () { return item_; }
  typename CS::super::const_iterator to_iterator () const { return item_; }
  
private:
  typename CS::super::iterator item_;
};



template<class Policy>
class ContentStoreImpl : public ContentStore,
                         protected ndnSIM::trie_with_policy< NameComponents,
                                                             ndnSIM::smart_pointer_payload_traits< EntryImpl< ContentStoreImpl< Policy > > >,
                                                             Policy >
{
public:
  typedef ndnSIM::trie_with_policy< NameComponents,
                                    ndnSIM::smart_pointer_payload_traits< EntryImpl< ContentStoreImpl< Policy > > >,
                                    Policy > super;
  
  typedef EntryImpl< ContentStoreImpl< Policy > > entry;
  
  static TypeId
  GetTypeId ();
  
  ContentStoreImpl () { };
  virtual ~ContentStoreImpl () { };
  
  // from ContentStore
  
  virtual inline boost::tuple<Ptr<Packet>, Ptr<const ContentObjectHeader>, Ptr<const Packet> >
  Lookup (Ptr<const InterestHeader> interest);
            
  virtual inline bool
  Add (Ptr<const ContentObjectHeader> header, Ptr<const Packet> packet);

  // virtual bool
  // Remove (Ptr<InterestHeader> header);
  
  virtual inline void
  Print (std::ostream &os) const;  

  virtual uint32_t
  GetSize () const;

  virtual Ptr<Entry>
  Begin ();

  virtual Ptr<Entry>
  End ();

  virtual Ptr<Entry>
  Next (Ptr<Entry>);

private:
  void
  SetMaxSize (uint32_t maxSize);

  uint32_t
  GetMaxSize () const;
};

} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_IMPL_H_
