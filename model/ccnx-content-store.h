/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011,2012 University of California, Los Angeles
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
 *         Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef CCNX_CONTENT_STORE_H
#define	CCNX_CONTENT_STORE_H

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"

#include <boost/tuple/tuple.hpp>

namespace  ns3
{

class Packet;
class CcnxContentObjectHeader;
class CcnxInterestHeader;
class CcnxNameComponents;

/**
 * \ingroup ccnx
 * \brief NDN content store entry
 *
 * Content store entry stores separately pseudo header and content of
 * ContentObject packet.  It is responsibility of the caller to
 * construct a fully formed CcnxPacket by calling Copy(), AddHeader(),
 * AddTail() on the packet received by GetPacket() method.
 *
 * GetFullyFormedCcnxPacket method provided as a convenience
 */
class CcnxContentStoreEntry : public SimpleRefCount<CcnxContentStoreEntry>
{
public:
  /**
   * \brief Construct content store entry
   *
   * \param header Parsed CcnxContentObject header
   * \param packet Original CCNx packet
   *
   * The constructor will make a copy of the supplied packet and calls
   * RemoveHeader and RemoveTail on the copy.
   */
  CcnxContentStoreEntry (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet);

  /**
   * \brief Get prefix of the stored entry
   * \returns prefix of the stored entry
   */
  const CcnxNameComponents&
  GetName () const;

  /**
   * \brief Get CcnxContentObjectHeader of the stored entry
   * \returns CcnxContentObjectHeader of the stored entry
   */
  Ptr<const CcnxContentObjectHeader>
  GetHeader () const;

  /**
   * \brief Get content of the stored entry
   * \returns content of the stored entry
   */
  Ptr<const Packet>
  GetPacket () const;

  /**
   * \brief Convenience method to create a fully formed CCNx packet from stored header and content
   * \returns A read-write copy of the packet with CcnxContentObjectHeader and CcxnContentObjectTail
   */
  Ptr<Packet>
  GetFullyFormedCcnxPacket () const;

private:
  Ptr<CcnxContentObjectHeader> m_header; ///< \brief non-modifiable CcnxContentObjectHeader
  Ptr<Packet> m_packet; ///< \brief non-modifiable content of the ContentObject packet
};


/**
 * \ingroup ccnx
 * \brief Base class for NDN content store
 *
 * Particular implementations should implement Lookup, Add, and Print methods
 */
class CcnxContentStore : public Object
{
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static
  TypeId GetTypeId ();

  /**
   * @brief Virtual destructor
   */
  virtual ~CcnxContentStore ();
            
  /**
   * \brief Find corresponding CS entry for the given interest
   *
   * \param interest Interest for which matching content store entry
   * will be searched
   *
   * If an entry is found, it is promoted to the top of most recent
   * used entries index, \see m_contentStore
   */
  virtual boost::tuple<Ptr<Packet>, Ptr<const CcnxContentObjectHeader>, Ptr<const Packet> >
  Lookup (Ptr<const CcnxInterestHeader> interest) = 0;
            
  /**
   * \brief Add a new content to the content store.
   *
   * \param header Fully parsed CcnxContentObjectHeader
   * \param packet Fully formed CCNx packet to add to content store
   * (will be copied and stripped down of headers)
   * @returns true if an existing entry was updated, false otherwise
   */
  virtual bool
  Add (Ptr<CcnxContentObjectHeader> header, Ptr<const Packet> packet) = 0;

  // /**
  //  * \brief Add a new content to the content store.
  //  *
  //  * \param header Interest header for which an entry should be removed
  //  * @returns true if an existing entry was removed, false otherwise
  //  */
  // virtual bool
  // Remove (Ptr<CcnxInterestHeader> header) = 0;
  
  /**
   * \brief Print out content store entries
   */
  virtual void
  Print (std::ostream &os) const = 0;

protected:
  TracedCallback<Ptr<const CcnxInterestHeader>,
                 Ptr<const CcnxContentObjectHeader> > m_cacheHitsTrace; ///< @brief trace of cache hits
    
  TracedCallback<Ptr<const CcnxInterestHeader> > m_cacheMissesTrace; ///< @brief trace of cache misses
};

inline std::ostream&
operator<< (std::ostream &os, const CcnxContentStore &cs)
{
  cs.Print (os);
  return os;
}

} // namespace ns3

#endif // CCNX_CONTENT_STORE_H
