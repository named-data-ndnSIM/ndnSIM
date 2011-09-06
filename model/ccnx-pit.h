/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _CCNX_PIT_H_
#define	_CCNX_PIT_H_

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include "hash-helper.h"
#include "ccnx-pit-entry.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <iostream>

namespace ns3 {

class Ccnx;
class CcnxFace;
class CcnxContentObjectHeader;
class CcnxInterestHeader;

/**
 * \ingroup ccnx
 * \private
 * \brief Private namespace for CCNx PIT implementation
 */
namespace __ccnx_private
{
// class i_prefix{}; ///< tag for prefix hash
class i_timestamp {}; ///< tag for timestamp-ordered records (for cleanup optimization)  
};

/**
 * \ingroup ccnx
 * \brief Typedef for RIT container implemented as a Boost.MultiIndex container
 *
 * - First index (tag<i_prefix>) is a unique hash index based on
 *   prefixes
 * - Second index (tag<i_timestamp>) is a sequenced index based on
 *   arrival order (for clean-up optimizations)
 *
 * \see http://www.boost.org/doc/libs/1_46_1/libs/multi_index/doc/ for more information on Boost.MultiIndex library
 */
struct CcnxPitEntryContainer
{
  typedef
  boost::multi_index::multi_index_container<
    CcnxPitEntry,
    boost::multi_index::indexed_by<
      // indexed by hash
      boost::multi_index::hashed_unique<
        boost::multi_index::tag<__ccnx_private::i_prefix>,
        boost::multi_index::const_mem_fun<CcnxPitEntry, const CcnxNameComponents&, &CcnxPitEntry::GetPrefix>,
        CcnxPrefixHash
        >,
      // sequenced to implement MRU
      boost::multi_index::sequenced<
        boost::multi_index::tag<__ccnx_private::i_timestamp> >
      >
    > type;
};

// typedef std::map<int,int> PitCounter;
// typedef std::map<int,int>::iterator PitCounterIterator;

// typedef std::map<int,double> PitBucket;
// typedef std::map<int,double>::iterator PitBucketIterator;


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/**
 * \ingroup ccnx
 * \brief Class implementing Pending Interests Table
 */
class CcnxPit : public CcnxPitEntryContainer::type, public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief PIT constructor
   */
  CcnxPit ();

  /**
   * \brief Find corresponding PIT entry for the given content name
   * \param prefix Prefix for which to lookup the entry
   * \returns const reference to Pit entry. If record not found,
   *          CcnxPitEntryNotFound exception will be thrown
   */
  const CcnxPitEntry&
  Lookup (const CcnxContentObjectHeader &header) const;

  /**
   * \brief Find corresponding PIT entry for the given content name
   * \param prefix Prefix for which to lookup the entry
   * \returns const reference to Pit entry. If record does not exist, it will be created
   */
  const CcnxPitEntry&
  Lookup (const CcnxInterestHeader &header);
  
  // remove a PIT entry
  //void erase (const string &contentName);
	
  // Reset pending state in outgoing interests
  // void resetPendingState( PitEntry &pe );

  //	// Check if there are any interfaces that we haven't sent data to yet
  //	bool areFreeInterfaces( PitEntry &pe, int interface );

  // // Periodically generate pre-calculated number of tokens (leak buckets)
  // void LeakBuckets( );
	
  // // Selectively leak a bucket
  // void LeakBucket (Ptr<CcnxFace> face, int amount);
	
  /**
   * \brief Set cleanup timeout
   *
   * Side effect: current clean up even (if any) will be cancelled and a new event started
   *
   * \param timeout cleanup timeout
   */
  void SetCleanupTimeout (const Time &timeout);

  /**
   * \brief Get cleanup timeout
   *
   * \returns cleanup timeout
   */
  Time GetCleanupTimeout () const;

public:
  // PitBucket				 maxBucketsPerInterface; // maximum number of buckets. Automatically computed based on link capacity
  // // averaging over 1 second (bandwidth * 1second)
  // PitBucket				 leakSize;				 // size of a periodic bucket leak
	
private:
  /** \brief Remove expired records from PIT */
  void CleanExpired ();

  friend std::ostream& operator<< (std::ostream& os, const CcnxPit &fib);
  
private:
  Time    m_cleanupTimeout; ///< \brief Configurable timeout of how often cleanup events are working
  EventId m_cleanupEvent;   ///< \brief Cleanup event

  // PitBucket	m_bucketsPerInterface;	///< \brief pending interface counter per interface
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<< (std::ostream& os, const CcnxPit &fib);
std::ostream& operator<< (std::ostream& os, const CcnxPitEntry &entry);
// std::ostream& operator<< (std::ostream& os, const CcnxFibFaceMetric &metric);

class CcnxPitEntryNotFound {};

} // namespace ns3

#endif	/* CCNX_PIT_H */
