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

using namespace __ccnx_private;

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
struct CcnxContentStoreByPrefix
{
  typedef
  CcnxContentStoreLruContainer::type::index<i_prefix>::type
  type;
};

CcnxContentStoreLru::CcnxContentStoreLru ()
  : m_maxSize (100)
{ } // this value shouldn't matter, NS-3 should call SetSize with default value specified in AddAttribute earlier
        
CcnxContentStoreLru::~CcnxContentStoreLru () 
{ }


boost::tuple<Ptr<Packet>, Ptr<const CcnxContentObjectHeader>, Ptr<const Packet> >
CcnxContentStoreLru::Lookup (Ptr<const CcnxInterestHeader> interest)
{
  NS_LOG_FUNCTION (this << interest->GetName ());
  CcnxContentStoreLruContainer::type::iterator it = m_contentStore.get<i_prefix> ().find (interest->GetName ());
  if (it != m_contentStore.end ())
    {
      // promote entry to the top
      m_contentStore.get<i_mru> ().relocate (m_contentStore.get<i_mru> ().begin (),
                                           m_contentStore.project<i_mru> (it));

      m_cacheHitsTrace (interest, it->GetHeader ());
      
      // return fully formed CCNx packet
      return boost::make_tuple (it->GetFullyFormedCcnxPacket (), it->GetHeader (), it->GetPacket ());
    }
  m_cacheMissesTrace (interest);
  return boost::tuple<Ptr<Packet>, Ptr<CcnxContentObjectHeader>, Ptr<Packet> > (0, 0, 0);
}   
    
bool 
CcnxContentStoreLru::Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet)
{
  NS_LOG_FUNCTION (this << header->GetName ());
  CcnxContentStoreLruContainer::type::iterator it = m_contentStore.get<i_prefix> ().find (header->GetName ());
  if (it == m_contentStore.end ())
    { // add entry to the top
      m_contentStore.get<i_mru> ().push_front (CcnxContentStoreEntry (header, packet));
      if (m_contentStore.size () > m_maxSize)
        m_contentStore.get<i_mru> ().pop_back ();
      return false;
    }
  else
    {
      /// @todo Wrong!!! Record should be updated and relocated, not just relocated
      // promote entry to the top
      m_contentStore.get<i_mru> ().relocate (m_contentStore.get<i_mru> ().begin (),
                                             m_contentStore.project<i_mru> (it));
      return true;
    }
}
    
void 
CcnxContentStoreLru::Print() const
{
  BOOST_FOREACH (const CcnxContentStoreEntry &entry, m_contentStore.get<i_mru> ())
    {
      NS_LOG_INFO (entry.GetName ());
    }
}

} // namespace ns3
