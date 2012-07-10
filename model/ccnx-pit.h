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

#include "ccnx-pit-entry.h"

namespace ns3 {

class Ccnx;
class CcnxFace;
class CcnxContentObjectHeader;
class CcnxInterestHeader;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/**
 * \ingroup ccnx
 * \brief Class implementing Pending Interests Table
 */
class CcnxPit : public Object
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
   * \brief Destructor
   */
  virtual ~CcnxPit ();

  /**
   * \brief Find corresponding PIT entry for the given content name
   * \param prefix Prefix for which to lookup the entry
   * \returns iterator to Pit entry. If record not found,
   *          return end() iterator
   */
  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxContentObjectHeader &header) const = 0;

  /**
   * \brief Find a PIT entry for the given content interest
   * \param header parsed interest header
   * \returns iterator to Pit entry. If record not found,
   *          return end() iterator
   */
  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxInterestHeader &header) = 0;

  /**
   * @brief Check if the Interest carries an existent nonce.
   * If not, nonce will be added to the list of known nonces
   * @returns true if interest is duplicate (carries an existent nonce), false otherwise
   */
  virtual bool
  CheckIfDuplicate (Ptr<CcnxPitEntry> entry, const CcnxInterestHeader &header) = 0;
  
  /**
   * @brief Creates a PIT entry for the given interest
   * @param header parsed interest header
   * @returns iterator to Pit entry. If record could not be created (e.g., limit reached),
   *          return end() iterator
   *
   * Note. This call assumes that the entry does not exist (i.e., there was a Lookup call before)
   */
  virtual Ptr<CcnxPitEntry>
  Create (const CcnxInterestHeader &header) = 0;
  
  /**
   * @brief Mark PIT entry deleted
   * @param entry PIT entry
   *
   * Effectively, this method removes all incoming/outgoing faces and set
   * lifetime +m_PitEntryDefaultLifetime from Now ()
   */
  virtual void
  MarkErased (Ptr<CcnxPitEntry> entry) = 0;

  /**
   * @brief Print out PIT contents for debugging purposes
   *
   * Note that there is no definite order in which entries are printed out
   */
  virtual void
  Print (std::ostream &os) const = 0;

  template<class A,class M>
  void
  modify (A, M)
  {
    ;
  }

  template<class A>
  void
  erase (A)
  {
    ;
  }

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  
  /**
   * @brief Static call to cheat python bindings
   */
  static inline Ptr<CcnxFib>
  GetCcnxPit (Ptr<Object> node);

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

protected:
  virtual void
  DoCleanExpired () = 0;
  
private:
  /**
   * @brief Remove expired records from PIT
   */
  void
  CleanExpired ();
  
  /**
   * \brief Set cleanup timeout
   *
   * Side effect: current clean up even (if any) will be cancelled and a new event started
   *
   * \param timeout cleanup timeout
   */
  void
  SetCleanupTimeout (const Time &timeout);

  /**
   * \brief Get cleanup timeout
   *
   * \returns cleanup timeout
   */
  Time
  GetCleanupTimeout () const;
  
protected:
  Time    m_cleanupTimeout; ///< \brief Configurable timeout of how often cleanup events are working
  EventId m_cleanupEvent;   ///< \brief Cleanup event

  // configuration variables. Check implementation of GetTypeId for more details
  Time    m_PitEntryPruningTimout;
  Time    m_PitEntryDefaultLifetime;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

inline std::ostream&
operator<< (std::ostream& os, const CcnxPit &pit)
{
  pit.Print (os);
  return os;
}

} // namespace ns3

#endif	/* CCNX_PIT_H */
