from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

from pybindgen.typehandlers.smart_ptr import StdSharedPtr

from ns3_ptr import Ns3PtrMemoryPolicy

import pybindgen.settings
import warnings

import sys

def module_init():
    root_module = Module('ns.ndnSIM', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    module.add_class('ObjectBase', allow_subclassing=True, import_from_module='ns.core')
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core',
                     template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'],
                     parent=module['ns3::ObjectBase'],
                     memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    module.add_class('Object', import_from_module='ns.core', parent=module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])

    module.add_class('TypeId', import_from_module='ns.core')
    module.add_class('AttributeValue', import_from_module='ns.core')

    module.add_class('NodeContainer', import_from_module='ns.network')
    module.add_class('Node', import_from_module='ns.network', parent=module['ns3::Object'])
    module.add_class('ApplicationContainer', import_from_module='ns.network')

    def reg_ndn(module):
        module.add_class('StackHelper')
        module.add_class('FibHelper')
        module.add_class('StrategyChoiceHelper')
        module.add_class('AppHelper')
        module.add_class('GlobalRoutingHelper')

        module.add_class('L3Protocol', parent=module.get_root()['ns3::Object'])

        module.add_class('Name')
        module.add_class('Interest')
        module.add_class('Data')
        module.add_class('Face', memory_policy=StdSharedPtr('ns3::ndn::Face'))
        module.add_class('FaceContainer', memory_policy=Ns3PtrMemoryPolicy('::ns3::ndn::FaceContainer'))

        def reg_name(module):
            module.add_class('Component')
        reg_name(module.add_cpp_namespace('name'))

        def reg_nfd(module):
            module.add_class('Forwarder', memory_policy=StdSharedPtr('::ns3::ndn::nfd::Forwarder'), is_singleton=True)
            module.add_class('Fib')
            module.add_class('Pit')
            module.add_class('Cs')

            def reg_fib(module):
                module.add_class('Entry')#, memory_policy=StdSharedPtr('ns3::ndn::nfd::fib::Entry'))
                module.add_class('NextHop')
                module.add_class('NextHopList')
            reg_fib(module.add_cpp_namespace('fib'))

            def reg_pit(module):
                module.add_class('Entry')#, memory_policy=StdSharedPtr('ns3::ndn::nfd::pit::Entry'))
            reg_pit(module.add_cpp_namespace('pit'))

            def reg_cs(module):
                module.add_class('Entry')#, memory_policy=StdSharedPtr('ns3::ndn::nfd::cs::Entry'))
            reg_cs(module.add_cpp_namespace('cs'))

        reg_nfd(module.add_cpp_namespace('nfd'))
    reg_ndn(module.add_cpp_namespace('ndn'))

def register_methods(root_module):
    reg_other_modules(root_module)

    def reg_stackhelper(cls):
        cls.add_constructor([])

        cls.add_method('Install', 'ns3::Ptr<ns3::ndn::FaceContainer>', [param('ns3::Ptr<ns3::Node>', 'node')], is_const=True)
        cls.add_method('Install', 'ns3::Ptr<ns3::ndn::FaceContainer>', [param('std::string const&', 'nodeName')], is_const=True)
        cls.add_method('Install', 'ns3::Ptr<ns3::ndn::FaceContainer>', [param('const ns3::NodeContainer&', 'c')], is_const=True)
        cls.add_method('InstallAll', 'ns3::Ptr<ns3::ndn::FaceContainer>', [], is_const=True)

        cls.add_method('SetDefaultRoutes', retval('void'), [param('bool', 'isEnabled', default_value='true')], is_const=True)
        cls.add_method('SetStackAttributes',
                       retval('void'),
                       [param('const std::string&', 'attr1', default_value='""'), param('const std::string&', 'value1', default_value='""'),
                        param('const std::string&', 'attr2', default_value='""'), param('const std::string&', 'value2', default_value='""'),
                        param('const std::string&', 'attr3', default_value='""'), param('const std::string&', 'value3', default_value='""'),
                        param('const std::string&', 'attr4', default_value='""'), param('const std::string&', 'value4', default_value='""')])

        cls.add_method('setCsSize', retval('void'), [param('size_t', 'maxSize')])
        cls.add_method('SetOldContentStore',
                       retval('void'),
                       [param('const std::string&', 'contentStoreClass'),
                        param('const std::string&', 'attr1', default_value='""'), param('const std::string&', 'value1', default_value='""'),
                        param('const std::string&', 'attr2', default_value='""'), param('const std::string&', 'value2', default_value='""'),
                        param('const std::string&', 'attr3', default_value='""'), param('const std::string&', 'value3', default_value='""'),
                        param('const std::string&', 'attr4', default_value='""'), param('const std::string&', 'value4', default_value='""')])
    reg_stackhelper(root_module['ns3::ndn::StackHelper'])

    def reg_fibhelper(cls):
        cls.add_method('AddRoute', retval('void'), [
            param('const std::string&', 'nodeName'), param('const std::string&', 'prefix'),
            param('uint32_t', 'faceId'), param('int32_t', 'metric'),
            ], is_const=True, is_static=True)
        cls.add_method('AddRoute', retval('void'), [
            param('ns3::Ptr<ns3::Node>', 'node'), param('const ns3::ndn::Name&', 'prefix'),
            param('uint32_t', 'faceId'), param('int32_t', 'metric')
            ], is_const=True, is_static=True)
        cls.add_method('AddRoute', retval('void'), [
            param('ns3::Ptr<ns3::Node>', 'node'), param('const ns3::ndn::Name&', 'prefix'),
            param('std::shared_ptr<ns3::ndn::Face>', 'face'),
            param('int32_t', 'metric'),
            ], is_const=True, is_static=True)
        cls.add_method('AddRoute', retval('void'), [
            param('ns3::Ptr<ns3::Node>', 'node'), param('const ns3::ndn::Name&', 'prefix'),
            param('ns3::Ptr<ns3::Node>', 'otherNode'),
            param('int32_t', 'metric'),
            ], is_const=True, is_static=True)
        cls.add_method('AddRoute', retval('void'), [
            param('const std::string&', 'nodeName'), param('const std::string&', 'prefix'),
            param('const std::string&', 'otherNodeName'),
            param('int32_t', 'metric'),
            ], is_const=True, is_static=True)
    reg_fibhelper(root_module['ns3::ndn::FibHelper'])

    def reg_strategychoicehelper(cls):
        cls.add_method('Install', retval('void'), [param('ns3::Ptr<ns3::Node>', 'node'),
                                                   param('const const std::string&', 'name'),
                                                   param('const const std::string&', 'strategy')], is_const=True, is_static=True)
        cls.add_method('Install', retval('void'), [param('const ns3::NodeContainer&', 'c'),
                                                   param('const const std::string&', 'name'),
                                                   param('const const std::string&', 'strategy')], is_const=True, is_static=True)
        cls.add_method('InstallAll', retval('void'), [param('const std::string&', 'name'),
                                                      param('const std::string&', 'strategy')], is_const=True, is_static=True)
    reg_strategychoicehelper(root_module['ns3::ndn::StrategyChoiceHelper'])

    def reg_apphelper(cls):
        cls.add_constructor([param('const std::string&', 'prefix')])
        cls.add_method('SetPrefix', 'void', [param('const std::string&', 'prefix')])
        cls.add_method('SetAttribute', 'void', [param('std::string', 'name'), param('const ns3::AttributeValue&', 'value')])
        cls.add_method('Install', 'ns3::ApplicationContainer', [param('ns3::NodeContainer', 'c')])
        cls.add_method('Install', 'ns3::ApplicationContainer', [param('ns3::Ptr<ns3::Node>', 'node')])
        cls.add_method('Install', 'ns3::ApplicationContainer', [param('std::string', 'nodeName')])
    reg_apphelper(root_module['ns3::ndn::AppHelper'])

    def reg_GlobalRoutingHelper(cls):
        cls.add_constructor([])
        cls.add_method('Install', 'void', [param('ns3::Ptr<ns3::Node>', 'node')])
        cls.add_method('Install', 'void', [param('const ns3::NodeContainer&', 'nodes')])
        cls.add_method('InstallAll', 'void', [])
        cls.add_method('AddOrigin', 'void', [param('const std::string&', 'prefix'), param('ns3::Ptr<ns3::Node>', 'node')])
        cls.add_method('AddOrigin', 'void', [param('const std::string&', 'prefix'), param('const std::string&', 'nodeName')])
        cls.add_method('AddOrigins', 'void', [param('const std::string&', 'prefix'), param('const ns3::NodeContainer&', 'nodes')])
        cls.add_method('AddOriginsForAll', 'void', [])
        cls.add_method('CalculateRoutes', 'void', [])
        cls.add_method('CalculateAllPossibleRoutes', 'void', [])
    reg_GlobalRoutingHelper(root_module['ns3::ndn::GlobalRoutingHelper'])

    def reg_Name(root_module, cls):
        cls.implicitly_converts_to(root_module['ns3::ndn::Interest'])
        cls.add_output_stream_operator()
        for op in ['==', '!=', '<', '<=', '>', '>=']:
            cls.add_binary_comparison_operator(op)
        cls.add_container_traits(retval('const ns3::ndn::name::Component&'),
                                 begin_method='begin', end_method='end', iterator_type='const_iterator')

        cls.add_constructor([])
        cls.add_constructor([param('const ns3::ndn::Name&', 'other')])
        cls.add_constructor([param('const std::string&', 'url')])
        cls.add_method('append', 'ns3::ndn::Name &', [param('const ns3::ndn::name::Component&', 'comp')])
        cls.add_method('get', 'const ns3::ndn::name::Component&', [param('int', 'index')], is_const=True)
        cls.add_method('getPrefix', 'ns3::ndn::Name', [param('size_t', 'len')], is_const=True)
        cls.add_method('size', 'size_t', [], is_const=True)
        cls.add_method('toUri', retval('std::string'), [], is_const=True)
    reg_Name(root_module, root_module['ns3::ndn::Name'])

    def reg_NameComponent(cls):
        cls.add_output_stream_operator()
        for op in ['==', '!=', '<', '<=', '>', '>=']:
            cls.add_binary_comparison_operator(op)

        cls.add_constructor([param('const ns3::ndn::name::Component&', 'arg0')])
        cls.add_constructor([])
        cls.add_method('fromNumber', 'ns3::ndn::name::Component', [param('uint64_t', 'number')])
        cls.add_method('fromNumberWithMarker', 'ns3::ndn::name::Component', [param('uint64_t', 'number'), param('unsigned char', 'marker')])
        cls.add_method('fromEscapedString', 'ns3::ndn::name::Component', [param('const std::string&', 'uri')])
    reg_NameComponent(root_module['ns3::ndn::name::Component'])

    def reg_Interest(cls):
        cls.add_output_stream_operator()

        cls.add_constructor([param('const ns3::ndn::Interest&', 'interest')])
        cls.add_constructor([])
    reg_Interest(root_module['ns3::ndn::Interest'])

    def reg_Data(cls):
        cls.add_output_stream_operator()

        cls.add_constructor([param('const ns3::ndn::Data&', 'data')])
        cls.add_constructor([])
    reg_Data(root_module['ns3::ndn::Data'])

    #########################################################################################
    ## Interface to NFD
    #########################################################################################

    def register_L3Protocol(cls):
        cls.add_method('getL3Protocol', 'ns3::Ptr<ns3::ndn::L3Protocol>', [param('ns3::Ptr<ns3::Object>', 'node')], is_static=True)
        cls.add_method('getForwarder', 'std::shared_ptr<ns3::ndn::nfd::Forwarder>', [])
    register_L3Protocol(root_module['ns3::ndn::L3Protocol'])

    def reg_Face(cls):
        cls.add_output_stream_operator()
        cls.add_method('getId', retval('int64_t'), [], is_const=True)
    reg_Face(root_module['ns3::ndn::Face'])

    def reg_NfdForwarder(cls):
        cls.add_method('getFib', retval('const ns3::ndn::nfd::Fib&', caller_manages_return=False), [], is_const=True)
        cls.add_method('getPit', retval('const ns3::ndn::nfd::Pit&', caller_manages_return=False), [], is_const=True)
        cls.add_method('getCs', retval('const ns3::ndn::nfd::Cs&', caller_manages_return=False), [], is_const=True)
    reg_NfdForwarder(root_module['ns3::ndn::nfd::Forwarder'])

    #############
    #### FIB ####
    def reg_NfdFib(root_module, cls):
        cls.add_method('size', retval('size_t'), [], is_const=True)
        cls.add_container_traits(retval('const ns3::ndn::nfd::fib::Entry&', caller_manages_return=False),
                                 begin_method='begin', end_method='end', iterator_type='const_iterator')

        # The following is not supported
        # cls.add_method('findLongestPrefixMatch', retval('std::shared_ptr<ns3::ndn::nfd::fib::Entry>'),
        #                [param('const ns3::ndn::Name&', 'prefix')], is_const=True)
        # cls.add_method('findExactMatch', retval('std::shared_ptr<ns3::ndn::nfd::fib::Entry>'),
        #                [param('const ns3::ndn::Name&', 'prefix')], is_const=True)
        # cls.add_method('findLongestPrefixMatch', retval('shared_ptr<ns3::ndn::nfd::fib::Entry>'),
        #                [param('const pit::Entry&', 'pitEntry')], is_const=True)
        # cls.add_method('findLongestPrefixMatch', retval('shared_ptr<ns3::ndn::nfd::fib::Entry>'),
        #                [param('const measurements::Entry&', 'measurementsEntry')], is_const=True)

        # cls.add_method('insert', retval('std::pair<std::shared_ptr<ns3::ndn::nfd::fib::Entry>, bool>'), [param('const ns3::ndn::Name&', 'prefix')])
        cls.add_method('erase', retval('void'), [param('const ns3::ndn::Name&', 'prefix')])
        cls.add_method('erase', retval('void'), [param('const ns3::ndn::nfd::fib::Entry&', 'entry')])
        cls.add_method('removeNextHopFromAllEntries', retval('void'), [param('std::shared_ptr<ns3::ndn::Face>', 'face')])

        def reg_Entry(cls):
            cls.add_method('getPrefix', 'const ns3::ndn::Name&', [], is_const=True)
            cls.add_method('getNextHops', retval('const ns3::ndn::nfd::fib::NextHopList&', caller_manages_return=False), [], is_const=True)
            cls.add_method('hasNextHops', 'bool', [], is_const=True)
        reg_Entry(root_module['ns3::ndn::nfd::fib::Entry'])

        def reg_NextHop(cls):
            cls.add_constructor([param('std::shared_ptr<ns3::ndn::Face>', 'face')])

            cls.add_method('getFace', 'std::shared_ptr<ns3::ndn::Face>', [], is_const=True)
            cls.add_method('setCost', 'void', [param('uint64_t', 'cost')])
            cls.add_method('getCost', 'uint64_t', [], is_const=True)
        reg_NextHop(root_module['ns3::ndn::nfd::fib::NextHop'])

        def reg_NextHopList(cls):
            cls.add_method('size', retval('size_t'), [], is_const=True)
            cls.add_container_traits(retval('const ns3::ndn::nfd::fib::NextHop&', caller_manages_return=False),
                                     begin_method='begin', end_method='end', iterator_type='const_iterator')
        reg_NextHopList(root_module['ns3::ndn::nfd::fib::NextHopList'])
    reg_NfdFib(root_module, root_module['ns3::ndn::nfd::Fib'])
    #### FIB ####
    #############

    #############
    #### PIT ####
    def reg_NfdPit(root_module, cls):
        cls.add_method('size', retval('size_t'), [], is_const=True)
        cls.add_container_traits(retval('const ns3::ndn::nfd::pit::Entry&', caller_manages_return=False),
                                 begin_method='begin', end_method='end', iterator_type='const_iterator')

        def reg_Entry(cls):
            cls.add_method('getInterest', retval('const ns3::ndn::Interest&'), [], is_const=True)
            cls.add_method('getName', retval('const ns3::ndn::Name&'), [], is_const=True)
        reg_Entry(root_module['ns3::ndn::nfd::pit::Entry'])
    reg_NfdPit(root_module, root_module['ns3::ndn::nfd::Pit'])
    #### PIT ####
    #############

    #############
    #### CS ####
    def reg_NfdCs(root_module, cls):
        cls.add_method('size', retval('size_t'), [], is_const=True)
        cls.add_container_traits(retval('const ns3::ndn::nfd::cs::Entry&', caller_manages_return=False),
                                 begin_method='begin', end_method='end', iterator_type='const_iterator')

        def reg_Entry(cls):
            cls.add_method('getName', retval('const ns3::ndn::Name&'), [], is_const=True)
        reg_Entry(root_module['ns3::ndn::nfd::cs::Entry'])
    reg_NfdCs(root_module, root_module['ns3::ndn::nfd::Cs'])
    #### CS ####
    #############

def reg_other_modules(root_module):
    def reg_ApplicationContainer(cls):
        cls.add_constructor([])
        cls.add_constructor([param('ns3::ApplicationContainer', 'container')])
    reg_ApplicationContainer(root_module['ns3::ApplicationContainer'])

def register_functions(root_module):
    return

def main():
    out = FileCodeSink(sys.stdout)
    root_module = module_init()
    register_types(root_module)
    register_methods(root_module)
    register_functions(root_module)
    root_module.generate(out)

if __name__ == '__main__':
    main()
