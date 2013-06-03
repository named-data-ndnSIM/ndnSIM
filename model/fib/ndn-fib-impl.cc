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

#include "ns3/ndn-face.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-forwarding-strategy.h"

#include "ns3/node.h"
#include "ns3/assert.h"
#include "ns3/names.h"
#include "ns3/log.h"

#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

NS_LOG_COMPONENT_DEFINE ("ndn.fib.FibImpl");

namespace ns3 {
namespace ndn {
namespace fib {

NS_OBJECT_ENSURE_REGISTERED (FibImpl);

TypeId 
FibImpl::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::fib::Default") // cheating ns3 object system
    .SetParent<Fib> ()
    .SetGroupName ("Ndn")
    .AddConstructor<FibImpl> ()
  ;
  return tid;
}

FibImpl::FibImpl ()
{
}

void
FibImpl::NotifyNewAggregate ()
{
  Object::NotifyNewAggregate ();
}

void 
FibImpl::DoDispose (void)
{
  clear ();
  Object::DoDispose ();
}


Ptr<Entry>
FibImpl::LongestPrefixMatch (const Interest &interest)
{
  super::iterator item = super::longest_prefix_match (interest.GetName ());
  // @todo use predicate to search with exclude filters

  if (item == super::end ())
    return 0;
  else
    return item->payload ();
}

Ptr<fib::Entry>
FibImpl::Find (const Name &prefix)
{
  super::iterator item = super::find_exact (prefix);

  if (item == super::end ())
    return 0;
  else
    return item->payload ();
}


Ptr<Entry>
FibImpl::Add (const Name &prefix, Ptr<Face> face, int32_t metric)
{
  return Add (Create<Name> (prefix), face, metric);
}
  
Ptr<Entry>
FibImpl::Add (const Ptr<const Name> &prefix, Ptr<Face> face, int32_t metric)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix) << boost::cref(*face) << metric);

  // will add entry if doesn't exists, or just return an iterator to the existing entry
  std::pair< super::iterator, bool > result = super::insert (*prefix, 0);
  if (result.first != super::end ())
    {
      if (result.second)
        {
          Ptr<EntryImpl> newEntry = Create<EntryImpl> (this, prefix);
          newEntry->SetTrie (result.first);
          result.first->set_payload (newEntry);
        }
  
      super::modify (result.first,
                     ll::bind (&Entry::AddOrUpdateRoutingMetric, ll::_1, face, metric));

      if (result.second)
        {
          // notify forwarding strategy about new FIB entry
          NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
          this->GetObject<ForwardingStrategy> ()->DidAddFibEntry (result.first->payload ());
        }
      
      return result.first->payload ();
    }
  else
    return 0;
}

void
FibImpl::Remove (const Ptr<const Name> &prefix)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

  super::iterator fibEntry = super::find_exact (*prefix);
  if (fibEntry != super::end ())
    {
      // notify forwarding strategy about soon be removed FIB entry
      NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
      this->GetObject<ForwardingStrategy> ()->WillRemoveFibEntry (fibEntry->payload ());

      super::erase (fibEntry);
    }
  // else do nothing
}

// void
// FibImpl::Invalidate (const Ptr<const Name> &prefix)
// {
//   NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

//   super::iterator foundItem, lastItem;
//   bool reachLast;
//   boost::tie (foundItem, reachLast, lastItem) = super::getTrie ().find (*prefix);
  
//   if (!reachLast || lastItem->payload () == 0)
//     return; // nothing to invalidate

//   super::modify (lastItem,
//                  ll::bind (&Entry::Invalidate, ll::_1));
// }

void
FibImpl::InvalidateAll ()
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId ());

  super::parent_trie::recursive_iterator item (super::getTrie ());
  super::parent_trie::recursive_iterator end (0);
  for (; item != end; item++)
    {
      if (item->payload () == 0) continue;

      super::modify (&(*item),
                     ll::bind (&Entry::Invalidate, ll::_1));
    }
}

void
FibImpl::RemoveFace (super::parent_trie &item, Ptr<Face> face)
{
  if (item.payload () == 0) return;
  NS_LOG_FUNCTION (this);

  super::modify (&item,
                 ll::bind (&Entry::RemoveFace, ll::_1, face));
}

void
FibImpl::RemoveFromAll (Ptr<Face> face)
{
  NS_LOG_FUNCTION (this);

  Ptr<Entry> entry = Begin ();
  while (entry != End ())
    {
      entry->RemoveFace (face);
      if (entry->m_faces.size () == 0)
        {
          Ptr<Entry> nextEntry = Next (entry);

          // notify forwarding strategy about soon be removed FIB entry
          NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
          this->GetObject<ForwardingStrategy> ()->WillRemoveFibEntry (entry);

          super::erase (StaticCast<EntryImpl> (entry)->to_iterator ());
          entry = nextEntry;
        }
      else
        {
          entry = Next (entry);
        }
    }
}

void
FibImpl::Print (std::ostream &os) const
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
FibImpl::GetSize () const
{
  return super::getPolicy ().size ();
}

Ptr<const Entry>
FibImpl::Begin () const
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

Ptr<const Entry>
FibImpl::End () const
{
  return 0;
}

Ptr<const Entry>
FibImpl::Next (Ptr<const Entry> from) const
{
  if (from == 0) return 0;
  
  super::parent_trie::const_recursive_iterator item (*StaticCast<const EntryImpl> (from)->to_iterator ());
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

Ptr<Entry>
FibImpl::Begin ()
{
  super::parent_trie::recursive_iterator item (super::getTrie ());
  super::parent_trie::recursive_iterator end (0);
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

Ptr<Entry>
FibImpl::End ()
{
  return 0;
}

Ptr<Entry>
FibImpl::Next (Ptr<Entry> from)
{
  if (from == 0) return 0;
  
  super::parent_trie::recursive_iterator item (*StaticCast<EntryImpl> (from)->to_iterator ());
  super::parent_trie::recursive_iterator end (0);
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


} // namespace fib
} // namespace ndn
} // namespace ns3
