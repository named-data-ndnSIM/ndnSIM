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

#ifndef RANDOM_POLICY_H_
#define RANDOM_POLICY_H_

#include "ns3/random-variable.h"

struct bla : public bi::set_member_hook<>
{
  uint32_t randomOrder;
};

template<class Key>
struct MemberHookLess
{
  bool operator () (const Key &a, const Key &b) const
  {
    return a.policy_hook_.randomOrder < b.policy_hook_.randomOrder;
  }
};

template<typename FullKey,
	 typename Payload, typename PayloadTraits
         >
struct random_policy_traits
{
  typedef bla policy_hook_type;
  typedef trie< FullKey, Payload, PayloadTraits, policy_hook_type > parent_trie;
  typedef typename bi::set< parent_trie,
                            bi::compare< MemberHookLess< parent_trie > >,
                            bi::member_hook< parent_trie,
                                             policy_hook_type,
                                             &parent_trie::policy_hook_ > > policy_container;

  class policy : public policy_container
  {
  public:
    policy ()
      : u_rand (0, std::numeric_limits<uint32_t>::max ())
      , max_size_ (100)
    {
    }

    inline void
    update (typename parent_trie::iterator item)
    {
      // do nothing. it's random policy
    }
  
    inline void
    insert (typename parent_trie::iterator item)
    {
      item->policy_hook_.randomOrder = u_rand.GetValue ();
      if (policy_container::size () >= max_size_)
        {
          typename parent_trie::iterator oldItem = &(*policy_container::begin ());
          policy_container::erase (policy_container::begin ());
          oldItem->erase ();
        }

      policy_container::insert (*item);
    }
  
    inline void
    lookup (typename parent_trie::iterator item)
    {
      // do nothing. it's random policy
    }
  
    inline void
    erase (typename parent_trie::iterator item)
    {
      policy_container::erase (policy_container::s_iterator_to (*item));
    }

    inline void
    set_max_size (size_t max_size)
    {
      max_size_ = max_size;
    }

    inline size_t
    get_max_size () const
    {
      return max_size_;
    }

  private:
    ns3::UniformVariable u_rand;
    size_t max_size_;
  };
};

#endif // RANDOM_POLICY_H
