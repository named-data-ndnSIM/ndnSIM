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

int
main (int argc, char *argv[])
{
  typedef trie_with_policy<ns3::CcnxNameComponents,
                           Integer,
                           smart_pointer_payload_traits<Integer>,
                           lru_policy_traits<CcnxNameComponents,
                                             Integer,
                                             smart_pointer_payload_traits<Integer> >
                           > trie;
  trie x;
  x.getPolicy ().set_max_size (100);
  
  // x.getTrie ().PrintStat (std::cout);
  
  ns3::CcnxNameComponents n1,n2,n3,n4;
  n1("a")("b")("c");
  n2("a")("b")("d");
  n3("a")("b")("f");
  n4("a")("b");

  ns3::Ptr<Integer> i = ns3::Create<Integer> (1);
  x.insert (n4, ns3::Create<Integer> (4));

  x.insert (n3, ns3::Create<Integer> (3));
  
  std::pair< trie::iterator, bool > item =
    x.insert (n2, ns3::Create<Integer> (2));
  // x.erase (item.first);

  x.insert (n1, ns3::Create<Integer> (1));
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

