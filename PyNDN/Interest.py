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
from Name import Name

class Interest (object):
    _interest = None

    def __init__(self,
                 name = None, scope = None, interestLifetime = None,
                 interest = None):
        if interest:
            if isinstance (interest, Interest):
                self._interest = interest._interest
            elif isinstance (interest, ns.ndnSIM.ndn.Interest):
                self._interest = interest
            else:
                raise TypeError ("Invalid type supplied for 'interest' parameter [%s]" % type (interest))
        else:
            self._interest = ns.ndnSIM.ndn.Interest ()
        
            self.name = name
            self.scope = scope
            self.interestLifetime = interestLifetime

    @staticmethod
    def fromWire (wire):
        return Interest (interest = ns.ndnSIM.ndn.Wire.ToInterestStr (wire))

    def toWire (self):
        return ns.ndnSIM.ndn.Wire.FromInterestStr (self._interest)

    def __getattr__ (self, name):
        if name == "_interest":
            return object.__getattr__ (self, name)

        elif name == "name":
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
            if value is None:
                return self._interest.SetName (ns.ndnSIM.ndn.Name ())
            elif isinstance (value, Name):
                return self._interest.SetName (value._name)
            elif isinstance (value, ns.ndnSIM.ndn.Name):
                return self._interest.SetName (value)
            elif isinstance (value, str):
                return self._interest.SetName (ns.ndnSIM.ndn.Name (value))
            else:
                raise ValueError ("Invalid name parameter")
        elif name == "scope":
            if value is None:
                return self._interest.SetScope (-1)
            elif isinstance (value, int):
                return self._interest.SetScope (value)
            else:
                raise ValueError ("Scope parameter should be int, [%s] supplied" % type (value))
        elif name == "interestLifetime":
            if value is None:
                return self._interest.SetInterestLifetime (ns.core.Time ())
            elif isinstance (value, float) or isinstance (value, int):
                return self._interest.SetInterestLifetime (ns.core.Seconds (value))
            else:
                raise ValueError ("interestLifetime parameter should be fload or int, [%s] supplied" % type (value))
        else:
            raise ValueError ("Unknown or unsupported attribute [%s]" % name)

    def __repr__(self):
        return "ndnSIM.Interest(%s)" % str (self._interest)
