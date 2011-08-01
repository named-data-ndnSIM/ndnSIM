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
