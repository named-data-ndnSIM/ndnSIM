## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('NDNabstraction', ['applications', 'core', 'network', 'internet', 'point-to-point'])
    module.includes = '.'
    module.source = [
        'model/ccn/ccn_charbuf.c',
		'model/ccn/ccn_name_util.c',
		'model/ccn/ccn_coding.c',
		'model/ccn/ccn_indexbuf.c',
		'model/ccn/ccn_random.c',
		'model/ccn/ccn_buf_decoder.c',
		'model/ccn/ccn_buf_encoder.c',
        
        'model/ndnabstraction-header.cc',

        'model/ndn_face.cc',
        'model/ndn_interestpacket.cc',
        'model/ndn_contentpacket.cc',
        'model/ndn_timeoutheader.cc',
        'model/ndn_nonceheader.cc',
        'model/ndn_namebuilder.cc',
        'model/ndn_stupidinterestgenerator.cc',
        'helper/ndnabstraction-helper.cc',
        'helper/ndn_stupidinterestgenerator_helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('NDNabstraction')
    module_test.source = [
        'test/ndnabstraction-basictest.cc',
		]

    headers = bld.new_task_gen('ns3header')
    headers.module = 'NDNabstraction'
    headers.source = [
        'model/ccn/ccn.h',
		'model/ccn/ccn_charbuf.h',
		'model/ccn/ccn_coding.h',
		'model/ccn/ccn_name_util.h',
		'model/ccn/ccn_indexbuf.h',
		'model/ccn/ccn_random.h',

        'model/ndnabstraction-header.h',

        'model/ndn_face.h',
        'model/ndn_interestpacket.h',
        'model/ndn_contentpacket.h',
        'model/ndn_timeoutheader.h',
        'model/ndn_nonceheader.h',
        'model/ndn_namebuilder.h',
        'model/ndn_stupidinterestgenerator.h',
        'helper/ndnabstraction-helper.h',
        'helper/ndn_stupidinterestgenerator_helper.h',
        ]


    if bld.env['ENABLE_OPENSSL']:
        module.uselib      = 'OPENSSL'

    if bld.env['ENABLE_EXAMPLES']:
        bld.add_subdirs('examples')

    #bld.ns3_python_bindings()
    #bld.env['CXXFLAGS']=[filter(lambda x: x not in bld.env, sublist) for sublist in [['-Wall'], ['-Werror'], ['-Wextra']]]
    # bld.env['CXXFLAGS']=[]
    # bld.env['CFLAGS']=[]
    

