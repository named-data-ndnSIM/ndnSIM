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

#ifndef _CCNB_PARSER_VOID_DEPTH_FIRST_VISITOR_H_
#define _CCNB_PARSER_VOID_DEPTH_FIRST_VISITOR_H_

#include "void-visitor.h"

NDN_NAMESPACE_BEGIN

namespace wire {
namespace CcnbParser {

/**
 * \ingroup ccnx-ccnb
 * \brief Depth-first visitor that takes one argument and returns nothing
 */
class VoidDepthFirstVisitor : public VoidVisitor
{
public:
  virtual void visit (Blob&,  boost::any);
  virtual void visit (Udata&, boost::any);
  virtual void visit (Tag&,   boost::any);
  virtual void visit (Attr&,  boost::any);
  virtual void visit (Dtag&,  boost::any);
  virtual void visit (Dattr&, boost::any);
  virtual void visit (Ext&,   boost::any);
};

} // CcnbParser
} // wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_VOID_DEPTH_FIRST_VISITOR_H_
