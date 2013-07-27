/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013 University of California, Los Angeles
 *                    Alexander Afanasyev
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

#include "ndn-ip-faces-helper.h"
#include "ndn-ip-face-stack.h"

#include "ns3/ndn-stack-helper.h"
#include "ns3/node-container.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ndn-tcp-face.h"
#include "ndn-udp-face.h"

NS_LOG_COMPONENT_DEFINE ("ndn.IpFacesHelper");

using namespace std;

namespace ns3 {
namespace ndn {

void
IpFacesHelper::Install (Ptr<Node> node)
{
  Ptr<IpFaceStack> stack = CreateObject<IpFaceStack> ();
  node->AggregateObject (stack);
}

void
IpFacesHelper::Install (const NodeContainer &nodes)
{
  for (NodeContainer::Iterator node = nodes.Begin ();
       node != nodes.End ();
       node ++)
    {
      Install (*node);
    }
}

void
IpFacesHelper::InstallAll ()
{
  Install (NodeContainer::GetGlobal ());
}


struct TcpPrefixRegistrator : SimpleRefCount<TcpPrefixRegistrator>
{
  TcpPrefixRegistrator (Ptr<Node> node, const std::string &prefix, int16_t metric)
    : m_node (node)
    , m_prefix (prefix)
    , m_metric (metric)
  {
  }

  void
  Run (Ptr<Face> face)
  {
    ndn::StackHelper::AddRoute (m_node, m_prefix, face, m_metric);
  }
private:
  Ptr<Node> m_node;
  std::string m_prefix;
  int16_t m_metric;
};

static void
ScheduledCreateTcp (Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric)
{
  Ptr<IpFaceStack> stack = node->GetObject<IpFaceStack> ();
  NS_ASSERT_MSG (stack != 0, "ndn::IpFaceStack needs to be installed on the node");

  Ptr<Face> face = stack->GetTcpFaceByAddress (address);
  if (face == 0)
    {
      Ptr<TcpPrefixRegistrator> registrator = Create<TcpPrefixRegistrator> (node, prefix, metric);
      stack->CreateOrGetTcpFace (address, MakeCallback (&TcpPrefixRegistrator::Run, registrator));
    }
  else
    {
      ndn::StackHelper::AddRoute (node, prefix, face, metric);
    }
}

void
IpFacesHelper::CreateTcpFace (const Time &when, Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric/* = 1*/)
{
  Simulator::ScheduleWithContext (node->GetId (), when, ScheduledCreateTcp, node, address, prefix, metric);
}

static void
ScheduledCreateUdp (Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric)
{
  Ptr<IpFaceStack> stack = node->GetObject<IpFaceStack> ();
  NS_ASSERT_MSG (stack != 0, "ndn::IpFaceStack needs to be installed on the node");

  Ptr<Face> face = stack->CreateOrGetUdpFace (address);
  ndn::StackHelper::AddRoute (node, prefix, face, metric);
}

void
IpFacesHelper::CreateUdpFace (const Time &when, Ptr<Node> node, Ipv4Address address, const std::string &prefix, int16_t metric/* = 1*/)
{
  Simulator::ScheduleWithContext (node->GetId (), when, ScheduledCreateUdp, node, address, prefix, metric);
}

} // namespace ndn
} // namespace ns3
