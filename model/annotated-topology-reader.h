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

namespace ns3 
{
    
/**
* \brief This class reads annotated topology and apply settings to the corresponding nodes and links
* Input File Format
* 1st line is     NumberOfNodes    TAB     NumberofLinks
* Nth line is     NodeID1  TAB    NodeID2   TAB  DataRateKBPS TAB OSPF   TAB    DelayMiliseconds   TAB   QueueSizeInPacketsNode1     TAB    QueueSizeInPacketsNode2    TAB    X-coordinate-node1     TAB    Y-coordinate-node1  TAB    X-coordinate-node2     TAB    Y-coordinate-node2
*
*/
class AnnotatedTopologyReader : public TopologyReader
{
public:
    typedef std::list< Link >::iterator LinksIterator;
    static TypeId GetTypeId (void);
        
  /**
   * \brief Constructor
   *
   * \param path ns3::Names path
   *
   * \see ns3::Names class
   */
  AnnotatedTopologyReader (const std::string &path="");
    virtual ~AnnotatedTopologyReader ();
        
    /**
    * \brief Main annotated topology reading function.
    *
    * This method opens an input stream and reads topology file with annotations.
    *
    * \return the container of the nodes created (or empty container if there was an error)
    */
  virtual
  NodeContainer Read (void);
    
    /**
   * \brief Set bounding box for the nodes
   */
  void
  SetBoundingBox (double ulx, double uly, double lrx, double lry);

  /**
   * \brief Assign IPv4 addresses to all links
   *
   * Note, IPv4 stack should be installed on all nodes prior this call
   *
   * Every link will receive /24 netmask
   *
   * \param base Starting IPv4 address (second link will have base+256)
   */
  void
  AssignIpv4Addresses (Ipv4Address base);
  
private:
  /**
     * \brief This method applies setting to corresponding nodes and links
     * NetDeviceContainer must be allocated
     * NodeContainer from Read method
     */
  void ApplySettings ();
  void ApplyOspfMetric ();
    
private:
    AnnotatedTopologyReader (const AnnotatedTopologyReader&);
    AnnotatedTopologyReader& operator= (const AnnotatedTopologyReader&);

  std::string m_path;
  double m_ulx;
  double m_uly;
  double m_lrx;
  double m_lry;
};
}


#endif


