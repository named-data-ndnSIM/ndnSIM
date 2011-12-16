/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2007 INRIA
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

#include "ns3/assert.h" 
#include "ns3/node.h" 
#include "ns3/boolean.h"

#include "ccnx.h"
#include "ccnx-face.h"
#include "ccnx-forwarding-strategy.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Ccnx);

TypeId 
Ccnx::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ccnx")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    // "OutInterests" trace is inside CcnxForwardingStrategy
    .AddTraceSource ("InInterests",   "InInterests",   MakeTraceSourceAccessor (&Ccnx::m_inInterests))
    .AddTraceSource ("DropInterests", "DropInterests", MakeTraceSourceAccessor (&Ccnx::m_dropInterests))
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutNacks",  "OutNacks",  MakeTraceSourceAccessor (&Ccnx::m_outNacks))
    .AddTraceSource ("InNacks",   "InNacks",   MakeTraceSourceAccessor (&Ccnx::m_inNacks))
    .AddTraceSource ("DropNacks", "DropNacks", MakeTraceSourceAccessor (&Ccnx::m_dropNacks))
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    .AddTraceSource ("OutData",  "OutData",  MakeTraceSourceAccessor (&Ccnx::m_outData))
    .AddTraceSource ("InData",   "InData",   MakeTraceSourceAccessor (&Ccnx::m_inData))
    .AddTraceSource ("DropData", "DropData", MakeTraceSourceAccessor (&Ccnx::m_dropData))
    
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
  ;
  return tid;
}

Ccnx::~Ccnx ()
{
}

} // namespace ns3
