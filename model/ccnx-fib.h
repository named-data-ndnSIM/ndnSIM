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

#ifndef _CCNX_FIB_H_
#define	_CCNX_FIB_H_

#include "hash-helper.h"
#include "ccnx-face.h"
#include "ns3/nstime.h"
#include "ns3/simple-ref-count.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <iostream>

namespace ns3 {

namespace __ccnx_private_fib
{
const uint8_t NDN_FIB_GREEN = 1; ///< \brief 
const uint8_t NDN_FIB_YELLOW = 2;
const uint8_t NDN_FIB_RED = 3;

class i_face {};
class i_metric {};
class i_nth {};
class i_prefix {};
}

class Ccnx;
class CcnxInterestHeader;

/**
 * \ingroup ccnx
 * \brief Structure holding various parameters associated with a (FibEntry, Face) tuple
 */
class CcnxFibFaceMetric
{
public:
  /**
   * \brief Metric constructor
   *
   * \param face Face for which metric
   * \param cost Initial value for routing cost
   */
  CcnxFibFaceMetric (Ptr<CcnxFace> face, int cost)
    : m_face (face)
    , m_status (__ccnx_private_fib::NDN_FIB_YELLOW)
    , m_routingCost (cost)
    , m_sRtt   (Seconds (0))
    , m_rttVar (Seconds (0))
  { }

  /**
   * \brief Comparison operator used by boost::multi_index::identity<>
   */
  bool
  operator< (const CcnxFibFaceMetric &m) const { return *m_face < *(m.m_face); } // return identity of the face

  Ptr<CcnxFace>
  GetFace () const { return m_face; }
  
private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxFibFaceMetric &metric);
public:
  Ptr<CcnxFace> m_face; ///< Face
  
  uint8_t m_status;		///< \brief Status of the next hop: 
						///<		- #__ccnx_private_fib::NDN_FIB_GREEN
						///<		- #__ccnx_private_fib::NDN_FIB_YELLOW
						///<		- #__ccnx_private_fib::NDN_FIB_RED
  
  uint32_t m_routingCost; ///< \brief routing protocol cost (interpretation of the value depends on the underlying routing protocol)

  Time m_sRtt;         ///< \brief smoothed round-trip time
  Time m_rttVar;       ///< \brief round-trip time variation
};

/**
 * \ingroup ccnx
 * \brief Typedef for indexed face container of CcnxFibEntry
 *
 * Currently, there are 2 indexes:
 * - by face (used to find record and update metric)
 * - by metric (face ranking)
 * - random access index (for fast lookup on nth face). Order is
 *   maintained manually to be equal to the 'by metric' order
 */
struct CcnxFibFaceMetricContainer
{
  typedef boost::multi_index::multi_index_container<
    CcnxFibFaceMetric,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private_fib::i_face>,
        boost::multi_index::identity<CcnxFibFaceMetric>
      >,

      // List of available faces ordered by (status, m_routingCost)
      boost::multi_index::ordered_non_unique<
        boost::multi_index::tag<__ccnx_private_fib::i_metric>,
        boost::multi_index::composite_key<
          CcnxFibFaceMetric,
          boost::multi_index::member<CcnxFibFaceMetric,uint8_t,&CcnxFibFaceMetric::m_status>,
          boost::multi_index::member<CcnxFibFaceMetric,uint32_t,&CcnxFibFaceMetric::m_routingCost>
        >
      >,

      // To optimize nth candidate selection (sacrifice a little bit space to gain speed)
      boost::multi_index::random_access<
        boost::multi_index::tag<__ccnx_private_fib::i_nth>
      >
    >
   > type;
};

/**
 * \ingroup ccnx
 * \brief Structure for FIB table entry, holding indexed list of
 *        available faces and their respective metrics
 */
class CcnxFibEntry : public SimpleRefCount<CcnxFibEntry>
{
public:
  /**
   * \brief Constructor
   * \param prefix Prefix for the FIB entry
   */
  CcnxFibEntry (Ptr<CcnxNameComponents> prefix)
    : m_prefix (prefix)
    , m_needsProbing (false)
  { }
	
  /**
   * \brief Update status of FIB next hop
   */
  void
  UpdateStatus (Ptr<CcnxFace> face, uint8_t status);

  /**
   * \brief Get prefix for the FIB entry
   */
  const CcnxNameComponents&
  GetPrefix () const { return *m_prefix; }

  /**
   * \brief Find "best route" candidate, skipping `skip' first candidates (modulo # of faces)
   */
  Ptr<CcnxFace>
  FindBestCandidate (int skip = 0);
	
private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxFibEntry &entry);

private:
  Ptr<CcnxNameComponents> m_prefix; ///< \brief Prefix of the FIB entry
  CcnxFibFaceMetricContainer::type m_faces; ///< \brief Indexed list of faces

  bool m_needsProbing;      ///< \brief flag indicating that probing should be performed 
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * \ingroup ccnx
 * \brief Typedef for indexed container for FIB entries
 *
 * Currently, there is only one index
 * - by prefix hash, which is used to perform prefix match
 */
struct CcnxFibEntryContainer
{
  typedef boost::multi_index::multi_index_container<
    CcnxFibEntry,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::hashed_unique<
        boost::multi_index::tag<__ccnx_private_fib::i_prefix>,
        boost::multi_index::const_mem_fun<CcnxFibEntry,
                                          const CcnxNameComponents&,
                                          &CcnxFibEntry::GetPrefix>,
        CcnxPrefixHash>

      // other indexes?
    >
  > type;
};

/**
 * \ingroup ccnx
 * \brief Class implementing FIB functionality
 */
class CcnxFib
{
public:
  /**
   * \brief Constructor
   * \param node smart pointer to Ccnx stack associated with particular node
   */
  CcnxFib (Ptr<Ccnx> node);

  // // Invalidate entries in FIB
  // // Will leave FIB records in hash, but assign metric=NETWORK_UNREACHABLE
  // void invalidate( );

  // //Find corresponding FIB entry for the given content name
  // //Longest match is performed
  // FibIterator lookup( const string &name );
  // bool isValid( const FibIterator &it ) { return it!=_fib.end(); }

  /**
   * \brief Perform longest prefix match
   *
   * \todo Implement exclude filters
   *
   * \param interest Interest packet header
   * \returns If entry found a pair <valid_iterator, true> will be returned, otherwise <invalid_iterator, false>
   */
  std::pair<CcnxFibEntryContainer::type::iterator, bool>
  LongestPrefixMatch (const CcnxInterestHeader &interest) const;
  
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
  // bool update( const string &name, int interfaceIndex, int metric, NodeAddress nextHop );
  // bool update( NodeAddress nodeId, int interfaceIndex, int metric, NodeAddress nextHop );
  // Bool update( NodeAddress nodeId, int metric, NodeAddress nextHop );

  // // Update Fib from OSPF routing table (through a hack in OSPF algorithm)
  // void updateFibFromOSPFv2( int interface );

  // // Update Fib from BGP routing table (using info from RibIn)
  // void updateFibFromBGP( );

  // // Update Fib from IP routing table
  // void updateFibFromIpRouting( );

  // // Update the status for all FIB records for the specified interface
  // void updateInterfaceStatus( int interface, int status );

  // void dump( );
  // void dump( const FibIterator &fib );

  // void resetProbing();    //reset needsProbing field for every FibEntry
private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxFib &fib);

private:
  Ptr<Ccnx> m_node;

  CcnxFibEntryContainer::type m_fib;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<< (std::ostream& os, const CcnxFib &fib);
std::ostream& operator<< (std::ostream& os, const CcnxFibEntry &entry);
std::ostream& operator<< (std::ostream& os, const CcnxFibFaceMetric &metric);
 
} // namespace ns3

#endif	/* NDN_FIB_H */
