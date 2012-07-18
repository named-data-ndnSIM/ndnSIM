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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "smart-flooding.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-pit.h"
#include "ns3/ccnx-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/boolean.h"

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("NdnSimSmartFlooding");

namespace ns3 {

using namespace __ccnx_private;

namespace ndnSIM {

NS_OBJECT_ENSURE_REGISTERED (SmartFlooding);
    
TypeId
SmartFlooding::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndnSIM::SmartFloodingy")
    .SetGroupName ("Ccnx")
    .SetParent <GreenYellowRed> ()
    .AddConstructor <SmartFlooding> ()
    ;
  return tid;
}
    
SmartFlooding::SmartFlooding ()
{
}

bool
SmartFlooding::DoPropagateInterest (const Ptr<CcnxFace> &incomingFace,
                                    Ptr<CcnxInterestHeader> &header,
                                    const Ptr<const Packet> &packet,
                                    Ptr<CcnxPitEntry> pitEntry)
{
  NS_LOG_FUNCTION (this);

  // Try to work out with just green faces
  bool greenOk = super::DoPropagateInterest (incomingFace, header, packet, pitEntry);
  if (greenOk)
    return true;

  int propagatedCount = 0;

  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<i_metric> ())
    {
      NS_LOG_DEBUG ("Trying " << boost::cref(metricFace));
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED) // all non-read faces are in the front of the list
        break;
      
      if (metricFace.m_face == incomingFace) 
        {
          NS_LOG_DEBUG ("continue (same as incoming)");
          continue; // same face as incoming, don't forward
        }

      CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
        pitEntry->GetOutgoing ().find (metricFace.m_face);
      
      if (outgoing != pitEntry->GetOutgoing ().end () &&
          outgoing->m_retxCount >= pitEntry->GetMaxRetxCount ())
        {
          NS_LOG_DEBUG ("continue (same as previous outgoing)");
          continue; // already forwarded before during this retransmission cycle
        }
      NS_LOG_DEBUG ("max retx count: " << pitEntry->GetMaxRetxCount ());

      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        {
          continue;
        }

      pitEntry->AddOutgoing (metricFace.m_face);

      Ptr<Packet> packetToSend = packet->Copy ();

      //transmission
      metricFace.m_face->Send (packetToSend);
      m_outInterests (header, metricFace.m_face);
      
      propagatedCount++;
    }

  NS_LOG_INFO ("Propagated to " << propagatedCount << " faces");
  return propagatedCount > 0;
}

} // namespace ndnSIM
} // namespace ns3
