/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ndn-global-router.h"

#include "ns3/ndn.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-name-components.h"

#include "ns3/channel.h"

using namespace boost;

namespace ns3 {

uint32_t NdnGlobalRouter::m_idCounter = 0;

NS_OBJECT_ENSURE_REGISTERED (NdnGlobalRouter);

TypeId 
NdnGlobalRouter::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::NdnGlobalRouter")
    .SetGroupName ("Ndn")
    .SetParent<Object> ()
  ;
  return tid;
}

NdnGlobalRouter::NdnGlobalRouter ()
{
  m_id = m_idCounter;
  m_idCounter ++;
}

void
NdnGlobalRouter::NotifyNewAggregate ()
{
  if (m_ndn == 0)
    {
      m_ndn = GetObject<Ndn> ();
    }
  Object::NotifyNewAggregate ();
}

uint32_t
NdnGlobalRouter::GetId () const
{
  return m_id;
}

Ptr<Ndn>
NdnGlobalRouter::GetNdn () const
{
  return m_ndn;
}

void
NdnGlobalRouter::AddLocalPrefix (Ptr< NdnNameComponents > prefix)
{
  m_localPrefixes.push_back (prefix);
}

void
NdnGlobalRouter::AddIncidency (Ptr< NdnFace > face, Ptr< NdnGlobalRouter > gr)
{
  m_incidencies.push_back (make_tuple (this, face, gr));
}

NdnGlobalRouter::IncidencyList &
NdnGlobalRouter::GetIncidencies ()
{
  return m_incidencies;
}

const NdnGlobalRouter::LocalPrefixList &
NdnGlobalRouter::GetLocalPrefixes () const
{
  return m_localPrefixes;
}

// void
// NdnGlobalRouter::AddIncidencyChannel (Ptr< NdnFace > face, Ptr< Channel > channel)
// {
//   m_incidenciesChannel.push_back (make_tuple (face, channel));
// }

}

