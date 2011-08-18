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
 */

#include "ccnx-content-store.h"
#include "ns3/log.h"


NS_LOG_COMPONENT_DEFINE ("CcnxContentStore");

namespace ns3
{
        
CcnxContentStore::CcnxContentStore( int maxSize )
    : m_maxSize(maxSize) { }
        
CcnxContentStore::~CcnxContentStore( ) 
    { }

//Find corresponding CS entry for the given content name
CsEntry* 
CcnxContentStore::Lookup(const string prefix )
{
    CriticalSection section(m_csMutex);
        
    CsEntry *result = &(m_contentStore.at(prefix));
    
    if(result != NULL)
        Promote (*result);
        
    return result;
}   
    
//move the given CS entry to the head of the list
void 
CcnxContentStore::Promote(CsEntry &ce )
{
    // should not lock mutex. Otherwise deadlocks will be welcome
    if( m_LRU.front() == &ce ) return;
        
    //assert( *(ce.lruPosition)==&ce ); // should point to the same object
        
    // swaping positions in _lru
    m_LRU.erase( ce.lruPosition );
    m_LRU.push_front( &ce );
    ce.lruPosition = m_LRU.begin( );
        
    //assert( *(ce.lruPosition)==&ce ); // should point to the same object
}
    
//Add entry to content store, if content store is full, use LRU replacement
void 
CcnxContentStore::Add( const string contentName, int contentSize )
{
    CriticalSection section(m_csMutex);
        
    m_contentStore.erase(m_contentStore.find(contentName));
    
    if((int)m_contentStore.size() == m_maxSize )
    {
        CsEntry *entry = m_LRU.back();
        m_contentStore.erase(m_contentStore.find(entry->contentName));
        m_LRU.pop_back( );
    }
        
    CsEntry ce;
    ce.contentName = contentName;
    ce.contentSize = contentSize;
    
    m_contentStore[contentName] = ce;
    
    CsEntry *ce_in_hash = &(m_contentStore.at(contentName));
    m_LRU.push_front( ce_in_hash );
    ce_in_hash->lruPosition = m_LRU.begin( );
}
    
void 
CcnxContentStore::Dump()
{
    CriticalSection section(m_csMutex);
        
    BOOST_FOREACH(string_key_hash_t<CsEntry>::value_type i, m_contentStore) 
    {
        NS_LOG_INFO ("Key = " << i.first << " Value = " << i.second.contentName);
    }
}
}