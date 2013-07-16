## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
# 
# Copyright (c) 2011-2013, Regents of the University of California
#                          Alexander Afanasyev
# 
# GNU 3.0 license, See the LICENSE file for more information
# 
# Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
# 

#
# Based on PyCCN code, copyrighted and licensed as follows
#
# Copyright (c) 2011-2013, Regents of the University of California
# BSD license, See the COPYING file for more information
# Written by: Derek Kulinski <takeda@takeda.tk>
#             Jeff Burke <jburke@ucla.edu>
#

__TT_BITS__ = 3
__TT_MASK__ = (1 << __TT_BITS__) - 1
__TT_HBIT__ = (1 << 7)
__TT_LBIT__ = __TT_HBIT__ - 1

DTAG_NAME       = 14
DTAG_COLLECTION = 17
DTAG_LINK       = 31

def blob(value):
    return _encode(len(value), 5) + value

def dtag(tag, value):
    return _encode(tag, 2) + value + '\x00'

def _encode(value, tt):
    global __TT_BITS__, __TT_HBIT__, __TT_LBIT__

    header = (value << __TT_BITS__) | tt

    blocks = []
    blocks.append((header & __TT_LBIT__) | __TT_HBIT__)
    header >>= 7

    while header != 0:
        blocks.append(header & __TT_LBIT__)
        header >>= 7

    blocks.reverse()

    return bytearray(blocks)

class CCNBDecoder(object):
    def __init__(self, ccnb_data):
        self.ccnb_data = ccnb_data
        self.reset()

    def reset(self):
        self.position = 0
        self.decoded = 0
        self.stack = []

    def _process_next_byte(self):
        global __TT_HBIT__, __TT_LBIT__

        assert self.position < len(self.ccnb_data)

        char = ord(self.ccnb_data[self.position])
        self.position += 1

        if self.decoded == 0 and char == 0:
            return None, True

        decoded = (self.decoded << 7) | (char & __TT_LBIT__)
        complete = (char & __TT_HBIT__) == __TT_HBIT__

        self.decoded = decoded if not complete else 0

        return decoded, complete

    def print_element(self, tt, value, data = None):
        if tt == 2:
            print "DTAG",

            if value == 14:
                print "Name"
            elif value == 15:
                print "Component"
            elif value == 17:
                print "Collection"
            elif value == 31:
                print "Link"
            else:
                print value

        elif tt == 5:
            print "BLOB",
            print value,
            print repr(data)
        else:
            print tt,
            print value,
            print repr(data)

    def get_tags(self):
        global __TT_MASK__, __TT_BITS__

        while self.position < len(self.ccnb_data):
            while True:
                decoded, complete = self._process_next_byte()
                if complete:
                    break

            if decoded is None:
                tt, value = self.stack.pop()
                print "Close",
            else:
                tt = decoded & __TT_MASK__
                value = decoded >> __TT_BITS__

            data = None
            if decoded is not None:
                if tt == 2:
                    self.stack.append((tt, value))
                elif tt == 5:
                    data = self.ccnb_data[self.position:self.position + value]
                    self.position += value

            self.print_element(tt, value, data)

