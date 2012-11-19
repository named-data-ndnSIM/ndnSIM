/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         
 */

#include "ndn-content-store.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ndn-name-components.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"

NS_LOG_COMPONENT_DEFINE ("ndn.cs.ContentStore");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED (ContentStore);

TypeId 
ContentStore::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::ContentStore")
    .SetGroupName ("Ndn")
    .SetParent<Object> ()

    .AddTraceSource ("CacheHits", "Trace called every time there is a cache hit",
                     MakeTraceSourceAccessor (&ContentStore::m_cacheHitsTrace))

    .AddTraceSource ("CacheMisses", "Trace called every time there is a cache miss",
                     MakeTraceSourceAccessor (&ContentStore::m_cacheMissesTrace))
    ;

  return tid;
}


ContentStore::~ContentStore () 
{
}

namespace cs {

//////////////////////////////////////////////////////////////////////

Entry::Entry (Ptr<const ContentObjectHeader> header, Ptr<const Packet> packet)
  : m_header (header)
  , m_packet (packet->Copy ())
{
}

Ptr<Packet>
Entry::GetFullyFormedNdnPacket () const
{
  static ContentObjectTail tail; ///< \internal for optimization purposes

  Ptr<Packet> packet = m_packet->Copy ();
  packet->AddHeader (*m_header);
  packet->AddTrailer (tail);
  return packet;
}

const NameComponents&
Entry::GetName () const
{
  return m_header->GetName ();
}

Ptr<const ContentObjectHeader>
Entry::GetHeader () const
{
  return m_header;
}

Ptr<const Packet>
Entry::GetPacket () const
{
  return m_packet;
}

} // namespace cs
} // namespace ndn
} // namespace ns3
