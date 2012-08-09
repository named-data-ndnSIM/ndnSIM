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

#ifndef _NDN_FIB_ENTRY_H_
#define	_NDN_FIB_ENTRY_H_

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/ndn.h"
#include "ns3/ndn-face.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace ns3
{

class NdnNameComponents;

/**
 * \ingroup ndn
 * \brief Structure holding various parameters associated with a (FibEntry, Face) tuple
 */
class NdnFibFaceMetric
{
public:
  /**
   * @brief Color codes for FIB face status
   */
  enum Status { NDN_FIB_GREEN = 1,
                NDN_FIB_YELLOW = 2,
                NDN_FIB_RED = 3 };
public:
  /**
   * \brief Metric constructor
   *
   * \param face Face for which metric
   * \param cost Initial value for routing cost
   */
  NdnFibFaceMetric (Ptr<NdnFace> face, int32_t cost)
    : m_face (face)
    , m_status (NDN_FIB_YELLOW)
    , m_routingCost (cost)
    , m_sRtt   (Seconds (0))
    , m_rttVar (Seconds (0))
  { }

  /**
   * \brief Comparison operator used by boost::multi_index::identity<>
   */
  bool
  operator< (const NdnFibFaceMetric &fm) const { return *m_face < *fm.m_face; } // return identity of the face

  /**
   * @brief Comparison between NdnFibFaceMetric and NdnFace
   */
  bool
  operator< (const Ptr<NdnFace> &face) const { return *m_face < *face; } 

  /**
   * @brief Return NdnFace associated with NdnFibFaceMetric
   */
  Ptr<NdnFace>
  GetFace () const { return m_face; }

  /**
   * \brief Recalculate smoothed RTT and RTT variation
   * \param rttSample RTT sample
   */
  void
  UpdateRtt (const Time &rttSample);
  
private:
  friend std::ostream& operator<< (std::ostream& os, const NdnFibFaceMetric &metric);
public:
  Ptr<NdnFace> m_face; ///< Face
  
  Status m_status;		///< \brief Status of the next hop: 
				///<		- NDN_FIB_GREEN
				///<		- NDN_FIB_YELLOW
				///<		- NDN_FIB_RED
  
  int32_t m_routingCost; ///< \brief routing protocol cost (interpretation of the value depends on the underlying routing protocol)

  Time m_sRtt;         ///< \brief smoothed round-trip time
  Time m_rttVar;       ///< \brief round-trip time variation
};

/**
 * \ingroup ndn
 * \brief Typedef for indexed face container of NdnFibEntry
 *
 * Currently, there are 2 indexes:
 * - by face (used to find record and update metric)
 * - by metric (face ranking)
 * - random access index (for fast lookup on nth face). Order is
 *   maintained manually to be equal to the 'by metric' order
 */
struct NdnFibFaceMetricContainer
{
  /// @cond include_hidden
  typedef boost::multi_index::multi_index_container<
    NdnFibFaceMetric,
    boost::multi_index::indexed_by<
      // For fast access to elements using NdnFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ndn_private::i_face>,
        boost::multi_index::member<NdnFibFaceMetric,Ptr<NdnFace>,&NdnFibFaceMetric::m_face>
      >,

      // List of available faces ordered by (status, m_routingCost)
      boost::multi_index::ordered_non_unique<
        boost::multi_index::tag<__ndn_private::i_metric>,
        boost::multi_index::composite_key<
          NdnFibFaceMetric,
          boost::multi_index::member<NdnFibFaceMetric,NdnFibFaceMetric::Status,&NdnFibFaceMetric::m_status>,
          boost::multi_index::member<NdnFibFaceMetric,int32_t,&NdnFibFaceMetric::m_routingCost>
        >
      >,

      // To optimize nth candidate selection (sacrifice a little bit space to gain speed)
      boost::multi_index::random_access<
        boost::multi_index::tag<__ndn_private::i_nth>
      >
    >
   > type;
  /// @endcond
};

/**
 * \ingroup ndn
 * \brief Structure for FIB table entry, holding indexed list of
 *        available faces and their respective metrics
 */
class NdnFibEntry : public SimpleRefCount<NdnFibEntry>
{
public:
  class NoFaces {}; ///< @brief Exception class for the case when FIB entry is not found
  
  /**
   * \brief Constructor
   * \param prefix smart pointer to the prefix for the FIB entry
   */
  NdnFibEntry (const Ptr<const NdnNameComponents> &prefix)
  : m_prefix (prefix)
  , m_needsProbing (false)
  { }
  
  /**
   * \brief Update status of FIB next hop
   * \param status Status to set on the FIB entry
   */
  void UpdateStatus (Ptr<NdnFace> face, NdnFibFaceMetric::Status status);

  /**
   * \brief Add or update routing metric of FIB next hop
   *
   * Initial status of the next hop is set to YELLOW
   */
  void AddOrUpdateRoutingMetric (Ptr<NdnFace> face, int32_t metric);

  /**
   * @brief Invalidate face
   *
   * Set routing metric on all faces to max and status to RED
   */
  void
  Invalidate ();

  /**
   * @brief Update RTT averages for the face
   */
  void
  UpdateFaceRtt (Ptr<NdnFace> face, const Time &sample);
  
  /**
   * \brief Get prefix for the FIB entry
   */
  const NdnNameComponents&
  GetPrefix () const { return *m_prefix; }

  /**
   * \brief Find "best route" candidate, skipping `skip' first candidates (modulo # of faces)
   *
   * throws NdnFibEntry::NoFaces if m_faces.size()==0
   */
  const NdnFibFaceMetric &
  FindBestCandidate (uint32_t skip = 0) const;

  /**
   * @brief Remove record associated with `face`
   */
  void
  RemoveFace (const Ptr<NdnFace> &face)
  {
    m_faces.erase (face);
  }
	
private:
  friend std::ostream& operator<< (std::ostream& os, const NdnFibEntry &entry);

public:
  Ptr<const NdnNameComponents> m_prefix; ///< \brief Prefix of the FIB entry
  NdnFibFaceMetricContainer::type m_faces; ///< \brief Indexed list of faces

  bool m_needsProbing;      ///< \brief flag indicating that probing should be performed 
};

std::ostream& operator<< (std::ostream& os, const NdnFibEntry &entry);
std::ostream& operator<< (std::ostream& os, const NdnFibFaceMetric &metric);

} // ns3

#endif // _NDN_FIB_ENTRY_H_
