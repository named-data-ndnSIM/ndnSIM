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

#include "rocketfuel-weights-reader.hpp"

#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/net-device-container.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include "ns3/uinteger.h"
#include "ns3/ipv4-address.h"

#include "ns3/mobility-model.h"

#include <regex.h>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <iomanip>
#include <set>

using namespace std;

NS_LOG_COMPONENT_DEFINE("RocketfuelWeightsReader");

namespace ns3 {

RocketfuelWeightsReader::RocketfuelWeightsReader(const std::string& path /*=""*/,
                                                 double scale /*=1.0*/)
  : AnnotatedTopologyReader(path, scale)
  , m_defaultBandwidth("100Mbps")
{
  NS_LOG_FUNCTION(this);

  // TypeId tid;
  // bool ok = TypeId::LookupByNameFailSafe ("ns3::SpringMobilityModel", &tid);
  // if (ok)
  //   SetMobilityModel ("ns3::SpringMobilityModel");
  // else
  //   Use default mobility model (supplied by AnnotatedTopologyReader)
}

RocketfuelWeightsReader::~RocketfuelWeightsReader()
{
  NS_LOG_FUNCTION(this);
}

void
RocketfuelWeightsReader::SetFileType(uint8_t inputType)
{
  m_inputType = inputType;
}

NodeContainer
RocketfuelWeightsReader::Read()
{
  if (m_inputType == POSITIONS)
    return AnnotatedTopologyReader::Read();

  ifstream topgen;
  topgen.open(GetFileName().c_str());

  if (!topgen.is_open()) {
    NS_LOG_ERROR("Cannot open file " << GetFileName() << " for reading");
    return m_nodes;
  }

  map<string, set<string>> processedLinks; // to eliminate duplications
  bool repeatedRun = LinksSize() > 0;
  std::list<Link>::iterator linkIterator = m_linksList.begin();

  while (!topgen.eof()) {
    string line;
    getline(topgen, line);
    if (line == "")
      continue;
    if (line[0] == '#')
      continue; // comments

    // NS_LOG_DEBUG ("Input: [" << line << "]");

    istringstream lineBuffer(line);
    string from, to, attribute;

    lineBuffer >> from >> to >> attribute;

    if (processedLinks[to].size() != 0
        && processedLinks[to].find(from) != processedLinks[to].end()) {
      continue; // duplicated link
    }
    processedLinks[from].insert(to);

    Ptr<Node> fromNode = Names::Find<Node>(m_path, from);
    if (fromNode == 0) {
      fromNode = CreateNode(from, 0);
    }

    Ptr<Node> toNode = Names::Find<Node>(m_path, to);
    if (toNode == 0) {
      toNode = CreateNode(to, 0);
    }

    Link* link;
    if (!repeatedRun)
      link = new Link(fromNode, from, toNode, to);
    else {
      NS_ASSERT(linkIterator != m_linksList.end());
      link = &(*linkIterator);

      linkIterator++;
    }

    switch (m_inputType) {
    case LINKS: {
      // links only
      // do nothing
      break;
    }
    case WEIGHTS: {
      if (attribute == "")
        attribute = "1";
      uint16_t metric = boost::lexical_cast<uint16_t>(attribute);
      link->SetAttribute("OSPF", boost::lexical_cast<string>(metric));
      break;
    }
    case LATENCIES:
      if (attribute == "")
        attribute = "1";

      link->SetAttribute("DataRate", m_defaultBandwidth);
      link->SetAttribute("Delay", attribute + "ms");
      if (!m_queue.empty()) {
        link->SetAttribute("MaxPackets", m_queue);
      }
      break;
    default:
      ; //
    }

    NS_LOG_DEBUG("Link " << from << " <==> " << to << " / " << attribute);
    if (!repeatedRun) {
      AddLink(*link);
      delete link;
    }
  }

  topgen.close();

  if (!repeatedRun) {
    NS_LOG_INFO("Rocketfuel topology created with " << m_nodes.GetN() << " nodes and "
                                                    << LinksSize() << " links");
  }
  return m_nodes;
}

void
RocketfuelWeightsReader::Commit()
{
  ApplySettings();

  // SpringMobilityHelper::InstallSprings (LinksBegin (), LinksEnd ());
}

} /* namespace ns3 */
