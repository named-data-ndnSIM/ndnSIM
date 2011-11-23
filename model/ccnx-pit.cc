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
#include "ns3/string.h"
#include "ns3/simulator.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxPit");

using namespace boost::tuples;
using namespace boost;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxPit);

using namespace __ccnx_private;

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
    .AddAttribute ("PitEntryPruningTimout",
                   "Timeout for PIT entry to live after being satisfied. To make sure recently satisfied interest will not be satisfied again",
                   StringValue ("100ms"),
                   MakeTimeAccessor (&CcnxPit::m_PitEntryPruningTimout),
                   MakeTimeChecker ())
    .AddAttribute ("PitEntryDefaultLifetime",
                   "Default lifetime of PIT entry (aka default Interest lifetime)",
                   StringValue("4s"),
                   MakeTimeAccessor (&CcnxPit::m_PitEntryDefaultLifetime),
                   MakeTimeChecker ())
    ;

  return tid;
}

CcnxPit::CcnxPit ()
{
}

CcnxPit::~CcnxPit ()
{
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events

  clear ();
}

void 
CcnxPit::NotifyNewAggregate ()
{
}

void 
CcnxPit::DoDispose ()
{
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events

  clear ();
}

void
CcnxPit::SetCleanupTimeout (const Time &timeout)
{
  m_cleanupTimeout = timeout;
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events

  // schedule even with new timeout
  m_cleanupEvent = Simulator::Schedule (m_cleanupTimeout,
                                        &CcnxPit::CleanExpired, this); 
}

Time
CcnxPit::GetCleanupTimeout () const
{
  return m_cleanupTimeout;
}

void CcnxPit::CleanExpired ()
{
  NS_LOG_LOGIC ("Cleaning PIT. Total: " << size ());
  Time now = Simulator::Now ();

  uint32_t count = 0;
  while( !empty() )
    {
      if( get<i_timestamp> ().front ().GetExpireTime () <= now ) // is the record stale?
        {
          get<i_timestamp> ().pop_front( );
          count ++;
        }
      else
        break; // nothing else to do. All later records will not be stale
    }

  // NS_LOG_LOGIC ("Cleaned " << count << " records. Total: " << size ());
  // schedule next even
  
  m_cleanupEvent = Simulator::Schedule (m_cleanupTimeout,
                                        &CcnxPit::CleanExpired, this); 
}

void
CcnxPit::SetFib (Ptr<CcnxFib> fib)
{
  m_fib = fib;
}

bool
CcnxPit::TryAddOutgoing (const CcnxPitEntry &pitEntry, Ptr<CcnxFace> face)
{
  NS_LOG_FUNCTION ("Face has " << m_bucketsPerFace[face->GetId()] <<
               " packets with max allowance " << maxBucketsPerFace[face->GetId()]); 
    
  if (m_bucketsPerFace[face->GetId()]+1.0 >= maxBucketsPerFace[face->GetId()])
    {
      NS_LOG_INFO("************ LIMIT **************");
      return false;
    }
    
  m_bucketsPerFace[face->GetId()] = m_bucketsPerFace[face->GetId()] + 1.0;

  modify (iterator_to (pitEntry),
          bind(&CcnxPitEntry::AddOutgoing, lambda::_1, face));
          
  return true;
}

const CcnxPitEntry&
CcnxPit::Lookup (const CcnxContentObjectHeader &header) const
{
  // NS_LOG_FUNCTION_NOARGS ();

  CcnxPitEntryContainer::type::iterator entry =
    get<i_prefix> ().find (header.GetName ());

  if (entry == end ())
    throw CcnxPitEntryNotFound();

  return *entry;
}

boost::tuple<const CcnxPitEntry&, bool, bool>
CcnxPit::Lookup (const CcnxInterestHeader &header)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG (m_fib != 0, "FIB should be set");

  bool isDuplicate = false;
  bool isNew = true;

  CcnxPitEntryContainer::type::iterator entry =
    get<i_prefix> ().find (header.GetName ());

  if (entry == end ())
    {
      CcnxFibEntryContainer::type::iterator fibEntry = m_fib->LongestPrefixMatch (header);
      NS_ASSERT_MSG (fibEntry != m_fib->end (),
                     "There should be at least default route set");

      entry = insert (end (),
                      CcnxPitEntry (Create<CcnxNameComponents> (header.GetName ()),
                                    Simulator::Now () +
                                    (header.GetInterestLifetime ().IsZero ()?m_PitEntryDefaultLifetime
                                     :                                       header.GetInterestLifetime ()),
                                    *fibEntry));

      // isDuplicate = false; // redundant
      // isNew = true; // also redundant
    }
  else
    {
      isNew = false;
      isDuplicate = entry->IsNonceSeen (header.GetNonce ());
    }

  if (!isDuplicate)
    {
      modify (entry,
              boost::bind(&CcnxPitEntry::AddSeenNonce, boost::lambda::_1, header.GetNonce ()));
    }

  return make_tuple (cref(*entry), isNew, isDuplicate);
}

///////////////////////////////////////////////////////////////////////////////////////////

void 
CcnxPit::LeakBuckets ()
{
  for (PitBucketIterator it = m_bucketsPerFace.begin(); 
       it != m_bucketsPerFace.end();
       it++)
    {
      it->second = std::max (0.0, it->second - leakSize[it->first]);
    }
}
    
void 
CcnxPit::LeakBucket (Ptr<CcnxFace> face, int amount)
{
  m_bucketsPerFace[face->GetId()] = std::max (0.0, m_bucketsPerFace[face->GetId()] - amount);
}


} // namespace ns3
