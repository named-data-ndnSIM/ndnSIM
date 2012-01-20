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
#include "ns3/random-variable.h"
#include "ns3/object-factory.h"

namespace ns3 
{
    
/**
 * \brief This class reads annotated topology and apply settings to the corresponding nodes and links
 *
 */
class AnnotatedTopologyReader : public TopologyReader
{
public:
  /**
   * \brief Constructor
   *
   * \param path ns3::Names path
   * \param scale Scaling factor for coordinates in input file
   *
   * \see ns3::Names class
   */
  AnnotatedTopologyReader (const std::string &path="", double scale=1.0);
  virtual ~AnnotatedTopologyReader ();
        
  /**
   * \brief Main annotated topology reading function.
   *
   * This method opens an input stream and reads topology file with annotations.
   *
   * \return the container of the nodes created (or empty container if there was an error)
   */
  virtual NodeContainer
  Read ();

  /**
   * \brief Get nodes read by the reader
   */
  NodeContainer
  GetNodes () const;
    
  /**
   * \brief Get links read by the reader
   */  
  const std::list<Link>&
  GetLinks () const;
  
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

  void
  SetBoundingBox (double ulx, double uly, double lrx, double lry);

  void
  SetMobilityModel (const std::string &model);

protected:
  Ptr<Node>
  CreateNode (const std::string name);

  Ptr<Node>
  CreateNode (const std::string name, double posX, double posY);
  
protected:
  /**
   * \brief This method applies setting to corresponding nodes and links
   * NetDeviceContainer must be allocated
   * NodeContainer from Read method
   */
  void ApplySettings ();
  void ApplyOspfMetric ();
    
protected:
  std::string m_path;
  NodeContainer m_nodes;

private:
  AnnotatedTopologyReader (const AnnotatedTopologyReader&);
  AnnotatedTopologyReader& operator= (const AnnotatedTopologyReader&);

  UniformVariable m_randX;
  UniformVariable m_randY;

  ObjectFactory m_mobilityFactory;
  double m_scale;
};

}

#endif


