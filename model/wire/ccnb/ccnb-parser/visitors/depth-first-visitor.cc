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

#include "depth-first-visitor.h"

#include "../syntax-tree/blob.h"
#include "../syntax-tree/udata.h"
#include "../syntax-tree/tag.h"
#include "../syntax-tree/dtag.h"
#include "../syntax-tree/attr.h"
#include "../syntax-tree/dattr.h"
#include "../syntax-tree/ext.h"

#include <boost/foreach.hpp>

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

boost::any
DepthFirstVisitor::visit (Blob &n, boost::any param)
{
  // Buffer n.m_blob;
  return n.m_blob;
}
 
boost::any
DepthFirstVisitor::visit (Udata &n, boost::any param)
{
  // std::string n.m_udata;
  return n.m_udata;
}
 
boost::any
DepthFirstVisitor::visit (Tag &n, boost::any param)
{
  // std::string n.m_tag;
  // std::list<Ptr<Block> > n.m_attrs;
  // std::list<Ptr<Block> > n.m_nestedBlocks;
  BOOST_FOREACH (Ptr<Block> block, n.m_attrs)
    {
      block->accept (*this, param);
    }
  BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
    {
      block->accept (*this, param);
    }
  return boost::any();
}

boost::any
DepthFirstVisitor::visit (Dtag &n, boost::any param)
{
  // std::string n.m_tag;
  // std::list<Ptr<Block> > n.m_attrs;
  // std::list<Ptr<Block> > n.m_nestedBlocks;
  BOOST_FOREACH (Ptr<Block> block, n.m_attrs)
    {
      block->accept (*this, param);
    }
  BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
    {
      block->accept (*this, param);
    }
  return boost::any ();
}

boost::any
DepthFirstVisitor::visit (Attr &n, boost::any param)
{
  // std::string n.m_attr;
  // Ptr<Udata> n.m_value;
  return boost::any ();
}

boost::any
DepthFirstVisitor::visit (Dattr &n, boost::any param)
{
  // uint32_t n.m_dattr;
  // Ptr<Udata> n.m_value;
  return boost::any ();
}
 
boost::any
DepthFirstVisitor::visit (Ext &n, boost::any param)
{
  // uint64_t n.m_extSubtype;
  return n.m_extSubtype;
}

} // CcnbParser
} // wire

NDN_NAMESPACE_END
