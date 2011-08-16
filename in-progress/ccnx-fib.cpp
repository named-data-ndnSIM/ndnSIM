/* 
 * File:   ndn_fib.cpp
 * Author: cawka
 * 
 * Created on December 15, 2010, 1:54 PM
 */

#include "ndn.h"
#include "ndn_fib.h"

#include <node.h>
#include <buffer.h>
#include <network_ip.h>
#include <partition.h>
#include <routing_ospfv2.h>
#include <routing_bgp.h>

//#define NDN_DEBUG_OSPF	0
//#define NDN_DEBUG_OSPF_NODES 0

//#define NDN_DUMP_FIB		0


NdnFib::NdnFib( Ndn &node ) : _node(node) { }

NdnFib::~NdnFib( ) { }


//Find corresponding FIB entry for the given content name
//Longest match is performed
FibIterator NdnFib::lookup( const string &name )
{
	string prefix=name;

	FibIterator entry;
	do
	{
		entry=_fib.find( prefix );

		prefix = GetLongestNamePrefix( prefix );
	} while( !isValid(entry) && prefix.size()>0 );

	return entry;
}

bool NdnFibNexthopSorter::operator()( const FibNexthop &first, const FibNexthop &second )
{
	// Right now is a very simple logic.
	// Probably this logic should be changed later
	if( first. cost==NETWORK_UNREACHABLE && second.cost>=0 ) return false;
	if( second.cost==NETWORK_UNREACHABLE && first. cost>=0 ) return true;
	return  first.cost < second.cost;
}

/**
 * Update FIB entry
 * If the entry exists, metric will be updated. Otherwise, new entry will be created
 * 
 * @param name				Prefix
 * @param interfaceIndex	Forwarding interface
 * @param metric			Routing metric
 * @param nextHop			Nexthop node address (IPv4)
 * @return true if a new entry created, false otherwise
 */
bool NdnFib::update( const string &name, int interfaceIndex, int metric, NodeAddress nextHop )
{
	FibIterator entry = _fib.find( name );
	if( isValid(entry) )
	{
		FibNexthopIterator nh = VALUE(entry).findNexthop( interfaceIndex );

		if( !VALUE(entry).isValid(nh) )
		{
			nh = VALUE(entry).forwardingList.insert( VALUE(entry).forwardingList.begin(),
											 FibNexthop(interfaceIndex,nextHop,metric) );
		}
		else
		{
			nh->cost = metric;
			nh->nextHop = nextHop;
		}

		VALUE(entry).forwardingList.sort( NdnFibNexthopSorter() );

		return false;
	}

	FibEntry &new_entry = _fib[name];
	new_entry.forwardingList.push_back( FibNexthop(interfaceIndex,nextHop,metric) );

	for( int interface=0; interface < _node.getNode()->numberInterfaces; interface++ )
	{
		NodeAddress src = NetworkIpGetInterfaceAddress( _node.getNode(), interface );

		if( isValidLink(src) && interface!=interfaceIndex )
		{
			new_entry.forwardingList.push_back( FibNexthop(interface,0,NETWORK_UNREACHABLE) );
		}
	}

	return true;
}

// helper for update call
bool NdnFib::update( NodeAddress nodeId, int metric, NodeAddress nextHop )
{
	ostringstream os;
	os << (int)nodeId;

	int interface = NetworkIpGetInterfaceIndexForNextHop( _node.getNode(), nextHop );

	return update( os.str(), interface, metric, nextHop );
}

// helper for update call
bool NdnFib::update( NodeAddress nodeId, int interfaceIndex, int metric, NodeAddress nextHop )
{
	ostringstream os;
	os << (int)nodeId;

	int interface = NetworkIpGetInterfaceIndexForNextHop( _node.getNode(), nextHop );

	return update( os.str(), interface, metric, nextHop ); //unknown metric
}

/**
 * Invalidate entries in FIB
 */
void NdnFib::invalidate( )
{
	for( FibRangeIterator fib=_fib.begin(); fib!=_fib.end(); fib++ )
	{
		for( FibNexthopIterator nh=VALUE(fib).forwardingList.begin(); nh!=VALUE(fib).forwardingList.end(); nh++ )
		{
			nh->cost = NETWORK_UNREACHABLE;
		}
	}
}

//compute and update STO value for Fib Entry (similar to RFC 2988)
//for now we pick the maximum rto of all forwardings
void FibEntry::updateSto( )
{
    assert( forwardingList.size() > 0 );

    clocktype max = 0;

    for( FibNexthopIterator it = forwardingList.begin();
		 it != forwardingList.end();
		 it++ )
    {
        clocktype sto = it->srtt + NDN_RTO_K * it->rttvar;

        if( sto > max )
			max = sto;
    }

    this->sto = max;
}


/**
 * Updating FIB using data from OSPF
 * @param node
 * @param interface	-2	Invalid OSPF (to purge FIB)
 *					-1	Normal OSPF
 *					0-N	SPF was calcylated using only interface `interface'
 *
 * @bug	All local networks will always appear in routing table with cost == 1
 */
void NdnFib::updateFibFromOSPFv2( int interface )
{
    Ospfv2Data* ospf = (Ospfv2Data*)NetworkIpGetRoutingProtocol(_node.getNode(), ROUTING_PROTOCOL_OSPFv2);

	if( interface==-2 ) invalidate( );

#ifdef NDN_DEBUG_OSPF
	if( interface==-1 ) printf( "Routing/interface costs\n" );
#endif // NDN_DEBUG_OSPF

#ifdef NDN_DEBUG_OSPF_NODES
	printf( "-- Node %d (interface %d) --\n", _node.getNode()->nodeId, interface );
#endif // NDN_DEBUG_OSPF_NODES

    Ospfv2RoutingTableRow* rowPtr = (Ospfv2RoutingTableRow*)
            BUFFER_GetData(&ospf->routingTable.buffer);

    for (int i = 0; i < ospf->routingTable.numRows; i++)
    {
		NodeAddress destNodeId = Ipv4ToNodeId( rowPtr[i].destAddr );

		if( destNodeId!=-1 ) update( destNodeId, rowPtr[i].metric, rowPtr[i].nextHop );
    }

#ifdef NDN_DUMP_FIB
	if( interface==-1 ) dump( );
#endif
}

void NdnFib::updateFibFromBGP( )
{
	BgpData* bgp=(BgpData*)_node.getNode()->appData.exteriorGatewayVar;

	assert( bgp->ip_version == NETWORK_IPV4 );

	invalidate( );

	int i=0;
	int numEntriesInAdjRibIn=BUFFER_GetCurrentSize( &( bgp->adjRibIn ) )
			/ sizeof(BgpRoutingInformationBase );

	BgpRoutingInformationBase* adjRibInPtr=(BgpRoutingInformationBase*)
			BUFFER_GetData( &( bgp->adjRibIn ) );

	for( i=0; i < numEntriesInAdjRibIn; i++ )
	{
		assert( adjRibInPtr[i].nextHop.networkType == NETWORK_IPV4 );

		NodeAddress destNodeId = Ipv4ToNodeId( GetIPv4Address(adjRibInPtr[i].destAddress.prefix) );

		if( destNodeId!=-1 && adjRibInPtr[i].isValid != FALSE )
		{
			char destNodeStr[NDN_MAX_NAME_LENGTH];
			memset(destNodeStr, 0, NDN_MAX_NAME_LENGTH);
			sprintf(destNodeStr, "%d", destNodeId);

			update( destNodeId,
					adjRibInPtr[i].asPathList->pathSegmentLength / 2,
					GetIPv4Address(adjRibInPtr[i].nextHop) );
		}
	}

#ifdef NDN_DUMP_FIB
	dump( );
#endif
}

void NdnFib::updateFibFromIpRouting( )
{
	invalidate( );

	for (int i = 0; i < _node.getNode()->partitionData->numNodes; i ++)
    {
		if( !_node.getNode()->networkData.networkVar->ndnEnabled ) continue;

        NodeAddress destNode = _node.getNode()->partitionData->nodeData[i]->nodeId;
		NodeAddress ipv4subnet = NodeIdToIpv4( destNode );

		int interfaceIndex;
		NodeAddress nextHopAddr;
        NetworkGetInterfaceAndNextHopFromForwardingTable(
			_node.getNode(), ipv4subnet, &interfaceIndex, &nextHopAddr );

        if( interfaceIndex != NETWORK_UNREACHABLE )
        {
			update( destNode, interfaceIndex, 1, nextHopAddr );
        }
    }
}



void NdnFib::dump( )
{
	if( _node.getNode()->numberInterfaces==1 ) return;  // do not dump FIB for `virtual' nodes

	printf( "Node %s: FIB\n", _node.getPrefix().c_str() );
	printf( "  Dest prefix      Interfaces(Costs)                  \n" );
	printf( "+-------------+--------------------------------------+\n" );

	for( FibRangeIterator fib=_fib.begin(); fib!=_fib.end(); fib++ )
	{
		dump( fib );
	}
}

void NdnFib::dump( const FibIterator &fib )
{
	printf( " %8s ", fib->first.c_str() );
	for( FibNexthopIterator nh=VALUE(fib).forwardingList.begin(); nh!=VALUE(fib).forwardingList.end(); nh++ )
	{
		if( nh!=VALUE(fib).forwardingList.begin() ) printf( "," );
		printf( "i%d(%d)", nh->interfaceIndex, nh->cost );
	}
	printf( "\n" );
}

void NdnFib::resetProbing()
{
    for(FibRangeIterator fib = _fib.begin(); fib != _fib.end(); fib++)
        VALUE(fib).needsProbing = true;
}

void NdnFib::updateInterfaceStatus( int interface, int status )
{
	for( FibRangeIterator fib = _fib.begin(); fib!=_fib.end(); fib++ )
	{
		VALUE(fib).updateStatus( interface, status );
	}
}