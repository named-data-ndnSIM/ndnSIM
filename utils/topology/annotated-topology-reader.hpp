/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// Based on the code by Hajime Tazaki <tazaki@sfc.wide.ad.jp>

#ifndef __ANNOTATED_TOPOLOGY_READER_H__
#define __ANNOTATED_TOPOLOGY_READER_H__

#include "ns3/topology-reader.h"
#include "ns3/random-variable-stream.h"
#include "ns3/object-factory.h"

namespace ns3 {

/**
 * \brief This class reads annotated topology and apply settings to the corresponding nodes and
 *links
 *
 */
class AnnotatedTopologyReader : public TopologyReader {
public:
  /**
   * \brief Constructor
   *
   * \param path ns3::Names path
   * \param scale Scaling factor for coordinates in input file
   *
   * \see ns3::Names class
   */
  AnnotatedTopologyReader(const std::string& path = "", double scale = 1.0);
  virtual ~AnnotatedTopologyReader();

  /**
   * \brief Main annotated topology reading function.
   *
   * This method opens an input stream and reads topology file with annotations.
   *
   * \return the container of the nodes created (or empty container if there was an error)
   */
  virtual NodeContainer
  Read();

  /**
   * \brief Get nodes read by the reader
   */
  virtual NodeContainer
  GetNodes() const;

  /**
   * \brief Get links read by the reader
   */
  virtual const std::list<Link>&
  GetLinks() const;

  /**
   * \brief Assign IPv4 addresses to all links
   *
   * Note, IPv4 stack should be installed on all nodes prior this call
   *
   * Every link will receive /24 netmask
   *
   * \param base Starting IPv4 address (second link will have base+256)
   */
  virtual void
  AssignIpv4Addresses(Ipv4Address base);

  /**
   * \brief Set bounding box where nodes will be randomly places (if positions are unspecified)
   * \param ulx Upper left x coordinate
   * \param uly Upper left y coordinate
   * \param lrx Lower right x coordinate
   * \param lry Lower right y coordinate
   */
  virtual void
  SetBoundingBox(double ulx, double uly, double lrx, double lry);

  /**
   * \brief Set mobility model to be used on nodes
   * \param model class name of the model
   */
  virtual void
  SetMobilityModel(const std::string& model);

  /**
   * \brief Apply OSPF metric on Ipv4 (if exists) and Ccnx (if exists) stacks
   */
  virtual void
  ApplyOspfMetric();

  /**
   * \brief Save positions (e.g., after manual modification using visualizer)
   */
  virtual void
  SaveTopology(const std::string& file);

  /**
   * \brief Save topology in graphviz format (.dot file)
   */
  virtual void
  SaveGraphviz(const std::string& file);

protected:
  Ptr<Node>
  CreateNode(const std::string name, uint32_t systemId);

  Ptr<Node>
  CreateNode(const std::string name, double posX, double posY, uint32_t systemId);

protected:
  /**
   * \brief This method applies setting to corresponding nodes and links
   * NetDeviceContainer must be allocated
   * NodeContainer from Read method
   */
  void
  ApplySettings();

protected:
  std::string m_path;
  NodeContainer m_nodes;

private:
  AnnotatedTopologyReader(const AnnotatedTopologyReader&);
  AnnotatedTopologyReader&
  operator=(const AnnotatedTopologyReader&);

  Ptr<UniformRandomVariable> m_randX;
  Ptr<UniformRandomVariable> m_randY;

  ObjectFactory m_mobilityFactory;
  double m_scale;

  uint32_t m_requiredPartitions;
};
}

#endif
