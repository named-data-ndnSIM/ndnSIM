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

#include "ns3/constant-position-mobility-model.h"
#include "ns3/random-variable.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace ns3 
{    

NS_LOG_COMPONENT_DEFINE ("AnnotatedTopologyReader");
    
NS_OBJECT_ENSURE_REGISTERED (AnnotatedTopologyReader);
    
TypeId AnnotatedTopologyReader::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::AnnotatedTopologyReader")
    .SetParent<Object> ()
    ;
    return tid;
}
    
AnnotatedTopologyReader::AnnotatedTopologyReader (const std::string &path)
  : m_path (path)
  , m_ulx (0)
  , m_uly (0)
  , m_lrx (100.0)
  , m_lry (100.0)
{
    NS_LOG_FUNCTION (this);
}
    
void
AnnotatedTopologyReader::SetBoundingBox (double ulx, double uly, double lrx, double lry)
{
  NS_LOG_FUNCTION (this << ulx << uly << lrx << lry);
  
  m_ulx = ulx;
  m_uly = uly;
  m_lrx = lrx;
  m_lry = lry;
}

AnnotatedTopologyReader::~AnnotatedTopologyReader ()
{
    NS_LOG_FUNCTION (this);
}
    
NodeContainer
AnnotatedTopologyReader::Read (void)
{
    ifstream topgen;
    topgen.open (GetFileName ().c_str ());
    NodeContainer nodes;
        
    if ( !topgen.is_open () )
    {
      NS_LOG_ERROR ("Cannot open file " << GetFileName () << " for reading");
        return nodes;
    }
        
    int linksNumber = 0;
    int nodesNumber = 0;
        
    string line;
    getline (topgen,line);
  istringstream headerLineBuffer (line);
        
  int totnode;
  int totlink;
  headerLineBuffer >> totnode;
  headerLineBuffer >> totlink;
  
    NS_LOG_INFO ("Annotated topology should have " << totnode << " nodes and " << totlink << " links");
        
    for (int i = 0; i < totlink && !topgen.eof (); i++)
    {
        getline (topgen,line);
      istringstream lineBuffer (line);
            
      string from;
      string to;
        lineBuffer >> from;
        lineBuffer >> to;
            
        if ( (!from.empty ()) && (!to.empty ()) )
        {
            NS_LOG_INFO ( linksNumber << " From: " << from << " to: " << to );
                
          Ptr<Node> fromNode = Names::Find<Node> (m_path, from);
          Ptr<Node> toNode   = Names::Find<Node> (m_path, to);
          
          if (fromNode == 0)
            {
              fromNode = CreateObject<Node> ();
              Names::Add (m_path, from, fromNode);
              nodes.Add (fromNode);
                nodesNumber++;
            }
                
          if (toNode == 0)
            {
              toNode = CreateObject<Node> ();
              Names::Add (m_path, to, toNode);
              nodes.Add (toNode);
                nodesNumber++;
            }
                
          Link link (fromNode, from, toNode, to);
                
          string dataRate;
          lineBuffer >> dataRate;
            
          string ospf;
          lineBuffer >> ospf;
                
          string delay;
          lineBuffer >> delay;
                
          string queueSizeNode1;
          lineBuffer >> queueSizeNode1;
                
          string queueSizeNode2;
          lineBuffer >> queueSizeNode2;

          if (dataRate.empty () ||
              ospf.empty () ||
              delay.empty () ||
              queueSizeNode1.empty () ||
              queueSizeNode2.empty ())
            {
              NS_LOG_ERROR ("File [" << GetFileName () << ":" << i+2 << " wrong format, skipping");
              continue;
            }
            
          link.SetAttribute ("DataRate", dataRate);
          link.SetAttribute ("OSPF", ospf);
          link.SetAttribute ("Delay", delay);
          link.SetAttribute ("QueueSizeNode1", queueSizeNode1);
          link.SetAttribute ("QueueSizeNode2", queueSizeNode2);
                
            AddLink (link);
                
            linksNumber++;
        }
    }
        
  NS_ASSERT (nodesNumber == totnode && linksNumber == totlink);
        
    NS_LOG_INFO ("Annotated topology created with " << nodesNumber << " nodes and " << linksNumber << " links");
    topgen.close ();
        
  ApplySettings ();
  AssignCoordinates ();
  
    return nodes;
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
        
  ApplyOspfMetric ();
        }
        
void
AnnotatedTopologyReader::ApplyOspfMetric ()
    {
  BOOST_FOREACH (const Link &link, m_linksList)
        {
      uint16_t metric = boost::lexical_cast<uint16_t> (link.GetAttribute ("OSPF"));
        
        {
        Ptr<Ipv4> ipv4 = link.GetFromNode ()->GetObject<Ipv4> ();
        NS_ASSERT (ipv4 != 0);
        
        int32_t interfaceId = ipv4->GetInterfaceForDevice (link.GetFromNetDevice ());
        NS_ASSERT (interfaceId >= 0);
        
        ipv4->SetMetric (interfaceId,metric);
        }
        
        {
        Ptr<Ipv4> ipv4 = link.GetToNode ()->GetObject<Ipv4> ();
        NS_ASSERT (ipv4 != 0);
        
        int32_t interfaceId = ipv4->GetInterfaceForDevice (link.GetToNetDevice ());
        NS_ASSERT (interfaceId >= 0);

        ipv4->SetMetric (interfaceId,metric);
    }
}
}

void
AnnotatedTopologyReader::ApplySettings ()
{
  PointToPointHelper p2p;
    
  // temporary queue, will be changed later
  p2p.SetQueue ("ns3::DropTailQueue",
                "MaxPackets", StringValue("100"));

  BOOST_FOREACH (Link &link, m_linksList)
    {
      NS_LOG_INFO ("DataRate = " + link.GetAttribute("DataRate")+"Kbps");
      p2p.SetDeviceAttribute ("DataRate", StringValue(link.GetAttribute("DataRate")+"Kbps"));
        
      NS_LOG_INFO ("Delay = " + link.GetAttribute("Delay")+"ms");
      p2p.SetChannelAttribute ("Delay", StringValue(link.GetAttribute("Delay")+"ms"));
        
      NetDeviceContainer nd = p2p.Install(link.GetFromNode (), link.GetToNode ());
      link.SetNetDevices (nd.Get (0), nd.Get (1));

      NS_LOG_INFO ("Queue: " << link.GetAttribute("QueueSizeNode1") << " <==> " << link.GetAttribute("QueueSizeNode2"));
        
      PointerValue txQueueFrom;
      link.GetFromNetDevice ()->GetAttribute ("TxQueue", txQueueFrom);
      NS_ASSERT (txQueueFrom.Get<DropTailQueue> () != 0);
        
      PointerValue txQueueTo;
      link.GetToNetDevice ()->GetAttribute ("TxQueue", txQueueTo);
      NS_ASSERT (txQueueTo.Get<DropTailQueue> () != 0);
        
      txQueueFrom.Get<DropTailQueue> ()->SetAttribute ("MaxPackets", StringValue (link.GetAttribute("QueueSizeNode1")));
      txQueueTo.  Get<DropTailQueue> ()->SetAttribute ("MaxPackets", StringValue (link.GetAttribute("QueueSizeNode2")));
        }
        }

void
AnnotatedTopologyReader::AssignCoordinates ()
{
  UniformVariable randX (m_ulx, m_lrx);
    double x = 0.0;
  UniformVariable randY (m_uly, m_lry);
    double y = 0.0;

  BOOST_FOREACH (Link &link, m_linksList)
    {
      Ptr<ConstantPositionMobilityModel> loc = link.GetFromNode ()->GetObject<ConstantPositionMobilityModel> ();
      if (loc != 0)
        continue; // no need to assign twice
        
            loc = CreateObject<ConstantPositionMobilityModel> ();
      link.GetFromNode ()->AggregateObject (loc);
        
      x = randX.GetValue();
      y = randY.GetValue();
        NS_LOG_INFO("X = "<<x <<"Y = "<<y);
        
      loc->SetPosition (Vector (x, y, 0));
        }
    }

}
