/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 UCLA
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
 * Author:  Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_GLOBAL_ROUTER_H
#define NDN_GLOBAL_ROUTER_H

#include "ns3/object.h"
#include "ns3/ptr.h"

#include <list>
#include <boost/tuple/tuple.hpp>

namespace ns3 {

class Channel;
class Ndn;
class NdnFace;
class NdnNameComponents;

/**
 * @brief Class representing global router interface for ndnSIM
 */
class NdnGlobalRouter : public Object
{
public:
  /**
   * @brief Graph edge
   */
  typedef boost::tuple< Ptr< NdnGlobalRouter >, Ptr< NdnFace >, Ptr< NdnGlobalRouter > > Incidency;
  /**
   * @brief List of graph edges
   */
  typedef std::list< Incidency > IncidencyList;
  /**
   * @brief List of locally exported prefixes
   */
  typedef std::list< Ptr<NdnNameComponents> > LocalPrefixList;
  
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  NdnGlobalRouter ();

  /**
   * @brief Get numeric ID of the node (internally assigned)
   */
  uint32_t
  GetId () const;

  /**
   * @brief Helper function to get smart pointer to Ndn object (basically, self)
   */
  Ptr<Ndn>
  GetNdn () const;

  /**
   * @brief Add new locally exported prefix
   * @param prefix Prefix
   */
  void
  AddLocalPrefix (Ptr< NdnNameComponents > prefix);

  /**
   * @brief Add edge to the node
   * @param face Face of the edge
   * @param ndn NdnGlobalRouter of another node
   */
  void
  AddIncidency (Ptr< NdnFace > face, Ptr< NdnGlobalRouter > ndn);

  /**
   * @brief Get list of edges that are connected to this node
   */
  IncidencyList &
  GetIncidencies ();

  /**
   * @brief Get list of locally exported prefixes
   */
  const LocalPrefixList &
  GetLocalPrefixes () const;

  // ??
protected:
  virtual void
  NotifyNewAggregate (); ///< @brief Notify when the object is aggregated to another object (e.g., Node)
  
private:
  uint32_t m_id;
  
  Ptr<Ndn> m_ndn;
  LocalPrefixList m_localPrefixes;
  IncidencyList m_incidencies;

  static uint32_t m_idCounter;
};

inline bool
operator == (const NdnGlobalRouter::Incidency &a,
             const NdnGlobalRouter::Incidency &b)
{
  return a.get<0> () == b.get<0> () &&
    a.get<1> () == b.get<1> () &&
    a.get<2> () == b.get<2> ();
}

inline bool
operator != (const NdnGlobalRouter::Incidency &a,
             const NdnGlobalRouter::Incidency &b)
{
  return ! (a == b);
}

}

#endif // NDN_GLOBAL_ROUTER_H
