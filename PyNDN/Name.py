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

def toWire (name):
    buf = ns.network.Buffer (ns.ndnSIM.ndn.Wire.FromNameSize (name))
    ns.ndnSIM.ndn.Wire.FromName (buf.Begin (), name)
    
    output = bytearray (buf.GetSize ())
    buf.CopyData (output, buf.GetSize ())
    
    return buf

import ns.ndnSIM
import ns.network

from copy import copy
import time, struct, random

class Name (ns.ndnSIM.ndn.Name):
    def __init__(self, name=None):
        super (Name, self).__init__ (name)

    @staticmethod
    def fromWire (wire):
        return ns.ndnSIM.ndn.Wire.ToName (wire)

    @staticmethod
    def toWire (name):
        buf = ns.network.Buffer ()
        buf.AddToStart (ns.ndnSIM.ndn.Wire.FromNameSize (name))
        ns.ndnSIM.ndn.Wire.FromName (buf.Begin (), name)

        output = bytearray (buf.GetSize ())
        buf.CopyData (output, buf.GetSize ())
        return output
    
    def get_ccnb(self):
        return _ndn.dump_charbuf(self.ccn_data)

    def __repr__(self):
        global NAME_NORMAL, NAME_ANY

        if self.type == NAME_NORMAL:
            return "ndn.Name('ccnx:" + _ndn.name_to_uri(self.ccn_data) + "')"
        elif self.type == NAME_ANY:
            return "ndn.Name(name_type=ndn.NAME_ANY)"
        else:
            raise ValueError("Name is of wrong type %d" % self.type)

    def __str__(self):
        global NAME_NORMAL, NAME_ANY

        if self.type == NAME_NORMAL:
            return _ndn.name_to_uri(self.ccn_data)
        elif self.type == NAME_ANY:
            return "<any>"
        else:
            raise ValueError("Name is of wrong type %d" % self.type)

    def __len__(self):
        return len(self.components)

    def __add__(self, other):
        return self.append(other)

    def __setattr__(self, name, value):
        raise TypeError("can't modify immutable instance")

    __delattr__ = __setattr__

    def __getattribute__(self, name):
        if name == "ccn_data":
            if object.__getattribute__(self, 'ccn_data_dirty'):
                self._setattr('ccn_data', _ndn.name_comps_to_ccn(self.components))
                self._setattr('ccn_data_dirty', False)
        return object.__getattribute__(self, name)

    def __getitem__(self, key):
        if type(key) is int:
            return self.components[key]
        elif type(key) is slice:
            return Name(self.components[key])
        else:
            raise ValueError("Unknown __getitem__ type: %s" % type(key))

    # def __setitem__(self, key, value):
    #     self.components[key] = value

    # def __delitem__(self, key):
    #     del self.components[key]

    # def __len__(self):
    #     return len(self.components)
