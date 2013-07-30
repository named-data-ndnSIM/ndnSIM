/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011-2013 University of California, Los Angeles
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

#ifndef NDN_WIRE_CCNB_SYNTAX_H
#define NDN_WIRE_CCNB_SYNTAX_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-name.h"

NDN_NAMESPACE_BEGIN

namespace wire {

/**
 * \brief Helper to encode CCNb blocks
 */
class Ccnb
{
public:
  /**
   * @brief Append CCNB block header
   * @param start Buffer to store serialized
   * @param value dictionary id of the block header
   * @param block_type Type of CCNB block
   *
   * @returns written length
   */
  static size_t
  AppendBlockHeader (Buffer::Iterator &start, size_t value, uint32_t block_type);

  /**
   * @brief Estimate size of the CCNB block header
   * @param value dictionary id of the block header
   * @returns estimated length
   */
  static size_t
  EstimateBlockHeader (size_t value);

  /**
   * @brief Add number in CCNB encoding
   * @param start Buffer to store serialized NdnInterest
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
   * @param start Buffer to store serialized Interest
   *
   * @returns written length
   */
  static size_t
  AppendCloser (Buffer::Iterator &start);

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
  AppendTaggedBlob (Buffer::Iterator &start, uint32_t dtag,
                    const uint8_t *data, size_t size);
  
  /**
   * Append a tagged BLOB, adding 0-byte padding if necessary
   *
   * This is a ccnb-encoded element with containing the BLOB as content
   *
   * @param start start iterator of  the buffer to append to.
   * @param dtag is the element's dtab
   * @param length minimum required length of the added field (padding added if necessary)
   * @param data points to the binary data
   * @param size is the size of the data, in bytes
   *
   * @returns written length
   */
  static size_t
  AppendTaggedBlobWithPadding (Buffer::Iterator &start, uint32_t dtag,
                               uint32_t length,
                               const uint8_t *data, size_t size);

  /**
   * @brief Estimate size of a tagged BLOB in CCNB enconding
   * @param dtag is the element's dtab
   * @param size is the size of the data, in bytes
   * @returns estimated length
   */
  static size_t
  EstimateTaggedBlob (uint32_t dtag, size_t size);

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
  AppendTaggedBlob (Buffer::Iterator &start, uint32_t dtag, const T &data);

  /**
   * Append value as a tagged BLOB (templated version), add 0-padding if necessary
   *
   * This is a ccnb-encoded element with containing the BLOB as content
   *
   * Data will be reinterpret_cast<const uint8_t*> and size will be obtained using sizeof
   *
   * @param start start iterator of  the buffer to append to.
   * @param dtag is the element's dtab
   * @param length minimum required length of the field
   * @param data a value to add
   *
   * @returns written length
   */
  template<class T>
  static inline size_t
  AppendTaggedBlobWithPadding (Buffer::Iterator &start, uint32_t dtag, uint32_t length, const T &data);

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
  AppendString (Buffer::Iterator &start, uint32_t dtag,
                const std::string &string);

  /**
   * @brief Estimate size of the string in CCNB encoding
   * @param dtag is the element's dtab
   * @param string UTF-8 string to be written
   * @returns estimated length
   */
  static size_t
  EstimateString (uint32_t dtag, const std::string &string);

  ////////////////////////////////
  // General use wire formatters
  ////////////////////////////////
  
  /**
   * @brief Append Name in CCNB encoding
   * @param start Buffer to store serialized Interest
   * @param name constant reference to Name object
   *
   * @returns written length
   */
  static size_t
  SerializeName (Buffer::Iterator &start, const Name &name);

  /**
   * @brief Estimate size of Name in CCNB encoding
   * @param name constant reference to Name object
   * @returns estimated length
   */
  static size_t
  SerializedSizeName (const Name &name);

  /**
   * @brief Deserialize Name from CCNB encodeing
   * @param start Buffer that stores serialized Interest
   * @param name Name object
   */
  static Ptr<Name>
  DeserializeName (Buffer::Iterator &start);
}; // Ccnb


template<class T>
inline size_t
Ccnb::AppendTaggedBlob (Buffer::Iterator &start, uint32_t dtag, const T &data)
{
  return AppendTaggedBlob (start, dtag, reinterpret_cast<const uint8_t*> (&data), sizeof (data));
}

template<class T>
inline size_t
Ccnb::AppendTaggedBlobWithPadding (Buffer::Iterator &start, uint32_t dtag, uint32_t length, const T &data)
{
  return AppendTaggedBlobWithPadding (start, dtag, length, reinterpret_cast<const uint8_t*> (&data), sizeof (data));
}

} // wire

NDN_NAMESPACE_END

#endif // NDN_WIRE_CCNB_SYNTAX_H
