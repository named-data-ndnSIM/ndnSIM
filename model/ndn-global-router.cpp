/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-global-router.hpp"

#include "model/ndn-l3-protocol.hpp"
#include "model/ndn-face.hpp"

#include "ns3/channel.h"

namespace ns3 {
namespace ndn {

uint32_t GlobalRouter::m_idCounter = 0;

NS_OBJECT_ENSURE_REGISTERED(GlobalRouter);

TypeId
GlobalRouter::GetTypeId()
{
  static TypeId tid = TypeId("ns3::ndn::GlobalRouter").SetGroupName("Ndn").SetParent<Object>();
  return tid;
}

GlobalRouter::GlobalRouter()
{
  m_id = m_idCounter;
  m_idCounter++;
}

void
GlobalRouter::NotifyNewAggregate()
{
  if (m_ndn == 0) {
    m_ndn = GetObject<L3Protocol>();
  }
  Object::NotifyNewAggregate();
}

uint32_t
GlobalRouter::GetId() const
{
  return m_id;
}

Ptr<L3Protocol>
GlobalRouter::GetL3Protocol() const
{
  return m_ndn;
}

void
GlobalRouter::AddLocalPrefix(shared_ptr<Name> prefix)
{
  m_localPrefixes.push_back(prefix);
}

void
GlobalRouter::AddIncidency(shared_ptr<Face> face, Ptr<GlobalRouter> gr)
{
  m_incidencies.push_back(std::make_tuple(this, face, gr));
}

GlobalRouter::IncidencyList&
GlobalRouter::GetIncidencies()
{
  return m_incidencies;
}

const GlobalRouter::LocalPrefixList&
GlobalRouter::GetLocalPrefixes() const
{
  return m_localPrefixes;
}

void
GlobalRouter::clear()
{
  m_idCounter = 0;
}

} // namespace ndn
} // namespace ns3
