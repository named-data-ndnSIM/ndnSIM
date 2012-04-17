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

#include "ccnx-global-router.h"

#include "ns3/ccnx.h"
#include "ns3/ccnx-face.h"
#include "ns3/ccnx-name-components.h"

#include "ns3/channel.h"

using namespace boost;

namespace ns3 {

uint32_t CcnxGlobalRouter::m_idCounter = 0;

NS_OBJECT_ENSURE_REGISTERED (CcnxGlobalRouter);

TypeId 
CcnxGlobalRouter::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxGlobalRouter")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
  ;
  return tid;
}

CcnxGlobalRouter::CcnxGlobalRouter ()
{
  m_id = m_idCounter;
  m_idCounter ++;
}

void
CcnxGlobalRouter::NotifyNewAggregate ()
{
  if (m_ccnx == 0)
    {
      m_ccnx = GetObject<Ccnx> ();
    }
  Object::NotifyNewAggregate ();
}

uint32_t
CcnxGlobalRouter::GetId () const
{
  return m_id;
}

Ptr<Ccnx>
CcnxGlobalRouter::GetCcnx () const
{
  return m_ccnx;
}

void
CcnxGlobalRouter::AddLocalPrefix (Ptr< CcnxNameComponents > prefix)
{
  m_localPrefixes.push_back (prefix);
}

void
CcnxGlobalRouter::AddIncidency (Ptr< CcnxFace > face, Ptr< CcnxGlobalRouter > gr)
{
  m_incidencies.push_back (make_tuple (this, face, gr));
}

CcnxGlobalRouter::IncidencyList &
CcnxGlobalRouter::GetIncidencies ()
{
  return m_incidencies;
}

const CcnxGlobalRouter::LocalPrefixList &
CcnxGlobalRouter::GetLocalPrefixes () const
{
  return m_localPrefixes;
}

// void
// CcnxGlobalRouter::AddIncidencyChannel (Ptr< CcnxFace > face, Ptr< Channel > channel)
// {
//   m_incidenciesChannel.push_back (make_tuple (face, channel));
// }

}

