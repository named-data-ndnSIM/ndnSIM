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

#include "uint32t-blob-visitor.h"
#include "../syntax-tree/blob.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

boost::any
Uint32tBlobVisitor::visit (Blob &n) 
{
  // Buffer n.m_blob;
  if (n.m_blobSize < 4)
    throw CcnbDecodingException ();
     
  return boost::any (*(reinterpret_cast<uint32_t*> (n.m_blob)));
}

boost::any
Uint32tBlobVisitor::visit (Udata &n)
{
  // std::string n.m_udata;
  throw CcnbDecodingException ();
}

} // CcnbParser
} // wire

NDN_NAMESPACE_END
