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
#include "ccnx-interest-header.h"
#include "ccnx-pit.h"
#include "ccnx-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxFloodingStrategy");

namespace ns3 
{

using namespace __ccnx_private;

NS_OBJECT_ENSURE_REGISTERED (CcnxFloodingStrategy);
    
TypeId CcnxFloodingStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxFloodingStrategy")
    .SetGroupName ("Ccnx")
    .SetParent <CcnxForwardingStrategy> ()
    .AddConstructor <CcnxFloodingStrategy> ()
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

  // Try to work out with just green faces
  bool greenOk = PropagateInterestViaGreen (pitEntry, incomingFace, header, packet);
  if (greenOk)
    return true;

  // boo... :(
  int propagatedCount = 0;

  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry.m_fibEntry.m_faces.get<i_metric> ())
    {
      NS_LOG_DEBUG ("Trying " << boost::cref(metricFace));
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED) // all non-read faces are in front
        break;
      
      if (metricFace.m_face == incomingFace) 
        {
          NS_LOG_DEBUG ("continue (same as incoming)");
          continue; // same face as incoming, don't forward
        }

      if (pitEntry.m_incoming.find (metricFace.m_face) != pitEntry.m_incoming.end ()) 
        {
          NS_LOG_DEBUG ("continue (same as previous incoming)");
          continue; // don't forward to face that we received interest from
        }

      CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
        pitEntry.m_outgoing.find (metricFace.m_face);
      
      if (outgoing != pitEntry.m_outgoing.end () &&
          outgoing->m_retxCount >= pitEntry.m_maxRetxCount)
        {
          NS_LOG_DEBUG ("continue (same as previous outgoing)");
          continue; // already forwarded before during this retransmission cycle
        }
      NS_LOG_DEBUG ("max retx count: " << pitEntry.m_maxRetxCount);

      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        {
          continue;
        }

      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind(&CcnxPitEntry::AddOutgoing, ll::_1, metricFace.m_face));

      // if (Simulator::GetContext ()==2)
      //   {
      //     NS_LOG_ERROR ("new outgoing entry for " << boost::cref (*metricFace.m_face));
      //     NS_LOG_ERROR ("size: " << pitEntry.m_outgoing.size ());
      //   }


      //update path stretch
      WeightsPathStretchTag pathStretch;
      //packet->PeekPacketTag(pathStretch);
      
      pathStretch.AddNewHop(metricFace.m_routingCost);
      packet->AddPacketTag(pathStretch);
      
      //transmission
      metricFace.m_face->Send (packet->Copy ());
      m_transmittedInterestsTrace (header, metricFace.m_face);
      
      propagatedCount++;
    }

  NS_LOG_INFO ("Propagated to " << propagatedCount << " faces");
  return propagatedCount > 0;
}
    
} //namespace ns3
