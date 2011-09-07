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

#include "ccnx-content-store.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"
#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE ("CcnxContentStore");

namespace ns3
{

// NS_OBJECT_ENSURE_REGISTERED (CcnxContentStore);

using namespace __ccnx_private;

TypeId 
CcnxContentStore::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CcnxContentStore")
    .SetGroupName ("Ccnx")
    .SetParent<Object> ()
    .AddConstructor<CcnxContentStore> ()
    .AddAttribute ("Size",
                   "Maximum number of packets that content storage can hold",
                   UintegerValue (100),
                   MakeUintegerAccessor (&CcnxContentStore::SetMaxSize,
                                         &CcnxContentStore::GetMaxSize),
                   MakeUintegerChecker<uint32_t> ())
    ;

  return tid;
}

CcnxContentObjectTail CcnxContentStoreEntry::m_tail;

//////////////////////////////////////////////////////////////////////
// Helper classes
//////////////////////////////////////////////////////////////////////
/**
 * \ingroup ccnx
 * \brief Typedef for hash index of content store container
 */
struct CcnxContentStoreByPrefix
{
  typedef
  CcnxContentStoreContainer::type::index<i_prefix>::type
  type;
};

/**
 * \ingroup ccnx
 * \brief Typedef for MRU index of content store container
 */
struct CcnxContentStoreByMru
{
  typedef
  CcnxContentStoreContainer::type::index<i_mru>::type
  type;
};

#ifdef _DEBUG
#define DUMP_INDEX_TAG i_ordered
#define DUMP_INDEX     CcnxContentStoreOrderedPrefix
/**
 * \ingroup ccnx
 * \brief Typedef for ordered index of content store container
 */
struct CcnxContentStoreOrderedPrefix
{
  typedef
  CcnxContentStoreContainer::type::index<i_ordered>::type
  type;
};
#else
#define DUMP_INDEX_TAG i_mru
#define DUMP_INDEX     CcnxContentStoreByMru
#endif

//////////////////////////////////////////////////////////////////////

CcnxContentStoreEntry::CcnxContentStoreEntry (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet)
  : m_header (header)
{
  m_packet = packet->Copy ();
  m_packet->RemoveHeader (*header);
  m_packet->RemoveTrailer (m_tail);
}

Ptr<Packet>
CcnxContentStoreEntry::GetFullyFormedCcnxPacket () const
{
  Ptr<Packet> packet = m_packet->Copy ();
  packet->AddHeader (*m_header);
  packet->AddTrailer (m_tail);
  return packet;
}

// /// Disabled copy constructor
// CcnxContentStoreEntry::CcnxContentStoreEntry (const CcnxContentStoreEntry &o)
// {
// }

// /// Disables copy operator
// CcnxContentStoreEntry& CcnxContentStoreEntry::operator= (const CcnxContentStoreEntry &o)
// {
//   return *this;
// }



CcnxContentStore::CcnxContentStore( )
  : m_maxSize(100) { } // this value shouldn't matter, NS-3 should call SetSize with default value specified in AddAttribute earlier
        
CcnxContentStore::~CcnxContentStore( ) 
{ }

/// Disabled copy constructor
CcnxContentStore::CcnxContentStore (const CcnxContentStore &o)
{
}

/// Disables copy operator
CcnxContentStore& CcnxContentStore::operator= (const CcnxContentStore &o)
{
  return *this;
}


Ptr<Packet>
CcnxContentStore::Lookup (Ptr<const CcnxInterestHeader> interest)
{
  NS_LOG_FUNCTION_NOARGS ();
  CcnxContentStoreContainer::type::iterator it = m_contentStore.get<i_prefix> ().find (interest->GetName ());
  if (it != m_contentStore.end ())
    {
      // promote entry to the top
      m_contentStore.get<i_mru> ().relocate (m_contentStore.get<i_mru> ().begin (),
                                           m_contentStore.project<i_mru> (it));

      // return fully formed CCNx packet
      return it->GetFullyFormedCcnxPacket ();
    }
  return 0;
}   
    
void 
CcnxContentStore::Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION_NOARGS ();
  CcnxContentStoreContainer::type::iterator it = m_contentStore.get<i_prefix> ().find (header->GetName ());
  if (it == m_contentStore.end ())
    { // add entry to the top
      m_contentStore.get<i_mru> ().push_front (CcnxContentStoreEntry (header, packet));
      if (m_contentStore.size () > m_maxSize)
        m_contentStore.get<i_mru> ().pop_back ();
    }
  else
    {
      // promote entry to the top
      m_contentStore.get<i_mru> ().relocate (m_contentStore.get<i_mru> ().begin (),
                                           m_contentStore.project<i_mru> (it));
    }
}
    
void 
CcnxContentStore::Print() const
{
  for( DUMP_INDEX::type::iterator it=m_contentStore.get<DUMP_INDEX_TAG> ().begin ();
       it != m_contentStore.get<DUMP_INDEX_TAG> ().end ();
       it++
       )
    {
      NS_LOG_INFO (it->GetName ());
    }
}

} // namespace ns3
