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

#include "ccnx-pit-entry.h"
#include "ccnx-name-components.h"
#include "ccnx-fib.h"

#include "ns3/simulator.h"

namespace ns3
{

CcnxPitEntry::CcnxPitEntry (Ptr<CcnxNameComponents> prefix,
                            const Time &expireTime,
                            const CcnxFibEntry &fibEntry)
  : m_prefix (prefix)
  , m_fibEntry (fibEntry)
  , m_expireTime (expireTime)
  , m_timerExpired (false)
  , m_counterExpirations (0)
{
}

CcnxPitEntryIncomingFaceContainer::type::iterator
CcnxPitEntry::AddIncoming (Ptr<CcnxFace> face)
{
  std::pair<CcnxPitEntryIncomingFaceContainer::type::iterator,bool> ret = 
    m_incoming.insert (CcnxPitEntryIncomingFace (face));

  NS_ASSERT_MSG (ret.second, "Something is wrong");

  return ret.first;
}

CcnxPitEntryOutgoingFaceContainer::type::iterator
CcnxPitEntry::AddOutgoing (Ptr<CcnxFace> face)
{
  std::pair<CcnxPitEntryOutgoingFaceContainer::type::iterator,bool> ret =
    m_outgoing.insert (CcnxPitEntryOutgoingFace (face));

  NS_ASSERT_MSG (ret.second, "Something is wrong");

  return ret.first;
}

void
CcnxPitEntry::RemoveAllReferencesToFace (Ptr<CcnxFace> face)
{
  CcnxPitEntryIncomingFaceContainer::type::iterator incoming =
    m_incoming.find (face);

  if (incoming != m_incoming.end ())
    m_incoming.erase (incoming);

  CcnxPitEntryOutgoingFaceContainer::type::iterator outgoing =
    m_outgoing.find (face);

  if (outgoing != m_outgoing.end ())
    m_outgoing.erase (outgoing);
}

}  
