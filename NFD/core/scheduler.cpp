/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014,  Regents of the University of California,
 *                      Arizona Board of Regents,
 *                      Colorado State University,
 *                      University Pierre & Marie Curie, Sorbonne University,
 *                      Washington University in St. Louis,
 *                      Beijing Institute of Technology,
 *                      The University of Memphis
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scheduler.hpp"

namespace ns3 {

/// @cond include_hidden

template<>
struct EventMemberImplObjTraits<std::function<void()>> {
  typedef std::function<void()> T;
  static T&
  GetReference(T& p)
  {
    return p;
  }
};

/// @endcond

} // namespace ns3

namespace nfd {
namespace scheduler {

EventId
schedule(const time::nanoseconds& after, const std::function<void()>& event)
{
  ns3::EventId id = ns3::Simulator::Schedule(ns3::NanoSeconds(after.count()),
                                             &std::function<void()>::operator(), event);
  return std::make_shared<ns3::EventId>(id);
}

void
cancel(const EventId& eventId)
{
  if (eventId != nullptr) {
    ns3::Simulator::Remove(*eventId);
    const_cast<EventId&>(eventId).reset();
  }
}

ScopedEventId::ScopedEventId()
{
}

ScopedEventId::ScopedEventId(const EventId& event)
  : m_event(event)
{
}

ScopedEventId::ScopedEventId(ScopedEventId&& other)
  : m_event(other.m_event)
{
  other.release();
}

ScopedEventId&
ScopedEventId::operator=(const EventId& event)
{
  if (m_event != event) {
    scheduler::cancel(m_event);
    m_event = event;
  }
  return *this;
}

ScopedEventId::~ScopedEventId()
{
  scheduler::cancel(m_event);
}

void
ScopedEventId::cancel()
{
  scheduler::cancel(m_event);
}

void
ScopedEventId::release()
{
  m_event.reset();
}

} // namespace scheduler
} // namespace nfd
