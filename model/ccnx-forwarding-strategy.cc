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

#include "ns3/assert.h"

#include "ccnx-forwarding-strategy.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ccnx-interest-header.h"

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxForwardingStrategy");

namespace ns3 {

using namespace __ccnx_private;

NS_OBJECT_ENSURE_REGISTERED (CcnxForwardingStrategy);

TypeId CcnxForwardingStrategy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxForwardingStrategy")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
    ;
  return tid;
}

CcnxForwardingStrategy::CcnxForwardingStrategy ()
{
}

CcnxForwardingStrategy::~CcnxForwardingStrategy ()
{
}

void
CcnxForwardingStrategy::SetPit (Ptr<CcnxPit> pit)
{
  m_pit = pit;
}

bool
CcnxForwardingStrategy::PropagateInterestViaGreen (const CcnxPitEntry  &pitEntry, 
                                                   const Ptr<CcnxFace> &incomingFace,
                                                   Ptr<CcnxInterestHeader> &header,
                                                   const Ptr<const Packet> &packet)
{
  // NS_LOG_FUNCTION (this);

  int propagatedCount = 0;
  
  BOOST_FOREACH (const CcnxFibFaceMetric &metricFace, pitEntry.m_fibEntry.m_faces.get<i_metric> ())
    {
      if (metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_RED ||
          metricFace.m_status == CcnxFibFaceMetric::NDN_FIB_YELLOW)
        break; //propagate only to green faces

      if (pitEntry.m_incoming.find (metricFace.m_face) != pitEntry.m_incoming.end ()) 
        continue; // don't forward to face that we received interest from

      CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
        pitEntry.m_outgoing.find (metricFace.m_face);
      
      if (outgoing != pitEntry.m_outgoing.end () &&
          outgoing->m_retxCount >= pitEntry.m_maxRetxCount)
        {
          continue;
        }
      
      bool faceAvailable = metricFace.m_face->IsBelowLimit ();
      if (!faceAvailable) // huh...
        {
          // let's try different green face
          continue;
        }

      m_pit->modify (m_pit->iterator_to (pitEntry),
                     ll::bind(&CcnxPitEntry::AddOutgoing, ll::_1, metricFace.m_face));

      metricFace.m_face->Send (packet->Copy ());
      
      propagatedCount++;
      break; // propagate only one interest
    }

  // if (Simulator::GetContext ()==1)
  //   {
  //     if (propagatedCount > 0)
  //       NS_LOG_DEBUG ("Propagate via a green face");
  //     else 
  //       NS_LOG_DEBUG ("Can't :(");
  //   }
  return propagatedCount > 0;
}


} //namespace ns3
