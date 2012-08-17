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

#ifndef PAYLOAD_WITH_INDEX_H_
#define PAYLOAD_WITH_INDEX_H_

namespace ns3 {
namespace ndn {
namespace ndnSIM {

template<typename PayloadTraits,
         typename IndexTraits>
class payload_with_index
{
public:
  typedef PayloadTraits::pointer_type iterator;
  
  typedef typename IndexTraits::template index<
    PayloadTraits,
    typename IndexTraits::template container_hook<parent_trie>::type >::type index_container;

  inline
  payload_with_index ()
    : index_ (*this)
  {
  }

  inline std::pair< iterator, bool >
  insert (typename iterator payload)
  {
    bool ok = policy_.insert (s_iterator_to (item.first));
    if (!ok)
      {
        item.first->erase (); // cannot insert
        return std::make_pair (end (), false);
      }

    return item;
  }

  // inline void
  // erase (const FullKey &key)
  // {
  //   iterator foundItem, lastItem;
  //   bool reachLast;
  //   boost::tie (foundItem, reachLast, lastItem) = trie_.find (key);
    
  //   if (!reachLast || lastItem->payload () == PayloadTraits::empty_payload)
  //     return; // nothing to invalidate

  //   erase (lastItem);
  // }

  // inline void
  // erase (iterator node)
  // {
  //   if (node == end ()) return;

  //   policy_.erase (s_iterator_to (node));
  //   node->erase (); // will do cleanup here
  // }

  // inline void
  // clear ()
  // {
  //   policy_.clear ();
  //   trie_.clear ();
  // }

  // template<typename Modifier>
  // bool
  // modify (iterator position, Modifier mod)
  // {
  //   if (position == end ()) return false;
  //   if (position->payload () == PayloadTraits::empty_payload) return false;

  //   mod (*position->payload ());
  //   policy_.update (position);
  //   return true;
  // }

private:
  mutable index_container policy_;
};

} // ndnSIM
} // ndn
} // ns3

#endif // PAYLOAD_WITH_POLICY_H_
