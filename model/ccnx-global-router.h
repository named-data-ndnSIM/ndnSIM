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

class CcnxGlobalRouter : public Object
{
public:
  typedef boost::tuple< Ptr< CcnxGlobalRouter >, Ptr< CcnxFace >, Ptr< CcnxGlobalRouter > > Incidency;
  typedef std::list< Incidency > IncidencyList;
  typedef std::list< Ptr<CcnxNameComponents> > LocalPrefixList;
  
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId
  GetTypeId ();

  CcnxGlobalRouter ();

  uint32_t
  GetId () const;
  
  Ptr<Ccnx>
  GetCcnx () const;
  
  void
  AddLocalPrefix (Ptr< CcnxNameComponents > prefix);

  /**
   * 
   */
  void
  AddIncidency (Ptr< CcnxFace > face, Ptr< CcnxGlobalRouter > ccnx);

  IncidencyList &
  GetIncidencies ();

  const LocalPrefixList &
  GetLocalPrefixes () const;

  // ??
protected:
  virtual void
  NotifyNewAggregate ();
  
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
