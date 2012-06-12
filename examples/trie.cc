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
#include "../utils/trie.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Trie");

class Integer : public ns3::SimpleRefCount<Integer>
{
public:
  Integer (int value) : value_ (value) {}
  
private:
  int value_;
};

int
main (int argc, char *argv[])
{
  trie<ns3::CcnxNameComponents, Integer, smart_pointer_payload_traits<Integer> > *x =
    new trie<ns3::CcnxNameComponents, Integer, smart_pointer_payload_traits<Integer> >("root");

  ns3::CcnxNameComponents n1,n2,n3,n5;
  n1("a")("b")("c");
  n2("a")("c");
  n3("b")("c");
  n5("a")("b");

  ns3::Ptr<Integer> i = ns3::Create<Integer> (1);
  x->insert (n1, i);
  x->insert (n2, i);
  x->insert (n3, i);
  x->insert (n5, i);

  ns3::CcnxNameComponents n4;
  n4("a")("c");
    
  // std::cout << *x->find (n4).get<0> ();

  x->prune ();
  // x->find (n5).get<0> ()->erase ();
  x->find (n1).get<0> ()->erase ();
    
  std::cout << "digraph trie {\n";
  std::cout << *x;
  std::cout << "}\n";

  delete x;
  
  return 0;
}

