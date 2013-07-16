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

__all__ = ['Face', 'Closure', 'ContentObject', 'Interest', 'Key', 'Name']

VERSION = 0.3

# import sys as _sys

# try:
# 	from ndn.Face import *
# 	from ndn.Closure import *
# 	from ndn.ContentObject import *
# 	from ndn.Interest import *
# 	from ndn.Key import *
# 	from ndn.Name import *
# 	from ndn import NameCrypto
#     from ndn.LocalPrefixDiscovery import *

# except ImportError:
# 	del _sys.modules[__name__]
# 	raise
