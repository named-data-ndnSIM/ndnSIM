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

#ifndef MULTI_POLICY_H_
#define MULTI_POLICY_H_

/// @cond include_hidden

#include "detail/multi-type-container.hpp"
#include "detail/multi-policy-container.hpp"
#include "detail/functor-hook.hpp"

#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/intrusive/options.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

template<typename Policies> // e.g., mpl::vector1< lru_policy_traits >
struct multi_policy_traits {
  typedef Policies policy_traits;

  struct getHook {
    template<class Item>
    struct apply {
      typedef typename Item::policy_hook_type type;
    };
  };
  typedef detail::multi_type_container<
    typename boost::mpl::transform1<policy_traits, getHook>::type> policy_hook_type;

  template<class Container>
  struct container_hook {
    typedef policy_hook_type type;
  };

  template<class Base, class Container, class Hook>
  struct policy {
    typedef boost::mpl::range_c<int, 0, boost::mpl::size<policy_traits>::type::value>
      policies_range;

    struct getPolicy {
      template<class Number>
      struct apply {
        typedef typename boost::mpl::at_c<policy_traits, Number::value>::type::
          template policy<Base, Container,
                          boost::intrusive::function_hook<detail::FunctorHook<Hook, Container,
                                                                              Number::value>>>::type
            type;
      };
    };

    typedef typename boost::mpl::transform1<policies_range, getPolicy,
                                            boost::mpl::back_inserter<boost::mpl::vector0<>>>::type
      policies;

    typedef detail::multi_policy_container<Base, policies> policy_container;

    class type : public policy_container {
    public:
      typedef policy policy_base; // to get access to get_time methods from outside
      typedef Container parent_trie;

      type(Base& base)
        : policy_container(base)
      {
      }

      inline void
      update(typename parent_trie::iterator item)
      {
        policy_container::update(item);
      }

      inline bool
      insert(typename parent_trie::iterator item)
      {
        return policy_container::insert(item);
      }

      inline void
      lookup(typename parent_trie::iterator item)
      {
        policy_container::lookup(item);
      }

      inline void
      erase(typename parent_trie::iterator item)
      {
        policy_container::erase(item);
      }

      inline void
      clear()
      {
        policy_container::clear();
      }

      struct max_size_setter {
        max_size_setter(policy_container& container, size_t size)
          : m_container(container)
          , m_size(size)
        {
        }

        template<typename U>
        void
        operator()(U index)
        {
          m_container.template get<U::value>().set_max_size(m_size);
        }

      private:
        policy_container& m_container;
        size_t m_size;
      };

      inline void
      set_max_size(size_t max_size)
      {
        boost::mpl::for_each<boost::mpl::range_c<int, 0,
                                                 boost::mpl::size<policy_traits>::type::value>>(
          max_size_setter(*this, max_size));
      }

      inline size_t
      get_max_size() const
      {
        // as max size should be the same everywhere, get the value from the first available policy
        return policy_container::template get<0>().get_max_size();
      }
    };
  };

  struct name_getter {
    name_getter(std::string& name)
      : m_name(name)
    {
    }

    template<typename U>
    void
    operator()(U index)
    {
      if (!m_name.empty())
        m_name += "::";
      m_name += boost::mpl::at_c<policy_traits, U::value>::type::GetName();
    }

    std::string& m_name;
  };

  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string
  GetName()
  {
    // combine names of all internal policies
    std::string name;
    boost::mpl::for_each<boost::mpl::range_c<int, 0, boost::mpl::size<policy_traits>::type::value>>(
      name_getter(name));

    return name;
  }
};

} // ndnSIM
} // ndn
} // ns3

/// @endcond

#endif // MULTI_POLICY_H_
