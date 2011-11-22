/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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

#include "ccnx-rit.h"

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/assert.h"

#include <utility>

NS_LOG_COMPONENT_DEFINE ("CcnxRit");

namespace ns3 {

using namespace __ccnx_private_rit;

NS_OBJECT_ENSURE_REGISTERED (CcnxRit);

TypeId 
CcnxRit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxRit")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
    .AddConstructor<CcnxRit> ()
    .AddAttribute ("RitTimeout",
                   "Timeout defining how long records should be kept in RIT",
                   TimeValue (Seconds (1)),
                   MakeTimeAccessor (&CcnxRit::GetRitTimeout, &CcnxRit::SetRitTimeout),
                   MakeTimeChecker ())
    .AddAttribute ("CleanupTimeout",
                   "Timeout defining how frequent RIT should be cleaned up",
                   TimeValue (Seconds (1)),
                   MakeTimeAccessor (&CcnxRit::GetCleanupTimeout, &CcnxRit::SetCleanupTimeout),
                   MakeTimeChecker ())
    ;

  return tid;
}
    
// /**
//  * \ingroup ccnx
//  * \brief Typedef for prefix hash index of RIT container
//  */
// struct CcnxRitByTimestamp
// {
//   typedef
//   CcnxRitContainer::type::index<timestamp>::type
//   type;
// };

//////////////////////////////////////////////////////////////////////


CcnxRit::CcnxRit( )
{
}

CcnxRit::~CcnxRit( )
{
  NS_LOG_FUNCTION_NOARGS ();
  
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events

  clear ();
}

void
CcnxRit::NotifyNewAggregate ()
{
}

void
CcnxRit::DoDispose ()
{
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events                                                                                                                     

  clear ();
}

void
CcnxRit::SetRitTimeout (const Time &timeout)
{
  m_ritTimeout = timeout;
}

Time
CcnxRit::GetRitTimeout () const
{
  return m_ritTimeout;
}

void
CcnxRit::SetCleanupTimeout (const Time &timeout)
{
  m_cleanupTimeout = timeout;
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel (); // cancel any scheduled cleanup events

  // schedule even with new timeout
  m_cleanupEvent = Simulator::Schedule (m_cleanupTimeout,
                                        &CcnxRit::CleanExpired, this); 
}

Time
CcnxRit::GetCleanupTimeout () const
{
  return m_cleanupTimeout;
}

bool
CcnxRit::WasRecentlySatisfied (const CcnxInterestHeader &header)
{
  // NS_LOG_FUNCTION_NOARGS ();
  std::pair<CcnxRitByNonce::type::iterator,CcnxRitByNonce::type::iterator>
    entries = get<nonce> ().equal_range (header.GetNonce ());
  
  if (entries.first == end ())
    return false;

  // check all entries if the name of RIT entry matches the name of interest
  for (CcnxRitByNonce::type::iterator it = entries.first; it != entries.second; it++)
    {
      // NS_LOG_DEBUG (it->m_prefix << " vs " << header.GetName () << " = " << (it->m_prefix == header.GetName ()));
      if (it->m_prefix == header.GetName ())
        return true;
    }

  return false;
}

void
CcnxRit::SetRecentlySatisfied (const CcnxInterestHeader &header)
{
  // NS_LOG_FUNCTION_NOARGS ();
  NS_ASSERT_MSG (!WasRecentlySatisfied (header), "Duplicate recent interest should not be added to RIT");
  
  get<timestamp> ().push_back (
                               CcnxRitEntry(header.GetName (),
                                            header.GetNonce (),
                                            Simulator::Now ()+m_ritTimeout)
                               );
}


void CcnxRit::CleanExpired ()
{
  NS_LOG_LOGIC ("Cleaning RIT, total: " << size ());
  Time now = Simulator::Now ();
  
  while( !empty() )
    {
      if( get<timestamp> ().front ().m_expireTime <= now ) // is the record stale?
        {
         get<timestamp> ().pop_front( );
        }
      else
        break; // nothing else to do. All later records will not be stale
    }
  
  // schedule next even
  m_cleanupEvent = Simulator::Schedule (m_cleanupTimeout,
                                        &CcnxRit::CleanExpired, this); 
}

} //namespace ns3
