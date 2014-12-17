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

#ifndef AGGREGATE_STATS_POLICY_H_
#define AGGREGATE_STATS_POLICY_H_

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/// @cond include_hidden

/**
 * @brief Traits for policy that just keeps track of number of elements
 * It's doing a rather expensive job, but just in case it needs to be extended later
 */
struct aggregate_stats_policy_traits {
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string
  GetName()
  {
    return "AggregateStats";
  }
  struct policy_hook_type {
  };

  template<class Container>
  struct container_hook {
    struct type {
    };
  };

  template<class Base, class Container, class Hook>
  struct policy {
    // typedef typename boost::intrusive::list< Container, Hook > policy_container;

    // could be just typedef
    class type {
    public:
      typedef Container parent_trie;

      type(Base& base)
        : base_(base)
        , m_updates(0)
        , m_inserts(0)
        , m_lookups(0)
        , m_erases(0)
      {
      }

      inline void
      update(typename parent_trie::iterator item)
      {
        m_updates++;
        // do nothing
      }

      inline bool
      insert(typename parent_trie::iterator item)
      {
        m_inserts++;
        return true;
      }

      inline void
      lookup(typename parent_trie::iterator item)
      {
        m_lookups++;
      }

      inline void
      erase(typename parent_trie::iterator item)
      {
        m_erases++;
      }

      inline void set_max_size(uint32_t)
      {
      }

      inline uint32_t
      get_max_size() const
      {
        return 0;
      }

      inline void
      clear()
      {
        // is called only at the end of simulation
      }

      inline void
      ResetStats()
      {
        m_updates = 0;
        m_inserts = 0;
        m_lookups = 0;
        m_erases = 0;
      }

      inline uint64_t
      GetUpdates() const
      {
        return m_updates;
      }

      inline uint64_t
      GetInserts() const
      {
        return m_inserts;
      }

      inline uint64_t
      GetLookups() const
      {
        return m_lookups;
      }

      inline uint64_t
      GetErases() const
      {
        return m_erases;
      }

    private:
      type()
        : base_(*((Base*)0)){};

    private:
      Base& base_;

      uint64_t m_updates;
      uint64_t m_inserts;
      uint64_t m_lookups;
      uint64_t m_erases;
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

/// @endcond

#endif // AGGREGATE_STATS_POLICY_H_
