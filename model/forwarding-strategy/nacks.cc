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

NS_LOG_COMPONENT_DEFINE ("NdnSimNacks");

namespace ns3 {
namespace ndnSIM {

NS_OBJECT_ENSURE_REGISTERED (Nacks);

TypeId
Nacks::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndnSIM::Nacks")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxForwardingStrategy> ()
    
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
Nacks::OnInterest (const Ptr<CcnxFace> &incomingFace,
                   Ptr<CcnxInterestHeader> &header,
                   const Ptr<const Packet> &packet)
{
  if (header->GetNack () > 0)
    OnNack (incomingFace, header, packet/*original packet*/);
  else
    super::OnInterest (incomingFace, header, packet/*original packet*/);  
}

void
Nacks::OnNack (const Ptr<CcnxFace> &face,
               Ptr<CcnxInterestHeader> &header,
               const Ptr<const Packet> &p)
{
  NS_ASSERT (false);
}

void
Nacks::DidReceiveDuplicateInterest (const Ptr<CcnxFace> &incomingFace,
                                    Ptr<CcnxInterestHeader> &header,
                                    const Ptr<const Packet> &packet,
                                    Ptr<CcnxPitEntry> pitEntry)
{
  super::DidReceiveDuplicateInterest (incomingFace, header, packet, pitEntry);

  if (m_nacksEnabled)
    {
      NS_LOG_DEBUG ("Sending NACK_LOOP");
      header->SetNack (CcnxInterestHeader::NACK_LOOP);
      Ptr<Packet> nack = Create<Packet> ();
      nack->AddHeader (*header);

      incomingFace->Send (nack);
      m_outNacks (header, incomingFace);
    }
}

void
Nacks::DidExhaustForwardingOptions (const Ptr<CcnxFace> &incomingFace,
                                    Ptr<CcnxInterestHeader> header,
                                    const Ptr<const Packet> &packet,
                                    Ptr<CcnxPitEntry> pitEntry)
{
  super::DidExhaustForwardingOptions (incomingFace, header, packet, pitEntry);

  if (m_nacksEnabled)
    {
      Ptr<Packet> packet = Create<Packet> ();
      header->SetNack (CcnxInterestHeader::NACK_GIVEUP_PIT);
      packet->AddHeader (*header);

      BOOST_FOREACH (const CcnxPitEntryIncomingFace &incoming, pitEntry->GetIncoming ())
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


} // namespace ndnSIM
} // namespace ns3
