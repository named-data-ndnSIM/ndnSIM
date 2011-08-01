/*
 *  ccn_random.cc
 *  Abstraction
 *
 *  Created by Ilya on 7/29/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ccn_random.h"
#include <openssl/rand.h>
//#include <openssl/rand.c>

/**
 * Generate pseudo-random bytes.
 *
 * @param buf is the destination buffer
 * @param size is in bytes
 */
void
ccn_random_bytes(unsigned char *buf, size_t size)
{
  int num = size;
  
  if (num < 0 || num != (int)size)
    abort();
  RAND_bytes(buf, num);
}

/**
 * Feed some entropy to the random number generator.
 * 
 * @param buf is the source buffer
 * @param size is in bytes
 * @param bits_of_entropy is an estimate; use 0 to make me guess
 */
void
ccn_add_entropy(const void *buf, size_t size, int bits_of_entropy)
{
  int num = size;
  
  if (num < 0 || num != (int)size)
    abort();
  /* Supply a hopefully conservative estimate of entropy. */
  if (bits_of_entropy <= 0)
    bits_of_entropy = (num < 32) ? 1 : num / 32;
  RAND_add((unsigned char *)buf, num, bits_of_entropy * 0.125);
}
