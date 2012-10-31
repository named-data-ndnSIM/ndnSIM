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
    
    ;
  return tid;
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

  m_bucket = std::max (0.0, m_bucket - leak);
}

} // namespace ndn
} // namespace ns3
