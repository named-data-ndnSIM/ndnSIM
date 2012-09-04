/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "content-store-impl.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

#include "../../utils/random-policy.h"
#include "../../utils/lru-policy.h"
#include "../../utils/fifo-policy.h"

NS_LOG_COMPONENT_DEFINE ("ndn.cs.ContentStoreImpl");

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

using namespace ndnSIM;

namespace cs {


template<>
TypeId
ContentStoreImpl< lru_policy_traits >::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::cs::Lru")
    .SetGroupName ("Ndn")
    .SetParent<ContentStore> ()
    .AddConstructor< ContentStoreImpl< lru_policy_traits > > ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in ContentStore. If 0, limit is not enforced",
                   StringValue ("100"),
                   MakeUintegerAccessor (&ContentStoreImpl< lru_policy_traits >::GetMaxSize,
                                         &ContentStoreImpl< lru_policy_traits >::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

template<>
TypeId
ContentStoreImpl< random_policy_traits >::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::cs::Random")
    .SetGroupName ("Ndn")
    .SetParent<ContentStore> ()
    .AddConstructor< ContentStoreImpl< random_policy_traits > > ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in ContentStore. If 0, limit is not enforced",
                   StringValue ("100"),
                   MakeUintegerAccessor (&ContentStoreImpl< random_policy_traits >::GetMaxSize,
                                         &ContentStoreImpl< random_policy_traits >::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

template<>
TypeId
ContentStoreImpl< fifo_policy_traits >::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ndn::cs::Fifo")
    .SetGroupName ("Ndn")
    .SetParent<ContentStore> ()
    .AddConstructor< ContentStoreImpl< fifo_policy_traits > > ()
    .AddAttribute ("MaxSize",
                   "Set maximum number of entries in ContentStore. If 0, limit is not enforced",
                   StringValue ("100"),
                   MakeUintegerAccessor (&ContentStoreImpl< fifo_policy_traits >::GetMaxSize,
                                         &ContentStoreImpl< fifo_policy_traits >::SetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

template<class Policy>
boost::tuple<Ptr<Packet>, Ptr<const ContentObjectHeader>, Ptr<const Packet> >
ContentStoreImpl<Policy>::Lookup (Ptr<const InterestHeader> interest)
{
  // NS_LOG_FUNCTION (this << interest->GetName ());

  /// @todo Change to search with predicate
  typename super::const_iterator node = this->deepest_prefix_match (interest->GetName ());
  
  if (node != this->end ())
    {
      this->m_cacheHitsTrace (interest, node->payload ()->GetHeader ());

      // NS_LOG_DEBUG ("cache hit with " << node->payload ()->GetHeader ()->GetName ());
      return boost::make_tuple (node->payload ()->GetFullyFormedNdnPacket (),
                                node->payload ()->GetHeader (),
                                node->payload ()->GetPacket ());
    }
  else
    {
      // NS_LOG_DEBUG ("cache miss for " << interest->GetName ());
      this->m_cacheMissesTrace (interest);
      return boost::tuple<Ptr<Packet>, Ptr<ContentObjectHeader>, Ptr<Packet> > (0, 0, 0);
    }
}   
    
template<class Policy>
bool 
ContentStoreImpl<Policy>::Add (Ptr<const ContentObjectHeader> header, Ptr<const Packet> packet)
{
  // NS_LOG_FUNCTION (this << header->GetName ());

  Ptr< entry > newEntry = Create< entry > (header, packet);
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
          return false;
        }
    }
  else
    return false; // cannot insert entry
}

template<class Policy>
void 
ContentStoreImpl<Policy>::Print (std::ostream &os) const
{
  for (typename super::policy_container::const_iterator item = this->getPolicy ().begin ();
       item != this->getPolicy ().end ();
       item++)
  // BOOST_FOREACH (const typename super::parent_trie &item, this->getPolicy ())
    {
      os << item->payload ()->GetName () << std::endl;
    }
}

template<class Policy>
void 
ContentStoreImpl<Policy>::SetMaxSize (uint32_t maxSize)
{
  this->getPolicy ().set_max_size (maxSize);
}

template<class Policy>
uint32_t
ContentStoreImpl<Policy>::GetMaxSize () const
{
  return this->getPolicy ().get_max_size ();
}

template<class Policy>
uint32_t
ContentStoreImpl<Policy>::GetSize () const
{
  return this->getPolicy ().size ();
}

template<class Policy>
Ptr<Entry>
ContentStoreImpl<Policy>::Begin ()
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
ContentStoreImpl<Policy>::End ()
{
  return 0;
}

template<class Policy>
Ptr<Entry>
ContentStoreImpl<Policy>::Next (Ptr<Entry> from)
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


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// explicit instantiation and registering
template class ContentStoreImpl<lru_policy_traits>;
template class ContentStoreImpl<random_policy_traits>;
template class ContentStoreImpl<fifo_policy_traits>;

NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, lru_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, random_policy_traits);
NS_OBJECT_ENSURE_REGISTERED_TEMPL(ContentStoreImpl, fifo_policy_traits);


} // namespace cs
} // namespace ndn
} // namespace ns3
