/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
//
// Copyright (c) 2010,2011 UCLA
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: 
//

#include "ccnx-pit.h"
#include <algorithm>

CcnxPit::CcnxPit( Ccnx &node )
: _node(node)
{
};

CcnxPit::~CcnxPit( ) { }

//Find corresponding CS entry for the given content name
PitIterator CcnxPit::lookup( const string &prefix )
{
	QNThreadLock lock( &_pitMutex );

	PitIterator entry=_pit.find( prefix );
	return entry;
}

// add new PIT entry
bool CcnxPit::add( const string &name, const PitIncomingInterest &interest )
{
	QNThreadLock lock( &_pitMutex );

	PitEntry *entry=NULL;

	PitIterator existent_entry = _pit.find( name );
	if( isValid(existent_entry) )
	{
		if( VALUE(existent_entry).timerExpired )
		{
			_node.fillAvailableInterfacesInPitEntry( VALUE(existent_entry) );
		}

		add( VALUE(existent_entry), interest );
	}
	else
	{
		PitEntry &entry = _pit[ name ];
		entry.contentName  = name;

		_node.fillAvailableInterfacesInPitEntry( entry );

		add( entry, interest );
	}
}

// Remove expired records from PIT
void CcnxPit::cleanExpired( clocktype time )
{
	QNThreadLock lock( &_pitMutex );

    while( !_pitExpirationList.empty() )
    {
		PitExpirationIterator entry = _pitExpirationList.begin( );

        if( VALUE(entry)->expireTime <= time )
        {
			deleteIncomingInterest( *(KEY(entry)), VALUE(entry)->interfaceIndex );

			// delete entry if all incoming interests expired
			if( KEY(entry)->incomingInterests.size()==0 )
			{
				_pit.erase( KEY(entry)->contentName );
			}
        }
        else
            break;
    }
}

//delete PIT entry
void CcnxPit::erase( const string &name )
{
	// should not call `lookup' method !!!
	
	QNThreadLock lock( &_pitMutex );

    PitIterator pe = _pit.find( name );
	
	if( !isValid(pe) ) return;

	if( VALUE(pe).timerMsg ) MESSAGE_CancelSelfMsg( _node.getNode(), VALUE(pe).timerMsg );

	PitIncomingIterator it = VALUE(pe).incomingInterests.begin();
	while( it!=VALUE(pe).incomingInterests.end() )
	{
		deleteIncomingInterest( VALUE(pe), it );

		it = VALUE(pe).incomingInterests.begin();
	}

	resetPendingState( VALUE(pe) );

	_pit.erase( name );
}

//delete incoming interest from PIT entry
//return 0 if interest does not exist, 1 otherwise
bool CcnxPit::deleteIncomingInterest( PitEntry &pe, int interfaceIndex )
{
	// should not lock thread !!! Otherwise there will be a deadlock
    if( pe.incomingInterests.size()==0 ) return false; //nothing to delete. Can happen when duplicate data arrives to the node

    PitIncomingIterator it = findIncoming( pe, interfaceIndex );

	if( !isValid(pe, it) ) return false;

	deleteIncomingInterest( pe, it );

    return true;
}

void CcnxPit::deleteAllIncomingInterests( PitEntry &pe )
{
	PitIncomingIterator it = pe.incomingInterests.begin();
	while( it!=pe.incomingInterests.end() )
	{
		deleteIncomingInterest( pe, it );

		it = pe.incomingInterests.begin();
	}
}

void CcnxPit::deleteIncomingInterest( PitEntry &pe, PitIncomingIterator it )
{
	assert( KEY(it->expirationPosition)==&pe );
	assert( VALUE(it->expirationPosition)->interfaceIndex==it->interfaceIndex );

	_pitExpirationList.erase( it->expirationPosition );
	pe.incomingInterests.erase( it );
}

//add new incoming interest to PIT entry
//returns false if interface already exists, true otherwise
bool CcnxPit::add( PitEntry &pe, const PitIncomingInterest &interest )
{
	pe.availableInterfaces.remove( interest.interfaceIndex );

    PitIncomingIterator it=findIncoming( pe, interest.interfaceIndex );

	if( isValid(pe, it) )
	{
		//update expiration time
		it->expireTime = interest.expireTime;
		it->nonce = interest.nonce;

		//move Interest to the end of the node's Interest list
		_pitExpirationList.erase( it->expirationPosition );
		_pitExpirationList.push_back( PitExpirationEntry(&pe,&(*it)) );
		
		it->expirationPosition = --_pitExpirationList.end();
		return false;
	}

	pe.incomingInterests.push_back( interest );
	PitIncomingInterest *incoming = &pe.incomingInterests.back();

    //Add to the end of the node's Interest list
	_pitExpirationList.push_back( PitExpirationEntry(&pe,incoming) );
	incoming->expirationPosition = -- _pitExpirationList.end();

    return true;
}

//add new outgoing interest to PIT entry
//returns false  interface limit reached or interest exists and is still marked as outstanding (nonce will not be changed)
//		  true otherwise
int CcnxPit::add( PitEntry &pe, const PitOutgoingInterest &interest )
{
	if( _node.isRateLimit && _bucketsPerInterface[interest.interfaceIndex]+1.0 >= maxBucketsPerInterface[interest.interfaceIndex] )
	{
//		printf( "DEBUG: bucket overflow. Should not forward anything to interface %d\n", interest.interfaceIndex );
		return false;
	}

	_bucketsPerInterface[interest.interfaceIndex] = _bucketsPerInterface[interest.interfaceIndex] + 1.0;
	pe.availableInterfaces.remove( interest.interfaceIndex );

	PitOutgoingIterator it = findOutgoing(pe, interest.interfaceIndex);
	if( isValid(pe, it) )
    {
		if( it->outstanding ) return false;

        it->retxNum ++;
        it->nonce = interest.nonce;
		it->outstanding = true;
		it->waitingInVain = false;
    }
	else
	{
		//add to the head of the list
		pe.outgoingInterests.push_front( interest );
	}
	
    return true;
}

void CcnxPit::resetPendingState( PitEntry &pe )
{
	for( PitOutgoingIterator it = pe.outgoingInterests.begin();
		 it != pe.outgoingInterests.end();
		 it++ )
	{
		it->outstanding = false;
	}
}

void CcnxPit::leakBuckets( )
{
	for( PitBucketIterator it=_bucketsPerInterface.begin(); 
		 it != _bucketsPerInterface.end();
		 it++ )
	{
		it->second = max( 0.0, it->second - leakSize[it->first] );
	}
}

void CcnxPit::leakBucket( int interface, int amount )
{
	_bucketsPerInterface[interface] = 
			max( 0.0, _bucketsPerInterface[interface] - amount );
}
