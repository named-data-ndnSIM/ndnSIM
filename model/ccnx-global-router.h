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

#ifndef CCNX_GLOBAL_ROUTER_H
#define CCNX_GLOBAL_ROUTER_H

#include "ns3/object.h"
#include "ns3/ptr.h"

#include <list>
#include <boost/tuple/tuple.hpp>

namespace ns3 {

class Channel;
class Ccnx;
class CcnxFace;
class CcnxNameComponents;

/**
 * @brief Class representing global router interface for ndnSIM
 */
class CcnxGlobalRouter : public Object
{
public:
  /**
   * @brief Graph edge
   */
  typedef boost::tuple< Ptr< CcnxGlobalRouter >, Ptr< CcnxFace >, Ptr< CcnxGlobalRouter > > Incidency;
  /**
   * @brief List of graph edges
   */
  typedef std::list< Incidency > IncidencyList;
  /**
   * @brief List of locally exported prefixes
   */
  typedef std::list< Ptr<CcnxNameComponents> > LocalPrefixList;
  
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
  CcnxGlobalRouter ();

  /**
   * @brief Get numeric ID of the node (internally assigned)
   */
  uint32_t
  GetId () const;

  /**
   * @brief Helper function to get smart pointer to Ccnx object (basically, self)
   */
  Ptr<Ccnx>
  GetCcnx () const;

  /**
   * @brief Add new locally exported prefix
   * @param prefix Prefix
   */
  void
  AddLocalPrefix (Ptr< CcnxNameComponents > prefix);

  /**
   * @brief Add edge to the node
   * @param face Face of the edge
   * @param ccnx CcnxGlobalRouter of another node
   */
  void
  AddIncidency (Ptr< CcnxFace > face, Ptr< CcnxGlobalRouter > ccnx);

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
  
  Ptr<Ccnx> m_ccnx;
  LocalPrefixList m_localPrefixes;
  IncidencyList m_incidencies;

  static uint32_t m_idCounter;
};

inline bool
operator == (const CcnxGlobalRouter::Incidency &a,
             const CcnxGlobalRouter::Incidency &b)
{
  return a.get<0> () == b.get<0> () &&
    a.get<1> () == b.get<1> () &&
    a.get<2> () == b.get<2> ();
}

inline bool
operator != (const CcnxGlobalRouter::Incidency &a,
             const CcnxGlobalRouter::Incidency &b)
{
  return ! (a == b);
}

}

#endif // CCNX_GLOBAL_ROUTER_H
