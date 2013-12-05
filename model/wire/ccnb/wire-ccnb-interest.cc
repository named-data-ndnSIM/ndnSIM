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
 * Author: Ilya Moiseenko <iliamo@cs.ucla.edu>
 *         Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "../ccnb.h"

#include "wire-ccnb.h"

#include "ns3/log.h"
#include "ns3/unused.h"
#include "ns3/packet.h"

#include "ccnb-parser/visitors/name-visitor.h"
#include "ccnb-parser/visitors/non-negative-integer-visitor.h"
#include "ccnb-parser/visitors/timestamp-visitor.h"
#include "ccnb-parser/visitors/uint32t-blob-visitor.h"

#include "ccnb-parser/syntax-tree/block.h"
#include "ccnb-parser/syntax-tree/dtag.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.wire.Ccnb.Interest");

NDN_NAMESPACE_BEGIN

namespace wire {
namespace ccnb {

NS_OBJECT_ENSURE_REGISTERED (Interest);

TypeId
Interest::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::Interest::Ccnb")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<Interest> ()
    ;
  return tid;
}

TypeId
Interest::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Interest::Interest ()
  : m_interest (Create<ndn::Interest> ())
{
}

Interest::Interest (Ptr<ndn::Interest> interest)
  : m_interest (interest)
{
}

Ptr<ndn::Interest>
Interest::GetInterest ()
{
  return m_interest;
}

Ptr<Packet>
Interest::ToWire (Ptr<const ndn::Interest> interest)
{
  Ptr<const Packet> p = interest->GetWire ();
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*interest->GetPayload ());
      Interest wireEncoding (ConstCast<ndn::Interest> (interest));
      packet->AddHeader (wireEncoding);
      interest->SetWire (packet);

      p = packet;
    }
  
  return p->Copy ();
}

Ptr<ndn::Interest>
Interest::FromWire (Ptr<Packet> packet)
{
  Ptr<ndn::Interest> interest = Create<ndn::Interest> ();
  Ptr<Packet> wire = packet->Copy ();

  Interest wireEncoding (interest);
  packet->RemoveHeader (wireEncoding);

  interest->SetPayload (packet);
  interest->SetWire (wire);

  return interest;
}

void
Interest::Serialize (Buffer::Iterator start) const
{
  Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Interest, CcnbParser::CCN_DTAG); // <Interest>
  
  // Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG); // <Name>
  Ccnb::SerializeName (start, m_interest->GetName());                // <Component>...</Component>...
  // Ccnb::AppendCloser (start);                               // </Name>

  // if (m_interest->GetMinSuffixComponents() >= 0)
  //   {
  //     Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_MinSuffixComponents, CcnbParser::CCN_DTAG);
  //     Ccnb::AppendNumber (start, m_interest->GetMinSuffixComponents ());
  //     Ccnb::AppendCloser (start);
  //   }
  // if (m_interest->GetMaxSuffixComponents() >= 0)
  //   {
  //     Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_MaxSuffixComponents, CcnbParser::CCN_DTAG);
  //     Ccnb::AppendNumber (start, m_interest->GetMaxSuffixComponents ());
  //     Ccnb::AppendCloser (start);
  //   }
  // if (m_interest->IsEnabledExclude() && m_interest->GetExclude().size() > 0)
  //   {
  //     Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Exclude, CcnbParser::CCN_DTAG); // <Exclude>
  //     Ccnb::AppendName (start, m_interest->GetExclude());                // <Component>...</Component>...
  //     Ccnb::AppendCloser (start);                                  // </Exclude>
  //   }
  // if (m_interest->IsEnabledChildSelector())
  //   {
  //     Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_ChildSelector, CcnbParser::CCN_DTAG);
  //     Ccnb::AppendNumber (start, 1);
  //     Ccnb::AppendCloser (start);
  //   }
  // if (m_interest->IsEnabledAnswerOriginKind())
  //   {
  //     Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_AnswerOriginKind, CcnbParser::CCN_DTAG);
  //     Ccnb::AppendNumber (start, 1);
  //     Ccnb::AppendCloser (start);
  //   }
  if (m_interest->GetScope() >= 0)
    {
      Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Scope, CcnbParser::CCN_DTAG);
      Ccnb::AppendNumber (start, m_interest->GetScope ());
      Ccnb::AppendCloser (start);
    }
  if (!m_interest->GetInterestLifetime().IsZero())
    {
      Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_InterestLifetime, CcnbParser::CCN_DTAG);
      Ccnb::AppendTimestampBlob (start, m_interest->GetInterestLifetime ());
      Ccnb::AppendCloser (start);
    }
  if (m_interest->GetNonce()>0)
    {
      uint32_t nonce = m_interest->GetNonce();
      Ccnb::AppendTaggedBlob (start, CcnbParser::CCN_DTAG_Nonce, nonce);
    }
    
  if (m_interest->GetNack ()>0)
    {
      Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Nack, CcnbParser::CCN_DTAG);
      Ccnb::AppendNumber (start, m_interest->GetNack ());
      Ccnb::AppendCloser (start);
    }
  Ccnb::AppendCloser (start); // </Interest>
}

uint32_t
Interest::GetSerializedSize () const
{
  size_t written = 0;
  written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Interest); // <Interest>
  
  // written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Name); // <Name>
  written += Ccnb::SerializedSizeName (m_interest->GetName()); // <Component>...</Component>...
  // written += 1; // </Name>

  // if (m_interest->GetMinSuffixComponents() >= 0)
  //   {
  //     written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_MinSuffixComponents);
  //     written += Ccnb::EstimateNumber (m_interest->GetMinSuffixComponents ());
  //     written += 1;
  //   }
  // if (m_interest->GetMaxSuffixComponents() >= 0)
  //   {
  //     written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_MaxSuffixComponents);
  //     written += Ccnb::EstimateNumber (m_interest->GetMaxSuffixComponents ());
  //     written += 1;
  //   }
  // if (m_interest->IsEnabledExclude() && m_interest->GetExclude().size() > 0)
  //   {
  //     written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Exclude);
  //     written += Ccnb::EstimateName (m_interest->GetExclude());                // <Component>...</Component>...
  //     written += 1;                                  // </Exclude>
  //   }
  // if (m_interest->IsEnabledChildSelector())
  //   {
  //     written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_ChildSelector);
  //     written += Ccnb::EstimateNumber (1);
  //     written += 1;
  //   }
  // if (m_interest->IsEnabledAnswerOriginKind())
  //   {
  //     written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_AnswerOriginKind);
  //     written += Ccnb::EstimateNumber (1);
  //     written += 1;
  //   }
  if (m_interest->GetScope() >= 0)
    {
      written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Scope);
      written += Ccnb::EstimateNumber (m_interest->GetScope ());
      written += 1;
    }
  if (!m_interest->GetInterestLifetime().IsZero())
    {
      written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_InterestLifetime);
      written += Ccnb::EstimateTimestampBlob (m_interest->GetInterestLifetime());
      written += 1;
    }
  if (m_interest->GetNonce()>0)
    {
      written += Ccnb::EstimateTaggedBlob (CcnbParser::CCN_DTAG_Nonce, sizeof(uint32_t));
    }
  if (m_interest->GetNack ()>0)
    {
        written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Nack);
        written += Ccnb::EstimateNumber (m_interest->GetNack ());
        written += 1;
    }

  written += 1; // </Interest>

  return written;
}

class InterestVisitor : public CcnbParser::VoidDepthFirstVisitor
{
public:
  virtual void visit (CcnbParser::Dtag &n, boost::any param/*should be CcnxInterest* */);
};

// We don't care about any other fields
void
InterestVisitor::visit (CcnbParser::Dtag &n, boost::any param/*should be Interest* */)
{
  // uint32_t n.m_dtag;
  // std::list<Ptr<Block> > n.m_nestedBlocks;

  static CcnbParser::NonNegativeIntegerVisitor nonNegativeIntegerVisitor;
  static CcnbParser::NameVisitor               nameVisitor;
  static CcnbParser::TimestampVisitor          timestampVisitor;
  static CcnbParser::Uint32tBlobVisitor        nonceVisitor;
  
  ndn::Interest &interest = *(boost::any_cast<ndn::Interest*> (param));

  switch (n.m_dtag)
    {
    case CcnbParser::CCN_DTAG_Interest:
      NS_LOG_DEBUG ("Interest");
  
      // process nested blocks
      BOOST_FOREACH (Ptr<CcnbParser::Block> block, n.m_nestedTags)
        {
          block->accept (*this, param);
        }
      break;
    case CcnbParser::CCN_DTAG_Name:
      {
        NS_LOG_DEBUG ("Name");

        // process name components
        Ptr<Name> name = Create<Name> ();
        n.accept (nameVisitor, GetPointer (name));
        interest.SetName (name);
        break;
      }
    // case CcnbParser::CCN_DTAG_MinSuffixComponents:
    //   NS_LOG_DEBUG ("MinSuffixComponents");
    //   if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
    //     throw CcnbParser::CcnbDecodingException ();
    //   interest.SetMinSuffixComponents (
    //            boost::any_cast<uint32_t> (
    //                                       (*n.m_nestedTags.begin())->accept(
    //                                                                        nonNegativeIntegerVisitor
    //                                                                        )));
    //   break;
    // case CcnbParser::CCN_DTAG_MaxSuffixComponents:
    //   NS_LOG_DEBUG ("MaxSuffixComponents");
    //   if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
    //     throw CcnbParser::CcnbDecodingException ();
    //   interest.SetMaxSuffixComponents (
    //            boost::any_cast<uint32_t> (
    //                                       (*n.m_nestedTags.begin())->accept(
    //                                                                        nonNegativeIntegerVisitor
    //                                                                        )));
    //   break;
    // case CcnbParser::CCN_DTAG_Exclude:
    //   {
    //     NS_LOG_DEBUG ("Exclude");
    //     // process exclude components
    //     Ptr<Name> exclude = Create<Name> ();
        
    //     BOOST_FOREACH (Ptr<CcnbParser::Block> block, n.m_nestedTags)
    //       {
    //         block->accept (nameVisitor, &(*exclude));
    //       }
    //     interest.SetExclude (exclude);
    //     break;
    //   }
    // case CcnbParser::CCN_DTAG_ChildSelector:
    //   NS_LOG_DEBUG ("ChildSelector");
    //   if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
    //     throw CcnbParser::CcnbDecodingException ();

    //   interest.SetChildSelector (
    //            1 == boost::any_cast<uint32_t> (
    //                                       (*n.m_nestedTags.begin())->accept(
    //                                                                        nonNegativeIntegerVisitor
    //                                                                        )));
    //   break;
    // case CCN_DTAG_AnswerOriginKind:
    //   NS_LOG_DEBUG ("AnswerOriginKind");
    //   if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
    //     throw CcnbParser::CcnbDecodingException ();
    //   interest.SetAnswerOriginKind (
    //            1 == boost::any_cast<uint32_t> (
    //                                       (*n.m_nestedTags.begin())->accept(
    //                                                                        nonNegativeIntegerVisitor
    //                                                                        )));
    //   break;
    case CcnbParser::CCN_DTAG_Scope: 
      NS_LOG_DEBUG ("Scope");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbParser::CcnbDecodingException ();
      interest.SetScope (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonNegativeIntegerVisitor
                                                                           )));
      break;
    case CcnbParser::CCN_DTAG_InterestLifetime:
      NS_LOG_DEBUG ("InterestLifetime");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbParser::CcnbDecodingException ();

      interest.SetInterestLifetime (
               boost::any_cast<Time> (
                                      (*n.m_nestedTags.begin())->accept(
                                                                        timestampVisitor
                                                                        )));
      break;
    case CcnbParser::CCN_DTAG_Nonce:
      NS_LOG_DEBUG ("Nonce");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbParser::CcnbDecodingException ();

      interest.SetNonce (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(
                                                                           nonceVisitor
                                                                           )));
      break;
    
            
    case CcnbParser::CCN_DTAG_Nack:
      NS_LOG_DEBUG ("Nack");
      if (n.m_nestedTags.size()!=1) // should be exactly one UDATA inside this tag
        throw CcnbParser::CcnbDecodingException ();
            
      interest.SetNack (
               boost::any_cast<uint32_t> (
                                          (*n.m_nestedTags.begin())->accept(nonNegativeIntegerVisitor)));
      break;
    }
}


uint32_t
Interest::Deserialize (Buffer::Iterator start)
{
  static InterestVisitor interestVisitor;

  Buffer::Iterator i = start;
  Ptr<CcnbParser::Block> root = CcnbParser::Block::ParseBlock (i);
  root->accept (interestVisitor, GetPointer (m_interest));
  
  return i.GetDistanceFrom (start);
}

void
Interest::Print (std::ostream &os) const
{
  os << "I: " << m_interest->GetName ();
  
  return;
  // os << "<Interest>\n  <Name>" << GetName () << "</Name>\n";
  // if (GetNack ()>0)
  //   {
  //     os << "  <NACK>";
  //     switch (GetNack ())
  //       {
  //       case NACK_LOOP:
  //         os << "loop";
  //         break;
  //       case NACK_CONGESTION:
  //         os << "congestion";
  //         break;
  //       default:
  //         os << "unknown";
  //         break;
  //       }
  //     os << "</NACK>\n";
  //   }
  // if (GetMinSuffixComponents () >= 0)
  //   os << "  <MinSuffixComponents>" << GetMinSuffixComponents () << "</MinSuffixComponents>\n";
  // if (GetMaxSuffixComponents () >= 0)
  //   os << "  <MaxSuffixComponents>" << m_maxSuffixComponents << "</MaxSuffixComponents>\n";
  // if (IsEnabledExclude () && GetExclude ().size()>0)
  //   os << "  <Exclude>" << GetExclude () << "</Exclude>\n";
  // if (IsEnabledChildSelector ())
  //   os << "  <ChildSelector />\n";
  // if (IsEnabledAnswerOriginKind ())
  //   os << "  <AnswerOriginKind />\n";
  // if (GetScope () >= 0)
  //   os << "  <Scope>" << GetScope () << "</Scope>\n";
  // if ( !GetInterestLifetime ().IsZero() )
  //   os << "  <InterestLifetime>" << GetInterestLifetime () << "</InterestLifetime>\n";
  // if (GetNonce ()>0)
  //   os << "  <Nonce>" << GetNonce () << "</Nonce>\n";
  // os << "</Interest>";
}

} // ccnb
} // wire

NDN_NAMESPACE_END
