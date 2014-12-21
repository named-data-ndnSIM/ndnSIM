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

#ifndef _BATCHES_H_
#define _BATCHES_H_

#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include "ns3/nstime.h"
#include <list>
#include <tuple>

namespace ns3 {

/**
 * @ingroup ndn-apps
 * @brief Class representing sets of (time, number) tuples with support of reading writing to
 * streams
 */
class Batches : public std::list<std::tuple<Time, uint32_t>> {
public:
  /**
   * @brief Default constructor
   */
  Batches(){};

  /**
   * @brief Add tuple
   * @param when   time for the tuple
   * @param amount number for the tuple
   */
  void
  Add(const Time& when, uint32_t amount)
  {
    push_back(std::make_tuple(when, amount));
  }
};

ATTRIBUTE_HELPER_HEADER(Batches);

/**
 * @brief Output contents of the Batches to the std::ostream
 * @param os reference to std::ostream
 * @param batch constant reference to Batch object
 */
std::ostream&
operator<<(std::ostream& os, const Batches& batch);

/**
 * \brief Read components from input and add them to components. Will read input stream till eof
 * Substrings separated by slashes will become separate components
 */
std::istream&
operator>>(std::istream& is, Batches& batch);

} // namespace ns3

#endif // _BATCHES_H_
