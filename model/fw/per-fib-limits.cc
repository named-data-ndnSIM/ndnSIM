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

#include "ns3/ndn-l3-protocol.h"
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
#include <boost/lexical_cast.hpp>
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
  m_announceEvent.Cancel ();
  
  super::DoDispose ();
}

void
PerFibLimits::NotifyNewAggregate ()
{
  super::NotifyNewAggregate ();

  if (m_pit != 0 && m_fib != 0)
    {
      m_announceEvent = Simulator::Schedule (Seconds (1.0),
                                             &PerFibLimits::AnnounceLimits, this);
    }
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

void
PerFibLimits::OnInterest (Ptr<Face> face,
                          Ptr<const InterestHeader> header,
                          Ptr<const Packet> origPacket)
{
  if (header->GetScope () != 0)
    super::OnInterest (face, header, origPacket);
  else
    ApplyAnnouncedLimit (face, header);
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
  // const ndnSIM::LoadStatsFace &stats = GetStatsTree ()["/"].incoming ().find (inFace)->second;
  // double weight = std::min (1.0, stats.GetSatisfiedRatio ().get<0> ());
  bool enqueued = m_pitQueues[outFace].Enqueue (inFace, pitEntry, 1);

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

  // if (pitEntry->GetOutgoing ().size () == 0)
  //   {
  //     Ptr<InterestHeader> nackHeader = Create<InterestHeader> (*pitEntry->GetInterest ());
      
  //     NS_ASSERT (pitEntry->GetFwTag<PitQueueTag> () != boost::shared_ptr<PitQueueTag> ());
  //     if (pitEntry->GetFwTag<PitQueueTag> ()->IsLastOneInQueues ())
  //       {
  //         nackHeader->SetNack (100);
  //       }
  //     else
  //       {
  //         nackHeader->SetNack (101);
  //       }
          
  //     Ptr<Packet> pkt = Create<Packet> ();
  //     pkt->AddHeader (*nackHeader);
      
  //     for (pit::Entry::in_container::iterator face = pitEntry->GetIncoming ().begin ();
  //          face != pitEntry->GetIncoming ().end ();
  //          face ++)
  //       {
  //         face->m_face->Send (pkt->Copy ());
  //       }
  //   }

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
          if (pitEntry == 0)
            {
              outFace->GetLimits ().RemoveOutstanding ();
              NS_LOG_DEBUG ("Though there are Interests in queue, weighted round robin decided that packet is not allowed yet");
              break;
            }

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
//   super::DidReceiveValidNack (inFace, nackCode, pitEntry);
  
//   // NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
//   PitQueue::Remove (pitEntry);
 

//   Ptr<InterestHeader> nackHeader = Create<InterestHeader> (*pitEntry->GetInterest ());
//   // nackHeader->SetNack (100);
//   Ptr<Packet> pkt = Create<Packet> ();
//   pkt->AddHeader (*nackHeader);

//   for (pit::Entry::in_container::iterator face = pitEntry->GetIncoming ().begin ();
//        face != pitEntry->GetIncoming ().end ();
//        face ++)
//     {
//       face->m_face->Send (pkt->Copy ());
//     }


  
//   for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
//        face != pitEntry->GetOutgoing ().end ();
//        face ++)
//     {
//       face->m_face->GetLimits ().RemoveOutstanding ();
//     }

//   m_pit->MarkErased (pitEntry);
  
//   ProcessFromQueue ();
}

void
PerFibLimits::AnnounceLimits ()
{
  Ptr<L3Protocol> l3 = GetObject<L3Protocol> ();
  NS_ASSERT (l3 != 0);

  if (l3->GetNFaces () < 2)
    {
      m_announceEvent = Simulator::Schedule (Seconds (1.0),
                                             &PerFibLimits::AnnounceLimits, this);
      return;
    }
  
  double sumOfWeights = 0;
  double weightNormalization = 1.0;
  for (uint32_t faceId = 0; faceId < l3->GetNFaces (); faceId ++)
    {
      Ptr<Face> inFace = l3->GetFace (faceId);
      
      const ndnSIM::LoadStatsFace &stats = GetStatsTree ()["/"].incoming ().find (inFace)->second;
      double weight = std::min (1.0, stats.GetSatisfiedRatio ().get<0> ());
      if (weight < 0) weight = 0.5;

      sumOfWeights += weight;
    }
  if (sumOfWeights >= 1)
    {
      // disable normalization (not necessary)
      weightNormalization = 1.0;
    }
  else
    {
      // sumOfWeights /= (l3->GetNFaces ());
      weightNormalization = 1 / sumOfWeights;
    }

  for (Ptr<fib::Entry> entry = m_fib->Begin ();
       entry != m_fib->End ();
       entry = m_fib->Next (entry))
    {
      InterestHeader announceInterest;
      announceInterest.SetScope (0); // link-local

      uint32_t totalAllowance = 0;
      for (fib::FaceMetricContainer::type::iterator fibFace = entry->m_faces.begin ();
           fibFace != entry->m_faces.end ();
           fibFace ++)
        {
          totalAllowance += fibFace->m_face->GetLimits ().GetMaxLimit ();
        }
      
      if (totalAllowance == 0)
        {
          // don't announce anything, there is no limit
          continue;
        }
      
      for (uint32_t faceId = 0; faceId < l3->GetNFaces (); faceId ++)
        {
          Ptr<Face> inFace = l3->GetFace (faceId);

          const ndnSIM::LoadStatsFace &stats = GetStatsTree ()["/"].incoming ().find (inFace)->second;
          double weight = std::min (1.0, stats.GetSatisfiedRatio ().get<0> ());
          if (weight < 0) weight = 0.5;

          Ptr<NameComponents> prefixWithLimit = Create<NameComponents> (entry->GetPrefix ());
          (*prefixWithLimit)
            ("limit")
            (static_cast<uint32_t> (std::max (1.0, weightNormalization * weight * totalAllowance)));
          
          announceInterest.SetName (prefixWithLimit);
          // lifetime is 0

          Ptr<Packet> pkt = Create<Packet> ();
          pkt->AddHeader (announceInterest);

          inFace->Send (pkt);
        }
    }

  m_announceEvent = Simulator::Schedule (Seconds (1.0),
                                         &PerFibLimits::AnnounceLimits, this);
}

void
PerFibLimits::ApplyAnnouncedLimit (Ptr<Face> inFace,
                                   Ptr<const InterestHeader> header)
{
  // Ptr<fib::Entry> fibEntry = m_fib->LongestPrefixMatch (header);
  // if (fibEntry == 0)
  //   return;

  uint32_t limit = boost::lexical_cast<uint32_t> (header->GetName ().GetLastComponent ());
  inFace->GetLimits ().SetMaxLimit (limit);
  
  // if (Simulator::GetContext () == 6 || Simulator::GetContext () == 4)
  //   {
      // std::cerr << Simulator::Now ().ToDouble (Time::S) << "s  from:" << *inFace << " " << *header << std::endl;
      // std::cerr << header->GetName ().GetLastComponent () << ", " << boost::lexical_cast<uint32_t> (header->GetName ().GetLastComponent ()) << std::endl;
  //   }
}


} // namespace fw
} // namespace ndn
} // namespace ns3
