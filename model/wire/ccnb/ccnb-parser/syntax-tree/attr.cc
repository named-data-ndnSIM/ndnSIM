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

#include "attr.h"
#include "../common.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

// length length in octets of UTF-8 encoding of tag name - 1 (minimum tag name length is 1) 
Attr::Attr (Buffer::Iterator &start, uint32_t length)
{
  m_attr.reserve (length+2); // extra byte for potential \0 at the end
  uint32_t i = 0;
  for (; !start.IsEnd () && i < (length+1); i++)
    {
      m_attr.push_back (start.ReadU8 ());
    }
  if (i < (length+1) && start.IsEnd ())
    throw CcnbDecodingException ();
  m_value = DynamicCast<Udata> (Block::ParseBlock (start));
  if (m_value == 0)
    throw CcnbDecodingException (); // "ATTR must be followed by UDATA field"
}

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END
