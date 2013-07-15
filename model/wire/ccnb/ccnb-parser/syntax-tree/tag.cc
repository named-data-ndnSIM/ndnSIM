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

#include "tag.h"

#include "base-attr.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

Tag::Tag (Buffer::Iterator &start, uint32_t length)
{
  m_tag.reserve (length+2); // extra byte for potential \0 at the end
  uint32_t i = 0;
  for (; !start.IsEnd () && i < (length+1); i++)
    {
      m_tag.push_back (start.ReadU8 ());
    }
  if (i < (length+1) && start.IsEnd ())
    throw CcnbDecodingException ();
  
  // parse attributes until first nested block reached
  while (!start.IsEnd () && BufferIteratorPeekU8 (start)!=CCN_CLOSE)
    {
      Ptr<Block> block = Block::ParseBlock (start);
      if (DynamicCast<BaseAttr> (block)!=0)
		m_attrs.push_back (block);
	  else
		{
		  m_nestedTags.push_back (block);
		  break;
		}
	}

  // parse the rest of nested blocks
  while (!start.IsEnd () && BufferIteratorPeekU8 (start)!=CCN_CLOSE)
    {
      Ptr<Block> block = Block::ParseBlock (start);
	  m_nestedTags.push_back (block);
    }
  
  if (start.IsEnd ()) //should not be the end
      throw CcnbDecodingException ();

  start.ReadU8 (); // read CCN_CLOSE
}

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END
