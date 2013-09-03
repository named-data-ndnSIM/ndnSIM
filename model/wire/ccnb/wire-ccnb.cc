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

#include "wire-ccnb.h"

#include <sstream>
#include <boost/foreach.hpp>
#include "ccnb-parser/common.h"
#include "ccnb-parser/visitors/name-visitor.h"
#include "ccnb-parser/syntax-tree/block.h"

NDN_NAMESPACE_BEGIN

namespace wire {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define CCN_TT_BITS 3
#define CCN_TT_MASK ((1 << CCN_TT_BITS) - 1)
#define CCN_MAX_TINY ((1 << (7-CCN_TT_BITS)) - 1)
#define CCN_TT_HBIT ((unsigned char)(1 << 7))

size_t
Ccnb::AppendBlockHeader (Buffer::Iterator &start, size_t val, uint32_t tt)
{
  unsigned char buf[1+8*((sizeof(val)+6)/7)];
  unsigned char *p = &(buf[sizeof(buf)-1]);
  size_t n = 1;
  p[0] = (CCN_TT_HBIT & ~CcnbParser::CCN_CLOSE) |
  ((val & CCN_MAX_TINY) << CCN_TT_BITS) |
  (CCN_TT_MASK & tt);
  val >>= (7-CCN_TT_BITS);
  while (val != 0) {
    (--p)[0] = (((unsigned char)val) & ~CCN_TT_HBIT) | CcnbParser::CCN_CLOSE;
    n++;
    val >>= 7;
  }
  start.Write (p,n);
  return n;
}

size_t
Ccnb::EstimateBlockHeader (size_t value)
{
  value >>= (7-CCN_TT_BITS);
  size_t n = 1;
  while (value>0)
    {
      value >>= 7;
      n++;
    }
  return n;
}

size_t
Ccnb::AppendNumber (Buffer::Iterator &start, uint32_t number)
{
  std::ostringstream os;
  os << number;

  size_t written = 0;
  written += AppendBlockHeader (start, os.str().size(), CcnbParser::CCN_UDATA);
  written += os.str().size();
  start.Write (reinterpret_cast<const unsigned char*>(os.str().c_str()), os.str().size());

  return written;
}

size_t
Ccnb::EstimateNumber (uint32_t number)
{
  std::ostringstream os;
  os << number;
  return EstimateBlockHeader (os.str ().size ()) + os.str ().size ();
}

size_t
Ccnb::AppendCloser (Buffer::Iterator &start)
{
  start.WriteU8 (CcnbParser::CCN_CLOSE);
  return 1;
}

size_t
Ccnb::AppendTimestampBlob (Buffer::Iterator &start, const Time &time)
{
  // the original function implements Markers... thought not sure what are these markers for...

  // Determine miminal number of bytes required to store the timestamp
  int required_bytes = 2; // 12 bits for fractions of a second, 4 bits left for seconds. Sometimes it is enough
  intmax_t ts = time.ToInteger (Time::S) >> 4;
  for (;  required_bytes < 7 && ts != 0; ts >>= 8) // not more than 6 bytes?
     required_bytes++;

  size_t len = AppendBlockHeader(start, required_bytes, CcnbParser::CCN_BLOB);

  // write part with seconds
  ts = time.ToInteger (Time::S) >> 4;
  for (int i = 0; i < required_bytes - 2; i++)
    start.WriteU8 ( ts >> (8 * (required_bytes - 3 - i)) );

  /* arithmetic contortions are to avoid overflowing 31 bits */
  ts = ((time.ToInteger (Time::S) & 15) << 12) +
       (((time.ToInteger (Time::NS) % 1000000000) / 5 * 8 + 195312) / 390625);
  for (int i = required_bytes - 2; i < required_bytes; i++)
    start.WriteU8 ( ts >> (8 * (required_bytes - 1 - i)) );

  return len + required_bytes;
}

size_t
Ccnb::EstimateTimestampBlob (const Time &time)
{
  int required_bytes = 2; // 12 bits for fractions of a second, 4 bits left for seconds. Sometimes it is enough
  intmax_t ts = time.ToInteger (Time::S) >> 4;
  for (;  required_bytes < 7 && ts != 0; ts >>= 8) // not more than 6 bytes?
     required_bytes++;

  return EstimateBlockHeader (required_bytes) + required_bytes;
}

size_t
Ccnb::AppendTaggedBlob (Buffer::Iterator &start, uint32_t dtag,
                  const uint8_t *data, size_t size)
{
  size_t written = AppendBlockHeader (start, dtag, CcnbParser::CCN_DTAG);
  /* 2 */
  if (size>0)
    {
      written += AppendBlockHeader (start, size, CcnbParser::CCN_BLOB);
      start.Write (data, size);
      written += size;
      /* size */
    }
  written += AppendCloser (start);
  /* 1 */

  return written;
}

size_t
Ccnb::AppendTaggedBlobWithPadding (Buffer::Iterator &start, uint32_t dtag,
                                   uint32_t length,
                                   const uint8_t *data, size_t size)
{
  if (size > length)
    {
      // no padding required
      return AppendTaggedBlob (start, dtag, data, size);
    }


  size_t written = AppendBlockHeader (start, dtag, CcnbParser::CCN_DTAG);

  /* 2 */
  if (length>0)
    {
      written += AppendBlockHeader (start, length, CcnbParser::CCN_BLOB);
      start.Write (data, size);
      start.WriteU8 (0, length - size);
      written += length;
      /* size */
    }
  written += AppendCloser (start);
  /* 1 */

  return written;
}

size_t
Ccnb::EstimateTaggedBlob (uint32_t dtag, size_t size)
{
  if (size>0)
    return EstimateBlockHeader (dtag) + EstimateBlockHeader (size) + size + 1;
  else
    return EstimateBlockHeader (dtag) + 1;
}

size_t
Ccnb::AppendString (Buffer::Iterator &start, uint32_t dtag,
                                  const std::string &string)
{
  size_t written = AppendBlockHeader (start, dtag, CcnbParser::CCN_DTAG);
  {
    written += AppendBlockHeader (start, string.size (), CcnbParser::CCN_UDATA);
    start.Write (reinterpret_cast<const uint8_t*> (string.c_str ()), string.size ());
    written += string.size ();
  }
  written += AppendCloser (start);

  return written;
}

size_t
Ccnb::EstimateString (uint32_t dtag, const std::string &string)
{
  return EstimateBlockHeader (dtag) + EstimateBlockHeader (string.size ()) + string.size () + 1;
}

size_t
Ccnb::SerializeName (Buffer::Iterator &start, const Name &name)
{
  size_t written = 0;
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG);
  BOOST_FOREACH (const name::Component &component, name)
    {
      written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_Component,
                                   reinterpret_cast<const uint8_t*>(component.buf ()), component.size());
    }
  written += AppendCloser (start);
  return written;
}

size_t
Ccnb::SerializedSizeName (const Name &name)
{
  size_t written = 0;
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Name);
  BOOST_FOREACH (const name::Component &component, name)
    {
      written += EstimateTaggedBlob (CcnbParser::CCN_DTAG_Component, component.size ());
    }
  written += 1;
  return written;
}

Ptr<Name>
Ccnb::DeserializeName (Buffer::Iterator &i)
{
  Ptr<Name> name = Create<Name> ();
  CcnbParser::NameVisitor nameVisitor;

  Ptr<CcnbParser::Block> root = CcnbParser::Block::ParseBlock (i);
  root->accept (nameVisitor, GetPointer (name));

  return name;
}

} // wire

NDN_NAMESPACE_END
