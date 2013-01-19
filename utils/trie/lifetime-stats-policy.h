/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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

#ifndef LIFETIME_STATS_POLICY_H_
#define LIFETIME_STATS_POLICY_H_

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>

#include <ns3/nstime.h>
#include <ns3/simulator.h>
#include <ns3/traced-callback.h>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

/**
 * @brief Traits for lifetime stats policy
 */
struct lifetime_stats_policy_traits
{
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string GetName () { return "LifetimeStats"; }

  struct policy_hook_type : public boost::intrusive::list_member_hook<> { Time timeWhenAdded; };

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
    typedef typename boost::intrusive::list< Container, Hook > policy_container;
    
    static Time& get_time (typename Container::iterator item)
    {
      return static_cast<typename policy_container::value_traits::hook_type*>
        (policy_container::value_traits::to_node_ptr(*item))->timeWhenAdded;
    }
      
    static const Time& get_time (typename Container::const_iterator item)
    {
      return static_cast<const typename policy_container::value_traits::hook_type*>
        (policy_container::value_traits::to_node_ptr(*item))->timeWhenAdded;
    }

    class type : public policy_container
    {
    public:
      typedef policy policy_base; // to get access to get_time methods from outside
      typedef Container parent_trie;

      type (Base &base)
        : base_ (base)
        , max_size_ (100)
        , m_willRemoveEntry (0)
      {
      }

      inline void
      update (typename parent_trie::iterator item)
      {
        // do nothing. it's random policy
      }
  
      inline bool
      insert (typename parent_trie::iterator item)
      {
        get_time (item) = Simulator::Now ();

        policy_container::push_back (*item);
        return true;
      }
  
      inline void
      lookup (typename parent_trie::iterator item)
      {
        // do nothing. it's random policy
      }
  
      inline void
      erase (typename parent_trie::iterator item)
      {
        Time lifetime = Simulator::Now () - get_time (item);

        if (m_willRemoveEntry != 0)
          {
            (*m_willRemoveEntry) (item->payload (), lifetime);
          }
        
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

      void
      set_traced_callback (TracedCallback< typename parent_trie::payload_traits::const_base_type, Time > *callback)
      {
        m_willRemoveEntry = callback;
      }
      
    private:
      type () : base_(*((Base*)0)) { };
      
    private:
      Base &base_;
      size_t max_size_;

      TracedCallback< typename parent_trie::payload_traits::const_base_type, Time > *m_willRemoveEntry;
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

#endif // LIFETIME_STATS_POLICY_H
