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

#include "interest-visitor.h"

#include "../syntax-tree/block.h"
#include "../syntax-tree/dtag.h"

#include "ns3/ndn-name.h"
#include "ns3/ndn-interest-header.h"

#include "ns3/assert.h"
#include "ns3/nstime.h"

#include "name-components-visitor.h"
#include "non-negative-integer-visitor.h"
#include "timestamp-visitor.h"
#include "uint32t-blob-visitor.h"

#include <boost/foreach.hpp>

#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("ndn.CcnbParser.InterestVisitor");

namespace ns3 {
namespace ndn {
namespace CcnbParser {

// We don't care about any other fields
void
InterestVisitor::visit (Dtag &n, boost::any param/*should be Interest* */)
{
  // uint32_t n.m_dtag;
  // std::list<Ptr<Block> > n.m_nestedBlocks;

  static NonNegativeIntegerVisitor nonNegativeIntegerVisitor;
  static NameVisitor     nameComponentsVisitor;
  static TimestampVisitor          timestampVisitor;
  static Uint32tBlobVisitor        nonceVisitor;
  
  Interest &interest = *(boost::any_cast<Interest*> (param));

  switch (n.m_dtag)
    {
    case CCN_DTAG_Interest:
      NS_LOG_DEBUG ("Interest");
  
      // process nested blocks
      BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
        {
          block->accept (*this, param);
        }
      break;
    case CCN_DTAG_Name:
      {
        NS_LOG_DEBUG ("Name");

        // process name components
        Ptr<Name> name = Create<Name> ();
        
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (nameComponentsVisitor, &(*name));
          }
        interest.SetName (name);
        break;
      }
    case CCN_DTAG_MinSuffixComponents:
      NS_LOG_DEBUG ("MinSuffixComponents");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetMinSuffixComponents (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_MaxSuffixComponents:
      NS_LOG_DEBUG ("MaxSuffixComponents");
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
        NS_LOG_DEBUG ("Exclude");
        // process exclude components
        Ptr<Name> exclude = Create<Name> ();
        
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (nameComponentsVisitor, &(*exclude));
          }
        interest.SetExclude (exclude);
        break;
      }
    case CCN_DTAG_ChildSelector:
      NS_LOG_DEBUG ("ChildSelector");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();

      interest.SetChildSelector (
               1 == boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_AnswerOriginKind:
      NS_LOG_DEBUG ("AnswerOriginKind");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetAnswerOriginKind (
               1 == boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_Scope: 
      NS_LOG_DEBUG ("Scope");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
      interest.SetScope (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CCN_DTAG_InterestLifetime:
      NS_LOG_DEBUG ("InterestLifetime");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();

      interest.SetInterestLifetime (
               boost::any_cast<Time> (
                                      (*n.m_nestedTags.begin())->accept(
                                                                        timestampVisitor
                                                                        )));
      break;
    case CCN_DTAG_Nonce:
      NS_LOG_DEBUG ("Nonce");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();

      interest.SetNonce (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonceVisitor
                                                                           )));
      break;
    
            
    case CCN_DTAG_Nack:
      NS_LOG_DEBUG ("Nack");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbDecodingException ();
            
      interest.SetNack (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(nonNegativeIntegerVisitor)));
      break;
    }
}

} // namespace CcnbParser
} // namespace ndn
} // namespace ns3
