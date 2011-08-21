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

#include "ccnb-parser-block.h"
#include "ns3/ccnb-parser-common.h"

namespace ns3 {
namespace CcnbParser {

const uint8_t CCN_TT_BITS = 3;
const uint8_t CCN_TT_MASK = ((1 << CCN_TT_BITS) - 1);
const uint8_t CCN_MAX_TINY= ((1 << (7-CCN_TT_BITS)) - 1);
const uint8_t CCN_TT_HBIT = ((uint8_t)(1 << 7));

Ptr<Block> Block::ParseBlock (Buffer::Iterator &start)
{
  uint32_t value = 0;

  // We will have problems if length field is more than 32 bits. Though it's really impossible
  uint8_t byte = 0;
  while (!(byte & CCN_TT_HBIT))
    {
      value <<= 8;
      value += byte;
      byte = start.ReadU8 ();
    }
  value <<= 4;
  value += ( (byte&(~CCN_TT_HBIT)) >> 3);

  switch (byte & CCN_TT_MASK)
    {
    case CCN_BLOB:
      return Create<Blob> (start, value);
    case CCN_UDATA:
      return Create<Udata> (start, value);
    case CCN_TAG:
      return Create<Tag> (start, value);
    case CCN_ATTR:
      return Create<Attr> (start, value);
    case CCN_DTAG:
      return Create<Dtag> (start, value);
    case CCN_DATTR:
      return Create<Dattr> (start, value);
    case CCN_EXT:
      return Create<Ext> (start, value);
    default:
      throw CcnxDecodingException ();
    }
}

}
}
