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

#include "ccnx-global-routing-helper.h"

#include "ns3/ccnx.h"
#include "../model/ccnx-net-device-face.h"
#include "../model/ccnx-global-router.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/ccnx-fib.h"

#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/node-list.h"
#include "ns3/channel-list.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/concept/assert.hpp>
// #include <boost/graph/graph_concepts.hpp>
// #include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "boost-graph-ccnx-global-routing-helper.h"

NS_LOG_COMPONENT_DEFINE ("CcnxGlobalRoutingHelper");

using namespace std;
using namespace boost;

namespace ns3 {

void
CcnxGlobalRoutingHelper::Install (Ptr<Node> node)
{
  NS_LOG_LOGIC ("Node: " << node->GetId ());
  
  Ptr<Ccnx> ccnx = node->GetObject<Ccnx> ();
  NS_ASSERT_MSG (ccnx != 0, "Cannot install CcnxGlobalRoutingHelper before Ccnx is installed on a node");

  Ptr<CcnxGlobalRouter> gr = node->GetObject<CcnxGlobalRouter> ();
  if (gr != 0)
    {
      NS_LOG_DEBUG ("CcnxGlobalRouter is already installed: " << gr);
      return; // already installed
    }
  
  gr = CreateObject<CcnxGlobalRouter> ();
  node->AggregateObject (gr);
  
  for (uint32_t faceId = 0; faceId < ccnx->GetNFaces (); faceId++)
    {
      Ptr<CcnxNetDeviceFace> face = DynamicCast<CcnxNetDeviceFace> (ccnx->GetFace (faceId));
      if (face == 0)
	{
	  NS_LOG_DEBUG ("Skipping non-netdevice face");
	  continue;
	}
      
      Ptr<NetDevice> nd = face->GetNetDevice ();
      if (nd == 0)
	{
	  NS_LOG_DEBUG ("Not a NetDevice associated with CcnxNetDeviceFace");
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
	      
	      Ptr<CcnxGlobalRouter> otherGr = otherNode->GetObject<CcnxGlobalRouter> ();
	      if (otherGr == 0)
		{
		  Install (otherNode);
		}
	      otherGr = otherNode->GetObject<CcnxGlobalRouter> ();
	      NS_ASSERT (otherGr != 0);
	      gr->AddIncidency (face, otherGr);
	    }
	}
      else
	{
	  Ptr<CcnxGlobalRouter> grChannel = ch->GetObject<CcnxGlobalRouter> ();
	  if (grChannel == 0)
	    {
	      Install (ch);
	    }
	  grChannel = ch->GetObject<CcnxGlobalRouter> ();
	  
	  gr->AddIncidency (0, grChannel);
	}
    }
}

void
CcnxGlobalRoutingHelper::Install (Ptr<Channel> channel)
{
  NS_LOG_LOGIC ("Channel: " << channel->GetId ());

  Ptr<CcnxGlobalRouter> gr = channel->GetObject<CcnxGlobalRouter> ();
  if (gr != 0)
    return;

  gr = CreateObject<CcnxGlobalRouter> ();
  channel->AggregateObject (gr);
  
  for (uint32_t deviceId = 0; deviceId < channel->GetNDevices (); deviceId ++)
    {
      Ptr<NetDevice> dev = channel->GetDevice (deviceId);

      Ptr<Node> node = dev->GetNode ();
      NS_ASSERT (node != 0);

      Ptr<CcnxGlobalRouter> grOther = node->GetObject<CcnxGlobalRouter> ();
      if (grOther == 0)
	{
	  Install (node);
	}
      grOther = node->GetObject<CcnxGlobalRouter> ();
      NS_ASSERT (grOther != 0);

      gr->AddIncidency (0, grOther);
    }
}

void
CcnxGlobalRoutingHelper::Install (const NodeContainer &nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node ++)
    {
      Install (*node);
    }
}

void
CcnxGlobalRoutingHelper::InstallAll ()
{
  Install (NodeContainer::GetGlobal ());
}


void
CcnxGlobalRoutingHelper::AddOrigin (const std::string &prefix, Ptr<Node> node)
{
  Ptr<CcnxGlobalRouter> gr = node->GetObject<CcnxGlobalRouter> ();
  NS_ASSERT_MSG (gr != 0,
		 "CcnxGlobalRouter is not installed on the node");

  Ptr<CcnxNameComponents> name = Create<CcnxNameComponents> (boost::lexical_cast<CcnxNameComponents> (prefix));
  gr->AddLocalPrefix (name);  
}

void
CcnxGlobalRoutingHelper::AddOrigins (const std::string &prefix, const NodeContainer &nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node++)
    {
      AddOrigin (prefix, *node);
    }
}

void
CcnxGlobalRoutingHelper::AddOrigin (const std::string &prefix, const std::string &nodeName)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  NS_ASSERT_MSG (node != 0, nodeName << "is not a Node");

  AddOrigin (prefix, node);
}

void
CcnxGlobalRoutingHelper::CalculateRoutes ()
{
  /**
   * Implementation of route calculation is heavily based on Boost Graph Library
   * See http://www.boost.org/doc/libs/1_49_0/libs/graph/doc/table_of_contents.html for more details
   */
  
  BOOST_CONCEPT_ASSERT(( VertexListGraphConcept< CcnxGlobalRouterGraph > ));
  BOOST_CONCEPT_ASSERT(( IncidenceGraphConcept< CcnxGlobalRouterGraph > ));
  
  CcnxGlobalRouterGraph graph;
  typedef graph_traits < CcnxGlobalRouterGraph >::vertex_descriptor vertex_descriptor;

  // For now we doing Dijkstra for every node.  Can be replaced with Bellman-Ford or Floyd-Warshall.
  // Other algorithms should be faster, but they need additional EdgeListGraph concept provided by the graph, which
  // is not obviously how implement in an efficient manner
  for (NodeList::Iterator node = NodeList::Begin (); node != NodeList::End (); node++)
    {
      Ptr<CcnxGlobalRouter> source = (*node)->GetObject<CcnxGlobalRouter> ();
      if (source == 0)
	{
	  NS_LOG_DEBUG ("Node " << (*node)->GetId () << " does not export CcnxGlobalRouter interface");
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

      Ptr<CcnxFib>  fib  = source->GetObject<CcnxFib> ();
      fib->InvalidateAll ();
      NS_ASSERT (fib != 0);
      
      // cout << "Reachability from Node: " << source->GetObject<Node> ()->GetId () << endl;
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
		// cout << " reachable via face " << *i->second.get<0> ()
		//      << " with distance " << i->second.get<1> () << endl;

		BOOST_FOREACH (const Ptr<const CcnxNameComponents> &prefix, i->first->GetLocalPrefixes ())
		  {
		    fib->Add (prefix, i->second.get<0> (), i->second.get<1> ());
		  }
		}
	    }
	}
    }
}


}
