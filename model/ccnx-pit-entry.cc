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

const CcnxNameComponents &
CcnxPitEntry::GetPrefix () const
{
  return *m_prefix;
}

// void
// CcnxPitEntry::AddIncoming (Ptr<CcnxFace> face)
// {
//   m_incoming.insert (CcnxPitEntryIncomingFace (face,     )
// }


// CcnxPitEntry::UpdateFibStatus::UpdateFibStatus (Ptr<CcnxFace> face,
//                                                 CcnxFibFaceMetric::Status status,
//                                                 Ptr<CcnxFib> fib)
//   : m_face (face)
//   , m_status (status)
//   , m_fib (fib)
// {
// }

// void
// CcnxPitEntry::UpdateFibStatus::operator() (CcnxPitEntry &entry)
// {
//   NS_ASSERT_MSG (false, "Broken");
//   m_fib->modify (m_fib->iterator_to (entry.m_fibEntry),
//                  CcnxFibEntry::UpdateStatus (m_face, m_status));
// }

// void
// CcnxPitEntry::EstimateRttAndRemoveFace::operator() (CcnxPitEntry &entry)
// {
//   // similar to Karn's Algorithm, we don't use RTT measurements for retx packets
//   if (m_outFace->m_retxNum>0)
//     return;

//   m_fib->modify (m_fib->iterator_to (entry.m_fibEntry),
//                 CcnxFibEntry::UpdateFaceRtt (m_outFace->m_face,
//                                              Simulator::Now() - m_outFace->m_sendTime));

//   entry.m_outgoing.erase (m_outFace);
// }

}  
