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

#ifndef _CCNB_PARSER_COMMON_H_
#define _CCNB_PARSER_COMMON_H_

#include "ns3/ndn-common.h"

#ifndef NDN_NAMESPACE_BEGIN
#error "dafaq"
#endif

NDN_NAMESPACE_BEGIN;

namespace wire {

namespace CcnbParser {

// forward declarations
class Block;
class Blob;
class Udata;
class Tag;
class Attr;
class Dtag;
class Dattr;
class Ext;


/**
 * \brief Exception thrown if there is a parsing error
 */
class CcnbDecodingException {};

/**
 * \brief Type tag for a ccnb start marker.
 *
 * \see http://www.ccnx.org/releases/latest/doc/technical/DTAG.html
 */
enum ccn_tt {
  CCN_EXT,        /**< starts composite extension - numval is subtype */
  CCN_TAG,        /**< starts composite - numval is tagnamelen-1 */ 
  CCN_DTAG,       /**< starts composite - numval is tagdict index (enum ccn_dtag) */
  CCN_ATTR,       /**< attribute - numval is attrnamelen-1, value follows */
  CCN_DATTR,      /**< attribute numval is attrdict index */
  CCN_BLOB,       /**< opaque binary data - numval is byte count */
  CCN_UDATA,      /**< UTF-8 encoded character data - numval is byte count */
  CCN_NO_TOKEN    /**< should not occur in encoding */
};

/** \brief CCN_CLOSE terminates composites */
enum {CCN_CLOSE = 0};

/**
 * \brief DTAG identifies ccnb-encoded elements.
 *
 * \see http://www.ccnx.org/releases/latest/doc/technical/DTAG.html
 */
enum ccn_dtag {
  CCN_DTAG_Any = 13,
  CCN_DTAG_Name = 14,
  CCN_DTAG_Component = 15,
  CCN_DTAG_Certificate = 16,
  CCN_DTAG_Collection = 17,
  CCN_DTAG_CompleteName = 18,
  CCN_DTAG_Content = 19,
  CCN_DTAG_SignedInfo = 20,
  CCN_DTAG_ContentDigest = 21,
  CCN_DTAG_ContentHash = 22,
  CCN_DTAG_Count = 24,
  CCN_DTAG_Header = 25,
  CCN_DTAG_Interest = 26,	/* 20090915 */
  CCN_DTAG_Key = 27,
  CCN_DTAG_KeyLocator = 28,
  CCN_DTAG_KeyName = 29,
  CCN_DTAG_Length = 30,
  CCN_DTAG_Link = 31,
  CCN_DTAG_LinkAuthenticator = 32,
  CCN_DTAG_NameComponentCount = 33,	/* DeprecatedInInterest */
  CCN_DTAG_RootDigest = 36,
  CCN_DTAG_Signature = 37,
  CCN_DTAG_Start = 38,
  CCN_DTAG_Timestamp = 39,
  CCN_DTAG_Type = 40,
  CCN_DTAG_Nonce = 41,
  CCN_DTAG_Scope = 42,
  CCN_DTAG_Exclude = 43,
  CCN_DTAG_Bloom = 44,
  CCN_DTAG_BloomSeed = 45,
  CCN_DTAG_AnswerOriginKind = 47,
  CCN_DTAG_InterestLifetime = 48,
  CCN_DTAG_Witness = 53,
  CCN_DTAG_SignatureBits = 54,
  CCN_DTAG_DigestAlgorithm = 55,
  CCN_DTAG_BlockSize = 56,
  CCN_DTAG_FreshnessSeconds = 58,
  CCN_DTAG_FinalBlockID = 59,
  CCN_DTAG_PublisherPublicKeyDigest = 60,
  CCN_DTAG_PublisherCertificateDigest = 61,
  CCN_DTAG_PublisherIssuerKeyDigest = 62,
  CCN_DTAG_PublisherIssuerCertificateDigest = 63,
  CCN_DTAG_Data = 64,	/* 20090915 */
  CCN_DTAG_WrappedKey = 65,
  CCN_DTAG_WrappingKeyIdentifier = 66,
  CCN_DTAG_WrapAlgorithm = 67,
  CCN_DTAG_KeyAlgorithm = 68,
  CCN_DTAG_Label = 69,
  CCN_DTAG_EncryptedKey = 70,
  CCN_DTAG_EncryptedNonceKey = 71,
  CCN_DTAG_WrappingKeyName = 72,
  CCN_DTAG_Action = 73,
  CCN_DTAG_FaceID = 74,
  CCN_DTAG_IPProto = 75,
  CCN_DTAG_Host = 76,
  CCN_DTAG_Port = 77,
  CCN_DTAG_MulticastInterface = 78,
  CCN_DTAG_ForwardingFlags = 79,
  CCN_DTAG_FaceInstance = 80,
  CCN_DTAG_ForwardingEntry = 81,
  CCN_DTAG_MulticastTTL = 82,
  CCN_DTAG_MinSuffixComponents = 83,
  CCN_DTAG_MaxSuffixComponents = 84,
  CCN_DTAG_ChildSelector = 85,
  CCN_DTAG_RepositoryInfo = 86,
  CCN_DTAG_Version = 87,
  CCN_DTAG_RepositoryVersion = 88,
  CCN_DTAG_GlobalPrefix = 89,
  CCN_DTAG_LocalName = 90,
  CCN_DTAG_Policy = 91,
  CCN_DTAG_Namespace = 92,
  CCN_DTAG_GlobalPrefixName = 93,
  CCN_DTAG_PolicyVersion = 94,
  CCN_DTAG_KeyValueSet = 95,
  CCN_DTAG_KeyValuePair = 96,
  CCN_DTAG_IntegerValue = 97,
  CCN_DTAG_DecimalValue = 98,
  CCN_DTAG_StringValue = 99,
  CCN_DTAG_BinaryValue = 100,
  CCN_DTAG_NameValue = 101,
  CCN_DTAG_Entry = 102,
  CCN_DTAG_ACL = 103,
  CCN_DTAG_ParameterizedName = 104,
  CCN_DTAG_Prefix = 105,
  CCN_DTAG_Suffix = 106,
  CCN_DTAG_Root = 107,
  CCN_DTAG_ProfileName = 108,
  CCN_DTAG_Parameters = 109,
  CCN_DTAG_InfoString = 110,
  CCN_DTAG_StatusResponse = 112,
  CCN_DTAG_StatusCode = 113,
  CCN_DTAG_StatusText = 114,
  CCN_DTAG_Nack = 200,
  CCN_DTAG_SequenceNumber = 256,
  CCN_DTAG_CCNProtocolDataUnit = 17702112
};

} // CcnbParser
} // wire

NDN_NAMESPACE_END

#endif // _CCNB_PARSER_COMMON_H_
