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

// struct SearchByFace
// {
//   /**
//    * \brief To perform effective searches by CcnxFace
//    */
//   bool
//   operator() (const CcnxPitIncomingInterest &m, const Ptr<CcnxFace> &face) const
//   {
//     return *(m.m_face) < *face;
//   } 

//   /**
//    * \brief To perform effective searches by CcnxFace
//    */
//   bool
//   operator() (const Ptr<CcnxFace> &face, const CcnxPitIncomingInterest &m) const
//   {
//     return *face < *(m.m_face);
//   } 

//   /**
//    * \brief To perform effective searches by CcnxFace
//    */
//   bool
//   operator() (const CcnxPitOutgoingInterest &m, const Ptr<CcnxFace> &face) const
//   {
//     return *(m.m_face) < *face;
//   } 

//   /**
//    * \brief To perform effective searches by CcnxFace
//    */
//   bool
//   operator() (const Ptr<CcnxFace> &face, const CcnxPitOutgoingInterest &m) const
//   {
//     return *face < *(m.m_face);
//   } 
// };


CcnxPitEntry::CcnxPitEntry (Ptr<CcnxNameComponents> prefix, const CcnxFibEntry &fibEntry)
  : m_prefix (prefix)
  , m_fibEntry (fibEntry)
  // , m_expireTime (?)
  , m_timerExpired (false)
  , m_counterExpirations (0)
{
}

const CcnxNameComponents &
CcnxPitEntry::GetPrefix () const
{
  return *m_prefix;
}

// CcnxPitEntry::SetFibEntry::SetFibEntry (Ptr<CcnxFibEntry> fib)
//   : m_fib (fib)
// {
// }

// void
// CcnxPitEntry::SetFibEntry::operator() (CcnxPitEntry &entry)
// {
//   entry.m_fib = m_fib;
// }

void
CcnxPitEntry::AddIncoming::operator() (CcnxPitEntry &entry)
{
  entry.m_incoming.insert (CcnxPitEntryIncomingFace (m_face));
}

void
CcnxPitEntry::DeleteIncoming::operator() (CcnxPitEntry &entry)
{
  entry.m_incoming.erase (m_face);
}

void
CcnxPitEntry::AddOutgoing::operator() (CcnxPitEntry &entry)
{
  entry.m_outgoing.insert (CcnxPitEntryOutgoingFace (m_face));
}

void
CcnxPitEntry::DeleteOutgoing::operator() (CcnxPitEntry &entry)
{
  entry.m_outgoing.erase (m_face);
}

void
CcnxPitEntry::ClearIncoming::operator() (CcnxPitEntry &entry)
{
  entry.m_incoming.clear ();
}

CcnxPitEntry::UpdateFibStatus::UpdateFibStatus (Ptr<CcnxFace> face, CcnxFibFaceMetric::Status status)
  : m_face (face)
  , m_status (status)
{
}

void
CcnxPitEntry::UpdateFibStatus::operator() (CcnxPitEntry &entry)
{
  NS_ASSERT_MSG (false, "Broken");
  // entry.m_fib->UpdateStatus (m_face, m_status);
}

void
CcnxPitEntry::EstimateRttAndRemoveFace::operator() (CcnxPitEntry &entry)
{
  // similar to Karn's Algorithm, we don't use RTT measurements for retx packets
  if (m_outFace->m_retxNum>0)
    return;

  m_fib->modify (m_fib->iterator_to (entry.m_fibEntry),
                CcnxFibEntry::UpdateFaceRtt (m_outFace->m_face,
                                             Simulator::Now() - m_outFace->m_sendTime));

  entry.m_outgoing.erase (m_outFace);
}

}  
