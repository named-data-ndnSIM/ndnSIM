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

#include "content-type-visitor.h"
#include "../syntax-tree/blob.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

boost::any
ContentTypeVisitor::visit (Blob &n) 
{
  // Buffer n.m_blob;
  if (n.m_blobSize != 3)
    throw CcnbDecodingException ();

  uint32_t type =
    (n.m_blob [0] << 16) |
    (n.m_blob [1] << 8 ) |
    (n.m_blob [2]      );
    
  return boost::any (type);
}

boost::any
ContentTypeVisitor::visit (Udata &n)
{
  // std::string n.m_udata;
  throw CcnbDecodingException ();
}

} // CcnbParser
} // wire

NDN_NAMESPACE_END
