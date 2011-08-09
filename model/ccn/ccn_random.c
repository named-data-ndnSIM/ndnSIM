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
