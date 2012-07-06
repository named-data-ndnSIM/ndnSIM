## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import os
import Logs
import Utils
import Options

from waflib.Errors import WafError

def options(opt):
    opt.tool_options('boost', tooldir=["waf-tools"])
    opt.add_option('--enable-ndn-plugins',
                   help=("Enable NDN plugins (may require patching)"),
                   dest='enable_ndn_plugins')

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
        conf.env['MODULES_NOT_BUILT'].append('ndnSIM')
        return

    conf.env['NDN_plugins'] = []
    if Options.options.enable_ndn_plugins:
        conf.env['NDN_plugins'] = Options.options.enable_ndn_plugins.split(',')
    
    conf.env['ENABLE_NDN_ABSTRACT']=True;


def build(bld):
    deps = ['core', 'network', 'point-to-point']
    deps.append ('internet') # Until RttEstimator is moved to network module
    if bld.env['ENABLE_PYTHON_BINDINGS']:
        deps.append ('visualizer')

    if 'topology' in bld.env['NDN_plugins']:
        deps.append ('topology-read')

    if 'mobility' in bld.env['NDN_plugins']:
        deps.append ('mobility')

    module = bld.create_ns3_module ('ndnSIM', deps)
    module.uselib = 'BOOST BOOST_IOSTREAMS'

    tests = bld.create_ns3_module_test_library('ndnSIM')
    headers = bld.new_task_gen(features=['ns3header'])
    headers.module = 'ndnSIM'

    if not bld.env['ENABLE_NDN_ABSTRACT']:
        bld.env['MODULES_NOT_BUILT'].append('ndnSIM')
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
        # "helper/ccnx-trace-helper.h",
        # "helper/tracers/ipv4-app-tracer.h",
        # "helper/tracers/ipv4-l3-tracer.h",
        # "helper/tracers/ccnx-app-tracer.h",
        # "helper/tracers/ccnx-l3-tracer.h",
        # "helper/tracers/ccnx-consumer-window-tracer.h",
        # "helper/tracers/ccnx-path-weight-tracer.h",
        "helper/ccnx-face-container.h",
        "helper/ccnx-global-routing-helper.h",

        "apps/ccnx-app.h",

        "model/ccnx.h",
        "model/ccnx-pit.h",
        "model/ccnx-pit-entry.h",
        "model/ccnx-pit-entry-incoming-face.h",
        "model/ccnx-pit-entry-outgoing-face.h",
        "model/ccnx-content-store.h",
        "model/ccnx-fib.h",
        "model/ccnx-fib-entry.h",
        "model/ccnx-face.h",
        "model/ccnx-app-face.h",
        "model/ccnx-net-device-face.h",
        "model/ccnx-interest-header.h",
        "model/ccnx-content-object-header.h",
        "model/ccnx-name-components.h",
        "model/ccnx-name-components-hash-helper.h",

        "utils/batches.h",
        # "utils/weights-path-stretch-tag.h",
        ]

    if 'topology' in bld.env['NDN_plugins']:
        headers.source.extend ([
            "plugins/topology/rocketfuel-weights-reader.h",
            "plugins/topology/annotated-topology-reader.h",
            ])
        module.source.extend (bld.path.ant_glob(['plugins/topology/*.cc']))

    if 'mobility' in bld.env['NDN_plugins']:
        headers.source.extend ([
            "plugins/mobility/spring-mobility-model.h",
            "plugins/mobility/spring-mobility-helper.h",
            ])
        module.source.extend (bld.path.ant_glob(['plugins/mobility/*.cc']))
    
    tests.source = bld.path.ant_glob('test/*.cc');

    if bld.env.ENABLE_EXAMPLES:
        bld.add_subdirs('examples')

    bld.ns3_python_bindings()
