## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('NDNabstraction', ['applications', 'core', 'network', 'internet', 'point-to-point'])
    module.includes = '.'
    module.source = [
        'model/ccn/ccn_charbuf.cc',
        'model/ccn/ccn_name_util.cc',
        'model/ccn/ccn_coding.cc',
        'model/ccn/ccn_indexbuf.cc',
        'model/ccn/ccn_random.cc',
        'model/ccn/ccn_buf_decoder.cc',
        'model/ccn/ccn_buf_encoder.cc',
        
        'model/name-components.cc',
        'model/interest-header.cc',
        'model/content-object-header.cc',

        'model/stupid-interest-generator.cc',
        'model/stupid-interest-sink.cc',
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

        # 'model/ndnabstraction-header.h',

        'model/name-components.h',
        'model/interest-header.h',
        'model/content-object-header.h',

        # 'helper/ndnabstraction-helper.h',
        'helper/ndn_stupidinterestgenerator_helper.h',

        'model/stupid-interest-generator.h',
        'model/stupid-interest-sink.h'
        ]


    if bld.env['ENABLE_OPENSSL']:
        module.uselib = 'OPENSSL'

    if bld.env['ENABLE_EXAMPLES']:
        bld.add_subdirs('examples')

    bld.ns3_python_bindings()
    #bld.env['CXXFLAGS']=[filter(lambda x: x not in bld.env, sublist) for sublist in [['-Wall'], ['-Werror'], ['-Wextra']]]
    # bld.env['CXXFLAGS']=[]
    # bld.env['CFLAGS']=[]
    

