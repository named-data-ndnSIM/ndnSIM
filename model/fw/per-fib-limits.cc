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
  // m_decayLimitsEvent.Cancel ();
}

void
PerFibLimits::RemoveFace (Ptr<Face> face)
{  
  super::RemoveFace (face);

  for (PitQueueMap::iterator item = m_pitQueues.begin ();
       item != m_pitQueues.end ();
       item ++)
    {
      item->second.Remove (face);
    }
  m_pitQueues.erase (face);
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
  
  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      // just suppress without any other action
      return false;
    }
  
  // if (pitEntry->GetFibEntry ()->GetLimits ().IsBelowLimit ())
  //   {
  //     if (outFace->GetLimits ().IsBelowLimit ())
  //       {
  //         pitEntry->AddOutgoing (outFace);
  //         return true;
  //       }
  //     else
  //       {
  //         NS_LOG_DEBUG ("Face limit. Reverting back per-prefix allowance");
  //         pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
  //       }
  //   }

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
      NS_LOG_DEBUG ("Face limit");
    }
  
  bool enqueued = m_pitQueues[outFace].Enqueue (inFace, pitEntry);
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

  PitQueue::Remove (pitEntry);
  
  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }

  ProcessFromQueue ();
  // pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
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
  
  // pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
}


void
PerFibLimits::ProcessFromQueue ()
{
  for (PitQueueMap::iterator queue = m_pitQueues.begin ();
       queue != m_pitQueues.end ();
       queue++)
    {
      Ptr<Face> outFace = queue->first;
      
      while (!queue->second.IsEmpty () && outFace->GetLimits ().IsBelowLimit ())
        {
          // now we have enqueued packet and have slot available. Send out delayed packet
          Ptr<pit::Entry> pitEntry = queue->second.Pop ();
          pitEntry->AddOutgoing (outFace);

          Ptr<Packet> packetToSend = Create<Packet> ();
          packetToSend->AddHeader (*pitEntry->GetInterest ());
          
          outFace->Send (packetToSend);
          DidSendOutInterest (outFace, pitEntry->GetInterest (), packetToSend, pitEntry);
        }
    }
}

// void
// PerFibLimits::DidReceiveValidNack (Ptr<Face> inFace,
//                                    uint32_t nackCode,
//                                    Ptr<pit::Entry> pitEntry)
// {
//   // super::DidReceiveValidNack (inFace, nackCode, pitEntry);

//   // ??
// }

// void
// PerFibLimits::DecayLimits ()
// {
//   for (Ptr<fib::Entry> entry = m_fib->Begin ();
//        entry != m_fib->End ();
//        entry = m_fib->Next (entry))
//     {
//       entry->GetLimits ().DecayCurrentLimit ();
//     }

//   m_decayLimitsEvent = Simulator::Schedule (Seconds (1.0), &PerFibLimits::DecayLimits, this);
// }


} // namespace fw
} // namespace ndn
} // namespace ns3
