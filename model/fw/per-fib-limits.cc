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

#include "per-fib-limits.h"

#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"
#include "ns3/ndn-pit.h"
#include "ns3/ndn-pit-entry.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/double.h"

#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ndn.fw.PerFibLimits");

namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (PerFibLimits);
  
TypeId
PerFibLimits::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::PerFibLimits")
    .SetGroupName ("Ndn")
    .SetParent <super> ()
    .AddConstructor <PerFibLimits> ()
    ;
  return tid;
}
    
PerFibLimits::PerFibLimits ()
{
}

void
PerFibLimits::DoDispose ()
{
  super::DoDispose ();
}

void
PerFibLimits::RemoveFace (Ptr<Face> face)
{  
  for (PitQueueMap::iterator item = m_pitQueues.begin ();
       item != m_pitQueues.end ();
       item ++)
    {
      item->second.Remove (face);
    }
  m_pitQueues.erase (face);

  super::RemoveFace (face);
}


bool
PerFibLimits::TrySendOutInterest (Ptr<Face> inFace,
                                  Ptr<Face> outFace,
                                  Ptr<const InterestHeader> header,
                                  Ptr<const Packet> origPacket,
                                  Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  // totally override all (if any) parent processing

  if (pitEntry->GetFwTag<PitQueueTag> () != boost::shared_ptr<PitQueueTag> ())
    {
      pitEntry->UpdateLifetime (Seconds (0.10));
      NS_LOG_DEBUG ("Packet is still in queue and is waiting for its processing");
      return true; // already in the queue
    }
  
  if (header->GetInterestLifetime () < Seconds (0.1))
    {
      NS_LOG_DEBUG( "What the fuck? Why interest lifetime is so short? [" << header->GetInterestLifetime ().ToDouble (Time::S) << "s]");
    }
  
  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      // just suppress without any other action
      return false;
    }

  NS_LOG_DEBUG ("Limit: " << outFace->GetLimits ().m_curMaxLimit << ", outstanding: " << outFace->GetLimits ().m_outstanding);
  
  if (outFace->GetLimits ().IsBelowLimit ())
    {
      pitEntry->AddOutgoing (outFace);

      //transmission
      Ptr<Packet> packetToSend = origPacket->Copy ();
      outFace->Send (packetToSend);

      DidSendOutInterest (outFace, header, origPacket, pitEntry);      
      return true;
    }
  else
    {
      NS_LOG_DEBUG ("Face limit for " << header->GetName ());
    }

  // hack
  // offset lifetime, so we don't keep entries in queue for too long
  // pitEntry->OffsetLifetime (Seconds (0.010) + );
  // std::cerr << (pitEntry->GetExpireTime () - Simulator::Now ()).ToDouble (Time::S) * 1000 << "ms" << std::endl;
  pitEntry->OffsetLifetime (Seconds (-pitEntry->GetInterest ()->GetInterestLifetime ().ToDouble (Time::S)));
  pitEntry->UpdateLifetime (Seconds (0.10));

  // const ndnSIM::LoadStatsFace &stats = GetStatsTree ()[header->GetName ()].incoming ().find (inFace)->second;
  const ndnSIM::LoadStatsFace &stats = GetStatsTree ()["/"].incoming ().find (inFace)->second;
  double weight = std::min (1.0, stats.GetSatisfiedRatio ().get<0> ());
  if (weight < 0)
    {
      // if stats is unknown, gracefully accept interest with normal priority
      weight = 1.0;
    }
  
  bool enqueued = m_pitQueues[outFace].Enqueue (inFace, pitEntry, weight);

  // if (Simulator::GetContext () == 6)
  //   {
  //     // std::cerr << "Attempt to enqueue packet for " << pitEntry->GetPrefix () << ": " << (enqueued?"succeeded":"failed") << std::endl;
  //   }
  
  if (enqueued)
    {
      NS_LOG_DEBUG ("PIT entry is enqueued for delayed processing. Telling that we forwarding possible");
      return true;
    }
  else
    return false;
}

void
PerFibLimits::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  super::WillEraseTimedOutPendingInterest (pitEntry);

  if (pitEntry->GetOutgoing ().size () == 0)
    {
      Ptr<InterestHeader> nackHeader = Create<InterestHeader> (*pitEntry->GetInterest ());
      
      NS_ASSERT (pitEntry->GetFwTag<PitQueueTag> () != boost::shared_ptr<PitQueueTag> ());
      if (pitEntry->GetFwTag<PitQueueTag> ()->IsLastOneInQueues ())
        {
          nackHeader->SetNack (100);
        }
      else
        {
          nackHeader->SetNack (101);
        }
          
      Ptr<Packet> pkt = Create<Packet> ();
      pkt->AddHeader (*nackHeader);
      
      for (pit::Entry::in_container::iterator face = pitEntry->GetIncoming ().begin ();
           face != pitEntry->GetIncoming ().end ();
           face ++)
        {
          face->m_face->Send (pkt->Copy ());
        }
    }
  
  PitQueue::Remove (pitEntry);
  
  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }

  ProcessFromQueue ();
}


void
PerFibLimits::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                          Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  super::WillSatisfyPendingInterest (inFace, pitEntry);
  
  PitQueue::Remove (pitEntry);

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }

  ProcessFromQueue ();
}


void
PerFibLimits::ProcessFromQueue ()
{
  NS_LOG_FUNCTION (this);
  
  for (PitQueueMap::iterator queue = m_pitQueues.begin ();
       queue != m_pitQueues.end ();
       queue++)
    {
      Ptr<Face> outFace = queue->first;

      NS_LOG_DEBUG ("Processing " << *outFace);
      
      while (!queue->second.IsEmpty () && outFace->GetLimits ().IsBelowLimit ())
        {
          // now we have enqueued packet and have slot available. Send out delayed packet
          Ptr<pit::Entry> pitEntry = queue->second.Pop ();
          NS_ASSERT_MSG (pitEntry != 0, "There *have to* be an entry in queue");

          // hack
          // offset lifetime back, so PIT entry wouldn't prematurely expire
          
          // std::cerr << Simulator::GetContext () << ", Lifetime before " << (pitEntry->GetExpireTime () - Simulator::Now ()).ToDouble (Time::S) << "s" << std::endl;
          pitEntry->OffsetLifetime (Seconds (-0.10) + Seconds (pitEntry->GetInterest ()->GetInterestLifetime ().ToDouble (Time::S)));
          // std::cerr << Simulator::GetContext () << ", Lifetime after " << (pitEntry->GetExpireTime () - Simulator::Now ()).ToDouble (Time::S) << "s" << std::endl;
                    
          pitEntry->AddOutgoing (outFace);

          Ptr<Packet> packetToSend = Create<Packet> ();
          Ptr<InterestHeader> header = Create<InterestHeader> (*pitEntry->GetInterest ());
          NS_LOG_DEBUG ("Adjust interest lifetime to " << pitEntry->GetExpireTime () - Simulator::Now () << "s");
          // header->SetInterestLifetime (
          //                              // header->GetInterestLifetime () - ()
          //                              pitEntry->GetExpireTime () - Simulator::Now ()
          //                              );
          // std::cerr << "New lifetime: " << (pitEntry->GetExpireTime () - Simulator::Now ()).ToDouble (Time::S) << "s" << std::endl;
          packetToSend->AddHeader (*header);

          NS_LOG_DEBUG ("Delayed sending for " << pitEntry->GetPrefix ());
          outFace->Send (packetToSend);
          DidSendOutInterest (outFace, pitEntry->GetInterest (), packetToSend, pitEntry);
        }
    }
}

void
PerFibLimits::DidReceiveValidNack (Ptr<Face> inFace,
                                   uint32_t nackCode,
                                   Ptr<pit::Entry> pitEntry)
{
  // NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  PitQueue::Remove (pitEntry);
 

  Ptr<InterestHeader> nackHeader = Create<InterestHeader> (*pitEntry->GetInterest ());
  nackHeader->SetNack (100);
  Ptr<Packet> pkt = Create<Packet> ();
  pkt->AddHeader (*nackHeader);

  for (pit::Entry::in_container::iterator face = pitEntry->GetIncoming ().begin ();
       face != pitEntry->GetIncoming ().end ();
       face ++)
    {
      face->m_face->Send (pkt->Copy ());
    }


  
  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }

  m_pit->MarkErased (pitEntry);
  
  ProcessFromQueue ();
}


} // namespace fw
} // namespace ndn
} // namespace ns3
