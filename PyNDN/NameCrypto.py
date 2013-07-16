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

from . import _ndn, Name

def new_state():
    return _ndn.nc_new_state()

def generate_application_key(fixture_key, app_name):
    app_id = _ndn.nc_app_id(app_name)
    app_key = _ndn.nc_app_key(fixture_key, app_id)
    return app_key

def authenticate_command(state, name, app_name, app_key):
    signed_name = _ndn.nc_authenticate_command(state, name.ccn_data, app_name, app_key)
    return Name(ccn_data = signed_name)

def authenticate_command_sig(state, name, app_name, key):
    signed_name = _ndn.nc_authenticate_command_sig(state, name.ccn_data, app_name, key.ccn_data_private)
    return Name(ccn_data = signed_name)

def verify_command(state, name, max_time, **args):
    if args.has_key('pub_key'): # TODO: use magic bytes to detect signature type, instead of asking caller to explicitly specify key type
        args['pub_key'] = args['pub_key'].ccn_data_public
    return _ndn.nc_verify_command(state, name.ccn_data, max_time, **args)
