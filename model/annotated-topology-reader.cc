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
    
AnnotatedTopologyReader::AnnotatedTopologyReader ()
{
    NS_LOG_FUNCTION (this);
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
    map<string, Ptr<Node> > nodeMap;
    NodeContainer nodes;
        
    if ( !topgen.is_open () )
    {
        return nodes;
    }
        
    string from;
    string to;
    string linkAttr;
        
    int linksNumber = 0;
    int nodesNumber = 0;
        
    int totnode = 0;
    int totlink = 0;
        
    istringstream lineBuffer;
    string line;
        
    getline (topgen,line);
    lineBuffer.str (line);
        
    lineBuffer >> totnode;
    lineBuffer >> totlink;
    NS_LOG_INFO ("Annotated topology should have " << totnode << " nodes and " << totlink << " links");
        
    if(!topgen.eof ())
        NS_LOG_INFO("!EOF");

        
    for (int i = 0; i < totlink && !topgen.eof (); i++)
    {
        //NS_LOG_INFO("Line #" <<i);
        getline (topgen,line);
        lineBuffer.clear ();
        lineBuffer.str (line);
            
        lineBuffer >> from;
        lineBuffer >> to;
            
            
        if ( (!from.empty ()) && (!to.empty ()) )
        {
            NS_LOG_INFO ( linksNumber << " From: " << from << " to: " << to );
                
            if ( nodeMap[from] == 0 )
            {
                Ptr<Node> tmpNode = CreateObject<Node> ();
                nodeMap[from] = tmpNode;
                nodes.Add (tmpNode);
                nodesNumber++;
            }
                
            if (nodeMap[to] == 0)
            {
                Ptr<Node> tmpNode = CreateObject<Node> ();
                nodeMap[to] = tmpNode;
                nodes.Add (tmpNode);
                nodesNumber++;
            }
                
            Link link ( nodeMap[from], from, nodeMap[to], to );
                
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("DataRate", linkAttr);
            }
                
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("Delay", linkAttr);
            }
                
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("QueueSizeNode1", linkAttr);
            }
                
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("QueueSizeNode2", linkAttr);
            }
                
            AddLink (link);
                
            linksNumber++;
        }
    }
        
    NS_LOG_INFO ("Annotated topology created with " << nodesNumber << " nodes and " << linksNumber << " links");
    topgen.close ();
        
    return nodes;
}
    
void
AnnotatedTopologyReader::ApplySettings(NetDeviceContainer* ndc, NodeContainer* nc)
{
    PointToPointHelper p2p;
    TopologyReader::ConstLinksIterator iter2;
    int i = 0;
    for ( iter2 = this->LinksBegin (); iter2 != this->LinksEnd (); iter2++, i++ )
    {
        /*std::string dataRate = iter2->GetAttribute("DataRate");
        NS_LOG_INFO("dataRate = "<<dataRate);
        dataRate += "Kbps";
        std::string delay = iter2->GetAttribute("Delay");
        NS_LOG_INFO("delay = "<<delay);
        delay += "ms";*/
        
        p2p.SetDeviceAttribute("DataRate", StringValue(iter2->GetAttribute("DataRate")+"Kbps"));
        NS_LOG_INFO("DataRate = " + iter2->GetAttribute("DataRate")+"Kbps");
        p2p.SetChannelAttribute("Delay", StringValue(iter2->GetAttribute("Delay")+"ms"));
        NS_LOG_INFO("Delay = " + iter2->GetAttribute("Delay")+"ms");
        p2p.SetQueue("ns3::DropTailQueue","MaxPackets",StringValue("100"));
        ndc[i] = p2p.Install(nc[i]);
        
        
        NodeContainer twoNodes = nc[i];
            
        Ptr<Node> nd = twoNodes.Get(0);
        if(nd==NULL)
            NS_LOG_INFO("nd = null");
        
        Ptr<Node> nd2 = twoNodes.Get(1);
        if(nd2==NULL)
            NS_LOG_INFO("nd2 = null");
        
        //NS_LOG_INFO("1");
        NS_LOG_INFO("#netdevices = " << nd->GetNDevices());
        NS_LOG_INFO("#netdevices = " << nd2->GetNDevices());
            
        Ptr<PointToPointNetDevice> device = nd->GetDevice(nd->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
            
        if(device==NULL)
            NS_LOG_INFO("device = 0");
            
        //NS_LOG_INFO("2");
            
        Ptr<PointToPointNetDevice> device2 = nd2->GetDevice(nd2->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
            
        if(device2==NULL)
            NS_LOG_INFO("device2 = 0");
            
        PointerValue tmp1;
        device->GetAttribute ("TxQueue", tmp1);
        //NS_LOG_INFO("2.5");
        Ptr<Object> txQueue1 = tmp1.GetObject ();
            
        PointerValue tmp2;
        device2->GetAttribute ("TxQueue", tmp2);
        Ptr<Object> txQueue2 = tmp2.GetObject ();
        //NS_LOG_INFO("3");
        Ptr<DropTailQueue> dtq1 = txQueue1->GetObject <DropTailQueue> ();
        NS_ASSERT (dtq1 != 0);
            
        Ptr<DropTailQueue> dtq2 = txQueue2->GetObject <DropTailQueue> ();
        NS_ASSERT (dtq2 != 0);
            
        std::string queuesize1 = iter2->GetAttribute("QueueSizeNode1");
        std::string queuesize2 = iter2->GetAttribute("QueueSizeNode2");
        //NS_LOG_INFO("4");
        txQueue1->SetAttribute("MaxPackets", UintegerValue (atoi(queuesize1.c_str())));
        txQueue2->SetAttribute("MaxPackets", UintegerValue (atoi(queuesize2.c_str())));
            
        UintegerValue limit;
        txQueue1->GetAttribute ("MaxPackets", limit);
        NS_LOG_INFO ("NetDevice #"<< device->GetIfIndex() << "has queue limit " << limit.Get () << " packets");
            
        txQueue2->GetAttribute ("MaxPackets", limit);
        NS_LOG_INFO ("NetDevice #"<< device2->GetIfIndex() << "has queue limit " << limit.Get () << " packets");
    }
}
}

