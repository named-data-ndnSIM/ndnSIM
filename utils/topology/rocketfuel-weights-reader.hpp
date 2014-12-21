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

#ifndef ROCKETFUEL_TOPOLOGY_WEIGHTS_READER_H
#define ROCKETFUEL_TOPOLOGY_WEIGHTS_READER_H

#include "ns3/ndnSIM/utils/topology/annotated-topology-reader.hpp"
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
class RocketfuelWeightsReader : public AnnotatedTopologyReader {
public:
  RocketfuelWeightsReader(const std::string& path = "", double scale = 1.0);
  virtual ~RocketfuelWeightsReader();

  void
  SetFileType(uint8_t inputType);

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
  Read(void);

  void
  Commit();

  enum { LINKS, WEIGHTS, LATENCIES, POSITIONS };

  inline void
  SetDefaultBandwidth(const std::string& bw);

  inline std::string
  GetDefaultBandwidth() const;

  inline void
  SetDefaultQueue(const std::string& queue);

  inline std::string
  GetDefaultQueue() const;

private:
  RocketfuelWeightsReader(const RocketfuelWeightsReader&);
  RocketfuelWeightsReader&
  operator=(const RocketfuelWeightsReader&);

private:
  uint8_t m_inputType;
  std::string m_defaultBandwidth; // since the topology files don't provide bandwidth parameter
  std::string m_queue;

}; // end class RocketfuelWeightsReader

inline void
RocketfuelWeightsReader::SetDefaultBandwidth(const std::string& bw)
{
  m_defaultBandwidth = bw;
}

inline std::string
RocketfuelWeightsReader::GetDefaultBandwidth() const
{
  return m_defaultBandwidth;
}

inline void
RocketfuelWeightsReader::SetDefaultQueue(const std::string& queue)
{
  m_queue = queue;
}

inline std::string
RocketfuelWeightsReader::GetDefaultQueue() const
{
  return m_queue;
}

}; // end namespace ns3

#endif /* ROCKETFUEL_TOPOLOGY_WEIGHTS_READER_H */
