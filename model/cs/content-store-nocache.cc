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

#include "content-store-nocache.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"

NS_LOG_COMPONENT_DEFINE ("ndn.cs.Nocache");

namespace ns3 {
namespace ndn {
namespace cs {

NS_OBJECT_ENSURE_REGISTERED (Nocache);

TypeId 
Nocache::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::cs::Nocache")
    .SetGroupName ("Ndn")
    .SetParent<ContentStore> ()
    .AddConstructor< Nocache > ()
    ;

  return tid;
}

Nocache::Nocache ()
{
}

Nocache::~Nocache () 
{
}

Ptr<Data>
Nocache::Lookup (Ptr<const Interest> interest)
{
  this->m_cacheMissesTrace (interest);
  return 0;
}

bool
Nocache::Add (Ptr<const Data> data)
{
  return false;
}

void
Nocache::Print (std::ostream &os) const
{
}

uint32_t
Nocache::GetSize () const
{
  return 0;
}

Ptr<cs::Entry>
Nocache::Begin ()
{
  return 0;
}

Ptr<cs::Entry>
Nocache::End ()
{
  return 0;
}

Ptr<cs::Entry>
Nocache::Next (Ptr<cs::Entry>)
{
  return 0;
}

} // namespace cs
} // namespace ndn
} // namespace ns3
