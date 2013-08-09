/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 *
 * GNU v3.0 license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_NDNCXX_DETAIL_TIMEOUTS_POLICY_H_
#define NDN_NDNCXX_DETAIL_TIMEOUTS_POLICY_H_

#include <boost/intrusive/options.hpp>
#include <boost/intrusive/list.hpp>

#include <ns3/nstime.h>
#include <ns3/simulator.h>

namespace ns3 {
namespace ndn {
namespace detail {

/**
 * @brief Traits for timeouts policy
 */
struct timeouts_policy_traits
{
  /// @brief Name that can be used to identify the policy (for NS-3 object model and logging)
  static std::string GetName () { return "Timeouts"; }

  struct policy_hook_type : public boost::intrusive::set_member_hook<> { Time timeWhenShouldExpire; };

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
    static Time& get_timeout (typename Container::iterator item)
    {
      return static_cast<typename policy_container::value_traits::hook_type*>
        (policy_container::value_traits::to_node_ptr(*item))->timeWhenShouldExpire;
    }

    static const Time& get_timeout (typename Container::const_iterator item)
    {
      return static_cast<const typename policy_container::value_traits::hook_type*>
        (policy_container::value_traits::to_node_ptr(*item))->timeWhenShouldExpire;
    }

    template<class Key>
    struct MemberHookLess
    {
      bool operator () (const Key &a, const Key &b) const
      {
        return get_timeout (&a) < get_timeout (&b);
      }
    };

    typedef boost::intrusive::multiset< Container,
                                        boost::intrusive::compare< MemberHookLess< Container > >,
                                        Hook > policy_container;


    class type : public policy_container
    {
    public:
      typedef policy policy_base; // to get access to get_timeout methods from outside
      typedef Container parent_trie;

      type (Base &base)
        : m_base (base)
      {
      }

      inline void
      update (typename parent_trie::iterator item)
      {
        // do nothing
      }

      inline bool
      insert (typename parent_trie::iterator item)
      {
        Time timeout = item->payload ()->GetInterest ()->GetInterestLifetime ();
        if (timeout.IsZero ()) timeout = Seconds (4.0);
        
        get_timeout (item) = Simulator::Now () + timeout;
        policy_container::insert (*item);

        if (policy_container::s_iterator_to (*item) == policy_container::begin ())
          {
            if (m_timeoutEvent.IsRunning ())
              {
                Simulator::Remove (m_timeoutEvent); // just canceling would not clean up list of events
              }

            m_timeoutEvent = Simulator::Schedule (timeout, &type::ProcessTimeoutEntry, this, item);
          }
        
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
        bool needRescheduling = false;
        if (policy_container::s_iterator_to (*item) == policy_container::begin ())
          {
            if (m_timeoutEvent.IsRunning ())
              {
                Simulator::Remove (m_timeoutEvent); // just canceling would not clean up list of events
              }
            needRescheduling = true;
          }

        // erase only if freshness is non zero (otherwise an item is not in the policy
        policy_container::erase (policy_container::s_iterator_to (*item));

        if (needRescheduling && !policy_container::empty ())
          {
            Time timeout = get_timeout (&*policy_container::begin ()) - Simulator::Now ();
            m_timeoutEvent = Simulator::Schedule (timeout, &type::ProcessTimeoutEntry, this, &*policy_container::begin ());
          }
      }

      inline void
      clear ()
      {
        policy_container::clear ();
      }

      inline void
      ProcessTimeoutEntry (typename parent_trie::iterator item)
      {
        item->payload ()->ProcessOnTimeout (item->payload ()->GetInterest ());

        m_base.erase (item);
      }

    private:
      type () : m_base (*((Base*)0)) { };

    private:
      Base &m_base;
      EventId m_timeoutEvent;
    };
  };
};

} // detail
} // ndn
} // ns3

#endif // NDN_NDNCXX_DETAIL_TIMEOUTS_STATS_POLICY_H
