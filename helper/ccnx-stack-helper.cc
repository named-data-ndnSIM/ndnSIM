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
 *          Ilya Moiseenko <iliamo@cs.ucla.edu> 
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/names.h"
#include "ns3/packet-socket-factory.h"
#include "ns3/config.h"
#include "ns3/simulator.h"
#include "ns3/string.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/node.h"
#include "ns3/core-config.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/wifi-net-device.h"

#include "../model/ccnx-forwarding-strategy.h"
#include "../model/ccnx-net-device-face.h"
#include "../model/ccnx-broadcast-net-device-face.h"
#include "../model/ccnx-l3-protocol.h"
#include "../model/ccnx-fib.h"

#include "ns3/node-list.h"
#include "ns3/loopback-net-device.h"
#include "ns3/global-router-interface.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-global-routing.h"
#include "../utils/ipv4-global-routing-ordered-nexthops.h"

#include "ns3/ipv4-routing-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/data-rate.h"

#include "ccnx-face-container.h"
#include "ccnx-stack-helper.h"

#include <limits>
#include <map>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxStackHelper");

namespace ns3 {
    
CcnxStackHelper::CcnxStackHelper ()
  : m_limitsEnabled (false)
  , m_needSetDefaultRoutes (false)
{
  m_strategyFactory.SetTypeId ("ns3::CcnxFloodingStrategy");
}
    
CcnxStackHelper::~CcnxStackHelper ()
{
}

void 
CcnxStackHelper::SetForwardingStrategy (std::string strategy)
{
  m_strategyFactory.SetTypeId (strategy);
}


void
CcnxStackHelper::SetDefaultRoutes (bool needSet)
{
  NS_LOG_FUNCTION (this << needSet);
  m_needSetDefaultRoutes = needSet;
}

void
CcnxStackHelper::EnableLimits (bool enable/* = true*/,
                               Time avgRtt/*=Seconds(0.1)*/,
                               uint32_t avgContentObject/*=1100*/,
                               uint32_t avgInterest/*=40*/)
{
  NS_LOG_INFO ("EnableLimits: " << enable);
  m_limitsEnabled = enable;
  m_avgRtt = avgRtt;
  m_avgContentObjectSize = avgContentObject;
  m_avgInterestSize = avgInterest;
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::Install (NodeContainer c) const
{
  Ptr<CcnxFaceContainer> faces = Create<CcnxFaceContainer> ();
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      faces->AddAll (Install (*i));
    }
  return faces;
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::InstallAll (void) const
{
  return Install (NodeContainer::GetGlobal ());
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::Install (Ptr<Node> node) const
{
  // NS_ASSERT_MSG (m_forwarding, "SetForwardingHelper() should be set prior calling Install() method");
  Ptr<CcnxFaceContainer> faces = Create<CcnxFaceContainer> ();
  
  if (node->GetObject<Ccnx> () != 0)
    {
      NS_FATAL_ERROR ("CcnxStackHelper::Install (): Installing " 
                      "a CcnxStack to a node with an existing Ccnx object");
      return 0;
    }

  Ptr<CcnxFib> fib = CreateObject<CcnxFib> ();
  node->AggregateObject (fib);

  Ptr<CcnxL3Protocol> ccnx = CreateObject<CcnxL3Protocol> ();
  node->AggregateObject (ccnx);

  ccnx->SetForwardingStrategy (m_strategyFactory.Create<CcnxForwardingStrategy> ());
  
  for (uint32_t index=0; index < node->GetNDevices (); index++)
    {
      Ptr<NetDevice> device = node->GetDevice (index);
      if (DynamicCast<LoopbackNetDevice> (device) != 0)
        continue; // don't create face for a LoopbackNetDevice

      Ptr<CcnxNetDeviceFace> face;
      if (DynamicCast<WifiNetDevice> (device) != 0)
        {
          // broadcast face
          face = CreateObject<CcnxBroadcastNetDeviceFace> (node, device);
        }
      else
        {
          // non-broadcast face
          face = CreateObject<CcnxNetDeviceFace> (node, device);
        }

      ccnx->AddFace (face);
      NS_LOG_LOGIC ("Node " << node->GetId () << ": added CcnxNetDeviceFace as face #" << *face);

      if (m_needSetDefaultRoutes)
        {
          // default route with lowest priority possible
          AddRoute (node, "/", face, std::numeric_limits<int32_t>::max ()); 
        }
      
      if (m_limitsEnabled)
        {
          NS_LOG_INFO ("Limits are enabled");
          Ptr<PointToPointNetDevice> p2p = DynamicCast<PointToPointNetDevice> (device);
          if (p2p != 0)
            {
              // Setup bucket filtering
              // Assume that we know average data packet size, and this size is equal default size
              // Set maximum buckets (averaging over 1 second)
      
              DataRateValue dataRate; device->GetAttribute ("DataRate", dataRate);
          
              NS_LOG_INFO("DataRate for this link is " << dataRate.Get());

              double maxInterestPackets = 1.0  * dataRate.Get ().GetBitRate () / 8.0 / (m_avgContentObjectSize + m_avgInterestSize);
              NS_LOG_INFO ("Max packets per second: " << maxInterestPackets);
              NS_LOG_INFO ("Max burst: " << m_avgRtt.ToDouble (Time::S) * maxInterestPackets);

              // Set bucket max to BDP
              face->SetBucketMax (m_avgRtt.ToDouble (Time::S) * maxInterestPackets); // number of interests allowed
              face->SetBucketLeak (maxInterestPackets);
            }
        }
        
      face->SetUp ();
      faces->Add (face);
    }
    
  return faces;
}

Ptr<CcnxFaceContainer>
CcnxStackHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return Install (node);
}


void
CcnxStackHelper::AddRoute (Ptr<Node> node, std::string prefix, Ptr<CcnxFace> face, int32_t metric)
{
  NS_LOG_LOGIC ("[" << node->GetId () << "]$ route add " << prefix << " via " << *face << " metric " << metric);

  Ptr<CcnxFib>  fib  = node->GetObject<CcnxFib> ();

  CcnxNameComponentsValue prefixValue;
  prefixValue.DeserializeFromString (prefix, MakeCcnxNameComponentsChecker ());
  fib->Add (prefixValue.Get (), face, metric);
}

void
CcnxStackHelper::AddRoute (std::string nodeName, std::string prefix, uint32_t faceId, int32_t metric)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  NS_ASSERT_MSG (node != 0, "Node [" << nodeName << "] does not exist");
  
  Ptr<Ccnx>     ccnx = node->GetObject<Ccnx> ();
  NS_ASSERT_MSG (ccnx != 0, "Ccnx stack should be installed on the node");

  Ptr<CcnxFace> face = ccnx->GetFace (faceId);
  NS_ASSERT_MSG (face != 0, "Face with ID [" << faceId << "] does not exist on node [" << nodeName << "]");

  AddRoute (node, prefix, face, metric);
}


void
CcnxStackHelper::InstallFakeGlobalRoutesImpl ()
{
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      NS_ASSERT_MSG ((*node)->GetObject<Ipv4> () != 0,
                     "InternetStack should be installed on all nodes");

      NS_ASSERT_MSG (Ipv4RoutingHelper::GetRouting<Ipv4GlobalRoutingOrderedNexthops>
                     (
                      (*node)->GetObject<Ipv4> ()->GetRoutingProtocol ()
                      ),
                     "InternetStack should have Ipv4GlobalRoutingOrderedNexthops as routing protocol");
      // Example:
      //
      // Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingUnorderedNexthops");
      // stack.SetRoutingHelper (ipv4RoutingHelper);
      //
      
      Ptr<GlobalRouter> globalRouter = (*node)->GetObject<GlobalRouter> ();
      if (globalRouter == 0) continue;

      globalRouter->InjectRoute (Ipv4Address((*node)->GetId ()), Ipv4Mask("255.255.255.255"));
    }
}

void
CcnxStackHelper::InstallFakeGlobalRoutes ()
{
  InstallFakeGlobalRoutesImpl ();
  Ipv4GlobalRoutingHelper::PopulateAllPossibleRoutingTables ();
}

void
CcnxStackHelper::InstallRouteTo (Ptr<Node> destNode)
{
  InstallRouteTo (boost::lexical_cast<std::string> (destNode->GetId ()), destNode);
}

void
CcnxStackHelper::InstallRouteTo (const std::string &prefix, Ptr<Node> destNode)
{
  Ipv4Address destIpv4 = Ipv4Address(destNode->GetId ());
  
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      if (destNode == *node) continue;
      
      Ptr<Ccnx> ccnx = (*node)->GetObject<Ccnx> ();
      NS_ASSERT_MSG (ccnx != 0, "CCNx stack should be installed on all nodes");

      Ptr<Ipv4> ipv4 = (*node)->GetObject<Ipv4> ();
      NS_ASSERT_MSG (ipv4 != 0,
                     "InternetStack should be installed on all nodes");
      
      Ptr<Ipv4GlobalRoutingOrderedNexthops> routing =
        Ipv4RoutingHelper::GetRouting<Ipv4GlobalRoutingOrderedNexthops> (ipv4->GetRoutingProtocol ());
      NS_ASSERT_MSG (routing != 0, "Ipv4GlobalRoutingOrderedNexthops should be used in InternetStack");

      Ptr<Ipv4GlobalRoutingOrderedNexthops::EntryContainer>
        routes = routing->Lookup (destIpv4);

      NS_ASSERT_MSG (routes != 0, "Should not happen... Call the developer");

      BOOST_FOREACH (const Ipv4RoutingTableEntry &entry, *routes)
        {
          Ptr<NetDevice> netDevice = ipv4->GetNetDevice (entry.GetInterface ());
          NS_ASSERT_MSG (netDevice != 0, "Should never happen. Call the popos");
          
          Ptr<CcnxFace> face = ccnx->GetFaceByNetDevice (netDevice);
          NS_ASSERT_MSG (face != 0, "Definitely should never happen. Call the president");
            
          AddRoute (*node, prefix, face, entry.GetMetric ());
        }
    }
}

void
CcnxStackHelper::InstallRoutesToAll ()
{
  for (NodeList::Iterator node = NodeList::Begin ();
       node != NodeList::End ();
       node ++)
    {
      InstallRouteTo (*node);
    }
}


} // namespace ns3
