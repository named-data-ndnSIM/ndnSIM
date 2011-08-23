/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */
#ifndef CCNX_ROUTE_H
#define CCNX_ROUTE_H

#include <list>
#include <map>
#include <ostream>

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include "ns3/name-components.h"

namespace ns3 {
  
class CcnxFace;

/**
 * \ingroup ccnxRouting
 *
 *\brief Ccnx route cache entry (similar to Linux struct rtable)
 *
 * This is a reference counted object.  In the future, we will add other 
 * entries from struct dst_entry, struct rtable, and struct dst_ops as needed.
 */
class CcnxRoute : public SimpleRefCount<CcnxRoute> 
{
public:
  CcnxRoute ();

  /**
   * \param dest Destination CcnxAddress
   */
  void SetPrefix (const Ptr<CcnxNameComponents> &dest);
  /**
   * \return Destination CcnxAddress of the route
   */
  const CcnxNameComponents& GetPrefix (void) const;

  /**
   * Equivalent in Linux to dst_entry.dev
   *
   * \param outputDevice pointer to NetDevice for outgoing packets
   */
  void SetOutputFace (Ptr<CcnxFace> outputDevice);
  /**
   * \return pointer to NetDevice for outgoing packets
   */
  Ptr<CcnxFace> GetOutputFace (void) const;

private:
  Ptr<CcnxNameComponents> m_prefix;
  Ptr<CcnxFace> m_outputFace;
};

std::ostream& operator<< (std::ostream& os, CcnxRoute const& route);

} //namespace ns3

#endif /* CCNX_ROUTE_H */
