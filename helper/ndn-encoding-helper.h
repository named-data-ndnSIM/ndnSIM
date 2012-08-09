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

#ifndef _NDN_ENCODING_HELPER_H_
#define _NDN_ENCODING_HELPER_H_

#include <sys/types.h>

#include "ccnb-parser/common.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"

namespace ns3 {
namespace ndn {

class NameComponents;

class InterestHeader;
class ContentObjectHeader;
  
/**
 * \brief Helper to encode/decode ccnb formatted Ndn message
 */
class EncodingHelper
{
public:
  /**
   * \brief Serialize NdnInterestHeader to Buffer::Iterator
   * @param start Buffer to store serialized NdnInterestHeader
   * @param interest Pointer to NdnInterestHeader to be serialized 
   * @return length of serialized NdnInterestHeader
   */
  static size_t
  Serialize (Buffer::Iterator start, const InterestHeader &interest);

  /**
   * \brief Compute the size of serialized NdnInterestHeader
   * @param interest Pointer to NdnInterestHeader
   * @return length 
   */
  static size_t
  GetSerializedSize (const InterestHeader &interest);
  
public:
  /**
   * @brief Append CCNB block header
   * @param start Buffer to store serialized NdnInterestHeader
   * @param value dictionary id of the block header
   * @param block_type Type of CCNB block
   *
   * @returns written length
   */
  static size_t
  AppendBlockHeader (Buffer::Iterator &start, size_t value, CcnbParser::ccn_tt block_type);

  /**
   * @brief Estimate size of the CCNB block header
   * @param value dictionary id of the block header
   * @returns estimated length
   */
  static size_t
  EstimateBlockHeader (size_t value);

  /**
   * @brief Add number in CCNB encoding
   * @param start Buffer to store serialized NdnInterestHeader
   * @param number Number to be written
   *
   * @returns written length
   */
  static size_t
  AppendNumber (Buffer::Iterator &start, uint32_t number);

  /**
   * @brief Estimate size of the number in CCNB encoding
   * @param number Number to be written
   * @returns estimated length
   */
  static size_t
  EstimateNumber (uint32_t number);

  /**
   * @brief Append CCNB closer tag (estimated size is 1)
   * @param start Buffer to store serialized InterestHeader
   *
   * @returns written length
   */
  static size_t
  AppendCloser (Buffer::Iterator &start);

  /**
   * @brief Append NameComponents in CCNB encoding
   * @param start Buffer to store serialized InterestHeader
   * @param name constant reference to NameComponents object
   *
   * @returns written length
   */
  static size_t
  AppendNameComponents (Buffer::Iterator &start, const NameComponents &name);

  /**
   * @brief Estimate size of NameComponents in CCNB encoding
   * @param name constant reference to NameComponents object
   * @returns estimated length
   */
  static size_t
  EstimateNameComponents (const NameComponents &name);

  /**
   * Append a binary timestamp as a BLOB using the ccn binary
   * Timestamp representation (12-bit fraction).
   *
   * @param start start iterator of  the buffer to append to.
   * @param time - Time object
   *
   * @returns written length
   */
  static size_t
  AppendTimestampBlob (Buffer::Iterator &start, const Time &time);

  /**
   * @brief Estimate size of a binary timestamp as a BLOB using CCNB enconding
   * @param time - Time object
   * @returns estimated length
   */
  static size_t
  EstimateTimestampBlob (const Time &time);

  /**
   * Append a tagged BLOB
   *
   * This is a ccnb-encoded element with containing the BLOB as content
   *
   * @param start start iterator of  the buffer to append to.
   * @param dtag is the element's dtab
   * @param data points to the binary data
   * @param size is the size of the data, in bytes
   *
   * @returns written length
   */
  static size_t
  AppendTaggedBlob (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag,
                    const uint8_t *data, size_t size);
  
  /**
   * @brief Estimate size of a tagged BLOB in CCNB enconding
   * @param dtag is the element's dtab
   * @param size is the size of the data, in bytes
   * @returns estimated length
   */
  static size_t
  EstimateTaggedBlob (CcnbParser::ccn_dtag dtag, size_t size);

  /**
   * Append value as a tagged BLOB (templated version)
   *
   * This is a ccnb-encoded element with containing the BLOB as content
   *
   * Data will be reinterpret_cast<const uint8_t*> and size will be obtained using sizeof
   *
   * @param start start iterator of  the buffer to append to.
   * @param dtag is the element's dtab
   * @param data a value to add
   *
   * @returns written length
   */
  template<class T>
  static inline size_t
  AppendTaggedBlob (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag, const T &data);

  /**
   * Append a tagged string (should be a valid UTF-8 coded string)
   *
   * This is a ccnb-encoded element with containing UDATA as content
   *
   * @param start start iterator of  the buffer to append to.
   * @param dtag is the element's dtab
   * @param string UTF-8 string to be written
   *
   * @returns written length
   */
  static size_t
  AppendString (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag,
                const std::string &string);

  /**
   * @brief Estimate size of the string in CCNB encoding
   * @param dtag is the element's dtab
   * @param string UTF-8 string to be written
   * @returns estimated length
   */
  static size_t
  EstimateString (CcnbParser::ccn_dtag dtag, const std::string &string);
};


template<class T>
size_t
EncodingHelper::AppendTaggedBlob (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag, const T &data)
{
  return AppendTaggedBlob (start, dtag, reinterpret_cast<const uint8_t*> (&data), sizeof (data));
}

} // namespace ndn
} // namespace ns3

#endif // _NDN_ENCODING_HELPER_H_

