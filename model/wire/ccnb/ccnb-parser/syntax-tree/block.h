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

#ifndef _CCNB_PARSER_BLOCK_H_
#define _CCNB_PARSER_BLOCK_H_

#include "ns3/simple-ref-count.h"
#include "ns3/buffer.h"
#include "ns3/ptr.h"

// visitors
#include "../visitors/void-no-argu-visitor.h"
#include "../visitors/void-visitor.h"
#include "../visitors/no-argu-visitor.h"
#include "../visitors/visitor.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Base class for ccnb-encoded node
 *
 * This class provides a static method to create a new block
 * (recursively) from the stream
 *
 * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
 */
class Block : public SimpleRefCount<Block>
{
public:
  // static int counter;
  /**
   * \brief Parsing stream (recursively) and creating a parsed BLOCK
   * object
   *
   * \param start buffer iterator pointing to the start position for parsing
   * \param dontParseBlock parameter to indicate whether the block should not be parsed, just length
   *                       of the block should be consumed (e.g., in case of "cheating" with content of Data packets)
   * \returns parsed ccnb-encoded block, that could contain more block inside
   */
  static Ptr<Block>
  ParseBlock (Buffer::Iterator &start, bool dontParseBlock = false);

  virtual ~Block ();
  
  virtual void accept( VoidNoArguVisitor &v )               = 0; ///< @brief Accept visitor void(*)()
  virtual void accept( VoidVisitor &v, boost::any param )   = 0; ///< @brief Accept visitor void(*)(boost::any)
  virtual boost::any accept( NoArguVisitor &v )             = 0; ///< @brief Accept visitor boost::any(*)()
  virtual boost::any accept( Visitor &v, boost::any param ) = 0; ///< @brief Accept visitor boost::any(*)(boost::any)
};

/**
 * @brief Necessary until Buffer::Iterator gets PeekU8 call
 * @param i buffer iterator
 * @return peeked uint8_t value
 */
inline
uint8_t
BufferIteratorPeekU8 (Buffer::Iterator &i)
{
  uint8_t ret = i.ReadU8 ();
  i.Prev ();
  return ret;
}

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_BLOCK_H_
