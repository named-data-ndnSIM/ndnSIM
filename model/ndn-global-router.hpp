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

#ifndef NDN_GLOBAL_ROUTER_H
#define NDN_GLOBAL_ROUTER_H

#include "ns3/ndnSIM/model/ndn-common.hpp"
#include "ns3/ndnSIM/model/ndn-face.hpp"

#include "ns3/object.h"
#include "ns3/ptr.h"

#include <list>
#include <tuple>

namespace ns3 {

class Channel;

namespace ndn {

class L3Protocol;

/**
 * @ingroup ndn-helpers
 * @brief Class representing global router interface for ndnSIM
 */
class GlobalRouter : public Object {
public:
  /**
   * @brief Graph edge
   */
  typedef std::tuple<Ptr<GlobalRouter>, shared_ptr<Face>, Ptr<GlobalRouter>> Incidency;
  /**
   * @brief List of graph edges
   */
  typedef std::list<Incidency> IncidencyList;
  /**
   * @brief List of locally exported prefixes
   */
  typedef std::list<shared_ptr<Name>> LocalPrefixList;

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
  GlobalRouter();

  /**
   * @brief Get numeric ID of the node (internally assigned)
   */
  uint32_t
  GetId() const;

  /**
   * @brief Helper function to get smart pointer to ndn::L3Protocol object (basically, self)
   */
  Ptr<L3Protocol>
  GetL3Protocol() const;

  /**
   * @brief Add new locally exported prefix
   * @param prefix Prefix
   */
  void
  AddLocalPrefix(shared_ptr<Name> prefix);

  /**
   * @brief Add edge to the node
   * @param face Face of the edge
   * @param ndn GlobalRouter of another node
   */
  void
  AddIncidency(shared_ptr<Face> face, Ptr<GlobalRouter> ndn);

  /**
   * @brief Get list of edges that are connected to this node
   */
  IncidencyList&
  GetIncidencies();

  /**
   * @brief Get list of locally exported prefixes
   */
  const LocalPrefixList&
  GetLocalPrefixes() const;

  /**
   * @brief Clear global state
   */
  static void
  clear();

protected:
  virtual void
  NotifyNewAggregate(); ///< @brief Notify when the object is aggregated to another object (e.g.,
                        /// Node)

private:
  uint32_t m_id;

  Ptr<L3Protocol> m_ndn;
  LocalPrefixList m_localPrefixes;
  IncidencyList m_incidencies;

  static uint32_t m_idCounter;
};

inline bool
operator==(const GlobalRouter::Incidency& a, const GlobalRouter::Incidency& b)
{
  return std::get<0>(a) == std::get<0>(b) && std::get<1>(a) == std::get<1>(b)
         && std::get<2>(a) == std::get<2>(b);
}

inline bool
operator!=(const GlobalRouter::Incidency& a, const GlobalRouter::Incidency& b)
{
  return !(a == b);
}

} // namespace ndn
} // namespace ns3

#endif // NDN_GLOBAL_ROUTER_H
