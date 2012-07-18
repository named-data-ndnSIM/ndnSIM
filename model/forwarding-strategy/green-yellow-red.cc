/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 *          Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "green-yellow-red.h"

#include "ns3/ccnx-pit.h"
#include "ns3/ccnx-pit-entry.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx-pit.h"
#include "ns3/ccnx-fib.h"
#include "ns3/ccnx-content-store.h"

#include "ns3/assert.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/boolean.h"
#include "ns3/string.h"

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/tuple/tuple.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("NdnSimGreenYellowRed");

namespace ns3 {

using namespace __ccnx_private;

namespace ndnSIM {

NS_OBJECT_ENSURE_REGISTERED (GreenYellowRed);

TypeId
GreenYellowRed::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndnSIM::GreenYellowRed")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxForwardingStrategy> ()

    ;
  return tid;
}

bool
GreenYellowRed::DoPropagateInterest (const Ptr<CcnxFace> &incomingFace,
                                     Ptr<CcnxInterestHeader> &header,
                                     const Ptr<const Packet> &packet,
                                     Ptr<CcnxPitEntry> pitEntry)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (m_pit != 0, "PIT should be aggregated with forwarding strategy");

  int propagatedCount = 0;
  
  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry->GetFibEntry ()->m_faces.get<i_metric> ())
    {
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED ||
          metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_YELLOW)
        break; //propagate only to green faces

      if (pitEntry->GetIncoming ().find (metricFace.m_face) != pitEntry->GetIncoming ().end ()) 
        continue; // don't forward to face that we received interest from

      CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
        pitEntry->GetOutgoing ().find (metricFace.m_face);
      
      if (outgoing != pitEntry->GetOutgoing ().end () &&
          outgoing->m_retxCount >= pitEntry->GetMaxRetxCount ())
        {
          NS_LOG_DEBUG ("retxCount: " << outgoing->m_retxCount << ", maxRetxCount: " << pitEntry->GetMaxRetxCount ());
          continue;
        }
      
      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        {
          // let's try different green face
          continue;
        }

      pitEntry->AddOutgoing (metricFace.m_face);

      Ptr<Packet> packetToSend = packet->Copy ();

      //transmission
      metricFace.m_face->Send (packetToSend);
      m_outInterests (header, metricFace.m_face);
      
      propagatedCount++;
      break; // propagate only one interest
    }

  return propagatedCount > 0;
}

void
GreenYellowRed::WillSatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                                            Ptr<CcnxPitEntry> pitEntry)
{
  // Update metric status for the incoming interface in the corresponding FIB entry
  pitEntry->GetFibEntry ()->UpdateStatus (incomingFace, CcnxFibFaceMetric::NDN_FIB_GREEN);

  super::WillSatisfyPendingInterest (incomingFace, pitEntry);
}

} // namespace ndnSIM
} // namespace ns3
