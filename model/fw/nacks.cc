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
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"
#include "ns3/ndn-pit.h"
#include "ns3/ndn-fib.h"
#include "ns3/ndn-content-store.h"
#include "ns3/ndnSIM/utils/ndn-fw-hop-count-tag.h"

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
Nacks::OnInterest (Ptr<Face> inFace,
                   Ptr<Interest> interest)
{
  if (interest->GetNack () > 0)
    OnNack (inFace, interest);
  else
    super::OnInterest (inFace, interest);
}

void
Nacks::OnNack (Ptr<Face> inFace,
               Ptr<Interest> nack)
{
  // NS_LOG_FUNCTION (inFace << nack->GetName ());
  m_inNacks (nack, inFace);

  Ptr<pit::Entry> pitEntry = m_pit->Lookup (*nack);
  if (pitEntry == 0)
    {
      // somebody is doing something bad
      m_dropNacks (nack, inFace);
      return;
    }

  DidReceiveValidNack (inFace, nack->GetNack (), nack, pitEntry);
}

void
Nacks::DidReceiveDuplicateInterest (Ptr<Face> inFace,
                                    Ptr<const Interest> interest,
                                    Ptr<pit::Entry> pitEntry)
{
  super::DidReceiveDuplicateInterest (inFace, interest, pitEntry);

  if (m_nacksEnabled)
    {
      NS_LOG_DEBUG ("Sending NACK_LOOP");
      Ptr<Interest> nack = Create<Interest> (*interest);
      nack->SetNack (Interest::NACK_LOOP);

      inFace->SendInterest (nack);
      m_outNacks (nack, inFace);
    }
}

void
Nacks::DidExhaustForwardingOptions (Ptr<Face> inFace,
                                    Ptr<const Interest> interest,
                                    Ptr<pit::Entry> pitEntry)
{
  if (m_nacksEnabled)
    {
      Ptr<Interest> nack = Create<Interest> (*interest);
      nack->SetNack (Interest::NACK_GIVEUP_PIT);

      BOOST_FOREACH (const pit::IncomingFace &incoming, pitEntry->GetIncoming ())
        {
          NS_LOG_DEBUG ("Send NACK for " << boost::cref (nack->GetName ()) << " to " << boost::cref (*incoming.m_face));
          incoming.m_face->SendInterest (nack);
          m_outNacks (nack, incoming.m_face);
        }

      pitEntry->ClearOutgoing (); // to force erasure of the record
    }

  super::DidExhaustForwardingOptions (inFace, interest, pitEntry);
}

void
Nacks::DidReceiveValidNack (Ptr<Face> inFace,
                            uint32_t nackCode,
                            Ptr<const Interest> nack,
                            Ptr<pit::Entry> pitEntry)
{
  NS_LOG_DEBUG ("nackCode: " << nackCode << " for [" << nack->GetName () << "]");

  // If NACK is NACK_GIVEUP_PIT, then neighbor gave up trying to and removed it's PIT entry.
  // So, if we had an incoming entry to this neighbor, then we can remove it now
  if (nackCode == Interest::NACK_GIVEUP_PIT)
    {
      pitEntry->RemoveIncoming (inFace);
    }

  if (nackCode == Interest::NACK_LOOP ||
      nackCode == Interest::NACK_CONGESTION ||
      nackCode == Interest::NACK_GIVEUP_PIT)
    {
      pitEntry->SetWaitingInVain (inFace);

      if (!pitEntry->AreAllOutgoingInVain ()) // not all ougtoing are in vain
        {
          NS_LOG_DEBUG ("Not all outgoing are in vain");
          // suppress
          // Don't do anything, we are still expecting data from some other face
          m_dropNacks (nack, inFace);
          return;
        }

      Ptr<Interest> interest = Create<Interest> (*nack);
      interest->SetNack (Interest::NORMAL_INTEREST);

      bool propagated = DoPropagateInterest (inFace, interest, pitEntry);
      if (!propagated)
        {
          DidExhaustForwardingOptions (inFace, interest, pitEntry);
        }
    }
}

} // namespace fw
} // namespace ndn
} // namespace ns3
