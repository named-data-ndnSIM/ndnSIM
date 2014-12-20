/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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

#include "ndn-limits-rate.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"
#include "ns3/ndn-face.h"
#include "ns3/node.h"

NS_LOG_COMPONENT_DEFINE ("ndn.Limits.Rate");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (LimitsRate);

TypeId
LimitsRate::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Limits::Rate")
    .SetGroupName ("Ndn")
    .SetParent <Limits> ()
    .AddConstructor <LimitsRate> ()

    .AddAttribute ("RandomizeLeak", "Randomize start time for token bucket leakage. May be helpful to prevent leak synchronizations",
                   TimeValue (Seconds (0.001)),
                   MakeTimeAccessor (&LimitsRate::m_leakRandomizationInteral),
                   MakeTimeChecker ())

    ;
  return tid;
}

void
LimitsRate::NotifyNewAggregate ()
{
  super::NotifyNewAggregate ();

  if (!m_isLeakScheduled)
    {
      if (GetObject<Face> () != 0)
        {
          NS_ASSERT_MSG (GetObject<Face> ()->GetNode () != 0, "Node object should exist on the face");

          m_isLeakScheduled = true;

          if (!m_leakRandomizationInteral.IsZero ())
            {
              UniformVariable r (0.0, m_leakRandomizationInteral.ToDouble (Time::S));
              Simulator::ScheduleWithContext (GetObject<Face> ()->GetNode ()->GetId (),
                                              Seconds (r.GetValue ()), &LimitsRate::LeakBucket, this, 0.0);
            }
          else
            {
              Simulator::ScheduleWithContext (GetObject<Face> ()->GetNode ()->GetId (),
                                              Seconds (0), &LimitsRate::LeakBucket, this, 0.0);
            }

        }
    }
}

void
LimitsRate::SetLimits (double rate, double delay)
{
  super::SetLimits (rate, delay);

  // maximum allowed burst
  m_bucketMax = GetMaxRate () * GetMaxDelay ();

  // amount of packets allowed every second (leak rate)
  m_bucketLeak = GetMaxRate ();
}


void
LimitsRate::UpdateCurrentLimit (double limit)
{
  NS_ASSERT_MSG (limit >= 0.0, "Limit should be greater or equal to zero");

  m_bucketLeak = std::min (limit, GetMaxRate ());
  m_bucketMax  = m_bucketLeak * GetMaxDelay ();
}

bool
LimitsRate::IsBelowLimit ()
{
  if (!IsEnabled ()) return true;

  return (m_bucketMax - m_bucket >= 1.0);
}

void
LimitsRate::BorrowLimit ()
{
  if (!IsEnabled ()) return;

  NS_ASSERT_MSG (m_bucketMax - m_bucket >= 1.0, "Should not be possible, unless we IsBelowLimit was not checked correctly");
  m_bucket += 1;
}

void
LimitsRate::ReturnLimit ()
{
  // do nothing
}

void
LimitsRate::LeakBucket (double interval)
{
  const double leak = m_bucketLeak * interval;

#ifdef NS3_LOG_ENABLE
  if (m_bucket>1)
    {
      NS_LOG_DEBUG ("Leak from " << m_bucket << " to " << std::max (0.0, m_bucket - leak));
    }
#endif

  double bucketOld = m_bucket;

  m_bucket = std::max (0.0, m_bucket - leak);

  // calculate interval so next time we will leak by 1.001, unless such interval would be more than 1 second
  double newInterval = 1.0;
  if (m_bucketLeak > 1.0)
    {
      newInterval = 1.001 / m_bucketLeak;
    }

  if (m_bucketMax - bucketOld < 1.0 &&
      m_bucketMax - m_bucket >= 1.0) // limit number of times this stuff is called
    {
      this->FireAvailableSlotCallback ();
    }

  Simulator::Schedule (Seconds (newInterval), &LimitsRate::LeakBucket, this, newInterval);
}

} // namespace ndn
} // namespace ns3
