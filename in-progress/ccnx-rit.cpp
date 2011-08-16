/* 
 * File:   ndn_rit.cpp
 * Author: cawka
 * 
 * Created on December 15, 2010, 2:02 PM
 */

#include "ndn_rit.h"

NdnRit::NdnRit( ) { }

NdnRit::~NdnRit( ) { }


//find corresponding RIT entry for the given nonce
RitIterator NdnRit::lookup( int nonce )
{
	QNThreadLock lock( &_ritMutex );

	RitIterator entry=_rit.find( nonce );
	return entry;
}


// add new RIT entry
// returns false if entry already exists, otherwise returns true
bool NdnRit::add( int nonce, clocktype expire )
{
	QNThreadLock lock( &_ritMutex );

	RitIterator entry=_rit.find( nonce );
	if( isValid(entry) ) return false;

    RitEntry re;
    re.nonce = nonce;
	re.expireTime = expire;

	_rit[ nonce ] = re;

	_ritExpirationList.push_back( &(_rit[nonce]) );
}


void NdnRit::cleanExpired( clocktype time )
{
	QNThreadLock lock( &_ritMutex );

	while( !_ritExpirationList.empty() )
    {
        RitEntry *head = _ritExpirationList.front( );

        if( head->expireTime <= time )
		{
			//delete the head RIT entry
			_rit.erase( head->nonce );
			_ritExpirationList.pop_front( );
		}
        else
            break;
    }
    //printf("Node %d: RIT entries\n", node->nodeId);
    //PrintRitEntries(ndn->riList);
}

