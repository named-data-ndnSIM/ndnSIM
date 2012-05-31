/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2007 INRIA
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */
#ifndef SPRING_MOBILITY_HELPER_H
#define SPRING_MOBILITY_HELPER_H

#include "ns3/topology-reader.h"

namespace ns3 {

/**
 * \ingroup mobility
 *
 * \brief 
 */
class SpringMobilityHelper
{
public:
  static void
  InstallSprings (Ptr<Node> node1, Ptr<Node> node2);

  static void
  InstallSprings (TopologyReader::ConstLinksIterator first, 
                  TopologyReader::ConstLinksIterator end);
};

} // namespace ns3

#endif // SPRING_MOBILITY_HELPER_H
