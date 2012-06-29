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
#include "ns3/uinteger.h"
#include "ns3/simulator.h"
#include "ccnx-interest-header.h"
#include "ccnx-content-object-header.h"

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxPit");

using namespace boost::tuples;
using namespace boost;
namespace ll = boost::lambda;

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
                   StringValue ("1s"),
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

    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in PIT. If 0, limit is not enforced",
                   StringValue ("0"),
                   MakeUintegerAccessor (&CcnxPit::m_maxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

CcnxPit::CcnxPit ()
{
}

CcnxPit::~CcnxPit ()
{
}

void 
CcnxPit::NotifyNewAggregate ()
{
  if (m_fib == 0)
    {
      m_fib = GetObject<CcnxFib> ();
    }
}

void 
CcnxPit::DoDispose ()
{
  if (m_cleanupEvent.IsRunning ())
    m_cleanupEvent.Cancel ();

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
  // NS_LOG_LOGIC ("Cleaning PIT. Total: " << size ());
  Time now = Simulator::Now ();

  // uint32_t count = 0;
  while (!empty ())
    {
      CcnxPit::index<i_timestamp>::type::iterator entry = get<i_timestamp> ().begin ();
      if (entry->GetExpireTime () <= now) // is the record stale?
        {
          get<i_timestamp> ().erase (entry);
          // count ++;
        }
      else
        break; // nothing else to do. All later records will not be stale
    }

  // schedule next event  
  m_cleanupEvent = Simulator::Schedule (m_cleanupTimeout,
                                        &CcnxPit::CleanExpired, this); 
}

CcnxPit::iterator
CcnxPit::Lookup (const CcnxContentObjectHeader &header) const
{
  iterator entry = end ();

  // do the longest prefix match
  const CcnxNameComponents &name = header.GetName ();
  for (size_t componentsCount = name.GetComponents ().size ()+1;
       componentsCount > 0;
       componentsCount--)
    {
      CcnxNameComponents subPrefix (name.GetSubComponents (componentsCount-1));

      entry = get<i_prefix> ().find (subPrefix);
      if (entry != end())
        return entry;
    }

  return end ();
}

CcnxPit::iterator
CcnxPit::Lookup (const CcnxInterestHeader &header)
{
  NS_LOG_FUNCTION (header.GetName ());
  NS_ASSERT_MSG (m_fib != 0, "FIB should be set");

  iterator entry = get<i_prefix> ().find (header.GetName ());
  if (entry == end ())
    return end ();
   
  return entry;
}

bool
CcnxPit::CheckIfDuplicate (CcnxPit::iterator entry, const CcnxInterestHeader &header)
{
  if (!entry->IsNonceSeen (header.GetNonce ()))
    {
      modify (entry,
              boost::bind(&CcnxPitEntry::AddSeenNonce, ll::_1, header.GetNonce ()));
      return false;
    }
  else
    return true;
}

CcnxPit::iterator
CcnxPit::Create (const CcnxInterestHeader &header)
{
  NS_ASSERT_MSG (get<i_prefix> ().find (header.GetName ()) == end (),
                 "Entry already exists, Create must not be called!!!");
  
  if (m_maxSize > 0 &&
      size () >= m_maxSize)
    {
      // remove old record
      get<i_timestamp> ().erase (get<i_timestamp> ().begin ());
    }
      
  CcnxFib::iterator fibEntry = m_fib->LongestPrefixMatch (header);
  // NS_ASSERT_MSG (fibEntry != m_fib->m_fib.end (),
  //                "There should be at least default route set" << " Prefix = "<<header.GetName() << "NodeID == " << m_fib->GetObject<Node>()->GetId() << "\n" << *m_fib);

  return insert (end (),
                 CcnxPitEntry (ns3::Create<CcnxNameComponents> (header.GetName ()),
                               header.GetInterestLifetime ().IsZero ()?m_PitEntryDefaultLifetime
                               :                                       header.GetInterestLifetime (),
                               fibEntry));
}


void
CcnxPit::MarkErased (CcnxPit::iterator entry)
{
  modify (entry,
          ll::bind (&CcnxPitEntry::SetExpireTime, ll::_1,
                    Simulator::Now () + m_PitEntryPruningTimout));
}


std::ostream& operator<< (std::ostream& os, const CcnxPit &pit)
{
  BOOST_FOREACH (const CcnxPitEntry &entry, pit)
    {
      if (entry.m_incoming.size () == 0 && entry.m_outgoing.size () == 0)
        continue; // these are stale to-be-removed records, so there is no need to print them out
      
      os << entry << std::endl;
    }

  return os;
}

} // namespace ns3
