## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('NDNabstraction', ['core', 'network'])
    module.includes = '.'
    module.source = [
        'model/ndnabstraction-header.cc',
        'model/ccn_charbuf.cc',
        'model/ccn_name_util.cc',
        'model/ccn_coding.cc',
        'model/ccn_indexbuf.cc',
        'model/ccn_random.cc',
        'model/ccn_buf_decoder.cc',
        'model/ccn_buf_encoder.cc',
        'model/ndn_face.cc',
        'model/ndn_interestpacket.cc',
        'model/ndn_contentpacket.cc',
        'model/ndn_timeoutheader.cc',
        'model/ndn_nonceheader.cc',
        'model/ndn_namebuilder.cc',
        'helper/ndnabstraction-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('NDNabstraction')
    module_test.source = [
        'test/ndnabstraction-basictest.cc',
		]

    headers = bld.new_task_gen('ns3header')
    headers.module = 'NDNabstraction'
    headers.source = [
        'model/ndnabstraction-header.h',
        'model/ccn_charbuf.h',
        'model/ccn_name_util.h',
        'model/ccn_coding.h',
        'model/ccn_indexbuf.h',
        'model/ccn_random.h',
        'model/ccn_ccn.h',
        'model/ndn_face.h',
        'model/ndn_interestpacket.h',
        'model/ndn_contentpacket.h',
        'model/ndn_timeoutheader.h',
        'model/ndn_nonceheader.h',
        'model/ndn_namebuilder.h',
        'helper/ndnabstraction-helper.h',
        ]


    if bld.env['ENABLE_OPENSSL']:
        module.uselib      = 'OPENSSL'

    if bld.env['ENABLE_EXAMPLES']:
        bld.add_subdirs('examples')

    #bld.ns3_python_bindings()
