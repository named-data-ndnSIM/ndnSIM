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

#ifndef CCNX_PIT_H
#define	CCNX_PIT_H

#include "ns3/nstime.h"
#include "hash-helper.h"
#include <list>

namespace ns3 {

class Ccnx;
class PitEntry;
class PitIncomingInterest;
typedef pair<PitEntry*,PitIncomingInterest*> PitExpirationEntry;
typedef list<PitExpirationEntry>::iterator PitExpirationIterator;

struct PitIncomingInterest
{
  int interfaceIndex;     // interface index of the incoming Interest
  Time expireTime;        // life time of the incoming Interest
  int nonce;				// nonce of the last received incoming Interest on this interface

  PitExpirationIterator expirationPosition;

public:
PitIncomingInterest( int _interface, clocktype _expire, int _nonce )
: interfaceIndex(_interface)
, expireTime(_expire)
	, nonce(_nonce)
  { };

  bool operator==( const PitIncomingInterest &dst ) { return interfaceIndex==dst.interfaceIndex; }
  bool operator==( int interface ) { return interfaceIndex==interface; }
};

struct PitOutgoingInterest
{
  int interfaceIndex;
  clocktype sendTime;     //time when the first outgoing interest is sent
  int retxNum;            //number of retransmission
  int nonce;              //nonce of the outgoing Interest
  bool outstanding;		//flag to indicate that this interest is currently pending
  bool waitingInVain;		//when flag is set, we do not expect data for this interest, only a small hope that it will happen
	
public:
PitOutgoingInterest( int interface, clocktype time, int nonce )
: interfaceIndex(interface)
, sendTime( time )
	, retxNum( 0 )
	, nonce( nonce )
	, outstanding( true )
	, waitingInVain( false )
  { };

  bool operator==( const PitIncomingInterest &dst ) { return interfaceIndex==dst.interfaceIndex; }
  bool operator==( int interface ) { return interfaceIndex==interface; }
};


//structure for PIT entry
struct PitEntry
{
  string contentName;
  list<PitIncomingInterest> incomingInterests;
  list<PitOutgoingInterest> outgoingInterests;
	
  bool timerExpired;      
  int  counterExpirations; //whether timer is expired (+ number of times timer expired)

  /**
   * This variable provides a list of interfaces that will be tried to propagate interest
   *
   * It should be initialized with all available interfaces upon reception of first or
   * any retransmitted (i.e., after STO expired), but not duplicate interests
   *  
   * Reaction to duplicate interests depends on strategy. 
   * 
   * In case of flooding, this variable should always be empty (it is filled initially with
   * all avaialble interfaces and then immediately emptied by interest flooding)
   * 
   * In case of routing strategies, this list will guide (and limit) the local recovery process
   */
  list<int> availableInterfaces;

  Message *timerMsg;      //the timer message, used to cancel message if data is received

  // Changing PIT removal policy. From now on it will be deleted only when all outgoing
  // interests are satisfied
  //
  //	bool dontDeleteOnFirstData;	//don't delete when first data packet comes 
  //								//(PIT will be deleted only upon timeout or reception data
  //								//packets or prunes for all outgoing interests)

public:
PitEntry()
: timerExpired( false )
, counterExpirations( 0 )
	, timerMsg( NULL )
  { }
	
  inline void fillAvailableInterfacesFromFIB( const FibEntry &fe );

  // Find best candidate, skipping `skip' first candidates (modulo availableInterfaces.size())
  // !!! assert( availableInterfaces.size()>0 ) !!!
  inline int findBestCandidate( int skip = 0 );
	
  // Get number of outgoing interests that we're expecting data from
  inline size_t numberOfPromisingInterests( ) const; 
};

typedef string_key_hash_t<PitEntry>::point_iterator PitIterator;
typedef string_key_hash_t<PitEntry>::iterator PitRangeIterator;

typedef list<PitIncomingInterest>::iterator PitIncomingIterator;
typedef list<PitOutgoingInterest>::iterator PitOutgoingIterator;
typedef list<PitOutgoingInterest>::const_iterator PitOutgoingConstIterator;

typedef map<int,int> PitCounter;
typedef map<int,int>::iterator PitCounterIterator;

typedef map<int,double> PitBucket;
typedef map<int,double>::iterator PitBucketIterator;


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// class implementing Pending Interests Table
class CcnxPit
{
 public:
  /**
   * PIT constructor
   */
  CcnxPit( Ccnx &node );
  virtual ~CcnxPit( );

  //Find corresponding PIT entry for the given content name
  PitIterator lookup( const string &prefix );
  bool isValid( const PitIterator &it ) { return it!=_pit.end(); }

  inline PitIncomingIterator findIncoming( PitEntry &pe, int interfaceIndex );
  inline bool isValid( PitEntry &pe, PitIncomingIterator it );
  inline PitOutgoingIterator findOutgoing( PitEntry &pe, int interfaceIndex );
  inline bool isValid( PitEntry &pe, PitOutgoingIterator it );

  // Add a new PIT entry and a correspondent new incoming interest
  bool add( const string &contentName, const PitIncomingInterest &interest );

  // Add a new outgoing interest
  int  add( PitEntry &pe, const PitOutgoingInterest &interest );

  // remove a PIT entry
  void erase( const string &contentName );
	
  //	inline bool InterestAlreadyForwarded( PitEntry &pe, int interfaceIndex );

  // Remove expired records from PIT
  void cleanExpired( clocktype time );

  // Dump content store entries
  void dump( );

  // Reset pending state in outgoing interests
  void resetPendingState( PitEntry &pe );

  //	// Check if there are any interfaces that we haven't sent data to yet
  //	bool areFreeInterfaces( PitEntry &pe, int interface );

  // Periodically generate pre-calculated number of tokens (leak buckets)
  void leakBuckets( );
	
  // Selectively leak a bucket
  void leakBucket( int interface, int amount );

  // Delete incoming interest for the interface
  bool deleteIncomingInterest( PitEntry &pe, int interfaceIndex );

  // Remove all incoming interests
  void deleteAllIncomingInterests( PitEntry &pe );
	
 public:
  PitBucket				 maxBucketsPerInterface; // maximum number of buckets. Automatically computed based on link capacity
  // averaging over 1 second (bandwidth * 1second)
  PitBucket				 leakSize;				 // size of a periodic bucket leak
	
 private:
  bool add( PitEntry &pe, const PitIncomingInterest &interest );
  void deleteIncomingInterest( PitEntry &pe, PitIncomingIterator it );

 private:
  Ccnx &_node;

  string_key_hash_t<PitEntry> _pit;

  list<PitExpirationEntry> _pitExpirationList;  //list of incoming Interests sorted by expiration time

  PitBucket				 _bucketsPerInterface;	// pending interface counter per interface
	
  QNThreadMutex			 _pitMutex; // just to make sure
};

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

PitIncomingIterator CcnxPit::findIncoming( PitEntry &pe, int interfaceIndex )
{
  return find( pe.incomingInterests.begin( ),
			   pe.incomingInterests.end( ),
			   interfaceIndex );
}

PitOutgoingIterator CcnxPit::findOutgoing( PitEntry &pe, int interfaceIndex )
{
  return find( pe.outgoingInterests.begin( ),
			   pe.outgoingInterests.end( ),
			   interfaceIndex );
}

bool CcnxPit::isValid( PitEntry &pe, PitIncomingIterator it )
{
  return it!=pe.incomingInterests.end( );
}

bool CcnxPit::isValid( PitEntry &pe, PitOutgoingIterator it )
{
  return it!=pe.outgoingInterests.end( );
}

int PitEntry::findBestCandidate( int skip/* = 0*/ )
{
  assert( availableInterfaces.size()>0 );

  skip = skip % availableInterfaces.size();
  list<int>::iterator candidate = availableInterfaces.begin( );
  while( skip>0 /* && candidate!=availableInterfaces.end() */ ) { skip--; candidate++; }

  return *candidate;
}

size_t PitEntry::numberOfPromisingInterests( ) const
{
  size_t count = 0;
	
  for( PitOutgoingConstIterator i = outgoingInterests.begin();
	   i!=outgoingInterests.end();
	   i++ )
	{
	  if( !i->waitingInVain ) count++;
	}

  return count;
}

} // namespace ns3

#endif	/* CCNX_PIT_H */
