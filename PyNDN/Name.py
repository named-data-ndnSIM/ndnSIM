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

class Name (object):
    _name = None

    def __init__ (self, 
                  value = None,
                  name = None):
        if name is not None:
            if isinstance (name, ns.ndnSIM.ndn.Name):
                self._name = name
            elif isinstance (name, Name):
                self._name = name._name
            else:
                raise TypeError ("Incorrect type for 'name' parameter [%s]" % type (name))
        else:
            if value is not None:
                if isinstance (value, Name):
                    self._name = ns.ndnSIM.ndn.Name (value._name)
                else:
                    self._name = ns.ndnSIM.ndn.Name (value)
            else:
                self._name = ns.ndnSIM.ndn.Name ()

    @staticmethod
    def fromWire (wire):
        return Name (name = ns.ndnSIM.ndn.Wire.ToName (wire))

    def toWire (self):
        return ns.ndnSIM.ndn.Wire.FromName (self._name)

    def append (self, value):
        if isinstance (value, Name):
            self._name.append (value._name)
        else:
            self._name.append (value)
        return self
    
    def __getattr__ (self, name):
        return self._name.__getattribute__ (name)

    def __len__ (self):
        return self._name.size ()

    def __add__ (self, other):
        return self._name.append (other)

    def __getitem__(self, key):
        if isinstance (key, int):
            if abs(key) < self._name.size ():
                return self._name.get (key)
            else:
                raise IndexError ("index out of range")
        elif isinstance (key, slice):
            name = ns.ndnSIM.ndn.Name ()
            for component in xrange (*key.indices (self.size ())):
                name.append (self._name.get (component))
            return name
        else:
            raise ValueError("Unknown __getitem__ type: %s" % type (key))

    def __repr__ (self):
        return "ndn.Name('" + self._name.toUri () + "')"

    def __str__ (self):
        return self._name.toUri ()

    def __eq__ (self, other):
        return self._name == other._name

    def isPrefixOf (self, other):
        return self[:] == other[:len(self)]

    @property
    def keyName (self):
        return self
