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

#ifndef _NDN_HEADER_HELPER_H_
#define _NDN_HEADER_HELPER_H_

#include "ns3/ptr.h"

namespace ns3 {

class Header;
class Packet;

namespace ndn {

class Name;
typedef Name NameComponents;

/**
 * @ingroup ndn-helpers
 *
 * \brief Class implementing functionality to detect Ndn packet type and
 * create the corresponding object
 *
 * Ndn doesn't really have a header, so we need this class to
 * determine type of Ndn packet and return corresponent header class,
 * Interest or Data
 *
 * Throws UnknownHeaderException if header type couldn't be determined
 */
class HeaderHelper
{
public:
  /**
     @brief enum for Ndn packet types
   */
  enum Type {INTEREST_CCNB, CONTENT_OBJECT_CCNB,
             INTEREST_NDNSIM, CONTENT_OBJECT_NDNSIM};

  /**
   *	Packet ::= Version
   *		   PacketType
   *		   (Interest | Data)
   *
   *        0                   1
   *        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
   *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   *        |    Version    |   PacketType  |
   *        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   *
   * For ccnb-encoding compatibility, ``Version`` / ``PacketType`` has two reserved values to denote ccnb-encoded packet:
   *
   * Version 0x01, PacketType 0xD2 --- ccnb-encoded ``Interest`` packet
   * Version 0x04, PacketType 0x82 --- ccnb-encoded ``Data`` packet
   *
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
  GetNdnHeaderType (Ptr<const Packet> packet);
};

  /**
   * \brief Exception thrown if NDN stack receives unrecognized message type
   */
class UnknownHeaderException {};

} // namespace ndn
} // namespace ns3

#endif // _NDN_HEADER_HELPER_H_
