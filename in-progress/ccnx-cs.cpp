/* 
 * File:   ndn_cs.cpp
 * Author: cawka
 * 
 * Created on December 15, 2010, 2:17 PM
 */

#include "ndn_cs.h"

NdnCs::NdnCs( int maxSize )
: _maxSize(maxSize) { }

//NdnCs::NdnCs( constNdnCs& orig ) { }

NdnCs::~NdnCs( ) { }

//Find corresponding CS entry for the given content name
CsIterator NdnCs::lookup( const string &prefix )
{
	QNThreadLock lock( &_csMutex );

	CsIterator entry=_cs.find( prefix );
	if( entry!=_cs.end() ) promote( entry->second );
	return entry;
}



//Add entry to content store, if content store is full, use LRU replacement
void NdnCs::add( const string &contentName, int contentSize )
{
	QNThreadLock lock( &_csMutex );

	_cs.erase( contentName ); // removing the old record

	if( _cs.size()==_maxSize )
	{
		CsEntry *entry=_lru.back();
		_cs.erase( entry->contentName );
		_lru.pop_back( );
	}

	CsEntry ce;
	ce.contentName = contentName;
	ce.contentSize = contentSize;

	_cs[ contentName ] = ce;

	CsEntry *ce_in_hash = &(_cs[ contentName ]);
	_lru.push_front( ce_in_hash );
	ce_in_hash->lruPosition = _lru.begin( );
}


//move the given CS entry to the head of the list
void NdnCs::promote( CsEntry &ce )
{
	// should not lock mutex. Otherwise deadlocks will be welcome
	if( _lru.front() == &ce ) return;

	assert( *(ce.lruPosition)==&ce ); // should point to the same object

	// swaping positions in _lru
	_lru.erase( ce.lruPosition );
	_lru.push_front( &ce );
	ce.lruPosition = _lru.begin( );

	assert( *(ce.lruPosition)==&ce ); // should point to the same object
}

void NdnCs::dump()
{
	QNThreadLock lock( &_csMutex );

	for( CsRangeIterator it=_cs.begin(); it!=_cs.end(); ++it )
	{
		printf("-%s-", it->second.contentName.c_str() );
	}

    printf("\n");
//    list<CsEntry *>::reverse_iterator rit;
//
//    for (rit = contentList->rbegin(); rit != contentList->rend(); rit ++)
//    {
//        temp = *rit;
//        printf("=%s=", temp->contentName);
//    }
//
//    printf("\n");
}
