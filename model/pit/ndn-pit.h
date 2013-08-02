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

#ifndef _NDN_PIT_H_
#define	_NDN_PIT_H_

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/event-id.h"

#include "ndn-pit-entry.h"

namespace ns3 {
namespace ndn {

/**
 * @ingroup ndn
 * @defgroup ndn-pit PIT
 */

/**
 * @ingroup ndn-pit
 * @brief Namespace for PIT operations
 */
namespace pit {
}

class L3Protocol;
class Face;
class Data;
class Interest;

typedef Interest InterestHeader;
typedef Data DataHeader;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

/**
 * @ingroup ndn-pit
 * @brief Class implementing Pending Interests Table
 */
class Pit : public Object
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
  Pit ();

  /**
   * \brief Destructor
   */
  virtual ~Pit ();

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
  virtual Ptr<pit::Entry>
  Lookup (const Data &header) = 0;

  /**
   * \brief Find a PIT entry for the given content interest
   * \param header parsed interest header
   * \returns iterator to Pit entry. If record not found,
   *          return end() iterator
   */
  virtual Ptr<pit::Entry>
  Lookup (const Interest &header) = 0;

  /**
   * @brief Get PIT entry for the prefix (exact match)
   *
   * @param prefix Name for PIT entry
   * @returns If entry is found, a valid iterator (Ptr<pit::Entry>) will be returned. Otherwise End () (==0)
   */
  virtual Ptr<pit::Entry>
  Find (const Name &prefix) = 0;

  /**
   * @brief Creates a PIT entry for the given interest
   * @param header parsed interest header
   * @returns iterator to Pit entry. If record could not be created (e.g., limit reached),
   *          return end() iterator
   *
   * Note. This call assumes that the entry does not exist (i.e., there was a Lookup call before)
   */
  virtual Ptr<pit::Entry>
  Create (Ptr<const Interest> header) = 0;

  /**
   * @brief Mark PIT entry deleted
   * @param entry PIT entry
   *
   * Effectively, this method removes all incoming/outgoing faces and set
   * lifetime +m_PitEntryDefaultLifetime from Now ()
   */
  virtual void
  MarkErased (Ptr<pit::Entry> entry) = 0;

  /**
   * @brief Print out PIT contents for debugging purposes
   *
   * Note that there is no definite order in which entries are printed out
   */
  virtual void
  Print (std::ostream &os) const = 0;

  /**
   * @brief Get number of entries in PIT
   */
  virtual uint32_t
  GetSize () const = 0;

  /**
   * @brief Return first element of FIB (no order guaranteed)
   */
  virtual Ptr<pit::Entry>
  Begin () = 0;

  /**
   * @brief Return item next after last (no order guaranteed)
   */
  virtual Ptr<pit::Entry>
  End () = 0;

  /**
   * @brief Advance the iterator
   */
  virtual Ptr<pit::Entry>
  Next (Ptr<pit::Entry>) = 0;

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Static call to cheat python bindings
   */
  static inline Ptr<Pit>
  GetPit (Ptr<Object> node);

  /**
   * @brief Get maximum PIT entry lifetime
   */
  inline const Time&
  GetMaxPitEntryLifetime () const;

  /**
   * @brief Set maximum PIT entry lifetime
   */
  inline void
  SetMaxPitEntryLifetime (const Time &maxLifetime);

protected:
  // configuration variables. Check implementation of GetTypeId for more details
  Time m_PitEntryPruningTimout;

  Time m_maxPitEntryLifetime;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

inline std::ostream&
operator<< (std::ostream& os, const Pit &pit)
{
  pit.Print (os);
  return os;
}

inline Ptr<Pit>
Pit::GetPit (Ptr<Object> node)
{
  return node->GetObject<Pit> ();
}

inline const Time&
Pit::GetMaxPitEntryLifetime () const
{
  return m_maxPitEntryLifetime;
}

inline void
Pit::SetMaxPitEntryLifetime (const Time &maxLifetime)
{
  m_maxPitEntryLifetime = maxLifetime;
}


} // namespace ndn
} // namespace ns3

#endif	/* NDN_PIT_H */
