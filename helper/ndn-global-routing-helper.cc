/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#endif

#include "ndn-global-routing-helper.h"

#include "ns3/ndn-l3-protocol.h"
#include "../model/ndn-net-device-face.h"
#include "../model/ndn-global-router.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-fib.h"

#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/node-list.h"
#include "ns3/channel-list.h"
#include "ns3/object-factory.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/concept/assert.hpp>
// #include <boost/graph/graph_concepts.hpp>
// #include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "boost-graph-ndn-global-routing-helper.h"

#include <math.h>

NS_LOG_COMPONENT_DEFINE ("ndn.GlobalRoutingHelper");

using namespace std;
using namespace boost;

namespace ns3 {
namespace ndn {

void
GlobalRoutingHelper::Install (Ptr<Node> node)
{
  NS_LOG_LOGIC ("Node: " << node->GetId ());

  Ptr<L3Protocol> ndn = node->GetObject<L3Protocol> ();
  NS_ASSERT_MSG (ndn != 0, "Cannot install GlobalRoutingHelper before Ndn is installed on a node");

  Ptr<GlobalRouter> gr = node->GetObject<GlobalRouter> ();
  if (gr != 0)
    {
      NS_LOG_DEBUG ("GlobalRouter is already installed: " << gr);
      return; // already installed
    }

  gr = CreateObject<GlobalRouter> ();
  node->AggregateObject (gr);

  for (uint32_t faceId = 0; faceId < ndn->GetNFaces (); faceId++)
    {
      Ptr<NetDeviceFace> face = DynamicCast<NetDeviceFace> (ndn->GetFace (faceId));
      if (face == 0)
	{
	  NS_LOG_DEBUG ("Skipping non-netdevice face");
	  continue;
	}

      Ptr<NetDevice> nd = face->GetNetDevice ();
      if (nd == 0)
	{
	  NS_LOG_DEBUG ("Not a NetDevice associated with NetDeviceFace");
	  continue;
	}

      Ptr<Channel> ch = nd->GetChannel ();

      if (ch == 0)
	{
	  NS_LOG_DEBUG ("Channel is not associated with NetDevice");
	  continue;
	}

      if (ch->GetNDevices () == 2) // e.g., point-to-point channel
	{
	  for (uint32_t deviceId = 0; deviceId < ch->GetNDevices (); deviceId ++)
	    {
	      Ptr<NetDevice> otherSide = ch->GetDevice (deviceId);
	      if (nd == otherSide) continue;

	      Ptr<Node> otherNode = otherSide->GetNode ();
	      NS_ASSERT (otherNode != 0);

	      Ptr<GlobalRouter> otherGr = otherNode->GetObject<GlobalRouter> ();
	      if (otherGr == 0)
		{
		  Install (otherNode);
		}
	      otherGr = otherNode->GetObject<GlobalRouter> ();
	      NS_ASSERT (otherGr != 0);
	      gr->AddIncidency (face, otherGr);
	    }
	}
      else
	{
	  Ptr<GlobalRouter> grChannel = ch->GetObject<GlobalRouter> ();
	  if (grChannel == 0)
	    {
	      Install (ch);
	    }
	  grChannel = ch->GetObject<GlobalRouter> ();

	  gr->AddIncidency (face, grChannel);
	}
    }
}

void
GlobalRoutingHelper::Install (Ptr<Channel> channel)
{
  NS_LOG_LOGIC ("Channel: " << channel->GetId ());

  Ptr<GlobalRouter> gr = channel->GetObject<GlobalRouter> ();
  if (gr != 0)
    return;

  gr = CreateObject<GlobalRouter> ();
  channel->AggregateObject (gr);

  for (uint32_t deviceId = 0; deviceId < channel->GetNDevices (); deviceId ++)
    {
      Ptr<NetDevice> dev = channel->GetDevice (deviceId);

      Ptr<Node> node = dev->GetNode ();
      NS_ASSERT (node != 0);

      Ptr<GlobalRouter> grOther = node->GetObject<GlobalRouter> ();
      if (grOther == 0)
	{
	  Install (node);
	}
      grOther = node->GetObject<GlobalRouter> ();
      NS_ASSERT (grOther != 0);

      gr->AddIncidency (0, grOther);
    }
}

void
GlobalRoutingHelper::Install (const NodeContainer &nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node ++)
    {
      Install (*node);
    }
}

void
GlobalRoutingHelper::InstallAll ()
{
  Install (NodeContainer::GetGlobal ());
}


void
GlobalRoutingHelper::AddOrigin (const std::string &prefix, Ptr<Node> node)
{
  Ptr<GlobalRouter> gr = node->GetObject<GlobalRouter> ();
  NS_ASSERT_MSG (gr != 0,
		 "GlobalRouter is not installed on the node");

  Ptr<Name> name = Create<Name> (boost::lexical_cast<Name> (prefix));
  gr->AddLocalPrefix (name);
}

void
GlobalRoutingHelper::AddOrigins (const std::string &prefix, const NodeContainer &nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node++)
    {
      AddOrigin (prefix, *node);
    }
}

void
GlobalRoutingHelper::AddOrigin (const std::string &prefix, const std::string &nodeName)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  NS_ASSERT_MSG (node != 0, nodeName << "is not a Node");

  AddOrigin (prefix, node);
}

void
GlobalRoutingHelper::AddOriginsForAll ()
{
  for (NodeList::Iterator node = NodeList::Begin (); node != NodeList::End (); node ++)
    {
      Ptr<GlobalRouter> gr = (*node)->GetObject<GlobalRouter> ();
      string name = Names::FindName (*node);

      if (gr != 0 && !name.empty ())
        {
          AddOrigin ("/"+name, *node);
        }
    }
}

void
GlobalRoutingHelper::CalculateRoutes (bool invalidatedRoutes/* = true*/)
{
  /**
   * Implementation of route calculation is heavily based on Boost Graph Library
   * See http://www.boost.org/doc/libs/1_49_0/libs/graph/doc/table_of_contents.html for more details
   */

  BOOST_CONCEPT_ASSERT(( VertexListGraphConcept< NdnGlobalRouterGraph > ));
  BOOST_CONCEPT_ASSERT(( IncidenceGraphConcept< NdnGlobalRouterGraph > ));

  NdnGlobalRouterGraph graph;
  // typedef graph_traits < NdnGlobalRouterGraph >::vertex_descriptor vertex_descriptor;

  // For now we doing Dijkstra for every node.  Can be replaced with Bellman-Ford or Floyd-Warshall.
  // Other algorithms should be faster, but they need additional EdgeListGraph concept provided by the graph, which
  // is not obviously how implement in an efficient manner
  for (NodeList::Iterator node = NodeList::Begin (); node != NodeList::End (); node++)
    {
      Ptr<GlobalRouter> source = (*node)->GetObject<GlobalRouter> ();
      if (source == 0)
	{
	  NS_LOG_DEBUG ("Node " << (*node)->GetId () << " does not export GlobalRouter interface");
	  continue;
	}

      DistancesMap    distances;

      dijkstra_shortest_paths (graph, source,
			       // predecessor_map (boost::ref(predecessors))
			       // .
			       distance_map (boost::ref(distances))
			       .
			       distance_inf (WeightInf)
			       .
			       distance_zero (WeightZero)
			       .
			       distance_compare (boost::WeightCompare ())
			       .
			       distance_combine (boost::WeightCombine ())
			       );

      // NS_LOG_DEBUG (predecessors.size () << ", " << distances.size ());

      Ptr<Fib>  fib  = source->GetObject<Fib> ();
      if (invalidatedRoutes)
        {
          fib->InvalidateAll ();
        }
      NS_ASSERT (fib != 0);

      NS_LOG_DEBUG ("Reachability from Node: " << source->GetObject<Node> ()->GetId ());
      for (DistancesMap::iterator i = distances.begin ();
	   i != distances.end ();
	   i++)
	{
	  if (i->first == source)
	    continue;
	  else
	    {
	      // cout << "  Node " << i->first->GetObject<Node> ()->GetId ();
	      if (i->second.get<0> () == 0)
		{
		  // cout << " is unreachable" << endl;
		}
	      else
		{
                  BOOST_FOREACH (const Ptr<const Name> &prefix, i->first->GetLocalPrefixes ())
                    {
                      NS_LOG_DEBUG (" prefix " << prefix << " reachable via face " << *i->second.get<0> ()
                                    << " with distance " << i->second.get<1> ()
                                    << " with delay " << i->second.get<2> ());

                      Ptr<fib::Entry> entry = fib->Add (prefix, i->second.get<0> (), i->second.get<1> ());
                      entry->SetRealDelayToProducer (i->second.get<0> (), Seconds (i->second.get<2> ()));

                      Ptr<Limits> faceLimits = i->second.get<0> ()->GetObject<Limits> ();

                      Ptr<Limits> fibLimits = entry->GetObject<Limits> ();
                      if (fibLimits != 0)
                        {
                          // if it was created by the forwarding strategy via DidAddFibEntry event
                          fibLimits->SetLimits (faceLimits->GetMaxRate (), 2 * i->second.get<2> () /*exact RTT*/);
                          NS_LOG_DEBUG ("Set limit for prefix " << *prefix << " " << faceLimits->GetMaxRate () << " / " <<
                                        2*i->second.get<2> () << "s (" << faceLimits->GetMaxRate () * 2 * i->second.get<2> () << ")");
                        }
                    }
		}
	    }
	}
    }
}

void
GlobalRoutingHelper::CalculateAllPossibleRoutes (bool invalidatedRoutes/* = true*/)
{
  /**
   * Implementation of route calculation is heavily based on Boost Graph Library
   * See http://www.boost.org/doc/libs/1_49_0/libs/graph/doc/table_of_contents.html for more details
   */

  BOOST_CONCEPT_ASSERT(( VertexListGraphConcept< NdnGlobalRouterGraph > ));
  BOOST_CONCEPT_ASSERT(( IncidenceGraphConcept< NdnGlobalRouterGraph > ));

  NdnGlobalRouterGraph graph;
  // typedef graph_traits < NdnGlobalRouterGraph >::vertex_descriptor vertex_descriptor;

  // For now we doing Dijkstra for every node.  Can be replaced with Bellman-Ford or Floyd-Warshall.
  // Other algorithms should be faster, but they need additional EdgeListGraph concept provided by the graph, which
  // is not obviously how implement in an efficient manner
  for (NodeList::Iterator node = NodeList::Begin (); node != NodeList::End (); node++)
    {
      Ptr<GlobalRouter> source = (*node)->GetObject<GlobalRouter> ();
      if (source == 0)
	{
	  NS_LOG_DEBUG ("Node " << (*node)->GetId () << " does not export GlobalRouter interface");
	  continue;
	}

      Ptr<Fib>  fib  = source->GetObject<Fib> ();
      if (invalidatedRoutes)
        {
          fib->InvalidateAll ();
        }
      NS_ASSERT (fib != 0);

      NS_LOG_DEBUG ("===========");
      NS_LOG_DEBUG ("Reachability from Node: " << source->GetObject<Node> ()->GetId () << " (" << Names::FindName (source->GetObject<Node> ()) << ")");

      Ptr<L3Protocol> l3 = source->GetObject<L3Protocol> ();
      NS_ASSERT (l3 != 0);

      // remember interface statuses
      std::vector<uint16_t> originalMetric (l3->GetNFaces ());
      for (uint32_t faceId = 0; faceId < l3->GetNFaces (); faceId++)
        {
          originalMetric[faceId] = l3->GetFace (faceId)->GetMetric ();
          l3->GetFace (faceId)->SetMetric (std::numeric_limits<uint16_t>::max ()-1); // value std::numeric_limits<uint16_t>::max () MUST NOT be used (reserved)
        }

      for (uint32_t enabledFaceId = 0; enabledFaceId < l3->GetNFaces (); enabledFaceId++)
        {
          if (DynamicCast<ndn::NetDeviceFace> (l3->GetFace (enabledFaceId)) == 0)
            continue;

          // enabling only faceId
          l3->GetFace (enabledFaceId)->SetMetric (originalMetric[enabledFaceId]);

          DistancesMap    distances;

          NS_LOG_DEBUG ("-----------");

          dijkstra_shortest_paths (graph, source,
                                   // predecessor_map (boost::ref(predecessors))
                                   // .
                                   distance_map (boost::ref(distances))
                                   .
                                   distance_inf (WeightInf)
                                   .
                                   distance_zero (WeightZero)
                                   .
                                   distance_compare (boost::WeightCompare ())
                                   .
                                   distance_combine (boost::WeightCombine ())
                                   );

          // NS_LOG_DEBUG (predecessors.size () << ", " << distances.size ());

          for (DistancesMap::iterator i = distances.begin ();
               i != distances.end ();
               i++)
            {
              if (i->first == source)
                continue;
              else
                {
                  // cout << "  Node " << i->first->GetObject<Node> ()->GetId ();
                  if (i->second.get<0> () == 0)
                    {
                      // cout << " is unreachable" << endl;
                    }
                  else
                    {
                      BOOST_FOREACH (const Ptr<const Name> &prefix, i->first->GetLocalPrefixes ())
                        {
                          NS_LOG_DEBUG (" prefix " << *prefix << " reachable via face " << *i->second.get<0> ()
                                        << " with distance " << i->second.get<1> ()
                                        << " with delay " << i->second.get<2> ());

                          if (i->second.get<0> ()->GetMetric () == std::numeric_limits<uint16_t>::max ()-1)
                            continue;

                          Ptr<fib::Entry> entry = fib->Add (prefix, i->second.get<0> (), i->second.get<1> ());
                          entry->SetRealDelayToProducer (i->second.get<0> (), Seconds (i->second.get<2> ()));

                          Ptr<Limits> faceLimits = i->second.get<0> ()->GetObject<Limits> ();

                          Ptr<Limits> fibLimits = entry->GetObject<Limits> ();
                          if (fibLimits != 0)
                            {
                              // if it was created by the forwarding strategy via DidAddFibEntry event
                              fibLimits->SetLimits (faceLimits->GetMaxRate (), 2 * i->second.get<2> () /*exact RTT*/);
                              NS_LOG_DEBUG ("Set limit for prefix " << *prefix << " " << faceLimits->GetMaxRate () << " / " <<
                                            2*i->second.get<2> () << "s (" << faceLimits->GetMaxRate () * 2 * i->second.get<2> () << ")");
                            }
                        }
                    }
                }
            }

          // disabling the face again
          l3->GetFace (enabledFaceId)->SetMetric (std::numeric_limits<uint16_t>::max ()-1);
        }

      // recover original interface statuses
      for (uint32_t faceId = 0; faceId < l3->GetNFaces (); faceId++)
        {
          l3->GetFace (faceId)->SetMetric (originalMetric[faceId]);
        }
    }
}


} // namespace ndn
} // namespace ns3
