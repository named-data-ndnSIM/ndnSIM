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

#include "ns3/simple-ref-count.h"
#include "ns3/node.h"

#include "ns3/ccnx-fib-entry.h"

namespace ns3 {

class CcnxInterestHeader;

/**
 * \ingroup ccnx
 * \brief Class implementing FIB functionality
 */
class CcnxFib : public Object
{
public:
  typedef ns3::Ptr<CcnxFibEntry> iterator; // not sure, but let's see what will happen
  typedef ns3::Ptr<const CcnxFibEntry> const_iterator;

  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId GetTypeId ();
  /**
   * @brief Default constructor
   */
  CcnxFib () {}
  
  /**
   * @brief Virtual destructor
   */
  virtual ~CcnxFib () { };
  
  /**
   * \brief Perform longest prefix match
   *
   * \todo Implement exclude filters
   *
   * \param interest Interest packet header
   * \returns If entry found a valid iterator will be returned, otherwise end ()
   */
  virtual iterator
  LongestPrefixMatch (const CcnxInterestHeader &interest) const = 0;
  
  /**
   * \brief Add or update FIB entry
   *
   * If the entry exists, metric will be updated. Otherwise, new entry will be created
   *
   * Entries in FIB never deleted. They can be invalidated with metric==NETWORK_UNREACHABLE
   *
   * @param name	Prefix
   * @param face	Forwarding face
   * @param metric	Routing metric
   */
  virtual iterator
  Add (const CcnxNameComponents &prefix, Ptr<CcnxFace> face, int32_t metric) = 0;

  /**
   * \brief Add or update FIB entry using smart pointer to prefix
   *
   * If the entry exists, metric will be updated. Otherwise, new entry will be created
   *
   * Entries in FIB never deleted. They can be invalidated with metric==NETWORK_UNREACHABLE
   *
   * @param name	Smart pointer to prefix
   * @param face	Forwarding face
   * @param metric	Routing metric
   */
  virtual iterator
  Add (const Ptr<const CcnxNameComponents> &prefix, Ptr<CcnxFace> face, int32_t metric) = 0;

  /**
   * @brief Remove FIB entry
   *
   * ! ATTENTION ! Use with caution.  All PIT entries referencing the corresponding FIB entry will become invalid.
   * So, simulation may crash.
   *
   * @param name	Smart pointer to prefix
   */
  virtual void
  Remove (const Ptr<const CcnxNameComponents> &prefix) = 0;

  // /**
  //  * @brief Invalidate FIB entry ("Safe" version of Remove)
  //  *
  //  * All faces for the entry will be assigned maximum routing metric and NDN_FIB_RED status   
  //  * @param name	Smart pointer to prefix
  //  */
  // virtual void
  // Invalidate (const Ptr<const CcnxNameComponents> &prefix) = 0;

  /**
   * @brief Invalidate all FIB entries
   */
  virtual void
  InvalidateAll () = 0;
  
  /**
   * @brief Remove all references to a face from FIB.  If for some enty that face was the only element,
   * this FIB entry will be removed.
   */
  virtual void
  RemoveFromAll (Ptr<CcnxFace> face) = 0;

  /**
   * @brief Print out entries in FIB
   */
  virtual void
  Print (std::ostream &os) const = 0;

  /**
   * @brief Return first element of FIB (no order guaranteed)
   */
  virtual const_iterator
  Begin () = 0;

  /**
   * @brief Return item next after last (no order guaranteed)
   */
  virtual const_iterator
  End () = 0;

  /**
   * @brief Advance the iterator
   */
  virtual const_iterator
  Next (const_iterator item) = 0;

  static inline Ptr<CcnxFib>
  GetCcnxFib (Ptr<Object> node);
  
private:
  friend std::ostream& operator<< (std::ostream& os, const CcnxFib &fib);
  CcnxFib(const CcnxFib&) {} ; ///< \brief copy constructor is disabled
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<< (std::ostream& os, const CcnxFib &fib);

Ptr<CcnxFib>
CcnxFib::GetCcnxFib (Ptr<Object> node)
{
  return node->GetObject<CcnxFib> ();
}

} // namespace ns3

#endif	/* NDN_FIB_H */
