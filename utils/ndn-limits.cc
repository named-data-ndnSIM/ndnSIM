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
Limits::SetMaxLimit (uint32_t max)
{
  m_maxLimit = max;
  m_curMaxLimit = max;
}


void
Limits::DecayCurrentLimit ()
{
  if (m_maxLimit == 0) return;
  
  if (!m_lastDecay.IsZero ())
    {
      const double tau = 100.0; // seconds
      double timeDiff = (Simulator::Now () - m_lastDecay).ToDouble (Time::S);

      NS_LOG_DEBUG ("m_maxLimit - (m_maxLimit - m_curMaxLimit) * exp (-timeDiff / tau)");
      NS_LOG_DEBUG (m_maxLimit << " - " << " ( " << m_maxLimit << " - " << (double)m_curMaxLimit << " ) " << " * " << " exp (- " << timeDiff << " / " << tau << " ) ");
      
      m_curMaxLimit = m_maxLimit - (m_maxLimit - m_curMaxLimit) * exp (-timeDiff / tau);
    }

  m_lastDecay = Simulator::Now ();
}

void
Limits::IncreaseLimit ()
{
  if (m_maxLimit == 0) return;
  
  // Additive increase
  m_curMaxLimit = std::min (1.0 * m_maxLimit,
                            (double)m_curMaxLimit + 1.0 / (double)m_curMaxLimit);
}

void
Limits::DecreaseLimit ()
{
  if (m_maxLimit == 0) return;
  
  const double maxDecreaseFrequency = 10.0;

  if (!m_lastDecrease.IsZero () && Simulator::Now () - m_lastDecrease < Seconds (1 / maxDecreaseFrequency))
    return;

  // Multiplicative decrease... almost
  m_curMaxLimit = 0.5 * m_curMaxLimit;
    
  m_lastDecrease = Simulator::Now ();
}


bool
Limits::IsBelowLimit ()
{
  if (m_maxLimit == 0) return true;

  if (m_curMaxLimit - m_outstanding > 1.0)
    {
      m_outstanding += 1;
      return true;
    }
  else
    return false;
}

void
Limits::RemoveOutstanding ()
{
  if (m_maxLimit == 0) return; //limits are disabled
  
  NS_LOG_DEBUG (m_outstanding);
  NS_ASSERT_MSG (m_outstanding >= 1, "Should not be possible, unless we decreasing this number twice somewhere");
  m_outstanding -= 1;
}

} // namespace ndn
} // namespace ns3
