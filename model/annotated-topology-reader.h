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

#ifndef __ANNOTATED_TOPOLOGY_READER_H__
#define __ANNOTATED_TOPOLOGY_READER_H__

#include "ns3/topology-reader.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/net-device-container.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace ns3 
{
    
/**
* \brief This class reads annotated topology and apply settings to the corresponding nodes and links
* Input File Format
* 1st line is     NumberOfNodes    TAB     NumberofLinks
* Nth line is     NodeID1  TAB    NodeID2   TAB  DataRateKBPS    TAB    DelayMiliseconds   TAB   QueueSizeInPacketsNode1     TAB    QueueSizeInPacketsNode2 
*
*/
class AnnotatedTopologyReader : public TopologyReader
{
public:
    typedef std::list< Link >::iterator LinksIterator;
    static TypeId GetTypeId (void);
        
    AnnotatedTopologyReader ();
    virtual ~AnnotatedTopologyReader ();
        
        
    /**
    * \brief Main annotated topology reading function.
    *
    * This method opens an input stream and reads topology file with annotations.
    *
    * \return the container of the nodes created (or empty container if there was an error)
    */
    virtual NodeContainer Read (void);
    
    /**
     * \brief This method applies setting to corresponding nodes and links
     * NetDeviceContainer must be allocated
     * NodeContainer from Read method
     */
    void ApplySettings(NetDeviceContainer *ndc, NodeContainer* nc);
        
private:
    AnnotatedTopologyReader (const AnnotatedTopologyReader&);
    AnnotatedTopologyReader& operator= (const AnnotatedTopologyReader&);
};
}


#endif


