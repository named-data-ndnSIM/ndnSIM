/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "ccnx-flooding-strategy.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ccnx-interest-header.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxFloodingStrategy");

namespace ns3 
{
    
NS_OBJECT_ENSURE_REGISTERED (CcnxFloodingStrategy);
    
TypeId CcnxFloodingStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxFloodingStrategy")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxForwardingStrategy> ()
    ;
  return tid;
}
    
CcnxFloodingStrategy::CcnxFloodingStrategy ()
{
}

bool
CcnxFloodingStrategy::PropagateInterest (const CcnxPitEntry  &pitEntry, 
                                         const Ptr<CcnxFace> &incomingFace,
                                         Ptr<CcnxInterestHeader> &header,
                                         const Ptr<const Packet> &packet,
                                         SendCallback sendCallback)
{
  NS_LOG_FUNCTION (this);
    
  int propagatedCount = 0;
  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry.m_fibEntry.m_faces)
    {
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED) // all non-read faces are in front
        break;
      
      if (metricFace.m_face == incomingFace) // same face as incoming, don't forward
        continue;

      if (pitEntry.m_outgoing.find (metricFace.m_face) != pitEntry.m_outgoing.end ()) // already forwarded before
        continue;

      bool faceAvailable = m_pit->TryAddOutgoing (pitEntry, metricFace.m_face);
      if (!faceAvailable) // huh...
          continue;
        
      sendCallback (metricFace.m_face, header, packet->Copy());
      propagatedCount++;
    }

  return propagatedCount > 0;
}
    
} //namespace ns3
