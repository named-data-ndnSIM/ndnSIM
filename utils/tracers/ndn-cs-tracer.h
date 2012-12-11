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
 * Author: Xiaoyan Hu <x......u@gmail.com>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef CCNX_CS_TRACER_H
#define CCNX_CS_TRACER_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

class Node;
class Packet;

namespace ndn {

class InterestHeader;
class ContentObjectHeader;

/**
 * @brief Base class for content store tracers (CacheHits and CacheMisses)
 */
class CsTracer : public SimpleRefCount<CsTracer>
{
public:
  /**
   * @brief Trace constructor that attaches to the node using node pointer
   * @param node  pointer to the node
   */
  CsTracer (Ptr<Node> node);

  /**
   * @brief Trace constructor that attaches to the node using node name
   * @param nodeName  name of the node registered using Names::Add
   */
  CsTracer (const std::string &node);

  /**
   * @brief Destructor
   */
  virtual ~CsTracer ();

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
  CacheHits (std::string context,
  Ptr<const InterestHeader>, Ptr<const ContentObjectHeader>) = 0;
  
  virtual void 
  CacheMisses (std::string context,
  Ptr<const InterestHeader>) = 0;

protected:
  std::string m_node;
  Ptr<Node> m_nodePtr;

  struct Stats
  {
    inline void Reset ()
    {
      m_cacheHits   = 0;
      m_cacheMisses = 0;
    }
    double m_cacheHits;
    double m_cacheMisses;
  };
};

/**
 * @brief Helper to dump the trace to an output stream
 */
inline std::ostream&
operator << (std::ostream &os, const CsTracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ndn
} // namespace ns3

#endif // CCNX_CS_TRACER_H
