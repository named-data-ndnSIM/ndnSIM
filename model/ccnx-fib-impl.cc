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
    .SetParent<Object> ()
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


CcnxFib::iterator
CcnxFibImpl::LongestPrefixMatch (const CcnxInterestHeader &interest) const
{
  super::iterator item = const_cast<CcnxFibImpl*> (this)->super::longest_prefix_match (interest.GetName ());
  // @todo use predicate to search with exclude filters

  if (item == super::end ())
    return 0;
  else
    return item->payload ();
}


CcnxFib::iterator
CcnxFibImpl::Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric)
{
  return Add (Create<CcnxNameComponents> (prefix), face, metric);
}
  
CcnxFib::iterator
CcnxFibImpl::Add (const Ptr<const CcnxNameComponents> &prefix, Ptr<CcnxFace> face, int32_t metric)
{
  NS_LOG_FUNCTION(this->GetObject<Node> ()->GetId () << boost::cref(*prefix) << boost::cref(*face) << metric);

  // will add entry if doesn't exists, or just return an iterator to the existing entry
  std::pair< super::iterator, bool > result = super::insert (*prefix, Create<CcnxFibEntry> (prefix));
  
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

void
CcnxFibImpl::Invalidate (const Ptr<const CcnxNameComponents> &prefix)
{
  NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*prefix));

  super::iterator foundItem, lastItem;
  bool reachLast;
  boost::tie (foundItem, reachLast, lastItem) = super::getTrie ().find (*prefix);
  
  if (!reachLast || lastItem->payload () == 0)
    return; // nothing to invalidate

  super::modify (lastItem,
                 ll::bind (&CcnxFibEntry::Invalidate, ll::_1));
}

void
CcnxFibImpl::InvalidateAll ()
{
  // NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId ());

  // for (super::iterator entry = m_fib.begin ();
  //      entry != m_fib.end ();
  //      entry ++)
  //   {
  //     m_fib.modify (entry,
  //                   ll::bind (&CcnxFibEntry::Invalidate, ll::_1));
  //   }
}

void
CcnxFibImpl::Remove (const CcnxFibEntry &entry, Ptr<CcnxFace> face)
{
  // NS_LOG_FUNCTION (this);

  // m_fib.modify (m_fib.iterator_to (entry),
  //               ll::bind (&CcnxFibEntry::RemoveFace, ll::_1, face));
  // if (entry.m_faces.size () == 0)
  //   {
  //     m_fib.erase (m_fib.iterator_to (entry));
  //   }
}

void
CcnxFibImpl::RemoveFromAll (Ptr<CcnxFace> face)
{
  // NS_LOG_FUNCTION (this);

  // for_each (m_fib.begin (), m_fib.end (), 
  //           ll::bind (static_cast< void (CcnxFib::*) (const CcnxFibEntry &, Ptr<CcnxFace>) > (&CcnxFib::Remove),
  //                     this, ll::_1, face));
}

void
CcnxFibImpl::Print (std::ostream &os) const
{
  // for (super::iterator entry = fib.m_fib.begin ();
  //      entry != fib.m_fib.end ();
  //      entry++)
  //   {
  //     os << entry->GetPrefix () << "\t" << *entry << "\n";
  //   }
}

} // namespace ns3
