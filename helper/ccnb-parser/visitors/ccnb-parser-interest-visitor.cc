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

#include "ccnb-parser-interest-visitor.h"

#include "ns3/ccnb-parser-block.h"
#include "ns3/ccnb-parser-dtag.h"
#include "ns3/ccnx-name-components.h"
#include "ns3/assert.h"
#include "ns3/nstime.h"

#include "ns3/ccnx-interest-header.h"
#include "ccnb-parser-name-components-visitor.h"
#include "ccnb-parser-non-negative-integer-visitor.h"
#include "ccnb-parser-timestamp-visitor.h"
#include "ccnb-parser-nonce-visitor.h"

#include <boost/foreach.hpp>

namespace ns3 {
namespace CcnbParser {

// We don't care about any other fields
void
InterestVisitor::visit (Dtag &n, boost::any param/*should be CcnxInterestHeader&*/)
{
  // uint32_t n.m_dtag;
  // std::list<Ptr<Block> > n.m_nestedBlocks;

  static NonNegativeIntegerVisitor nonNegativeIntegerVisitor;
  static NameComponentsVisitor     nameComponentsVisitor;
  static TimestampVisitor          timestampVisitor;
  static NonceVisitor              nonceVisitor;
  
  CcnxInterestHeader &interest = boost::any_cast<CcnxInterestHeader&> (param);
  
  switch (n.m_dtag)
    {
    case CCN_DTAG_Interest:
      // process nested blocks
      BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
        {
          block->accept (*this, param);
        }
      break;
    case CCN_DTAG_Name:
      {
        // process name components
        Ptr<CcnxNameComponents> name = Create<CcnxNameComponents> ();
        
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (nameComponentsVisitor, *name);
          }
        interest.SetName (name);
        break;
      }
    case CCN_DTAG_MinSuffixComponents:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetMinSuffixComponents (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_MaxSuffixComponents:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetMaxSuffixComponents (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_Exclude:
      {
        // process exclude components
        Ptr<CcnxNameComponents> exclude = Create<CcnxNameComponents> ();
        
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (nameComponentsVisitor, *exclude);
          }
        interest.SetExclude (exclude);
        break;
      }
    case CCN_DTAG_ChildSelector:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();

      interest.SetChildSelector (
               1 == boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_AnswerOriginKind:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetAnswerOriginKind (
               1 == boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_Scope: 
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetScope (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_InterestLifetime:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();

      interest.SetInterestLifetime (
               boost::any_cast<Time> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           timestampVisitor
                                                                           )));
      break;
    case CCN_DTAG_Nonce:
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();

      interest.SetNonce (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonceVisitor
                                                                           )));
      break;
    }
}

} // namespace CcnbParser
} // namespace ns3
