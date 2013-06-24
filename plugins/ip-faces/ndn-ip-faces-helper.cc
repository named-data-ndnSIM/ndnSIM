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


static void
CreateTcpFaceStep2 (Ptr<Node> node, Ipv4Address address, const std::string &prefix)
{
  Ptr<Face> face = TcpFace::CreateOrGetFace (node, address);
  ndn::StackHelper::AddRoute (node, prefix, face, 1);
}

static void
CreateTcpFaceStep1 (Ptr<Node> node, Ipv4Address address, const std::string &prefix)
{
  TcpFace::CreateOrGetFace (node, address);
  
  Simulator::ScheduleWithContext (node->GetId (), Seconds (1.0), CreateTcpFaceStep2, node, address, prefix);
}

void
IpFacesHelper::CreateTcpFace (Ptr<Node> node, Ipv4Address address, const std::string &prefix)
{
  Simulator::ScheduleWithContext (node->GetId (), Seconds (1.0), CreateTcpFaceStep1, node, address, prefix);
}


} // namespace ndn
} // namespace ns3
