## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('NDNabstraction', ['internet', 'config-store', 'tools', 'point-to-point', 'wifi', 'mobility', 'applications', 'csma'])
    module.includes = '.'
    module.source = [
        'model/ndnabstraction-header.c',
        'helper/ndnabstraction-helper.c',
        ]

    module_test = bld.create_ns3_module_test_library('NDNabstraction')
    module_test.source = [
        'test/ndnabstraction-basictest.c',
		]

    headers = bld.new_task_gen('ns3header')
    headers.module = 'NDNabstraction'
    headers.source = [
        'model/ndnabstraction-header.h',
        'helper/ndnabstraction-helper.h',
        ]


    if bld.env['ENABLE_EXAMPLES']:
        bld.add_subdirs('examples')

    #bld.ns3_python_bindings()
