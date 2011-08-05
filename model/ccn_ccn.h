/*
 *  ccn_ccn.h
 *  Abstraction
 *
 *  Created by Ilya on 7/29/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CCN_CCN_DEFINED
#define CCN_CCN_DEFINED

#include <stdint.h>
#include "ccn_coding.h"
#include "ccn_charbuf.h"
#include "ccn_indexbuf.h"

/***********************************
 * Writing Names
 * Names for interests are constructed in charbufs using 
 * the following routines.
 */

/*
 * ccn_name_init: reset charbuf to represent an empty Name in binary format
 * Return value is 0, or -1 for error.
 */
int ccn_name_init(struct ccn_charbuf *c);

/*
 * ccn_name_append: add a Component to a Name
 * The component is an arbitrary string of n octets, no escaping required.
 * Return value is 0, or -1 for error.
 */
int ccn_name_append(struct ccn_charbuf *c, const void *component, size_t n);

/*
 * ccn_name_append_str: add a Component that is a \0 terminated string.
 * The component added is the bytes of the string without the \0.
 * This function is convenient for those applications that construct 
 * component names from simple strings.
 * Return value is 0, or -1 for error
 */
int ccn_name_append_str(struct ccn_charbuf *c, const char *s);

/*
 * ccn_name_append_components: add sequence of ccnb-encoded Components
 *    to a ccnb-encoded Name
 * start and stop are offsets from ccnb
 * Return value is 0, or -1 for obvious error
 */
int ccn_name_append_components(struct ccn_charbuf *c,
                               const unsigned char *ccnb,
                               size_t start, size_t stop);

enum ccn_marker {
    CCN_MARKER_NONE = -1,
    CCN_MARKER_SEQNUM  = 0x00, /**< consecutive block sequence numbers */
    CCN_MARKER_CONTROL = 0xC1, /**< commands, etc. */ 
    CCN_MARKER_OSEQNUM = 0xF8, /**< deprecated */
    CCN_MARKER_BLKID   = 0xFB, /**< nonconsecutive block ids */
    CCN_MARKER_VERSION = 0xFD  /**< timestamp-based versioning */
};

/*
 * ccn_name_append_numeric: add binary Component to ccnb-encoded Name
 * These are special components used for marking versions, fragments, etc.
 * Return value is 0, or -1 for error
 * see doc/technical/NameConventions.html
 */
int ccn_name_append_numeric(struct ccn_charbuf *c,
                            enum ccn_marker tag, uintmax_t value);

/*
 * ccn_name_append_nonce: add nonce Component to ccnb-encoded Name
 * Uses %C1.N.n marker.
 * see doc/technical/NameConventions.html
 */
int ccn_name_append_nonce(struct ccn_charbuf *c);

/*
 * ccn_name_split: find Component boundaries in a ccnb-encoded Name
 * Thin veneer over ccn_parse_Name().
 * returns -1 for error, otherwise the number of Components
 * components arg may be NULL to just do a validity check
 */
int ccn_name_split(const struct ccn_charbuf *c,
                   struct ccn_indexbuf* components);

/*
 * ccn_name_chop: Chop the name down to n components.
 * returns -1 for error, otherwise the new number of Components
 * components arg may be NULL; if provided it must be consistent with
 * some prefix of the name, and is updated accordingly.
 * n may be negative to say how many components to remove instead of how
 * many to leave, e.g. -1 will remove just the last component.
 */
int ccn_name_chop(struct ccn_charbuf *c,
                  struct ccn_indexbuf* components, int n);




/*********** Interest parsing ***********/

/*
 * The parse of an interest results in an array of offsets into the 
 * wire representation, with the start and end of each major element and
 * a few of the inportant sub-elements.  The following enum allows those
 * array items to be referred to symbolically.  The *_B_* indices correspond
 * to beginning offsets and the *_E_* indices correspond to ending offsets.
 * An omitted element has its beginning and ending offset equal to each other.
 * Normally these offsets will end up in non-decreasing order.
 * Some aliasing tricks may be played here, e.g. since
 * offset[CCN_PI_E_ComponentLast] is always equal to
 * offset[CCN_PI_E_LastPrefixComponent],
 * we may define CCN_PI_E_ComponentLast = CCN_PI_E_LastPrefixComponent.
 * However, code should not rely on that,
 * since it may change from time to time as the
 * interest schema evolves.
 */
enum ccn_parsed_interest_offsetid {
  CCN_PI_B_Name,
  CCN_PI_B_Component0,
  CCN_PI_B_LastPrefixComponent,
  CCN_PI_E_LastPrefixComponent,
  CCN_PI_E_ComponentLast = CCN_PI_E_LastPrefixComponent,
  CCN_PI_E_Name,
  CCN_PI_B_MinSuffixComponents,
  CCN_PI_E_MinSuffixComponents,
  CCN_PI_B_MaxSuffixComponents,
  CCN_PI_E_MaxSuffixComponents,
  CCN_PI_B_PublisherID, // XXX - rename
  CCN_PI_B_PublisherIDKeyDigest,
  CCN_PI_E_PublisherIDKeyDigest,
  CCN_PI_E_PublisherID,
  CCN_PI_B_Exclude,
  CCN_PI_E_Exclude,
  CCN_PI_B_ChildSelector,
  CCN_PI_E_ChildSelector,
  CCN_PI_B_AnswerOriginKind,
  CCN_PI_E_AnswerOriginKind,
  CCN_PI_B_Scope,
  CCN_PI_E_Scope,
  CCN_PI_B_InterestLifetime,
  CCN_PI_E_InterestLifetime,
  CCN_PI_B_Nonce,
  CCN_PI_E_Nonce,
  CCN_PI_B_OTHER,
  CCN_PI_E_OTHER,
  CCN_PI_E
};


struct ccn_parsed_interest {
  int magic;
  int prefix_comps;
  int min_suffix_comps;
  int max_suffix_comps;
  int orderpref;
  int answerfrom;
  int scope;
  unsigned short offset[CCN_PI_E+1];
};

/*
 * Bitmasks for AnswerOriginKind
 */
#define CCN_AOK_CS      0x1     /* Answer from content store */
#define CCN_AOK_NEW     0x2     /* OK to produce new content */
#define CCN_AOK_DEFAULT (CCN_AOK_CS | CCN_AOK_NEW)
#define CCN_AOK_STALE   0x4     /* OK to answer with stale data */
#define CCN_AOK_EXPIRE  0x10    /* Mark as stale (must have Scope 0) */

/***********************************
 * Low-level binary formatting
 */

/*
 * Append a ccnb start marker
 *
 * This forms the basic building block of ccnb-encoded data.
 * c is the buffer to append to.
 * Return value is 0, or -1 for error.
 */
int ccn_charbuf_append_tt(struct ccn_charbuf *c, size_t val, enum ccn_tt tt);

/**
 * Append a CCN_CLOSE
 *
 * Use this to close off an element in ccnb-encoded data.
 * @param c is the buffer to append to.
 * @returns 0 for success or -1 for error.
 */
int ccn_charbuf_append_closer(struct ccn_charbuf *c);

/***********************************
 * Slightly higher level binary formatting
 */

/*
 * Append a non-negative integer as a UDATA.
 */
int ccnb_append_number(struct ccn_charbuf *c, int nni);

/*
 * Append a binary timestamp
 * as a BLOB using the ccn binary Timestamp representation (12-bit fraction).
 */
int ccnb_append_timestamp_blob(struct ccn_charbuf *c,
                               enum ccn_marker marker,
                               long long secs, int nsecs);

/*
 * Append a binary timestamp, using the current time.
 */
int ccnb_append_now_blob(struct ccn_charbuf *c, enum ccn_marker marker);

/*
 * Append a start-of-element marker.
 */
int ccnb_element_begin(struct ccn_charbuf *c, enum ccn_dtag dtag);

/*
 * Append an end-of-element marker.
 * This is the same as ccn_charbuf_append_closer()
 */
int ccnb_element_end(struct ccn_charbuf *c);

/*
 * Append a tagged BLOB
 */
int ccnb_append_tagged_blob(struct ccn_charbuf *c, enum ccn_dtag dtag,
                            const void *data, size_t size);

/*
 * Append a tagged UDATA string, with printf-style formatting
 */
int ccnb_tagged_putf(struct ccn_charbuf *c, enum ccn_dtag dtag,
                     const char *fmt, ...);



/***********************************
 * Binary decoding
 * These routines require that the whole binary object be buffered.
 */

struct ccn_buf_decoder {
  struct ccn_skeleton_decoder decoder;
  const unsigned char *buf;
  size_t size;
};

struct ccn_buf_decoder *ccn_buf_decoder_start(struct ccn_buf_decoder *d,
                                              const unsigned char *buf, size_t size);

void ccn_buf_advance(struct ccn_buf_decoder *d);
int ccn_buf_advance_past_element(struct ccn_buf_decoder *d);

/* The match routines return a boolean - true for match */
int ccn_buf_match_dtag(struct ccn_buf_decoder *d, enum ccn_dtag dtag);

int ccn_buf_match_some_dtag(struct ccn_buf_decoder *d);

int ccn_buf_match_some_blob(struct ccn_buf_decoder *d);
int ccn_buf_match_blob(struct ccn_buf_decoder *d,
                       const unsigned char **bufp, size_t *sizep);

int ccn_buf_match_udata(struct ccn_buf_decoder *d, const char *s);

int ccn_buf_match_attr(struct ccn_buf_decoder *d, const char *s);

/* On error, the parse routines enter an error state and return a negative value. */
/*int ccn_parse_required_tagged_BLOB(struct ccn_buf_decoder *d,
                                   enum ccn_dtag dtag,
                                   int minlen, int maxlen);
int ccn_parse_optional_tagged_BLOB(struct ccn_buf_decoder *d,
                                   enum ccn_dtag dtag,
                                   int minlen, int maxlen);
int ccn_parse_nonNegativeInteger(struct ccn_buf_decoder *d);
int ccn_parse_optional_tagged_nonNegativeInteger(struct ccn_buf_decoder *d,
                                                 enum ccn_dtag dtag);
int ccn_parse_uintmax(struct ccn_buf_decoder *d, uintmax_t *result);
int ccn_parse_tagged_string(struct ccn_buf_decoder *d,
                            enum ccn_dtag dtag, struct ccn_charbuf *store);*/
/* check the decoder error state for these two - result can't be negative */
/*uintmax_t ccn_parse_required_tagged_binary_number(struct ccn_buf_decoder *d,
                                                  enum ccn_dtag dtag,
                                                  int minlen, int maxlen);
uintmax_t ccn_parse_optional_tagged_binary_number(struct ccn_buf_decoder *d,
                                                  enum ccn_dtag dtag,
                                                  int minlen, int maxlen,
                                                  uintmax_t default_value);*/

/**
 * Enter an error state if element closer not found.
 */
void ccn_buf_check_close(struct ccn_buf_decoder *d);

/*
 * ccn_ref_tagged_BLOB: Get address & size associated with blob-valued element
 * Returns 0 for success, negative value for error.
 */
int ccn_ref_tagged_BLOB(enum ccn_dtag tt,
                        const unsigned char *buf,
                        size_t start, size_t stop,
                        const unsigned char **presult, size_t *psize);

/*
 * ccn_parse_Name: Parses a ccnb-encoded name
 * components may be NULL, otherwise is filled in with Component boundary offsets
 * Returns the number of Components in the Name, or -1 if there is an error.
 */
int ccn_parse_Name(struct ccn_buf_decoder *d, struct ccn_indexbuf *components);

/***********************************
 * Authenticators and signatures for content are constructed in charbufs
 * using the following routines.
 */

enum ccn_content_type {
  CCN_CONTENT_DATA = 0x0C04C0,
  CCN_CONTENT_ENCR = 0x10D091,
  CCN_CONTENT_GONE = 0x18E344,
  CCN_CONTENT_KEY  = 0x28463F,
  CCN_CONTENT_LINK = 0x2C834A,
  CCN_CONTENT_NACK = 0x34008A
};


/*********** ContentObject parsing ***********/
/* Analogous to enum ccn_parsed_interest_offsetid, but for content */
enum ccn_parsed_content_object_offsetid {
  CCN_PCO_B_Signature,
  CCN_PCO_B_DigestAlgorithm,
  CCN_PCO_E_DigestAlgorithm,
  CCN_PCO_B_Witness,
  CCN_PCO_E_Witness,
  CCN_PCO_B_SignatureBits,
  CCN_PCO_E_SignatureBits,
  CCN_PCO_E_Signature,
  CCN_PCO_B_Name,
  CCN_PCO_B_Component0,
  CCN_PCO_E_ComponentN,
  CCN_PCO_E_ComponentLast = CCN_PCO_E_ComponentN,
  CCN_PCO_E_Name,
  CCN_PCO_B_SignedInfo,
  CCN_PCO_B_PublisherPublicKeyDigest,
  CCN_PCO_E_PublisherPublicKeyDigest,
  CCN_PCO_B_Timestamp,
  CCN_PCO_E_Timestamp,
  CCN_PCO_B_Type,
  CCN_PCO_E_Type,
  CCN_PCO_B_FreshnessSeconds,
  CCN_PCO_E_FreshnessSeconds,
  CCN_PCO_B_FinalBlockID,
  CCN_PCO_E_FinalBlockID,
  CCN_PCO_B_KeyLocator,
  /* Exactly one of Key, Certificate, or KeyName will be present */
  CCN_PCO_B_Key_Certificate_KeyName,
  CCN_PCO_B_KeyName_Name,
  CCN_PCO_E_KeyName_Name,
  CCN_PCO_B_KeyName_Pub,
  CCN_PCO_E_KeyName_Pub,
  CCN_PCO_E_Key_Certificate_KeyName,
  CCN_PCO_E_KeyLocator,
  CCN_PCO_E_SignedInfo,
  CCN_PCO_B_Content,
  CCN_PCO_E_Content,
  CCN_PCO_E
};

struct ccn_parsed_ContentObject {
  int magic;
  enum ccn_content_type type;
  int name_ncomps;
  unsigned short offset[CCN_PCO_E+1];
  unsigned char digest[32];	/* Computed only when needed */
  int digest_bytes;
};

int ccn_encode_ContentObject(struct ccn_charbuf *buf,
                             const struct ccn_charbuf *Name,
                             const void *data,
                             size_t size);
#endif
