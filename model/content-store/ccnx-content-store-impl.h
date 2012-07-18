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

#ifndef CCNX_CONTENT_STORE_IMPL_H_
#define CCNX_CONTENT_STORE_IMPL_H_

#include "ccnx-content-store.h"
#include "ns3/packet.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include <boost/foreach.hpp>

namespace ns3
{

template<class Container>
class CcnxContentStoreImpl : public CcnxContentStore,
                             protected Container
{
public:
  // from CcnxContentStore
  
  virtual inline boost::tuple<Ptr<Packet>, Ptr<const CcnxContentObjectHeader>, Ptr<const Packet> >
  Lookup (Ptr<const CcnxInterestHeader> interest);
            
  virtual inline bool
  Add (Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> packet);

  // virtual bool
  // Remove (Ptr<CcnxInterestHeader> header);
  
  virtual inline void
  Print (std::ostream &os) const;  
};


template<class Container>
boost::tuple<Ptr<Packet>, Ptr<const CcnxContentObjectHeader>, Ptr<const Packet> >
CcnxContentStoreImpl<Container>::Lookup (Ptr<const CcnxInterestHeader> interest)
{
  // NS_LOG_FUNCTION (this << interest->GetName ());

  /// @todo Change to search with predicate
  typename Container::const_iterator node = this->deepest_prefix_match (interest->GetName ());
  
  if (node != this->end ())
    {
      this->m_cacheHitsTrace (interest, node->payload ()->GetHeader ());

      // NS_LOG_DEBUG ("cache hit with " << node->payload ()->GetHeader ()->GetName ());
      return boost::make_tuple (node->payload ()->GetFullyFormedCcnxPacket (),
                                node->payload ()->GetHeader (),
                                node->payload ()->GetPacket ());
    }
  else
    {
      // NS_LOG_DEBUG ("cache miss for " << interest->GetName ());
      this->m_cacheMissesTrace (interest);
      return boost::tuple<Ptr<Packet>, Ptr<CcnxContentObjectHeader>, Ptr<Packet> > (0, 0, 0);
    }
}   
    
template<class Container>
bool 
CcnxContentStoreImpl<Container>::Add (Ptr<const CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
  // NS_LOG_FUNCTION (this << header->GetName ());

  return
    this->insert (header->GetName (), Create<CcnxContentStoreEntry> (header, packet))
    .second;
}
    
template<class Container>
void 
CcnxContentStoreImpl<Container>::Print (std::ostream &os) const
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

#endif // CCNX_CONTENT_STORE_IMPL_H_
