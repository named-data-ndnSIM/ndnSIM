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
#include "ns3/log.h"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxPitEntry");

namespace ns3
{

CcnxPitEntry::CcnxPitEntry (Ptr<CcnxNameComponents> prefix,
                            const Time &expireTime,
                            const CcnxFibEntry &fibEntry)
  : m_prefix (prefix)
  , m_fibEntry (fibEntry)
  , m_expireTime (Simulator::Now () + expireTime)
  // , m_timerExpired (false)
  // , m_counterExpirations (0)
  , m_maxRetxCount (0)
{
}

void
CcnxPitEntry::SetExpireTime (const Time &expireTime)
{
  NS_LOG_FUNCTION (expireTime.ToDouble (Time::S));
  m_expireTime = expireTime;
}


void
CcnxPitEntry::UpdateLifetime (const Time &offsetTime)
{
  NS_LOG_FUNCTION (offsetTime.ToDouble (Time::S));
  
  Time newExpireTime = Simulator::Now () + offsetTime;
  if (newExpireTime > m_expireTime)
    m_expireTime = newExpireTime;
  
  NS_LOG_INFO ("Updated lifetime to " << m_expireTime.ToDouble (Time::S));
}

CcnxPitEntryIncomingFaceContainer::type::iterator
CcnxPitEntry::AddIncoming (Ptr<CcnxFace> face)
{
  std::pair<CcnxPitEntryIncomingFaceContainer::type::iterator,bool> ret = 
    m_incoming.insert (CcnxPitEntryIncomingFace (face));

  NS_ASSERT_MSG (ret.second, "Something is wrong");

  return ret.first;
}

void
CcnxPitEntry::RemoveIncoming (Ptr<CcnxFace> face)
{
  m_incoming.erase (face);
}


CcnxPitEntryOutgoingFaceContainer::type::iterator
CcnxPitEntry::AddOutgoing (Ptr<CcnxFace> face)
{
  std::pair<CcnxPitEntryOutgoingFaceContainer::type::iterator,bool> ret =
    m_outgoing.insert (CcnxPitEntryOutgoingFace (face));

  if (!ret.second)
    { // outgoing face already exists
      m_outgoing.modify (ret.first,
                         ll::bind (&CcnxPitEntryOutgoingFace::UpdateOnRetransmit, ll::_1));
    }

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

void
CcnxPitEntry::SetWaitingInVain (CcnxPitEntryOutgoingFaceContainer::type::iterator face)
{
  NS_LOG_DEBUG (boost::cref (*face->m_face));

  m_outgoing.modify (face,
                     (&ll::_1)->*&CcnxPitEntryOutgoingFace::m_waitingInVain = true);
}

bool
CcnxPitEntry::AreAllOutgoingInVain () const
{
  NS_LOG_DEBUG (m_outgoing.size ());

  bool inVain = true;
  std::for_each (m_outgoing.begin (), m_outgoing.end (),
                 ll::var(inVain) &= (&ll::_1)->*&CcnxPitEntryOutgoingFace::m_waitingInVain);

  NS_LOG_DEBUG ("inVain " << inVain);
  return inVain;
}

bool
CcnxPitEntry::AreTherePromisingOutgoingFacesExcept (Ptr<CcnxFace> face) const
{
  bool inVain = true;
  std::for_each (m_outgoing.begin (), m_outgoing.end (),
                 ll::var(inVain) &=
                 ((&ll::_1)->*&CcnxPitEntryOutgoingFace::m_face == face ||
                  (&ll::_1)->*&CcnxPitEntryOutgoingFace::m_waitingInVain));

  return !inVain;
}

void
CcnxPitEntry::IncreaseAllowedRetxCount ()
{
  NS_LOG_ERROR (this);
  if (Simulator::Now () - m_lastRetransmission >= MilliSeconds (100))
    {
      // cheat:
      // don't allow retransmission faster than every 100ms
      m_maxRetxCount++;
      m_lastRetransmission = Simulator::Now ();
    }
}

std::ostream& operator<< (std::ostream& os, const CcnxPitEntry &entry)
{
  os << "Prefix: " << *entry.m_prefix << "\n";
  os << "In: ";
  bool first = true;
  BOOST_FOREACH (const CcnxPitEntryIncomingFace &face, entry.m_incoming)
    {
      if (!first)
        os << ",";
      else
        first = false;
      
      os << *face.m_face;
    }

  os << "\nOut: ";
  first = true;
  BOOST_FOREACH (const CcnxPitEntryOutgoingFace &face, entry.m_outgoing)
    {
      if (!first)
        os << ",";
      else
        first = false;
      
      os << *face.m_face;
    }
  os << "\nNonces: ";
  first = true;
  BOOST_FOREACH (uint32_t nonce, entry.m_seenNonces)
    {
      if (!first)
        os << ",";
      else
        first = false;
      
      os << nonce;
    }

  return os;
}


}
