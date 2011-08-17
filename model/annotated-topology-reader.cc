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
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "ns3/log.h"

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
        
        /*for (int i = 0; i <= totnode; i++)
        {
            getline (topgen,line);
        }*/
        
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
    
} /* namespace ns3 */

