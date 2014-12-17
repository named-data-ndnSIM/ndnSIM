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

#ifndef PAYLOAD_POLICY_H_
#define PAYLOAD_POLICY_H_

/// @cond include_hidden

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/**
 * @brief Traits for policy that keeps items in a sorted order using payload member
 */
template<class Member>
struct payload_policy_traits {
  struct policy_hook_type : public boost::intrusive::set_member_hook<> {
  };

  template<class Container>
  struct container_hook {
    typedef boost::intrusive::member_hook<Container, policy_hook_type, &Container::policy_hook_>
      type;
  };

  template<class Base, class Container, class Hook>
  struct policy {
    typedef typename boost::intrusive::list<Container, Hook> policy_container;

    // could be just typedef
    class type : public policy_container {
    public:
      typedef Container parent_trie;

      type(Base& base)
        : base_(base)
        , max_size_(100)
      {
      }

      inline void
      update(typename parent_trie::iterator item)
      {
        // do relocation
        policy_container::splice(policy_container::end(), *this,
                                 policy_container::s_iterator_to(*item));
      }

      inline bool
      insert(typename parent_trie::iterator item)
      {
        if (policy_container::size() >= max_size_) {
          base_.erase(&(*policy_container::begin()));
        }

        policy_container::push_back(*item);
        return true;
      }

      inline void
      lookup(typename parent_trie::iterator item)
      {
        // do relocation
        policy_container::splice(policy_container::end(), *this,
                                 policy_container::s_iterator_to(*item));
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
      size_t max_size_;
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

/// @endcond

#endif // PAYLOAD_POLICY_H
