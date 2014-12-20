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

#include "ndn-limits-window.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ndn.Limits.Window");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (LimitsWindow);

TypeId
LimitsWindow::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Limits::Window")
    .SetGroupName ("Ndn")
    .SetParent <Limits> () 
    .AddConstructor <LimitsWindow> ()
    
    .AddTraceSource ("CurMaxLimit",
                     "Current maximum limit",
                     MakeTraceSourceAccessor (&LimitsWindow::m_curMaxLimit))

    .AddTraceSource ("Outstanding",
                     "Number of outstanding interests",
                     MakeTraceSourceAccessor (&LimitsWindow::m_outstanding))
    ;
  return tid;
}

void
LimitsWindow::UpdateCurrentLimit (double limit)
{
  NS_ASSERT_MSG (limit >= 0.0, "Limit should be greater or equal to zero");
  
  m_curMaxLimit = std::min (limit, GetMaxRate () * GetMaxDelay ());
}

bool
LimitsWindow::IsBelowLimit ()
{
  if (!IsEnabled ()) return true;

  return (m_curMaxLimit - m_outstanding >= 1.0);
}

void
LimitsWindow::BorrowLimit ()
{
  if (!IsEnabled ()) return; 

  NS_ASSERT_MSG (m_curMaxLimit - m_outstanding >= 1.0, "Should not be possible, unless we IsBelowLimit was not checked correctly");
  m_outstanding += 1;
}

void
LimitsWindow::ReturnLimit ()
{
  if (!IsEnabled ()) return; 

  NS_ASSERT_MSG (m_outstanding >= (uint32_t)1, "Should not be possible, unless we decreasing this number twice somewhere");
  m_outstanding -= 1;

  FireAvailableSlotCallback ();
}

} // namespace ndn
} // namespace ns3
