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

#include "ndn-content-store.hpp"

#include "ns3/log.h"
#include "ns3/packet.h"

NS_LOG_COMPONENT_DEFINE("ndn.cs.ContentStore");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(ContentStore);

TypeId
ContentStore::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::ContentStore")
      .SetGroupName("Ndn")
      .SetParent<Object>()

      .AddTraceSource("CacheHits", "Trace called every time there is a cache hit",
                      MakeTraceSourceAccessor(&ContentStore::m_cacheHitsTrace),
                      "ns3::ndn::ContentStore::CacheHitsCallback")

      .AddTraceSource("CacheMisses", "Trace called every time there is a cache miss",
                      MakeTraceSourceAccessor(&ContentStore::m_cacheMissesTrace),
                      "ns3::ndn::ContentStrore::CacheMissesCallback");

  return tid;
}

ContentStore::~ContentStore()
{
}

namespace cs {

//////////////////////////////////////////////////////////////////////

Entry::Entry(Ptr<ContentStore> cs, shared_ptr<const Data> data)
  : m_cs(cs)
  , m_data(data)
{
}

const Name&
Entry::GetName() const
{
  return m_data->getName();
}

shared_ptr<const Data>
Entry::GetData() const
{
  return m_data;
}

Ptr<ContentStore>
Entry::GetContentStore()
{
  return m_cs;
}

} // namespace cs
} // namespace ndn
} // namespace ns3
