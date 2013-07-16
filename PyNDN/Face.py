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

# from ns3.ndnSIM import ApiFace, Interest, Name

class Face (object):
    def __init__(self, node):
        self._face = ns.ndnSIM.ndn.ApiFace (node)

    def connect (self):
        pass

    def disconnect (self):
        pass

    def defer_verification (self, deferVerification = True):
        pass

    def expressInterestSimple (self, name, onData, onTimeout, template = None):
        
        pass
        # class TrivialExpressClosure (Closure.Closure):
        #     __slots__ = ["_baseName", "_onData", "_onTimeout"];

        #     def __init__ (self, baseName, onData, onTimeout):
        #         self._baseName = baseName
        #         self._onData = onData
        #         self._onTimeout = onTimeout

        #     def upcall(self, kind, upcallInfo):
        #         if (kind == Closure.UPCALL_CONTENT or
        #             kind == Closure.UPCALL_CONTENT_UNVERIFIED or
        #             kind == Closure.UPCALL_CONTENT_UNVERIFIED or
        #             kind == Closure.UPCALL_CONTENT_KEYMISSING or
        #             kind == Closure.UPCALL_CONTENT_RAW):
        #             return self._onData (self._baseName, upcallInfo.Interest, upcallInfo.ContentObject, kind)
        #         elif (kind == Closure.UPCALL_INTEREST_TIMED_OUT):
        #             return self._onTimeout (self._baseName, upcallInfo.Interest)
        #         return Closure.RESULT_OK

        # trivial_closure = TrivialExpressClosure (name, onData, onTimeout)
        # self.expressInterest (name, trivial_closure, template)

    def setInterestFilterSimple (self, name, onInterest, flags = None):
        pass
        # class TrivialFilterClosure (Closure.Closure):
        #     # __slots__ = ["_baseName", "_onInterest"];

        #     def __init__ (self, baseName, onInterest):
        #         self._baseName = baseName
        #         self._onInterest = onInterest

        #     def upcall(self, kind, upcallInfo):
        #         if (kind == Closure.UPCALL_INTEREST):
        #             return self._onInterest (self._baseName, upcallInfo.Interest)
        #         return Closure.RESULT_OK

        # trivial_closure = TrivialFilterClosure (name, onInterest)
        # self.setInterestFilter (name, trivial_closure, flags)

    def clearInterestFilter(self, name):
        pass

    # Blocking!
    def get(self, name, template = None, timeoutms = 3000):
    # self._acquire_lock("get")
    # try:
    #     return _ndn.get(self, name, template, timeoutms)
    # finally:
    #     self._release_lock("get")

    # def put(self, contentObject):
    #     self._acquire_lock("put")
    #     try:
    #     return _ndn.put(self, contentObject)
    #     finally:
    #     self._release_lock("put")

    @staticmethod
    def getDefaultKey():
        pass
    # return _ndn.get_default_key()

# class EventLoop(object):
#     def __init__(self, *handles):
#     self.running = False
#     self.fds = {}
#     for handle in handles:
#         self.fds[handle.fileno()] = handle
#         self.eventLock = threading.Lock ()
#         self.events = []

#     def execute (self, event):
#         self.eventLock.acquire ()
#         self.events.append (event)
#         self.eventLock.release ()

#     def run_scheduled(self):
#     wait = {}
#     for fd, handle in zip(self.fds.keys(), self.fds.values()):
#         wait[fd] = handle.process_scheduled()
#     return wait[sorted(wait, key=wait.get)[0]] / 1000.0

#     def run_once(self):
#     fd_read = self.fds.values()
#     fd_write = []
#     for handle in self.fds.values():
#         if handle.output_is_pending():
#         fd_write.append(handle)

#     timeout = min(self.run_scheduled(), 1.000)

#         res = select.select(fd_read, fd_write, [], timeout)

#     handles = set(res[0]).union(res[1])
#     for handle in handles:
#         handle.run(0)

#     def run(self):
#     self.running = True
#         while self.running:
#             try:
#                 self.eventLock.acquire ()
#                 for event in self.events:
#                     event ()
#                 self.events = []
#                 self.eventLock.release ()

#                 self.run_once()
#             except select.error, e:
#                 if e[0] == 4:
#                     continue
#                 else:
#                     raise
#         self.running = False

#     def stop(self):
#     self.running = False
#         for fd, handle in zip(self.fds.keys(), self.fds.values()):
#             handle.disconnect ()
