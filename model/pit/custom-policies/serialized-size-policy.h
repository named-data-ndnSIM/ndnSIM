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

#ifndef SERIALIZED_POLICY_H_
#define SERIALIZED_POLICY_H_

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/set.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/**
 * @brief Traits for Least Recently Used replacement policy
 */
struct serialized_size_policy_traits
{
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string GetName () { return "SerializedSize"; }

  struct policy_hook_type : public boost::intrusive::set_member_hook<> { uint32_t size; };

  template<class Container>
  struct container_hook
  {
    typedef boost::intrusive::member_hook< Container,
                                           policy_hook_type,
                                           &Container::policy_hook_ > type;
  };

  template<class Base,
           class Container,
           class Hook>
  struct policy
  {
    static uint32_t& get_size (typename Container::iterator item)
    {
      return static_cast<typename policy_container::value_traits::hook_type*>
        (policy_container::value_traits::to_node_ptr(*item))->size;
    }

    static const uint32_t& get_size (typename Container::const_iterator item)
    {
      return static_cast<const typename policy_container::value_traits::hook_type*>
        (policy_container::value_traits::to_node_ptr(*item))->size;
    }

    template<class Key>
    struct MemberHookLess
    {
      bool operator () (const Key &a, const Key &b) const
      {
        return get_size (&a) < get_size (&b);
      }
    };

    typedef typename boost::intrusive::multiset< Container,
                                                 boost::intrusive::compare< MemberHookLess< Container > >,
                                                 Hook > policy_container;

    // could be just typedef
    class type : public policy_container
    {
    public:
      typedef Container parent_trie;

      type (Base &base)
        : base_ (base)
        , max_size_ (10000) // size in bytes. Default ~10 kilobytes
        , current_space_used_ (0)
      {
      }

      inline void
      update (typename parent_trie::iterator item)
      {
        // in case size got changed
        current_space_used_ -= get_size (item);
        policy_container::erase (*item);

        if (item->payload ()->GetInterest ()->GetWire ())
          {
            policy::get_size (item) = item->payload ()->GetInterest ()->GetWire ()->GetSize ();
          }
        else
          {
            policy::get_size (item) = 0;
          }
        current_space_used_ += get_size (item); // this operation can violate policy constraint, so in some case
                                                // it may be necessary to remove some other element
        policy_container::insert (*item);
      }

      inline bool
      insert (typename parent_trie::iterator item)
      {
        uint32_t interestSize = 0;
        if (item->payload ()->GetInterest ()->GetWire ())
          {
            interestSize = item->payload ()->GetInterest ()->GetWire ()->GetSize ();
          }

        // can't use logging here
        NS_LOG_DEBUG ("Number of entries: " << policy_container::size ()
                      << ", space used: " << current_space_used_
                      << ", name: " << item->payload ()->GetPrefix ()
                      << ", interest size: " << interestSize);

        if (max_size_ != 0 && current_space_used_ + interestSize > max_size_)
          {
            NS_LOG_DEBUG ("Rejecting PIT entry");

            // the current version just fails to add an element, but it also possible
            // to remove the largest element (last element in multi_map policy container)
            return false;
          }

        policy::get_size (item) = interestSize;
        current_space_used_ += interestSize;

        policy_container::insert (*item);
        return true;
      }

      inline void
      lookup (typename parent_trie::iterator item)
      {
        // do nothing
      }

      inline void
      erase (typename parent_trie::iterator item)
      {
        NS_LOG_DEBUG ("Erasing entry with name: " << item->payload ()->GetPrefix ());

        current_space_used_ -= policy::get_size (item);
        policy_container::erase (policy_container::s_iterator_to (*item));
      }

      inline void
      clear ()
      {
        policy_container::clear ();
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

      inline uint32_t
      get_current_space_used () const
      {
        return current_space_used_;
      }

    private:
      type () : base_(*((Base*)0)) { };

    private:
      Base &base_;
      uint32_t max_size_;
      uint32_t current_space_used_;
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

#endif
