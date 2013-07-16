/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_WIRE_NDNSIM_SYNTAX_H
#define NDN_WIRE_NDNSIM_SYNTAX_H

#include "ns3/ptr.h"
#include "ns3/nstime.h"
#include "ns3/buffer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-name.h"

NDN_NAMESPACE_BEGIN

namespace wire {

/**
 * \brief Helper to encode ndnSIM wire elements
 */
class NdnSim
{
public:
  /**
   * @brief Append Name in ndnSIM encoding
   * @param start Buffer to store serialized Interest
   * @param name constant reference to Name object
   *
   * @returns written length
   */
  static size_t
  SerializeName (Buffer::Iterator &start, const Name &name);

  /**
   * @brief Estimate size of Name in ndnSIM encoding
   * @param name constant reference to Name object
   * @returns estimated length
   */
  static size_t
  SerializedSizeName (const Name &name);

  /**
   * @brief Deserialize Name from ndnSIM encodeing
   * @param start Buffer that stores serialized Interest
   * @param name Name object
   */
  static Ptr<Name>
  DeserializeName (Buffer::Iterator &start);
}; // NdnSim

} // wire

NDN_NAMESPACE_END

#endif // NDN_WIRE_NDNSIM_SYNTAX_H
