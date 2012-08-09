/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
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
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef _NDN_DECODING_HELPER_H_
#define _NDN_DECODING_HELPER_H_

#include <cstring>
#include "ns3/buffer.h"

namespace ns3 {
namespace ndn {

class InterestHeader;
class ContentObjectHeader;

/**
 * \brief Helper class to decode ccnb formatted Ndn message
 */
class DecodingHelper
{
public:
  /**
   * \brief Deserialize Buffer::Iterator to NdnInterestHeader
   * @param start Buffer containing serialized Ndn message
   * @param interest Pointer to the NdnInterestHeader to hold deserialized value
   * @return Number of bytes used for deserialization
   */
  static size_t
  Deserialize (Buffer::Iterator start, InterestHeader &interest);

  /**
   * \brief Deserialize Buffer::Iterator to NdnContentObjectHeader
   * @param start Buffer containing serialized Ndn message
   * @param contentObject Pointer to the NdnContentObjectHeader to hold deserialized value
   * @return Number of bytes used for deserialization
   */
  // static size_t
  // Deserialize (Buffer::Iterator start, ContentObjectHeader &contentObject);
};

} // namespace ndn
} // namespace ns3

#endif // _NDN_DECODING_HELPER_H_
