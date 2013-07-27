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

#include "ndn-pit-entry.h"

#include "ns3/ndn-pit.h"
#include "ns3/ndn-fib.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-interest.h"

#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/foreach.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ndn.pit.Entry");

namespace ns3 {
namespace ndn {
namespace pit {

Entry::Entry (Pit &container,
              Ptr<const Interest> header,
              Ptr<fib::Entry> fibEntry)
  : m_container (container)
  , m_interest (header)
  , m_fibEntry (fibEntry)
  , m_maxRetxCount (0)
{
  NS_LOG_FUNCTION (this);

  // UpdateLifetime is (and should) be called from the forwarding strategy

  UpdateLifetime ((!header->GetInterestLifetime ().IsZero ()?
                   header->GetInterestLifetime ():
                   Seconds (1.0)));
}

Entry::~Entry ()
{
  NS_LOG_FUNCTION (GetPrefix ());
}

void
Entry::UpdateLifetime (const Time &offsetTime)
{
  NS_LOG_FUNCTION (this);

  Time newExpireTime = Simulator::Now () + (m_container.GetMaxPitEntryLifetime ().IsZero () ?
                                            offsetTime :
                                            std::min (m_container.GetMaxPitEntryLifetime (), offsetTime));
  if (newExpireTime > m_expireTime)
    m_expireTime = newExpireTime;

  NS_LOG_INFO (this->GetPrefix () << ", Updated lifetime to " << m_expireTime.ToDouble (Time::S) << "s, " << (m_expireTime-Simulator::Now ()).ToDouble (Time::S) << "s left");
}

void
Entry::OffsetLifetime (const Time &offsetTime)
{
  m_expireTime += offsetTime;
  if (m_expireTime < Simulator::Now ())
    {
      m_expireTime = Simulator::Now ();
    }
  NS_LOG_INFO (this->GetPrefix () << ", Offsetting lifetime to " << m_expireTime.ToDouble (Time::S) << "s, " << (m_expireTime-Simulator::Now ()).ToDouble (Time::S) << "s left");
}


const Name &
Entry::GetPrefix () const
{
  return m_interest->GetName ();
}

const Time &
Entry::GetExpireTime () const
{
  return m_expireTime;
}

bool
Entry::IsNonceSeen (uint32_t nonce) const
{
  return m_seenNonces.find (nonce) != m_seenNonces.end ();
}

void
Entry::AddSeenNonce (uint32_t nonce)
{
  m_seenNonces.insert (nonce);
}


Entry::in_iterator
Entry::AddIncoming (Ptr<Face> face)
{
  std::pair<in_iterator,bool> ret =
    m_incoming.insert (IncomingFace (face));

  // NS_ASSERT_MSG (ret.second, "Something is wrong");

  return ret.first;
}

void
Entry::RemoveIncoming (Ptr<Face> face)
{
  m_incoming.erase (face);
}

void
Entry::ClearIncoming ()
{
  m_incoming.clear ();
}

Entry::out_iterator
Entry::AddOutgoing (Ptr<Face> face)
{
  std::pair<out_iterator,bool> ret =
    m_outgoing.insert (OutgoingFace (face));

  if (!ret.second)
    { // outgoing face already exists
      const_cast<OutgoingFace&>(*ret.first).UpdateOnRetransmit ();
      // m_outgoing.modify (ret.first,
      //                    ll::bind (&OutgoingFace::UpdateOnRetransmit, ll::_1));
    }

  return ret.first;
}

void
Entry::ClearOutgoing ()
{
  m_outgoing.clear ();
}

void
Entry::RemoveAllReferencesToFace (Ptr<Face> face)
{
  in_iterator incoming = m_incoming.find (face);

  if (incoming != m_incoming.end ())
    m_incoming.erase (incoming);

  out_iterator outgoing =
    m_outgoing.find (face);

  if (outgoing != m_outgoing.end ())
    m_outgoing.erase (outgoing);
}

// void
// Entry::SetWaitingInVain (Entry::out_iterator face)
// {
//   NS_LOG_DEBUG (boost::cref (*face->m_face));

//   m_outgoing.modify (face,
//                      (&ll::_1)->*&EntryOutgoingFace::m_waitingInVain = true);
// }

void
Entry::SetWaitingInVain (Ptr<Face> face)
{
  // NS_LOG_DEBUG (boost::cref (*face->m_face));

  out_iterator item = m_outgoing.find (face);
  if (item == m_outgoing.end ())
    return;

  const_cast<OutgoingFace&>(*item).m_waitingInVain = true;
  // m_outgoing.modify (item,
  //                    (&ll::_1)->*&OutgoingFace::m_waitingInVain = true);
}


bool
Entry::AreAllOutgoingInVain () const
{
  NS_LOG_DEBUG (m_outgoing.size ());

  bool inVain = true;
  std::for_each (m_outgoing.begin (), m_outgoing.end (),
                 ll::var(inVain) &= (&ll::_1)->*&OutgoingFace::m_waitingInVain);

  NS_LOG_DEBUG ("inVain " << inVain);
  return inVain;
}

bool
Entry::AreTherePromisingOutgoingFacesExcept (Ptr<Face> face) const
{
  bool inVain = true;
  std::for_each (m_outgoing.begin (), m_outgoing.end (),
                 ll::var(inVain) &=
                 ((&ll::_1)->*&OutgoingFace::m_face == face ||
                  (&ll::_1)->*&OutgoingFace::m_waitingInVain));

  return !inVain;
}

void
Entry::IncreaseAllowedRetxCount ()
{
  if (Simulator::Now () - m_lastRetransmission >= MilliSeconds (100))
    {
      // cheat:
      // don't allow retransmission faster than every 100ms
      m_maxRetxCount++;
      m_lastRetransmission = Simulator::Now ();
    }
}

Ptr<fib::Entry>
Entry::GetFibEntry ()
{
  return m_fibEntry;
};

const Entry::in_container &
Entry::GetIncoming () const
{
  return m_incoming;
}

const Entry::out_container &
Entry::GetOutgoing () const
{
  return m_outgoing;
}

uint32_t
Entry::GetOutgoingCount () const
{
  return m_outgoing.size ();
}


uint32_t
Entry::GetMaxRetxCount () const
{
  return m_maxRetxCount;
}

Ptr<const Interest>
Entry::GetInterest () const
{
  return m_interest;
}

std::ostream& operator<< (std::ostream& os, const Entry &entry)
{
  os << "Prefix: " << entry.GetPrefix () << "\n";
  os << "In: ";
  bool first = true;
  BOOST_FOREACH (const IncomingFace &face, entry.m_incoming)
    {
      if (!first)
        os << ",";
      else
        first = false;

      os << *face.m_face;
    }

  os << "\nOut: ";
  first = true;
  BOOST_FOREACH (const OutgoingFace &face, entry.m_outgoing)
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


} // namespace pit
} // namespace ndn
} // namespace ns3
