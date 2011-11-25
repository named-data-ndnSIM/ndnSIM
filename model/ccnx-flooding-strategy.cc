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

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

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
                                         const Ptr<const Packet> &packet)
{
  NS_LOG_FUNCTION (this);
    
  int propagatedCount = 0;
  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry.m_fibEntry.m_faces)
    {
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED) // all non-read faces are in front
        break;
      
      if (metricFace.m_face == incomingFace) 
        continue; // same face as incoming, don't forward

      if (pitEntry.m_incoming.find (metricFace.m_face) != pitEntry.m_incoming.end ()) 
        continue; // don't forward to face that we received interest from
      
      if (pitEntry.m_outgoing.find (metricFace.m_face) != pitEntry.m_outgoing.end ()) 
        continue; // already forwarded before

      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        continue;

      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind(&CcnxPitEntry::AddOutgoing, ll::_1, metricFace.m_face));

      // NS_LOG_DEBUG ("new outgoing entry for " << boost::cref (*metricFace.m_face));

      metricFace.m_face->Send (packet->Copy ());
      
      propagatedCount++;
    }

  NS_LOG_INFO ("Propagated to " << propagatedCount << " faces");
  return propagatedCount > 0;
}
    
} //namespace ns3
