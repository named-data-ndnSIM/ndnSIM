/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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

#include "generic-tests.h"
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

#include "../model/pit/ndn-pit-impl.h"
#include "../utils/trie/persistent-policy.h"
#include "../apps/ndn-producer.h"

#include <boost/lexical_cast.hpp>

using namespace std;

NS_LOG_COMPONENT_DEFINE ("ndn.test.Generic");

namespace ns3 {
namespace ndn {

void
GenericTests::DoRun ()
{
  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper p2p;
  p2p.Install (nodes);
  
  StackHelper ndn;
  ndn.SetDefaultRoutes (true);
  ndn.Install (nodes);

  Ptr<Node> node = nodes.Get (0);
  
  Ptr<Pit> pit = node->GetObject<Pit> ();
  
  Ptr<InterestHeader> header = Create<InterestHeader> ();
  header->SetName (Create<NameComponents> ("/bla"));
  header->SetInterestLifetime (Seconds (100));

  Ptr<pit::Entry> e1 = pit->Create (header);
  NS_ASSERT (e1 != 0);

  header = Create<InterestHeader> ();
  header->SetName (Create<NameComponents> ("/foo"));
  header->SetInterestLifetime (Seconds (100));

  Ptr<pit::Entry> e2 = pit->Create (header);
  NS_ASSERT (e2 != 0);

  list< Ptr< pit::Entry > > l;

  l.push_back (e1);
  l.push_back (e2);

  BOOST_FOREACH (Ptr<pit::Entry> entry, l)
    {
      cout << "* " << entry->GetPrefix () << endl;
    }

  l.remove (e2);
  
  BOOST_FOREACH (Ptr<pit::Entry> entry, l)
    {
      cout << "* " << entry->GetPrefix () << endl;
    }
}

} // namespace ndn
} // namespace ns3
