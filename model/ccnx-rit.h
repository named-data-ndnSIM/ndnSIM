/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
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

#ifndef _CCNX_RIT_H_
#define	_CCNX_RIT_H_

#include <list>
#include "ns3/nstime.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

namespace ns3 {

class CcnxInterestHeader;

/**
 * \ingroup ccnx
 * \brief Recently satisfied interest 
 *
 * This entry holds information about recently satisfied interest
 *
 * RIT entries will stay in the table for a while before being cleaned out
 */
struct CcnxRitEntry
{
  CcnxNameComponents m_prefix; ///< \brief Prefix of the recently satisfied interest
  uint32_t m_nonce; ///< \brief Nonce of the recently satisfied interest
  Time m_expireTime;  ///< \brief Time when the record should be removed

  CcnxRitEntry (const CcnxNameComponents &prefix, uint32_t nonce, const Time &timeout)
    : m_prefix (prefix)
    , m_nonce (nonce)
    , m_expireTime (timeout)
  { }
};

/**
 * \ingroup ccnx
 * \brief Private namespace for CCNx RIT implementation
 */
namespace __ccnx_private_rit
{
class nonce {}; ///< tag for nonce hash
class timestamp {}; ///< tag for timestamp-ordered records (for cleanup optimization)  
};

/**
 * \ingroup ccnx
 * \brief Typedef for RIT container implemented as a Boost.MultiIndex container
 *
 * - First index (tag<nonce>) is a non-unique hash index based on
 *   nonce (there could be several records with the same nonce,
 *   provided that the prefixes are different
 * - Second index (tag<timestamp>) is a sequenced index based on
 *   arrival order (for clean-up optimizations)
 *
 * Container allows having non-unique nonce values.  In the
 * implementation it is allowed only if prefixes are different. During
 * the lookup process, the nonce field is checked first and, if a
 * match found, the prefix field will be checked to match prefix of
 * the interest.
 *
 * \see http://www.boost.org/doc/libs/1_46_1/libs/multi_index/doc/ for more information on Boost.MultiIndex library
 */
struct CcnxRitContainer
{
  typedef
  boost::multi_index::multi_index_container<
    CcnxRitEntry,
    boost::multi_index::indexed_by<
      boost::multi_index::hashed_non_unique<
        boost::multi_index::tag<__ccnx_private_rit::nonce>,
        boost::multi_index::member<CcnxRitEntry, uint32_t, &CcnxRitEntry::m_nonce>
        >,
      boost::multi_index::sequenced<
        boost::multi_index::tag<__ccnx_private_rit::timestamp> >
      >
    > type;
};

    

/**
 * \ingroup ccnx
 * \brief Recently satisfied interest storage
 *
 * This storage holds information about all recently satisfied
 * interests (prefix and nonce).
 *
 * There is no hard limit on number of entries (limited by the amount
 * of the available memory).  Entries are removed after preconfigured
 * amount of time (RitTimeout, default is 1 second).
 */
class CcnxRit : public CcnxRitContainer::type, public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();

  /**
   * \brief Default constructor
   */
  CcnxRit ();
  virtual ~CcnxRit( );

  /**
   * \brief Find corresponding RIT entry for the given content name
   *
   * This check consists of two steps.  First, we find records with
   * the same nonce.  Then check prefixes of all found records and if
   * prefix of any of them matched prefix of the interest, this
   * function returns true.  In all other cases, it will return false.
   *
   * \param header header of the interest packet in question
   *
   * \returns true if the same interest was recently satisfied
   */
  bool WasRecentlySatisfied (const CcnxInterestHeader &header);

  /**
   * \brief Add a new RIT entry
   *
   * This function asserts (only in debug) if the same interest is
   * already present in RIT.  The caller is responsible of calling
   * WasRecentlySatisfied before calling SetRecentlySatisfied.
   * 
   * \param header header of the interest packet in question
   */
  void SetRecentlySatisfied (const CcnxInterestHeader &header);

  /**
   * \brief Set RIT entries lifetime
   *
   * \param lifetime of RIT entries timeout
   */
  void SetRitTimeout (const Time &timeout);

  /**
   * \brief Get RIT entries lifetime
   *
   * \returns lifetime of RIT entries timeout
   */
  Time GetRitTimeout () const;
  
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

private:
  /**
   * \brief Periodic even to clean up stalled entries
   */
  void CleanExpired ();
	
private:
  Time    m_ritTimeout; ///< \brief Configurable timeout of RIT entries
  Time    m_cleanupTimeout; ///< \brief Configurable timeout of how often cleanup events are working
  EventId m_cleanupEvent; ///< \brief Cleanup event
};
  
//////////////////////////////////////////////////////////////////////
// Helper classes
//////////////////////////////////////////////////////////////////////
/**
 * \ingroup ccnx
 * \brief Typedef for nonce hash index of RIT container
 */
struct CcnxRitByNonce
{
  typedef
  CcnxRitContainer::type::index<__ccnx_private_rit::nonce>::type
  type;
};

} // namespace ns3

#endif // _CCNX_RIT_H_
