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
from Name import Name

class Key (object):
    def __init__ (self):
        self.publicKeyID = None # SHA256 hash
        self.fakeKey = 0

    def generateRSA(self, numbits):
        randVar = ns.core.UniformVariable ()
        self.fakeKey = randVar.GetInteger (0, 2147483647)

    def privateToDER(self):
        return self.fakeKey

    def publicToDER(self):
        return self.privateToDER ()

    def privateToPEM(self, filename = None, password = None):
        if filename:
            f = open(filename, 'w')
            f.write (self.fakeKey)
            f.close()
        else:
            return self.fakeKey

    def publicToPEM(self, filename = None):
        return privateToPEM (filename)

    def fromDER(self, private = None, public = None):
        if private:
            self.fakeKey = hash(private)
        elif public:
            self.fakeKey = hash(public)

    def fromPEM(self, filename = None, private = None, public = None, password = None):
        if filename:
            f = open(filename, 'r')
            self.fakeKey = hash(f.read ())
            f.close()
        elif private:
            self.fakeKey = hash(private)
        elif public:
            self.fakeKey = hash(public)

    @staticmethod
    def createFromDER (private = None, public = None):
        key = Key ()
        key.fromDER (private, public)
        return key

    @staticmethod
    def createFromPEM (filename = None, private = None, public = None, password = None):
        key = Key ()
        key.fromPEM (filename, private, public, password)
        return key

    @staticmethod
    def getDefaultKey ():
        context = ns.core.Simulator.GetContext ()
        key = Key ()
        key.fakeKey = context
        return key
