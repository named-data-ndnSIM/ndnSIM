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

#ifndef _CCNB_PARSER_EXT_H_
#define _CCNB_PARSER_EXT_H_

#include "ccnb-parser-ext.h"

namespace ns3 {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Class to represent EXT ccnb-encoded node
 *
 * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
 */
class Ext : public Block
{
public:
  /**
   * \brief Constructor that actually parsed ccnb-encoded DTAG block
   *
   * \param start buffer iterator pointing to the next byte past EXT block
   * \param extSubtype extension type (extracted from the value field)
   *
   * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
   */
  Ext (Buffer::Iterator &start, uint32_t extSubtype);

  virtual void accept( Visitor &v )                           { v.visit( *this ); }
  virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( GJNoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

  uint64_t m_extSubtype; ///< \brief Extension type
};

}
}

#endif // _CCNB_PARSER_EXT_H_
