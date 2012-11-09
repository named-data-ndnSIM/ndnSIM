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

TypeId
Limits::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::Limits")
    .SetGroupName ("Ndn")
    .SetParent <Object> ()
    
    ;
  return tid;
}

Limits::Limits ()
  : m_maxRate (-1)
  , m_maxDelay (1.0)
  , m_handler (MakeNullCallback<void> ())
  , m_linkDelay (0)
{
}


void
Limits::RegisterAvailableSlotCallback (CallbackHandler handler)
{
  m_handler = handler;
}

void
Limits::FireAvailableSlotCallback ()
{
  if (!m_handler.IsNull ())
    m_handler ();
}


} // namespace ndn
} // namespace ns3
