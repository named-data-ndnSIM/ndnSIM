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

__all__ = ['Face', 'Name', 'Interest', 'Data', 'Key', 'EventLoop']

VERSION = 0.3
NDNSIM = True

import sys as _sys

try:
    from Face import Face
    from Name import Name
    from Interest import Interest
    from Data import Data
    from Key import Key

    from EventLoop import EventLoop
    from KeyLocator import KeyLocator
    from SignedInfo import SignedInfo, CONTENT_DATA, CONTENT_ENCR, CONTENT_GONE, CONTENT_KEY, CONTENT_LINK, CONTENT_NACK
    # no signature

    # no NameCrypto
    # no LocalPrefixDiscovery

    import nre

except ImportError:
    del _sys.modules[__name__]
    raise
