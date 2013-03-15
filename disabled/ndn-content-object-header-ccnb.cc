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

#include "ndn-content-object-header-ccnb.h"

#include "ns3/log.h"
#include "../helper/ndn-encoding-helper.h"
#include "../helper/ndn-decoding-helper.h"

#include "../helper/ccnb-parser/common.h"
#include "../helper/ccnb-parser/visitors/void-depth-first-visitor.h"
#include "../helper/ccnb-parser/visitors/name-components-visitor.h"
#include "../helper/ccnb-parser/visitors/non-negative-integer-visitor.h"
#include "../helper/ccnb-parser/visitors/timestamp-visitor.h"
#include "../helper/ccnb-parser/visitors/string-visitor.h"
#include "../helper/ccnb-parser/visitors/uint32t-blob-visitor.h"
#include "../helper/ccnb-parser/visitors/content-type-visitor.h"

#include "../helper/ccnb-parser/syntax-tree/block.h"
#include "../helper/ccnb-parser/syntax-tree/dtag.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.ContentObject");

namespace ns3 {
namespace ndn {

using namespace CcnbParser;

const std::string ContentObject::Signature::DefaultDigestAlgorithm = "2.16.840.1.101.3.4.2.1";

NS_OBJECT_ENSURE_REGISTERED (ContentObject);
NS_OBJECT_ENSURE_REGISTERED (ContentObjectTail);

TypeId
ContentObject::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::ContentObject")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<ContentObject> ()
    ;
  return tid;
}

ContentObject::ContentObject ()
{
}

void
ContentObject::SetName (const Ptr<Name> &name)
{
  m_name = name;
}

const Name&
ContentObject::GetName () const
{
  if (m_name==0) throw ContentObjectException();
  return *m_name;
}

Ptr<const Name>
ContentObject::GetNamePtr () const
{
  return m_name;
}

#define CCNB EncodingHelper // just to simplify writing

void
ContentObject::Serialize (Buffer::Iterator start) const
{
  size_t written = 0;
  written += CCNB::AppendBlockHeader (start, CCN_DTAG_ContentObject, CCN_DTAG); // <ContentObject>

  // fake signature
  written += CCNB::AppendBlockHeader (start, CCN_DTAG_Signature, CCN_DTAG); // <Signature>
  // Signature ::= √DigestAlgorithm? 
  //               Witness?         
  //               √SignatureBits
  if (GetSignature ().GetDigestAlgorithm () != Signature::DefaultDigestAlgorithm)
    {
      written += CCNB::AppendString (start, CCN_DTAG_DigestAlgorithm, GetSignature ().GetDigestAlgorithm ());
    }
  written += CCNB::AppendTaggedBlob (start, CCN_DTAG_SignatureBits, GetSignature ().GetSignatureBits ()); // <SignatureBits />
  written += CCNB::AppendCloser (start);                                    // </Signature>  

  written += CCNB::AppendBlockHeader (start, CCN_DTAG_Name, CCN_DTAG);    // <Name>
  written += CCNB::AppendName (start, GetName()); //   <Component>...</Component>...
  written += CCNB::AppendCloser (start);                                  // </Name>  

  // fake signature
  written += CCNB::AppendBlockHeader (start, CCN_DTAG_SignedInfo, CCN_DTAG); // <SignedInfo>
  // SignedInfo ::= √PublisherPublicKeyDigest
  //                √Timestamp
  //                √Type?
  //                √FreshnessSeconds?
  //                FinalBlockID?
  //                KeyLocator?
  written += CCNB::AppendTaggedBlob (start, CCN_DTAG_PublisherPublicKeyDigest,         // <PublisherPublicKeyDigest>...
                                     GetSignedInfo ().GetPublisherPublicKeyDigest ());
  
  written += CCNB::AppendBlockHeader (start, CCN_DTAG_Timestamp, CCN_DTAG);            // <Timestamp>...
  written += CCNB::AppendTimestampBlob (start, GetSignedInfo ().GetTimestamp ());
  written += CCNB::AppendCloser (start);

  if (GetSignedInfo ().GetContentType () != DATA)
    {
      uint8_t type[3];
      type[0] = (GetSignedInfo ().GetContentType () >> 16) & 0xFF;
      type[1] = (GetSignedInfo ().GetContentType () >> 8 ) & 0xFF;
      type[2] = (GetSignedInfo ().GetContentType ()      ) & 0xFF;
      
      written += CCNB::AppendTaggedBlob (start, CCN_DTAG_Type, type, 3);
    }
  if (GetSignedInfo ().GetFreshness () > Seconds(0))
    {
      written += CCNB::AppendBlockHeader (start, CCN_DTAG_FreshnessSeconds, CCN_DTAG);
      written += CCNB::AppendNumber (start, GetSignedInfo ().GetFreshness ().ToInteger (Time::S));
      written += CCNB::AppendCloser (start);
    }
  if (GetSignedInfo ().GetKeyLocator () != 0)
    {
      written += CCNB::AppendBlockHeader (start, CCN_DTAG_KeyLocator, CCN_DTAG); // <KeyLocator>
      {
        written += CCNB::AppendBlockHeader (start, CCN_DTAG_KeyName, CCN_DTAG);    // <KeyName>
        {
          written += CCNB::AppendBlockHeader (start, CCN_DTAG_Name, CCN_DTAG);       // <Name>
          written += CCNB::AppendName (start, GetName());                  //   <Component>...</Component>...
          written += CCNB::AppendCloser (start);                                     // </Name>
        }
        written += CCNB::AppendCloser (start);                                     // </KeyName>
      }
      written += CCNB::AppendCloser (start);                                     // </KeyLocator>
    }
  
  written += CCNB::AppendCloser (start);                                     // </SignedInfo>

  written += CCNB::AppendBlockHeader (start, CCN_DTAG_Content, CCN_DTAG); // <Content>

  // there are no closing tags !!!
  // The closing tag is handled by ContentObjectTail
}

uint32_t
ContentObject::GetSerializedSize () const
{
  size_t written = 0;
  written += CCNB::EstimateBlockHeader (CCN_DTAG_ContentObject); // <ContentObject>

  // fake signature
  written += CCNB::EstimateBlockHeader (CCN_DTAG_Signature); // <Signature>
  // Signature ::= DigestAlgorithm? 
  //               Witness?         
  //               SignatureBits   
  if (GetSignature ().GetDigestAlgorithm () != Signature::DefaultDigestAlgorithm)
    {
      written += CCNB::EstimateString (CCN_DTAG_DigestAlgorithm, GetSignature ().GetDigestAlgorithm ());
    }
  written += CCNB::EstimateTaggedBlob (CCN_DTAG_SignatureBits,
                                       sizeof (GetSignature ().GetSignatureBits ()));      // <SignatureBits />
  written += 1;                                    // </Signature>  

  written += CCNB::EstimateBlockHeader (CCN_DTAG_Name);    // <Name>
  written += CCNB::EstimateName (GetName()); //   <Component>...</Component>...
  written += 1;                                  // </Name>  

  // fake signature
  written += CCNB::EstimateBlockHeader (CCN_DTAG_SignedInfo); // <SignedInfo>
  // SignedInfo ::= √PublisherPublicKeyDigest
  //                √Timestamp
  //                √Type?
  //                √FreshnessSeconds?
  //                FinalBlockID?
  //                KeyLocator?
  
  written += CCNB::EstimateTaggedBlob (CCN_DTAG_PublisherPublicKeyDigest,                          // <PublisherPublicKeyDigest>...
                                       sizeof (GetSignedInfo ().GetPublisherPublicKeyDigest ()));
  
  written += CCNB::EstimateBlockHeader (CCN_DTAG_Timestamp);                  // <Timestamp>...
  written += CCNB::EstimateTimestampBlob (GetSignedInfo ().GetTimestamp ());
  written += 1;

  if (GetSignedInfo ().GetContentType () != DATA)
    {
      written += CCNB::EstimateTaggedBlob (CCN_DTAG_Type, 3);
    }
  if (GetSignedInfo ().GetFreshness () > Seconds(0))
    {
      written += CCNB::EstimateBlockHeader (CCN_DTAG_FreshnessSeconds);
      written += CCNB::EstimateNumber (GetSignedInfo ().GetFreshness ().ToInteger (Time::S));
      written += 1;
    }

  if (GetSignedInfo ().GetKeyLocator () != 0)
    {
      written += CCNB::EstimateBlockHeader (CCN_DTAG_KeyLocator); // <KeyLocator>
      {
        written += CCNB::EstimateBlockHeader (CCN_DTAG_KeyName);    // <KeyName>
        {
          written += CCNB::EstimateBlockHeader (CCN_DTAG_Name);       // <Name>
          written += CCNB::EstimateName (GetName());        //   <Component>...</Component>...
          written += 1;                                               // </Name>
        }
        written += 1;                                               // </KeyName>
      }
      written += 1;                                               // </KeyLocator>
    }
  
  written += 1; // </SignedInfo>

  written += CCNB::EstimateBlockHeader (CCN_DTAG_Content); // <Content>

  // there are no closing tags !!!
  // The closing tag is handled by ContentObjectTail
  return written;
}
#undef CCNB

class ContentObjectVisitor : public VoidDepthFirstVisitor
{
public:
  virtual void visit (Dtag &n, boost::any param/*should be ContentObject* */)
  {
    // uint32_t n.m_dtag;
    // std::list<Ptr<Block> > n.m_nestedBlocks;
    static NameVisitor nameComponentsVisitor;
    static NonNegativeIntegerVisitor nonNegativeIntegerVisitor;
    static TimestampVisitor          timestampVisitor;
    static StringVisitor     stringVisitor;
    static Uint32tBlobVisitor uint32tBlobVisitor;
    static ContentTypeVisitor contentTypeVisitor;
  
    ContentObject &contentObject = *(boost::any_cast<ContentObject*> (param));
  
    switch (n.m_dtag)
      {
      case CCN_DTAG_ContentObject:
        // process nested blocks
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }
        break;
      case CCN_DTAG_Name:
        {
          // process name components
          Ptr<Name> name = Create<Name> ();
        
          BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
            {
              block->accept (nameComponentsVisitor, &(*name));
            }
          contentObject.SetName (name);
          break;
        }

      case CCN_DTAG_Signature: 
        // process nested blocks
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }      
        break;

      case CCN_DTAG_DigestAlgorithm:
        NS_LOG_DEBUG ("DigestAlgorithm");
        if (n.m_nestedTags.size ()!=1) // should be exactly one UDATA inside this tag
          throw CcnbDecodingException ();
        
        contentObject.GetSignature ().SetDigestAlgorithm
          (boost::any_cast<std::string> ((*n.m_nestedTags.begin())->accept
                                         (stringVisitor)));
        break;

      case CCN_DTAG_SignatureBits:
        NS_LOG_DEBUG ("SignatureBits");
        if (n.m_nestedTags.size ()!=1) // should be only one nested tag
          throw CcnbDecodingException ();

        contentObject.GetSignature ().SetSignatureBits
          (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
                                      (uint32tBlobVisitor)));
        break;

      case CCN_DTAG_SignedInfo:
        // process nested blocks
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }      
        break;
      
      case CCN_DTAG_PublisherPublicKeyDigest:
        NS_LOG_DEBUG ("PublisherPublicKeyDigest");
        if (n.m_nestedTags.size ()!=1) // should be only one nested tag
          throw CcnbDecodingException ();

        contentObject.GetSignedInfo ().SetPublisherPublicKeyDigest
          (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
                                      (uint32tBlobVisitor)));
        break;

      case CCN_DTAG_Timestamp:
        NS_LOG_DEBUG ("Timestamp");
        if (n.m_nestedTags.size()!=1) // should be exactly one nested tag
          throw CcnbDecodingException ();

        contentObject.GetSignedInfo ().SetTimestamp
          (boost::any_cast<Time> ((*n.m_nestedTags.begin())->accept
                                  (timestampVisitor)));
        break;

      case CCN_DTAG_Type:
        NS_LOG_DEBUG ("Type");
        if (n.m_nestedTags.size ()!=1) // should be only one nested tag
          throw CcnbDecodingException ();

        contentObject.GetSignedInfo ().SetContentType
          (static_cast<ContentObject::ContentType>
           (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
                                       (contentTypeVisitor))));
        break;
        
      case CCN_DTAG_FreshnessSeconds:
        NS_LOG_DEBUG ("FreshnessSeconds");
      
        if (n.m_nestedTags.size()!=1) // should be exactly one nested tag
          throw CcnbDecodingException ();
        
        contentObject.GetSignedInfo ().SetFreshness
          (Seconds
           (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
                                       (nonNegativeIntegerVisitor))));
        break;
      
      case CCN_DTAG_KeyLocator:
        // process nested blocks
        BOOST_FOREACH (Ptr<Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }      
        break;

      case CCN_DTAG_KeyName:
        {
          if (n.m_nestedTags.size ()!=1) // should be exactly one nested tag
            throw CcnbDecodingException ();

          Ptr<BaseTag> nameTag = DynamicCast<BaseTag>(n.m_nestedTags.front ());
          if (nameTag == 0)
            throw CcnbDecodingException ();

          // process name components
          Ptr<Name> name = Create<Name> ();
        
          BOOST_FOREACH (Ptr<Block> block, nameTag->m_nestedTags)
            {
              block->accept (nameComponentsVisitor, &(*name));
            }
          contentObject.GetSignedInfo ().SetKeyLocator (name);
          break;
        }

      case CCN_DTAG_Content: // !!! HACK
        // This hack was necessary for memory optimizations (i.e., content is virtual payload)
        NS_ASSERT_MSG (n.m_nestedTags.size() == 0, "Parser should have stopped just after processing <Content> tag");
        break;
      
      default: // ignore all other stuff
        break;
      }
  }
};

uint32_t
ContentObject::Deserialize (Buffer::Iterator start)
{
  static ContentObjectVisitor contentObjectVisitor;

  Buffer::Iterator i = start;
  Ptr<Block> root = Block::ParseBlock (i);
  root->accept (contentObjectVisitor, this);

  return i.GetDistanceFrom (start);
}
  
TypeId
ContentObject::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
  
void
ContentObject::Print (std::ostream &os) const
{
  os << "D: " << GetName ();
  // os << "<ContentObject><Name>" << GetName () << "</Name><Content>";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ContentObjectTail::ContentObjectTail ()
{
}

TypeId
ContentObjectTail::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::ContentObjectTail")
    .SetParent<Trailer> ()
    .AddConstructor<ContentObjectTail> ()
    ;
  return tid;
}

TypeId
ContentObjectTail::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
ContentObjectTail::Print (std::ostream &os) const
{
  os << "</Content></ContentObject>";
}

uint32_t
ContentObjectTail::GetSerializedSize (void) const
{
  return 2;
}

void
ContentObjectTail::Serialize (Buffer::Iterator start) const
{
  Buffer::Iterator i = start;
  i.Prev (2); // Trailer interface requires us to go backwards
  
  i.WriteU8 (0x00); // </Content>
  i.WriteU8 (0x00); // </ContentObject>
}

uint32_t
ContentObjectTail::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  i.Prev (2); // Trailer interface requires us to go backwards

  uint8_t closing_tag_content = i.ReadU8 ();
  NS_ASSERT_MSG (closing_tag_content==0, "Should be a closing tag </Content> (0x00)");

  uint8_t closing_tag_content_object = i.ReadU8 ();
  NS_ASSERT_MSG (closing_tag_content_object==0, "Should be a closing tag </ContentObject> (0x00)");

  return 2;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

ContentObject::SignedInfo::SignedInfo ()
  : m_publisherPublicKeyDigest (0)
  // ,  m_timestamp
  , m_type (DATA)
  // , m_freshness
  // , FinalBlockID
  // , KeyLocator
{
}

void
ContentObject::SignedInfo::SetPublisherPublicKeyDigest (uint32_t digest)
{
  m_publisherPublicKeyDigest = digest;
}

uint32_t
ContentObject::SignedInfo::GetPublisherPublicKeyDigest () const
{
  return m_publisherPublicKeyDigest;
}

void
ContentObject::SignedInfo::SetTimestamp (const Time &timestamp)
{
  m_timestamp = timestamp;
}

Time
ContentObject::SignedInfo::GetTimestamp () const
{
  return m_timestamp;
}

void
ContentObject::SignedInfo::SetContentType (ContentObject::ContentType type)
{
  m_type = type;
}

ContentObject::ContentType
ContentObject::SignedInfo::GetContentType () const
{
  return m_type;
}

void
ContentObject::SignedInfo::SetFreshness (const Time &freshness)
{
  m_freshness = freshness;
}

Time
ContentObject::SignedInfo::GetFreshness () const
{
  return m_freshness;
}

void
ContentObject::SignedInfo::SetKeyLocator (Ptr<const Name> keyLocator)
{
  m_keyLocator = keyLocator;
}

Ptr<const Name>
ContentObject::SignedInfo::GetKeyLocator () const
{
  return m_keyLocator;
}

} // namespace ndn
} // namespace ns3
