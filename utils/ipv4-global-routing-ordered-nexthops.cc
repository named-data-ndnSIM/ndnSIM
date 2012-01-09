// -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2008 University of Washington
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "ns3/names.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/net-device.h"
#include "ns3/ipv4-route.h"
#include "ipv4-global-routing-ordered-nexthops.h"

#include <boost/lambda/lambda.hpp>
#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("Ipv4GlobalRoutingOrderedNexthops");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4GlobalRoutingOrderedNexthops);

TypeId 
Ipv4GlobalRoutingOrderedNexthops::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("ns3::Ipv4GlobalRoutingOrderedNexthops")
    .SetParent<Ipv4GlobalRouting> ()
    .AddConstructor<Ipv4GlobalRoutingOrderedNexthops> ()
  ;
  return tid;
}

Ipv4GlobalRoutingOrderedNexthops::Ipv4GlobalRoutingOrderedNexthops ()
{
}

void
Ipv4GlobalRoutingOrderedNexthops::AddRouteTo (Ipv4Address dest, 
                                         Ipv4Mask destMask, 
                                         Ipv4Address nextHop, 
                                         uint32_t interface,
                                         uint32_t metric/*=0*/)
{
  // if (m_ipv4->GetObject<Node> ()->GetId ()!=3) return;
  NS_LOG_FUNCTION (dest << destMask << nextHop << interface << metric);
  
  // First, make sure we don't try to add route to ourselves
  int32_t iface = m_ipv4->GetInterfaceForPrefix (dest, destMask);
  NS_LOG_LOGIC ("Iface " << iface << " for " << dest);
  if (destMask != Ipv4Mask::GetZero () && iface >= 0)
    {
      NS_LOG_LOGIC ("Do not add route to ourselves");
      return;
    }

  // Second, there is no reason to add p2p route that equals to the next hop
  if (destMask == Ipv4Mask::GetOnes () && dest == nextHop)
    {
      NS_LOG_LOGIC ("Ignore route to nexthop via nexhop");
      return;
    }

  Ptr<EntryContainer> nextHops = 0;
  
  Ipv4AddressTrieMap::iterator route =
    m_routes.find (dest.CombineMask (destMask));
  if (route == m_routes.end ())
    {
      nextHops = Create<EntryContainer> ();
      m_routes[dest.CombineMask (destMask)] = nextHops;
    }
  else
    {
      nextHops = route->second;
    }

  std::pair<EntryContainer::iterator,bool> result =
  nextHops->insert (Ipv4RoutingTableEntry::CreateNetworkRouteTo (dest, destMask, nextHop, interface, metric));
  if (!result.second)
    {
      NS_LOG_LOGIC ("Entry for the interface already exists");
      if (result.first->GetMetric () > metric)
        {
          NS_LOG_LOGIC ("Update metric");
          nextHops->modify (result.first,
                            boost::bind(&Ipv4RoutingTableEntry::SetMetric, boost::lambda::_1, metric));
        }
    }
  
  nextHops->get<i_index> ().rearrange (nextHops->get<i_metric> ().begin ());
}

Ptr<Ipv4Route>
Ipv4GlobalRoutingOrderedNexthops::LookupGlobal (Ipv4Address dest, Ptr<NetDevice> oif)
{
  NS_LOG_FUNCTION (this << dest << oif);

  // Cheating with lookups. Need to redesign Trie
  Ipv4AddressTrieMap::const_iterator longest_prefix_map;
  Ipv4Mask mask ("255.255.255.255");
  do {
    NS_LOG_DEBUG ("Try mask " << mask);
    longest_prefix_map = m_routes.longest_prefix_match (dest.CombineMask (mask));
    mask = Ipv4Mask (mask.Get () << 8);
  } while (longest_prefix_map == m_routes.end () && !mask.IsEqual (Ipv4Mask::GetZero ()));

  if (longest_prefix_map == m_routes.end ())
    {
      NS_LOG_LOGIC ("Route not found...");
      return 0;
    }

  const Ipv4RoutingTableEntry & entry = longest_prefix_map->second->get<i_index> ()[0];
  
  if (oif != 0 && oif == m_ipv4->GetNetDevice (entry.GetInterface ()))
    {
      NS_LOG_LOGIC ("Route points to the incoming interface. Return empty route");
      return 0;
    }
  
  // create a Ipv4Route object from the selected routing table entry
  Ptr<Ipv4Route> rtentry = Create<Ipv4Route> ();
  rtentry->SetDestination (entry.GetDest ());
  rtentry->SetSource (m_ipv4->GetAddress (entry.GetInterface (), 0).GetLocal ());
  rtentry->SetGateway (entry.GetGateway ());
  rtentry->SetOutputDevice (m_ipv4->GetNetDevice (entry.GetInterface ()));
  return rtentry;
}

const Ptr<Ipv4GlobalRoutingOrderedNexthops::EntryContainer>
Ipv4GlobalRoutingOrderedNexthops::Lookup (Ipv4Address dest)
{
  NS_LOG_FUNCTION (this << dest);
  
  Ipv4AddressTrieMap::const_iterator longest_prefix_map = m_routes.longest_prefix_match (dest);
  if (longest_prefix_map == m_routes.end ())
    {
      return 0;
    }

  return longest_prefix_map->second;
}


void
Ipv4GlobalRoutingOrderedNexthops::DeleteRoutes ()
{
  m_routes.clear ();
}

void
Ipv4GlobalRoutingOrderedNexthops::PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const
{
  std::ostream* os = stream->GetStream ();
  if (m_routes.size () > 0)
    {
      *os << "Destination    Iface(Metric),...,Iface(Metric)" << std::endl;
      for (Ipv4AddressTrieMap::const_iterator i=m_routes.begin (); i != m_routes.end (); i++)
        {
          std::ostringstream dest, gw;
          const Ipv4RoutingTableEntry &route = i->second->get<i_index> ()[0];
          dest << route.GetDest () << "/" << route.GetDestNetworkMask().GetPrefixLength ();
          gw   << route.GetGateway ();

          *os << std::setiosflags (std::ios::left) << std::setw (15) << dest.str ();

          for (EntryContainer::index<i_metric>::type::iterator metric = i->second->get<i_metric> ().begin ();
               metric != i->second->get<i_metric> ().end ();
               metric ++)
            {
              if (metric != i->second->get<i_metric> ().begin ())
                *os << ",";
              *os << metric->GetInterface () << "(" << metric->GetMetric () << ")";
            }

          // for (EntryContainer::iterator metric = i->second->begin ();
          //      metric != i->second->end ();
          //      metric ++)
          //   {
          //     if (metric != i->second->begin ())
          //       *os << ",";
          //     *os << metric->GetInterface () << "(" << metric->GetMetric () << ")";
          //   }
          
          *os << std::endl;
        }
    }
}

Ptr<Ipv4Route>
Ipv4GlobalRoutingOrderedNexthops::RouteOutput (Ptr<Packet> p, const Ipv4Header &header,
                                          Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
//
// First, see if this is a multicast packet we have a route for.  If we
// have a route, then send the packet down each of the specified interfaces.
//
  if (header.GetDestination ().IsMulticast ())
    {
      NS_LOG_LOGIC ("Multicast destination-- returning false");
      return 0; // Let other routing protocols try to handle this
    }
//
// See if this is a unicast packet we have a route for.
//
  NS_LOG_LOGIC ("Unicast destination- looking up");
  Ptr<Ipv4Route> rtentry = LookupGlobal (header.GetDestination (), oif);
  if (rtentry)
    {
      sockerr = Socket::ERROR_NOTERROR;
    }
  else
    {
      sockerr = Socket::ERROR_NOROUTETOHOST;
    }
  return rtentry;
}

bool 
Ipv4GlobalRoutingOrderedNexthops::RouteInput (Ptr<const Packet> p, const Ipv4Header &header,
                                         Ptr<const NetDevice> idev,
                                         UnicastForwardCallback ucb, MulticastForwardCallback mcb,
                                         LocalDeliverCallback lcb, ErrorCallback ecb)
{ 

  NS_LOG_FUNCTION (this << p << header << header.GetSource () << header.GetDestination () << idev);
  // Check if input device supports IP
  NS_ASSERT (m_ipv4->GetInterfaceForDevice (idev) >= 0);
  uint32_t iif = m_ipv4->GetInterfaceForDevice (idev);

  if (header.GetDestination ().IsMulticast ())
    {
      NS_LOG_LOGIC ("Multicast destination-- returning false");
      return false; // Let other routing protocols try to handle this
    }

  if (header.GetDestination ().IsBroadcast ())
    {
      NS_LOG_LOGIC ("For me (Ipv4Addr broadcast address)");
      // TODO:  Local Deliver for broadcast
      // TODO:  Forward broadcast
    }

  // TODO:  Configurable option to enable RFC 1222 Strong End System Model
  // Right now, we will be permissive and allow a source to send us
  // a packet to one of our other interface addresses; that is, the
  // destination unicast address does not match one of the iif addresses,
  // but we check our other interfaces.  This could be an option
  // (to remove the outer loop immediately below and just check iif).
  for (uint32_t j = 0; j < m_ipv4->GetNInterfaces (); j++)
    {
      for (uint32_t i = 0; i < m_ipv4->GetNAddresses (j); i++)
        {
          Ipv4InterfaceAddress iaddr = m_ipv4->GetAddress (j, i);
          Ipv4Address addr = iaddr.GetLocal ();
          if (addr.IsEqual (header.GetDestination ()))
            {
              if (j == iif)
                {
                  NS_LOG_LOGIC ("For me (destination " << addr << " match)");
                }
              else
                {
                  NS_LOG_LOGIC ("For me (destination " << addr << " match) on another interface " << header.GetDestination ());
                }
              lcb (p, header, iif);
              return true;
            }
          if (header.GetDestination ().IsEqual (iaddr.GetBroadcast ()))
            {
              NS_LOG_LOGIC ("For me (interface broadcast address)");
              lcb (p, header, iif);
              return true;
            }
          NS_LOG_LOGIC ("Address "<< addr << " not a match");
        }
    }
  // Check if input device supports IP forwarding
  if (m_ipv4->IsForwarding (iif) == false)
    {
      NS_LOG_LOGIC ("Forwarding disabled for this interface");
      ecb (p, header, Socket::ERROR_NOROUTETOHOST);
      return false;
    }
  // Next, try to find a route
  NS_LOG_LOGIC ("Unicast destination- looking up global route");
  Ptr<Ipv4Route> rtentry = LookupGlobal (header.GetDestination ());
  if (rtentry != 0)
    {
      NS_LOG_LOGIC ("Found unicast destination- calling unicast callback");
      ucb (rtentry, p, header);
      return true;
    }
  else
    {
      NS_LOG_LOGIC ("Did not find unicast destination- returning false");
      return false; // Let other routing protocols try to handle this
                    // route request.
    }
}

} // namespace ns3
