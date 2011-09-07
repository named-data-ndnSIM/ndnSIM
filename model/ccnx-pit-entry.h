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

#ifndef _CCNX_PIT_ENTRY_H_
#define _CCNX_PIT_ENTRY_H_

#include "ns3/ptr.h"

#include "ccnx-pit-entry-incoming-face.h"
#include "ccnx-pit-entry-outgoing-face.h"
#include "ccnx-fib.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <iostream>

namespace ns3 {

class CcnxFace;
class CcnxNameComponents;

namespace __ccnx_private
{
// class i_face {};
}

/**
 * \ingroup ccnx
 * \brief Typedef for indexed face container of CcnxPitEntryIncomingFace
 *
 * Indexes:
 * - by face (may be it will be possible to replace with just the std::map)
 */
struct CcnxPitEntryIncomingFaceContainer
{
  typedef boost::multi_index::multi_index_container<
    CcnxPitEntryIncomingFace,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private::i_face>,
        boost::multi_index::member<CcnxPitEntryIncomingFace, Ptr<CcnxFace>, &CcnxPitEntryIncomingFace::m_face>
      >
    >
   > type;
};

/**
 * \ingroup ccnx
 * \brief Typedef for indexed face container of CcnxPitEntryOutgoingFace
 *
 * Indexes:
 * - by face (may be it will be possible to replace with just the std::map)
 */
struct CcnxPitEntryOutgoingFaceContainer
{
  typedef boost::multi_index::multi_index_container<
    CcnxPitEntryOutgoingFace,
    boost::multi_index::indexed_by<
      // For fast access to elements using CcnxFace
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<__ccnx_private::i_face>,
        boost::multi_index::member<CcnxPitEntryOutgoingFace, Ptr<CcnxFace>, &CcnxPitEntryOutgoingFace::m_face>
      >
    >
   > type;
};


/**
 * \ingroup ccnx
 * \brief structure for PIT entry
 */
struct CcnxPitEntry
{
public:
  /**
   * \brief PIT entry constructor
   * \param prefix Prefix of the PIT entry
   * \param fibEntry A FIB entry associated with the PIT entry
   */
  CcnxPitEntry (Ptr<CcnxNameComponents> prefix, const CcnxFibEntry &fibEntry);
  
  // // Get number of outgoing interests that we're expecting data from
  // inline size_t numberOfPromisingInterests( ) const; 

  // /**
  //  * \brief Unary function to set or update FIB entry with this PIT entry
  //  * \param fib smart pointer to FIB entry
  //  */
  // struct SetFibEntry
  // {
  //   SetFibEntry (Ptr<CcnxFibEntry> fib);
  //   void operator() (CcnxPitEntry &entry);
  // private:
  //   Ptr<CcnxFibEntry> m_fib;
  // };
  
  /**
   * \brief Unary Function to add incoming interest to the PIT entry
   *
   * \param incomingFace smart pointer to the face of the incoming interest
   * \returns const iterator to a newly added or updated
   * CcnxPitIncomingInterest entry
   */
  struct AddIncoming
  {
    AddIncoming (Ptr<CcnxFace> incomingFace) : m_face (incomingFace) {}
    void operator() (CcnxPitEntry &entry);
    
  private:
    Ptr<CcnxFace> m_face;
    Time m_lifeTime;
  };

  /**
   * \brief Unary function to delete incoming interest for the interface
   * \param face face that should be removed from the list of incoming interests
   */
  struct DeleteIncoming
  {
    DeleteIncoming (Ptr<CcnxFace> face) : m_face (face) {}
    void operator() (CcnxPitEntry &entry);
  private:
    Ptr<CcnxFace> m_face;
  };

  /**
   * \brief Unary function to add outgoing interest to PIT entry
   *
   * \param outgoingFace smart pointer to the face of the outgoing interest
   * \returns const iterator to a newly added or updated
   * CcnxPitOutgoingInterest entry
   */
  struct AddOutgoing
  {
    AddOutgoing (Ptr<CcnxFace> outgoingFace) : m_face (outgoingFace) {}
    void operator() (CcnxPitEntry &entry);
  private:
    Ptr<CcnxFace> m_face;
  };

  /**
   * \brief Unary function to delete incoming interest for the interface
   * \param face face that should be removed from the list of incoming interests
   */
  struct DeleteOutgoing
  {
    DeleteOutgoing (Ptr<CcnxFace> face) : m_face (face) {}
    void operator() (CcnxPitEntry &entry);
  private:
    Ptr<CcnxFace> m_face;
  };

  /**
   * \brief Unary function to remove all incoming interests
   */
  struct ClearIncoming
  {
    ClearIncoming () {};
    void operator() (CcnxPitEntry &entry);
  };

  /**
   * \brief Unary function to update FIB status
   */
  struct UpdateFibStatus
  {
    UpdateFibStatus (Ptr<CcnxFace> face, CcnxFibFaceMetric::Status status);
    void operator() (CcnxPitEntry &entry);
  private:
    Ptr<CcnxFace> m_face;
    CcnxFibFaceMetric::Status m_status;
  };

  /**
   * \brief Unary function to estimate RTT and update smoothed RTT value in FIB
   * \param outFace iterator of the outgoing face entry
   */
  struct EstimateRttAndRemoveFace
  {
    EstimateRttAndRemoveFace (CcnxPitEntryOutgoingFaceContainer::type::iterator outFace, Ptr<CcnxFib> fib)
      : m_outFace (outFace), m_fib (fib) { };
    void operator() (CcnxPitEntry &entry);
  private:
    CcnxPitEntryOutgoingFaceContainer::type::iterator m_outFace;
    Ptr<CcnxFib> m_fib;
  };

  const CcnxNameComponents &
  GetPrefix () const;

  const Time &
  GetExpireTime () const { return m_expireTime; }

private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxPitEntry &entry);
  
public:
  Ptr<CcnxNameComponents> m_prefix; ///< \brief Prefix of the PIT entry
  const CcnxFibEntry &m_fibEntry; ///< \brief FIB entry related to this prefix
  
  CcnxPitEntryIncomingFaceContainer::type m_incoming; ///< \brief container for incoming interests
  CcnxPitEntryOutgoingFaceContainer::type m_outgoing; ///< \brief container for outgoing interests

  Time m_expireTime;         ///< \brief Time when PIT entry will be removed
  bool m_timerExpired;       ///< \brief flag indicating that PIT timer has expired
  int  m_counterExpirations; ///< \brief whether timer is expired (+ number of times timer expired)
};


} // namespace ns3

#endif // _CCNX_PIT_ENTRY_H_
