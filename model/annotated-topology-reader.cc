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
                link.SetAttribute ("OSPF", linkAttr);
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
            
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("X1", linkAttr);
            }
            
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("Y1", linkAttr);
            }
            
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("X2", linkAttr);
            }
            
            lineBuffer >> linkAttr;
            if ( !linkAttr.empty () )
            {
                link.SetAttribute ("Y2", linkAttr);
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
    InternetStackHelper stack;
    Ipv4AddressHelper address;
    address.SetBase ("10.1.0.0", "255.255.255.0");
    
    Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
    stack.SetRoutingHelper (ipv4RoutingHelper);
    
    
    
    //This loop passes all links and checks if ipv4 is installed on the node
    // if not, it installs. 
    // We can't use stack.Install(nc) because in nc there are duplicates and assertion fails
    TopologyReader::ConstLinksIterator iter;
    int j = 0;
    for ( iter = this->LinksBegin (); iter != this->LinksEnd (); iter++, j++ )
    {
        NodeContainer twoNodes = nc[j];
        
        Ptr<Node> nd = twoNodes.Get(0);
        if(nd==NULL)
            NS_LOG_INFO("nd = null");
        
        Ptr<Node> nd2 = twoNodes.Get(1);
        if(nd2==NULL)
            NS_LOG_INFO("nd2 = null");
        
        Ptr<Ipv4> ipv4 = nd->GetObject<Ipv4>();
        if(ipv4 == 0)
        {
            NS_LOG_INFO("ipv4 = null");
            stack.Install(nd);
        }
        
        Ptr<Ipv4> ipv42 = nd2->GetObject<Ipv4>();
        if(ipv42 == 0)
        {
            NS_LOG_INFO("ipv42 = null");
            stack.Install(nd2);
        }
        
        //NS_LOG_INFO("#netdevices = " << nd->GetNDevices());
        //NS_LOG_INFO("#netdevices = " << nd2->GetNDevices());
    }
    
    NS_LOG_INFO("ITER2");
    uint32_t base = 0;
    PointToPointHelper p2p;
    TopologyReader::ConstLinksIterator iter2;
    int i = 0;
    for ( iter2 = this->LinksBegin (); iter2 != this->LinksEnd (); iter2++, i++ )
    {
        p2p.SetDeviceAttribute("DataRate", StringValue(iter2->GetAttribute("DataRate")+"Kbps"));
        NS_LOG_INFO("DataRate = " + iter2->GetAttribute("DataRate")+"Kbps");
        p2p.SetChannelAttribute("Delay", StringValue(iter2->GetAttribute("Delay")+"ms"));
        NS_LOG_INFO("Delay = " + iter2->GetAttribute("Delay")+"ms");
        p2p.SetQueue("ns3::DropTailQueue","MaxPackets",StringValue("100"));
        ndc[i] = p2p.Install(nc[i]);
        
        Ipv4Address address1(base+i*256 + 1);
        Ipv4Address address2(base+i*256 + 2);
        
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
            
        Ptr<NetDevice> device = nd->GetDevice(nd->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
        
        if(device==NULL)
            NS_LOG_INFO("device = 0");
        
        std::string ospf = iter2->GetAttribute("OSPF");
        uint16_t metric = atoi(ospf.c_str());
        NS_LOG_INFO("OSPF metric = " << metric);
        
        {
        NetDeviceContainer* temp = new NetDeviceContainer[1];
        temp->Add(device);
        address.Assign (*temp);
        }
        
        Ptr<Ipv4> ipv4 = nd->GetObject<Ipv4>();
        if(ipv4 == 0)
        {
            NS_LOG_INFO("ipv4 = null");
            //stack.Install(nd);
            /*NetDeviceContainer* temp = new NetDeviceContainer[1];
            temp->Add(device);
            address.Assign (*temp);
            ipv4 = nd->GetObject<Ipv4>();*/
        }
        
        NS_LOG_INFO("Before GetID");
        int32_t interfaceId = ipv4->GetInterfaceForDevice(device);
        NS_LOG_INFO("InterfaceID = " << interfaceId);
        ipv4->SetMetric(interfaceId,metric);
        
        
        

        
        /*Ptr<Ipv4> ipv4 = nd->GetObject<Ipv4>();
        
        if(ipv4 == 0)
            NS_LOG_INFO("ipv4 = null");
        int32_t interfaceId = ipv4->GetInterfaceForDevice(device);
        ipv4->SetMetric(interfaceId,metric);*/
        
        //Ptr<Ipv4Interface> interface = nd->GetDevice(nd->GetNDevices()-1)->GetObject<Ipv4Interface> ();
        //ipv4->SetMetric(metric);
            
        //NS_LOG_INFO("2");
            
        Ptr<NetDevice> device2 = nd2->GetDevice(nd2->GetNDevices()-1)->GetObject<PointToPointNetDevice> ();
            
        if(device2==NULL)
            NS_LOG_INFO("device2 = 0");
            
        {
            NetDeviceContainer* temp = new NetDeviceContainer[1];
            temp->Add(device2);
            address.Assign (*temp);
        }
        
        Ptr<Ipv4> ipv42 = nd2->GetObject<Ipv4>();
        if(ipv42 == 0)
        {
            NS_LOG_INFO("ipv42 = null");
            /*stack.Install(nd2);
            NetDeviceContainer* temp = new NetDeviceContainer[1];
            temp->Add(device2);
            address.Assign (*temp);
            ipv42 = nd2->GetObject<Ipv4>();*/
        }
        
        NS_LOG_INFO("Before GetID");
        interfaceId = ipv42->GetInterfaceForDevice(device2);
        NS_LOG_INFO("InterfaceID = " << interfaceId);
        ipv42->SetMetric(interfaceId,metric);

        
        
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

    /*
void
AnnotatedTopologyReader::ApplyOspfMetric(NetDeviceContainer* ndc, NodeContainer* nc)
{
    InternetStackHelper stack;
    Ipv4AddressHelper address;
    address.SetBase ("10.0.0.0", "255.255.255.252");
    
    Ipv4GlobalRoutingHelper ipv4RoutingHelper ("ns3::Ipv4GlobalRoutingOrderedNexthops");
    stack.SetRoutingHelper (ipv4RoutingHelper);

    
    TopologyReader::ConstLinksIterator iter2;
    int i = 0;
    for ( iter2 = this->LinksBegin (); iter2 != this->LinksEnd (); iter2++, i++ )
    {
        NodeContainer twoNodes = nc[i];
        Ptr<NetDevice> device = ndc[i].Get(0);
        Ptr<NetDevice> device2 = ndc[i].Get(1);
        
        //Ptr<Node> nd = twoNodes.Get(0);
        Ptr<Node> nd = device->GetNode();
        if(nd==NULL)
            NS_LOG_INFO("nd = null");
        
        //Ptr<Node> nd2 = twoNodes.Get(1);
        Ptr<Node> nd2 = device->GetNode();
        if(nd2==NULL)
            NS_LOG_INFO("nd2 = null");

        
        
        std::string ospf = iter2->GetAttribute("OSPF");
        uint16_t metric = atoi(ospf.c_str());
        NS_LOG_INFO("OSPF metric = " << metric);
        
        Ptr<Ipv4> ipv4 = nd->GetObject<Ipv4>();
        
        if(ipv4 == 0)
        {
            NS_LOG_INFO("ipv4 = null");
            stack.Install(nd);
            NetDeviceContainer* temp = new NetDeviceContainer[1];
            temp->Add(device);
            address.Assign (*temp);
            ipv4 = nd->GetObject<Ipv4>();
        }
        
        NS_LOG_INFO("Before GetID");
        int32_t interfaceId = ipv4->GetInterfaceForDevice(device);
        NS_LOG_INFO("InterfaceID = " << interfaceId);
        ipv4->SetMetric(interfaceId,metric);
        
        
        
        Ptr<Ipv4> ipv42 = nd2->GetObject<Ipv4>();
        if(ipv42 == 0)
        {
            NS_LOG_INFO("ipv42 = null");
            stack.Install(nd2);
            NetDeviceContainer* temp = new NetDeviceContainer[1];
            temp->Add(device2);
            address.Assign (*temp);
            ipv42 = nd2->GetObject<Ipv4>();
        }

        //if(ipv4 == 0)
        //    NS_LOG_INFO("ipv4 = null");
        
        NS_LOG_INFO("Before GetID");
        interfaceId = ipv42->GetInterfaceForDevice(device2);
        if(interfaceId == -1)
        {
            NS_LOG_INFO("interfaceID = -1");
            stack.Install(nd2);
            NetDeviceContainer* temp = new NetDeviceContainer[1];
            temp->Add(device2);
            address.Assign (*temp);
            ipv42 = nd2->GetObject<Ipv4>();
            interfaceId = ipv42->GetInterfaceForDevice(device2);
        }
        NS_LOG_INFO("InterfaceID = " << interfaceId);
        ipv42->SetMetric(interfaceId,metric);

    }
}*/

void
AnnotatedTopologyReader::BoundingBox (NodeContainer* nc, double ulx, double uly, double lrx, double lry)
{
    
    UniformVariable randX(ulx, lrx);
    double x = 0.0;
    UniformVariable randY(uly, lry);
    double y = 0.0;


    PointToPointHelper p2p;
    TopologyReader::ConstLinksIterator iter2;
    int i = 0;
    for ( iter2 = this->LinksBegin (); iter2 != this->LinksEnd (); iter2++, i++ )
    {
        std::string x1str = iter2->GetAttribute("X1");
        uint16_t x1 = atoi(x1str.c_str());
        
        std::string x2str = iter2->GetAttribute("X2");
        uint16_t x2 = atoi(x2str.c_str());
        
        
        std::string y1str = iter2->GetAttribute("Y1");
        uint16_t y1 = atoi(y1str.c_str());
        
        std::string y2str = iter2->GetAttribute("Y2");
        uint16_t y2 = atoi(y2str.c_str());
        
        NodeContainer twoNodes = nc[i];
        
        Ptr<Node> nd = twoNodes.Get(0);
        if(nd==NULL)
            NS_LOG_INFO("nd = null");
        
        Ptr<Node> nd2 = twoNodes.Get(1);
        if(nd2==NULL)
            NS_LOG_INFO("nd2 = null");
        
        Ptr<ConstantPositionMobilityModel> loc = nd->GetObject<ConstantPositionMobilityModel> ();
        if (loc ==0)
        {
            loc = CreateObject<ConstantPositionMobilityModel> ();
            nd->AggregateObject (loc);
        }
        
        x = x1; //randX.GetValue();
        y = y1; //randY.GetValue();
        NS_LOG_INFO("X = "<<x <<"Y = "<<y);
        Vector locVec (x, y, 0);
        loc->SetPosition (locVec);
        
        
        Ptr<ConstantPositionMobilityModel> loc2 = nd2->GetObject<ConstantPositionMobilityModel> ();
        if (loc2 ==0)
        {
            loc2 = CreateObject<ConstantPositionMobilityModel> ();
            nd2->AggregateObject (loc2);
        }
        
        x = x2; //randX.GetValue();
        y = y2; //randY.GetValue();
        NS_LOG_INFO("X = "<<x <<"Y = "<<y);
        Vector locVec2 (x, y, 0);
        loc2->SetPosition (locVec2);
    }
}
}

