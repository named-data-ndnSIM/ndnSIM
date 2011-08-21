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

#include "ccnb-parser-dtag.h"
#include "ns3/ccnb-parser-common.h"

namespace ns3 {
namespace CcnbParser {

Dtag::Dtag (Buffer::Iterator &start, uint32_t dtag)
{
  m_dtag = dtag;

  /**
   * Hack
   *
   * Stop processing after encountering <Content> dtag.  Actual
   * content (including virtual payload) will be stored in Packet
   * buffer
   */
  if (dtag == Ccnx::CCN_DTAG_Content)
    return; // hack #1. Do not process nesting block for <Content>
  
  // parse attributes until first nested block reached
  while (!start.IsEnd () && start.PeekU8 ()!=CCN_CLOSE)
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
  while (!start.IsEnd () && start.PeekU8 ()!=CCN_CLOSE)
    {
      // hack #2. Stop processing nested blocks if last block was <Content>
      if (m_dtag == Ccnx::CCN_DTAG_ContentObject && // we are in <ContentObject>
          DynamicCast<Dtag> (m_nestedBlocks.back())!=0 && // last block is DTAG
          DynamicCast<Dtag> (m_nestedBlocks.back())->m_dtag == CCN_DTAG_Content) 
        {
          return; 
        }

      m_nestedBlocks.push_back (Block::ParseBlock (start));
    }
  if (start.IsEnd ())
      throw CcnxDecodingException ();

  start.ReadU8 (); // read CCN_CLOSE
}

}
}
