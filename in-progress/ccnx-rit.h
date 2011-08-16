/* 
 * File:   ndn_rit.h
 * Author: cawka
 *
 * Created on December 15, 2010, 2:02 PM
 */

#ifndef NDN_RIT_H
#define	NDN_RIT_H

#include "hash_helper.h"
#include <qualnet_mutex.h>
#include <clock.h>
#include <list>
using namespace std;

//recent interest
struct RitEntry
{
    int nonce;
    clocktype expireTime;  //RIT entries will stay in the table for a while before being cleaned out
};

typedef int_key_hash_t<RitEntry>::point_iterator RitIterator;
typedef int_key_hash_t<RitEntry>::iterator RitRangeIterator;

class NdnRit
{
public:
	NdnRit( );
	virtual ~NdnRit( );

	//Find corresponding RIT entry for the given content name
	RitIterator lookup( int nonce );
	bool isValid( const RitIterator &it ) { return it!=_rit.end(); }

	//add new RIT entry
	bool add( int nonce, clocktype expireTime );

	// Delete expired records
	void cleanExpired( clocktype time );
	
private:
	int_key_hash_t<RitEntry> _rit;

	list<RitEntry*> _ritExpirationList;

	QNThreadMutex   _ritMutex;
};

#endif	/* NdnRit_H */

