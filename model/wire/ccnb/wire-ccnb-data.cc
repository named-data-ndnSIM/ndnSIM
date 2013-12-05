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

#include "ccnb-parser/common.h"
#include "ccnb-parser/visitors/void-depth-first-visitor.h"
#include "ccnb-parser/visitors/name-visitor.h"
#include "ccnb-parser/visitors/non-negative-integer-visitor.h"
#include "ccnb-parser/visitors/timestamp-visitor.h"
#include "ccnb-parser/visitors/string-visitor.h"
#include "ccnb-parser/visitors/uint32t-blob-visitor.h"
#include "ccnb-parser/visitors/content-type-visitor.h"

#include "ccnb-parser/syntax-tree/block.h"
#include "ccnb-parser/syntax-tree/dtag.h"

#include <boost/foreach.hpp>

NS_LOG_COMPONENT_DEFINE ("ndn.wire.Ccnb.Data");

NDN_NAMESPACE_BEGIN

namespace wire {
namespace ccnb {

// const std::string DefaultDigestAlgorithm = "2.16.840.1.101.3.4.2.1";

class DataTrailer : public Trailer
{
public:
  DataTrailer ()
  {
  }

  static TypeId GetTypeId ()
  {
    static TypeId tid = TypeId ("ns3::ndn::Data::Ccnb::Closer")
      .SetGroupName ("Ndn")
      .SetParent<Trailer> ()
      .AddConstructor<DataTrailer> ()
      ;
    return tid;
  }

  virtual TypeId GetInstanceTypeId (void) const
  {
    return GetTypeId ();
  }

  virtual void Print (std::ostream &os) const
  {
  }

  virtual uint32_t GetSerializedSize (void) const
  {
    return 2;
  }

  virtual void Serialize (Buffer::Iterator end) const
  {
    Buffer::Iterator i = end;
    i.Prev (2); // Trailer interface requires us to go backwards

    i.WriteU8 (0x00); // </Content>
    i.WriteU8 (0x00); // </Data>
  }

  virtual uint32_t Deserialize (Buffer::Iterator end)
  {
    Buffer::Iterator i = end;
    i.Prev (2); // Trailer interface requires us to go backwards

    uint8_t closing_tag_content = i.ReadU8 ();
    NS_ASSERT_MSG (closing_tag_content==0, "Should be a closing tag </Content> (0x00)");

    uint8_t closing_tag_content_object = i.ReadU8 ();
    NS_ASSERT_MSG (closing_tag_content_object==0, "Should be a closing tag </Data> (0x00)");

    return 2;
  }
};

NS_OBJECT_ENSURE_REGISTERED (Data);
NS_OBJECT_ENSURE_REGISTERED (DataTrailer);

TypeId
Data::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ndn::Data::Ccnb")
    .SetGroupName ("Ndn")
    .SetParent<Header> ()
    .AddConstructor<Data> ()
    ;
  return tid;
}

TypeId
Data::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

Data::Data ()
  : m_data (Create<ndn::Data> ())
{
}

Data::Data (Ptr<ndn::Data> data)
  : m_data (data)
{
}

Ptr<ndn::Data>
Data::GetData ()
{
  return m_data;
}

Ptr<Packet>
Data::ToWire (Ptr<const ndn::Data> data)
{
  static DataTrailer trailer;

  Ptr<const Packet> p = data->GetWire ();
  if (!p)
    {
      Ptr<Packet> packet = Create<Packet> (*data->GetPayload ());
      Data wireEncoding (ConstCast<ndn::Data> (data));
      packet->AddHeader (wireEncoding);
      packet->AddTrailer (trailer);
      data->SetWire (packet);

      p = packet;
    }

  return p->Copy ();
}

Ptr<ndn::Data>
Data::FromWire (Ptr<Packet> packet)
{
  static DataTrailer trailer;

  Ptr<ndn::Data> data = Create<ndn::Data> ();
  Ptr<Packet> wire = packet->Copy ();

  Data wireEncoding (data);
  packet->RemoveHeader (wireEncoding);
  packet->RemoveTrailer (trailer);

  data->SetPayload (packet);
  data->SetWire (wire);

  return data;
}

void
Data::Serialize (Buffer::Iterator start) const
{
  Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Data, CcnbParser::CCN_DTAG); // <Data>

  // fake signature
  Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Signature, CcnbParser::CCN_DTAG); // <Signature>
  // Signature ::= √DigestAlgorithm?
  //               Witness?
  //               √SignatureBits
  // if (GetSignature ().GetDigestAlgorithm () != Signature::DefaultDigestAlgorithm)
  //   {
  //     Ccnb::AppendString (start, CcnbParser::CCN_DTAG_DigestAlgorithm, GetSignature ().GetDigestAlgorithm ());
  //   }
  Ccnb::AppendString (start, CcnbParser::CCN_DTAG_DigestAlgorithm, "NOP");
  Ccnb::AppendTaggedBlobWithPadding (start, CcnbParser::CCN_DTAG_SignatureBits, 16, m_data->GetSignature ()); // <SignatureBits />
  Ccnb::AppendCloser (start);                                    // </Signature>

  // Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG);    // <Name>
  Ccnb::SerializeName (start, m_data->GetName());                                      //   <Component>...</Component>...
  // Ccnb::AppendCloser (start);                                                          // </Name>

  // fake signature
  Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_SignedInfo, CcnbParser::CCN_DTAG); // <SignedInfo>
  // SignedInfo ::= √PublisherPublicKeyDigest
  //                √Timestamp
  //                √Type?
  //                √FreshnessSeconds?
  //                FinalBlockID?
  //                KeyLocator?
  // Ccnb::AppendTaggedBlob (start, CcnbParser::CCN_DTAG_PublisherPublicKeyDigest,         // <PublisherPublicKeyDigest>...
  //                         GetSignedInfo ().GetPublisherPublicKeyDigest ());

  Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Timestamp, CcnbParser::CCN_DTAG);            // <Timestamp>...
  Ccnb::AppendTimestampBlob (start, m_data->GetTimestamp ());
  Ccnb::AppendCloser (start);

  // if (GetSignedInfo ().GetContentType () != DATA)
  //   {
  //     uint8_t type[3];
  //     type[0] = (GetSignedInfo ().GetContentType () >> 16) & 0xFF;
  //     type[1] = (GetSignedInfo ().GetContentType () >> 8 ) & 0xFF;
  //     type[2] = (GetSignedInfo ().GetContentType ()      ) & 0xFF;

  //     Ccnb::AppendTaggedBlob (start, CCN_DTAG_Type, type, 3);
  //   }
  if (m_data->GetFreshness () > Seconds(0))
    {
      Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_FreshnessSeconds, CcnbParser::CCN_DTAG);
      Ccnb::AppendNumber (start, m_data->GetFreshness ().ToInteger (Time::S));
      Ccnb::AppendCloser (start);
    }
  if (m_data->GetKeyLocator () != 0)
    {
      Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_KeyLocator, CcnbParser::CCN_DTAG); // <KeyLocator>
      {
        Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_KeyName, CcnbParser::CCN_DTAG);    // <KeyName>
        {
          // Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG);       // <Name>
          Ccnb::SerializeName (start, *m_data->GetKeyLocator ());         //   <Component>...</Component>...
          // Ccnb::AppendCloser (start);                                     // </Name>
        }
        Ccnb::AppendCloser (start);                                     // </KeyName>
      }
      Ccnb::AppendCloser (start);                                     // </KeyLocator>
    }

  Ccnb::AppendCloser (start);                                     // </SignedInfo>

  Ccnb::AppendBlockHeader (start, CcnbParser::CCN_DTAG_Content, CcnbParser::CCN_DTAG); // <Content>

  uint32_t payloadSize = m_data->GetPayload ()->GetSize ();
  if (payloadSize > 0)
    Ccnb::AppendBlockHeader (start, payloadSize, CcnbParser::CCN_BLOB);

  // there are no closing tags !!!
  // The closing tag is handled by DataTail
}

uint32_t
Data::GetSerializedSize () const
{
  size_t written = 0;
  written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Data); // <Data>

  // fake signature
  written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Signature); // <Signature>
  // Signature ::= DigestAlgorithm?
  //               Witness?
  //               SignatureBits
  // if (GetSignature ().GetDigestAlgorithm () != Signature::DefaultDigestAlgorithm)
  //   {
  //     written += Ccnb::EstimateString (CcnbParser::CCN_DTAG_DigestAlgorithm, GetSignature ().GetDigestAlgorithm ());
  //   }
  written += Ccnb::EstimateString (CcnbParser::CCN_DTAG_DigestAlgorithm, "NOP");
  // "signature" will be always padded to 16 octets
  written += Ccnb::EstimateTaggedBlob (CcnbParser::CCN_DTAG_SignatureBits, 16);      // <SignatureBits />
  // written += Ccnb::EstimateTaggedBlob (CcnbParser::CCN_DTAG_SignatureBits, sizeof (m_data->GetSignature ()));      // <SignatureBits />
  written += 1;                                    // </Signature>

  //written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Name);    // <Name>
  written += Ccnb::SerializedSizeName (m_data->GetName ()); //   <Component>...</Component>...
  //written += 1;                                  // </Name>

  // fake signature
  written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_SignedInfo); // <SignedInfo>
  // SignedInfo ::= √PublisherPublicKeyDigest
  //                √Timestamp
  //                √Type?
  //                √FreshnessSeconds?
  //                FinalBlockID?
  //                KeyLocator?

  // written += Ccnb::EstimateTaggedBlob (CCN_DTAG_PublisherPublicKeyDigest,                          // <PublisherPublicKeyDigest>...
  //                                      sizeof (GetSignedInfo ().GetPublisherPublicKeyDigest ()));

  written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Timestamp);                  // <Timestamp>...
  written += Ccnb::EstimateTimestampBlob (m_data->GetTimestamp ());
  written += 1;

  // if (GetSignedInfo ().GetContentType () != DATA)
  //   {
  //     written += Ccnb::EstimateTaggedBlob (CcnbParser::CCN_DTAG_Type, 3);
  //   }
  if (m_data->GetFreshness () > Seconds(0))
    {
      written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_FreshnessSeconds);
      written += Ccnb::EstimateNumber (m_data->GetFreshness ().ToInteger (Time::S));
      written += 1;
    }

  if (m_data->GetKeyLocator () != 0)
    {
      written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_KeyLocator); // <KeyLocator>
      {
        written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_KeyName);    // <KeyName>
        {
          //written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Name);       // <Name>
          written += Ccnb::SerializedSizeName (*m_data->GetKeyLocator ());        //   <Component>...</Component>...
          //written += 1;                                               // </Name>
        }
        written += 1;                                               // </KeyName>
      }
      written += 1;                                               // </KeyLocator>
    }

  written += 1; // </SignedInfo>

  written += Ccnb::EstimateBlockHeader (CcnbParser::CCN_DTAG_Content); // <Content>

  uint32_t payloadSize = m_data->GetPayload ()->GetSize ();
  if (payloadSize > 0)
    written += Ccnb::EstimateBlockHeader (payloadSize);

  // there are no closing tags !!!
  // The closing tag is handled by DataTail
  return written;
}

class DataVisitor : public CcnbParser::VoidDepthFirstVisitor
{
public:
  virtual void visit (CcnbParser::Dtag &n, boost::any param/*should be Data* */)
  {
    // uint32_t n.m_dtag;
    // std::list< Ptr<CcnbParser::Block> > n.m_nestedBlocks;
    static CcnbParser::NameVisitor nameVisitor;
    static CcnbParser::NonNegativeIntegerVisitor nonNegativeIntegerVisitor;
    static CcnbParser::TimestampVisitor          timestampVisitor;
    static CcnbParser::StringVisitor      stringVisitor;
    static CcnbParser::Uint32tBlobVisitor uint32tBlobVisitor;
    static CcnbParser::ContentTypeVisitor contentTypeVisitor;

    ndn::Data &contentObject = *(boost::any_cast<ndn::Data*> (param));

    switch (n.m_dtag)
      {
      case CcnbParser::CCN_DTAG_Data:
        // process nested blocks
        BOOST_FOREACH (Ptr<CcnbParser::Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }
        break;
      case CcnbParser::CCN_DTAG_Name:
        {
          // process name components
          Ptr<Name> name = Create<Name> ();
          n.accept (nameVisitor, GetPointer (name));
          contentObject.SetName (name);
          break;
        }

      case CcnbParser::CCN_DTAG_Signature:
        // process nested blocks
        BOOST_FOREACH (Ptr<CcnbParser::Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }
        break;

      // case CCN_DTAG_DigestAlgorithm:
      //   NS_LOG_DEBUG ("DigestAlgorithm");
      //   if (n.m_nestedTags.size ()!=1) // should be exactly one UDATA inside this tag
      //     throw CcnbParser::CcnbDecodingException ();

      //   contentObject.GetSignature ().SetDigestAlgorithm
      //     (boost::any_cast<std::string> ((*n.m_nestedTags.begin())->accept
      //                                    (stringVisitor)));
      //   break;

      case CcnbParser::CCN_DTAG_SignatureBits:
        NS_LOG_DEBUG ("SignatureBits");
        if (n.m_nestedTags.size ()!=1) // should be only one nested tag
          throw CcnbParser::CcnbDecodingException ();

        contentObject.SetSignature
          (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
                                      (uint32tBlobVisitor)));
        break;

      case CcnbParser::CCN_DTAG_SignedInfo:
        // process nested blocks
        BOOST_FOREACH (Ptr<CcnbParser::Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }
        break;

      // case CCN_DTAG_PublisherPublicKeyDigest:
      //   NS_LOG_DEBUG ("PublisherPublicKeyDigest");
      //   if (n.m_nestedTags.size ()!=1) // should be only one nested tag
      //     throw CcnbParser::CcnbDecodingException ();

      //   contentObject.GetSignedInfo ().SetPublisherPublicKeyDigest
      //     (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
      //                                 (uint32tBlobVisitor)));
      //   break;

      case CcnbParser::CCN_DTAG_Timestamp:
        NS_LOG_DEBUG ("Timestamp");
        if (n.m_nestedTags.size()!=1) // should be exactly one nested tag
          throw CcnbParser::CcnbDecodingException ();

        contentObject.SetTimestamp
          (boost::any_cast<Time> ((*n.m_nestedTags.begin())->accept
                                  (timestampVisitor)));
        break;

      // case CCN_DTAG_Type:
      //   NS_LOG_DEBUG ("Type");
      //   if (n.m_nestedTags.size ()!=1) // should be only one nested tag
      //     throw CcnbParser::CcnbDecodingException ();

      //   contentObject.GetSignedInfo ().SetContentType
      //     (static_cast<Data::ContentType>
      //      (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
      //                                  (contentTypeVisitor))));
      //   break;

      case CcnbParser::CCN_DTAG_FreshnessSeconds:
        NS_LOG_DEBUG ("FreshnessSeconds");

        if (n.m_nestedTags.size()!=1) // should be exactly one nested tag
          throw CcnbParser::CcnbDecodingException ();

        contentObject.SetFreshness
          (Seconds
           (boost::any_cast<uint32_t> ((*n.m_nestedTags.begin())->accept
                                       (nonNegativeIntegerVisitor))));
        break;

      case CcnbParser::CCN_DTAG_KeyLocator:
        // process nested blocks
        BOOST_FOREACH (Ptr<CcnbParser::Block> block, n.m_nestedTags)
          {
            block->accept (*this, param);
          }
        break;

      case CcnbParser::CCN_DTAG_KeyName:
        {
          if (n.m_nestedTags.size ()!=1) // should be exactly one nested tag
            throw CcnbParser::CcnbDecodingException ();

          // process name components
          Ptr<Name> name = Create<Name> ();
          n.accept (nameVisitor, GetPointer (name));
          contentObject.SetKeyLocator (name);
          break;
        }

      case CcnbParser::CCN_DTAG_Content: // !!! HACK
        // This hack was necessary for memory optimizations (i.e., content is virtual payload)
        NS_ASSERT_MSG (n.m_nestedTags.size() == 0, "Parser should have stopped just after processing <Content> tag");
        break;

      default: // ignore all other stuff
        break;
      }
  }
};

uint32_t
Data::Deserialize (Buffer::Iterator start)
{
  static DataVisitor contentObjectVisitor;

  Buffer::Iterator i = start;
  Ptr<CcnbParser::Block> root = CcnbParser::Block::ParseBlock (i);
  root->accept (contentObjectVisitor, GetPointer (m_data));

  return i.GetDistanceFrom (start);
}

void
Data::Print (std::ostream &os) const
{
  os << "D: " << m_data->GetName ();
  // os << "<Data><Name>" << GetName () << "</Name><Content>";
}

} // ccnb
} // wire

NDN_NAMESPACE_END
