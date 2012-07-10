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

#include "ccnx-fib-impl.h"

#include "ccnx.h"
#include "ccnx-face.h"
#include "ccnx-interest-header.h"

#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/log.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("CcnxFibImpl");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (CcnxFibImpl);

TypeId 
CcnxFibImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxFib") // cheating ns3 object system
    .SetParent<CcnxFib> ()
    .SetGroupName ("Ccnx")
    .AddConstructor<CcnxFibImpl> ()
  ;
  return tid;
}

CcnxFibImpl::CcnxFibImpl ()
{
}

void
CcnxFibImpl::NotifyNewAggregate ()
{
  Object::NotifyNewAggregate ();
}

void 
CcnxFibImpl::DoDispose (void)
{
  clear ();
  Object::DoDispose ();
}


Ptr<CcnxFibEntry>
CcnxFibImpl::LongestPrefixMatch (const CcnxInterestHeader &interest) const
{
  super::iterator item = const_cast<CcnxFibImpl*> (this)->super::longest_prefix_match (interest.GetName ());
  // @todo use predicate to search with exclude filters

  if (item == super::end ())
    return 0;
  else
    return item->payload ();
}


Ptr<CcnxFibEntry>
CcnxFibImpl::Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric)
{
  return Add (Create<CcnxNameComponents> (prefix), face, metric);
}
  
Ptr<CcnxFibEntry>
CcnxFibImpl::Add (const Ptr<const CcnxNameComponents> &prefix, Ptr<CcnxFace> face, int32_t metric)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix) << boost::cref(*face) << metric);

  // will add entry if doesn't exists, or just return an iterator to the existing entry
  Ptr<CcnxFibEntryImpl> newEntry = Create<CcnxFibEntryImpl> (prefix);
  std::pair< super::iterator, bool > result = super::insert (*prefix, newEntry);
  newEntry->SetTrie (result.first);
  
  NS_ASSERT_MSG (face != NULL, "Trying to modify NULL face");
  
  super::modify (result.first,
                 ll::bind (&CcnxFibEntry::AddOrUpdateRoutingMetric, ll::_1, face, metric));
    
  return result.first->payload ();
}

void
CcnxFibImpl::Remove (const Ptr<const CcnxNameComponents> &prefix)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

  super::erase (*prefix);
}

// void
// CcnxFibImpl::Invalidate (const Ptr<const CcnxNameComponents> &prefix)
// {
//   NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

//   super::iterator foundItem, lastItem;
//   bool reachLast;
//   boost::tie (foundItem, reachLast, lastItem) = super::getTrie ().find (*prefix);
  
//   if (!reachLast || lastItem->payload () == 0)
//     return; // nothing to invalidate

//   super::modify (lastItem,
//                  ll::bind (&CcnxFibEntry::Invalidate, ll::_1));
// }

void
CcnxFibImpl::InvalidateAll ()
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId ());

  super::parent_trie::recursive_iterator item (super::getTrie ());
  super::parent_trie::recursive_iterator end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;

      super::modify (&(*item),
                     ll::bind (&CcnxFibEntry::Invalidate, ll::_1));
    }
}

void
CcnxFibImpl::Remove (super::parent_trie &item, Ptr<CcnxFace> face)
{
  if (item.payload () == 0) return;
  NS_LOG_FUNCTION (this);

  super::modify (&item,
                 ll::bind (&CcnxFibEntry::RemoveFace, ll::_1, face));
}

void
CcnxFibImpl::RemoveFromAll (Ptr<CcnxFace> face)
{
  NS_LOG_FUNCTION (this);

  std::for_each (super::parent_trie::recursive_iterator (super::getTrie ()),
                 super::parent_trie::recursive_iterator (0), 
                 ll::bind (static_cast< void (CcnxFib::*) (super::parent_trie &, Ptr<CcnxFace>) > (&CcnxFibImpl::Remove),
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
CcnxFibImpl::Print (std::ostream &os) const
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

Ptr<const CcnxFibEntry>
CcnxFibImpl::Begin ()
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

Ptr<const CcnxFibEntry>
CcnxFibImpl::End ()
{
  return 0;
}

Ptr<const CcnxFibEntry>
CcnxFibImpl::Next (Ptr<const CcnxFibEntry> from)
{
  if (from == 0) return 0;
  
  super::parent_trie::const_recursive_iterator item (*StaticCast<const CcnxFibEntryImpl> (from)->to_iterator ());
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
