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
                          'helper/*.cc',
                          'helper/ccnb-parser/*.cc',
                          'helper/ccnb-parser/visitors/*.cc',
                          'helper/ccnb-parser/syntax-tree/*.cc'])

    headers.source = bld.path.ant_glob(['model/*.h', 'apps/*.h', 
                          'helper/*.h',
                          'helper/ccnb-parser/*.h',
                          'helper/ccnb-parser/visitors/*.h',
                          'helper/ccnb-parser/syntax-tree/*.h'])

    headers.source = [x.path_from(bld.path) for x in headers.source]

    tests.source = bld.path.ant_glob('test/*.cc');

    if True or bld.env['ENABLE_EXAMPLES']:
        obj = bld.create_ns3_program('ccnx-test', ['NDNabstraction', 'internet'])
        obj.source = 'examples/ccnx-test.cc'
        
        obj = bld.create_ns3_program('ccnx-routing-simple', ['NDNabstraction', 'point-to-point-layout'])
        obj.source = 'examples/ccnx-routing-simple.cc'
        
        obj = bld.create_ns3_program('ccnx-grid', ['NDNabstraction', 'point-to-point-layout'])
        obj.source = 'examples/ccnx-grid.cc'

        obj = bld.create_ns3_program('syntactic-topology', ['NDNabstraction', 'point-to-point-layout'])
        obj.source = 'examples/syntactic-topology-ndnabstraction.cc'

        obj = bld.create_ns3_program('annotated-topology', ['NDNabstraction', 'point-to-point-layout'])
        obj.source = 'examples/annotated-topology-read-example.cc'

        obj = bld.create_ns3_program('interest-header-example', ['NDNabstraction'])
        obj.source = 'examples/interest-header-example.cc'

    #     for path in ["examples"]:
    #         anode = bld.path.find_dir (path)
    #         if not anode or not anode.is_child_of(bld.srcnode):
    #             raise Utils.WscriptError("Unable to use '%s' - either because \
    #             it's not a relative path"", or it's not child of \
    #            '%s'."%(name,bld.srcnode))
    #         bld.rescan(anode)
    #         for filename in bld.cache_dir_contents[anode.id]:
    #             if filename.startswith('.') or not filename.endswith(".cc"):
    #                 continue
    #             name = filename[:-len(".cc")]
    #             obj = bld.create_ns3_program(name, ['NDNabstraction'])
    #             obj.path = obj.path.find_dir (path)
    #             obj.source = filename
    #             obj.target = name
    #             obj.name = obj.target
    #             obj.uselib = 'BOOST BOOST_IOSTREAMS'

    # bld.ns3_python_bindings()
