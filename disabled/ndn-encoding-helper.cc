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

#include "ndn-encoding-helper.h"

#include "ns3/ndn-name.h"
#include "ns3/ndn-interest-header.h"
#include "ns3/ndn-content-object-header.h"

#include <sstream>
#include <boost/foreach.hpp>

namespace ns3 {
namespace ndn {

size_t
EncodingHelper::Serialize (Buffer::Iterator start, const Interest &interest)
{
  size_t written = 0;
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Interest, CcnbParser::CCN_DTAG); // <Interest>
  
  written += AppendBlockHeader (start, CcnbParser::CCN_DTAG_Name, CcnbParser::CCN_DTAG); // <Name>
  written += AppendName (start, interest.GetName());                // <Component>...</Component>...
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
      written += AppendName (start, interest.GetExclude());                // <Component>...</Component>...
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
      written += AppendTaggedBlob (start, CcnbParser::CCN_DTAG_Nonce, nonce);
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
EncodingHelper::GetSerializedSize (const Interest &interest)
{
  size_t written = 0;
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Interest); // <Interest>
  
  written += EstimateBlockHeader (CcnbParser::CCN_DTAG_Name); // <Name>
  written += EstimateName (interest.GetName()); // <Component>...</Component>...
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
      written += EstimateName (interest.GetExclude());                // <Component>...</Component>...
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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#define CCN_TT_BITS 3
#define CCN_TT_MASK ((1 << CCN_TT_BITS) - 1)
#define CCN_MAX_TINY ((1 << (7-CCN_TT_BITS)) - 1)
#define CCN_TT_HBIT ((unsigned char)(1 << 7))

size_t
EncodingHelper::AppendBlockHeader (Buffer::Iterator &start, size_t val, CcnbParser::ccn_tt tt)
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
EncodingHelper::EstimateBlockHeader (size_t value)
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
EncodingHelper::AppendNumber (Buffer::Iterator &start, uint32_t number)
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
EncodingHelper::EstimateNumber (uint32_t number)
{
  std::ostringstream os;
  os << number;
  return EstimateBlockHeader (os.str ().size ()) + os.str ().size ();
}
  
size_t
EncodingHelper::AppendCloser (Buffer::Iterator &start)
{
  start.WriteU8 (CcnbParser::CCN_CLOSE);
  return 1;
}

size_t
EncodingHelper::AppendName (Buffer::Iterator &start, const Name &name)
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
EncodingHelper::EstimateName (const Name &name)
{
  size_t written = 0;
  BOOST_FOREACH (const std::string &component, name.GetComponents())
    {
      written += EstimateTaggedBlob (CcnbParser::CCN_DTAG_Component, component.size());
    }
  return written;
}

size_t
EncodingHelper::AppendTimestampBlob (Buffer::Iterator &start, const Time &time)
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
EncodingHelper::EstimateTimestampBlob (const Time &time)
{
  int required_bytes = 2; // 12 bits for fractions of a second, 4 bits left for seconds. Sometimes it is enough
  intmax_t ts = time.ToInteger (Time::S) >> 4;
  for (;  required_bytes < 7 && ts != 0; ts >>= 8) // not more than 6 bytes?
     required_bytes++;

  return EstimateBlockHeader (required_bytes) + required_bytes;
}

size_t
EncodingHelper::AppendTaggedBlob (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag,
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
EncodingHelper::EstimateTaggedBlob (CcnbParser::ccn_dtag dtag, size_t size)
{
  if (size>0)
    return EstimateBlockHeader (dtag) + EstimateBlockHeader (size) + size + 1;
  else
    return EstimateBlockHeader (dtag) + 1;
}

size_t
EncodingHelper::AppendString (Buffer::Iterator &start, CcnbParser::ccn_dtag dtag,
                                  const std::string &string)
{
  size_t written = AppendBlockHeader (start, dtag, CcnbParser::CCN_DTAG);
  {
    written += AppendBlockHeader (start, string.size (), CcnbParser::CCN_UDATA);
    start.Write (reinterpret_cast<const uint8_t*> (string.c_str ()), string.size ());
    written += string.size ();
  }
  written += AppendCloser (start);

  return written;
}

size_t
EncodingHelper::EstimateString (CcnbParser::ccn_dtag dtag, const std::string &string)
{
  return EstimateBlockHeader (dtag) + EstimateBlockHeader (string.size ()) + string.size () + 1;
}


} // namespace ndn
} // namespace ns3
