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

#include "stats-based-randomized-interest-accept.h"

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

NS_LOG_COMPONENT_DEFINE ("ndn.fw.StatsBasedRandomizedInterestAccept");

namespace ns3 {
namespace ndn {
namespace fw {

NS_OBJECT_ENSURE_REGISTERED (StatsBasedRandomizedInterestAccept);
  
TypeId
StatsBasedRandomizedInterestAccept::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fw::StatsBasedRandomizedInterestAccept")
    .SetGroupName ("Ndn")
    .SetParent <super> ()
    .AddConstructor <StatsBasedRandomizedInterestAccept> ()

    .AddAttribute ("Threshold", "Minimum number of incoming interests to enable dropping decision",
                   DoubleValue (0.25),
                   MakeDoubleAccessor (&StatsBasedRandomizedInterestAccept::m_threshold),
                   MakeDoubleChecker<double> ())
    
    .AddAttribute ("GraceAcceptProbability", "Probability to accept Interest even though stats telling that satisfaction ratio is 0",
                   DoubleValue (0.01),
                   MakeDoubleAccessor (&StatsBasedRandomizedInterestAccept::m_graceAcceptProbability),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}
    
StatsBasedRandomizedInterestAccept::StatsBasedRandomizedInterestAccept ()
{
}

void
StatsBasedRandomizedInterestAccept::DoDispose ()
{
  super::DoDispose ();
}

bool
StatsBasedRandomizedInterestAccept::WillSendOutInterest (Ptr<Face> outFace,
                                                         Ptr<const InterestHeader> header,
                                                         Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());
  // override all (if any) parent processing
  
  pit::Entry::out_iterator outgoing =
    pitEntry->GetOutgoing ().find (outFace);

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
      if (outFace->GetLimits ().IsBelowLimit ())
        {
          pitEntry->AddOutgoing (outFace);
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
StatsBasedRandomizedInterestAccept::WillEraseTimedOutPendingInterest (Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }
  
  pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
}


void
StatsBasedRandomizedInterestAccept::WillSatisfyPendingInterest (Ptr<Face> inFace,
                                                                Ptr<pit::Entry> pitEntry)
{
  NS_LOG_FUNCTION (this << pitEntry->GetPrefix ());

  super::WillSatisfyPendingInterest (inFace, pitEntry);

  for (pit::Entry::out_container::iterator face = pitEntry->GetOutgoing ().begin ();
       face != pitEntry->GetOutgoing ().end ();
       face ++)
    {
      face->m_face->GetLimits ().RemoveOutstanding ();
    }
  
  pitEntry->GetFibEntry ()->GetLimits ().RemoveOutstanding ();
}


} // namespace fw
} // namespace ndn
} // namespace ns3
