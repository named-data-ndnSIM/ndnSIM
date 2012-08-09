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

namespace ns3
{

template<class Container>
class NdnContentStoreImpl : public NdnContentStore,
                             protected Container
{
public:
  // from NdnContentStore
  
  virtual inline boost::tuple<Ptr<Packet>, Ptr<const NdnContentObjectHeader>, Ptr<const Packet> >
  Lookup (Ptr<const NdnInterestHeader> interest);
            
  virtual inline bool
  Add (Ptr<const NdnContentObjectHeader> header, Ptr<const Packet> packet);

  // virtual bool
  // Remove (Ptr<NdnInterestHeader> header);
  
  virtual inline void
  Print (std::ostream &os) const;  
};


template<class Container>
boost::tuple<Ptr<Packet>, Ptr<const NdnContentObjectHeader>, Ptr<const Packet> >
NdnContentStoreImpl<Container>::Lookup (Ptr<const NdnInterestHeader> interest)
{
  // NS_LOG_FUNCTION (this << interest->GetName ());

  /// @todo Change to search with predicate
  typename Container::const_iterator node = this->deepest_prefix_match (interest->GetName ());
  
  if (node != this->end ())
    {
      this->m_cacheHitsTrace (interest, node->payload ()->GetHeader ());

      // NS_LOG_DEBUG ("cache hit with " << node->payload ()->GetHeader ()->GetName ());
      return boost::make_tuple (node->payload ()->GetFullyFormedNdnPacket (),
                                node->payload ()->GetHeader (),
                                node->payload ()->GetPacket ());
    }
  else
    {
      // NS_LOG_DEBUG ("cache miss for " << interest->GetName ());
      this->m_cacheMissesTrace (interest);
      return boost::tuple<Ptr<Packet>, Ptr<NdnContentObjectHeader>, Ptr<Packet> > (0, 0, 0);
    }
}   
    
template<class Container>
bool 
NdnContentStoreImpl<Container>::Add (Ptr<const NdnContentObjectHeader> header, Ptr<const Packet> packet)
{
  // NS_LOG_FUNCTION (this << header->GetName ());

  return
    this->insert (header->GetName (), Create<NdnContentStoreEntry> (header, packet))
    .second;
}
    
template<class Container>
void 
NdnContentStoreImpl<Container>::Print (std::ostream &os) const
{
  for (typename Container::policy_container::const_iterator item = this->getPolicy ().begin ();
       item != this->getPolicy ().end ();
       item++)
  // BOOST_FOREACH (const typename Container::parent_trie &item, this->getPolicy ())
    {
      os << item->payload ()->GetName () << std::endl;
    }
}


} // namespace ns3

#endif // NDN_CONTENT_STORE_IMPL_H_
