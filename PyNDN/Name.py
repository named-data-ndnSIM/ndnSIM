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
from . import _ndn

from copy import copy
import time, struct, random

NAME_NORMAL = 0
NAME_ANY    = 1

class Name(object):
    def __init__(self, components=[], name_type=NAME_NORMAL, ccn_data=None, ccnb_buffer=None):
        self._setattr('type', name_type)

        # py-ndn
        #self._setattr('ccn_data_dirty', True)
        self._setattr('ccn_data', ccn_data)

                # Name from simple buffer containing name in ccnb encoding
                if ccnb_buffer:
                        self._setattr('components', _ndn.name_comps_from_ccn_buffer (bytes (ccnb_buffer)))

        # Name from CCN
        elif ccn_data:
            self._setattr('components', _ndn.name_comps_from_ccn(ccn_data))
            self._setattr('ccn_data_dirty', False)

        # Copy Name from another Name object
        elif isinstance(components, self.__class__):
            self._setattr('components', copy(components.components))
            if not components.ccn_data_dirty:
                self._setattr('ccn_data', components.ccn_data)
                self._setattr('ccn_data_dirty', False)

        # Name as string (URI)
        elif type(components) is str:
            ccn_data = _ndn.name_from_uri(components)
            self._setattr('components', _ndn.name_comps_from_ccn(ccn_data))
            self._setattr('ccn_data', ccn_data)
            self._setattr('ccn_data_dirty', False)

        # Otherwise assume name is a list
        else:
            self._setattr('components', copy(components))

    def _setattr(self, name, value):
        if name == 'components' or name == 'ccn_data':
            self._setattr('ccn_data_dirty', True)
        super(Name, self).__setattr__(name, value)

    def _append(self, component):
        components = copy(self.components)
        components.append(component)

        return Name(components)

    def append(self, value):
        components = copy(self.components)
                if isinstance (value, Name):
                        components.extend (value.components)
                else:   
                        components.append (bytes (value))
        return Name(components)

    def appendKeyID(self, digest):
        if isinstance(digest, ndn.Key):
            digest = digest.publicKeyID

        component = b'\xc1.M.K\x00'
        component += digest

        return self._append(component)

    def appendVersion(self, version = None):
        if not version:
            inttime = int(time.time() * 4096 + 0.5)
            bintime = struct.pack("!Q", inttime)
            version = bintime.lstrip(b'\x00')
        component = b'\xfd' + version

        return self._append(component)

    def appendSegment(self, segment):
        return self._append(self.num2seg(segment))

    def appendNonce(self):
        val = random.getrandbits(64)
        component = b'\xc1.N\x00' + struct.pack("@Q", val)

        return self._append(component)

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

    def __setitem__(self, key, value):
        self.components[key] = value

    def __delitem__(self, key):
        del self.components[key]

    def __len__(self):
        return len(self.components)

    def __lt__(self, other):
        return _ndn.compare_names(self.ccn_data, other.ccn_data) < 0

    def __gt__(self, other):
        return _ndn.compare_names(self.ccn_data, other.ccn_data) > 0

    def __eq__(self, other):
        return _ndn.compare_names(self.ccn_data, other.ccn_data) == 0

    def __le__(self, other):
        return _ndn.compare_names(self.ccn_data, other.ccn_data) <= 0

    def __ge__(self, other):
        return _ndn.compare_names(self.ccn_data, other.ccn_data) >= 0

    def __ne__(self, other):
        return _ndn.compare_names(self.ccn_data, other.ccn_data) != 0

    @staticmethod
    def num2seg(num):
        return b'\x00' + struct.pack('!Q', num).lstrip(b'\x00')

    @staticmethod
    def seg2num(segment):
        return long(struct.unpack("!Q", (8 - len(segment)) * "\x00" + segment)[0])

        def isPrefixOf (self, other):
                return self[:] == other[:len(self)]
