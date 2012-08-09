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
 */

#include "nacks.h"

#include "ns3/ndn-pit.h"
#include "ns3/ndn-pit-entry.h"
#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"
#include "ns3/ndn-pit.h"
#include "ns3/ndn-fib.h"
#include "ns3/ndn-content-store.h"

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

NS_LOG_COMPONENT_DEFINE ("ndn.fw.Nacks");

namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (Nacks);

TypeId
Nacks::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::Nacks")
    .SetGroupName ("Ndn")
    .SetParent<ForwardingStrategy> ()
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutNacks",  "OutNacks",  MakeTraceSourceAccessor (&Nacks::m_outNacks))
    .AddTraceSource ("InNacks",   "InNacks",   MakeTraceSourceAccessor (&Nacks::m_inNacks))
    .AddTraceSource ("DropNacks", "DropNacks", MakeTraceSourceAccessor (&Nacks::m_dropNacks))
    
    .AddAttribute ("EnableNACKs", "Enabling support of NACKs",
                   BooleanValue (false),
                   MakeBooleanAccessor (&Nacks::m_nacksEnabled),
                   MakeBooleanChecker ())
    ;
  return tid;
}

void
Nacks::OnInterest (const Ptr<Face> &incomingFace,
                   Ptr<InterestHeader> &header,
                   const Ptr<const Packet> &packet)
{
  if (header->GetNack () > 0)
    OnNack (incomingFace, header, packet/*original packet*/);
  else
    super::OnInterest (incomingFace, header, packet/*original packet*/);  
}

void
Nacks::OnNack (const Ptr<Face> &incomingFace,
               Ptr<InterestHeader> &header,
               const Ptr<const Packet> &packet)
{
  NS_ASSERT (m_nacksEnabled);

  // NS_LOG_FUNCTION (incomingFace << header << packet);
  m_inNacks (header, incomingFace);

  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*header);
  if (pitEntry == 0)
    {
      // somebody is doing something bad
      m_dropNacks (header, incomingFace);
      return;
    }
  
  // This was done in error. Never, never do anything, except normal leakage. This way we ensure that we will not have losses,
  // at least when there is only one client
  //
  // incomingFace->LeakBucketByOnePacket ();

  pitEntry->SetWaitingInVain (incomingFace);

  DidReceiveValidNack (incomingFace, header->GetNack (), pitEntry);
  
  if (!pitEntry->AreAllOutgoingInVain ()) // not all ougtoing are in vain
    {
      NS_LOG_DEBUG ("Not all outgoing are in vain");
      // suppress
      // Don't do anything, we are still expecting data from some other face
      m_dropNacks (header, incomingFace);
      return;
    }
  
  Ptr<Packet> nonNackInterest = Create<Packet> ();
  header->SetNack (InterestHeader::NORMAL_INTEREST);
  nonNackInterest->AddHeader (*header);
  
  bool propagated = DoPropagateInterest (incomingFace, header, nonNackInterest, pitEntry);
  if (!propagated)
    {
      DidExhaustForwardingOptions (incomingFace, header, nonNackInterest, pitEntry);
    }  
}

void
Nacks::DidReceiveDuplicateInterest (const Ptr<Face> &incomingFace,
                                    Ptr<InterestHeader> &header,
                                    const Ptr<const Packet> &packet,
                                    Ptr<pit::Entry> pitEntry)
{
  super::DidReceiveDuplicateInterest (incomingFace, header, packet, pitEntry);

  if (m_nacksEnabled)
    {
      NS_LOG_DEBUG ("Sending NACK_LOOP");
      header->SetNack (InterestHeader::NACK_LOOP);
      Ptr<Packet> nack = Create<Packet> ();
      nack->AddHeader (*header);

      incomingFace->Send (nack);
      m_outNacks (header, incomingFace);
    }
}

void
Nacks::DidExhaustForwardingOptions (const Ptr<Face> &incomingFace,
                                    Ptr<InterestHeader> header,
                                    const Ptr<const Packet> &packet,
                                    Ptr<pit::Entry> pitEntry)
{
  super::DidExhaustForwardingOptions (incomingFace, header, packet, pitEntry);

  if (m_nacksEnabled)
    {
      Ptr<Packet> packet = Create<Packet> ();
      header->SetNack (InterestHeader::NACK_GIVEUP_PIT);
      packet->AddHeader (*header);

      BOOST_FOREACH (const pit::IncomingFace &incoming, pitEntry->GetIncoming ())
        {
          NS_LOG_DEBUG ("Send NACK for " << boost::cref (header->GetName ()) << " to " << boost::cref (*incoming.m_face));
          incoming.m_face->Send (packet->Copy ());

          m_outNacks (header, incoming.m_face);
        }
  
      // All incoming interests cannot be satisfied. Remove them
      pitEntry->ClearIncoming ();

      // Remove also outgoing
      pitEntry->ClearOutgoing ();
  
      // Set pruning timout on PIT entry (instead of deleting the record)
      m_pit->MarkErased (pitEntry);
    }  
}

void
Nacks::DidReceiveValidNack (const Ptr<Face> &incomingFace,
                            uint32_t nackCode,
                            Ptr<pit::Entry> pitEntry)
{
  // If NACK is NACK_GIVEUP_PIT, then neighbor gave up trying to and removed it's PIT entry.
  // So, if we had an incoming entry to this neighbor, then we can remove it now
  if (nackCode == InterestHeader::NACK_GIVEUP_PIT)
    {
      pitEntry->RemoveIncoming (incomingFace);
    }

  pitEntry->GetFibEntry ()->UpdateStatus (incomingFace, fib::FaceMetric::NDN_FIB_YELLOW);
}

} // namespace fw
} // namespace ndn
} // namespace ns3
