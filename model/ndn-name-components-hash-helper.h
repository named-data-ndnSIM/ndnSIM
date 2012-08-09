/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 */

#ifndef NDN_HASH_HELPER_H
#define NDN_HASH_HELPER_H

#include <string>
#include <boost/foreach.hpp>
#include "ndn-name-components.h"

namespace ns3
{

/**
 * \ingroup ndn-helpers
 * \brief Helper providing hash value for the name prefix
 *
 * The whole prefix is considered as a long string with '/' delimiters
 *
 * \todo Testing is required to determine if this hash function
 * actually provides good hash results
 */
struct NdnPrefixHash : public std::unary_function<NdnNameComponents, std::size_t>
{
  /**
   * @brief Compute hash of the name prefix
   */
  std::size_t
  operator() (const NdnNameComponents &prefix) const
  {
    std::size_t hash = 23;
    BOOST_FOREACH (const std::string &str, prefix.GetComponents ())
      {
        hash += str.size ();
        hash = ((hash << 6) ^ (hash >> 27)) + '/';
        BOOST_FOREACH (char c, str)
          {
            hash = ((hash << 6) ^ (hash >> 27)) + c;
          }
      }
    return hash;
  }
};
} // namespace ns3

#endif // NDN_HASH_HELPER_H
