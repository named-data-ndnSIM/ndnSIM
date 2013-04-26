/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_L3_TRACER_H
#define CCNX_L3_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class Node;
class Packet;

namespace ndn {

namespace pit {
class Entry;
}

class Face;

class Interest;
class ContentObject;

typedef Interest InterestHeader;
typedef ContentObject ContentObjectHeader;

/**
 * @brief Base class for network-layer (incoming/outgoing Interests and Data) tracing of NDN stack
 */
class L3Tracer : public SimpleRefCount<L3Tracer>
{
public:
  /**
   * @brief Trace constructor that attaches to the node using node pointer
   * @param node  pointer to the node
   */
  L3Tracer (Ptr<Node> node);

  /**
   * @brief Trace constructor that attaches to the node using node name
   * @param nodeName  name of the node registered using Names::Add
   */
  L3Tracer (const std::string &node);

  /**
   * @brief Destructor
   */
  virtual ~L3Tracer ();

  /**
   * @brief Print head of the trace (e.g., for post-processing)
   *
   * @param os reference to output stream
   */
  virtual void
  PrintHeader (std::ostream &os) const = 0;

  /**
   * @brief Print current trace data
   *
   * @param os reference to output stream
   */
  virtual void
  Print (std::ostream &os) const = 0;

protected:
  void
  Connect ();

  virtual void
  OutInterests  (std::string context,
                 Ptr<const Interest>, Ptr<const Face>) = 0;

  virtual void
  InInterests   (std::string context,
                 Ptr<const Interest>, Ptr<const Face>) = 0;

  virtual void
  DropInterests (std::string context,
                 Ptr<const Interest>, Ptr<const Face>) = 0;

  virtual void
  OutNacks  (std::string context,
             Ptr<const Interest>, Ptr<const Face>) = 0;

  virtual void
  InNacks   (std::string context,
             Ptr<const Interest>, Ptr<const Face>) = 0;

  virtual void
  DropNacks (std::string context,
             Ptr<const Interest>, Ptr<const Face>) = 0;


  virtual void
  OutData  (std::string context,
            Ptr<const ContentObject>, Ptr<const Packet>, bool fromCache, Ptr<const Face>) = 0;

  virtual void
  InData   (std::string context,
            Ptr<const ContentObject>, Ptr<const Packet>, Ptr<const Face>) = 0;

  virtual void
  DropData (std::string context,
            Ptr<const ContentObject>, Ptr<const Packet>, Ptr<const Face>) = 0;

  virtual void
  SatisfiedInterests (Ptr<const pit::Entry>) = 0;

  virtual void
  TimedOutInterests (Ptr<const pit::Entry>) = 0;

protected:
  std::string m_node;
  Ptr<Node> m_nodePtr;

  struct Stats
  {
    inline void Reset ()
    {
      m_inInterests   = 0;
      m_outInterests  = 0;
      m_dropInterests = 0;
      m_inNacks       = 0;
      m_outNacks      = 0;
      m_dropNacks     = 0;
      m_inData        = 0;
      m_outData       = 0;
      m_dropData      = 0;
      m_satisfiedInterests = 0;
      m_timedOutInterests = 0;
    }

    double m_inInterests;
    double m_outInterests;
    double m_dropInterests;
    double m_inNacks;
    double m_outNacks;
    double m_dropNacks;
    double m_inData;
    double m_outData;
    double m_dropData;
    double m_satisfiedInterests;
    double m_timedOutInterests;
  };
};

/**
 * @brief Helper to dump the trace to an output stream
 */
inline std::ostream&
operator << (std::ostream &os, const L3Tracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ndn
} // namespace ns3

#endif // CCNX_L3_TRACER_H
