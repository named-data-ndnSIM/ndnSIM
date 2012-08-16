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
    .SetParent <FwStats> ()
    .AddConstructor <PerFibLimits> ()

    .AddAttribute ("Threshold", "Minimum number of incoming interests to enable dropping decision",
                   DoubleValue (0.25),
                   MakeDoubleAccessor (&PerFibLimits::m_threshold),
                   MakeDoubleChecker<double> ())
    
    .AddAttribute ("GraceAcceptProbability", "Probability to accept Interest even though stats telling that satisfaction ratio is 0",
                   DoubleValue (0.01),
                   MakeDoubleAccessor (&PerFibLimits::m_graceAcceptProbability),
                   MakeDoubleChecker<double> ())
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

  // check stats
  Ptr<Face> inFace = pitEntry->GetIncoming ().begin ()->m_face;
  // const ndnSIM::LoadStatsFace &stats = GetStatsTree ()[header->GetName ()].incoming ().find (inFace)->second;
  const ndnSIM::LoadStatsFace &stats = GetStatsTree ()["/"].incoming ().find (inFace)->second;

  if (stats.count ().GetStats ().get<0> () >= m_threshold * pitEntry->GetFibEntry ()->GetLimits ().GetMaxLimit ())
  {
    double ratio = std::min (1.0, stats.GetSatisfiedRatio ().get<0> ());
    // NS_ASSERT_MSG (ratio > 0, "If count is a reasonable value, ratio cannot be negative");
    UniformVariable randAccept (0, 1);
    double dice = randAccept.GetValue ();
    if (ratio < 0 || dice < ratio + m_graceAcceptProbability)
      {
        // ok, accepting the interests
      }
    else
      {
        // boo. bad luck
        return false;
      }
  }
  
  if (pitEntry->GetFibEntry ()->GetLimits ().IsBelowLimit ())
    {
      if (outgoingFace->GetLimits ().IsBelowLimit ())
        {
          pitEntry->AddOutgoing (outgoingFace);
          return true;
        }
      else
        {
          NS_LOG_DEBUG ("Face limit. Reverting back per-prefix allowance");
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
      face->m_face->GetLimits ().RemoveOutstanding ();
      // face->m_face->GetLimits ()->DecreaseLimit (); !!! do not decrease per-face limit. it doesn't make sense !!!
    }
  
  pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
  // pitEntry->GetFibEntry ()->GetLimits ().DecreaseLimit (); // multiplicative decrease

  if (!m_decayLimitsEvent.IsRunning ())
    {
      UniformVariable rand (0,5);
      m_decayLimitsEvent = Simulator::Schedule (Seconds (1.0) + Seconds (0.001 * rand.GetValue ()), &PerFibLimits::DecayLimits, this);
    }
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
      face->m_face->GetLimits ().RemoveOutstanding ();
      // face->m_face->GetLimits ()->IncreaseLimit (); !!! do not increase (as do not decrease) per-face limit. again, it doesn't make sense
    }
  
  pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
  // pitEntry->GetFibEntry ()->GetLimits ().IncreaseLimit (); // additive increase
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
