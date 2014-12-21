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

#include "content-store-nocache.hpp"

#include "ns3/log.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE("ndn.cs.Nocache");

namespace ns3 {
namespace ndn {
namespace cs {

NS_OBJECT_ENSURE_REGISTERED(Nocache);

TypeId
Nocache::GetTypeId(void)
{
  static TypeId tid = TypeId("ns3::ndn::cs::Nocache")
                        .SetGroupName("Ndn")
                        .SetParent<ContentStore>()
                        .AddConstructor<Nocache>();

  return tid;
}

Nocache::Nocache()
{
}

Nocache::~Nocache()
{
}

shared_ptr<Data>
Nocache::Lookup(shared_ptr<const Interest> interest)
{
  this->m_cacheMissesTrace(interest);
  return 0;
}

bool
Nocache::Add(shared_ptr<const Data> data)
{
  return false;
}

void
Nocache::Print(std::ostream& os) const
{
}

uint32_t
Nocache::GetSize() const
{
  return 0;
}

Ptr<cs::Entry>
Nocache::Begin()
{
  return 0;
}

Ptr<cs::Entry>
Nocache::End()
{
  return 0;
}

Ptr<cs::Entry> Nocache::Next(Ptr<cs::Entry>)
{
  return 0;
}

} // namespace cs
} // namespace ndn
} // namespace ns3
