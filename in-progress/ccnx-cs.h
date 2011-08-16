/* 
 * File:   ndn_cs.h
 * Author: cawka
 *
 * Created on December 15, 2010, 2:17 PM
 */

#ifndef NDN_CS_H
#define	NDN_CS_H

#include "ndn_common.h"
#include "hash_helper.h"
#include <qualnet_mutex.h>
#include <list>
#include <string>

using namespace std;

class CsEntry;
typedef list<CsEntry*>::iterator CsLruIterator;

//structure for CS entry
struct CsEntry
{
    string contentName;
    int contentSize;

	CsLruIterator lruPosition;
};

typedef string_key_hash_t<CsEntry>::point_iterator CsIterator;
typedef string_key_hash_t<CsEntry>::iterator CsRangeIterator;

// class implementing NDN content store
class NdnCs
{
public:
	NdnCs( int max_size=NDN_CONTENT_STORE_SIZE );
	virtual ~NdnCs( );

	// Find corresponding CS entry for the given content name
	CsIterator lookup( const string &prefix );
	bool isValid( const CsIterator &it ) { return it!=_cs.end(); }
	
	// Add new content to the content store. Old content will be replaced
	void add( const string &contentName, int contentSize );

	// Dump content store entries
	void dump( );

protected:
	//move the given CS entry to the head of the list
	void promote( CsEntry &entry );

private:
	int							_maxSize; // maximum number of entries in cache

	string_key_hash_t<CsEntry>	_cs;	// actual content store

	list<CsEntry*>				_lru;	// LRU index of the content store
	QNThreadMutex				_csMutex; // just to make sure we are not
										  // getting problems with multiple threads
};

#endif	/* NDN_CS_H */
