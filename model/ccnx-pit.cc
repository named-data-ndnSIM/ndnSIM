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

#include "ccnx-pit.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

NS_LOG_COMPONENT_DEFINE ("CcnxPit");

namespace ns3 {

// NS_OBJECT_ENSURE_REGISTERED (CcnxPit);

using namespace __ccnx_private;

// size_t
// PitEntry::numberOfPromisingInterests(e_pi ) const
// {
//   size_t count = 0;

//   BOOST_FOREACH (const CcnxPitOutgoingInterest &interest, m_outgoingInterests)
//     {
//     }
//   for( PitOutgoingConstIterator i = outgoingInterests.begin();
// 	   i!=outgoingInterests.end();
// 	   i++ )
// 	{
// 	  if( !i->waitingInVain ) count++;
// 	}

//   return count;
// }

TypeId 
CcnxPit::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxPit")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
    .AddConstructor<CcnxPit> ()
    .AddAttribute ("CleanupTimeout",
                   "Timeout defining how frequent RIT should be cleaned up",
                   TimeValue (Seconds (1)),
                   MakeTimeAccessor (&CcnxPit::GetCleanupTimeout, &CcnxPit::SetCleanupTimeout),
                   MakeTimeChecker ())
    ;

  return tid;
}

CcnxPit::CcnxPit ()
{
}

void
CcnxPit::SetCleanupTimeout (const Time &timeout)
{
  m_cleanupTimeout = timeout;
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events

  // schedule even with new timeout
  m_cleanupEvent = Simulator::Schedule (Simulator::Now () + m_cleanupTimeout,
                                        &CcnxPit::CleanExpired, this); 
}

Time
CcnxPit::GetCleanupTimeout () const
{
  return m_cleanupTimeout;
}

void CcnxPit::CleanExpired ()
{
  NS_LOG_LOGIC ("Cleaning PIT");
  Time now = Simulator::Now ();
  
  while( !empty() )
    {
      if( get<i_timestamp> ().front ().GetExpireTime () <= now ) // is the record stale?
        {
          get<i_timestamp> ().pop_front( );
        }
      else
        break; // nothing else to do. All later records will not be stale
    }
  
  // schedule next even
  m_cleanupEvent = Simulator::Schedule (Simulator::Now () + m_cleanupTimeout,
                                        &CcnxPit::CleanExpired, this); 
}

const CcnxPitEntry&
CcnxPit::Lookup (const CcnxContentObjectHeader &header) const
{
  CcnxPitEntryContainer::type::iterator entry =
    get<i_prefix> ().find (header.GetName ());

  if (entry != end ())
    throw CcnxPitEntryNotFound();

  return *entry;
}

const CcnxPitEntry&
CcnxPit::Lookup (const CcnxInterestHeader &header)
{
  CcnxPitEntryContainer::type::iterator entry =
    get<i_prefix> ().find (header.GetName ());

  if (entry != end ())
    entry = insert (end (), CcnxPitEntry (Create<CcnxNameComponents> (header.GetName ())));

  return *entry;
}


} // namespace ns3
