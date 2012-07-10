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
   *
   * Not that this call should be repeated enough times until it return 0.
   * This way all records with shorter or equal prefix as in content object will be found
   * and satisfied.
   *
   * \param prefix Prefix for which to lookup the entry
   * \returns smart pointer to PIT entry. If record not found,
   *          returns 0
   */
  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxContentObjectHeader &header) = 0;

  /**
   * \brief Find a PIT entry for the given content interest
   * \param header parsed interest header
   * \returns iterator to Pit entry. If record not found,
   *          return end() iterator
   */
  virtual Ptr<CcnxPitEntry>
  Lookup (const CcnxInterestHeader &header) = 0;

  /**
   * @brief Creates a PIT entry for the given interest
   * @param header parsed interest header
   * @returns iterator to Pit entry. If record could not be created (e.g., limit reached),
   *          return end() iterator
   *
   * Note. This call assumes that the entry does not exist (i.e., there was a Lookup call before)
   */
  virtual Ptr<CcnxPitEntry>
  Create (Ptr<const CcnxInterestHeader> header) = 0;
  
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

  // template<class A,class M>
  // void
  // modify (A, M)
  // {
  //   ;
  // }

  // template<class A>
  // void
  // erase (A)
  // {
  //   ;
  // }

  /**
   * @brief Return first element of FIB (no order guaranteed)
   */
  virtual Ptr<CcnxPitEntry>
  Begin () = 0;

  /**
   * @brief Return item next after last (no order guaranteed)
   */
  virtual Ptr<CcnxPitEntry>
  End () = 0;

  /**
   * @brief Advance the iterator
   */
  virtual Ptr<CcnxPitEntry>
  Next (Ptr<CcnxPitEntry>) = 0;

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  
  /**
   * @brief Static call to cheat python bindings
   */
  static inline Ptr<CcnxPit>
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
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

inline std::ostream&
operator<< (std::ostream& os, const CcnxPit &pit)
{
  pit.Print (os);
  return os;
}

inline Ptr<CcnxPit>
CcnxPit::GetCcnxPit (Ptr<Object> node)
{
  return node->GetObject<CcnxPit> ();
}


} // namespace ns3

#endif	/* CCNX_PIT_H */
