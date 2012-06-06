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

#ifndef _CCNX_HEADER_HELPER_H_
#define _CCNX_HEADER_HELPER_H_

#include "ns3/ptr.h"

#define INTEREST_BYTE0 0x01
#define INTEREST_BYTE1 0xD2

#define CONTENT_OBJECT_BYTE0 0x04
#define CONTENT_OBJECT_BYTE1 0x82

namespace ns3
{

class Header;
class Packet;

/**
 * \ingroup ccnx-helpers
 *
 * \brief Class implementing functionality to detect CCNx packet type and
 * create the corresponding object
 *
 * CCNx doesn't really have a header, so we need this class to
 * determine type of CCNx packet and return corresponent header class,
 * CcnxInterestHeader or CcnxContentObjectHeader
 *
 * Throws CcnxUnknownHeaderException if header type couldn't be determined
 */
class CcnxHeaderHelper
{
public:
  /**
     @brief enum for Ccnx packet types
   */
  enum Type {INTEREST, CONTENT_OBJECT}; 

  /**
   * Static function to create an appropriate CCNx header
   *
   * It peeks first 2 bytes of a packet.
   *
   *  All interests start with 
   *   +-----------------+  +---+---------+-------+
   *   | 0 0 0 0 0 0 0 1 |  | 1 | 1 0 1 0 | 0 1 0 |   (0x01 0xD2)
   *   +-----------------+  +---+---------+-------+
   *
   *  All content objects start with 
   *   +-----------------+  +---+---------+-------+
   *   | 0 0 0 0 0 1 0 0 |  | 1 | 0 0 0 0 | 0 1 0 |   (0x04 0x82)
   *   +-----------------+  +---+---------+-------+
   *                          ^             ^^^^^
   *                          |               |
   *                      terminator      DTAG (Dictionary TAG)
   *
   * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
   */
  
  static Type
  GetCcnxHeaderType (Ptr<const Packet> packet);
};

  /**
   * \ingroup ccnx
   * \defgroup ccnx-exceptions Exceptions
   */
  /**
   * \ingroup ccnx-exceptions
   * \brief Exception thrown if CCNx stack receives unrecognized
   * message type
   */
class CcnxUnknownHeaderException {};

} // namespace ns3

#endif // _CCNX_HEADER_HELPER_H_
