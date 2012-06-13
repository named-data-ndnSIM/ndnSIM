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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ccnx-content-store-lru.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE ("CcnxContentStoreLru");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED (CcnxContentStoreLru);

TypeId 
CcnxContentStoreLru::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentStoreLru")
    .SetGroupName ("Ccnx")
    .SetParent<CcnxContentStore> ()
    .AddConstructor<CcnxContentStoreLru> ()
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&CcnxContentStoreLru::SetMaxSize,
                                         &CcnxContentStoreLru::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

void
CcnxContentStoreLru::SetMaxSize (uint32_t maxSize)
{
  m_maxSize = maxSize;
  m_contentStore.getPolicy ().set_max_size (maxSize);
}

uint32_t
CcnxContentStoreLru::GetMaxSize () const
{
  return m_maxSize;
}


//////////////////////////////////////////////////////////////////////
// Helper classes
//////////////////////////////////////////////////////////////////////
/**
 * \ingroup ccnx
 * \brief Typedef for hash index of content store container
 */

CcnxContentStoreLru::CcnxContentStoreLru ()
  : m_maxSize (100)
{ } // this value shouldn't matter, NS-3 should call SetSize with default value specified in AddAttribute earlier
        
CcnxContentStoreLru::~CcnxContentStoreLru () 
{ }


boost::tuple<Ptr<Packet>, Ptr<const CcnxContentObjectHeader>, Ptr<const Packet> >
CcnxContentStoreLru::Lookup (Ptr<const CcnxInterestHeader> interest)
{
  NS_LOG_FUNCTION (this << interest->GetName ());

  /// @todo Change to search with predicate
  CcnxContentStoreLruContainer::iterator node =
    m_contentStore.deepest_prefix_match (interest->GetName ());
  
  if (node != m_contentStore.end ())
    {
      m_cacheHitsTrace (interest, node->payload ()->GetHeader ());

      NS_LOG_DEBUG ("cache hit with " << node->payload ()->GetHeader ()->GetName ());
      return boost::make_tuple (node->payload ()->GetFullyFormedCcnxPacket (),
                                node->payload ()->GetHeader (),
                                node->payload ()->GetPacket ());
    }
  else
    {
      NS_LOG_DEBUG ("cache miss for " << interest->GetName ());
      m_cacheMissesTrace (interest);
      return boost::tuple<Ptr<Packet>, Ptr<CcnxContentObjectHeader>, Ptr<Packet> > (0, 0, 0);
    }
}   
    
bool 
CcnxContentStoreLru::Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << header->GetName ());

  return
    m_contentStore
    .insert (header->GetName (), Create<CcnxContentStoreEntry> (header, packet))
    .second;
}
    
void 
CcnxContentStoreLru::Print () const
{
  BOOST_FOREACH (const CcnxContentStoreLruContainer::parent_trie &item, m_contentStore.getPolicy ())
    {
      NS_LOG_INFO (item.payload ()->GetName ());
    }
}

} // namespace ns3
