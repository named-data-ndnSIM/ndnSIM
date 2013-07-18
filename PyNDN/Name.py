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

class Name ():
    _name = None

    def __init__ (self, 
                  value = None,
                  name = None):
        if name:
            if type (name) is ns.ndnSIM.ndn.Name:
                self._name = name
            elif type (name) is Name:
                self._name = name._name
            else:
                raise TypeError ("Incorrect type for 'name' parameter [%s]" % type (name))
        else:
            if value:
                self._name = ns.ndnSIM.ndn.Name (value)
            else:
                self._name = ns.ndnSIM.ndn.Name ()

    @staticmethod
    def fromWire (wire):
        return ns.ndnSIM.ndn.Wire.ToName (wire)

    @staticmethod
    def toWire (name):
        return ns.ndnSIM.ndn.Wire.FromName (name)
    
    def __getattr__ (self, name):
        return self._name.__getattribute__ (name)

    def __len__ (self):
        return self._name.size ()

    def __add__ (self, other):
        return self._name.append (other)

    def __getitem__(self, key):
        if type (key) is int:
            if abs(key) < self._name.size ():
                return self._name.get (key)
            else:
                raise IndexError ("index out of range")
        elif type (key) is slice:
            name = ns.ndnSIM.ndn.Name ()
            for component in xrange (*key.indices (self.size ())):
                name.append (self._name.get (component))
            return name
        else:
            raise ValueError("Unknown __getitem__ type: %s" % type (key))

    def __repr__ (self):
        return "ndnSIM.Name('" + self._name.toUri () + "')"

