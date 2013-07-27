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

#include "udata.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

Udata::Udata (Buffer::Iterator &start, uint32_t length)
{
  // Ideally, the code should look like this. Unfortunately, we don't have normal compatible iterators
  // Buffer::Iterator realStart = start;
  // start.Next (length); // advancing forward
  // m_udata.assign (realStart, start/*actually, it is the end*/);

  m_udata.reserve (length+1); //just in case we will need \0 at the end later
  // this is actually the way Read method is implemented in network/src/buffer.cc
  uint32_t i = 0;
  for (; !start.IsEnd () && i < length; i++)
    {
      m_udata.push_back (start.ReadU8 ());
    }

  if (i < length && start.IsEnd ())
    throw CcnbDecodingException ();
}

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END
