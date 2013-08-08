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
import traceback

deleteList = []

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

        converter = ExpressInterestConverter (onData, onTimeout)
        deleteList.append (converter)

        self.ExpressInterest (interest._interest, converter.handleOnData, converter.handleOnTimeout)

    def setInterestFilter (self, name, onInterest, flags = None):
        """
        onInterest: void <name, interest>
        """

        if isinstance (name, Name):
            name = name._name
        elif isinstance (name, ns.ndnSIM.ndn.Name):
            pass
        else:
            raise TypeError ("Wrong type for 'name' parameter [%s]" % type (name))

        converter = OnInterestConvert (onInterest)
        deleteList.append (converter)

        self.SetInterestFilter (name, converter)

    def clearInterestFilter (self, name):
        if isinstance (name, Name):
            name = name._name
        elif isinstance (name, ns.ndnSIM.ndn.Name):
            pass
        else:
            raise TypeError ("Wrong type for 'name' parameter [%s]" % type (name))

        # @bug: memory leak, deleteList need to remove previosly set callback... but how?
        self.ClearInterestFilter (name)

    def get (self, name, template = None, timeoutms = 3000):
        raise NotImplementedError ("NS-3 simulation cannot have syncrhonous operations")

    def put (self, data):
        if isinstance (data, Data):
            self.Put (data._data)
        elif isinstance (data, ns.ndnSIM.ndn.Data):
            self.Put (data)
        else:
            raise TypeError ("Unsupported type to publish data [%s]" % type (data))

def removeFromDeleteList (object):
    try:
        deleteList.remove (object)
    except:
        pass

class ExpressInterestConverter:
    def __init__ (self, onData, onTimeout):
        self.onData = onData
        self.onTimeout = onTimeout

    def handleOnData (self, interest, data):
        ns.core.Simulator.ScheduleNow (removeFromDeleteList, self)
        try:
            if self.onData:
                return self.onData (Interest (interest=interest), Data (data = data))
        except Exception, e:
            traceback.print_exc()

    def handleOnTimeout (self, interest):
        ns.core.Simulator.ScheduleNow (removeFromDeleteList, self)
        try:
            if self.onTimeout:
                self.onTimeout (Interest (interest=interest))
        except Exception, e:
            traceback.print_exc()

class OnInterestConvert (object):
    def __init__ (self, onInterest):
        self.onInterest = onInterest
    def __call__ (self, name, interest):
        ns.core.Simulator.ScheduleNow (removeFromDeleteList, self)
        try:
            if self.onInterest:
                self.onInterest (Name (name = name), Interest (interest = interest))
        except Exception, e:
            traceback.print_exc()
