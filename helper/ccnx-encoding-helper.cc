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

#include "ccnx-encoding-helper.h"

#include "ns3/ccnx-name-components.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

#include <sstream>
#include <boost/foreach.hpp>

namespace ns3 {

size_t
CcnxEncodingHelper::Serialize (Buffer::Iterator start, const CcnxInterestHeader &interest)
{
  size_t written = 0;
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Interest, CcnbParser::CCN_DTAG); // <Interest>
  
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG); // <Name>
  written += AppendNameComponents (start, interest.GetName());                // <Component>...</Component>...
  written += AppendCloser (start);                               // </Name>

  if (interest.GetMinSuffixComponents() >= 0)
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_MinSuffixComponents, CcnbParser::CCN_DTAG);
      written += AppendNumber (start, interest.GetMinSuffixComponents ());
      written += AppendCloser (start);
    }
  if (interest.GetMaxSuffixComponents() >= 0)
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_MaxSuffixComponents, CcnbParser::CCN_DTAG);
      written += AppendNumber (start, interest.GetMaxSuffixComponents ());
      written += AppendCloser (start);
    }
  if (interest.IsEnabledExclude() && interest.GetExclude().size() > 0)
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Exclude, CcnbParser::CCN_DTAG); // <Exclude>
      written += AppendNameComponents (start, interest.GetExclude());                // <Component>...</Component>...
      written += AppendCloser (start);                                  // </Exclude>
    }
  if (interest.IsEnabledChildSelector())
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_ChildSelector, CcnbParser::CCN_DTAG);
      written += AppendNumber (start, 1);
      written += AppendCloser (start);
    }
  if (interest.IsEnabledAnswerOriginKind())
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_AnswerOriginKind, CcnbParser::CCN_DTAG);
      written += AppendNumber (start, 1);
      written += AppendCloser (start);
    }
  if (interest.GetScope() >= 0)
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Scope, CcnbParser::CCN_DTAG);
      written += AppendNumber (start, interest.GetScope ());
      written += AppendCloser (start);
    }
  if (!interest.GetInterestLifetime().IsZero())
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_InterestLifetime, CcnbParser::CCN_DTAG);
      written += AppendTimestampBlob (start, interest.GetInterestLifetime ());
      written += AppendCloser (start);
    }
  if (interest.GetNonce()>0)
    {
      uint32_t nonce = interest.GetNonce();
      written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_Nonce,
                                   reinterpret_cast<const uint8_t*>(&nonce),
                                   sizeof(nonce));
    }
    
  if (interest.GetNack ()>0)
    {
      written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Nack, CcnbParser::CCN_DTAG);
      written += AppendNumber (start, interest.GetNack ());
      written += AppendCloser (start);
    }
  written += AppendCloser (start); // </Interest>

  return written;
}

size_t
CcnxEncodingHelper::GetSerializedSize (const CcnxInterestHeader &interest)
{
  size_t written = 0;
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Interest); // <Interest>
  
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Name); // <Name>
  written += EstimateNameComponents (interest.GetName()); // <Component>...</Component>...
  written += 1; // </Name>

  if (interest.GetMinSuffixComponents() >= 0)
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_MinSuffixComponents);
      written += EstimateNumber (interest.GetMinSuffixComponents ());
      written += 1;
    }
  if (interest.GetMaxSuffixComponents() >= 0)
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_MaxSuffixComponents);
      written += EstimateNumber (interest.GetMaxSuffixComponents ());
      written += 1;
    }
  if (interest.IsEnabledExclude() && interest.GetExclude().size() > 0)
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Exclude);
      written += EstimateNameComponents (interest.GetExclude());                // <Component>...</Component>...
      written += 1;                                  // </Exclude>
    }
  if (interest.IsEnabledChildSelector())
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_ChildSelector);
      written += EstimateNumber (1);
      written += 1;
    }
  if (interest.IsEnabledAnswerOriginKind())
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_AnswerOriginKind);
      written += EstimateNumber (1);
      written += 1;
    }
  if (interest.GetScope() >= 0)
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Scope);
      written += EstimateNumber (interest.GetScope ());
      written += 1;
    }
  if (!interest.GetInterestLifetime().IsZero())
    {
      written += EstimateBlockHeader (CcnbParser::CCN_DTAG_InterestLifetime);
      written += EstimateTimestampBlob (interest.GetInterestLifetime());
      written += 1;
    }
  if (interest.GetNonce()>0)
    {
      written += EstimateTaggedBlob (CcnbParser::CCN_DTAG_Nonce, sizeof(uint32_t));
    }
  if (interest.GetNack ()>0)
    {
        written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Nack);
        written += EstimateNumber (interest.GetNack ());
        written += 1;
    }

  written += 1; // </Interest>

  return written;
}

size_t
CcnxEncodingHelper::Serialize (Buffer::Iterator start, const CcnxContentObjectHeader &contentObject)
{
  size_t written = 0;
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_ContentObject, CcnbParser::CCN_DTAG); // <ContentObject>

  // fake signature
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Signature, CcnbParser::CCN_DTAG); // <Signature>
  // Signature ::= DigestAlgorithm? 
  //               Witness?         
  //               SignatureBits   
  written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_SignatureBits, 0, 0);      // <SignatureBits />
  written += AppendCloser (start);                                    // </Signature>  

  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG);    // <Name>
  written += AppendNameComponents (start, contentObject.GetName()); //   <Component>...</Component>...
  written += AppendCloser (start);                                  // </Name>  

  // fake signature
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_SignedInfo, CcnbParser::CCN_DTAG); // <SignedInfo>
  // SignedInfo ::= PublisherPublicKeyDigest
  //                Timestamp
  //                Type?
  //                FreshnessSeconds?
  //                FinalBlockID?
  //                KeyLocator?
  written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_PublisherPublicKeyDigest, 0, 0); // <PublisherPublicKeyDigest />
  written += AppendCloser (start);                                     // </SignedInfo>

  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Content, CcnbParser::CCN_DTAG); // <Content>

  // there are no closing tags !!!
  return written;
}

size_t
CcnxEncodingHelper::GetSerializedSize (const CcnxContentObjectHeader &contentObject)
{
  size_t written = 0;
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_ContentObject); // <ContentObject>

  // fake signature
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Signature); // <Signature>
  // Signature ::= DigestAlgorithm? 
  //               Witness?         
  //               SignatureBits   
  written += EstimateTaggedBlob (CcnbParser::CCN_DTAG_SignatureBits, 0);      // <SignatureBits />
  written += 1;                                    // </Signature>  

  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Name);    // <Name>
  written += EstimateNameComponents (contentObject.GetName()); //   <Component>...</Component>...
  written += 1;                                  // </Name>  

  // fake signature
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_SignedInfo); // <SignedInfo>
  // SignedInfo ::= PublisherPublicKeyDigest
  //                Timestamp
  //                Type?
  //                FreshnessSeconds?
  //                FinalBlockID?
  //                KeyLocator?
  written += EstimateTaggedBlob (CcnbParser::CCN_DTAG_PublisherPublicKeyDigest, 0); // <PublisherPublicKeyDigest />
  written += 1;                                     // </SignedInfo>

  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Content); // <Content>

  // there are no closing tags !!!
  return written;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define CCN_TT_BITS 3
#define CCN_TT_MASK ((1 << CCN_TT_BITS) - 1)
#define CCN_MAX_TINY ((1 << (7-CCN_TT_BITS)) - 1)
#define CCN_TT_HBIT ((unsigned char)(1 << 7))

size_t
CcnxEncodingHelper::AppendBlockHeader (Buffer::Iterator &start, size_t val, CcnbParser::ccn_tt tt)
{
  unsigned char buf[1+8*((sizeof(val)+6)/7)];
  unsigned char *p = &(buf[sizeof(buf)-1]);
  size_t n = 1;
  p[0] = (CCN_TT_HBIT & ~CcnbParser::CCN_CLOSE) |
  ((val & CCN_MAX_TINY) << CCN_TT_BITS) |
  (CCN_TT_MASK & tt);
  val >>= (7-CCN_TT_BITS);
  while (val != 0) {
    (--p)[0] = (((unsigned char)val) & ~CCN_TT_HBIT) | CcnbParser::CCN_CLOSE;
    n++;
    val >>= 7;
  }
  start.Write (p,n);
  return n;
}

size_t
CcnxEncodingHelper::EstimateBlockHeader (size_t value)
{
  value >>= (7-CCN_TT_BITS);
  size_t n = 1;
  while (value>0)
    {
      value >>= 7;
      n++;
    }
  return n;
}

size_t
CcnxEncodingHelper::AppendNumber (Buffer::Iterator &start, uint32_t number)
{
  std::ostringstream os;
  os << number;

  size_t written = 0;
  written += AppendBlockHeader (start, os.str().size(), CcnbParser::CCN_UDATA);
  written += os.str().size();
  start.Write (reinterpret_cast<const unsigned char*>(os.str().c_str()), os.str().size());

  return written;
}

size_t
CcnxEncodingHelper::EstimateNumber (uint32_t number)
{
  std::ostringstream os;
  os << number;
  return EstimateBlockHeader (os.str ().size ()) + os.str ().size ();
}
  
size_t
CcnxEncodingHelper::AppendCloser (Buffer::Iterator &start)
{
  start.WriteU8 (CcnbParser::CCN_CLOSE);
  return 1;
}

size_t
CcnxEncodingHelper::AppendNameComponents (Buffer::Iterator &start, const CcnxNameComponents &name)
{
  size_t written = 0;
  BOOST_FOREACH (const std::string &component, name.GetComponents())
    {
      written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_Component,
                                   reinterpret_cast<const uint8_t*>(component.c_str()), component.size());
    }
  return written;
}

size_t
CcnxEncodingHelper::EstimateNameComponents (const CcnxNameComponents &name)
{
  size_t written = 0;
  BOOST_FOREACH (const std::string &component, name.GetComponents())
    {
      written += EstimateTaggedBlob (CcnbParser::CCN_DTAG_Component, component.size());
    }
  return written;
}

size_t
CcnxEncodingHelper::AppendTimestampBlob (Buffer::Iterator &start, const Time &time)
{
  // the original function implements Markers... thought not sure what are these markers for...

  // Determine miminal number of bytes required to store the timestamp
  int required_bytes = 2; // 12 bits for fractions of a second, 4 bits left for seconds. Sometimes it is enough
  intmax_t ts = time.ToInteger (Time::S) >> 4;
  for (;  required_bytes < 7 && ts != 0; ts >>= 8) // not more than 6 bytes?
     required_bytes++;
  
  size_t len = AppendBlockHeader(start, required_bytes, CcnbParser::CCN_BLOB);

  // write part with seconds
  ts = time.ToInteger (Time::S) >> 4;
  for (int i = 0; i < required_bytes - 2; i++)
    start.WriteU8 ( ts >> (8 * (required_bytes - 3 - i)) );

  /* arithmetic contortions are to avoid overflowing 31 bits */
  ts = ((time.ToInteger (Time::S) & 15) << 12) +
       (((time.ToInteger (Time::NS) % 1000000000) / 5 * 8 + 195312) / 390625);
  for (int i = required_bytes - 2; i < required_bytes; i++)
    start.WriteU8 ( ts >> (8 * (required_bytes - 1 - i)) );
  
  return len + required_bytes;
}

size_t
CcnxEncodingHelper::EstimateTimestampBlob (const Time &time)
{
  int required_bytes = 2; // 12 bits for fractions of a second, 4 bits left for seconds. Sometimes it is enough
  intmax_t ts = time.ToInteger (Time::S) >> 4;
  for (;  required_bytes < 7 && ts != 0; ts >>= 8) // not more than 6 bytes?
     required_bytes++;

  return EstimateBlockHeader (required_bytes) + required_bytes;
}

size_t
CcnxEncodingHelper::AppendTaggedBlob (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag,
                  const uint8_t *data, size_t size)
{
  size_t written = AppendBlockHeader (start, dtag, CcnbParser::CCN_DTAG);
  /* 2 */
  if (size>0)
    {
      written += AppendBlockHeader (start, size, CcnbParser::CCN_BLOB);
      start.Write (data, size);
      written += size;
      /* size */
    }
  written += AppendCloser (start);
  /* 1 */

  return written;
}

size_t
CcnxEncodingHelper::EstimateTaggedBlob (CcnbParser::ccn_dtag dtag, size_t size)
{
  if (size>0)
    return EstimateBlockHeader (dtag) + EstimateBlockHeader (size) + size + 1;
  else
    return EstimateBlockHeader (dtag) + 1;
}



} // namespace ns3
