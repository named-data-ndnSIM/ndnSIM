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

#include "ccnx-pit-impl.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/ccnx-forwarding-strategy.h"

#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

NS_LOG_COMPONENT_DEFINE ("CcnxPitImpl");

using namespace boost::tuples;
using namespace boost;
namespace ll = boost::lambda;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxPitImpl);


// CcnxPitEntryImpl::CcnxPitEntryImpl (CcnxPit &pit,
//                                     Ptr<const CcnxInterestHeader> header,
//                                     Ptr<CcnxFibEntry> fibEntry)
//   : CcnxPitEntry (pit, header, fibEntry)
//   , item_ (0)
// {
//   static_cast<CcnxPitImpl&> (m_container).i_time.insert (*this);
// }

// CcnxPitEntryImpl::~CcnxPitEntryImpl ()
// {
//   static_cast<CcnxPitImpl&> (m_container).i_time.erase (*this);
// }

TypeId
CcnxPitImpl::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::CcnxPit")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxPit> ()
    .AddConstructor<CcnxPitImpl> ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in PIT. If 0, limit is not enforced",
                   StringValue ("0"),
                   MakeUintegerAccessor (&CcnxPitImpl::GetMaxSize, &CcnxPitImpl::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

CcnxPitImpl::CcnxPitImpl ()
{
}

CcnxPitImpl::~CcnxPitImpl ()
{
}

uint32_t
CcnxPitImpl::GetMaxSize () const
{
  return getPolicy ().get_max_size ();
}

void
CcnxPitImpl::SetMaxSize (uint32_t maxSize)
{
  getPolicy ().set_max_size (maxSize);
}

void 
CcnxPitImpl::NotifyNewAggregate ()
{
  if (m_fib == 0)
    {
      m_fib = GetObject<CcnxFib> ();
    }
  if (m_forwardingStrategy == 0)
    {
      m_forwardingStrategy = GetObject<CcnxForwardingStrategy> ();
    }

  CcnxPit::NotifyNewAggregate ();
}

void 
CcnxPitImpl::DoDispose ()
{
  super::clear ();

  m_forwardingStrategy = 0;
  m_fib = 0;

  CcnxPit::DoDispose ();
}

void CcnxPitImpl::RescheduleCleaning ()
{
  m_cleanEvent.Cancel ();
  if (i_time.empty ())
    {
      NS_LOG_DEBUG ("No items in PIT");
      return;
    }

  Time nextEvent = i_time.begin ()->GetExpireTime () - Simulator::Now ();
  if (nextEvent < 0) nextEvent = Seconds (0);
  
  NS_LOG_DEBUG ("Schedule next cleaning in " <<
                nextEvent.ToDouble (Time::S) << "s (at " <<
                i_time.begin ()->GetExpireTime () << "s abs time");
  m_cleanEvent = Simulator::Schedule (nextEvent,
                                      &CcnxPitImpl::CleanExpired, this);
}

void
CcnxPitImpl::CleanExpired ()
{
  NS_LOG_LOGIC ("Cleaning PIT. Total: " << i_time.size ());
  Time now = Simulator::Now ();

  // uint32_t count = 0;
  while (!i_time.empty ())
    {
      time_index::iterator entry = i_time.begin ();
      if (entry->GetExpireTime () <= now) // is the record stale?
        {
          m_forwardingStrategy->WillErasePendingInterest (entry->to_iterator ()->payload ());
          super::erase (entry->to_iterator ());
      //     // count ++;
        }
      else
        break; // nothing else to do. All later records will not be stale
    }

  RescheduleCleaning ();
}

Ptr<CcnxPitEntry>
CcnxPitImpl::Lookup (const CcnxContentObjectHeader &header)
{
  /// @todo use predicate to search with exclude filters  
  super::iterator item = super::longest_prefix_match (header.GetName ());

  if (item == super::end ())
    return 0;
  else
    return item->payload (); // which could also be 0
}

Ptr<CcnxPitEntry>
CcnxPitImpl::Lookup (const CcnxInterestHeader &header)
{
  NS_LOG_FUNCTION (header.GetName ());
  NS_ASSERT_MSG (m_fib != 0, "FIB should be set");
  NS_ASSERT_MSG (m_forwardingStrategy != 0, "Forwarding strategy  should be set");

  super::iterator foundItem, lastItem;
  bool reachLast;
  boost::tie (foundItem, reachLast, lastItem) = super::getTrie ().find (header.GetName ());

  if (!reachLast || lastItem == super::end ())
    return 0;
  else
    return lastItem->payload (); // which could also be 0
}

Ptr<CcnxPitEntry>
CcnxPitImpl::Create (Ptr<const CcnxInterestHeader> header)
{
  Ptr<CcnxFibEntry> fibEntry = m_fib->LongestPrefixMatch (*header);
  if (fibEntry == 0)
    return 0;
  
  // NS_ASSERT_MSG (fibEntry != 0,
  //                "There should be at least default route set" <<
  //                " Prefix = "<< header->GetName() << ", NodeID == " << m_fib->GetObject<Node>()->GetId() << "\n" << *m_fib);

  Ptr< entry > newEntry = ns3::Create< entry > (boost::ref (*this), header, fibEntry);
  std::pair< super::iterator, bool > result = super::insert (header->GetName (), newEntry);
  if (result.first != super::end ())
    {
      if (result.second)
        {
          newEntry->SetTrie (result.first);
          return newEntry;
        }
      else
        {
          // should we do anything?
          // update payload? add new payload?
          return result.first->payload ();
        }
    }
  else
    return 0;
}


void
CcnxPitImpl::MarkErased (Ptr<CcnxPitEntry> item)
{
  // entry->SetExpireTime (Simulator::Now () + m_PitEntryPruningTimout);
  super::erase (StaticCast< entry > (item)->to_iterator ());
}


void
CcnxPitImpl::Print (std::ostream& os) const
{
  // !!! unordered_set imposes "random" order of item in the same level !!!
  super::parent_trie::const_recursive_iterator item (super::getTrie ()), end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;

      os << item->payload ()->GetPrefix () << "\t" << *item->payload () << "\n";
    }
}

uint32_t
CcnxPitImpl::GetSize () const
{
  return super::getPolicy ().size ();
}

Ptr<CcnxPitEntry>
CcnxPitImpl::Begin ()
{
  super::parent_trie::recursive_iterator item (super::getTrie ()), end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;
      break;
    }

  if (item == end)
    return End ();
  else
    return item->payload ();
}

Ptr<CcnxPitEntry>
CcnxPitImpl::End ()
{
  return 0;
}

Ptr<CcnxPitEntry>
CcnxPitImpl::Next (Ptr<CcnxPitEntry> from)
{
  if (from == 0) return 0;
  
  super::parent_trie::recursive_iterator
    item (*StaticCast< entry > (from)->to_iterator ()),
    end (0);
  
  for (item++; item != end; item++)
    {
      if (item->payload () == 0) continue;
      break;
    }

  if (item == end)
    return End ();
  else
    return item->payload ();
}


} // namespace ns3
