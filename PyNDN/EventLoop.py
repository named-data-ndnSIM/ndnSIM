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

import ns.core

class EventLoop (object):
    """
    Class to provide compatibility with real PyNDN implementation.
    """
    
    def __init__ (self, *handlers):
        pass
    
    def execute (self, event):
        ns.core.Simulator.ScheduleNow (event)

    def run (self):
        pass

    def stop (self):
        pass
