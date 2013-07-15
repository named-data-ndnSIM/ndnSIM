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

#ifndef _CCNB_PARSER_BLOB_H_
#define _CCNB_PARSER_BLOB_H_

#include "block.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Class to represent BLOB ccnb-encoded node
 *
 * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
 */
class Blob : public Block
{
public:
  /**
   * \brief Constructor that actually parsed ccnb-encoded BLOB block
   *
   * \param start  buffer iterator pointing to the first byte of BLOB data in ccnb-encoded block 
   * \param length length of data in BLOB block (extracted from the value field)
   *
   * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
   */
  Blob (Buffer::Iterator &start, uint32_t length);
  ~Blob ();
  
  virtual void accept( VoidNoArguVisitor &v )               { v.visit( *this ); }
  virtual void accept( VoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( NoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( Visitor &v, boost::any param ) { return v.visit( *this, param ); }

  char* m_blob; ///< \brief field holding a parsed BLOB value of the block
  uint32_t  m_blobSize; ///< @brief field representing size of the BLOB field stored
};

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_BLOB_H_
