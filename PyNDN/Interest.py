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

import ns.ndnSIM
import ns.core
import Name

class Interest (object):
    _interest = None

    def __init__(self, name = None, 
                 scope = None, interestLifetime = None):
        self._interest = ns.ndnSIM.ndn.Interest ()
        
        self.name = name
        self.scope = scope
        self.interestLifetime = interestLifetime

    def __getattr__ (self, name):
        if name == "name":
            return Name (self._interest.GetName ())
        elif name == "scope":
            return self._interest.GetScope ()
        elif name == "interestLifetime":
            return self._interest.GetInterestLifetime ().ToDouble (ns.core.Time.S)
        else:
            return self._interest.__getattribute__ (name)

    def __setattr__(self, name, value):
        if name == "_interest":
            return object.__setattr__ (self, name, value)
        elif name == "name":
            if not value:
                return self._interest.SetName (ns.ndnSIM.ndn.Name ())
            elif type (value) is Name:
                return self._interest.SetName (value._name)
            elif type (value) is ns.ndnSIM.ndn.Name:
                return self._interest.SetName (value)
            elif type (value) is str:
                return self._interest.SetName (ns.ndnSIM.ndn.Name (value))
            else:
                raise ValueError ("Invalid name parameter")
        elif name == "scope":
            if not value:
                return self._interest.SetScope (-1)
            elif type (value) is int:
                return self._interest.SetScope (value)
            else:
                raise ValueError ("Scope parameter should be int, [%s] supplied" % type (value))
        elif name == "interestLifetime":
            if not value:
                return self._interest.SetInterestLifetime (ns.core.Time ())
            elif type (value) is float or type (value) is int:
                return self._interest.SetInterestLifetime (ns.core.Seconds (value))
            else:
                raise ValueError ("interestLifetime parameter should be fload or int, [%s] supplied" % type (value))
        else:
            raise ValueError ("Unknown or unsupported attribute [%s]" % name)

    def __repr__(self):
        return "ndnSIM.Interest(%s)" % str (self._interest)
