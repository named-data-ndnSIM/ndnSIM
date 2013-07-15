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

#include "string-visitor.h"
#include "../syntax-tree/udata.h"
#include "../syntax-tree/blob.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

boost::any
StringVisitor::visit (Blob &n) 
{
  // Buffer n.m_blob;
  return std::string (n.m_blob, n.m_blobSize);
}

boost::any
StringVisitor::visit (Udata &n)
{
  // std::string n.m_udata;
  return n.m_udata;
}

} // CcnbParser
} // wire

NDN_NAMESPACE_END
