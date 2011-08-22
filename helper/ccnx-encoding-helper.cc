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
 * Author: 
 */

#include "ccnx-encoding-helper.h"

#include "ns3/name-components.h"
#include "ns3/ccnx-interest-header.h"
#include "ns3/ccnx-content-object-header.h"

#include <sstream>
#include <boost/foreach.hpp>

namespace ns3 {

#define CCN_TT_BITS 3
#define CCN_TT_MASK ((1 << CCN_TT_BITS) - 1)
#define CCN_MAX_TINY ((1 << (7-CCN_TT_BITS)) - 1)
#define CCN_TT_HBIT ((unsigned char)(1 << 7))

size_t
CcnxEncodingHelper::AppendBlockHeader (Buffer::Iterator start, size_t val, CcnbParser::ccn_tt tt)
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
CcnxEncodingHelper::AppendNumber (Buffer::Iterator start, uint32_t number)
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
CcnxEncodingHelper::CcnxEncodingHelper::AppendCloser (Buffer::Iterator start)
{
  start.WriteU8 (CcnbParser::CCN_CLOSE);
  return 1;
}

size_t
CcnxEncodingHelper::AppendNameComponents (Buffer::Iterator start, const Name::Components &name)
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
CcnxEncodingHelper::AppendTimestampBlob (Buffer::Iterator start, Time time)
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
  ts = ((time.ToInteger (Time::S) & 15) << 12) + ((time.ToInteger (Time::NS) / 5 * 8 + 195312) / 390625);
  for (int i = required_bytes - 2; i < required_bytes; i++)
    start.WriteU8 ( ts >> (8 * (required_bytes - 1 - i)) );
  
  return len + required_bytes;
}

size_t
CcnxEncodingHelper::AppendTaggedBlob (Buffer::Iterator start, CcnbParser::ccn_dtag dtag,
                  const uint8_t *data, size_t size)
{
  size_t written = AppendBlockHeader (start, dtag, CcnbParser::CCN_DTAG);
  if (size>0)
    {
      written += AppendBlockHeader (start, size, CcnbParser::CCN_BLOB);
      start.Write (data, size);
      written += size;
    }
  written += AppendCloser (start);

  return written;
}


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
  if (interest.GetExclude().size() > 0)
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
      written += AppendTimestampBlob (start, interest.GetInterestLifetime());
      written += AppendCloser (start);
    }
  if (interest.GetNonce()>0)
    {
      uint32_t nonce = interest.GetNonce();
      written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_Nonce,
                                   reinterpret_cast<const uint8_t*>(&nonce),
                                   sizeof(nonce));
    }
  written += AppendCloser (start); // </Interest>

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

  // there is no closing tag !!!
  return written;
}

} // namespace ns3
