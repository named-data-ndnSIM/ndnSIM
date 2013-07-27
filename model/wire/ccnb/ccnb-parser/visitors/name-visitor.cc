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

#include "name-visitor.h"

#include "string-visitor.h"
#include "../syntax-tree/dtag.h"

#include "ns3/ndn-name.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

void
NameVisitor::visit (Dtag &n, boost::any param/*should be Name* */)
{
  // uint32_t n.m_dtag;
  // std::list<Ptr<Block> > n.m_nestedBlocks;
  static StringVisitor stringVisitor; 
 
  Name &components = *(boost::any_cast<Name*> (param));

  switch (n.m_dtag)
    {
    case CCN_DTAG_Component:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      components.append (
                      boost::any_cast<std::string> ((*n.m_nestedTags.begin())->accept(
                                                                                      stringVisitor
                                                                                      )));
      break;
    default:
      VoidDepthFirstVisitor::visit (n, param);
      break;
    }
}

} // CcnbParser
} // wire

NDN_NAMESPACE_END
