## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import os
import Logs
import Utils
import Options

from waflib.Errors import WafError

def options(opt):
    opt.tool_options('boost', tooldir=["waf-tools"])

def configure(conf):
    try:
        conf.check_tool('boost')
        conf.check_boost(lib = 'iostreams')
    except WafError:
        conf.env['LIB_BOOST'] = []

    if not conf.env['LIB_BOOST']:
        conf.report_optional_feature("ndn-abstract", "NDN abstraction", False,
                                     "Required boost libraries not found")
        conf.env['ENABLE_NDN_ABSTRACT']=False;
        conf.env['MODULES_NOT_BUILT'].append('NDNabstraction')
        return
    
    conf.env['ENABLE_NDN_ABSTRACT']=True;


def build(bld):
    deps = ['core', 'network', 'point-to-point',
            'topology-read','internet','applications',
            'point-to-point-layout', 'netanim']
    if bld.env['ENABLE_PYTHON_BINDINGS']:
        deps.append ('visualizer')

    module = bld.create_ns3_module ('NDNabstraction', deps)
    module.uselib = 'BOOST BOOST_IOSTREAMS'

    tests = bld.create_ns3_module_test_library('NDNabstraction')
    headers = bld.new_task_gen(features=['ns3header'])
    headers.module = 'NDNabstraction'

    if not bld.env['ENABLE_NDN_ABSTRACT']:
        bld.env['MODULES_NOT_BUILT'].append('NDNabstraction')
        return
   
    module.source = bld.path.ant_glob(['model/*.cc', 'apps/*.cc', 
                                       'utils/*.cc',
                                       'helper/*.cc',
                                       'helper/tracers/*.cc',
                                       'helper/ccnb-parser/*.cc',
                                       'helper/ccnb-parser/visitors/*.cc',
                                       'helper/ccnb-parser/syntax-tree/*.cc'])

    headers.source = [
        "helper/ccnx-stack-helper.h",
        "helper/ccnx-app-helper.h",
        "helper/ccnx-header-helper.h",
        "helper/ccnx-trace-helper.h",
        "helper/tracers/ipv4-app-tracer.h",
        "helper/tracers/ipv4-l3-tracer.h",
        "helper/tracers/ccnx-app-tracer.h",
        "helper/tracers/ccnx-l3-tracer.h",
        "helper/tracers/ccnx-consumer-window-tracer.h",
        "helper/tracers/ccnx-path-weight-tracer.h",
        "helper/ccnx-face-container.h",

        "apps/ccnx-app.h",

        "model/hash-helper.h",
        "model/ccnx.h",
        "model/ccnx-face.h",

        "model/ccnx-interest-header.h",
        "model/ccnx-content-object-header.h",
        "model/ccnx-name-components.h",
        "model/ccnx-path-stretch-tag.h",
        "model/ccnx-fib.h",

        "utils/spring-mobility-model.h",
        "utils/spring-mobility-helper.h",

        "model/rocketfuel-weights-reader.h",
        "model/annotated-topology-reader.h",

        "model/batches.h"
        ]

    tests.source = bld.path.ant_glob('test/*.cc');

    if True or bld.env['ENABLE_EXAMPLES']:
        obj = bld.create_ns3_program('ccnx-routing-simple', ['NDNabstraction'])
        obj.source = 'examples/ccnx-routing-simple.cc'
        
        obj = bld.create_ns3_program('ccnx-grid', ['NDNabstraction'])
        obj.source = 'examples/ccnx-grid.cc'

        obj = bld.create_ns3_program('annotated-topology', ['NDNabstraction'])
        obj.source = 'examples/annotated-topology-read-example.cc'

        obj = bld.create_ns3_program('ccnx-sprint-topology', ['NDNabstraction'])
        obj.source = 'examples/sprint-topology.cc'

        obj = bld.create_ns3_program('ccnx-abilene-topology', ['NDNabstraction'])
        obj.source = 'examples/abilene-topology.cc'

        obj = bld.create_ns3_program('ccnx-synthetic-topology', ['NDNabstraction'])
        obj.source = 'examples/synthetic-topology.cc'

        obj = bld.create_ns3_program('congestion-pop', ['NDNabstraction'])
        obj.source = 'examples/congestion-pop.cc'

        obj = bld.create_ns3_program('link-failure', ['NDNabstraction'])
        obj.source = 'examples/link-failure-sprint.cc'

        obj = bld.create_ns3_program('link-failure-base', ['NDNabstraction'])
        obj.source = 'examples/link-failure-base.cc'

        obj = bld.create_ns3_program('blackhole-sprint', ['NDNabstraction'])
        obj.source = 'examples/blackhole-sprint.cc'
        
        obj = bld.create_ns3_program('congestion-zoom', ['NDNabstraction'])
        obj.source = 'examples/congestion-zoom.cc'

        obj = bld.create_ns3_program('vanet-ccnx', ['NDNabstraction', 'highway-mobility'])
        obj.source = 'examples/vanet-ccnx.cc'

        obj = bld.create_ns3_program('car2car-wifi', ['internet', 'applications', 'visualizer', 'NDNabstraction'])
        obj.source = 'examples/car2car-wifi.cc'


    bld.ns3_python_bindings()
