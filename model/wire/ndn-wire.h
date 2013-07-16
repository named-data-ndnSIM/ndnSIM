/** -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/* 
 * Copyright (c) 2013, Regents of the University of California
 *                     Alexander Afanasyev
 * 
 * GNU 3.0 license, See the LICENSE file for more information
 * 
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_WIRE_H
#define NDN_WIRE_H

#include "ns3/buffer.h"

#include "ns3/ndn-common.h"
#include "ns3/ndn-name.h"
#include "ns3/ndn-interest.h"
#include "ns3/ndn-content-object.h"

NDN_NAMESPACE_BEGIN

struct Wire
{
  enum
    {
      WIRE_FORMAT_DEFAULT = -2,
      WIRE_FORMAT_AUTODETECT = -1,
    
      WIRE_FORMAT_NDNSIM = 0,
      WIRE_FORMAT_CCNB = 1
    };

  static Ptr<Packet>
  FromInterest (Ptr<const Interest> interest, int8_t wireFormat = WIRE_FORMAT_DEFAULT);

  static Ptr<Interest>
  ToInterest (Ptr<Packet> packet, int8_t type = WIRE_FORMAT_AUTODETECT);

  static Ptr<Packet>
  FromData (Ptr<const ContentObject> data, int8_t wireFormat = WIRE_FORMAT_DEFAULT);

  static Ptr<ContentObject>
  ToData (Ptr<Packet> packet, int8_t type = WIRE_FORMAT_AUTODETECT);

  /*
   * @brief Get size of buffer to fit wire-formatted name object
   */
  static uint32_t
  FromNameSize (Ptr<const Name> name, int8_t wireFormat = WIRE_FORMAT_DEFAULT);
  
  /**
   * @brief Convert name to wire format
   */
  static void
  FromName (Buffer::Iterator start, Ptr<const Name> name, int8_t wireFormat = WIRE_FORMAT_DEFAULT);

  /**
   * @brief Convert name from wire format
   */
  static Ptr<Name>
  ToName (Buffer::Iterator start, int8_t wireFormat = WIRE_FORMAT_DEFAULT);
};

NDN_NAMESPACE_END

#endif // NDN_WIRE_H
