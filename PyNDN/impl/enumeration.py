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

import ndn
from ndn.impl import ccnb

def ccnb_enumerate(names):
    out = bytearray()

    for name in names:
        out += ccnb.dtag(ccnb.DTAG_LINK, name.get_ccnb())

    return ccnb.dtag(ccnb.DTAG_COLLECTION, out)
