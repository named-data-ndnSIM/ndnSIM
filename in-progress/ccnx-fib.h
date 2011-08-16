/* 
 * File:   ndn_fib.h
 * Author: cawka
 *
 * Created on December 15, 2010, 1:54 PM
 */

#ifndef NDN_FIB_H
#define	NDN_FIB_H

#include "hash_helper.h"
#include <clock.h>
#include <main.h>

class Ndn;

const int NDN_FIB_GREEN = 1;
const int NDN_FIB_YELLOW = 2;
const int NDN_FIB_RED = 3;

//structure for Fib outgoing interface
struct FibNexthop
{
    int interfaceIndex;     //interface index of the node
	NodeAddress nextHop;	//next-hop
	int cost;				//routing protocol cost to route an interest via this interface
    int packetCount;        //record the number of packets forwarded using this interface

	clocktype srtt;         //smoothed round-trip time
    clocktype rttvar;       //round-trip time variation
	
	int status;				// Status of the next hop: 
							//		- #NDN_FIB_GREEN
							//		- #NDN_FIB_YELLOW
							//		- #NDN_FIB_RED

	bool operator==( int interface ) const { return interfaceIndex==interface; }
	FibNexthop( ) {}
	FibNexthop( int _interface, int _nextHop, int _cost )
			: interfaceIndex(_interface), nextHop(_nextHop), cost(_cost)
			, packetCount(1), srtt(0), rttvar(0), status(NDN_FIB_YELLOW) { }
};

typedef list<FibNexthop>::iterator FibNexthopIterator;
typedef list<FibNexthop>::const_iterator FibNexthopConstIterator;

//structure for FIB table entry
struct FibEntry
{
    list<FibNexthop> forwardingList;
    clocktype sto;	  //retransmission time out

	bool needsProbing;      //set to true when probing timer goes out

	FibEntry( ) : sto(0), needsProbing(false) { }

	// Find nexthop record
	inline FibNexthopIterator findNexthop( int interfaceIndex );
	bool isValid( const FibNexthopIterator &nh )
		{ return nh!=forwardingList.end(); }

	// Compute and update RTO value for Fib Entry (RFC 2988)
	// (for now we pick the maximum RTO of all forwardings)
	void updateSto( );
	
	// Update status of FIB next hop
	inline void updateStatus( int interface, int status );
};

typedef string_key_hash_t<FibEntry>::point_iterator FibIterator;
typedef string_key_hash_t<FibEntry>::iterator FibRangeIterator;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Class implementing FIB functionality
class NdnFib
{
public:
	NdnFib( Ndn &node );
	virtual ~NdnFib( );

	// Invalidate entries in FIB
	// Will leave FIB records in hash, but assign metric=NETWORK_UNREACHABLE
	void invalidate( );

	//Find corresponding FIB entry for the given content name
	//Longest match is performed
	FibIterator lookup( const string &name );
	bool isValid( const FibIterator &it ) { return it!=_fib.end(); }

	/**
	 * Update FIB entry
	 * If the entry exists, metric will be updated. Otherwise, new entry will be created
	 *
	 * Entries in FIB never deleted. They can be invalidated with metric==NETWORK_UNREACHABLE
	 *
	 * @param name				Prefix
	 * @param interfaceIndex	Forwarding interface
	 * @param metric			Routing metric
	 * @param nextHop			Nexthop node address (IPv4)
	 * @return true if a new entry created, false otherwise
	 */
	bool update( const string &name, int interfaceIndex, int metric, NodeAddress nextHop );
	bool update( NodeAddress nodeId, int interfaceIndex, int metric, NodeAddress nextHop );
	bool update( NodeAddress nodeId, int metric, NodeAddress nextHop );

	// Update Fib from OSPF routing table (through a hack in OSPF algorithm)
	void updateFibFromOSPFv2( int interface );

	// Update Fib from BGP routing table (using info from RibIn)
	void updateFibFromBGP( );

	// Update Fib from IP routing table
	void updateFibFromIpRouting( );

	// Update the status for all FIB records for the specified interface
	void updateInterfaceStatus( int interface, int status );
	
	void dump( );
	void dump( const FibIterator &fib );

    void resetProbing();    //reset needsProbing field for every FibEntry
private:

private:
	Ndn &_node;

	string_key_hash_t<FibEntry> _fib;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class NdnFibNexthopSorter
{
public:
	bool operator()( const FibNexthop &first, const FibNexthop &second );
};

FibNexthopIterator FibEntry::findNexthop( int interfaceIndex )
{
	return find( forwardingList.begin( ),
				 forwardingList.end( ),
				 interfaceIndex );
}

//inline FibNexthopIterator FibEntry::findBestNexthop( int bestNum, int excludeInterface )
//{
//	// First adjust number of available interfaces (to make sure we have correct ranking function)
//	int num_valid_interfaces = forwardingList.size();
//	FibNexthopIterator nh;
//	for( nh=forwardingList.begin(); nh!=forwardingList.end(); nh++ )
//	{
//		if( nh->interfaceIndex==excludeInterface ) num_valid_interfaces--;
//	}
//
//	if( num_valid_interfaces==0 ) return forwardingList.end();
//
//	bestNum = bestNum % num_valid_interfaces;
//	int i=0;
//	for( nh=forwardingList.begin(); nh!=forwardingList.end(); nh++ ) // skip bestNum % size() FIB records
//	{
//		if( nh->interfaceIndex==excludeInterface ) continue;
//		if( i==bestNum ) break;
//
//		i++;
//	}
//
//	if( nh!=forwardingList.end() )
//	{
//		assert( nh->interfaceIndex!=excludeInterface );
//// 		printf( "%d best => i%d\n", bestNum, nh->interfaceIndex );
//	}
//// 	else
//// 	{
//// 		printf( "No other hops available\n" );
//// 	}
//
//
//	return nh;
//}

void FibEntry::updateStatus( int interface, int status )
{
	FibNexthopIterator nh = findNexthop( interface );
	if( isValid(nh) )
	{
		nh->status = status;
	}
}

#endif	/* NDN_FIB_H */
