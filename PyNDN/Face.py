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
import ns.ndnSIM

class Face (ns.ndnSIM.ndn.ApiFace):
    def __init__(self):
        self.nodeId = ns.core.Simulator.GetContext ()
        self.node = ns.core.NodeList.GetNode (self.nodeId)
        super(Face, self).__init__ (self.node)

    def connect (self):
        pass

    def disconnect (self):
        self.Shutdown ()

    def defer_verification (self, deferVerification = True):
        pass

    def expressInterestSimple (self, name, onData, onTimeout, template = None):
        if template:
            interest = ns.ndnSIM.ndn.Interest (template)
        else:
            interest = ns.ndnSIM.ndn.Interest ()

        interest.SetName (name)
        self.ExpressInterest (interest, onData, onTimeout)

    def setInterestFilterSimple (self, name, onInterest, flags = None):
        self.SetInterestFilter (name, onInterest)

    def clearInterestFilter(self, name):
        self.ClearInterestFilter (name)

    def get(self, name, template = None, timeoutms = 3000):
        raise ("NS-3 simulation cannot have syncrhonous operations")

    def put(self, data):
        self.Put (data)

    @staticmethod
    def getDefaultKey ():
        return None

class EventLoop(object):
    def execute (self, event):
        ns.core.Simulator.ScheduleNow (event)

    def run (self, timeoutMs):
        ns.core.Simulator.Stop (ns.core.MilliSeconds (timeoutMs))
        ns.core.Simulator.Run ()
        ns.core.Simulator.Destroy ()
