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

import struct

class Flag(int):
    __initialized = False
    _flags = None
    _prefix = None
    __flags_values__ = None

    @classmethod
    def initialize(cls):
        cls._flags = {}
        cls.__flags_values__ = {}
        cls.__initialized = True

    @classmethod
    def new_flag(cls, name, value):
        if not cls.__initialized:
            cls.initialize()

        cls._flags[value] = name

        obj = cls(value)
        cls.__flags_values__[value] = obj

        return obj

    def __new__(cls, value):
        if cls.__flags_values__.has_key(value):
            return cls.__flags_values__[value]

        return super(Flag, cls).__new__(cls, value)

    def generate_repr(self):
        val = long(self)
        flags = [name for i, name in self._flags.items() if i & val]
        return " | ".join(flags)

    def __repr__(self):
        if self._prefix:
            return self._prefix + "." + self.generate_repr()

        t = type(self)
        type_name = "%s.%s" % (t.__module__, t.__name__)
        return "<flags %s of type %s>" % (self.generate_repr(), type_name)

    def __and__(self, other):
        cls = type(self)
        return cls(long(self) & long(other))

    def __xor__(self, other):
        cls = type(self)
        return cls(long(self) ^ long(other))

    def __or__(self, other):
        cls = type(self)
        return cls(long(self) | long(other))

class Enum(Flag):
    def __new__(cls, value):
        if cls.__flags_values__.has_key(value):
            return cls.__flags_values__[value]

        if cls._flags.has_key(value):
            return super(Enum, cls).__new__(cls, value)

        raise ValueError("invalid flag value: %d" % value)

    def generate_repr(self):
        return self._flags[long(self)]

def ccn2py_time(value):
    bintime = b'\x00' * (8 - len(value)) + value
    inttime = struct.unpack("!Q", bintime)[0]
    return inttime / 4096.0

def py2ccn_time(value):
    inttime = int(value * 4096 + 0.5)
    bintime = struct.pack("!Q", inttime)
    return bintime.lstrip(b'\x00')

class Const (object):
    def __init__ (self, obj):
        object.__setattr__ (self, "__internal_object", obj)
        object.__setattr__ (self, "__bases__", type(obj))

    def __getattribute__ (self, name):
        if name != "__bases__":
            return object.__getattribute__ (self, "__internal_object").__getattribute__ (name)
        else:
            return object.__getattribute__ (self, name)

    def __getitem__(self, key):
        return object.__getattribute__ (self, "__internal_object").__getitem__ (key)

    def __setattr__ (self, name, value):
        raise TypeError ("Const %s cannot be modified" % type (object.__getattribute__ (self, "__internal_object")))

    def __repr__ (self):
        return "const %s" % object.__getattribute__ (self, "__internal_object").__repr__ ()

    def __str__(self):
       return object.__getattribute__ (self, "__internal_object").__str__ ()

    def __add__(self, other):
        raise TypeError ("Const %s cannot be modified" % type (object.__getattribute__ (self, "__internal_object")))

    def __delattr__ (self, name, value):
        raise TypeError ("Const %s cannot be modified" % type (object.__getattribute__ (self, "__internal_object")))

    def __setitem__(self, key, value):
        raise TypeError ("Const %s cannot be modified" % type (object.__getattribute__ (self, "__internal_object")))

    def __delitem__(self, key):
        raise TypeError ("Const %s cannot be modified" % type (object.__getattribute__ (self, "__internal_object")))
