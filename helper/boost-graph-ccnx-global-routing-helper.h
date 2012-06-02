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


#ifndef BOOST_GRAPH_CCNX_GLOBAL_ROUTING_HELPER_H
#define BOOST_GRAPH_CCNX_GLOBAL_ROUTING_HELPER_H

/// @cond include_hidden

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/ref.hpp>

#include "ns3/ccnx-face.h"
#include "ns3/node-list.h"
#include "ns3/channel-list.h"
#include "../model/ccnx-global-router.h"
#include <list>
#include <map>

namespace boost
{

class CcnxGlobalRouterGraph
{
public:
  typedef ns3::Ptr< ns3::CcnxGlobalRouter > Vertice;
  typedef uint16_t edge_property_type;
  typedef uint32_t vertex_property_type;
  
  CcnxGlobalRouterGraph ()
  {
    for (ns3::NodeList::Iterator node = ns3::NodeList::Begin (); node != ns3::NodeList::End (); node++)
      {
        ns3::Ptr<ns3::CcnxGlobalRouter> gr = (*node)->GetObject<ns3::CcnxGlobalRouter> ();
	if (gr != 0)
	  m_vertices.push_back (gr);
      }

    for (ns3::ChannelList::Iterator channel = ns3::ChannelList::Begin (); channel != ns3::ChannelList::End (); channel++)
      {
        ns3::Ptr<ns3::CcnxGlobalRouter> gr = (*channel)->GetObject<ns3::CcnxGlobalRouter> ();
	if (gr != 0)
	  m_vertices.push_back (gr);
      }
  }

  const std::list< Vertice > &
  GetVertices () const
  {
    return m_vertices;
  }
  
public:
  std::list< Vertice > m_vertices;
};


class ccnx_global_router_graph_category :
    public virtual vertex_list_graph_tag,
    public virtual incidence_graph_tag
{
};


template<>
struct graph_traits< CcnxGlobalRouterGraph >
{
  // Graph concept
  typedef CcnxGlobalRouterGraph::Vertice vertex_descriptor;
  typedef ns3::CcnxGlobalRouter::Incidency edge_descriptor;
  typedef directed_tag directed_category;
  typedef disallow_parallel_edge_tag edge_parallel_category;
  typedef ccnx_global_router_graph_category traversal_category;

  // VertexList concept
  typedef std::list< vertex_descriptor >::const_iterator vertex_iterator;
  typedef size_t vertices_size_type;

  // AdjacencyGraph concept
  typedef ns3::CcnxGlobalRouter::IncidencyList::iterator out_edge_iterator;
  typedef size_t degree_size_type;

  // typedef size_t edges_size_type;
};

} // namespace boost

namespace boost
{

inline
graph_traits< CcnxGlobalRouterGraph >::vertex_descriptor
source(
       graph_traits< CcnxGlobalRouterGraph >::edge_descriptor e,
       const CcnxGlobalRouterGraph& g)
{
  return e.get<0> ();
}

inline
graph_traits< CcnxGlobalRouterGraph >::vertex_descriptor
target(
       graph_traits< CcnxGlobalRouterGraph >::edge_descriptor e,
       const CcnxGlobalRouterGraph& g)
{
  return e.get<2> ();
}

inline
std::pair< graph_traits< CcnxGlobalRouterGraph >::vertex_iterator,
	   graph_traits< CcnxGlobalRouterGraph >::vertex_iterator >
vertices (const CcnxGlobalRouterGraph&g)
{
  return make_pair (g.GetVertices ().begin (), g.GetVertices ().end ());
}

inline
graph_traits< CcnxGlobalRouterGraph >::vertices_size_type
num_vertices(const CcnxGlobalRouterGraph &g)
{
  return g.GetVertices ().size ();
}
  

inline
std::pair< graph_traits< CcnxGlobalRouterGraph >::out_edge_iterator,
	   graph_traits< CcnxGlobalRouterGraph >::out_edge_iterator >  
out_edges(
	  graph_traits< CcnxGlobalRouterGraph >::vertex_descriptor u, 
	  const CcnxGlobalRouterGraph& g)
{
  return std::make_pair(u->GetIncidencies ().begin (),
			u->GetIncidencies ().end ());
}

inline
graph_traits< CcnxGlobalRouterGraph >::degree_size_type
out_degree(
	  graph_traits< CcnxGlobalRouterGraph >::vertex_descriptor u, 
	  const CcnxGlobalRouterGraph& g)
{
  return u->GetIncidencies ().size ();
}


//////////////////////////////////////////////////////////////
// Property maps

struct EdgeWeights
{
  EdgeWeights (const CcnxGlobalRouterGraph &graph)
  : m_graph (graph)
  { 
  }

private:
  const CcnxGlobalRouterGraph &m_graph;
};


struct VertexIds
{
  VertexIds (const CcnxGlobalRouterGraph &graph)
  : m_graph (graph)
  { 
  }

private:
  const CcnxGlobalRouterGraph &m_graph;
};

template<>
struct property_map< CcnxGlobalRouterGraph, edge_weight_t >
{
  typedef const EdgeWeights const_type;
  typedef EdgeWeights type;
};

template<>
struct property_map< CcnxGlobalRouterGraph, vertex_index_t >
{
  typedef const VertexIds const_type;
  typedef VertexIds type;
};


template<>
struct property_traits< EdgeWeights >
{
  // Metric property map
  typedef tuple< ns3::Ptr<ns3::CcnxFace>, uint16_t > value_type;
  typedef tuple< ns3::Ptr<ns3::CcnxFace>, uint16_t > reference;
  typedef ns3::CcnxGlobalRouter::Incidency key_type;
  typedef readable_property_map_tag category;
};

const property_traits< EdgeWeights >::value_type WeightZero (0, 0);
const property_traits< EdgeWeights >::value_type WeightInf (0, std::numeric_limits<uint16_t>::max ());

struct WeightCompare :
    public std::binary_function<property_traits< EdgeWeights >::reference,
                                property_traits< EdgeWeights >::reference,
                                bool>
{
  bool
  operator () (property_traits< EdgeWeights >::reference a,
               property_traits< EdgeWeights >::reference b) const
  {
    return a.get<1> () < b.get<1> ();
  }

  bool
  operator () (property_traits< EdgeWeights >::reference a,
               uint32_t b) const
  {
    return a.get<1> () < b;
  }
  
  bool
  operator () (uint32_t a,
               uint32_t b) const
  {
    return a < b;
  }

};

struct WeightCombine :
    public std::binary_function<uint32_t,
                                property_traits< EdgeWeights >::reference,
                                uint32_t>
{
  uint32_t
  operator () (uint32_t a, property_traits< EdgeWeights >::reference b) const
  {
    return a + b.get<1> ();
  }

  tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t >
  operator () (tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t > a,
               property_traits< EdgeWeights >::reference b) const
  {
    if (a.get<0> () == 0)
      return make_tuple (b.get<0> (), a.get<1> () + b.get<1> ());
    else
      return make_tuple (a.get<0> (), a.get<1> () + b.get<1> ());
  }
};
  
template<>
struct property_traits< VertexIds >
{
  // Metric property map
  typedef uint32_t value_type;
  typedef uint32_t reference;
  typedef ns3::Ptr< ns3::CcnxGlobalRouter > key_type;
  typedef readable_property_map_tag category;
};


inline EdgeWeights
get(edge_weight_t,
    const CcnxGlobalRouterGraph &g)
{
  return EdgeWeights (g);
}


inline VertexIds
get(vertex_index_t,
    const CcnxGlobalRouterGraph &g)
{
  return VertexIds (g);
}

template<class M, class K, class V>
inline void
put (reference_wrapper< M > mapp,
     K a, V p)
{
  mapp.get ()[a] = p;
}

// void
// put (cref< std::map< ns3::Ptr<ns3::CcnxGlobalRouter>, ns3::Ptr<ns3::CcnxGlobalRouter> > > map,

uint32_t
get (const boost::VertexIds&, ns3::Ptr<ns3::CcnxGlobalRouter> &gr)
{
  return gr->GetId ();
}

inline property_traits< EdgeWeights >::reference
get(const boost::EdgeWeights&, ns3::CcnxGlobalRouter::Incidency &edge)
{
  if (edge.get<1> () == 0)
    return property_traits< EdgeWeights >::reference (0, 0);
  else
    return property_traits< EdgeWeights >::reference (edge.get<1> (), edge.get<1> ()->GetMetric ());
}

struct PredecessorsMap :
  public std::map< ns3::Ptr< ns3::CcnxGlobalRouter >, ns3::Ptr< ns3::CcnxGlobalRouter > >
{
};

template<>
struct property_traits< reference_wrapper<PredecessorsMap> >
{
  // Metric property map
  typedef ns3::Ptr< ns3::CcnxGlobalRouter > value_type;
  typedef ns3::Ptr< ns3::CcnxGlobalRouter > reference;
  typedef ns3::Ptr< ns3::CcnxGlobalRouter > key_type;
  typedef read_write_property_map_tag category;
};


struct DistancesMap :
  public std::map< ns3::Ptr< ns3::CcnxGlobalRouter >, tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t > >
{
};

template<>
struct property_traits< reference_wrapper<DistancesMap> >
{
  // Metric property map
  typedef tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t > value_type;
  typedef tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t > reference;
  typedef ns3::Ptr< ns3::CcnxGlobalRouter > key_type;
  typedef read_write_property_map_tag category;
};

inline tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t >
get (DistancesMap &map, ns3::Ptr<ns3::CcnxGlobalRouter> key)
{
  boost::DistancesMap::iterator i = map.find (key);
  if (i == map.end ())
    return tuple< ns3::Ptr<ns3::CcnxFace>, uint32_t > (0, std::numeric_limits<uint32_t>::max ());
  else
    return i->second;
}

} // namespace boost

/// @endcond

#endif // BOOST_GRAPH_CCNX_GLOBAL_ROUTING_HELPER_H
