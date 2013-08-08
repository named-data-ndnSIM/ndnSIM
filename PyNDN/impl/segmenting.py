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

import math
import ndn

class Wrapper(object):
    def __init__(self, name, key):
        self.name = name
        self.key = key

        kl = ndn.KeyLocator(key)
        self.signed_info = ndn.SignedInfo(key_locator = kl, key_digest = key.publicKeyID)

    def __call__(self, chunk, segment, segments):
        name = self.name + ndn.Name.num2seg(segment)
        self.signed_info.finalBlockID = ndn.Name.num2seg(segments - 1)

        co = ndn.Data(name = name, content = chunk, signed_info = self.signed_info)
        co.sign(self.key)

        return co

def segmenter(data, wrapper = None, chunk_size = 4096):
    segment = 0
    segments = math.ceil(len(data) / float(chunk_size))

    while segment < segments:
        start = segment * chunk_size
        end = min(start + chunk_size, len(data))
        chunk = data[start : end]

        if wrapper is not None:
            chunk = wrapper(chunk, segment, segments)

        yield chunk

        segment += 1

