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

#include "ndn_contentstore.h"

namespace ns3
{
namespace NDNabstraction
{
    
ContentStore::ContentStore( int maxSize )
: m_maxSize(maxSize) { }


ContentStore::~ContentStore( ) 
{ }

//Find corresponding CS entry for the given content name
CsEntry* 
ContentStore::Lookup(const char *prefix )
{
	CriticalSection section(m_csMutex);
    
    struct CsEntry *result = NULL;
    
    HASH_FIND_STR (m_contentStore, prefix, result);
    
    if(result != NULL)
        Promote (*result);
    
	return result;
}



//Add entry to content store, if content store is full, use LRU replacement
void 
ContentStore::Add( const char *contentName, int contentSize )
{
    CriticalSection section(m_csMutex);
    
    // removing the old record
    struct CsEntry *tmp = NULL;
    HASH_FIND_STR (m_contentStore, contentName, tmp);
    HASH_DELETE (hh, m_contentStore,tmp);
    //free(tmp);
    
    int size = (int)HASH_COUNT(m_contentStore);
    
	if(size == m_maxSize )
	{
		CsEntry *entry = m_LRU.back();
        HASH_DELETE (hh, m_contentStore,entry);//_cs.erase( entry->contentName );
		m_LRU.pop_back( );
	}
    
	struct CsEntry *ce = (struct CsEntry*)malloc(sizeof(struct CsEntry));
	ce->contentName = (char*)contentName;
	ce->contentSize = contentSize;
    
	//_cs[ contentName ] = ce;
    HASH_ADD_KEYPTR (hh, m_contentStore, ce->contentName, strlen(ce->contentName), ce);
    
	//CsEntry *ce_in_hash = &(_cs[ contentName ]);
    struct CsEntry *ce_in_hash = NULL;
    HASH_FIND_STR (m_contentStore, contentName, ce_in_hash);
	m_LRU.push_front( ce_in_hash );
	ce_in_hash->lruPosition = m_LRU.begin( );
}


//move the given CS entry to the head of the list
void 
ContentStore::Promote(CsEntry &ce )
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

void 
ContentStore::Dump()
{
	CriticalSection section(m_csMutex);
    
    struct CsEntry *s, *tmp;
    
    HASH_ITER(hh, m_contentStore, s, tmp) 
    {
        printf("-%s-", s->contentName);
        /* ... it is safe to delete and free s here */
    }
    
    printf("\n");
    
}
}
}
