/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
#include "ns3/ccnx.h"

namespace ns3 {

class Node;

class CcnxL3Tracer : public SimpleRefCount<CcnxL3Tracer>
{
public:
  CcnxL3Tracer (Ptr<Node> node);
  CcnxL3Tracer (const std::string &node);
  virtual ~CcnxL3Tracer () { };

  void
  Connect ();
  
  virtual void
  PrintHeader (std::ostream &os) const = 0;

  virtual void
  Print (std::ostream &os) const = 0;
  
  virtual void
  OutInterests  (std::string context,
                 Ptr<const CcnxInterest>, Ptr<const CcnxFace>) = 0;

  virtual void
  InInterests   (std::string context,
                 Ptr<const CcnxInterest>, Ptr<const CcnxFace>) = 0;

  virtual void
  DropInterests (std::string context,
                 Ptr<const CcnxInterest>, Ccnx::DropReason, Ptr<const CcnxFace>) = 0;
  
  virtual void
  OutNacks  (std::string context,
             Ptr<const CcnxInterest>, Ptr<const CcnxFace>) = 0;

  virtual void
  InNacks   (std::string context,
             Ptr<const CcnxInterest>, Ptr<const CcnxFace>) = 0;

  virtual void
  DropNacks (std::string context,
             Ptr<const CcnxInterest>, Ccnx::DropReason, Ptr<const CcnxFace>) = 0;

  
  virtual void
  OutData  (std::string context,
            Ptr<const CcnxData>, Ptr<const Packet>, bool fromCache, Ptr<const CcnxFace>) = 0;

  virtual void
  InData   (std::string context,
            Ptr<const CcnxData>, Ptr<const Packet>, Ptr<const CcnxFace>) = 0;

  virtual void
  DropData (std::string context,
            Ptr<const CcnxData>, Ptr<const Packet>, Ccnx::DropReason, Ptr<const CcnxFace>) = 0;

protected:
  std::string m_node;
  Ptr<Node> m_nodePtr;

  struct Stats
  {
    void Reset ();
    
    uint64_t m_inInterests;
    uint64_t m_outInterests;
    uint64_t m_dropInterests;
    uint64_t m_inNacks;
    uint64_t m_outNacks;
    uint64_t m_dropNacks;
    uint64_t m_inData;
    uint64_t m_outData;
    uint64_t m_dropData;
  };
};

inline std::ostream&
operator << (std::ostream &os, const CcnxL3Tracer &tracer)
{
  os << "# ";
  tracer.PrintHeader (os);
  os << "\n";
  tracer.Print (os);
  return os;
}

} // namespace ns3

#endif // CCNX_L3_TRACER_H
