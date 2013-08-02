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

#include "dtag.h"

#include "base-attr.h"
#include "base-tag.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

Dtag::Dtag (Buffer::Iterator &start, uint32_t dtag)
{
  m_dtag = dtag;
  // std::cout << m_dtag << ", position: " << Block::counter << "\n";  
  /**
   * Hack
   *
   * Stop processing after encountering "Content" dtag.  Actual
   * content (including virtual payload) will be stored in Packet
   * buffer
   */
  if (dtag == CCN_DTAG_Content)
    {
      Block::ParseBlock (start, true); // process length field and ignore it
      return; // hack #1. Do not process nesting block for <Content>
    }
  
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
      // hack #2. Stop processing nested blocks if last block was <Content>
      if (m_dtag == CCN_DTAG_Data && // we are in <Data>
          DynamicCast<Dtag> (m_nestedTags.back())!=0 && // last block is DTAG
          DynamicCast<Dtag> (m_nestedTags.back())->m_dtag == CCN_DTAG_Content) 
        {
          return; 
        }

      m_nestedTags.push_back (Block::ParseBlock (start));
    }

  // hack #3. Stop processing when last tag was <Data>
  if (m_dtag == CCN_DTAG_Data && // we are in <Data>
      DynamicCast<Dtag> (m_nestedTags.back())!=0 && // last block is DTAG
      DynamicCast<Dtag> (m_nestedTags.back())->m_dtag == CCN_DTAG_Content) 
    {
      return; 
    }

  if (start.IsEnd ())
      throw CcnbDecodingException ();

  start.ReadU8 (); // read CCN_CLOSE
  // std::cout << "closer, position = " << Block::counter << "\n";
  // Block::counter ++;
}

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END
