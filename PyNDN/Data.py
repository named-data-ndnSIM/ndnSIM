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
from Name import Name
from SignedInfo import SignedInfo

class DataError (Exception):
    pass

class Data (object):
    _data = None

    def __init__(self,
                 name = None, content = None, signed_info = None,
                 data = None):
        if data:
            if isinstance (data, Data):
                self._data = data._data
            elif isinstance (data, ns.ndnSIM.ndn.Data):
                self._data = data
            else:
                raise TypeError ("Invalid type supplied for 'data' parameter [%s]" % type (data))

            self.signedInfo = SignedInfo ()
            # timestamp
            self.signedInfo.freshnessSeconds = self._data.GetFreshness ().ToDouble (ns.core.Time.S)
            if self._data.GetKeyLocator ():
                self.signedInfo.keyLocator = Name (name = self._data.GetKeyLocator ())
        else:
            self._data = ns.ndnSIM.ndn.Data ()

            self.name = name
            self.content = content
            self.signedInfo = signed_info or SignedInfo ()

    @staticmethod
    def fromWire (wire):
        return Data (data = ns.ndnSIM.ndn.Wire.ToDataStr (bytes (wire)))

    def sign (self, key):
        """There is no actual signing in ndnSIM for now, but we will assign signature bits based on key"""
        self._data.SetSignature (key.fakeKey)

    def toWire (self):
        if self._data.GetSignature () == 0:
            raise DataError ("Data packet has not been signed, cannot create wire format")

        return ns.ndnSIM.ndn.Wire.FromDataStr (self._data)

    def verify_signature (self, key):
        """There is no actual signing in ndnSIM for now, but we will check if signature matches the key"""
        return True
        # return self._data.GetSignature () == key.fakeKey

    def __getattr__ (self, name):
        if name == "_data":
            return object.__getattr__ (self, name)

        elif name == "signedInfo":
            return object.__getattr__ (self, name)
        elif name == "name":
            return Name (self._data.GetName ())
        elif name == "scope":
            return self._data.GetScope ()
        elif name == "interestLifetime":
            return self._data.GetInterestLifetime ().ToDouble (ns.core.Time.S)
        elif name == "content":
            pkt = self._data.GetPayload ()
            return ns.ndnSIM.ndn.PacketToBuffer (pkt)
        else:
            return self._data.__getattribute__ (name)

    def __setattr__(self, name, value):
        if name == "_data":
            return object.__setattr__ (self, name, value)

        elif name == 'signedInfo':
            if value is None:
                return
            if isinstance (value, SignedInfo):
                object.__setattr__ (self, name, value)

                if value.timestamp:
                    # ?
                    pass
                if value.freshnessSeconds:
                    self._data.SetFreshness (ns.core.Seconds (value.freshnessSeconds))
                if value.keyLocator:
                    self._data.SetKeyLocator (value._name)
            else:
                raise TypeError ("signedInfo can be assigned either None or SignedInfo object, [%s] supplied" % type (value))
        elif name == "name":
            if value is None:
                return self._data.SetName (ns.ndnSIM.ndn.Name ())
            elif isinstance (value, Name):
                return self._data.SetName (value._name)
            elif isinstance (value, ns.ndnSIM.ndn.Name):
                return self._data.SetName (value)
            elif isinstance (value, str):
                return self._data.SetName (ns.ndnSIM.ndn.Name (value))
            else:
                raise ValueError ("Invalid name parameter")
        elif name == "content":
            if value is None:
                pkt = ns.network.Packet ()
                self._data.SetPayload (pkt)
            else:
                pkt = ns.ndnSIM.ndn.BufferToPacket (bytes (value))
                self._data.SetPayload (pkt)
        else:
            raise NameError ("Unknown attribute [%s]" % name)

    def __repr__(self):
        return "ndn.Data(%s; %s)" % str (self._data, self.signedInfo)
