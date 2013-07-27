/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 */

#ifndef _BATCHES_H_
#define _BATCHES_H_

#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include "ns3/nstime.h"
#include <list>
#include <boost/tuple/tuple.hpp>

namespace ns3 {

/**
 * @ingroup ndn-apps
 * @brief Class representing sets of (time, number) tuples with support of reading writing to streams
 */
class Batches : public std::list<boost::tuple<Time, uint32_t> >
{
public:
  /**
   * @brief Default constructor
   */
  Batches () { };

  /**
   * @brief Add tuple
   * @param when   time for the tuple
   * @param amount number for the tuple
   */
  void
  Add (const Time &when, uint32_t amount)
  {
    push_back (boost::make_tuple<Time, uint32_t> (when, amount));
  }
};

ATTRIBUTE_HELPER_HEADER (Batches);

/**
 * @brief Output contents of the Batches to the std::ostream
 * @param os reference to std::ostream
 * @param batch constant reference to Batch object
 */
std::ostream &
operator << (std::ostream &os, const Batches &batch);

/**
 * \brief Read components from input and add them to components. Will read input stream till eof
 * Substrings separated by slashes will become separate components
 */
std::istream &
operator >> (std::istream &is, Batches &batch);

} // namespace ns3

#endif // _BATCHES_H_
