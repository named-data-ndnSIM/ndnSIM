/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef RANDOM_POLICY_H_
#define RANDOM_POLICY_H_

/// @cond include_hidden

#include "ns3/random-variable-stream.h"
#include "ns3/double.h"

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/set.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/**
 * @brief Traits for random replacement policy
 */
struct random_policy_traits {
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string
  GetName()
  {
    return "Random";
  }

  struct policy_hook_type : public boost::intrusive::set_member_hook<> {
    uint32_t randomOrder;
  };

  template<class Container>
  struct container_hook {
    typedef boost::intrusive::member_hook<Container, policy_hook_type, &Container::policy_hook_>
      type;
  };

  template<class Base, class Container, class Hook>
  struct policy {
    static uint32_t&
    get_order(typename Container::iterator item)
    {
      return static_cast<typename policy_container::value_traits::hook_type*>(
               policy_container::value_traits::to_node_ptr(*item))->randomOrder;
    }

    static const uint32_t&
    get_order(typename Container::const_iterator item)
    {
      return static_cast<const typename policy_container::value_traits::hook_type*>(
               policy_container::value_traits::to_node_ptr(*item))->randomOrder;
    }

    template<class Key>
    struct MemberHookLess {
      bool
      operator()(const Key& a, const Key& b) const
      {
        return get_order(&a) < get_order(&b);
      }
    };

    typedef boost::intrusive::multiset<Container,
                                       boost::intrusive::compare<MemberHookLess<Container>>,
                                       Hook> policy_container;

    // could be just typedef
    class type : public policy_container {
    public:
      typedef policy policy_base; // to get access to get_order methods from outside
      typedef Container parent_trie;

      type(Base& base)
        : base_(base)
        , u_rand(CreateObject<UniformRandomVariable>())
        , max_size_(100)
      {
        u_rand->SetAttribute("Min", DoubleValue(0));
        u_rand->SetAttribute("Max", DoubleValue(std::numeric_limits<uint32_t>::max()));
      }

      inline void
      update(typename parent_trie::iterator item)
      {
        // do nothing. it's random policy
      }

      inline bool
      insert(typename parent_trie::iterator item)
      {
        get_order(item) = u_rand->GetValue();

        if (max_size_ != 0 && policy_container::size() >= max_size_) {
          if (MemberHookLess<Container>()(*item, *policy_container::begin())) {
            // std::cout << "Cannot add. Signaling fail\n";
            // just return false. Indicating that insert "failed"
            return false;
          }
          else {
            // removing some random element
            base_.erase(&(*policy_container::begin()));
          }
        }

        policy_container::insert(*item);
        return true;
      }

      inline void
      lookup(typename parent_trie::iterator item)
      {
        // do nothing. it's random policy
      }

      inline void
      erase(typename parent_trie::iterator item)
      {
        policy_container::erase(policy_container::s_iterator_to(*item));
      }

      inline void
      clear()
      {
        policy_container::clear();
      }

      inline void
      set_max_size(size_t max_size)
      {
        max_size_ = max_size;
      }

      inline size_t
      get_max_size() const
      {
        return max_size_;
      }

    private:
      type()
        : base_(*((Base*)0)){};

    private:
      Base& base_;
      Ptr<UniformRandomVariable> u_rand;
      size_t max_size_;
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

/// @endcond

#endif // RANDOM_POLICY_H
