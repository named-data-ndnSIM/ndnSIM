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

#ifndef ndn_contentstore_h
#define	ndn_contentstore_h

#include <ns3/system-mutex.h>
#include <list>
#include <string>
#include "uthash.h"

using namespace std;

//size of content store
#define NDN_CONTENT_STORE_SIZE 100

namespace  ns3
{
namespace NDNabstraction
{
    
class CsEntry;
typedef list<CsEntry*>::iterator CsLruIterator;
    
//structure for CS entry
struct CsEntry
{
    char *contentName;
    int contentSize;
    
	CsLruIterator lruPosition;
    UT_hash_handle hh;         /* makes this structure hashable */
};


class ContentStore
{
public:
	ContentStore( int max_size=NDN_CONTENT_STORE_SIZE );
	virtual ~ContentStore( );
    
	// Find corresponding CS entry for the given content name
	CsEntry* Lookup( const char *prefix );
	//bool isValid( const CsIterator &it ) { return it!=_cs.end(); }
	
	// Add new content to the content store. Old content will be replaced
	void Add( const char *contentName, int contentSize );
    
	// Dump content store entries
	void Dump( );
    
protected:
	//move the given CS entry to the head of the list
	void Promote( CsEntry &entry );
    
private:
	int                   m_maxSize; // maximum number of entries in cache
    
    struct CsEntry  *m_contentStore;     // actual content store
	list<CsEntry*>			  m_LRU;	// LRU index of the content store
	SystemMutex				m_csMutex;   // just to make sure we are not
};
    
}
}
#endif
