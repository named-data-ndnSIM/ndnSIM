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

#include "block.h"

#include "blob.h"
#include "udata.h"
#include "tag.h"
#include "dtag.h"
#include "attr.h"
#include "dattr.h"
#include "ext.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ndn.CcnbParser.Block");

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/// @cond include_hidden
const uint8_t CCN_TT_BITS = 3;
const uint8_t CCN_TT_MASK = ((1 << CCN_TT_BITS) - 1);
const uint8_t CCN_MAX_TINY= ((1 << (7-CCN_TT_BITS)) - 1);
const uint8_t CCN_TT_HBIT = ((uint8_t)(1 << 7));
/// @endcond

// int Block::counter = 0;

Ptr<Block> Block::ParseBlock (Buffer::Iterator &start, bool dontParseBlock)
{
  // std::cout << "<< pos: " << counter << "\n";
  uint32_t value = 0;

  // We will have problems if length field is more than 32 bits. Though it's really impossible
  uint8_t byte = 0;
  while (!start.IsEnd() && !(byte & CCN_TT_HBIT))
    {
      value <<= 7;
      value += byte;
      byte = start.ReadU8 ();
      // Block::counter ++;
    }
  if (start.IsEnd())
    CcnbDecodingException ();

  if (dontParseBlock)
    {
      return 0;
    }
  
  value <<= 4;
  value += ( (byte&(~CCN_TT_HBIT)) >> 3);
  
  /**
   * Huh. After fighting with NS-3, it became apparent that Create<T>(...) construct
   * doesn't work with references.  Just simply doesn't work.  wtf?
   */
  switch (byte & CCN_TT_MASK)
    {
    case CCN_BLOB:
      return Ptr<Blob> (new Blob(start, value), false);
    case CCN_UDATA:
      return Ptr<Udata> (new Udata(start, value), false);
    case CCN_TAG:
      return Ptr<Tag> (new Tag(start, value), false);
    case CCN_ATTR:
      return Ptr<Attr> (new Attr(start, value), false);
    case CCN_DTAG:
      return Ptr<Dtag> (new Dtag(start, value), false);
    case CCN_DATTR:
      return Ptr<Dattr> (new Dattr(start, value), false);
    case CCN_EXT:
      return Ptr<Ext> (new Ext(start, value), false);
    default:
      throw CcnbDecodingException ();
    }
}

Block::~Block ()
{
}

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END
