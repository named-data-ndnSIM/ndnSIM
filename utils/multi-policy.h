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

#ifndef MULTI_POLICY_H_
#define MULTI_POLICY_H_

#include "detail/multi-type-container.h"
#include "detail/multi-policy-container.h"
#include "detail/functor-hook.h"

#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/intrusive/options.hpp>

namespace ns3 {
namespace ndn {
namespace ndnSIM {

template<typename Policies> // e.g., mpl::vector1< lru_policy_traits >
struct multi_policy_traits
{
  typedef Policies policy_traits;

  struct getHook { template<class Item> struct apply { typedef typename Item::policy_hook_type type; }; };
  typedef detail::multi_type_container< typename boost::mpl::transform1<policy_traits, getHook>::type > policy_hook_type;
  
  template<class Container>
  struct container_hook
  {
    typedef policy_hook_type type;
  };

  template<class Base,
           class Container,
           class Hook>
  struct policy 
  {
    typedef boost::mpl::range_c<int, 0, boost::mpl::size<policy_traits>::type::value> policies_range;

    struct getPolicy
    {
      template<class Number>
      struct apply
      {
        typedef
        typename boost::mpl::at_c<policy_traits, Number::value>::type::
        template policy<Base,
                        Container,
                        boost::intrusive::function_hook< detail::FunctorHook <Hook,
                                                                              Container,
                                                                              Number::value> > >::type
        type;
      };
    };
    
    typedef
    typename boost::mpl::transform1<policies_range,
                                    getPolicy,
                                    boost::mpl::back_inserter< boost::mpl::vector0<> > >::type policies;
                             
    
    typedef detail::multi_policy_container< Base, policies > policy_container;
    
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
        policy_container::update (item);
      }
  
      inline bool
      insert (typename parent_trie::iterator item)
      {
        return policy_container::insert (item);
      }
  
      inline void
      lookup (typename parent_trie::iterator item)
      {
        policy_container::lookup (item);
      }
  
      inline void
      erase (typename parent_trie::iterator item)
      {
        policy_container::erase (item);
      }
      
      inline void
      clear ()
      {
        policy_container::clear ();
      }
    };
  };
};

} // ndnSIM
} // ndn
} // ns3

#endif // MULTI_POLICY_H_
