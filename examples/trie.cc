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
#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/clear.hpp>
#include <boost/mpl/sequence_tag.hpp>

using namespace ns3;
namespace mpl = boost::mpl;

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

template <class T>
struct wrap
{
  T value_;
};

template< class Vector >
struct multi_type_container
  : public mpl::inherit_linearly< Vector, mpl::inherit<wrap<mpl::_2>, mpl::_1 >
  >::type
{
  template<int N>
  struct index
  {
    typedef typename mpl::at_c<Vector, N>::type type;
  };
  
  template<class T>
  T &
  get ()
  {
    return static_cast< wrap<T> &> (*this).value_;
  }

  template<class T>
  const T &
  get () const
  {
    return static_cast< const wrap<T> &> (*this).value_;
  }
  
  template<int N>
  typename mpl::at_c<Vector, N>::type &
  get ()
  {
    typedef typename mpl::at_c<Vector, N>::type T;
    return static_cast< wrap<T> &> (*this).value_;
  }

  template<int N>
  const typename mpl::at_c<Vector, N>::type &
  get () const
  {
    typedef typename mpl::at_c<Vector, N>::type T;
    return static_cast< const wrap<T> &> (*this).value_;
  }
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template< class Base, class Value >
struct policy_wrap
{
  typedef Value value_type;
  
  policy_wrap (Base &base)
  : value_ (base)
  { }

  Value value_;
};

template< class Base, class Super/*empy_wrap/previous level*/, class Value/*policy_wrap< element in vector >*/ >
struct inherit_with_base : Super, Value
{
  inherit_with_base (Base &base)
  : Super (base), Value (base)
  { }

  void
  update (typename Base::iterator item)
  {
    Value::value_.update (item);
    Super::update (item);
  }

  bool
  insert (typename Base::iterator item)
  {
    // BOOST_MPL_ASSERT ((boost::is_same<Item,typename Base::iterator>));
    bool ok = Value::value_.insert (item);
    if (!ok)
      return false;

    ok = Super::insert (item);
    if (!ok)
      {
        Value::value_.erase (item);
        return false;
      }
    return true;
  }

  void
  lookup (typename Base::iterator item)
  {
    Value::value_.lookup (item);
    Super::lookup (item);
  }

  void
  erase (typename Base::iterator item)
  {
    Value::value_.erase (item);
    Super::erase (item);
  }  
};

template< class Base >
struct empty_policy_wrap
{
  empty_policy_wrap (Base &base) { }

  void update (typename Base::iterator item) {}
  bool insert (typename Base::iterator item) { return true; }
  void lookup (typename Base::iterator item) {}
  void erase (typename Base::iterator item) {}
};

template< class Base, class Vector >
struct multi_policy_container
  : public mpl::fold< Vector,
                      empty_policy_wrap<Base>,
                      inherit_with_base<Base,
                                        mpl::_1/*empty/previous*/,
                                        policy_wrap<Base, mpl::_2>/*element in vector*/>
                      >::type
{
  typedef typename mpl::fold< Vector,
                              empty_policy_wrap<Base>,
                              inherit_with_base<Base,
                                                mpl::_1/*empty/previous*/,
                                                policy_wrap<Base, mpl::_2>/*element in vector*/>
                              >::type super;
  
  multi_policy_container (Base &base)
  : super (base)
  { }

  template<int N>
  struct index
  {
    typedef typename mpl::at_c<Vector, N>::type type;
  };
  
  template<class T>
  T &
  get ()
  {
    return static_cast< policy_wrap<Base, T> &> (*this).value_;
  }

  template<class T>
  const T &
  get () const
  {
    return static_cast< const policy_wrap<Base, T> &> (*this).value_;
  }

  template<int N>
  typename mpl::at_c<Vector, N>::type &
  get ()
  {
    typedef typename mpl::at_c<Vector, N>::type T;
    return static_cast< policy_wrap<Base, T> &> (*this).value_;
  }

  template<int N>
  const typename mpl::at_c<Vector, N>::type &
  get () const
  {
    typedef typename mpl::at_c<Vector, N>::type T;
    return static_cast< const policy_wrap<Base, T> &> (*this).value_;
  }
};


template<class BaseHook, class ValueType, int N>
struct FunctorHook
{
  typedef typename BaseHook::template index<N>::type hook_type;
  typedef hook_type*            hook_ptr;
  typedef const hook_type*      const_hook_ptr;
  
  typedef ValueType             value_type;
  typedef value_type*           pointer;
  typedef const value_type*     const_pointer;
  
  //Required static functions
  static hook_ptr to_hook_ptr (value_type &value)
  {  return &value.policy_hook_.template get<N> (); }
  
  static const_hook_ptr to_hook_ptr(const value_type &value)
  {  return &value.policy_hook_.template get<N> (); }
  
  static pointer to_value_ptr(hook_ptr n)
  {
    return
      bi::get_parent_from_member<value_type>
      (static_cast<BaseHook*> (bi::get_parent_from_member< wrap<hook_type> >(n, &wrap<hook_type>::value_)),
       &value_type::policy_hook_);
  }
  static const_pointer to_value_ptr(const_hook_ptr n)
  {
    return
      bi::get_parent_from_member<value_type>
      (static_cast<const BaseHook*> (bi::get_parent_from_member< wrap<hook_type> >(n, &wrap<hook_type>::value_)),
       &value_type::policy_hook_);
  }
};

template<typename Policies> // e.g., mpl::vector1< lru_policy_traits >
struct multi_policy_traits
{
  typedef Policies policy_traits;

  struct getHook
  {
    template<class Item>
    struct apply
    {
      typedef typename Item::policy_hook_type type;
    };
  };
    
  typedef multi_type_container< typename mpl::transform1<policy_traits, getHook>::type > policy_hook_type;
  typedef mpl::range_c<int, 0, mpl::size<policy_traits>::type::value> policies_range;
  
  template<class Container>
  struct container_hook
  {
    typedef void type;
    // struct getFunctionHook
    // {
    //   template<class Number>
    //   struct apply
    //   {
    //     // typedef void type;
    //     typedef bi::function_hook< FunctorHook <policy_hook_type,
    //                                             Container,
    //                                             Number::value> > type;
    //   };
    // };
    
    // struct type
    // {
    //   typedef
    //   typename mpl::transform1<policies_range,
    //                            getFunctionHook,
    //                            mpl::back_inserter< mpl::vector0<> > >::type hooks;
    // };
  };

  template<class Base,
           class Container,
           class Hook>
  struct policy 
  {
    // 1 hooks
    // 2 actual policies

    // struct getPolicy
    // {
    //   template<class Item>
    //   struct apply
    //   {
    //     typedef typename Item::template policy<Base,
    //                                            Container,
    //                                            typename mpl::at_c<typename Hook::hooks,0>::type >::type type;
    //   };
    // };
    
    // typedef typename mpl::transform1<policy_traits, getPolicy>::type policies;

    struct getPolicy
    {
      template<class Number>
      struct apply
      {
        typedef
        typename mpl::at_c<policy_traits, Number::value>::type::
        template policy<Base,
                        Container,
                        bi::function_hook< FunctorHook <policy_hook_type,
                                                        Container,
                                                        Number::value> > >::type
        type;
      };
    };
    
    typedef
    typename mpl::transform1<policies_range,
                             getPolicy,
                             mpl::back_inserter< mpl::vector0<> > >::type policies;
                             
    
    typedef multi_policy_container< Base, policies > policy_container;
    
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
    };
  };
};


int
main (int argc, char *argv[])
{
  CommandLine args;
  args.Parse (argc, argv);

  // typedef multi_policy_traits<lru_policy_traits> traits;
  // // traits::policy<traits::hook>
  // traits::policy<Bla1,Bla2, traits::container_hook<Bla2>::type> x;
  
  typedef trie_with_policy<
    ns3::CcnxNameComponents,
    smart_pointer_payload_traits<Integer>,
    multi_policy_traits<
      mpl::vector2<lru_policy_traits,fifo_policy_traits>
      // mpl::vector1<lru_policy_traits>
      > > trie;
  //   // multi_policy_traits<lru_policy_traits, random_policy_traits> > trie;
  
  trie x;
  x.getPolicy ().get<0> ().set_max_size (3);
  x.getPolicy ().get<1> ().set_max_size (100);
  // // x.getPolicy ().get<1> ().set_max_size (3);
  // // // x.getPolicy ().get1 ().set_max_size (10);
  // // // x.getPolicy ().get2 ().set_max_size (3);
  
  // // // x.getTrie ().PrintStat (std::cout);
  
  ns3::CcnxNameComponents n1,n2,n3,n4;
  // // // n1("a")("b")("c");
  // // // n2("a")("b")("d");
  // // // n3("a")("b")("f");
  // // // n4("a")("b");

  n1("a");
  n2("b");
  n3("c");
  n4("d");

  x.insert (n1, ns3::Create<Integer> (1));
  x.insert (n2, ns3::Create<Integer> (2));
  // // // x.longest_prefix_match (n1);
  x.insert (n3, ns3::Create<Integer> (3));
  x.insert (n4, ns3::Create<Integer> (4));
  x.insert (n4, ns3::Create<Integer> (4));

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

