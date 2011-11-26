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

#include "ccnx-bestroute-strategy.h"
#include "ccnx-interest-header.h"

#include "ns3/assert.h"
#include "ns3/log.h"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxBestRouteStrategy");

namespace ns3 
{
    
NS_OBJECT_ENSURE_REGISTERED (CcnxBestRouteStrategy);
  
TypeId CcnxBestRouteStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxBestRouteStrategy")
    .SetGroupName ("Ccnx")
    .SetParent <CcnxForwardingStrategy> ()
    .AddConstructor <CcnxBestRouteStrategy> ()
    ;
  return tid;
}
    
CcnxBestRouteStrategy::CcnxBestRouteStrategy ()
{
}
    
bool
CcnxBestRouteStrategy::PropagateInterest (const CcnxPitEntry  &pitEntry, 
                                          const Ptr<CcnxFace> &incomingFace,
                                          Ptr<CcnxInterestHeader> &header,
                                          const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (this);
  bool forwardedCount = 0;

  try
    {
      for (uint32_t skip = 0; skip < pitEntry.m_fibEntry.m_faces.size (); skip++)
        {
          const CcnxFibFaceMetric bestMetric = pitEntry.m_fibEntry.FindBestCandidate (skip);

          if (bestMetric.m_status == CcnxFibFaceMetric::NDN_FIB_RED) // no point to send there
            continue;

          if (pitEntry.m_incoming.find (bestMetric.m_face) != pitEntry.m_incoming.end ()) 
            continue; // don't forward to face that we received interest from

          if (pitEntry.m_outgoing.find (bestMetric.m_face) != pitEntry.m_outgoing.end ()) // already forwarded before
            continue;

          bool faceAvailable = bestMetric.m_face->IsBelowLimit ();
          if (!faceAvailable) // huh...
            continue;

          m_pit->modify (m_pit->iterator_to (pitEntry),
                         ll::bind(&CcnxPitEntry::AddOutgoing, ll::_1, bestMetric.m_face));

          // NS_LOG_DEBUG ("new outgoing entry for " << boost::cref (*metricFace.m_face));

          bestMetric.m_face->Send (packet->Copy ());

          forwardedCount++;
          break; // if we succeeded in sending one packet, stop
        }
    }
  catch (CcnxFibEntry::NoFaces)
    {
    }

  return forwardedCount > 0;
}
    
} //namespace ns3
