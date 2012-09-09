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

#include "dynamic-limits.h"

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

NS_LOG_COMPONENT_DEFINE ("ndn.fw.DynamicLimits");

namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (DynamicLimits);
  
TypeId
DynamicLimits::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::DynamicLimits")
    .SetGroupName ("Ndn")
    .SetParent <super> ()
    .AddConstructor <DynamicLimits> ()

    .AddAttribute ("AnnounceLimits", "Enable limit announcement using scope 0 interests",
                   BooleanValue (false),
                   MakeBooleanAccessor (&DynamicLimits::m_announceLimits),
                   MakeBooleanChecker ())

    ;
  return tid;
}
    
DynamicLimits::DynamicLimits ()
{
}

void
DynamicLimits::DoDispose ()
{
  m_announceEvent.Cancel ();
  
  super::DoDispose ();
}

void
DynamicLimits::NotifyNewAggregate ()
{
  super::NotifyNewAggregate ();

  if (m_announceLimits)
    {
      if (m_pit != 0 && m_fib != 0)
        {
          if (!m_announceEvent.IsRunning ())
            {
              m_announceEvent = Simulator::Schedule (Seconds (1.0),
                                                     &DynamicLimits::AnnounceLimits, this);
            }
        }
    }
}

void
DynamicLimits::OnInterest (Ptr<Face> face,
                          Ptr<const InterestHeader> header,
                          Ptr<const Packet> origPacket)
{
  if (header->GetScope () != 0)
    super::OnInterest (face, header, origPacket);
  else
    ApplyAnnouncedLimit (face, header);
}


void
DynamicLimits::AnnounceLimits ()
{
  Ptr<L3Protocol> l3 = GetObject<L3Protocol> ();
  NS_ASSERT (l3 != 0);

  if (l3->GetNFaces () < 2)
    {
      m_announceEvent = Simulator::Schedule (Seconds (1.0),
                                             &DynamicLimits::AnnounceLimits, this);
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
                                         &DynamicLimits::AnnounceLimits, this);
}

void
DynamicLimits::ApplyAnnouncedLimit (Ptr<Face> inFace,
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
