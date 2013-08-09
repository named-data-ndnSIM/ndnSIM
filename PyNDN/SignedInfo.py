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

import utils

class ContentType(utils.Enum):
    _prefix = "ndn"

CONTENT_DATA = ContentType.new_flag('CONTENT_DATA', 0x0C04C0)
CONTENT_ENCR = ContentType.new_flag('CONTENT_ENCR', 0x10D091)
CONTENT_GONE = ContentType.new_flag('CONTENT_GONE', 0x18E344)
CONTENT_KEY  = ContentType.new_flag('CONTENT_KEY',  0x28463F)
CONTENT_LINK = ContentType.new_flag('CONTENT_LINK', 0x2C834A)
CONTENT_NACK = ContentType.new_flag('CONTENT_NACK', 0x34008A)

class SignedInfo (object):
    def __init__(self, keyLocator = None, freshness = None,
                 timestamp = None, type = CONTENT_DATA, *kw, **kwargs):

        self.timestamp = timestamp
        self.freshnessSeconds = freshness
        self.keyLocator = keyLocator
        self.type = type

        # all other parameters are silently ignored

    def __repr__(self):
        args = []

        if self.keyLocator is not None:
            args += ["keyLocator=%r" % self.keyLocator]
        if self.freshnessSeconds is not None:
            args += ["freshness=%r" % self.freshnessSeconds]
        if self.timestamp is not None:
            args += ["timestamp=%r" % self.timestamp]
        if self.type != CONTENT_DATA:
            args += ["type=%r" % self.type]

        return "ndn.SignedInfo(%s)" % ", ".join(args)

