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

#include "ndn-fib-impl.h"

#include "ns3/ndn.h"
#include "ns3/ndn-face.h"
#include "ns3/ndn-interest-header.h"

#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/log.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("NdnFibImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NdnFibImpl);

TypeId 
NdnFibImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdnFib") // cheating ns3 object system
    .SetParent<NdnFib> ()
    .SetGroupName ("Ndn")
    .AddConstructor<NdnFibImpl> ()
  ;
  return tid;
}

NdnFibImpl::NdnFibImpl ()
{
}

void
NdnFibImpl::NotifyNewAggregate ()
{
  Object::NotifyNewAggregate ();
}

void 
NdnFibImpl::DoDispose (void)
{
  clear ();
  Object::DoDispose ();
}


Ptr<NdnFibEntry>
NdnFibImpl::LongestPrefixMatch (const NdnInterestHeader &interest)
{
  super::iterator item = super::longest_prefix_match (interest.GetName ());
  // @todo use predicate to search with exclude filters

  if (item == super::end ())
    return 0;
  else
    return item->payload ();
}


Ptr<NdnFibEntry>
NdnFibImpl::Add (const NdnNameComponents &prefix, Ptr<NdnFace> face, int32_t metric)
{
  return Add (Create<NdnNameComponents> (prefix), face, metric);
}
  
Ptr<NdnFibEntry>
NdnFibImpl::Add (const Ptr<const NdnNameComponents> &prefix, Ptr<NdnFace> face, int32_t metric)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix) << boost::cref(*face) << metric);

  // will add entry if doesn't exists, or just return an iterator to the existing entry
  std::pair< super::iterator, bool > result = super::insert (*prefix, 0);
  if (result.first != super::end ())
    {
      if (result.second)
        {
            Ptr<NdnFibEntryImpl> newEntry = Create<NdnFibEntryImpl> (prefix);
            newEntry->SetTrie (result.first);
            result.first->set_payload (newEntry);
        }
  
      super::modify (result.first,
                     ll::bind (&NdnFibEntry::AddOrUpdateRoutingMetric, ll::_1, face, metric));
    
      return result.first->payload ();
    }
  else
    return 0;
}

void
NdnFibImpl::Remove (const Ptr<const NdnNameComponents> &prefix)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

  super::erase (*prefix);
}

// void
// NdnFibImpl::Invalidate (const Ptr<const NdnNameComponents> &prefix)
// {
//   NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

//   super::iterator foundItem, lastItem;
//   bool reachLast;
//   boost::tie (foundItem, reachLast, lastItem) = super::getTrie ().find (*prefix);
  
//   if (!reachLast || lastItem->payload () == 0)
//     return; // nothing to invalidate

//   super::modify (lastItem,
//                  ll::bind (&NdnFibEntry::Invalidate, ll::_1));
// }

void
NdnFibImpl::InvalidateAll ()
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId ());

  super::parent_trie::recursive_iterator item (super::getTrie ());
  super::parent_trie::recursive_iterator end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;

      super::modify (&(*item),
                     ll::bind (&NdnFibEntry::Invalidate, ll::_1));
    }
}

void
NdnFibImpl::RemoveFace (super::parent_trie &item, Ptr<NdnFace> face)
{
  if (item.payload () == 0) return;
  NS_LOG_FUNCTION (this);

  super::modify (&item,
                 ll::bind (&NdnFibEntry::RemoveFace, ll::_1, face));
}

void
NdnFibImpl::RemoveFromAll (Ptr<NdnFace> face)
{
  NS_LOG_FUNCTION (this);

  std::for_each (super::parent_trie::recursive_iterator (super::getTrie ()),
                 super::parent_trie::recursive_iterator (0), 
                 ll::bind (&NdnFibImpl::RemoveFace,
                           this, ll::_1, face));

  super::parent_trie::recursive_iterator trieNode (super::getTrie ());
  super::parent_trie::recursive_iterator end (0);
  for (; trieNode != end; trieNode++)
    {
      if (trieNode->payload () == 0) continue;
      
      if (trieNode->payload ()->m_faces.size () == 0)
        {
          trieNode = super::parent_trie::recursive_iterator (trieNode->erase ());
        }
    }
}

void
NdnFibImpl::Print (std::ostream &os) const
{
  // !!! unordered_set imposes "random" order of item in the same level !!!
  super::parent_trie::const_recursive_iterator item (super::getTrie ());
  super::parent_trie::const_recursive_iterator end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;

      os << item->payload ()->GetPrefix () << "\t" << *item->payload () << "\n";
    }
}

uint32_t
NdnFibImpl::GetSize () const
{
  return super::getPolicy ().size ();
}

Ptr<const NdnFibEntry>
NdnFibImpl::Begin ()
{
  super::parent_trie::const_recursive_iterator item (super::getTrie ());
  super::parent_trie::const_recursive_iterator end (0);
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

Ptr<const NdnFibEntry>
NdnFibImpl::End ()
{
  return 0;
}

Ptr<const NdnFibEntry>
NdnFibImpl::Next (Ptr<const NdnFibEntry> from)
{
  if (from == 0) return 0;
  
  super::parent_trie::const_recursive_iterator item (*StaticCast<const NdnFibEntryImpl> (from)->to_iterator ());
  super::parent_trie::const_recursive_iterator end (0);
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
