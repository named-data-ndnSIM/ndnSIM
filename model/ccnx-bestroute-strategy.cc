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
#include "ccnx-pit.h"
#include "ccnx-pit-entry.h"
#include "ccnx-path-stretch-tag.h"

#include "ns3/assert.h"
#include "ns3/log.h"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxBestRouteStrategy");

namespace ns3 
{

using namespace __ccnx_private;

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

  

  // Try to work out with just green faces
  bool greenOk = PropagateInterestViaGreen (pitEntry, incomingFace, header, packet);
  if (greenOk)
    return true;

  int propagatedCount = 0;

  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry.m_fibEntry.m_faces.get<i_metric> ())
    {
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED) // all non-read faces are in front
        break;
      
      if (metricFace.m_face == incomingFace) 
        continue; // same face as incoming, don't forward

      if (pitEntry.m_incoming.find (metricFace.m_face) != pitEntry.m_incoming.end ()) 
        continue; // don't forward to face that we received interest from

      CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
        pitEntry.m_outgoing.find (metricFace.m_face);
      
      if (outgoing != pitEntry.m_outgoing.end () &&
          outgoing->m_retxCount >= pitEntry.m_maxRetxCount)
        {
          NS_LOG_ERROR (outgoing->m_retxCount << " >= " << pitEntry.m_maxRetxCount);
          continue; // already forwarded before during this retransmission cycle
        }

      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        {
          continue;
        }

      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind(&CcnxPitEntry::AddOutgoing, ll::_1, metricFace.m_face));

      Ptr<Packet> packetToSend = packet->Copy ();
      TagPacket (packetToSend, metricFace);

      //transmission
      metricFace.m_face->Send (packetToSend);
      m_transmittedInterestsTrace (header, metricFace.m_face);
      
      propagatedCount++;
      break; // do only once
    }

  NS_LOG_INFO ("Propagated to " << propagatedCount << " faces");
  return propagatedCount > 0;
}
    
} //namespace ns3
