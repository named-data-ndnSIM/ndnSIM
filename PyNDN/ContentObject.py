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

from . import _ndn
import utils

class ContentType(utils.Enum):
    _prefix = "ndn"

CONTENT_DATA = ContentType.new_flag('CONTENT_DATA', 0x0C04C0)
CONTENT_ENCR = ContentType.new_flag('CONTENT_ENCR', 0x10D091)
CONTENT_GONE = ContentType.new_flag('CONTENT_GONE', 0x18E344)
CONTENT_KEY = ContentType.new_flag('CONTENT_KEY', 0x28463F)
CONTENT_LINK = ContentType.new_flag('CONTENT_LINK', 0x2C834A)
CONTENT_NACK = ContentType.new_flag('CONTENT_NACK', 0x34008A)

class ContentObject(object):
    def __init__(self, name = None, content = None, signed_info = None):
        self.name = name
        self.content = content

        self.signedInfo = signed_info or SignedInfo()
        self.digestAlgorithm = None # Default

        # generated
        self.signature = None
        self.verified = False

        # py-ndn
        self.ccn = None # Reference to CCN object
        self.ccn_data_dirty = True
        self.ccn_data = None  # backing charbuf

    # this is the finalization step
    # must pass a key here, there is no "default key" because
    # an NDN Face is not required to create the content object
    # thus there is no access to the ccn library keystore.
    #
    def sign(self, key):
        self.ccn_data = _ndn.encode_ContentObject(self, self.name.ccn_data, \
            self.content, self.signedInfo.ccn_data, key)
        self.ccn_data_dirty = False

    def digest(self):
        return _ndn.digest_contentobject(self.ccn_data)

    def verify_content(self, handle):
        return _ndn.verify_content(handle.ccn_data, self.ccn_data)

    def verify_signature(self, key):
        return _ndn.verify_signature(self.ccn_data, key.ccn_data_public)

    def matchesInterest(self, interest):
        return _ndn.content_matches_interest(self.ccn_data, interest.ccn_data)

    def __setattr__(self, name, value):
        if name == 'name' or name == 'content' or name == 'signedInfo' or name == 'digestAlgorithm':
            self.ccn_data_dirty = True

        if name == 'content':
            object.__setattr__(self, name, _ndn.content_to_bytes(value))
        else:
            object.__setattr__(self, name, value)

    def __getattribute__(self, name):
        if name == "ccn_data":
            if object.__getattribute__(self, 'ccn_data_dirty'):
                raise _ndn.CCNContentObjectError("Call sign() to finalize \
                    before accessing ccn_data for a ContentObject")
        return object.__getattribute__(self, name)

    # Where do we support versioning and segmentation?

    def __str__(self):
        ret = []
        ret.append("Name: %s" % self.name)
        ret.append("Content: %r" % self.content)
        ret.append("DigestAlg: %r" % self.digestAlgorithm)
        ret.append("SignedInfo: %s" % self.signedInfo)
        ret.append("Signature: %s" % self.signature)
        return "\n".join(ret)

    def __repr__(self):
        args = []

        if self.name is not None:
            args += ["name=%r" % self.name]

        if self.content is not None:
            args += ["content=%r" % self.content]

        if self.signedInfo is not None:
            args += ["signed_info=%r" % self.signedInfo]

        if self.signature is not None:
            args += ["<signed>"]

        return "ndn.ContentObject(%s)" % ", ".join(args)

    def get_ccnb(self):
        return _ndn.dump_charbuf(self.ccn_data)

        @staticmethod
        def from_ccnb (ccnb):
                return _ndn.ContentObject_obj_from_ccn_buffer (ccnb)

class Signature(object):
    def __init__(self):
        self.digestAlgorithm = None
        self.witness = None
        self.signatureBits = None

        # py-ndn
        self.ccn_data_dirty = False
        self.ccn_data = None

    def __setattr__(self, name, value):
        if name == 'witness' or name == 'signatureBits' or name == 'digestAlgorithm':
            self.ccn_data_dirty = True
        object.__setattr__(self, name, value)

    def __getattribute__(self, name):
        if name == "ccn_data":
            if object.__getattribute__(self, 'ccn_data_dirty'):
                self.ccn_data = _ndn.Signature_obj_to_ccn(self)
                self.ccn_data_dirty = False
        return object.__getattribute__(self, name)

    def __str__(self):
        res = []
        res.append("digestAlgorithm = %s" % self.digestAlgorithm)
        res.append("witness = %s" % self.witness)
        res.append("signatureBits = %r" % self.signatureBits)
        return "\n".join(res)

class SignedInfo(object):
    def __init__(self, key_digest = None, key_locator = None, type = CONTENT_DATA,
            freshness = None, final_block = None, py_timestamp = None,
            timestamp = None):

        self.publisherPublicKeyDigest = key_digest

        if py_timestamp is not None:
            if timestamp:
                raise ValueError("You can define only timestamp or py_timestamp")
            self.timeStamp = utils.py2ccn_time(py_timestamp)
        else:
            self.timeStamp = timestamp

        self.type = type
        self.freshnessSeconds = freshness
        self.finalBlockID = final_block
        self.keyLocator = key_locator

        # py-ndn
        self.ccn_data_dirty = True
        self.ccn_data = None  # backing charbuf

    def __setattr__(self, name, value):
        if name != "ccn_data" and name != "ccn_data_dirty":
            self.ccn_data_dirty = True

        if name == "type" and type(value) is not ContentType:
            value = ContentType(value)

        object.__setattr__(self, name, value)

    def __getattribute__(self, name):
        if name == "ccn_data":
            if object.__getattribute__(self, 'ccn_data_dirty'):
                key_locator = self.keyLocator.ccn_data if self.keyLocator else None
                self.ccn_data = _ndn.SignedInfo_to_ccn(\
                    self.publisherPublicKeyDigest, self.type, self.timeStamp, \
                    self.freshnessSeconds or (-1), self.finalBlockID, key_locator)
                self.ccn_data_dirty = False

        if name == "py_timestamp":
            ts = self.timeStamp
            if ts is None:
                return None
            return None if ts is None else utils.ccn2py_time(ts)

        return object.__getattribute__(self, name)

    def __repr__(self):
        args = []

        if self.publisherPublicKeyDigest is not None:
            args += ["key_digest=%r" % self.publisherPublicKeyDigest]
        if self.keyLocator is not None:
            args += ["key_locator=%r" % self.keyLocator]
        if self.type is not None:
            args += ["type=%r" % self.type]
        if self.freshnessSeconds is not None:
            args += ["freshness=%r" % self.freshnessSeconds]
        if self.finalBlockID is not None:
            args += ["final_block=%r" % self.finalBlockID]
        if self.timeStamp is not None:
            args += ["py_timestamp=%r" % self.py_timestamp]

        return "ndn.SignedInfo(%s)" % ", ".join(args)

#
#
# These are not used in signing in Python (all the info needed is in SignedInfo)
# But it is here in case the parsing of the c library version of signing params
# is needed.

class SigningParams(object):
    CCN_SP_TEMPL_TIMESTAMP = 0x0001
    CCN_SP_TEMPL_FINAL_BLOCK_ID = 0x0002
    CCN_SP_TEMPL_FRESHNESS = 0x0004
    CCN_SP_TEMPL_KEY_LOCATOR = 0x0008
    CCN_SP_FINAL_BLOCK = 0x0010
    CCN_SP_OMIT_KEY_LOCATOR = 0x0020

    def __init__(self):
        self.flags;       # Use the CCN_SP flags above
        self.type;        # Content type, really should be somewhere else, it's not that related to signing
        self.freshness;

        # These three are only relevant, for now, if they are coming *from* a c object
        # otherwise, API version is filled in from CCN_SIGNING_PARAMS_INIT and
        # both template and key will come from the ContentObject's SignedInfo object
        self.apiVersion;
        self.template;    # SignedInfo referred to by this content object,
        self.key;         # Key to use - this should filled by a lookup against content object's signedinfo,

        # py-ndn
        self.ccn_data_dirty = False
        self.ccn_data = None  # backing ccn_signing_params

    def __setattr__(self, name, value):
        if name != "ccn_data" and name != "ccn_data_dirty":
            self.ccn_data_dirty = True
        object.__setattr__(self, name, value)

    def __getattribute__(self, name):
        if name == "ccn_data":
            if object.__getattribute__(self, 'ccn_data_dirty'):
                self.ccn_data = _ndn._ndn_SigningParams_to_ccn(self)
                self.ccn_data_dirty = False
        return object.__getattribute__(self, name)

    def __get_ccn(self):
        pass
        # Call ccn_signed_info_create
