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
import ns.network
import ns.ndnSIM
from Data import Data
from Interest import Interest
from Name import Name

import functools

class Face (ns.ndnSIM.ndn.ApiFace):
    def __init__(self):
        self.nodeId = ns.core.Simulator.GetContext ()
        self.node = ns.network.NodeList.GetNode (self.nodeId)
        super(Face, self).__init__ (self.node)

    def connect (self):
        pass

    def disconnect (self):
        self.Shutdown ()

    def defer_verification (self, deferVerification = True):
        pass

    def expressInterest (self, name, onData, onTimeout, template = None):
        """
        onData:    void <interest, name>
        onTimeout: void <interest>
        """

        interest = Interest (interest = template)
        interest.name = name

        class OnDataConvert:
            def __init__ (self, onData):
                self.onData = onData
            def __call__ (self, interest, data):
                if self.onData:
                    return self.onData (Interest (interest=interest), Data (data = data))

        class OnTimeoutConvert:
            def __init__ (self, onTimeout):
                self.onTimeout = onTimeout
            def __call__ (self, interest):
                if self.onTimeout:
                    self.onTimeout (Interest (interest=interest))

        self.ExpressInterest (interest._interest, OnDataConvert (onData), OnTimeoutConvert (onTimeout))

    def setInterestFilter (self, name, onInterest, flags = None):
        """
        onInterest: void <name, interest>
        """

        class OnInterestConvert:
            def __init__ (self, onInterest):
                self.onInterest = onInterest
            def __call__ (self, name, interest):
                if self.onInterest:
                    self.onInterest (Name (name = name), Interest (interest = interest))

        if isinstance (name, Name):
            name = name._name
        elif isinstance (name, ns.ndnSIM.ndn.Name):
            pass
        else:
            raise TypeError ("Wrong type for 'name' parameter [%s]" % type (name))

        self.SetInterestFilter (name, OnInterestConvert (onInterest))

    def clearInterestFilter (self, name):
        if isinstance (name, Name):
            name = name._name
        elif isinstance (name, ns.ndnSIM.ndn.Name):
            pass
        else:
            raise TypeError ("Wrong type for 'name' parameter [%s]" % type (name))

        self.ClearInterestFilter (name)

    def get (self, name, template = None, timeoutms = 3000):
        raise NotImplementedError ("NS-3 simulation cannot have syncrhonous operations")

    def put (self, data):
        self.Put (data)
