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

#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"
#include "ns3/ndn-pit.h"
#include "ns3/ndn-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"


#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ndn.fw.Stats");

namespace ns3 {
namespace ndn {

using namespace ndnSIM;

namespace fw {

NS_OBJECT_ENSURE_REGISTERED (FwStats);
  
TypeId
FwStats::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::Stats")
    .SetGroupName ("Ndn")
    .SetParent <super> ()
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
FwStats::OnInterest (Ptr<Face> face,
                     Ptr<const InterestHeader> header,
                     Ptr<const Packet> origPacket)
{
  super::OnInterest (face, header, origPacket);
  
  m_stats.Rx (header->GetName ().cut (1), face, origPacket->GetSize ());

  ScheduleRefreshingIfNecessary ();
}

void
FwStats::OnData (Ptr<Face> face,
                 Ptr<const ContentObjectHeader> header,
                 Ptr<Packet> payload,
                 Ptr<const Packet> origPacket)
{
  super::OnData (face, header, payload, origPacket);
  
  m_stats.Rx (header->GetName ().cut (1), face, origPacket->GetSize ());

  ScheduleRefreshingIfNecessary ();
}


void
FwStats::FailedToCreatePitEntry (Ptr<Face> inFace,
                                 Ptr<const InterestHeader> header,
                                 Ptr<const Packet> origPacket)
{
  super::FailedToCreatePitEntry (inFace, header, origPacket);

  // Kind of cheating... But at least this way we will have some statistics
  m_stats.NewPitEntry (header->GetName ().cut (1));
  m_stats.Incoming (header->GetName ().cut (1), inFace);
  m_stats.Timeout (header->GetName ().cut (1));

  ScheduleRefreshingIfNecessary ();
}

void
FwStats::DidCreatePitEntry (Ptr<Face> inFace,
                            Ptr<const InterestHeader> header,
                            Ptr<const Packet> origPacket,
                            Ptr<pit::Entry> pitEntry)
{
  super::DidCreatePitEntry (inFace, header, origPacket, pitEntry);
  
  m_stats.NewPitEntry (header->GetName ().cut (1));
  m_stats.Incoming (header->GetName ().cut (1), inFace);
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                     Ptr<pit::Entry> pitEntry)
{
  super::WillSatisfyPendingInterest (inFace, pitEntry);
  
  m_stats.Satisfy (pitEntry->GetPrefix ().cut (1));
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::DidSendOutInterest (Ptr<Face> outFace,
                             Ptr<const InterestHeader> header,
                             Ptr<const Packet> origPacket,
                             Ptr<pit::Entry> pitEntry)
{
  super::DidSendOutInterest (outFace, header, origPacket, pitEntry);

  m_stats.Outgoing (header->GetName ().cut (1), outFace);
  m_stats.Tx (header->GetName ().cut (1), outFace, origPacket->GetSize ());
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::DidSendOutData (Ptr<Face> outFace,
                         Ptr<const ContentObjectHeader> header,
                         Ptr<const Packet> payload,
                         Ptr<const Packet> origPacket,
                         Ptr<pit::Entry> pitEntry)
{
  super::DidSendOutData (outFace, header, payload, origPacket, pitEntry);

  m_stats.Tx (header->GetName ().cut (1), outFace, origPacket->GetSize ());
  
  ScheduleRefreshingIfNecessary ();
}


void
FwStats::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  super::WillEraseTimedOutPendingInterest (pitEntry);

  m_stats.Timeout (pitEntry->GetPrefix ().cut (1));
  
  ScheduleRefreshingIfNecessary ();
}

void
FwStats::DidExhaustForwardingOptions (Ptr<Face> inFace,
                                      Ptr<const InterestHeader> header,
                                      Ptr<const Packet> origPacket,
                                      Ptr<pit::Entry> pitEntry)
{
  super::DidExhaustForwardingOptions (inFace, header, origPacket, pitEntry);
  
  if (pitEntry->GetOutgoing ().size () == 0)
    {
      m_stats.Timeout (pitEntry->GetPrefix ().cut (1));
  
      ScheduleRefreshingIfNecessary ();
    }
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

void
FwStats::RemoveFace (Ptr<Face> face)
{
  m_stats.RemoveFace (face);

  super::RemoveFace (face);
}

void
FwStats::DidReceiveValidNack (Ptr<Face> inFace,
                              uint32_t nackCode,
                              Ptr<pit::Entry> pitEntry)
{
  m_stats.Satisfy (pitEntry->GetPrefix ().cut (1));
  ScheduleRefreshingIfNecessary ();
  
  // m_stats.UndoNewPitEntry (header->GetName ().cut (1));
  // m_stats.UndoOutgoing (header->GetName ().cut (1), inFace);

  // for (pit::Enty::in_container::iterator item = pitEntry->GetIncoming ().begin ();
  //      item != pitEntry->GetIncoming ().end ();
  //      item ++)
  //   {
  //     m_stats.UndoIncoming (header->GetName ().cut (1), item->m_face);
  //   }
}


} // namespace fw
} // namespace ndn
} // namespace ns3
