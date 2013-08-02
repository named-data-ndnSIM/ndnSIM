/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
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

#ifndef NDN_CONTENT_STORE_NOCACHE_H
#define	NDN_CONTENT_STORE_NOCACHE_H

#include "ns3/ndn-content-store.h"

namespace ns3 {
namespace ndn {
namespace cs {

/**
 * @ingroup ndn-cs
 * @brief Implementation of ContentStore that completely disables caching
 */
class Nocache : public ContentStore
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
   * @brief Default constructor
   */
  Nocache ();
  
  /**
   * @brief Virtual destructor
   */
  virtual
  ~Nocache ();

  virtual Ptr<Data>
  Lookup (Ptr<const Interest> interest);

  virtual bool
  Add (Ptr<const Data> data);

  virtual void
  Print (std::ostream &os) const;

  virtual uint32_t
  GetSize () const;

  virtual Ptr<cs::Entry>
  Begin ();

  virtual Ptr<cs::Entry>
  End ();

  virtual Ptr<cs::Entry>
  Next (Ptr<cs::Entry>);

};

} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_NOCACHE_H
