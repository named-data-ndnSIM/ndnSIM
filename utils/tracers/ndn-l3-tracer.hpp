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

#ifndef NDN_L3_TRACER_H
#define NDN_L3_TRACER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/NFD/daemon/table/pit-entry.hpp"

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class Node;
class Packet;

namespace ndn {

/**
 * @ingroup ndn-tracers
 * @brief Base class for network-layer (incoming/outgoing Interests and Data) tracing of NDN stack
 */
class L3Tracer : public SimpleRefCount<L3Tracer> {
public:
  /**
   * @brief Trace constructor that attaches to the node using node pointer
   * @param node  pointer to the node
   */
  L3Tracer(Ptr<Node> node);

  /**
   * @brief Trace constructor that attaches to the node using node name
   * @param nodeName  name of the node registered using Names::Add
   */
  L3Tracer(const std::string& node);

  /**
   * @brief Destructor
   */
  virtual ~L3Tracer();

  /**
   * @brief Print head of the trace (e.g., for post-processing)
   *
   * @param os reference to output stream
   */
  virtual void
  PrintHeader(std::ostream& os) const = 0;

  /**
   * @brief Print current trace data
   *
   * @param os reference to output stream
   */
  virtual void
  Print(std::ostream& os) const = 0;

protected:
  void
  Connect();

  virtual void
  OutInterests(const Interest&, const Face&) = 0;

  virtual void
  InInterests(const Interest&, const Face&) = 0;

  virtual void
  OutData(const Data&, const Face&) = 0;

  virtual void
  InData(const Data&, const Face&) = 0;

  virtual void
  OutNack(const lp::Nack& nack, const Face&) = 0;

  virtual void
  InNack(const lp::Nack&, const Face&) = 0;

  virtual void
  SatisfiedInterests(const nfd::pit::Entry&, const Face&, const Data&) = 0;

  virtual void
  TimedOutInterests(const nfd::pit::Entry&) = 0;

protected:
  std::string m_node;
  Ptr<Node> m_nodePtr;

  struct Stats {
    inline void
    Reset()
    {
      m_inInterests = 0;
      m_outInterests = 0;
      m_inData = 0;
      m_outData = 0;
      m_inNack = 0;
      m_outNack = 0;
      m_satisfiedInterests = 0;
      m_timedOutInterests = 0;

      m_outSatisfiedInterests = 0;
      m_outTimedOutInterests = 0;
    }

    double m_inInterests;
    double m_outInterests;
    double m_inData;
    double m_outData;
    double m_inNack;
    double m_outNack;
    double m_satisfiedInterests;
    double m_timedOutInterests;
    double m_outSatisfiedInterests;
    double m_outTimedOutInterests;
  };
};

/**
 * @brief Helper to dump the trace to an output stream
 */
inline std::ostream&
operator<<(std::ostream& os, const L3Tracer& tracer)
{
  os << "# ";
  tracer.PrintHeader(os);
  os << "\n";
  tracer.Print(os);
  return os;
}

} // namespace ndn
} // namespace ns3

#endif // NDN_L3_TRACER_H
