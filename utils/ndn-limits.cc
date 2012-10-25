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

#include "ndn-limits.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/random-variable.h"

NS_LOG_COMPONENT_DEFINE ("ndn.Limits");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (Limits);

TypeId
Limits::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Limits")
    .SetGroupName ("Ndn")
    .SetParent <Object> ()
    .AddConstructor <Limits> ()
    
    .AddTraceSource ("CurMaxLimit",
                     "Current maximum limit",
                     MakeTraceSourceAccessor (&Limits::m_curMaxLimit))
                     
    .AddTraceSource ("Outstanding",
                     "Number of outstanding interests",
                     MakeTraceSourceAccessor (&Limits::m_outstanding))
    ;
  return tid;
}

void
Limits::SetMaxLimit (double max)
{
  m_maxLimit = max;
  m_curMaxLimit = max;
}

double
Limits::GetMaxLimit () const
{
  return m_maxLimit;
}

void
Limits::UpdateCurrentLimit (double limit)
{
  NS_ASSERT_MSG (limit >= 0.0, "Limit should be greater or equal to zero");
  
  m_curMaxLimit = std::min (limit, m_maxLimit);
}

bool
Limits::IsBelowLimit ()
{
  if (!IsEnabled ()) return true;

  if (m_curMaxLimit - m_outstanding >= 1.0)
    {
      // static UniformVariable acceptanceProbability (0, m_curMaxLimit);
      // double value = acceptanceProbability.GetValue ();
      double value = m_outstanding + 1;
      
      if (m_outstanding < value)
        {
          m_outstanding += 1.0;
          return true;
        }
      else
        return false;
    }
  else
    return false;
}

void
Limits::RemoveOutstanding ()
{
  if (!IsEnabled ()) return; 

  NS_LOG_DEBUG (m_outstanding);
  NS_ASSERT_MSG (m_outstanding >= (uint32_t)1, "Should not be possible, unless we decreasing this number twice somewhere");
  m_outstanding -= 1;
}

} // namespace ndn
} // namespace ns3
