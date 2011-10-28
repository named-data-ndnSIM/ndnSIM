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

void
CcnxPit::SetFib (Ptr<CcnxFib> fib)
{
  m_fib = fib;
}

/*CcnxPitEntryContainer::type::iterator
CcnxPit::Add (const CcnxInterestHeader &header, CcnxFibEntryContainer::type::iterator fibEntry, Ptr<CcnxFace> face)
{
    if( m_bucketsPerFace[face->GetId()]+1.0 >= maxBucketsPerFace[face->GetId()] )
	{
        //		printf( "DEBUG: bucket overflow. Should not forward anything to interface %d\n", interest.interfaceIndex );
		return end();
	}
    
    CcnxPitEntryContainer::type::iterator entry = insert (end (),
                    CcnxPitEntry (Create<CcnxNameComponents> (header.GetName ()),
                                *fibEntry));
    return entry;
}*/

    
    
bool
CcnxPit::TryAddOutgoing(CcnxPitEntryContainer::type::iterator pitEntry, Ptr<CcnxFace> face)
{
    NS_LOG_INFO ("Face has " << m_bucketsPerFace[face->GetId()] << " packets with max allowance " << maxBucketsPerFace[face->GetId()]); 
    
    if((face->IsLocal() == false) 
       && (m_bucketsPerFace[face->GetId()]+1.0 >= maxBucketsPerFace[face->GetId()] ))
	{
		return false;
	}
    
    m_bucketsPerFace[face->GetId()] = m_bucketsPerFace[face->GetId()] + 1.0;
	
    NS_LOG_INFO(this->size());
    NS_LOG_INFO("before modify");
    NS_LOG_INFO(pitEntry->GetPrefix());
    modify (pitEntry, CcnxPitEntry::AddOutgoing(face));
    NS_LOG_INFO("after modify");
    return true;
}

const CcnxPitEntry&
CcnxPit::Lookup (const CcnxContentObjectHeader &header) const
{
  NS_LOG_FUNCTION_NOARGS ();

  CcnxPitEntryContainer::type::iterator entry =
    get<i_prefix> ().find (header.GetName ());

  if (entry == end ())
    throw CcnxPitEntryNotFound();

  return *entry;
}

CcnxPitEntryContainer::type::iterator
CcnxPit::Lookup (const CcnxInterestHeader &header, CcnxFibEntryContainer::type::iterator &outFibEntry)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG (m_fib != 0, "FIB should be set");

  CcnxPitEntryContainer::type::iterator entry =
    get<i_prefix> ().find (header.GetName ());

  CcnxFibEntryContainer::type::iterator fibEntry = m_fib->LongestPrefixMatch (header);
  if (fibEntry == m_fib->end ())
    {
      NS_LOG_WARN ("FIB entry wasn't found. Creating an empty record");
      fibEntry = m_fib->insert (m_fib->end (), CcnxFibEntry (header.GetName ()));
    }
  
  if (entry == end ())
  {
      NS_LOG_INFO("entry == end");
      NS_LOG_INFO(this->size());
        entry = insert (end (),
                    CcnxPitEntry (Create<CcnxNameComponents> (header.GetName ()),
                                  *fibEntry));
      NS_LOG_INFO(this->size());
  }
  outFibEntry = fibEntry;
  return entry;
}

void 
CcnxPit::LeakBuckets( )
{
    for( PitBucketIterator it=m_bucketsPerFace.begin(); 
        it != m_bucketsPerFace.end();
        it++ )
    {
        it->second = std::max( 0.0, it->second - leakSize[it->first] );
    }
}
    
void 
CcnxPit::LeakBucket(Ptr<CcnxFace> face, int amount )
{
    m_bucketsPerFace[face->GetId()] = std::max( 0.0, m_bucketsPerFace[face->GetId()] - amount );
}


} // namespace ns3
