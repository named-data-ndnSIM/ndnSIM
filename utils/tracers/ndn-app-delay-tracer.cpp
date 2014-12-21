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

#include "ndn-app-delay-tracer.hpp"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/config.h"
#include "ns3/names.h"
#include "ns3/callback.h"

#include "apps/ndn-app.hpp"
#include "ns3/simulator.h"
#include "ns3/node-list.h"
#include "ns3/log.h"

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#include <fstream>

NS_LOG_COMPONENT_DEFINE("ndn.AppDelayTracer");

namespace ns3 {
namespace ndn {

static std::list<std::tuple<shared_ptr<std::ostream>, std::list<Ptr<AppDelayTracer>>>>
  g_tracers;

void
AppDelayTracer::Destroy()
{
  g_tracers.clear();
}

void
AppDelayTracer::InstallAll(const std::string& file)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<AppDelayTracer>> tracers;
  shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    Ptr<AppDelayTracer> trace = Install(*node, outputStream);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

void
AppDelayTracer::Install(const NodeContainer& nodes, const std::string& file)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<AppDelayTracer>> tracers;
  shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  for (NodeContainer::Iterator node = nodes.Begin(); node != nodes.End(); node++) {
    Ptr<AppDelayTracer> trace = Install(*node, outputStream);
    tracers.push_back(trace);
  }

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

void
AppDelayTracer::Install(Ptr<Node> node, const std::string& file)
{
  using namespace boost;
  using namespace std;

  std::list<Ptr<AppDelayTracer>> tracers;
  shared_ptr<std::ostream> outputStream;
  if (file != "-") {
    shared_ptr<std::ofstream> os(new std::ofstream());
    os->open(file.c_str(), std::ios_base::out | std::ios_base::trunc);

    if (!os->is_open()) {
      NS_LOG_ERROR("File " << file << " cannot be opened for writing. Tracing disabled");
      return;
    }

    outputStream = os;
  }
  else {
    outputStream = shared_ptr<std::ostream>(&std::cout, std::bind([]{}));
  }

  Ptr<AppDelayTracer> trace = Install(node, outputStream);
  tracers.push_back(trace);

  if (tracers.size() > 0) {
    // *m_l3RateTrace << "# "; // not necessary for R's read.table
    tracers.front()->PrintHeader(*outputStream);
    *outputStream << "\n";
  }

  g_tracers.push_back(std::make_tuple(outputStream, tracers));
}

Ptr<AppDelayTracer>
AppDelayTracer::Install(Ptr<Node> node, shared_ptr<std::ostream> outputStream)
{
  NS_LOG_DEBUG("Node: " << node->GetId());

  Ptr<AppDelayTracer> trace = Create<AppDelayTracer>(outputStream, node);

  return trace;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

AppDelayTracer::AppDelayTracer(shared_ptr<std::ostream> os, Ptr<Node> node)
  : m_nodePtr(node)
  , m_os(os)
{
  m_node = boost::lexical_cast<std::string>(m_nodePtr->GetId());

  Connect();

  std::string name = Names::FindName(node);
  if (!name.empty()) {
    m_node = name;
  }
}

AppDelayTracer::AppDelayTracer(shared_ptr<std::ostream> os, const std::string& node)
  : m_node(node)
  , m_os(os)
{
  Connect();
}

AppDelayTracer::~AppDelayTracer(){};

void
AppDelayTracer::Connect()
{
  Config::ConnectWithoutContext("/NodeList/" + m_node
                                  + "/ApplicationList/*/LastRetransmittedInterestDataDelay",
                                MakeCallback(&AppDelayTracer::LastRetransmittedInterestDataDelay,
                                             this));

  Config::ConnectWithoutContext("/NodeList/" + m_node + "/ApplicationList/*/FirstInterestDataDelay",
                                MakeCallback(&AppDelayTracer::FirstInterestDataDelay, this));
}

void
AppDelayTracer::PrintHeader(std::ostream& os) const
{
  os << "Time"
     << "\t"
     << "Node"
     << "\t"
     << "AppId"
     << "\t"
     << "SeqNo"
     << "\t"

     << "Type"
     << "\t"
     << "DelayS"
     << "\t"
     << "DelayUS"
     << "\t"
     << "RetxCount"
     << "\t"
     << "HopCount"
     << "";
}

void
AppDelayTracer::LastRetransmittedInterestDataDelay(Ptr<App> app, uint32_t seqno, Time delay,
                                                   int32_t hopCount)
{
  *m_os << Simulator::Now().ToDouble(Time::S) << "\t" << m_node << "\t" << app->GetId() << "\t"
        << seqno << "\t"
        << "LastDelay"
        << "\t" << delay.ToDouble(Time::S) << "\t" << delay.ToDouble(Time::US) << "\t" << 1 << "\t"
        << hopCount << "\n";
}

void
AppDelayTracer::FirstInterestDataDelay(Ptr<App> app, uint32_t seqno, Time delay, uint32_t retxCount,
                                       int32_t hopCount)
{
  *m_os << Simulator::Now().ToDouble(Time::S) << "\t" << m_node << "\t" << app->GetId() << "\t"
        << seqno << "\t"
        << "FullDelay"
        << "\t" << delay.ToDouble(Time::S) << "\t" << delay.ToDouble(Time::US) << "\t" << retxCount
        << "\t" << hopCount << "\n";
}

} // namespace ndn
} // namespace ns3
