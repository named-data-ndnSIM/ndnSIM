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

#include "ccnx-decoding-helper.h"

#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

#include "ccnb-parser/visitors/ccnb-parser-interest-visitor.h"
#include "ccnb-parser/visitors/ccnb-parser-content-object-visitor.h"

#include "ccnb-parser/syntax-tree/ccnb-parser-block.h"
#include "ccnb-parser/syntax-tree/ccnb-parser-dtag.h"

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("CcnxDecodingHelper");

namespace ns3 {

size_t
CcnxDecodingHelper::Deserialize (Buffer::Iterator start, CcnxInterestHeader &interest)
{
  static CcnbParser::InterestVisitor interestVisitor;

  Buffer::Iterator i = start;
  Ptr<CcnbParser::Block> root = CcnbParser::Block::ParseBlock (i);
  root->accept (interestVisitor, &interest);
  
  return i.GetDistanceFrom (start);
}

size_t
CcnxDecodingHelper::Deserialize (Buffer::Iterator start, CcnxContentObjectHeader &contentObject)
{
  static CcnbParser::ContentObjectVisitor contentObjectVisitor;

  Buffer::Iterator i = start;
  Ptr<CcnbParser::Block> root = CcnbParser::Block::ParseBlock (i);
  root->accept (contentObjectVisitor, &contentObject);

  return i.GetDistanceFrom (start);
}

} // namespace ns3
