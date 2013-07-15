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

#ifndef _CCNB_PARSER_ATTR_H_
#define _CCNB_PARSER_ATTR_H_

#include "base-attr.h"
#include <string>

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Class to represent ATTR ccnb-encoded node
 *
 * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
 */
class Attr : public BaseAttr
{
public:
  /**
   * \brief Constructor that actually parsed ccnb-encoded ATTR block
   *
   * \param start  buffer iterator pointing to the first byte of ATTR block name
   * \param length length of ATTR name (extracted from the value field)
   *
   * \see http://www.ccnx.org/releases/latest/doc/technical/BinaryEncoding.html
   */
  Attr (Buffer::Iterator &start, uint32_t length);
  
  virtual void accept( VoidNoArguVisitor &v )               { v.visit( *this ); }
  virtual void accept( VoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
  virtual boost::any accept( NoArguVisitor &v )             { return v.visit( *this ); }
  virtual boost::any accept( Visitor &v, boost::any param ) { return v.visit( *this, param ); }

  std::string m_attr; ///< field holding name of the attribute
};

} // namespace CcnbParser
} // namespace wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_ATTR_H_
