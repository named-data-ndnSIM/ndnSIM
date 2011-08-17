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
 * Author: 
 */

#ifndef _CCNX_ENCODING_HELPER_H_
#define _CCNX_ENCODING_HELPER_H_

#include <sys/types.h>

#include "ns3/ccnx.h"
#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"

namespace ns3 {

namespace Name{ class Components; }

class CcnxInterestHeader;
class CcnxContentObjectHeader;
  
/**
 * Helper to encode/decode ccnb formatted CCNx message
 *
 */
class CcnxEncodingHelper
{
public:
  static size_t
  Serialize (Buffer::Iterator start, const CcnxInterestHeader &interest);

  static size_t
  Serialize (Buffer::Iterator start, const CcnxContentObjectHeader &contentObject);

private:
  static size_t
  AppendBlockHeader (Buffer::Iterator start, size_t value, Ccnx::ccn_tt block_type);

  static size_t
  AppendNumber (Buffer::Iterator start, uint32_t number);

  static size_t
  AppendCloser (Buffer::Iterator start);

  static size_t
  AppendNameComponents (Buffer::Iterator start, const Name::Components &name);

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
  AppendTimestampBlob (Buffer::Iterator start, Time time);

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
  AppendTaggedBlob (Buffer::Iterator start, Ccnx::ccn_dtag dtag,
                    const uint8_t *data, size_t size);
  
};

} // namespace ns3

#endif // _CCNX_ENCODING_HELPER_H_

