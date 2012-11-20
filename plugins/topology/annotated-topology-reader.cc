/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "annotated-topology-reader.h"

#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/net-device-container.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-address.h"
#include "ns3/ndn-l3-protocol.h"
#include "ns3/ndn-face.h"
#include "ns3/random-variable.h"

#include "ns3/constant-position-mobility-model.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <set>

#ifdef NS3_MPI
#include <ns3/mpi-interface.h>
#endif

using namespace std;

namespace ns3 {    

NS_LOG_COMPONENT_DEFINE ("AnnotatedTopologyReader");
    
AnnotatedTopologyReader::AnnotatedTopologyReader (const std::string &path, double scale/*=1.0*/)
  : m_path (path)
  , m_randX (0, 100.0)
  , m_randY (0, 100.0)
  , m_scale (scale)
  , m_requiredPartitions (1)
{
  NS_LOG_FUNCTION (this);

  SetMobilityModel ("ns3::ConstantPositionMobilityModel");
}
    
void
AnnotatedTopologyReader::SetBoundingBox (double ulx, double uly, double lrx, double lry)
{
  NS_LOG_FUNCTION (this << ulx << uly << lrx << lry);
  
  m_randX = UniformVariable (ulx, lrx);
  m_randY = UniformVariable (uly, lry);
}

void
AnnotatedTopologyReader::SetMobilityModel (const std::string &model)
{
  NS_LOG_FUNCTION (this << model);
  m_mobilityFactory.SetTypeId (model);
}

AnnotatedTopologyReader::~AnnotatedTopologyReader ()
{
  NS_LOG_FUNCTION (this);
}

Ptr<Node>
AnnotatedTopologyReader::CreateNode (const std::string name, uint32_t systemId)
{
  NS_LOG_FUNCTION (this << name);
  m_requiredPartitions = std::max (m_requiredPartitions, systemId + 1);
  
  Ptr<Node> node = CreateObject<Node> (systemId);

  Names::Add (m_path, name, node);
  m_nodes.Add (node);

  return node;
}

Ptr<Node>
AnnotatedTopologyReader::CreateNode (const std::string name, double posX, double posY, uint32_t systemId)
{
  NS_LOG_FUNCTION (this << name << posX << posY);
  m_requiredPartitions = std::max (m_requiredPartitions, systemId + 1);
  
  Ptr<Node> node = CreateObject<Node> (systemId);
  Ptr<MobilityModel> loc = DynamicCast<MobilityModel> (m_mobilityFactory.Create ());
  node->AggregateObject (loc);

  loc->SetPosition (Vector (posX, posY, 0));

  Names::Add (m_path, name, node);
  m_nodes.Add (node);

  return node;
}

NodeContainer
AnnotatedTopologyReader::GetNodes () const
{
  return m_nodes;
}

const std::list<TopologyReader::Link>&
AnnotatedTopologyReader::GetLinks () const
{
  return m_linksList;
}

NodeContainer
AnnotatedTopologyReader::Read (void)
{
  ifstream topgen;
  topgen.open (GetFileName ().c_str ());
        
  if ( !topgen.is_open () || !topgen.good () )
    {
      NS_FATAL_ERROR ("Cannot open file " << GetFileName () << " for reading");
      return m_nodes;
    }

  while (!topgen.eof ())
    {
      string line;
      getline (topgen, line);

      if (line == "router") break;
    }

  if (topgen.eof ())
    {
      NS_FATAL_ERROR ("Topology file " << GetFileName () << " does not have \"router\" section");
      return m_nodes;
    }

  while (!topgen.eof ())
    {
      string line;
      getline (topgen,line);
      if (line[0] == '#') continue; // comments
      if (line=="link") break; // stop reading nodes
      
      istringstream lineBuffer (line);
      string name, city;
      double latitude = 0, longitude = 0;
      uint32_t systemId = 0;

      lineBuffer >> name >> city >> latitude >> longitude >> systemId;
      if (name.empty ()) continue;

      Ptr<Node> node;
      
      if (abs(latitude) > 0.001 && abs(latitude) > 0.001)
        node = CreateNode (name, m_scale*longitude, -m_scale*latitude, systemId);
      else
        {
          UniformVariable var (0,200);
          node = CreateNode (name, var.GetValue (), var.GetValue (), systemId);
          // node = CreateNode (name, systemId);
        }
    }

  map<string, set<string> > processedLinks; // to eliminate duplications
  
  if (topgen.eof ())
    {
      NS_FATAL_ERROR ("Topology file " << GetFileName () << " does not have \"link\" section");
      return m_nodes;
    }

  // SeekToSection ("link"); 
  while (!topgen.eof ())
    {
      string line;
      getline (topgen,line);
      if (line == "") continue;
      if (line[0] == '#') continue; // comments

      // NS_LOG_DEBUG ("Input: [" << line << "]");
      
      istringstream lineBuffer (line);
      string from, to, capacity, metric, delay, maxPackets;

      lineBuffer >> from >> to >> capacity >> metric >> delay >> maxPackets;

      if (processedLinks[to].size () != 0 &&
          processedLinks[to].find (from) != processedLinks[to].end ())
        {
          continue; // duplicated link
        }
      processedLinks[from].insert (to);
      
      Ptr<Node> fromNode = Names::Find<Node> (m_path, from);
      NS_ASSERT_MSG (fromNode != 0, from << " node not found");
      Ptr<Node> toNode   = Names::Find<Node> (m_path, to);
      NS_ASSERT_MSG (toNode != 0, to << " node not found");

      Link link (fromNode, from, toNode, to);
      
      link.SetAttribute ("DataRate", capacity);
      link.SetAttribute ("OSPF", metric);

      if (!delay.empty ())
          link.SetAttribute ("Delay", delay);
      if (!maxPackets.empty ())
        link.SetAttribute ("MaxPackets", maxPackets);
      
      AddLink (link);
      NS_LOG_DEBUG ("New link " << from << " <==> " << to << " / " << capacity << " with " << metric << " metric (" << delay << ", " << maxPackets << ")");
    }
        
  NS_LOG_INFO ("Annotated topology created with " << m_nodes.GetN () << " nodes and " << LinksSize () << " links");
  topgen.close ();
        
  ApplySettings ();
  
  return m_nodes;
}
    
void
AnnotatedTopologyReader::AssignIpv4Addresses (Ipv4Address base)
{
  Ipv4AddressHelper address (base, Ipv4Mask ("/24"));
    
  BOOST_FOREACH (const Link &link, m_linksList)
    {
      address.Assign (NetDeviceContainer (link.GetFromNetDevice (),
                                          link.GetToNetDevice ()));
        
      base = Ipv4Address (base.Get () + 256);
      address.SetBase (base, Ipv4Mask ("/24"));
    }
}
        
void
AnnotatedTopologyReader::ApplyOspfMetric ()
{
  BOOST_FOREACH (const Link &link, m_linksList)
    {
      NS_LOG_DEBUG ("OSPF: " << link.GetAttribute ("OSPF"));
      uint16_t metric = boost::lexical_cast<uint16_t> (link.GetAttribute ("OSPF"));
        
      {
        Ptr<Ipv4> ipv4 = link.GetFromNode ()->GetObject<Ipv4> ();
        if (ipv4 != 0)
          {
            int32_t interfaceId = ipv4->GetInterfaceForDevice (link.GetFromNetDevice ());
            NS_ASSERT (interfaceId >= 0);
        
            ipv4->SetMetric (interfaceId,metric);
          }

        Ptr<ndn::L3Protocol> ndn = link.GetFromNode ()->GetObject<ndn::L3Protocol> ();
        if (ndn != 0)
          {
            Ptr<ndn::Face> face = ndn->GetFaceByNetDevice (link.GetFromNetDevice ());
            NS_ASSERT (face != 0);
            
            face->SetMetric (metric);
          }
      }
        
      {
        Ptr<Ipv4> ipv4 = link.GetToNode ()->GetObject<Ipv4> ();
        if (ipv4 != 0)
          {
            int32_t interfaceId = ipv4->GetInterfaceForDevice (link.GetToNetDevice ());
            NS_ASSERT (interfaceId >= 0);

            ipv4->SetMetric (interfaceId,metric);
          }
        
        Ptr<ndn::L3Protocol> ndn = link.GetToNode ()->GetObject<ndn::L3Protocol> ();
        if (ndn != 0)
          {
            Ptr<ndn::Face> face = ndn->GetFaceByNetDevice (link.GetToNetDevice ());
            NS_ASSERT (face != 0);
            
            face->SetMetric (metric);
          }
      }
    }
}

void
AnnotatedTopologyReader::ApplySettings ()
{
#ifdef NS3_MPI
  if (MpiInterface::IsEnabled () &&
      MpiInterface::GetSize () != m_requiredPartitions)
    {
      std::cerr << "MPI interface is enabled, but number of partitions (" << MpiInterface::GetSize ()
                << ") is not equal to number of partitions in the topology (" << m_requiredPartitions << ")";
      exit (-1);
    }
#endif
  
  PointToPointHelper p2p;

  BOOST_FOREACH (Link &link, m_linksList)
    {
      // cout << "Link: " << Findlink.GetFromNode () << ", " << link.GetToNode () << endl;
      string tmp;

      if (link.GetAttributeFailSafe ("DataRate", tmp))
        {
          NS_LOG_INFO ("DataRate = " + link.GetAttribute("DataRate"));
          p2p.SetDeviceAttribute ("DataRate", StringValue (link.GetAttribute ("DataRate")));
        }

      if (link.GetAttributeFailSafe ("Delay", tmp))
        {
          NS_LOG_INFO ("Delay = " + link.GetAttribute("Delay"));
          p2p.SetChannelAttribute ("Delay", StringValue (link.GetAttribute ("Delay")));
        }

      NetDeviceContainer nd = p2p.Install(link.GetFromNode (), link.GetToNode ());
      link.SetNetDevices (nd.Get (0), nd.Get (1));

      if (link.GetAttributeFailSafe ("MaxPackets", tmp))
        {
          NS_LOG_INFO ("MaxPackets = " + link.GetAttribute ("MaxPackets"));

          PointerValue txQueue;

          link.GetToNetDevice ()->GetAttribute ("TxQueue", txQueue);
          NS_ASSERT (txQueue.Get<DropTailQueue> () != 0);       
          txQueue.Get<DropTailQueue> ()->SetAttribute ("MaxPackets", StringValue (link.GetAttribute ("MaxPackets")));

          link.GetFromNetDevice ()->GetAttribute ("TxQueue", txQueue);
          NS_ASSERT (txQueue.Get<DropTailQueue> () != 0);
          txQueue.Get<DropTailQueue> ()->SetAttribute ("MaxPackets", StringValue (link.GetAttribute ("MaxPackets")));
        }
        
    }
}

void
AnnotatedTopologyReader::SaveTopology (const std::string &file) const
{
  ofstream os (file.c_str (), ios::trunc);
  os << "# any empty lines and lines starting with '#' symbol is ignored\n"
     << "\n"
     << "# The file should contain exactly two sections: router and link, each starting with the corresponding keyword\n"
     << "\n" 
     << "# router section defines topology nodes and their relative positions (e.g., to use in visualizer)\n"
     << "router\n"
     << "\n"
     << "# each line in this section represents one router and should have the following data\n"
     << "# node  comment     yPos    xPos\n";
  
  for (NodeContainer::Iterator node = m_nodes.Begin ();
       node != m_nodes.End ();
       node++)
    {
      std::string name = Names::FindName (*node);
      Ptr<MobilityModel> mobility = (*node)->GetObject<MobilityModel> ();
      Vector position = mobility->GetPosition ();

      os << name << "\t" << "NA" << "\t" << -position.y << "\t" << position.x << "\n";
    }

  os << "# link section defines point-to-point links between nodes and characteristics of these links\n"
     << "\n"
     << "link\n"
     << "\n"
     << "# Each line should be in the following format (only first two are required, the rest can be omitted)\n"
     << "# srcNode   dstNode     bandwidth   metric  delay   queue\n"
     << "# bandwidth: link bandwidth\n"
     << "# metric: routing metric\n"
     << "# delay:  link delay\n"
     << "# queue:  MaxPackets for transmission queue on the link (both directions)\n";

  for (std::list<Link>::const_iterator link = m_linksList.begin ();
       link != m_linksList.end ();
       link ++)
    {
      os << Names::FindName (link->GetFromNode ()) << "\t";
      os << Names::FindName (link->GetToNode ()) << "\t";

      string tmp;
      if (link->GetAttributeFailSafe ("DataRate", tmp))
        os << link->GetAttribute("DataRate") << "\t";
      else
        NS_FATAL_ERROR ("DataRate must be specified for the link");

      if (link->GetAttributeFailSafe ("OSPF", tmp))
        os << link->GetAttribute("OSPF") << "\t";
      else
        os << "1\t";

      if (link->GetAttributeFailSafe ("Delay", tmp))
        {
          os << link->GetAttribute("Delay") << "\t";

          if (link->GetAttributeFailSafe ("MaxPackets", tmp))
            {
              os << link->GetAttribute("MaxPackets") << "\t";
            }
        }
      os << "\n";
    }  
}

}
