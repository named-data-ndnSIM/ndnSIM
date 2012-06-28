/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hajime Tazaki
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
 * Author: Hajime Tazaki (tazaki@sfc.wide.ad.jp)
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef ROCKETFUEL_TOPOLOGY_WEIGHTS_READER_H
#define ROCKETFUEL_TOPOLOGY_WEIGHTS_READER_H

#include "annotated-topology-reader.h"
#include "ns3/net-device-container.h"

namespace ns3 {
    
// ------------------------------------------------------------
// --------------------------------------------
/**
 * \brief Topology file reader (extension of Rocketfuel-format type).
 *
 * http://www.cs.washington.edu/research/networking/rocketfuel/
 *
 * Only weights and latencies file is supported
 */
class RocketfuelWeightsReader : public AnnotatedTopologyReader
{
public:
  RocketfuelWeightsReader (const std::string &path="", double scale=1.0);
  virtual ~RocketfuelWeightsReader ();

  void
  SetFileType (uint8_t inputType);
  
  /**
   * \brief Main topology reading function.
   *
   * This method opens an input stream and reads the Rocketfuel-format file.
   * Every row represents a topology link (the ids of a couple of nodes),
   * so the input file is read line by line to figure out how many links
   * and nodes are in the topology.
   *
   * \return the container of the nodes created (or empty container if there was an error)
   */
  virtual NodeContainer 
  Read (void);

  void
  Commit ();

  enum
    {
      LINKS,
      WEIGHTS,
      LATENCIES,
      POSITIONS
    };
  
private:
  RocketfuelWeightsReader (const RocketfuelWeightsReader&);
  RocketfuelWeightsReader& operator= (const RocketfuelWeightsReader&);
  
private:
  uint8_t m_inputType;
  
}; // end class RocketfuelWeightsReader

}; // end namespace ns3


#endif /* ROCKETFUEL_TOPOLOGY_WEIGHTS_READER_H */
