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

#include "ndnSIM-trie.h"

#include "../utils/trie/trie-with-policy.h"
#include "../utils/trie/lru-policy.h"
#include "../utils/trie/random-policy.h"
#include "../utils/trie/fifo-policy.h"
#include "../utils/trie/multi-policy.h"

#include <boost/lexical_cast.hpp>

using namespace std;
using namespace ns3;
using namespace ndn::ndnSIM;
using namespace boost;

NS_LOG_COMPONENT_DEFINE ("ndn.Trie");

// class Integer : public ns3::SimpleRefCount<Integer>
// {
// public:
//   ::Integer (int value) : value_ (value) {}

//   operator int () const { return value_; }
// private:
//   int value_;
// };

// std::ostream &
// operator << (std::ostream &os, const ::Integer &i)
// {
//   os << (int)i;
//   return os;
// }

void
TrieTest::DoRun ()
{
  cerr << "TrieTest is temporarily broken" << endl;
  // typedef trie_with_policy<
  //   ns3::NdnName,
  //   smart_pointer_payload_traits<Integer>,
  //   multi_policy_traits<
  //     mpl::vector2<lru_policy_traits,random_policy_traits>
  //     > > trie;
  
  // trie x;
  // x.getPolicy ().get<0> ().set_max_size (100);
  // x.getPolicy ().get<1> ().set_max_size (3);
  // // // x.getPolicy ().get<1> ().set_max_size (3);
  // // // // x.getPolicy ().get1 ().set_max_size (10);
  // // // // x.getPolicy ().get2 ().set_max_size (3);
  
  // // // // x.getTrie ().PrintStat (std::cout);
  
  // ns3::NdnName n1,n2,n3,n4;
  // // // // n1("a")("b")("c");
  // // // // n2("a")("b")("d");
  // // // // n3("a")("b")("f");
  // // // // n4("a")("b");

  // n1("a");
  // n2("b");
  // n3("c");
  // n4("d");

  // x.insert (n1, ns3::Create<Integer> (1));
  // x.insert (n2, ns3::Create<Integer> (2));
  // // // // x.longest_prefix_match (n1);
  // x.insert (n3, ns3::Create<Integer> (3));
  // x.insert (n4, ns3::Create<Integer> (4));
  // x.insert (n4, ns3::Create<Integer> (4));

  // std::cout << "digraph trie {\n";
  // std::cout << x.getTrie ();
  // std::cout << "}\n";

  // Ptr<Node> node = CreateObject<Node> ();
  // Names::Add ("TestNode", node);
  // Ptr<ndn::App> app = CreateObject<ndn::App> ();
  // node->AddApplication (app);
  
  // ObjectFactory factory ("ns3::ndn::fib::Default");
  
  // Ptr<ndn::Fib> fib = factory.Create<ndn::Fib> ();
  // node->AggregateObject (fib);
  // Ptr<ndn::Face> face = CreateObject<ndn::AppFace> (app);

  // fib->Add (lexical_cast<ndn::Name> ("/bla"), face, 1);
  // fib->Add (lexical_cast<ndn::Name> ("/bla/1"), face, 1);
  // fib->Add (lexical_cast<ndn::Name> ("/bla/2"), face, 1);
  // fib->Add (lexical_cast<ndn::Name> ("/bla/3"), face, 1);
  // fib->Add (lexical_cast<ndn::Name> ("/bla/1/1"), face, 1);
  // fib->Add (lexical_cast<ndn::Name> ("/bla/1/2"), face, 1);
  
  // cout << *fib << endl;

  // fib->RemoveFromAll (face);

  // cout << *fib << endl;
  // BOOST_FOREACH (const trie::parent_trie &item, x.getPolicy ())
  //   {
  //     std::cout << *item.payload () << " " << std::endl;
  //   }

  // ns3::NdnName n4;
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
}

