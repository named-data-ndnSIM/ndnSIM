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

    .AddAttribute ("ExpDecayTau", "Parameter 'tau' for the exponential delay of the current maximum limit variable",
                   TimeValue (Seconds (100.0)),
                   MakeTimeAccessor (&Limits::m_exponentialDecayTau),
                   MakeTimeChecker ()
                   )

    .AddAttribute ("NonDecreasePeriod", "Only one decrease is allowed per one NonDecreasePeriod",
                   TimeValue (Seconds (0.1)),
                   MakeTimeAccessor (&Limits::m_nonDecreasePeriod),
                   MakeTimeChecker ()
                   )

    .AddAttribute ("AdditiveIncrease", "Parameter for additive increase",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&Limits::m_additiveIncrease),
                   MakeDoubleChecker<double> ()
                   )

    .AddAttribute ("MultiplicativeDecrease", "Parameter for multiplicative decrease",
                   DoubleValue (0.5),
                   MakeDoubleAccessor (&Limits::m_multiplicativeDecrease),
                   MakeDoubleChecker<double> ()
                   )
    
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

uint32_t
Limits::GetMaxLimit () const
{
  return m_maxLimit;
}

void
Limits::DecayCurrentLimit ()
{
  if (!IsEnabled ()) return;
  
  if (!m_lastDecay.IsZero ())
    {
      double timeDiff = (Simulator::Now () - m_lastDecay).ToDouble (Time::S);

      NS_LOG_DEBUG ("m_maxLimit - (m_maxLimit - m_curMaxLimit) * exp (-timeDiff / tau)");
      NS_LOG_DEBUG (m_maxLimit << " - " << " ( " << m_maxLimit << " - " << (double)m_curMaxLimit << " ) " << " * " << " exp (- " << timeDiff << " / " << m_exponentialDecayTau.ToDouble (Time::S) << " ) ");
      
      m_curMaxLimit = m_maxLimit - (m_maxLimit - m_curMaxLimit) * exp (-timeDiff / m_exponentialDecayTau.ToDouble (Time::S));
    }

  m_lastDecay = Simulator::Now ();
}

void
Limits::IncreaseLimit ()
{
  if (!IsEnabled ()) return;
  
  // Additive increase
  m_curMaxLimit = std::min (1.0 * m_maxLimit,
                            (double)m_curMaxLimit + m_additiveIncrease / (double)m_curMaxLimit);
}

void
Limits::DecreaseLimit ()
{
  if (!IsEnabled ()) return;

//   m_curMaxLimit = std::max (0.0,
//                             (double)m_curMaxLimit - m_multiplicativeDecrease / (double)m_curMaxLimit);
// }
  
  if (m_lastDecrease.IsZero () ||
      (!m_lastDecrease.IsZero () && Simulator::Now () - m_lastDecrease > m_nonDecreasePeriod)) // allow
    {
      // Multiplicative decrease... almost
      m_curMaxLimit = m_multiplicativeDecrease * m_curMaxLimit;
    
      m_lastDecrease = Simulator::Now ();
    }
}


bool
Limits::IsBelowLimit ()
{
  if (!IsEnabled ()) return true;

  if (m_curMaxLimit - m_outstanding > 1.0)
    {
      // static UniformVariable acceptanceProbability (0, m_curMaxLimit);
      // double value = acceptanceProbability.GetValue ();
      double value = m_outstanding+ 1;
      
      if (m_outstanding < value)
        {
          m_outstanding += 1.0;
          return true;
        }
      else
        return false;
      
      // m_outstanding += 1;
      // return true;
    }
  else
    return false;
}

void
Limits::RemoveOutstanding ()
{
  if (!IsEnabled ()) return; //limits are disabled
  
  NS_LOG_DEBUG (m_outstanding);
  NS_ASSERT_MSG (m_outstanding >= 1, "Should not be possible, unless we decreasing this number twice somewhere");
  m_outstanding -= 1;
}

} // namespace ndn
} // namespace ns3
