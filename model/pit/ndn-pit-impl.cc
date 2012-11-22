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

#include "ndn-pit-impl.h"

#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"
#include "ns3/ndn-forwarding-strategy.h"

#include "../../utils/trie/empty-policy.h"
#include "../../utils/trie/persistent-policy.h"
#include "../../utils/trie/random-policy.h"
#include "../../utils/trie/lru-policy.h"

#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/uinteger.h"
#include "ns3/simulator.h"

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.pit.PitImpl");

using namespace boost::tuples;
using namespace boost;
namespace ll = boost::lambda;

#define NS_OBJECT_ENSURE_REGISTERED_TEMPL(type, templ)  \
  static struct X ## type ## templ ## RegistrationClass \
  {                                                     \
    X ## type ## templ ## RegistrationClass () {        \
      ns3::TypeId tid = type<templ>::GetTypeId ();      \
      tid.GetParent ();                                 \
    }                                                   \
  } x_ ## type ## templ ## RegistrationVariable

namespace ns3 {
namespace ndn {
namespace pit {

using namespace ndnSIM;

template<>
TypeId
PitImpl<persistent_policy_traits>::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::pit::Persistent")
    .SetGroupName ("Ndn")
    .SetParent<Pit> ()
    .AddConstructor< PitImpl< persistent_policy_traits > > ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in PIT. If 0, limit is not enforced",
                   StringValue ("0"),
                   MakeUintegerAccessor (&PitImpl< persistent_policy_traits >::GetMaxSize,
                                         &PitImpl< persistent_policy_traits >::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

template<>
TypeId
PitImpl<random_policy_traits>::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::pit::Random")
    .SetGroupName ("Ndn")
    .SetParent<Pit> ()
    .AddConstructor< PitImpl< random_policy_traits > > ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in PIT. If 0, limit is not enforced",
                   StringValue ("0"),
                   MakeUintegerAccessor (&PitImpl< random_policy_traits >::GetMaxSize,
                                         &PitImpl< random_policy_traits >::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

template<>
TypeId
PitImpl<lru_policy_traits>::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::pit::Lru")
    .SetGroupName ("Ndn")
    .SetParent<Pit> ()
    .AddConstructor< PitImpl< lru_policy_traits > > ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in PIT. If 0, limit is not enforced",
                   StringValue ("0"),
                   MakeUintegerAccessor (&PitImpl< lru_policy_traits >::GetMaxSize,
                                         &PitImpl< lru_policy_traits >::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

template<class Policy>
PitImpl<Policy>::PitImpl ()
{
}

template<class Policy>
PitImpl<Policy>::~PitImpl ()
{
}

template<class Policy>
uint32_t
PitImpl<Policy>::GetMaxSize () const
{
  return super::getPolicy ().get_max_size ();
}

template<class Policy>
void
PitImpl<Policy>::SetMaxSize (uint32_t maxSize)
{
  super::getPolicy ().set_max_size (maxSize);
}

template<class Policy>
void 
PitImpl<Policy>::NotifyNewAggregate ()
{
  if (m_fib == 0)
    {
      m_fib = GetObject<Fib> ();
    }
  if (m_forwardingStrategy == 0)
    {
      m_forwardingStrategy = GetObject<ForwardingStrategy> ();
    }

  Pit::NotifyNewAggregate ();
}

template<class Policy>
void 
PitImpl<Policy>::DoDispose ()
{
  super::clear ();

  m_forwardingStrategy = 0;
  m_fib = 0;

  Pit::DoDispose ();
}

template<class Policy>
void
PitImpl<Policy>::RescheduleCleaning ()
{  
  // m_cleanEvent.Cancel ();
  Simulator::Remove (m_cleanEvent); // slower, but better for memory
  if (i_time.empty ())
    {
      // NS_LOG_DEBUG ("No items in PIT");
      return;
    }

  Time nextEvent = i_time.begin ()->GetExpireTime () - Simulator::Now ();
  if (nextEvent <= 0) nextEvent = Seconds (0);
  
  NS_LOG_DEBUG ("Schedule next cleaning in " <<
                nextEvent.ToDouble (Time::S) << "s (at " <<
                i_time.begin ()->GetExpireTime () << "s abs time");
  
  m_cleanEvent = Simulator::Schedule (nextEvent,
                                      &PitImpl<Policy>::CleanExpired, this);
}

template<class Policy>
void
PitImpl<Policy>::CleanExpired ()
{
  NS_LOG_LOGIC ("Cleaning PIT. Total: " << i_time.size ());
  Time now = Simulator::Now ();

  // uint32_t count = 0;
  while (!i_time.empty ())
    {
      typename time_index::iterator entry = i_time.begin ();
      if (entry->GetExpireTime () <= now) // is the record stale?
        {
          m_forwardingStrategy->WillEraseTimedOutPendingInterest (entry->to_iterator ()->payload ());
          super::erase (entry->to_iterator ());
          // count ++;
        }
      else
        break; // nothing else to do. All later records will not be stale
    }

  if (super::getPolicy ().size ())
    {
      NS_LOG_DEBUG ("Size: " << super::getPolicy ().size ());
      NS_LOG_DEBUG ("i_time size: " << i_time.size ());
    }
  RescheduleCleaning ();
}

template<class Policy>
Ptr<Entry>
PitImpl<Policy>::Lookup (const ContentObjectHeader &header)
{
  /// @todo use predicate to search with exclude filters  
  typename super::iterator item = super::longest_prefix_match (header.GetName ());

  if (item == super::end ())
    return 0;
  else
    return item->payload (); // which could also be 0
}

template<class Policy>
Ptr<Entry>
PitImpl<Policy>::Lookup (const InterestHeader &header)
{
  // NS_LOG_FUNCTION (header.GetName ());
  NS_ASSERT_MSG (m_fib != 0, "FIB should be set");
  NS_ASSERT_MSG (m_forwardingStrategy != 0, "Forwarding strategy  should be set");

  typename super::iterator foundItem, lastItem;
  bool reachLast;
  boost::tie (foundItem, reachLast, lastItem) = super::getTrie ().find (header.GetName ());

  if (!reachLast || lastItem == super::end ())
    return 0;
  else
    return lastItem->payload (); // which could also be 0
}

template<class Policy>
Ptr<Entry>
PitImpl<Policy>::Create (Ptr<const InterestHeader> header)
{
  NS_LOG_DEBUG (header->GetName ());
  Ptr<fib::Entry> fibEntry = m_fib->LongestPrefixMatch (*header);
  if (fibEntry == 0)
    return 0;
  
  // NS_ASSERT_MSG (fibEntry != 0,
  //                "There should be at least default route set" <<
  //                " Prefix = "<< header->GetName() << ", NodeID == " << m_fib->GetObject<Node>()->GetId() << "\n" << *m_fib);

  Ptr< entry > newEntry = ns3::Create< entry > (boost::ref (*this), header, fibEntry);
  std::pair< typename super::iterator, bool > result = super::insert (header->GetName (), newEntry);
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


template<class Policy>
void
PitImpl<Policy>::MarkErased (Ptr<Entry> item)
{
  // entry->SetExpireTime (Simulator::Now () + m_PitEntryPruningTimout);
  super::erase (StaticCast< entry > (item)->to_iterator ());
}


template<class Policy>
void
PitImpl<Policy>::Print (std::ostream& os) const
{
  // !!! unordered_set imposes "random" order of item in the same level !!!
  typename super::parent_trie::const_recursive_iterator item (super::getTrie ()), end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;

      os << item->payload ()->GetPrefix () << "\t" << *item->payload () << "\n";
    }
}

template<class Policy>
uint32_t
PitImpl<Policy>::GetSize () const
{
  return super::getPolicy ().size ();
}

template<class Policy>
Ptr<Entry>
PitImpl<Policy>::Begin ()
{
  typename super::parent_trie::recursive_iterator item (super::getTrie ()), end (0);
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

template<class Policy>
Ptr<Entry>
PitImpl<Policy>::End ()
{
  return 0;
}

template<class Policy>
Ptr<Entry>
PitImpl<Policy>::Next (Ptr<Entry> from)
{
  if (from == 0) return 0;
  
  typename super::parent_trie::recursive_iterator
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


// explicit instantiation and registering
template class PitImpl<persistent_policy_traits>;
template class PitImpl<random_policy_traits>;
template class PitImpl<lru_policy_traits>;

NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, persistent_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, random_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(PitImpl, lru_policy_traits);

#ifdef DOXYGEN
// /**
//  * \brief PIT in which new entries will be rejected if PIT size reached its limit
//  */
class Persistent : public PitImpl<persistent_policy_traits> { };

/**
 * \brief PIT in which PIT reaches its limit, random entry (could be the newly created one) will be removed from PIT
 */
class Random : public PitImpl<random_policy_traits> { };

/**
 * \brief PIT in which  the least recently used entry (the oldest entry with minimum number of incoming faces)
 * will be removed when PIT size reached its limit
 */
class Lru : public PitImpl<lru_policy_traits> { };
#endif

} // namespace pit
} // namespace ndn
} // namespace ns3
