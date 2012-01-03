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
#include "ipv4-global-routing-unordered-nexthops.h"

#include <iomanip>

#ifndef UINT32_MAX
# define UINT32_MAX     (4294967295U)
#endif

NS_LOG_COMPONENT_DEFINE ("Ipv4GlobalRoutingUnorderedNexthops");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ipv4GlobalRoutingUnorderedNexthops);

TypeId 
Ipv4GlobalRoutingUnorderedNexthops::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("ns3::Ipv4GlobalRoutingUnorderedNexthops")
    .SetParent<Ipv4GlobalRouting> ()
    .AddConstructor<Ipv4GlobalRoutingUnorderedNexthops> ()
  ;
  return tid;
}

Ipv4GlobalRoutingUnorderedNexthops::Ipv4GlobalRoutingUnorderedNexthops ()
: m_numLogicalEntries (1)
{
}

void
Ipv4GlobalRoutingUnorderedNexthops::FixRoutes ()
{
  for (Ipv4AddressTrieMap::iterator route = m_routes.begin ();
       route != m_routes.end ();
       route ++)
    {
      if (route->second->size () < m_numLogicalEntries)
        {
          NS_LOG_WARN ("Not all entries got a new routing entry. Making one based on previous set");
          route->second->push_back (route->second->back ());
          route->second->back ().SetMetric (UINT32_MAX); // just to ensure we have consistency across the sets
        }
    }
  m_numLogicalEntries ++;
}

void
Ipv4GlobalRoutingUnorderedNexthops::AddRouteTo (Ipv4Address dest, 
                                                Ipv4Mask destMask, 
                                                Ipv4Address nextHop, 
                                                uint32_t interface,
                                                uint32_t metric/*=0*/)
{
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

  // NS_LOG_ERROR ("numLogicalEntries: " << m_numLogicalEntries << ", nextHops->size: " << nextHops->size ());
  NS_ASSERT_MSG (nextHops->size ()==m_numLogicalEntries ||
                 nextHops->size ()==m_numLogicalEntries-1,
                 "Number of entries in nextHops should be either equal to number of logical entries, or one smaller");

  if (nextHops->size ()==m_numLogicalEntries-1)
    {
      // new entry
      nextHops->push_back (Ipv4RoutingTableEntry::CreateNetworkRouteTo (dest, destMask, nextHop, interface, metric));
    }
  else
    {
      if (nextHops->back ().GetMetric () > metric)
        {
          // update entry if new metric is smaller
          nextHops->back () = Ipv4RoutingTableEntry::CreateNetworkRouteTo (dest, destMask, nextHop, interface, metric);
        }
    }
}

Ptr<Ipv4Route>
Ipv4GlobalRoutingUnorderedNexthops::LookupGlobal (uint32_t entryNum, Ipv4Address dest, Ptr<NetDevice> oif)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_LOGIC ("Looking for route for destination " << dest);

  NS_ASSERT_MSG (m_numLogicalEntries > 0,
    "Number of logical entries should be at least one");

  NS_ASSERT_MSG (entryNum < m_numLogicalEntries,
               "Number of requested logical entry should be smaller than total number of logical entries");
  
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

  const Ipv4RoutingTableEntry & entry = (*longest_prefix_map->second)[entryNum];
  
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

void
Ipv4GlobalRoutingUnorderedNexthops::DeleteRoutes ()
{
  m_routes.clear ();
}

void
Ipv4GlobalRoutingUnorderedNexthops::PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const
{
  std::ostream* os = stream->GetStream ();
  if (m_routes.size () > 0)
    {
      *os << "Destination    Iface(Metric),...,Iface(Metric)" << std::endl;
      for (Ipv4AddressTrieMap::const_iterator i=m_routes.begin (); i != m_routes.end (); i++)
        {
          if (i->second->size ()==0) continue;
          const Ipv4RoutingTableEntry &route = i->second->front ();

          std::ostringstream dest;
          dest << route.GetDest () << "/" << route.GetDestNetworkMask().GetPrefixLength ();
          *os << std::setiosflags (std::ios::left) << std::setw (15) << dest.str ();

          for (EntryContainer::iterator entry = i->second->begin ();
               entry != i->second->end ();
               entry ++)
            {
              if (entry != i->second->begin ())
                *os << ",";
              *os << entry->GetInterface () << "(" << entry->GetMetric () << ")";
            }

          *os << std::endl;
        }
    }
}

Ptr<Ipv4Route>
Ipv4GlobalRoutingUnorderedNexthops::RouteOutput (Ptr<Packet> p, const Ipv4Header &header,
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
  Ptr<Ipv4Route> rtentry = LookupGlobal (0, header.GetDestination (), oif);
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
Ipv4GlobalRoutingUnorderedNexthops::RouteInput (Ptr<const Packet> p, const Ipv4Header &header,
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
  Ptr<Ipv4Route> rtentry = LookupGlobal (0, header.GetDestination ());
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
