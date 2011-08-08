/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 */

/*
 *  ccn_buf_encoder.cc
 *  Abstraction
 *
 *  Created by Ilya on 7/29/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "ccn_ccn.h"
#include "ccn_charbuf.h"
#include "ccn_coding.h"
#include "ccn_indexbuf.h"

/**
 * Append a ccnb start marker
 *
 * This forms the basic building block of ccnb-encoded data.
 * @param c is the buffer to append to.
 * @param val is the numval, intepreted according to tt (see enum ccn_tt).
 * @param tt is the type field.
 * @returns 0 for success or -1 for error.
 */
int
ccn_charbuf_append_tt(struct ccn_charbuf *c, size_t val, enum ccn_tt tt)
{
  unsigned char buf[1+8*((sizeof(val)+6)/7)];
  unsigned char *p = &(buf[sizeof(buf)-1]);
  int n = 1;
  p[0] = (CCN_TT_HBIT & ~CCN_CLOSE) |
  ((val & CCN_MAX_TINY) << CCN_TT_BITS) |
  (CCN_TT_MASK & tt);
  val >>= (7-CCN_TT_BITS);
  while (val != 0) {
    (--p)[0] = (((unsigned char)val) & ~CCN_TT_HBIT) | CCN_CLOSE;
    n++;
    val >>= 7;
  }
  return(ccn_charbuf_append(c, p, n));
}


/**
 * Encode and sign a ContentObject.
 * @param buf is the output buffer where encoded object is written.
 * @param Name is the ccnb-encoded name from ccn_name_init and friends.
 * @param SignedInfo is the ccnb-encoded info from ccn_signed_info_create.
 * @param data pintes to the raw data to be encoded.
 * @param size is the size, in bytes, of the raw data to be encoded.
 * @param digest_algorithm may be NULL for default.
 * @param private_key is the private key to use for signing.
 * @returns 0 for success or -1 for error.
 */
int
ccn_encode_ContentObject(struct ccn_charbuf *buf,
                         const struct ccn_charbuf *Name,
                         /*const struct ccn_charbuf *SignedInfo,*/
                         const void *data,
                         size_t size
                         /*const char *digest_algorithm,
                         const struct ccn_pkey *private_key*/
                         )
{
    int res = 0;
    //struct ccn_sigc *sig_ctx;
    //struct ccn_signature *signature;
    //size_t signature_size;
    struct ccn_charbuf *content_header;
    size_t closer_start;
    
    content_header = ccn_charbuf_create();
    res |= ccn_charbuf_append_tt(content_header, CCN_DTAG_Content, CCN_DTAG);
    if (size != 0)
        res |= ccn_charbuf_append_tt(content_header, size, CCN_BLOB);
    closer_start = content_header->length;
    res |= ccn_charbuf_append_closer(content_header);
    if (res < 0)
        return(-1);
    //sig_ctx = ccn_sigc_create();
    //if (sig_ctx == NULL)
    //    return(-1);
    //if (0 != ccn_sigc_init(sig_ctx, digest_algorithm))
    //    return(-1);
    //if (0 != ccn_sigc_update(sig_ctx, Name->buf, Name->length))
    //    return(-1);
    //if (0 != ccn_sigc_update(sig_ctx, SignedInfo->buf, SignedInfo->length))
    //    return(-1);
    //if (0 != ccn_sigc_update(sig_ctx, content_header->buf, closer_start))
    //    return(-1);
    //if (0 != ccn_sigc_update(sig_ctx, data, size))
    //    return(-1);
    //if (0 != ccn_sigc_update(sig_ctx, content_header->buf + closer_start,
    //                         content_header->length - closer_start))
    //    return(-1);
    //signature = calloc(1, ccn_sigc_signature_max_size(sig_ctx, private_key));
    //if (signature == NULL)
    //    return(-1);
    //res = ccn_sigc_final(sig_ctx, signature, &signature_size, private_key);
    //if (0 != res) {
    //    free(signature);
    //    return(-1);
    //}
    //ccn_sigc_destroy(&sig_ctx);
    res |= ccn_charbuf_append_tt(buf, CCN_DTAG_ContentObject, CCN_DTAG);
    //res |= ccn_encode_Signature(buf, digest_algorithm,
    //                            NULL, 0, signature, signature_size);
    res |= ccn_charbuf_append_charbuf(buf, Name);
    //res |= ccn_charbuf_append_charbuf(buf, SignedInfo);
    res |= ccnb_append_tagged_blob(buf, CCN_DTAG_Content, data, size);
    res |= ccn_charbuf_append_closer(buf);
    //free(signature);
    ccn_charbuf_destroy(&content_header);
    return(res == 0 ? 0 : -1);
}

/**
 * Append a tagged BLOB
 *
 * This is a ccnb-encoded element with containing the BLOB as content
 * @param c is the buffer to append to.
 * @param dtag is the element's dtab
 * @param data points to the binary data
 * @param size is the size of the data, in bytes
 * @returns 0 for success or -1 for error.
 */
int
ccnb_append_tagged_blob(struct ccn_charbuf *c,
                        enum ccn_dtag dtag,
                        const void *data,
                        size_t size)
{
    int res;
    
    res = ccn_charbuf_append_tt(c, dtag, CCN_DTAG);
    if (size != 0) {
        res |= ccn_charbuf_append_tt(c, size, CCN_BLOB);
        res |= ccn_charbuf_append(c, data, size);
    }
    res |= ccn_charbuf_append_closer(c);
    return(res == 0 ? 0 : -1);
}
