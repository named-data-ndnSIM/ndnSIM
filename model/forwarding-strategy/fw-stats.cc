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
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#include "fw-stats.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx-pit.h"
#include "ns3/ccnx-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"


#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("NdnSimFwStats");

namespace ns3 {
namespace ndnSIM {

NS_OBJECT_ENSURE_REGISTERED (FwStats);
  
TypeId
FwStats::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndnSIM::FwStats")
    .SetGroupName ("Ccnx")
    .SetParent <BestRoute> ()
    .AddConstructor <FwStats> ()

    .AddTraceSource ("Stats", "Fired every time stats tree is updated",
                     MakeTraceSourceAccessor (&FwStats::m_statsTrace))
    ;
  return tid;
}
    
FwStats::FwStats ()
{
}

void
FwStats::DoDispose ()
{
  BestRoute::DoDispose ();
  m_statsRefreshEvent.Cancel ();
}

void
FwStats::FailedToCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                                 Ptr<CcnxInterestHeader> header,
                                 const Ptr<const Packet> &packet)
{
  super::FailedToCreatePitEntry (incomingFace, header, packet);

  // Kind of cheating... But at least this way we will have some statistics
  m_stats.NewPitEntry (header->GetName ());
  m_stats.Timeout (header->GetName ());
}

void
FwStats::DidCreatePitEntry (const Ptr<CcnxFace> &incomingFace,
                            Ptr<CcnxInterestHeader> header,
                            const Ptr<const Packet> &packet,
                            Ptr<CcnxPitEntry> pitEntry)
{
  super::DidCreatePitEntry (incomingFace, header, packet, pitEntry);
  
  m_stats.NewPitEntry (header->GetName ());
  m_stats.Incoming (header->GetName (), incomingFace);
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::WillSatisfyPendingInterest (const Ptr<CcnxFace> &incomingFace,
                                     Ptr<CcnxPitEntry> pitEntry)
{
  super::WillSatisfyPendingInterest (incomingFace, pitEntry);
  
  m_stats.Satisfy (pitEntry->GetPrefix ());
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::DidSendOutInterest (const Ptr<CcnxFace> &outgoingFace,
                             Ptr<CcnxInterestHeader> header,
                             Ptr<CcnxPitEntry> pitEntry)
{
  super::DidSendOutInterest (outgoingFace, header, pitEntry);

  m_stats.Outgoing (header->GetName (), outgoingFace);
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::WillErasePendingInterest (Ptr<CcnxPitEntry> pitEntry)
{
  super::WillErasePendingInterest (pitEntry);

  m_stats.Timeout (pitEntry->GetPrefix ());
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::ScheduleRefreshingIfNecessary ()
{
  if (m_statsRefreshEvent.IsRunning ()) return;
  m_statsRefreshEvent = Simulator::Schedule (Seconds (1.0), &FwStats::RefreshStats, this);
}

void
FwStats::RefreshStats ()
{
  m_stats.Step ();
  m_statsTrace (this, m_stats);
  
  NS_LOG_DEBUG (m_stats["/"]);

  if (!m_stats["/"].IsZero ())
    {
      m_statsRefreshEvent = Simulator::Schedule (Seconds (1.0), &FwStats::RefreshStats, this);
    }
}



} // namespace ndnSIM
} // namespace ns3
