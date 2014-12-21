/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef NDN_CONTENT_STORE_NOCACHE_H
#define NDN_CONTENT_STORE_NOCACHE_H

#include "ns3/ndnSIM/model/ndn-common.hpp"

#include "ns3/ndnSIM/model/cs/ndn-content-store.hpp"

namespace ns3 {
namespace ndn {
namespace cs {

/**
 * @ingroup ndn-cs
 * @brief Implementation of ContentStore that completely disables caching
 */
class Nocache : public ContentStore {
public:
  /**
   * \brief Interface ID
   *
   * \return interface ID
   */
  static TypeId
  GetTypeId();

  /**
   * @brief Default constructor
   */
  Nocache();

  /**
   * @brief Virtual destructor
   */
  virtual ~Nocache();

  virtual shared_ptr<Data>
  Lookup(shared_ptr<const Interest> interest);

  virtual bool
  Add(shared_ptr<const Data> data);

  virtual void
  Print(std::ostream& os) const;

  virtual uint32_t
  GetSize() const;

  virtual Ptr<cs::Entry>
  Begin();

  virtual Ptr<cs::Entry>
  End();

  virtual Ptr<cs::Entry> Next(Ptr<cs::Entry>);
};

} // namespace cs
} // namespace ndn
} // namespace ns3

#endif // NDN_CONTENT_STORE_NOCACHE_H
