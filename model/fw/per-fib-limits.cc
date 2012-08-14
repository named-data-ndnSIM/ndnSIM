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

#include "per-fib-limits.h"

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
    .SetParent <FwStats> ()
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
  BestRoute::DoDispose ();
  m_decayLimitsEvent.Cancel ();
}

bool
PerFibLimits::WillSendOutInterest (const Ptr<Face> &outgoingFace,
                                   Ptr<InterestHeader> header,
                                   Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  // override all (if any) parent processing
  
  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outgoingFace);

  if (outgoing != pitEntry->GetOutgoing ().end ())
    {
      return false;
    }

  if (pitEntry->GetFibEntry ()->GetLimits ().IsBelowLimit ())
    {
      if (outgoingFace->GetLimits ()->IsBelowLimit ())
        {
          pitEntry->AddOutgoing (outgoingFace);
          return true;
        }
      else
        {
          pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
        }
    }
  
  return false;
}

void
PerFibLimits::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ()->RemoveOutstanding ();
      // face->m_face->GetLimits ()->DecreaseLimit (); !!! do not decrease per-face limit. it doesn't make sense !!!
    }
  
  pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
  pitEntry->GetFibEntry ()->GetLimits ().DecreaseLimit (); // multiplicative decrease

  if (!m_decayLimitsEvent.IsRunning ())
    m_decayLimitsEvent = Simulator::Schedule (Seconds (1.0), &PerFibLimits::DecayLimits, this);
}


void
PerFibLimits::WillSatisfyPendingInterest (const Ptr<Face> &incomingFace,
                                          Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  super::WillSatisfyPendingInterest (incomingFace, pitEntry);

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ()->RemoveOutstanding ();
      // face->m_face->GetLimits ()->IncreaseLimit (); !!! do not increase (as do not decrease) per-face limit. again, it doesn't make sense
    }
  
  pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
  pitEntry->GetFibEntry ()->GetLimits ().IncreaseLimit (); // additive increase
}


// void
// PerFibLimits::DidReceiveValidNack (const Ptr<Face> &incomingFace,
//                                    uint32_t nackCode,
//                                    Ptr<pit::Entry> pitEntry)
// {
//   // super::DidReceiveValidNack (incomingFace, nackCode, pitEntry);

//   // ??
// }

void
PerFibLimits::DecayLimits ()
{
  for (Ptr<fib::Entry> entry = m_fib->Begin ();
       entry != m_fib->End ();
       entry = m_fib->Next (entry))
    {
      entry->GetLimits ().DecayCurrentLimit ();
    }

  m_decayLimitsEvent = Simulator::Schedule (Seconds (1.0), &PerFibLimits::DecayLimits, this);
}


} // namespace fw
} // namespace ndn
} // namespace ns3
