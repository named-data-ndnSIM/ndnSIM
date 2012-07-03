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

#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "../utils/trie-with-policy.h"
#include "../utils/lru-policy.h"
#include "../utils/random-policy.h"
#include "../utils/fifo-policy.h"
#include <boost/intrusive/parent_from_member.hpp>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Trie");

class Integer : public ns3::SimpleRefCount<Integer>
{
public:
  Integer (int value) : value_ (value) {}

  operator int () const { return value_; }
private:
  int value_;
};

std::ostream &
operator << (std::ostream &os, const Integer &i)
{
  os << (int)i;
  return os;
}

template<class Policy1, class Policy2>
struct multi_policy_hook
{
  Policy1 hook1_;
  Policy2 hook2_;
};

template<class Base, class Container1, class Container2>
struct multi_policy_container
{
  typedef Container1 nth_container1;
  typedef Container2 nth_container2;
    
  multi_policy_container (Base &base)
    : container1_ (base)
    , container2_ (base)
  {
  }
  
  nth_container1 container1_;
  nth_container2 container2_;
};

template<class BaseHook, class ValueType, class HookType>
struct Functor1
{
  typedef HookType hook_type;
  typedef hook_type*            hook_ptr;
  typedef const hook_type*      const_hook_ptr;
  
  typedef ValueType             value_type;
  typedef value_type*           pointer;
  typedef const value_type*     const_pointer;
  
  //Required static functions
  static hook_ptr to_hook_ptr (value_type &value)
  {  return &value.policy_hook_.hook1_; }
  
  static const_hook_ptr to_hook_ptr(const value_type &value)
  {  return &value.policy_hook_.hook1_; }
  
  static pointer to_value_ptr(hook_ptr n)
  {
    return bi::get_parent_from_member<value_type>
      (bi::get_parent_from_member<BaseHook >
       (n, &BaseHook::hook1_)
       , &value_type::policy_hook_
       );
  }
  static const_pointer to_value_ptr(const_hook_ptr n)
  {
    return bi::get_parent_from_member<value_type>
      (bi::get_parent_from_member<BaseHook>
       (n, &BaseHook::hook1_)
       , &value_type::policy_hook_
       );
  }
};

template<class BaseHook, class ValueType, class HookType>
struct Functor2
{
  typedef HookType hook_type;
  typedef hook_type*            hook_ptr;
  typedef const hook_type*      const_hook_ptr;
  
  typedef ValueType             value_type;
  typedef value_type*           pointer;
  typedef const value_type*     const_pointer;
  
  //Required static functions
  static hook_ptr to_hook_ptr (value_type &value)
  {  return &value.policy_hook_.hook2_; }
  
  static const_hook_ptr to_hook_ptr(const value_type &value)
  {  return &value.policy_hook_.hook2_; }
  
  static pointer to_value_ptr(hook_ptr n)
  {
    return bi::get_parent_from_member<value_type>
      (bi::get_parent_from_member<BaseHook >
       (n, &BaseHook::hook2_)
       , &value_type::policy_hook_
       );
  }
  static const_pointer to_value_ptr(const_hook_ptr n)
  {
    return bi::get_parent_from_member<value_type>
      (bi::get_parent_from_member<BaseHook>
       (n, &BaseHook::hook2_)
       , &value_type::policy_hook_
       );
  }
};


struct multi_policy_traits
{
  typedef multi_policy_hook< lru_policy_traits::policy_hook_type
                             ,
                             random_policy_traits::policy_hook_type
                             > policy_hook_type;

  template<class Container>
  struct container_hook
  {
    struct type
    {
      typedef bi::function_hook< Functor1 <policy_hook_type,
                                           Container,
                                           typename lru_policy_traits::policy_hook_type> > hook1;

      struct hook2
      {
        typedef bi::function_hook< Functor2 <policy_hook_type,
                                             Container,
                                             typename random_policy_traits::policy_hook_type> > hook_type;
        
        static uint32_t& get_order (typename Container::iterator item)
        {
          return item->policy_hook_.hook2_.randomOrder;
        }
        
        static const uint32_t& get_order (typename Container::const_iterator item)
        {
          return item->policy_hook_.hook2_.randomOrder;
        }
      };
      
    };
  };

  template<class Base,
           class Container,
           class Hook>
  struct policy 
  {
    typedef multi_policy_container <
      Base, 
      typename lru_policy_traits::policy< Base, Container, typename Hook::hook1 >::type
      ,
      typename random_policy_traits::policy< Base, Container, typename Hook::hook2 >::type
      > policy_container;
    
    class type : public policy_container
    {
    public:
      typedef Container parent_trie;

      type (Base &base)
        : policy_container (base)
      {
      }

      inline void
      update (typename parent_trie::iterator item)
      {
        this->container1_.update (item);
        this->container2_.update (item);
      }
  
      inline bool
      insert (typename parent_trie::iterator item)
      {
        bool ok;
        ok = this->container1_.insert (item);
        if (!ok)
          {
            // nothing to undo. just return false
            return false;
          }
        
        ok = this->container2_.insert (item);
        if (!ok)
          {
            // undo and return false
            this->container1_.erase (item);
            return false;
          }
        return true;
      }
  
      inline void
      lookup (typename parent_trie::iterator item)
      {
        this->container1_.lookup (item);
        this->container2_.lookup (item);
      }
  
      inline void
      erase (typename parent_trie::iterator item)
      {
        this->container1_.erase (item);
        this->container2_.erase (item);
      }

      inline typename policy_container::nth_container1 &
      get1 ()
      {
        return this->container1_;
      }

      inline typename policy_container::nth_container2 &
      get2 ()
      {
        return this->container2_;
      }
    };
  };
};

int
main (int argc, char *argv[])
{
  CommandLine args;
  args.Parse (argc, argv);
  
  typedef trie_with_policy<
    ns3::CcnxNameComponents,
    smart_pointer_payload_traits<Integer>,
    multi_policy_traits > trie;
  
  trie x;
  x.getPolicy ().get1 ().set_max_size (10);
  x.getPolicy ().get2 ().set_max_size (3);
  
  // x.getTrie ().PrintStat (std::cout);
  
  ns3::CcnxNameComponents n1,n2,n3,n4;
  // n1("a")("b")("c");
  // n2("a")("b")("d");
  // n3("a")("b")("f");
  // n4("a")("b");

  n1("a");
  n2("b");
  n3("c");
  n4("d");

  x.insert (n1, ns3::Create<Integer> (1));
  x.insert (n2, ns3::Create<Integer> (2));
  x.longest_prefix_match (n1);
  x.insert (n3, ns3::Create<Integer> (3));
  x.insert (n4, ns3::Create<Integer> (4));
  // x.insert (n4, ns3::Create<Integer> (4));

  std::cout << "digraph trie {\n";
  std::cout << x.getTrie ();
  std::cout << "}\n";

  // BOOST_FOREACH (const trie::parent_trie &item, x.getPolicy ())
  //   {
  //     std::cout << *item.payload () << " " << std::endl;
  //   }

  // ns3::CcnxNameComponents n4;
  // n4("a")("c");
    
  // // std::cout << *x->find (n4).get<0> ();

  // x->prune ();
  // // x->find (n5).get<0> ()->erase ();
  // x->find (n1).get<0> ()->erase ();
    
  // std::cout << "digraph trie {\n";
  // std::cout << *x;
  // std::cout << "}\n";

  // x->PrintStat (std::cout);

  // delete x;
  
  return 0;
}

