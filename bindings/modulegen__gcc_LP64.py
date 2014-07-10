from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers


import pybindgen.settings
import warnings

class ErrorHandler(pybindgen.settings.ErrorHandler):
    def handle_error(self, wrapper, exception, traceback_):
        warnings.warn("exception %r in wrapper %s" % (exception, wrapper))
        return True
pybindgen.settings.error_handler = ErrorHandler()


import sys

def module_init():
    root_module = Module('ns.ndnSIM', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
    
    ## address.h (module 'network'): ns3::Address [class]
    module.add_class('Address', import_from_module='ns.network')
    ## address.h (module 'network'): ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'], import_from_module='ns.network')
    ## application-container.h (module 'network'): ns3::ApplicationContainer [class]
    module.add_class('ApplicationContainer', import_from_module='ns.network')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList [class]
    module.add_class('AttributeConstructionList', import_from_module='ns.core')
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item [struct]
    module.add_class('Item', import_from_module='ns.core', outer_class=root_module['ns3::AttributeConstructionList'])
    ## buffer.h (module 'network'): ns3::Buffer [class]
    module.add_class('Buffer', import_from_module='ns.network')
    ## buffer.h (module 'network'): ns3::Buffer::Iterator [class]
    module.add_class('Iterator', import_from_module='ns.network', outer_class=root_module['ns3::Buffer'])
    ## packet.h (module 'network'): ns3::ByteTagIterator [class]
    module.add_class('ByteTagIterator', import_from_module='ns.network')
    ## packet.h (module 'network'): ns3::ByteTagIterator::Item [class]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::ByteTagIterator'])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList [class]
    module.add_class('ByteTagList', import_from_module='ns.network')
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator [class]
    module.add_class('Iterator', import_from_module='ns.network', outer_class=root_module['ns3::ByteTagList'])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item [struct]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::ByteTagList::Iterator'])
    ## callback.h (module 'core'): ns3::CallbackBase [class]
    module.add_class('CallbackBase', import_from_module='ns.core')
    ## event-id.h (module 'core'): ns3::EventId [class]
    module.add_class('EventId', import_from_module='ns.core')
    ## hash.h (module 'core'): ns3::Hasher [class]
    module.add_class('Hasher', import_from_module='ns.core')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address [class]
    module.add_class('Ipv4Address', import_from_module='ns.network')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address [class]
    root_module['ns3::Ipv4Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask [class]
    module.add_class('Ipv4Mask', import_from_module='ns.network')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address [class]
    module.add_class('Ipv6Address', import_from_module='ns.network')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address [class]
    root_module['ns3::Ipv6Address'].implicitly_converts_to(root_module['ns3::Address'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix [class]
    module.add_class('Ipv6Prefix', import_from_module='ns.network')
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer [class]
    module.add_class('NetDeviceContainer', import_from_module='ns.network')
    ## node-container.h (module 'network'): ns3::NodeContainer [class]
    module.add_class('NodeContainer', import_from_module='ns.network')
    ## object-base.h (module 'core'): ns3::ObjectBase [class]
    module.add_class('ObjectBase', allow_subclassing=True, import_from_module='ns.core')
    ## object.h (module 'core'): ns3::ObjectDeleter [struct]
    module.add_class('ObjectDeleter', import_from_module='ns.core')
    ## object-factory.h (module 'core'): ns3::ObjectFactory [class]
    module.add_class('ObjectFactory', import_from_module='ns.core')
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata [class]
    module.add_class('PacketMetadata', import_from_module='ns.network')
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item [struct]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::PacketMetadata'])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item [enumeration]
    module.add_enum('', ['PAYLOAD', 'HEADER', 'TRAILER'], outer_class=root_module['ns3::PacketMetadata::Item'], import_from_module='ns.network')
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator [class]
    module.add_class('ItemIterator', import_from_module='ns.network', outer_class=root_module['ns3::PacketMetadata'])
    ## packet.h (module 'network'): ns3::PacketTagIterator [class]
    module.add_class('PacketTagIterator', import_from_module='ns.network')
    ## packet.h (module 'network'): ns3::PacketTagIterator::Item [class]
    module.add_class('Item', import_from_module='ns.network', outer_class=root_module['ns3::PacketTagIterator'])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList [class]
    module.add_class('PacketTagList', import_from_module='ns.network')
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData [struct]
    module.add_class('TagData', import_from_module='ns.network', outer_class=root_module['ns3::PacketTagList'])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData_e [enumeration]
    module.add_enum('TagData_e', ['MAX_SIZE'], outer_class=root_module['ns3::PacketTagList::TagData'], import_from_module='ns.network')
    ## random-variable.h (module 'core'): ns3::RandomVariable [class]
    module.add_class('RandomVariable', import_from_module='ns.core')
    ## rng-seed-manager.h (module 'core'): ns3::RngSeedManager [class]
    module.add_class('RngSeedManager', import_from_module='ns.core')
    ## sequence-number.h (module 'network'): ns3::SequenceNumber<unsigned int, int> [class]
    module.add_class('SequenceNumber32', import_from_module='ns.network')
    ## random-variable.h (module 'core'): ns3::SequentialVariable [class]
    module.add_class('SequentialVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simulator.h (module 'core'): ns3::Simulator [class]
    module.add_class('Simulator', destructor_visibility='private', import_from_module='ns.core')
    ## tag.h (module 'network'): ns3::Tag [class]
    module.add_class('Tag', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer [class]
    module.add_class('TagBuffer', import_from_module='ns.network')
    ## nstime.h (module 'core'): ns3::TimeWithUnit [class]
    module.add_class('TimeWithUnit', import_from_module='ns.core')
    ## traced-value.h (module 'core'): ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status> [class]
    module.add_class('TracedValue', template_parameters=['ns3::ndn::fib::FaceMetric::Status'])
    ## random-variable.h (module 'core'): ns3::TriangularVariable [class]
    module.add_class('TriangularVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## type-id.h (module 'core'): ns3::TypeId [class]
    module.add_class('TypeId', import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeFlag [enumeration]
    module.add_enum('AttributeFlag', ['ATTR_GET', 'ATTR_SET', 'ATTR_CONSTRUCT', 'ATTR_SGC'], outer_class=root_module['ns3::TypeId'], import_from_module='ns.core')
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation [struct]
    module.add_class('AttributeInformation', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation [struct]
    module.add_class('TraceSourceInformation', import_from_module='ns.core', outer_class=root_module['ns3::TypeId'])
    ## random-variable.h (module 'core'): ns3::UniformVariable [class]
    module.add_class('UniformVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::WeibullVariable [class]
    module.add_class('WeibullVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ZetaVariable [class]
    module.add_class('ZetaVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ZipfVariable [class]
    module.add_class('ZipfVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## empty.h (module 'core'): ns3::empty [class]
    module.add_class('empty', import_from_module='ns.core')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t [class]
    module.add_class('int64x64_t', import_from_module='ns.core')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::impl_type [enumeration]
    module.add_enum('impl_type', ['int128_impl', 'cairo_impl', 'ld_impl'], outer_class=root_module['ns3::int64x64_t'], import_from_module='ns.core')
    ## chunk.h (module 'network'): ns3::Chunk [class]
    module.add_class('Chunk', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## random-variable.h (module 'core'): ns3::ConstantVariable [class]
    module.add_class('ConstantVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::DeterministicVariable [class]
    module.add_class('DeterministicVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::EmpiricalVariable [class]
    module.add_class('EmpiricalVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ErlangVariable [class]
    module.add_class('ErlangVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable [class]
    module.add_class('ExponentialVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::GammaVariable [class]
    module.add_class('GammaVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## header.h (module 'network'): ns3::Header [class]
    module.add_class('Header', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## random-variable.h (module 'core'): ns3::IntEmpiricalVariable [class]
    module.add_class('IntEmpiricalVariable', import_from_module='ns.core', parent=root_module['ns3::EmpiricalVariable'])
    ## random-variable.h (module 'core'): ns3::LogNormalVariable [class]
    module.add_class('LogNormalVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h (module 'core'): ns3::NormalVariable [class]
    module.add_class('NormalVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## object.h (module 'core'): ns3::Object [class]
    module.add_class('Object', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    ## object.h (module 'core'): ns3::Object::AggregateIterator [class]
    module.add_class('AggregateIterator', import_from_module='ns.core', outer_class=root_module['ns3::Object'])
    ## random-variable.h (module 'core'): ns3::ParetoVariable [class]
    module.add_class('ParetoVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeChecker', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeChecker>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::CallbackImplBase', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CallbackImplBase>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::EventImpl', 'ns3::empty', 'ns3::DefaultDeleter<ns3::EventImpl>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Hash::Implementation', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Hash::Implementation>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::L2Tracer', 'ns3::empty', 'ns3::DefaultDeleter<ns3::L2Tracer>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::TopologyReader', 'ns3::empty', 'ns3::DefaultDeleter<ns3::TopologyReader>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::TraceSourceAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::TraceSourceAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::AppDelayTracer', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::AppDelayTracer>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::CsTracer', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::CsTracer>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::Data', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::Data>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::Exclude', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::Exclude>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::FaceContainer', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::FaceContainer>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::ndn::Interest', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::Interest>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::L3Tracer', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::L3Tracer>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::Name', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::Name>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::cs::Entry', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::cs::Entry>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::ndn::pit::Entry', 'ns3::empty', 'ns3::DefaultDeleter<ns3::ndn::pit::Entry>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## nstime.h (module 'core'): ns3::Time [class]
    module.add_class('Time', import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time::Unit [enumeration]
    module.add_enum('Unit', ['Y', 'D', 'H', 'MIN', 'S', 'MS', 'US', 'NS', 'PS', 'FS', 'LAST'], outer_class=root_module['ns3::Time'], import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time [class]
    root_module['ns3::Time'].implicitly_converts_to(root_module['ns3::int64x64_t'])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader [class]
    module.add_class('TopologyReader', import_from_module='ns.topology_read', parent=root_module['ns3::SimpleRefCount< ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> >'])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link [class]
    module.add_class('Link', import_from_module='ns.topology_read', outer_class=root_module['ns3::TopologyReader'])
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor [class]
    module.add_class('TraceSourceAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    ## trailer.h (module 'network'): ns3::Trailer [class]
    module.add_class('Trailer', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## annotated-topology-reader.h (module 'ndnSIM'): ns3::AnnotatedTopologyReader [class]
    module.add_class('AnnotatedTopologyReader', parent=root_module['ns3::TopologyReader'])
    ## application.h (module 'network'): ns3::Application [class]
    module.add_class('Application', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## attribute.h (module 'core'): ns3::AttributeAccessor [class]
    module.add_class('AttributeAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    ## attribute.h (module 'core'): ns3::AttributeChecker [class]
    module.add_class('AttributeChecker', allow_subclassing=False, automatic_type_narrowing=True, import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    ## attribute.h (module 'core'): ns3::AttributeValue [class]
    module.add_class('AttributeValue', allow_subclassing=False, automatic_type_narrowing=True, import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    ## boolean.h (module 'core'): ns3::BooleanChecker [class]
    module.add_class('BooleanChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## boolean.h (module 'core'): ns3::BooleanValue [class]
    module.add_class('BooleanValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## callback-based-app.h (module 'ndnSIM'): ns3::CallbackBasedApp [class]
    module.add_class('CallbackBasedApp', parent=root_module['ns3::Application'])
    ## callback.h (module 'core'): ns3::CallbackChecker [class]
    module.add_class('CallbackChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## callback.h (module 'core'): ns3::CallbackImplBase [class]
    module.add_class('CallbackImplBase', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    ## callback.h (module 'core'): ns3::CallbackValue [class]
    module.add_class('CallbackValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## double.h (module 'core'): ns3::DoubleValue [class]
    module.add_class('DoubleValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue [class]
    module.add_class('EmptyAttributeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## enum.h (module 'core'): ns3::EnumChecker [class]
    module.add_class('EnumChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## enum.h (module 'core'): ns3::EnumValue [class]
    module.add_class('EnumValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## event-impl.h (module 'core'): ns3::EventImpl [class]
    module.add_class('EventImpl', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    ## integer.h (module 'core'): ns3::IntegerValue [class]
    module.add_class('IntegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker [class]
    module.add_class('Ipv4AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue [class]
    module.add_class('Ipv4AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker [class]
    module.add_class('Ipv4MaskChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue [class]
    module.add_class('Ipv4MaskValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker [class]
    module.add_class('Ipv6AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue [class]
    module.add_class('Ipv6AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker [class]
    module.add_class('Ipv6PrefixChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue [class]
    module.add_class('Ipv6PrefixValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## l2-tracer.h (module 'ndnSIM'): ns3::L2Tracer [class]
    module.add_class('L2Tracer', parent=root_module['ns3::SimpleRefCount< ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> >'])
    ## net-device.h (module 'network'): ns3::NetDevice [class]
    module.add_class('NetDevice', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## net-device.h (module 'network'): ns3::NetDevice::PacketType [enumeration]
    module.add_enum('PacketType', ['PACKET_HOST', 'NS3_PACKET_HOST', 'PACKET_BROADCAST', 'NS3_PACKET_BROADCAST', 'PACKET_MULTICAST', 'NS3_PACKET_MULTICAST', 'PACKET_OTHERHOST', 'NS3_PACKET_OTHERHOST'], outer_class=root_module['ns3::NetDevice'], import_from_module='ns.network')
    ## nix-vector.h (module 'network'): ns3::NixVector [class]
    module.add_class('NixVector', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    ## node.h (module 'network'): ns3::Node [class]
    module.add_class('Node', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker [class]
    module.add_class('ObjectFactoryChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue [class]
    module.add_class('ObjectFactoryValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## packet.h (module 'network'): ns3::Packet [class]
    module.add_class('Packet', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker [class]
    module.add_class('RandomVariableChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue [class]
    module.add_class('RandomVariableValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): ns3::RocketfuelWeightsReader [class]
    module.add_class('RocketfuelWeightsReader', parent=root_module['ns3::AnnotatedTopologyReader'])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): ns3::RocketfuelWeightsReader [enumeration]
    module.add_enum('', ['LINKS', 'WEIGHTS', 'LATENCIES', 'POSITIONS'], outer_class=root_module['ns3::RocketfuelWeightsReader'])
    ## nstime.h (module 'core'): ns3::TimeValue [class]
    module.add_class('TimeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## type-id.h (module 'core'): ns3::TypeIdChecker [class]
    module.add_class('TypeIdChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## type-id.h (module 'core'): ns3::TypeIdValue [class]
    module.add_class('TypeIdValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## uinteger.h (module 'core'): ns3::UintegerValue [class]
    module.add_class('UintegerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## address.h (module 'network'): ns3::AddressChecker [class]
    module.add_class('AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## address.h (module 'network'): ns3::AddressValue [class]
    module.add_class('AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## l2-rate-tracer.h (module 'ndnSIM'): ns3::L2RateTracer [class]
    module.add_class('L2RateTracer', parent=root_module['ns3::L2Tracer'])
    module.add_container('std::map< std::string, std::string >', ('std::string', 'std::string'), container_type='map')
    module.add_container('std::list< ns3::TopologyReader::Link >', 'ns3::TopologyReader::Link', container_type='list')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >', 'ns3::SequenceNumber16')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >*', 'ns3::SequenceNumber16*')
    typehandlers.add_type_alias('ns3::SequenceNumber< short unsigned int, short int >&', 'ns3::SequenceNumber16&')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >', 'ns3::SequenceNumber32')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >*', 'ns3::SequenceNumber32*')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned int, int >&', 'ns3::SequenceNumber32&')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned char, signed char >', 'ns3::SequenceNumber8')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned char, signed char >*', 'ns3::SequenceNumber8*')
    typehandlers.add_type_alias('ns3::SequenceNumber< unsigned char, signed char >&', 'ns3::SequenceNumber8&')
    typehandlers.add_type_alias('ns3::RngSeedManager', 'ns3::SeedManager')
    typehandlers.add_type_alias('ns3::RngSeedManager*', 'ns3::SeedManager*')
    typehandlers.add_type_alias('ns3::RngSeedManager&', 'ns3::SeedManager&')
    module.add_typedef(root_module['ns3::RngSeedManager'], 'SeedManager')
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace Hash
    
    nested_module = module.add_cpp_namespace('Hash')
    register_types_ns3_Hash(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace ndn
    
    nested_module = module.add_cpp_namespace('ndn')
    register_types_ns3_ndn(nested_module)
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_Hash(module):
    root_module = module.get_root()
    
    ## hash-function.h (module 'core'): ns3::Hash::Implementation [class]
    module.add_class('Implementation', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >'])
    typehandlers.add_type_alias('uint64_t ( * ) ( char const *, size_t ) *', 'ns3::Hash::Hash64Function_ptr')
    typehandlers.add_type_alias('uint64_t ( * ) ( char const *, size_t ) **', 'ns3::Hash::Hash64Function_ptr*')
    typehandlers.add_type_alias('uint64_t ( * ) ( char const *, size_t ) *&', 'ns3::Hash::Hash64Function_ptr&')
    typehandlers.add_type_alias('uint32_t ( * ) ( char const *, size_t ) *', 'ns3::Hash::Hash32Function_ptr')
    typehandlers.add_type_alias('uint32_t ( * ) ( char const *, size_t ) **', 'ns3::Hash::Hash32Function_ptr*')
    typehandlers.add_type_alias('uint32_t ( * ) ( char const *, size_t ) *&', 'ns3::Hash::Hash32Function_ptr&')
    
    ## Register a nested module for the namespace Function
    
    nested_module = module.add_cpp_namespace('Function')
    register_types_ns3_Hash_Function(nested_module)
    

def register_types_ns3_Hash_Function(module):
    root_module = module.get_root()
    
    ## hash-fnv.h (module 'core'): ns3::Hash::Function::Fnv1a [class]
    module.add_class('Fnv1a', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash32 [class]
    module.add_class('Hash32', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash64 [class]
    module.add_class('Hash64', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])
    ## hash-murmur3.h (module 'core'): ns3::Hash::Function::Murmur3 [class]
    module.add_class('Murmur3', import_from_module='ns.core', parent=root_module['ns3::Hash::Implementation'])

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_ndn(module):
    root_module = module.get_root()
    
    ## ndn-app.h (module 'ndnSIM'): ns3::ndn::App [class]
    module.add_class('App', parent=root_module['ns3::Application'])
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): ns3::ndn::AppDelayTracer [class]
    module.add_class('AppDelayTracer', parent=root_module['ns3::SimpleRefCount< ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> >'])
    ## ndn-app-helper.h (module 'ndnSIM'): ns3::ndn::AppHelper [class]
    module.add_class('AppHelper')
    ## blob.h (module 'ndnSIM'): ns3::ndn::Blob [class]
    module.add_class('Blob')
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::ContentStore [class]
    module.add_class('ContentStore', parent=root_module['ns3::Object'])
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::CsTracer [class]
    module.add_class('CsTracer', parent=root_module['ns3::SimpleRefCount< ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> >'])
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::Data [class]
    module.add_class('Data', parent=root_module['ns3::SimpleRefCount< ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> >'])
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::DataException [class]
    module.add_class('DataException')
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude [class]
    module.add_class('Exclude', parent=root_module['ns3::SimpleRefCount< ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> >'])
    ## ndn-face.h (module 'ndnSIM'): ns3::ndn::Face [class]
    module.add_class('Face', parent=root_module['ns3::Object'])
    ## ndn-face.h (module 'ndnSIM'): ns3::ndn::Face::Flags [enumeration]
    module.add_enum('Flags', ['APPLICATION'], outer_class=root_module['ns3::ndn::Face'])
    ## ndn-face-container.h (module 'ndnSIM'): ns3::ndn::FaceContainer [class]
    module.add_class('FaceContainer', parent=root_module['ns3::SimpleRefCount< ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> >'])
    ## ndn-fib.h (module 'ndnSIM'): ns3::ndn::Fib [class]
    module.add_class('Fib', parent=root_module['ns3::Object'])
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): ns3::ndn::ForwardingStrategy [class]
    module.add_class('ForwardingStrategy', parent=root_module['ns3::Object'])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): ns3::ndn::GlobalRoutingHelper [class]
    module.add_class('GlobalRoutingHelper')
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::HeaderHelper [class]
    module.add_class('HeaderHelper')
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::HeaderHelper::Type [enumeration]
    module.add_enum('Type', ['INTEREST_CCNB', 'CONTENT_OBJECT_CCNB', 'INTEREST_NDNSIM', 'CONTENT_OBJECT_NDNSIM'], outer_class=root_module['ns3::ndn::HeaderHelper'])
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::Interest [class]
    module.add_class('Interest', parent=root_module['ns3::SimpleRefCount< ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> >'])
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::Interest [enumeration]
    module.add_enum('', ['NORMAL_INTEREST', 'NACK_LOOP', 'NACK_CONGESTION', 'NACK_GIVEUP_PIT'], outer_class=root_module['ns3::ndn::Interest'])
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::InterestException [class]
    module.add_class('InterestException')
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): ns3::ndn::IpFacesHelper [class]
    module.add_class('IpFacesHelper')
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::ndn::L3Protocol [class]
    module.add_class('L3Protocol', parent=root_module['ns3::Object'])
    ## ndn-l3-tracer.h (module 'ndnSIM'): ns3::ndn::L3Tracer [class]
    module.add_class('L3Tracer', parent=root_module['ns3::SimpleRefCount< ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> >'])
    ## ndn-limits.h (module 'ndnSIM'): ns3::ndn::Limits [class]
    module.add_class('Limits', parent=root_module['ns3::Object'])
    ## ndn-link-control-helper.h (module 'ndnSIM'): ns3::ndn::LinkControlHelper [class]
    module.add_class('LinkControlHelper')
    ## name.h (module 'ndnSIM'): ns3::ndn::Name [class]
    module.add_class('Name', parent=root_module['ns3::SimpleRefCount< ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> >'])
    ## name.h (module 'ndnSIM'): ns3::ndn::NameChecker [class]
    module.add_class('NameChecker', parent=root_module['ns3::AttributeChecker'])
    ## name.h (module 'ndnSIM'): ns3::ndn::NameValue [class]
    module.add_class('NameValue', parent=root_module['ns3::AttributeValue'])
    ## ndn-net-device-face.h (module 'ndnSIM'): ns3::ndn::NetDeviceFace [class]
    module.add_class('NetDeviceFace', parent=root_module['ns3::ndn::Face'])
    ## ndn-pit.h (module 'ndnSIM'): ns3::ndn::Pit [class]
    module.add_class('Pit', parent=root_module['ns3::Object'])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttEstimator [class]
    module.add_class('RttEstimator', parent=root_module['ns3::Object'])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory [class]
    module.add_class('RttHistory')
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::ndn::StackHelper [class]
    module.add_class('StackHelper')
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::UnknownHeaderException [class]
    module.add_class('UnknownHeaderException')
    ## ndn-wire.h (module 'ndnSIM'): ns3::ndn::Wire [struct]
    module.add_class('Wire')
    ## ndn-wire.h (module 'ndnSIM'): ns3::ndn::Wire [enumeration]
    module.add_enum('', ['WIRE_FORMAT_DEFAULT', 'WIRE_FORMAT_AUTODETECT', 'WIRE_FORMAT_NDNSIM', 'WIRE_FORMAT_CCNB'], outer_class=root_module['ns3::ndn::Wire'])
    ## ndn-api-face.h (module 'ndnSIM'): ns3::ndn::ApiFace [class]
    module.add_class('ApiFace', parent=root_module['ns3::ndn::Face'])
    ## ndn-app-face.h (module 'ndnSIM'): ns3::ndn::AppFace [class]
    module.add_class('AppFace', parent=root_module['ns3::ndn::Face'])
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): ns3::ndn::L3AggregateTracer [class]
    module.add_class('L3AggregateTracer', parent=root_module['ns3::ndn::L3Tracer'])
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): ns3::ndn::L3RateTracer [class]
    module.add_class('L3RateTracer', parent=root_module['ns3::ndn::L3Tracer'])
    module.add_container('std::vector< char >', 'char', container_type='vector')
    module.add_container('std::map< ns3::ndn::name::Component, bool, std::greater< ns3::ndn::name::Component >, std::allocator< std::pair< ns3::ndn::name::Component const, bool > > >', ('ns3::ndn::name::Component', 'bool'), container_type='map')
    module.add_container('std::vector< ns3::Ptr< ns3::ndn::Face > >', 'ns3::Ptr< ns3::ndn::Face >', container_type='vector')
    typehandlers.add_type_alias('ns3::ndn::Name', 'ns3::ndn::NameComponents')
    typehandlers.add_type_alias('ns3::ndn::Name*', 'ns3::ndn::NameComponents*')
    typehandlers.add_type_alias('ns3::ndn::Name&', 'ns3::ndn::NameComponents&')
    module.add_typedef(root_module['ns3::ndn::Name'], 'NameComponents')
    typehandlers.add_type_alias('ns3::ndn::Data', 'ns3::ndn::DataHeader')
    typehandlers.add_type_alias('ns3::ndn::Data*', 'ns3::ndn::DataHeader*')
    typehandlers.add_type_alias('ns3::ndn::Data&', 'ns3::ndn::DataHeader&')
    module.add_typedef(root_module['ns3::ndn::Data'], 'DataHeader')
    typehandlers.add_type_alias('std::deque< ns3::ndn::RttHistory, std::allocator< ns3::ndn::RttHistory > >', 'ns3::ndn::RttHistory_t')
    typehandlers.add_type_alias('std::deque< ns3::ndn::RttHistory, std::allocator< ns3::ndn::RttHistory > >*', 'ns3::ndn::RttHistory_t*')
    typehandlers.add_type_alias('std::deque< ns3::ndn::RttHistory, std::allocator< ns3::ndn::RttHistory > >&', 'ns3::ndn::RttHistory_t&')
    typehandlers.add_type_alias('ns3::Time', 'ns3::ndn::TimeInterval')
    typehandlers.add_type_alias('ns3::Time*', 'ns3::ndn::TimeInterval*')
    typehandlers.add_type_alias('ns3::Time&', 'ns3::ndn::TimeInterval&')
    module.add_typedef(root_module['ns3::Time'], 'TimeInterval')
    typehandlers.add_type_alias('ns3::ndn::Interest', 'ns3::ndn::InterestHeader')
    typehandlers.add_type_alias('ns3::ndn::Interest*', 'ns3::ndn::InterestHeader*')
    typehandlers.add_type_alias('ns3::ndn::Interest&', 'ns3::ndn::InterestHeader&')
    module.add_typedef(root_module['ns3::ndn::Interest'], 'InterestHeader')
    
    ## Register a nested module for the namespace cs
    
    nested_module = module.add_cpp_namespace('cs')
    register_types_ns3_ndn_cs(nested_module)
    
    
    ## Register a nested module for the namespace fib
    
    nested_module = module.add_cpp_namespace('fib')
    register_types_ns3_ndn_fib(nested_module)
    
    
    ## Register a nested module for the namespace fw
    
    nested_module = module.add_cpp_namespace('fw')
    register_types_ns3_ndn_fw(nested_module)
    
    
    ## Register a nested module for the namespace name
    
    nested_module = module.add_cpp_namespace('name')
    register_types_ns3_ndn_name(nested_module)
    
    
    ## Register a nested module for the namespace pit
    
    nested_module = module.add_cpp_namespace('pit')
    register_types_ns3_ndn_pit(nested_module)
    
    
    ## Register a nested module for the namespace time
    
    nested_module = module.add_cpp_namespace('time')
    register_types_ns3_ndn_time(nested_module)
    

def register_types_ns3_ndn_cs(module):
    root_module = module.get_root()
    
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::cs::Entry [class]
    module.add_class('Entry', parent=root_module['ns3::SimpleRefCount< ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> >'])
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::cs::Stats [struct]
    module.add_class('Stats')

def register_types_ns3_ndn_fib(module):
    root_module = module.get_root()
    
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry [class]
    module.add_class('Entry', parent=root_module['ns3::Object'])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::NoFaces [class]
    module.add_class('NoFaces', outer_class=root_module['ns3::ndn::fib::Entry'])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetric [class]
    module.add_class('FaceMetric')
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetric::Status [enumeration]
    module.add_enum('Status', ['NDN_FIB_GREEN', 'NDN_FIB_YELLOW', 'NDN_FIB_RED'], outer_class=root_module['ns3::ndn::fib::FaceMetric'])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetricContainer [struct]
    module.add_class('FaceMetricContainer')
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_face [class]
    module.add_class('i_face')
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_metric [class]
    module.add_class('i_metric')
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_nth [class]
    module.add_class('i_nth')

def register_types_ns3_ndn_fw(module):
    root_module = module.get_root()
    
    ## ndn-fw-tag.h (module 'ndnSIM'): ns3::ndn::fw::Tag [class]
    module.add_class('Tag')

def register_types_ns3_ndn_name(module):
    root_module = module.get_root()
    
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component [class]
    module.add_class('Component', parent=root_module['ns3::ndn::Blob'])

def register_types_ns3_ndn_pit(module):
    root_module = module.get_root()
    
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::pit::Entry [class]
    module.add_class('Entry', parent=root_module['ns3::SimpleRefCount< ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> >'])
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::pit::EntryIsNotEmpty [struct]
    module.add_class('EntryIsNotEmpty')
    ## ndn-pit-entry-incoming-face.h (module 'ndnSIM'): ns3::ndn::pit::IncomingFace [struct]
    module.add_class('IncomingFace')
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace [struct]
    module.add_class('OutgoingFace')
    module.add_container('std::set< ns3::ndn::pit::IncomingFace >', 'ns3::ndn::pit::IncomingFace', container_type='set')
    module.add_container('std::set< ns3::ndn::pit::OutgoingFace >', 'ns3::ndn::pit::OutgoingFace', container_type='set')
    module.add_container('std::set< unsigned int >', 'unsigned int', container_type='set')

def register_types_ns3_ndn_time(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3ApplicationContainer_methods(root_module, root_module['ns3::ApplicationContainer'])
    register_Ns3AttributeConstructionList_methods(root_module, root_module['ns3::AttributeConstructionList'])
    register_Ns3AttributeConstructionListItem_methods(root_module, root_module['ns3::AttributeConstructionList::Item'])
    register_Ns3Buffer_methods(root_module, root_module['ns3::Buffer'])
    register_Ns3BufferIterator_methods(root_module, root_module['ns3::Buffer::Iterator'])
    register_Ns3ByteTagIterator_methods(root_module, root_module['ns3::ByteTagIterator'])
    register_Ns3ByteTagIteratorItem_methods(root_module, root_module['ns3::ByteTagIterator::Item'])
    register_Ns3ByteTagList_methods(root_module, root_module['ns3::ByteTagList'])
    register_Ns3ByteTagListIterator_methods(root_module, root_module['ns3::ByteTagList::Iterator'])
    register_Ns3ByteTagListIteratorItem_methods(root_module, root_module['ns3::ByteTagList::Iterator::Item'])
    register_Ns3CallbackBase_methods(root_module, root_module['ns3::CallbackBase'])
    register_Ns3EventId_methods(root_module, root_module['ns3::EventId'])
    register_Ns3Hasher_methods(root_module, root_module['ns3::Hasher'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3NetDeviceContainer_methods(root_module, root_module['ns3::NetDeviceContainer'])
    register_Ns3NodeContainer_methods(root_module, root_module['ns3::NodeContainer'])
    register_Ns3ObjectBase_methods(root_module, root_module['ns3::ObjectBase'])
    register_Ns3ObjectDeleter_methods(root_module, root_module['ns3::ObjectDeleter'])
    register_Ns3ObjectFactory_methods(root_module, root_module['ns3::ObjectFactory'])
    register_Ns3PacketMetadata_methods(root_module, root_module['ns3::PacketMetadata'])
    register_Ns3PacketMetadataItem_methods(root_module, root_module['ns3::PacketMetadata::Item'])
    register_Ns3PacketMetadataItemIterator_methods(root_module, root_module['ns3::PacketMetadata::ItemIterator'])
    register_Ns3PacketTagIterator_methods(root_module, root_module['ns3::PacketTagIterator'])
    register_Ns3PacketTagIteratorItem_methods(root_module, root_module['ns3::PacketTagIterator::Item'])
    register_Ns3PacketTagList_methods(root_module, root_module['ns3::PacketTagList'])
    register_Ns3PacketTagListTagData_methods(root_module, root_module['ns3::PacketTagList::TagData'])
    register_Ns3RandomVariable_methods(root_module, root_module['ns3::RandomVariable'])
    register_Ns3RngSeedManager_methods(root_module, root_module['ns3::RngSeedManager'])
    register_Ns3SequenceNumber32_methods(root_module, root_module['ns3::SequenceNumber32'])
    register_Ns3SequentialVariable_methods(root_module, root_module['ns3::SequentialVariable'])
    register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    register_Ns3Simulator_methods(root_module, root_module['ns3::Simulator'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
    register_Ns3TimeWithUnit_methods(root_module, root_module['ns3::TimeWithUnit'])
    register_Ns3TracedValue__Ns3NdnFibFaceMetricStatus_methods(root_module, root_module['ns3::TracedValue< ns3::ndn::fib::FaceMetric::Status >'])
    register_Ns3TriangularVariable_methods(root_module, root_module['ns3::TriangularVariable'])
    register_Ns3TypeId_methods(root_module, root_module['ns3::TypeId'])
    register_Ns3TypeIdAttributeInformation_methods(root_module, root_module['ns3::TypeId::AttributeInformation'])
    register_Ns3TypeIdTraceSourceInformation_methods(root_module, root_module['ns3::TypeId::TraceSourceInformation'])
    register_Ns3UniformVariable_methods(root_module, root_module['ns3::UniformVariable'])
    register_Ns3WeibullVariable_methods(root_module, root_module['ns3::WeibullVariable'])
    register_Ns3ZetaVariable_methods(root_module, root_module['ns3::ZetaVariable'])
    register_Ns3ZipfVariable_methods(root_module, root_module['ns3::ZipfVariable'])
    register_Ns3Empty_methods(root_module, root_module['ns3::empty'])
    register_Ns3Int64x64_t_methods(root_module, root_module['ns3::int64x64_t'])
    register_Ns3Chunk_methods(root_module, root_module['ns3::Chunk'])
    register_Ns3ConstantVariable_methods(root_module, root_module['ns3::ConstantVariable'])
    register_Ns3DeterministicVariable_methods(root_module, root_module['ns3::DeterministicVariable'])
    register_Ns3EmpiricalVariable_methods(root_module, root_module['ns3::EmpiricalVariable'])
    register_Ns3ErlangVariable_methods(root_module, root_module['ns3::ErlangVariable'])
    register_Ns3ExponentialVariable_methods(root_module, root_module['ns3::ExponentialVariable'])
    register_Ns3GammaVariable_methods(root_module, root_module['ns3::GammaVariable'])
    register_Ns3Header_methods(root_module, root_module['ns3::Header'])
    register_Ns3IntEmpiricalVariable_methods(root_module, root_module['ns3::IntEmpiricalVariable'])
    register_Ns3LogNormalVariable_methods(root_module, root_module['ns3::LogNormalVariable'])
    register_Ns3NormalVariable_methods(root_module, root_module['ns3::NormalVariable'])
    register_Ns3Object_methods(root_module, root_module['ns3::Object'])
    register_Ns3ObjectAggregateIterator_methods(root_module, root_module['ns3::Object::AggregateIterator'])
    register_Ns3ParetoVariable_methods(root_module, root_module['ns3::ParetoVariable'])
    register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    register_Ns3SimpleRefCount__Ns3HashImplementation_Ns3Empty_Ns3DefaultDeleter__lt__ns3HashImplementation__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >'])
    register_Ns3SimpleRefCount__Ns3L2Tracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3L2Tracer__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> >'])
    register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    register_Ns3SimpleRefCount__Ns3TopologyReader_Ns3Empty_Ns3DefaultDeleter__lt__ns3TopologyReader__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> >'])
    register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    register_Ns3SimpleRefCount__Ns3NdnAppDelayTracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnAppDelayTracer__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> >'])
    register_Ns3SimpleRefCount__Ns3NdnCsTracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnCsTracer__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> >'])
    register_Ns3SimpleRefCount__Ns3NdnData_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnData__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> >'])
    register_Ns3SimpleRefCount__Ns3NdnExclude_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnExclude__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> >'])
    register_Ns3SimpleRefCount__Ns3NdnFaceContainer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnFaceContainer__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> >'])
    register_Ns3SimpleRefCount__Ns3NdnInterest_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnInterest__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> >'])
    register_Ns3SimpleRefCount__Ns3NdnL3Tracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnL3Tracer__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> >'])
    register_Ns3SimpleRefCount__Ns3NdnName_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnName__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> >'])
    register_Ns3SimpleRefCount__Ns3NdnCsEntry_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnCsEntry__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> >'])
    register_Ns3SimpleRefCount__Ns3NdnPitEntry_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnPitEntry__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> >'])
    register_Ns3Time_methods(root_module, root_module['ns3::Time'])
    register_Ns3TopologyReader_methods(root_module, root_module['ns3::TopologyReader'])
    register_Ns3TopologyReaderLink_methods(root_module, root_module['ns3::TopologyReader::Link'])
    register_Ns3TraceSourceAccessor_methods(root_module, root_module['ns3::TraceSourceAccessor'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3AnnotatedTopologyReader_methods(root_module, root_module['ns3::AnnotatedTopologyReader'])
    register_Ns3Application_methods(root_module, root_module['ns3::Application'])
    register_Ns3AttributeAccessor_methods(root_module, root_module['ns3::AttributeAccessor'])
    register_Ns3AttributeChecker_methods(root_module, root_module['ns3::AttributeChecker'])
    register_Ns3AttributeValue_methods(root_module, root_module['ns3::AttributeValue'])
    register_Ns3BooleanChecker_methods(root_module, root_module['ns3::BooleanChecker'])
    register_Ns3BooleanValue_methods(root_module, root_module['ns3::BooleanValue'])
    register_Ns3CallbackBasedApp_methods(root_module, root_module['ns3::CallbackBasedApp'])
    register_Ns3CallbackChecker_methods(root_module, root_module['ns3::CallbackChecker'])
    register_Ns3CallbackImplBase_methods(root_module, root_module['ns3::CallbackImplBase'])
    register_Ns3CallbackValue_methods(root_module, root_module['ns3::CallbackValue'])
    register_Ns3DoubleValue_methods(root_module, root_module['ns3::DoubleValue'])
    register_Ns3EmptyAttributeValue_methods(root_module, root_module['ns3::EmptyAttributeValue'])
    register_Ns3EnumChecker_methods(root_module, root_module['ns3::EnumChecker'])
    register_Ns3EnumValue_methods(root_module, root_module['ns3::EnumValue'])
    register_Ns3EventImpl_methods(root_module, root_module['ns3::EventImpl'])
    register_Ns3IntegerValue_methods(root_module, root_module['ns3::IntegerValue'])
    register_Ns3Ipv4AddressChecker_methods(root_module, root_module['ns3::Ipv4AddressChecker'])
    register_Ns3Ipv4AddressValue_methods(root_module, root_module['ns3::Ipv4AddressValue'])
    register_Ns3Ipv4MaskChecker_methods(root_module, root_module['ns3::Ipv4MaskChecker'])
    register_Ns3Ipv4MaskValue_methods(root_module, root_module['ns3::Ipv4MaskValue'])
    register_Ns3Ipv6AddressChecker_methods(root_module, root_module['ns3::Ipv6AddressChecker'])
    register_Ns3Ipv6AddressValue_methods(root_module, root_module['ns3::Ipv6AddressValue'])
    register_Ns3Ipv6PrefixChecker_methods(root_module, root_module['ns3::Ipv6PrefixChecker'])
    register_Ns3Ipv6PrefixValue_methods(root_module, root_module['ns3::Ipv6PrefixValue'])
    register_Ns3L2Tracer_methods(root_module, root_module['ns3::L2Tracer'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3ObjectFactoryChecker_methods(root_module, root_module['ns3::ObjectFactoryChecker'])
    register_Ns3ObjectFactoryValue_methods(root_module, root_module['ns3::ObjectFactoryValue'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3RandomVariableChecker_methods(root_module, root_module['ns3::RandomVariableChecker'])
    register_Ns3RandomVariableValue_methods(root_module, root_module['ns3::RandomVariableValue'])
    register_Ns3RocketfuelWeightsReader_methods(root_module, root_module['ns3::RocketfuelWeightsReader'])
    register_Ns3TimeValue_methods(root_module, root_module['ns3::TimeValue'])
    register_Ns3TypeIdChecker_methods(root_module, root_module['ns3::TypeIdChecker'])
    register_Ns3TypeIdValue_methods(root_module, root_module['ns3::TypeIdValue'])
    register_Ns3UintegerValue_methods(root_module, root_module['ns3::UintegerValue'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3L2RateTracer_methods(root_module, root_module['ns3::L2RateTracer'])
    register_Ns3HashImplementation_methods(root_module, root_module['ns3::Hash::Implementation'])
    register_Ns3HashFunctionFnv1a_methods(root_module, root_module['ns3::Hash::Function::Fnv1a'])
    register_Ns3HashFunctionHash32_methods(root_module, root_module['ns3::Hash::Function::Hash32'])
    register_Ns3HashFunctionHash64_methods(root_module, root_module['ns3::Hash::Function::Hash64'])
    register_Ns3HashFunctionMurmur3_methods(root_module, root_module['ns3::Hash::Function::Murmur3'])
    register_Ns3NdnApp_methods(root_module, root_module['ns3::ndn::App'])
    register_Ns3NdnAppDelayTracer_methods(root_module, root_module['ns3::ndn::AppDelayTracer'])
    register_Ns3NdnAppHelper_methods(root_module, root_module['ns3::ndn::AppHelper'])
    register_Ns3NdnBlob_methods(root_module, root_module['ns3::ndn::Blob'])
    register_Ns3NdnContentStore_methods(root_module, root_module['ns3::ndn::ContentStore'])
    register_Ns3NdnCsTracer_methods(root_module, root_module['ns3::ndn::CsTracer'])
    register_Ns3NdnData_methods(root_module, root_module['ns3::ndn::Data'])
    register_Ns3NdnDataException_methods(root_module, root_module['ns3::ndn::DataException'])
    register_Ns3NdnExclude_methods(root_module, root_module['ns3::ndn::Exclude'])
    register_Ns3NdnFace_methods(root_module, root_module['ns3::ndn::Face'])
    register_Ns3NdnFaceContainer_methods(root_module, root_module['ns3::ndn::FaceContainer'])
    register_Ns3NdnFib_methods(root_module, root_module['ns3::ndn::Fib'])
    register_Ns3NdnForwardingStrategy_methods(root_module, root_module['ns3::ndn::ForwardingStrategy'])
    register_Ns3NdnGlobalRoutingHelper_methods(root_module, root_module['ns3::ndn::GlobalRoutingHelper'])
    register_Ns3NdnHeaderHelper_methods(root_module, root_module['ns3::ndn::HeaderHelper'])
    register_Ns3NdnInterest_methods(root_module, root_module['ns3::ndn::Interest'])
    register_Ns3NdnInterestException_methods(root_module, root_module['ns3::ndn::InterestException'])
    register_Ns3NdnIpFacesHelper_methods(root_module, root_module['ns3::ndn::IpFacesHelper'])
    register_Ns3NdnL3Protocol_methods(root_module, root_module['ns3::ndn::L3Protocol'])
    register_Ns3NdnL3Tracer_methods(root_module, root_module['ns3::ndn::L3Tracer'])
    register_Ns3NdnLimits_methods(root_module, root_module['ns3::ndn::Limits'])
    register_Ns3NdnLinkControlHelper_methods(root_module, root_module['ns3::ndn::LinkControlHelper'])
    register_Ns3NdnName_methods(root_module, root_module['ns3::ndn::Name'])
    register_Ns3NdnNameChecker_methods(root_module, root_module['ns3::ndn::NameChecker'])
    register_Ns3NdnNameValue_methods(root_module, root_module['ns3::ndn::NameValue'])
    register_Ns3NdnNetDeviceFace_methods(root_module, root_module['ns3::ndn::NetDeviceFace'])
    register_Ns3NdnPit_methods(root_module, root_module['ns3::ndn::Pit'])
    register_Ns3NdnRttEstimator_methods(root_module, root_module['ns3::ndn::RttEstimator'])
    register_Ns3NdnRttHistory_methods(root_module, root_module['ns3::ndn::RttHistory'])
    register_Ns3NdnStackHelper_methods(root_module, root_module['ns3::ndn::StackHelper'])
    register_Ns3NdnUnknownHeaderException_methods(root_module, root_module['ns3::ndn::UnknownHeaderException'])
    register_Ns3NdnWire_methods(root_module, root_module['ns3::ndn::Wire'])
    register_Ns3NdnApiFace_methods(root_module, root_module['ns3::ndn::ApiFace'])
    register_Ns3NdnAppFace_methods(root_module, root_module['ns3::ndn::AppFace'])
    register_Ns3NdnL3AggregateTracer_methods(root_module, root_module['ns3::ndn::L3AggregateTracer'])
    register_Ns3NdnL3RateTracer_methods(root_module, root_module['ns3::ndn::L3RateTracer'])
    register_Ns3NdnCsEntry_methods(root_module, root_module['ns3::ndn::cs::Entry'])
    register_Ns3NdnCsStats_methods(root_module, root_module['ns3::ndn::cs::Stats'])
    register_Ns3NdnFibEntry_methods(root_module, root_module['ns3::ndn::fib::Entry'])
    register_Ns3NdnFibEntryNoFaces_methods(root_module, root_module['ns3::ndn::fib::Entry::NoFaces'])
    register_Ns3NdnFibFaceMetric_methods(root_module, root_module['ns3::ndn::fib::FaceMetric'])
    register_Ns3NdnFibFaceMetricContainer_methods(root_module, root_module['ns3::ndn::fib::FaceMetricContainer'])
    register_Ns3NdnFibI_face_methods(root_module, root_module['ns3::ndn::fib::i_face'])
    register_Ns3NdnFibI_metric_methods(root_module, root_module['ns3::ndn::fib::i_metric'])
    register_Ns3NdnFibI_nth_methods(root_module, root_module['ns3::ndn::fib::i_nth'])
    register_Ns3NdnFwTag_methods(root_module, root_module['ns3::ndn::fw::Tag'])
    register_Ns3NdnNameComponent_methods(root_module, root_module['ns3::ndn::name::Component'])
    register_Ns3NdnPitEntry_methods(root_module, root_module['ns3::ndn::pit::Entry'])
    register_Ns3NdnPitEntryIsNotEmpty_methods(root_module, root_module['ns3::ndn::pit::EntryIsNotEmpty'])
    register_Ns3NdnPitIncomingFace_methods(root_module, root_module['ns3::ndn::pit::IncomingFace'])
    register_Ns3NdnPitOutgoingFace_methods(root_module, root_module['ns3::ndn::pit::OutgoingFace'])
    return

def register_Ns3Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## address.h (module 'network'): ns3::Address::Address() [constructor]
    cls.add_constructor([])
    ## address.h (module 'network'): ns3::Address::Address(uint8_t type, uint8_t const * buffer, uint8_t len) [constructor]
    cls.add_constructor([param('uint8_t', 'type'), param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h (module 'network'): ns3::Address::Address(ns3::Address const & address) [copy constructor]
    cls.add_constructor([param('ns3::Address const &', 'address')])
    ## address.h (module 'network'): bool ns3::Address::CheckCompatible(uint8_t type, uint8_t len) const [member function]
    cls.add_method('CheckCompatible', 
                   'bool', 
                   [param('uint8_t', 'type'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h (module 'network'): uint32_t ns3::Address::CopyAllFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyAllFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h (module 'network'): uint32_t ns3::Address::CopyAllTo(uint8_t * buffer, uint8_t len) const [member function]
    cls.add_method('CopyAllTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint8_t', 'len')], 
                   is_const=True)
    ## address.h (module 'network'): uint32_t ns3::Address::CopyFrom(uint8_t const * buffer, uint8_t len) [member function]
    cls.add_method('CopyFrom', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint8_t', 'len')])
    ## address.h (module 'network'): uint32_t ns3::Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## address.h (module 'network'): void ns3::Address::Deserialize(ns3::TagBuffer buffer) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')])
    ## address.h (module 'network'): uint8_t ns3::Address::GetLength() const [member function]
    cls.add_method('GetLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): uint32_t ns3::Address::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): bool ns3::Address::IsInvalid() const [member function]
    cls.add_method('IsInvalid', 
                   'bool', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): bool ns3::Address::IsMatchingType(uint8_t type) const [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('uint8_t', 'type')], 
                   is_const=True)
    ## address.h (module 'network'): static uint8_t ns3::Address::Register() [member function]
    cls.add_method('Register', 
                   'uint8_t', 
                   [], 
                   is_static=True)
    ## address.h (module 'network'): void ns3::Address::Serialize(ns3::TagBuffer buffer) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buffer')], 
                   is_const=True)
    return

def register_Ns3ApplicationContainer_methods(root_module, cls):
    ## application-container.h (module 'network'): ns3::ApplicationContainer::ApplicationContainer(ns3::ApplicationContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ApplicationContainer const &', 'arg0')])
    ## application-container.h (module 'network'): ns3::ApplicationContainer::ApplicationContainer() [constructor]
    cls.add_constructor([])
    ## application-container.h (module 'network'): ns3::ApplicationContainer::ApplicationContainer(ns3::Ptr<ns3::Application> application) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Application >', 'application')])
    ## application-container.h (module 'network'): ns3::ApplicationContainer::ApplicationContainer(std::string name) [constructor]
    cls.add_constructor([param('std::string', 'name')])
    ## application-container.h (module 'network'): void ns3::ApplicationContainer::Add(ns3::ApplicationContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::ApplicationContainer', 'other')])
    ## application-container.h (module 'network'): void ns3::ApplicationContainer::Add(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## application-container.h (module 'network'): void ns3::ApplicationContainer::Add(std::string name) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'name')])
    ## application-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Application>*,std::vector<ns3::Ptr<ns3::Application>, std::allocator<ns3::Ptr<ns3::Application> > > > ns3::ApplicationContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Application > const, std::vector< ns3::Ptr< ns3::Application > > >', 
                   [], 
                   is_const=True)
    ## application-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Application>*,std::vector<ns3::Ptr<ns3::Application>, std::allocator<ns3::Ptr<ns3::Application> > > > ns3::ApplicationContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Application > const, std::vector< ns3::Ptr< ns3::Application > > >', 
                   [], 
                   is_const=True)
    ## application-container.h (module 'network'): ns3::Ptr<ns3::Application> ns3::ApplicationContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## application-container.h (module 'network'): uint32_t ns3::ApplicationContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## application-container.h (module 'network'): void ns3::ApplicationContainer::Start(ns3::Time start) [member function]
    cls.add_method('Start', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application-container.h (module 'network'): void ns3::ApplicationContainer::Stop(ns3::Time stop) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    return

def register_Ns3AttributeConstructionList_methods(root_module, cls):
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::AttributeConstructionList(ns3::AttributeConstructionList const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeConstructionList const &', 'arg0')])
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::AttributeConstructionList() [constructor]
    cls.add_constructor([])
    ## attribute-construction-list.h (module 'core'): void ns3::AttributeConstructionList::Add(std::string name, ns3::Ptr<ns3::AttributeChecker const> checker, ns3::Ptr<ns3::AttributeValue> value) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker'), param('ns3::Ptr< ns3::AttributeValue >', 'value')])
    ## attribute-construction-list.h (module 'core'): std::_List_const_iterator<ns3::AttributeConstructionList::Item> ns3::AttributeConstructionList::Begin() const [member function]
    cls.add_method('Begin', 
                   'std::_List_const_iterator< ns3::AttributeConstructionList::Item >', 
                   [], 
                   is_const=True)
    ## attribute-construction-list.h (module 'core'): std::_List_const_iterator<ns3::AttributeConstructionList::Item> ns3::AttributeConstructionList::End() const [member function]
    cls.add_method('End', 
                   'std::_List_const_iterator< ns3::AttributeConstructionList::Item >', 
                   [], 
                   is_const=True)
    ## attribute-construction-list.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeConstructionList::Find(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('Find', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True)
    return

def register_Ns3AttributeConstructionListItem_methods(root_module, cls):
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::Item() [constructor]
    cls.add_constructor([])
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::Item(ns3::AttributeConstructionList::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeConstructionList::Item const &', 'arg0')])
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::checker [variable]
    cls.add_instance_attribute('checker', 'ns3::Ptr< ns3::AttributeChecker const >', is_const=False)
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    ## attribute-construction-list.h (module 'core'): ns3::AttributeConstructionList::Item::value [variable]
    cls.add_instance_attribute('value', 'ns3::Ptr< ns3::AttributeValue >', is_const=False)
    return

def register_Ns3Buffer_methods(root_module, cls):
    ## buffer.h (module 'network'): ns3::Buffer::Buffer() [constructor]
    cls.add_constructor([])
    ## buffer.h (module 'network'): ns3::Buffer::Buffer(uint32_t dataSize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize')])
    ## buffer.h (module 'network'): ns3::Buffer::Buffer(uint32_t dataSize, bool initialize) [constructor]
    cls.add_constructor([param('uint32_t', 'dataSize'), param('bool', 'initialize')])
    ## buffer.h (module 'network'): ns3::Buffer::Buffer(ns3::Buffer const & o) [copy constructor]
    cls.add_constructor([param('ns3::Buffer const &', 'o')])
    ## buffer.h (module 'network'): bool ns3::Buffer::AddAtEnd(uint32_t end) [member function]
    cls.add_method('AddAtEnd', 
                   'bool', 
                   [param('uint32_t', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::AddAtEnd(ns3::Buffer const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Buffer const &', 'o')])
    ## buffer.h (module 'network'): bool ns3::Buffer::AddAtStart(uint32_t start) [member function]
    cls.add_method('AddAtStart', 
                   'bool', 
                   [param('uint32_t', 'start')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator ns3::Buffer::Begin() const [member function]
    cls.add_method('Begin', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): void ns3::Buffer::CopyData(std::ostream * os, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'void', 
                   [param('std::ostream *', 'os'), param('uint32_t', 'size')], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::CopyData(uint8_t * buffer, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')], 
                   is_const=True)
    ## buffer.h (module 'network'): ns3::Buffer ns3::Buffer::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Buffer', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## buffer.h (module 'network'): ns3::Buffer ns3::Buffer::CreateFullCopy() const [member function]
    cls.add_method('CreateFullCopy', 
                   'ns3::Buffer', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator ns3::Buffer::End() const [member function]
    cls.add_method('End', 
                   'ns3::Buffer::Iterator', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): int32_t ns3::Buffer::GetCurrentEndOffset() const [member function]
    cls.add_method('GetCurrentEndOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): int32_t ns3::Buffer::GetCurrentStartOffset() const [member function]
    cls.add_method('GetCurrentStartOffset', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): uint8_t const * ns3::Buffer::PeekData() const [member function]
    cls.add_method('PeekData', 
                   'uint8_t const *', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): void ns3::Buffer::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3BufferIterator_methods(root_module, cls):
    ## buffer.h (module 'network'): ns3::Buffer::Iterator::Iterator(ns3::Buffer::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator const &', 'arg0')])
    ## buffer.h (module 'network'): ns3::Buffer::Iterator::Iterator() [constructor]
    cls.add_constructor([])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size')])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::CalculateIpChecksum(uint16_t size, uint32_t initialChecksum) [member function]
    cls.add_method('CalculateIpChecksum', 
                   'uint16_t', 
                   [param('uint16_t', 'size'), param('uint32_t', 'initialChecksum')])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::GetDistanceFrom(ns3::Buffer::Iterator const & o) const [member function]
    cls.add_method('GetDistanceFrom', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator const &', 'o')], 
                   is_const=True)
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): bool ns3::Buffer::Iterator::IsEnd() const [member function]
    cls.add_method('IsEnd', 
                   'bool', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): bool ns3::Buffer::Iterator::IsStart() const [member function]
    cls.add_method('IsStart', 
                   'bool', 
                   [], 
                   is_const=True)
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'void', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Next(uint32_t delta) [member function]
    cls.add_method('Next', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h (module 'network'): uint8_t ns3::Buffer::Iterator::PeekU8() [member function]
    cls.add_method('PeekU8', 
                   'uint8_t', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Prev() [member function]
    cls.add_method('Prev', 
                   'void', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Prev(uint32_t delta) [member function]
    cls.add_method('Prev', 
                   'void', 
                   [param('uint32_t', 'delta')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Read(ns3::Buffer::Iterator start, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::ReadLsbtohU16() [member function]
    cls.add_method('ReadLsbtohU16', 
                   'uint16_t', 
                   [])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::ReadLsbtohU32() [member function]
    cls.add_method('ReadLsbtohU32', 
                   'uint32_t', 
                   [])
    ## buffer.h (module 'network'): uint64_t ns3::Buffer::Iterator::ReadLsbtohU64() [member function]
    cls.add_method('ReadLsbtohU64', 
                   'uint64_t', 
                   [])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::ReadNtohU16() [member function]
    cls.add_method('ReadNtohU16', 
                   'uint16_t', 
                   [])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::ReadNtohU32() [member function]
    cls.add_method('ReadNtohU32', 
                   'uint32_t', 
                   [])
    ## buffer.h (module 'network'): uint64_t ns3::Buffer::Iterator::ReadNtohU64() [member function]
    cls.add_method('ReadNtohU64', 
                   'uint64_t', 
                   [])
    ## buffer.h (module 'network'): uint16_t ns3::Buffer::Iterator::ReadU16() [member function]
    cls.add_method('ReadU16', 
                   'uint16_t', 
                   [])
    ## buffer.h (module 'network'): uint32_t ns3::Buffer::Iterator::ReadU32() [member function]
    cls.add_method('ReadU32', 
                   'uint32_t', 
                   [])
    ## buffer.h (module 'network'): uint64_t ns3::Buffer::Iterator::ReadU64() [member function]
    cls.add_method('ReadU64', 
                   'uint64_t', 
                   [])
    ## buffer.h (module 'network'): uint8_t ns3::Buffer::Iterator::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
                   [])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::Write(ns3::Buffer::Iterator start, ns3::Buffer::Iterator end) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::Buffer::Iterator', 'end')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtolsbU16(uint16_t data) [member function]
    cls.add_method('WriteHtolsbU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtolsbU32(uint32_t data) [member function]
    cls.add_method('WriteHtolsbU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtolsbU64(uint64_t data) [member function]
    cls.add_method('WriteHtolsbU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtonU16(uint16_t data) [member function]
    cls.add_method('WriteHtonU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtonU32(uint32_t data) [member function]
    cls.add_method('WriteHtonU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteHtonU64(uint64_t data) [member function]
    cls.add_method('WriteHtonU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU16(uint16_t data) [member function]
    cls.add_method('WriteU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU32(uint32_t data) [member function]
    cls.add_method('WriteU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU64(uint64_t data) [member function]
    cls.add_method('WriteU64', 
                   'void', 
                   [param('uint64_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU8(uint8_t data) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data')])
    ## buffer.h (module 'network'): void ns3::Buffer::Iterator::WriteU8(uint8_t data, uint32_t len) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'data'), param('uint32_t', 'len')])
    return

def register_Ns3ByteTagIterator_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::ByteTagIterator::ByteTagIterator(ns3::ByteTagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagIterator const &', 'arg0')])
    ## packet.h (module 'network'): bool ns3::ByteTagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::ByteTagIterator::Item ns3::ByteTagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::ByteTagIterator::Item', 
                   [])
    return

def register_Ns3ByteTagIteratorItem_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::ByteTagIterator::Item::Item(ns3::ByteTagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagIterator::Item const &', 'arg0')])
    ## packet.h (module 'network'): uint32_t ns3::ByteTagIterator::Item::GetEnd() const [member function]
    cls.add_method('GetEnd', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::ByteTagIterator::Item::GetStart() const [member function]
    cls.add_method('GetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::ByteTagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::TypeId ns3::ByteTagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    return

def register_Ns3ByteTagList_methods(root_module, cls):
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::ByteTagList() [constructor]
    cls.add_constructor([])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::ByteTagList(ns3::ByteTagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList const &', 'o')])
    ## byte-tag-list.h (module 'network'): ns3::TagBuffer ns3::ByteTagList::Add(ns3::TypeId tid, uint32_t bufferSize, int32_t start, int32_t end) [member function]
    cls.add_method('Add', 
                   'ns3::TagBuffer', 
                   [param('ns3::TypeId', 'tid'), param('uint32_t', 'bufferSize'), param('int32_t', 'start'), param('int32_t', 'end')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::Add(ns3::ByteTagList const & o) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::ByteTagList const &', 'o')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::AddAtEnd(int32_t adjustment, int32_t appendOffset) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'appendOffset')])
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::AddAtStart(int32_t adjustment, int32_t prependOffset) [member function]
    cls.add_method('AddAtStart', 
                   'void', 
                   [param('int32_t', 'adjustment'), param('int32_t', 'prependOffset')])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator ns3::ByteTagList::Begin(int32_t offsetStart, int32_t offsetEnd) const [member function]
    cls.add_method('Begin', 
                   'ns3::ByteTagList::Iterator', 
                   [param('int32_t', 'offsetStart'), param('int32_t', 'offsetEnd')], 
                   is_const=True)
    ## byte-tag-list.h (module 'network'): void ns3::ByteTagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    return

def register_Ns3ByteTagListIterator_methods(root_module, cls):
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Iterator(ns3::ByteTagList::Iterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList::Iterator const &', 'arg0')])
    ## byte-tag-list.h (module 'network'): uint32_t ns3::ByteTagList::Iterator::GetOffsetStart() const [member function]
    cls.add_method('GetOffsetStart', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## byte-tag-list.h (module 'network'): bool ns3::ByteTagList::Iterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item ns3::ByteTagList::Iterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::ByteTagList::Iterator::Item', 
                   [])
    return

def register_Ns3ByteTagListIteratorItem_methods(root_module, cls):
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::Item(ns3::ByteTagList::Iterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ByteTagList::Iterator::Item const &', 'arg0')])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::Item(ns3::TagBuffer buf) [constructor]
    cls.add_constructor([param('ns3::TagBuffer', 'buf')])
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::buf [variable]
    cls.add_instance_attribute('buf', 'ns3::TagBuffer', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::end [variable]
    cls.add_instance_attribute('end', 'int32_t', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::size [variable]
    cls.add_instance_attribute('size', 'uint32_t', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::start [variable]
    cls.add_instance_attribute('start', 'int32_t', is_const=False)
    ## byte-tag-list.h (module 'network'): ns3::ByteTagList::Iterator::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3CallbackBase_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackBase::CallbackBase(ns3::CallbackBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackBase const &', 'arg0')])
    ## callback.h (module 'core'): ns3::CallbackBase::CallbackBase() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::Ptr<ns3::CallbackImplBase> ns3::CallbackBase::GetImpl() const [member function]
    cls.add_method('GetImpl', 
                   'ns3::Ptr< ns3::CallbackImplBase >', 
                   [], 
                   is_const=True)
    ## callback.h (module 'core'): ns3::CallbackBase::CallbackBase(ns3::Ptr<ns3::CallbackImplBase> impl) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CallbackImplBase >', 'impl')], 
                        visibility='protected')
    ## callback.h (module 'core'): static std::string ns3::CallbackBase::Demangle(std::string const & mangled) [member function]
    cls.add_method('Demangle', 
                   'std::string', 
                   [param('std::string const &', 'mangled')], 
                   is_static=True, visibility='protected')
    return

def register_Ns3EventId_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('==')
    ## event-id.h (module 'core'): ns3::EventId::EventId(ns3::EventId const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EventId const &', 'arg0')])
    ## event-id.h (module 'core'): ns3::EventId::EventId() [constructor]
    cls.add_constructor([])
    ## event-id.h (module 'core'): ns3::EventId::EventId(ns3::Ptr<ns3::EventImpl> const & impl, uint64_t ts, uint32_t context, uint32_t uid) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::EventImpl > const &', 'impl'), param('uint64_t', 'ts'), param('uint32_t', 'context'), param('uint32_t', 'uid')])
    ## event-id.h (module 'core'): void ns3::EventId::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## event-id.h (module 'core'): uint32_t ns3::EventId::GetContext() const [member function]
    cls.add_method('GetContext', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): uint64_t ns3::EventId::GetTs() const [member function]
    cls.add_method('GetTs', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): uint32_t ns3::EventId::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): bool ns3::EventId::IsExpired() const [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): bool ns3::EventId::IsRunning() const [member function]
    cls.add_method('IsRunning', 
                   'bool', 
                   [], 
                   is_const=True)
    ## event-id.h (module 'core'): ns3::EventImpl * ns3::EventId::PeekEventImpl() const [member function]
    cls.add_method('PeekEventImpl', 
                   'ns3::EventImpl *', 
                   [], 
                   is_const=True)
    return

def register_Ns3Hasher_methods(root_module, cls):
    ## hash.h (module 'core'): ns3::Hasher::Hasher(ns3::Hasher const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hasher const &', 'arg0')])
    ## hash.h (module 'core'): ns3::Hasher::Hasher() [constructor]
    cls.add_constructor([])
    ## hash.h (module 'core'): ns3::Hasher::Hasher(ns3::Ptr<ns3::Hash::Implementation> hp) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Hash::Implementation >', 'hp')])
    ## hash.h (module 'core'): uint32_t ns3::Hasher::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')])
    ## hash.h (module 'core'): uint32_t ns3::Hasher::GetHash32(std::string const s) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('std::string const', 's')])
    ## hash.h (module 'core'): uint64_t ns3::Hasher::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')])
    ## hash.h (module 'core'): uint64_t ns3::Hasher::GetHash64(std::string const s) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('std::string const', 's')])
    ## hash.h (module 'core'): ns3::Hasher & ns3::Hasher::clear() [member function]
    cls.add_method('clear', 
                   'ns3::Hasher &', 
                   [])
    return

def register_Ns3Ipv4Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address(ns3::Ipv4Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address(uint32_t address) [constructor]
    cls.add_constructor([param('uint32_t', 'address')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address::Ipv4Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv4Address::CombineMask(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('CombineMask', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv4Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): uint32_t ns3::Ipv4Address::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv4Address::GetSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('GetSubnetDirectedBroadcast', 
                   'ns3::Ipv4Address', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Address ns3::Ipv4Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsEqual(ns3::Ipv4Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Address const &', 'other')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsLocalMulticast() const [member function]
    cls.add_method('IsLocalMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static bool ns3::Ipv4Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Address::IsSubnetDirectedBroadcast(ns3::Ipv4Mask const & mask) const [member function]
    cls.add_method('IsSubnetDirectedBroadcast', 
                   'bool', 
                   [param('ns3::Ipv4Mask const &', 'mask')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Set(uint32_t address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'address')])
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    return

def register_Ns3Ipv4Mask_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask(ns3::Ipv4Mask const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask(uint32_t mask) [constructor]
    cls.add_constructor([param('uint32_t', 'mask')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask::Ipv4Mask(char const * mask) [constructor]
    cls.add_constructor([param('char const *', 'mask')])
    ## ipv4-address.h (module 'network'): uint32_t ns3::Ipv4Mask::Get() const [member function]
    cls.add_method('Get', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): uint32_t ns3::Ipv4Mask::GetInverse() const [member function]
    cls.add_method('GetInverse', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Mask ns3::Ipv4Mask::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Mask ns3::Ipv4Mask::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): uint16_t ns3::Ipv4Mask::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): static ns3::Ipv4Mask ns3::Ipv4Mask::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_static=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Mask::IsEqual(ns3::Ipv4Mask other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv4Mask', 'other')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4Mask::IsMatch(ns3::Ipv4Address a, ns3::Ipv4Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv4Address', 'a'), param('ns3::Ipv4Address', 'b')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Mask::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4Mask::Set(uint32_t mask) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint32_t', 'mask')])
    return

def register_Ns3Ipv6Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(char const * address) [constructor]
    cls.add_constructor([param('char const *', 'address')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(uint8_t * address) [constructor]
    cls.add_constructor([param('uint8_t *', 'address')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const & addr) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'addr')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address::Ipv6Address(ns3::Ipv6Address const * addr) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const *', 'addr')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address ns3::Ipv6Address::CombinePrefix(ns3::Ipv6Prefix const & prefix) [member function]
    cls.add_method('CombinePrefix', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::Deserialize(uint8_t const * buf) [member function]
    cls.add_method('Deserialize', 
                   'ns3::Ipv6Address', 
                   [param('uint8_t const *', 'buf')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAllHostsMulticast() [member function]
    cls.add_method('GetAllHostsMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAllNodesMulticast() [member function]
    cls.add_method('GetAllNodesMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAllRoutersMulticast() [member function]
    cls.add_method('GetAllRoutersMulticast', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetAny() [member function]
    cls.add_method('GetAny', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv6Address::GetIpv4MappedAddress() const [member function]
    cls.add_method('GetIpv4MappedAddress', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAllHostsMulticast() const [member function]
    cls.add_method('IsAllHostsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAllNodesMulticast() const [member function]
    cls.add_method('IsAllNodesMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAllRoutersMulticast() const [member function]
    cls.add_method('IsAllRoutersMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsAny() const [member function]
    cls.add_method('IsAny', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsDocumentation() const [member function]
    cls.add_method('IsDocumentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsEqual(ns3::Ipv6Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Address const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsIpv4MappedAddress() const [member function]
    cls.add_method('IsIpv4MappedAddress', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLinkLocal() const [member function]
    cls.add_method('IsLinkLocal', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLinkLocalMulticast() const [member function]
    cls.add_method('IsLinkLocalMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLocalhost() const [member function]
    cls.add_method('IsLocalhost', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static bool ns3::Ipv6Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsSolicitedMulticast() const [member function]
    cls.add_method('IsSolicitedMulticast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac16Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac16Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac48Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac64Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac64Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac16Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac16Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac48Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac64Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac64Address', 'mac')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeIpv4MappedAddress(ns3::Ipv4Address addr) [member function]
    cls.add_method('MakeIpv4MappedAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv4Address', 'addr')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeSolicitedAddress(ns3::Ipv6Address addr) [member function]
    cls.add_method('MakeSolicitedAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Serialize(uint8_t * buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Set(char const * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('char const *', 'address')])
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Address::Set(uint8_t * address) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint8_t *', 'address')])
    return

def register_Ns3Ipv6Prefix_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(uint8_t * prefix) [constructor]
    cls.add_constructor([param('uint8_t *', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(char const * prefix) [constructor]
    cls.add_constructor([param('char const *', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(uint8_t prefix) [constructor]
    cls.add_constructor([param('uint8_t', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const & prefix) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'prefix')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix::Ipv6Prefix(ns3::Ipv6Prefix const * prefix) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const *', 'prefix')])
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Prefix::GetBytes(uint8_t * buf) const [member function]
    cls.add_method('GetBytes', 
                   'void', 
                   [param('uint8_t *', 'buf')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetLoopback() [member function]
    cls.add_method('GetLoopback', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetOnes() [member function]
    cls.add_method('GetOnes', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): uint8_t ns3::Ipv6Prefix::GetPrefixLength() const [member function]
    cls.add_method('GetPrefixLength', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Prefix ns3::Ipv6Prefix::GetZero() [member function]
    cls.add_method('GetZero', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Prefix::IsEqual(ns3::Ipv6Prefix const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Prefix const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Prefix::IsMatch(ns3::Ipv6Address a, ns3::Ipv6Address b) const [member function]
    cls.add_method('IsMatch', 
                   'bool', 
                   [param('ns3::Ipv6Address', 'a'), param('ns3::Ipv6Address', 'b')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6Prefix::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3NetDeviceContainer_methods(root_module, cls):
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'arg0')])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer() [constructor]
    cls.add_constructor([])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(ns3::Ptr<ns3::NetDevice> dev) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice >', 'dev')])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(std::string devName) [constructor]
    cls.add_constructor([param('std::string', 'devName')])
    ## net-device-container.h (module 'network'): ns3::NetDeviceContainer::NetDeviceContainer(ns3::NetDeviceContainer const & a, ns3::NetDeviceContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NetDeviceContainer const &', 'a'), param('ns3::NetDeviceContainer const &', 'b')])
    ## net-device-container.h (module 'network'): void ns3::NetDeviceContainer::Add(ns3::NetDeviceContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NetDeviceContainer', 'other')])
    ## net-device-container.h (module 'network'): void ns3::NetDeviceContainer::Add(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## net-device-container.h (module 'network'): void ns3::NetDeviceContainer::Add(std::string deviceName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'deviceName')])
    ## net-device-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::NetDevice>*,std::vector<ns3::Ptr<ns3::NetDevice>, std::allocator<ns3::Ptr<ns3::NetDevice> > > > ns3::NetDeviceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::NetDevice > const, std::vector< ns3::Ptr< ns3::NetDevice > > >', 
                   [], 
                   is_const=True)
    ## net-device-container.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::NetDeviceContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## net-device-container.h (module 'network'): uint32_t ns3::NetDeviceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NodeContainer_methods(root_module, cls):
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'arg0')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer() [constructor]
    cls.add_constructor([])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(std::string nodeName) [constructor]
    cls.add_constructor([param('std::string', 'nodeName')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd')])
    ## node-container.h (module 'network'): ns3::NodeContainer::NodeContainer(ns3::NodeContainer const & a, ns3::NodeContainer const & b, ns3::NodeContainer const & c, ns3::NodeContainer const & d, ns3::NodeContainer const & e) [constructor]
    cls.add_constructor([param('ns3::NodeContainer const &', 'a'), param('ns3::NodeContainer const &', 'b'), param('ns3::NodeContainer const &', 'c'), param('ns3::NodeContainer const &', 'd'), param('ns3::NodeContainer const &', 'e')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Add(ns3::NodeContainer other) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::NodeContainer', 'other')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Add(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Add(std::string nodeName) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'nodeName')])
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): void ns3::NodeContainer::Create(uint32_t n) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## node-container.h (module 'network'): void ns3::NodeContainer::Create(uint32_t n, uint32_t systemId) [member function]
    cls.add_method('Create', 
                   'void', 
                   [param('uint32_t', 'n'), param('uint32_t', 'systemId')])
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): ns3::Ptr<ns3::Node> ns3::NodeContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## node-container.h (module 'network'): static ns3::NodeContainer ns3::NodeContainer::GetGlobal() [member function]
    cls.add_method('GetGlobal', 
                   'ns3::NodeContainer', 
                   [], 
                   is_static=True)
    ## node-container.h (module 'network'): uint32_t ns3::NodeContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::begin() const [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::begin() [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node >, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [])
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::end() const [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node > const, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [], 
                   is_const=True)
    ## node-container.h (module 'network'): __gnu_cxx::__normal_iterator<ns3::Ptr<ns3::Node>*,std::vector<ns3::Ptr<ns3::Node>, std::allocator<ns3::Ptr<ns3::Node> > > > ns3::NodeContainer::end() [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Node >, std::vector< ns3::Ptr< ns3::Node > > >', 
                   [])
    ## node-container.h (module 'network'): uint32_t ns3::NodeContainer::size() const [member function]
    cls.add_method('size', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3ObjectBase_methods(root_module, cls):
    ## object-base.h (module 'core'): ns3::ObjectBase::ObjectBase() [constructor]
    cls.add_constructor([])
    ## object-base.h (module 'core'): ns3::ObjectBase::ObjectBase(ns3::ObjectBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectBase const &', 'arg0')])
    ## object-base.h (module 'core'): void ns3::ObjectBase::GetAttribute(std::string name, ns3::AttributeValue & value) const [member function]
    cls.add_method('GetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'value')], 
                   is_const=True)
    ## object-base.h (module 'core'): bool ns3::ObjectBase::GetAttributeFailSafe(std::string name, ns3::AttributeValue & attribute) const [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'attribute')], 
                   is_const=True)
    ## object-base.h (module 'core'): ns3::TypeId ns3::ObjectBase::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## object-base.h (module 'core'): static ns3::TypeId ns3::ObjectBase::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object-base.h (module 'core'): void ns3::ObjectBase::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::SetAttributeFailSafe(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceConnect(std::string name, std::string context, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceConnect', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceConnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceConnectWithoutContext', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceDisconnect(std::string name, std::string context, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceDisconnect', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): bool ns3::ObjectBase::TraceDisconnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceDisconnectWithoutContext', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h (module 'core'): void ns3::ObjectBase::ConstructSelf(ns3::AttributeConstructionList const & attributes) [member function]
    cls.add_method('ConstructSelf', 
                   'void', 
                   [param('ns3::AttributeConstructionList const &', 'attributes')], 
                   visibility='protected')
    ## object-base.h (module 'core'): void ns3::ObjectBase::NotifyConstructionCompleted() [member function]
    cls.add_method('NotifyConstructionCompleted', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectDeleter_methods(root_module, cls):
    ## object.h (module 'core'): ns3::ObjectDeleter::ObjectDeleter() [constructor]
    cls.add_constructor([])
    ## object.h (module 'core'): ns3::ObjectDeleter::ObjectDeleter(ns3::ObjectDeleter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectDeleter const &', 'arg0')])
    ## object.h (module 'core'): static void ns3::ObjectDeleter::Delete(ns3::Object * object) [member function]
    cls.add_method('Delete', 
                   'void', 
                   [param('ns3::Object *', 'object')], 
                   is_static=True)
    return

def register_Ns3ObjectFactory_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## object-factory.h (module 'core'): ns3::ObjectFactory::ObjectFactory(ns3::ObjectFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactory const &', 'arg0')])
    ## object-factory.h (module 'core'): ns3::ObjectFactory::ObjectFactory() [constructor]
    cls.add_constructor([])
    ## object-factory.h (module 'core'): ns3::ObjectFactory::ObjectFactory(std::string typeId) [constructor]
    cls.add_constructor([param('std::string', 'typeId')])
    ## object-factory.h (module 'core'): ns3::Ptr<ns3::Object> ns3::ObjectFactory::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## object-factory.h (module 'core'): ns3::TypeId ns3::ObjectFactory::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::SetTypeId(ns3::TypeId tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('ns3::TypeId', 'tid')])
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::SetTypeId(char const * tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('char const *', 'tid')])
    ## object-factory.h (module 'core'): void ns3::ObjectFactory::SetTypeId(std::string tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('std::string', 'tid')])
    return

def register_Ns3PacketMetadata_methods(root_module, cls):
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::PacketMetadata(uint64_t uid, uint32_t size) [constructor]
    cls.add_constructor([param('uint64_t', 'uid'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::PacketMetadata(ns3::PacketMetadata const & o) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata const &', 'o')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddAtEnd(ns3::PacketMetadata const & o) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::PacketMetadata const &', 'o')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddHeader(ns3::Header const & header, uint32_t size) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddPaddingAtEnd(uint32_t end) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::AddTrailer(ns3::Trailer const & trailer, uint32_t size) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator ns3::PacketMetadata::BeginItem(ns3::Buffer buffer) const [member function]
    cls.add_method('BeginItem', 
                   'ns3::PacketMetadata::ItemIterator', 
                   [param('ns3::Buffer', 'buffer')], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata ns3::PacketMetadata::CreateFragment(uint32_t start, uint32_t end) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::PacketMetadata', 
                   [param('uint32_t', 'start'), param('uint32_t', 'end')], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): uint32_t ns3::PacketMetadata::Deserialize(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): static void ns3::PacketMetadata::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet-metadata.h (module 'network'): static void ns3::PacketMetadata::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet-metadata.h (module 'network'): uint32_t ns3::PacketMetadata::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): uint64_t ns3::PacketMetadata::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveAtEnd(uint32_t end) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'end')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveAtStart(uint32_t start) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'start')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveHeader(ns3::Header const & header, uint32_t size) [member function]
    cls.add_method('RemoveHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): void ns3::PacketMetadata::RemoveTrailer(ns3::Trailer const & trailer, uint32_t size) [member function]
    cls.add_method('RemoveTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer'), param('uint32_t', 'size')])
    ## packet-metadata.h (module 'network'): uint32_t ns3::PacketMetadata::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3PacketMetadataItem_methods(root_module, cls):
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::Item() [constructor]
    cls.add_constructor([])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::Item(ns3::PacketMetadata::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata::Item const &', 'arg0')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::current [variable]
    cls.add_instance_attribute('current', 'ns3::Buffer::Iterator', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::currentSize [variable]
    cls.add_instance_attribute('currentSize', 'uint32_t', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::currentTrimedFromEnd [variable]
    cls.add_instance_attribute('currentTrimedFromEnd', 'uint32_t', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::currentTrimedFromStart [variable]
    cls.add_instance_attribute('currentTrimedFromStart', 'uint32_t', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::isFragment [variable]
    cls.add_instance_attribute('isFragment', 'bool', is_const=False)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3PacketMetadataItemIterator_methods(root_module, cls):
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator::ItemIterator(ns3::PacketMetadata::ItemIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketMetadata::ItemIterator const &', 'arg0')])
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::ItemIterator::ItemIterator(ns3::PacketMetadata const * metadata, ns3::Buffer buffer) [constructor]
    cls.add_constructor([param('ns3::PacketMetadata const *', 'metadata'), param('ns3::Buffer', 'buffer')])
    ## packet-metadata.h (module 'network'): bool ns3::PacketMetadata::ItemIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet-metadata.h (module 'network'): ns3::PacketMetadata::Item ns3::PacketMetadata::ItemIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::PacketMetadata::Item', 
                   [])
    return

def register_Ns3PacketTagIterator_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::PacketTagIterator::PacketTagIterator(ns3::PacketTagIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagIterator const &', 'arg0')])
    ## packet.h (module 'network'): bool ns3::PacketTagIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::PacketTagIterator::Item ns3::PacketTagIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::PacketTagIterator::Item', 
                   [])
    return

def register_Ns3PacketTagIteratorItem_methods(root_module, cls):
    ## packet.h (module 'network'): ns3::PacketTagIterator::Item::Item(ns3::PacketTagIterator::Item const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagIterator::Item const &', 'arg0')])
    ## packet.h (module 'network'): void ns3::PacketTagIterator::Item::GetTag(ns3::Tag & tag) const [member function]
    cls.add_method('GetTag', 
                   'void', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::TypeId ns3::PacketTagIterator::Item::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    return

def register_Ns3PacketTagList_methods(root_module, cls):
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::PacketTagList() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::PacketTagList(ns3::PacketTagList const & o) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList const &', 'o')])
    ## packet-tag-list.h (module 'network'): void ns3::PacketTagList::Add(ns3::Tag const & tag) const [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData const * ns3::PacketTagList::Head() const [member function]
    cls.add_method('Head', 
                   'ns3::PacketTagList::TagData const *', 
                   [], 
                   is_const=True)
    ## packet-tag-list.h (module 'network'): bool ns3::PacketTagList::Peek(ns3::Tag & tag) const [member function]
    cls.add_method('Peek', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet-tag-list.h (module 'network'): bool ns3::PacketTagList::Remove(ns3::Tag & tag) [member function]
    cls.add_method('Remove', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet-tag-list.h (module 'network'): void ns3::PacketTagList::RemoveAll() [member function]
    cls.add_method('RemoveAll', 
                   'void', 
                   [])
    ## packet-tag-list.h (module 'network'): bool ns3::PacketTagList::Replace(ns3::Tag & tag) [member function]
    cls.add_method('Replace', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    return

def register_Ns3PacketTagListTagData_methods(root_module, cls):
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData() [constructor]
    cls.add_constructor([])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::TagData(ns3::PacketTagList::TagData const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PacketTagList::TagData const &', 'arg0')])
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::count [variable]
    cls.add_instance_attribute('count', 'uint32_t', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::data [variable]
    cls.add_instance_attribute('data', 'uint8_t [ 20 ]', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::next [variable]
    cls.add_instance_attribute('next', 'ns3::PacketTagList::TagData *', is_const=False)
    ## packet-tag-list.h (module 'network'): ns3::PacketTagList::TagData::tid [variable]
    cls.add_instance_attribute('tid', 'ns3::TypeId', is_const=False)
    return

def register_Ns3RandomVariable_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## random-variable.h (module 'core'): ns3::RandomVariable::RandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::RandomVariable::RandomVariable(ns3::RandomVariable const & o) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariable const &', 'o')])
    ## random-variable.h (module 'core'): uint32_t ns3::RandomVariable::GetInteger() const [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::RandomVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    return

def register_Ns3RngSeedManager_methods(root_module, cls):
    ## rng-seed-manager.h (module 'core'): ns3::RngSeedManager::RngSeedManager() [constructor]
    cls.add_constructor([])
    ## rng-seed-manager.h (module 'core'): ns3::RngSeedManager::RngSeedManager(ns3::RngSeedManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RngSeedManager const &', 'arg0')])
    ## rng-seed-manager.h (module 'core'): static uint64_t ns3::RngSeedManager::GetNextStreamIndex() [member function]
    cls.add_method('GetNextStreamIndex', 
                   'uint64_t', 
                   [], 
                   is_static=True)
    ## rng-seed-manager.h (module 'core'): static uint64_t ns3::RngSeedManager::GetRun() [member function]
    cls.add_method('GetRun', 
                   'uint64_t', 
                   [], 
                   is_static=True)
    ## rng-seed-manager.h (module 'core'): static uint32_t ns3::RngSeedManager::GetSeed() [member function]
    cls.add_method('GetSeed', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## rng-seed-manager.h (module 'core'): static void ns3::RngSeedManager::SetRun(uint64_t run) [member function]
    cls.add_method('SetRun', 
                   'void', 
                   [param('uint64_t', 'run')], 
                   is_static=True)
    ## rng-seed-manager.h (module 'core'): static void ns3::RngSeedManager::SetSeed(uint32_t seed) [member function]
    cls.add_method('SetSeed', 
                   'void', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    return

def register_Ns3SequenceNumber32_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('+', root_module['ns3::SequenceNumber32'], root_module['ns3::SequenceNumber32'], param('ns3::SequenceNumber< unsigned int, int > const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::SequenceNumber32'], root_module['ns3::SequenceNumber32'], param('int', 'right'))
    cls.add_inplace_numeric_operator('+=', param('int', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::SequenceNumber32'], root_module['ns3::SequenceNumber32'], param('int', 'right'))
    cls.add_inplace_numeric_operator('-=', param('int', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## sequence-number.h (module 'network'): ns3::SequenceNumber<unsigned int, int>::SequenceNumber() [constructor]
    cls.add_constructor([])
    ## sequence-number.h (module 'network'): ns3::SequenceNumber<unsigned int, int>::SequenceNumber(unsigned int value) [constructor]
    cls.add_constructor([param('unsigned int', 'value')])
    ## sequence-number.h (module 'network'): ns3::SequenceNumber<unsigned int, int>::SequenceNumber(ns3::SequenceNumber<unsigned int, int> const & value) [copy constructor]
    cls.add_constructor([param('ns3::SequenceNumber< unsigned int, int > const &', 'value')])
    ## sequence-number.h (module 'network'): unsigned int ns3::SequenceNumber<unsigned int, int>::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'unsigned int', 
                   [], 
                   is_const=True)
    return

def register_Ns3SequentialVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(ns3::SequentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SequentialVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(double f, double l, double i=1, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('double', 'i', default_value='1'), param('uint32_t', 'c', default_value='1')])
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(double f, double l, ns3::RandomVariable const & i, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('ns3::RandomVariable const &', 'i'), param('uint32_t', 'c', default_value='1')])
    return

def register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::SimpleRefCount(ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Simulator_methods(root_module, cls):
    ## simulator.h (module 'core'): ns3::Simulator::Simulator(ns3::Simulator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Simulator const &', 'arg0')])
    ## simulator.h (module 'core'): static void ns3::Simulator::Cancel(ns3::EventId const & id) [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static uint32_t ns3::Simulator::GetContext() [member function]
    cls.add_method('GetContext', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::GetDelayLeft(ns3::EventId const & id) [member function]
    cls.add_method('GetDelayLeft', 
                   'ns3::Time', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Ptr<ns3::SimulatorImpl> ns3::Simulator::GetImplementation() [member function]
    cls.add_method('GetImplementation', 
                   'ns3::Ptr< ns3::SimulatorImpl >', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::GetMaximumSimulationTime() [member function]
    cls.add_method('GetMaximumSimulationTime', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static uint32_t ns3::Simulator::GetSystemId() [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static bool ns3::Simulator::IsExpired(ns3::EventId const & id) [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static bool ns3::Simulator::IsFinished() [member function]
    cls.add_method('IsFinished', 
                   'bool', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::Now() [member function]
    cls.add_method('Now', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Remove(ns3::EventId const & id) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::EventId const &', 'id')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::SetImplementation(ns3::Ptr<ns3::SimulatorImpl> impl) [member function]
    cls.add_method('SetImplementation', 
                   'void', 
                   [param('ns3::Ptr< ns3::SimulatorImpl >', 'impl')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::SetScheduler(ns3::ObjectFactory schedulerFactory) [member function]
    cls.add_method('SetScheduler', 
                   'void', 
                   [param('ns3::ObjectFactory', 'schedulerFactory')], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_static=True)
    ## simulator.h (module 'core'): static void ns3::Simulator::Stop(ns3::Time const & time) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'time')], 
                   is_static=True)
    return

def register_Ns3Tag_methods(root_module, cls):
    ## tag.h (module 'network'): ns3::Tag::Tag() [constructor]
    cls.add_constructor([])
    ## tag.h (module 'network'): ns3::Tag::Tag(ns3::Tag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Tag const &', 'arg0')])
    ## tag.h (module 'network'): void ns3::Tag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_virtual=True)
    ## tag.h (module 'network'): uint32_t ns3::Tag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h (module 'network'): static ns3::TypeId ns3::Tag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## tag.h (module 'network'): void ns3::Tag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## tag.h (module 'network'): void ns3::Tag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3TagBuffer_methods(root_module, cls):
    ## tag-buffer.h (module 'network'): ns3::TagBuffer::TagBuffer(ns3::TagBuffer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TagBuffer const &', 'arg0')])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer::TagBuffer(uint8_t * start, uint8_t * end) [constructor]
    cls.add_constructor([param('uint8_t *', 'start'), param('uint8_t *', 'end')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::CopyFrom(ns3::TagBuffer o) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('ns3::TagBuffer', 'o')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::Read(uint8_t * buffer, uint32_t size) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')])
    ## tag-buffer.h (module 'network'): double ns3::TagBuffer::ReadDouble() [member function]
    cls.add_method('ReadDouble', 
                   'double', 
                   [])
    ## tag-buffer.h (module 'network'): uint16_t ns3::TagBuffer::ReadU16() [member function]
    cls.add_method('ReadU16', 
                   'uint16_t', 
                   [])
    ## tag-buffer.h (module 'network'): uint32_t ns3::TagBuffer::ReadU32() [member function]
    cls.add_method('ReadU32', 
                   'uint32_t', 
                   [])
    ## tag-buffer.h (module 'network'): uint64_t ns3::TagBuffer::ReadU64() [member function]
    cls.add_method('ReadU64', 
                   'uint64_t', 
                   [])
    ## tag-buffer.h (module 'network'): uint8_t ns3::TagBuffer::ReadU8() [member function]
    cls.add_method('ReadU8', 
                   'uint8_t', 
                   [])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::TrimAtEnd(uint32_t trim) [member function]
    cls.add_method('TrimAtEnd', 
                   'void', 
                   [param('uint32_t', 'trim')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::Write(uint8_t const * buffer, uint32_t size) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteDouble(double v) [member function]
    cls.add_method('WriteDouble', 
                   'void', 
                   [param('double', 'v')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU16(uint16_t data) [member function]
    cls.add_method('WriteU16', 
                   'void', 
                   [param('uint16_t', 'data')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU32(uint32_t data) [member function]
    cls.add_method('WriteU32', 
                   'void', 
                   [param('uint32_t', 'data')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU64(uint64_t v) [member function]
    cls.add_method('WriteU64', 
                   'void', 
                   [param('uint64_t', 'v')])
    ## tag-buffer.h (module 'network'): void ns3::TagBuffer::WriteU8(uint8_t v) [member function]
    cls.add_method('WriteU8', 
                   'void', 
                   [param('uint8_t', 'v')])
    return

def register_Ns3TimeWithUnit_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## nstime.h (module 'core'): ns3::TimeWithUnit::TimeWithUnit(ns3::TimeWithUnit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeWithUnit const &', 'arg0')])
    ## nstime.h (module 'core'): ns3::TimeWithUnit::TimeWithUnit(ns3::Time const time, ns3::Time::Unit const unit) [constructor]
    cls.add_constructor([param('ns3::Time const', 'time'), param('ns3::Time::Unit const', 'unit')])
    return

def register_Ns3TracedValue__Ns3NdnFibFaceMetricStatus_methods(root_module, cls):
    ## traced-value.h (module 'core'): ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::TracedValue() [constructor]
    cls.add_constructor([])
    ## traced-value.h (module 'core'): ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::TracedValue(ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status> const & o) [copy constructor]
    cls.add_constructor([param('ns3::TracedValue< ns3::ndn::fib::FaceMetric::Status > const &', 'o')])
    ## traced-value.h (module 'core'): ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::TracedValue(ns3::ndn::fib::FaceMetric::Status const & v) [constructor]
    cls.add_constructor([param('ns3::ndn::fib::FaceMetric::Status const &', 'v')])
    ## traced-value.h (module 'core'): void ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::Connect(ns3::CallbackBase const & cb, std::basic_string<char,std::char_traits<char>,std::allocator<char> > path) [member function]
    cls.add_method('Connect', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb'), param('std::string', 'path')])
    ## traced-value.h (module 'core'): void ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::ConnectWithoutContext(ns3::CallbackBase const & cb) [member function]
    cls.add_method('ConnectWithoutContext', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb')])
    ## traced-value.h (module 'core'): void ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::Disconnect(ns3::CallbackBase const & cb, std::basic_string<char,std::char_traits<char>,std::allocator<char> > path) [member function]
    cls.add_method('Disconnect', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb'), param('std::string', 'path')])
    ## traced-value.h (module 'core'): void ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::DisconnectWithoutContext(ns3::CallbackBase const & cb) [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb')])
    ## traced-value.h (module 'core'): ns3::ndn::fib::FaceMetric::Status ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::ndn::fib::FaceMetric::Status', 
                   [], 
                   is_const=True)
    ## traced-value.h (module 'core'): void ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status>::Set(ns3::ndn::fib::FaceMetric::Status const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::ndn::fib::FaceMetric::Status const &', 'v')])
    return

def register_Ns3TriangularVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::TriangularVariable::TriangularVariable(ns3::TriangularVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TriangularVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::TriangularVariable::TriangularVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::TriangularVariable::TriangularVariable(double s, double l, double mean) [constructor]
    cls.add_constructor([param('double', 's'), param('double', 'l'), param('double', 'mean')])
    return

def register_Ns3TypeId_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## type-id.h (module 'core'): ns3::TypeId::TypeId(char const * name) [constructor]
    cls.add_constructor([param('char const *', 'name')])
    ## type-id.h (module 'core'): ns3::TypeId::TypeId() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::TypeId(ns3::TypeId const & o) [copy constructor]
    cls.add_constructor([param('ns3::TypeId const &', 'o')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddAttribute(std::string name, std::string help, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeAccessor const> accessor, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('AddAttribute', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeAccessor const >', 'accessor'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddAttribute(std::string name, std::string help, uint32_t flags, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeAccessor const> accessor, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('AddAttribute', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('uint32_t', 'flags'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeAccessor const >', 'accessor'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::AddTraceSource(std::string name, std::string help, ns3::Ptr<ns3::TraceSourceAccessor const> accessor) [member function]
    cls.add_method('AddTraceSource', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::Ptr< ns3::TraceSourceAccessor const >', 'accessor')])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation ns3::TypeId::GetAttribute(uint32_t i) const [member function]
    cls.add_method('GetAttribute', 
                   'ns3::TypeId::AttributeInformation', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetAttributeFullName(uint32_t i) const [member function]
    cls.add_method('GetAttributeFullName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetAttributeN() const [member function]
    cls.add_method('GetAttributeN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::Callback<ns3::ObjectBase*,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ns3::TypeId::GetConstructor() const [member function]
    cls.add_method('GetConstructor', 
                   'ns3::Callback< ns3::ObjectBase *, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetGroupName() const [member function]
    cls.add_method('GetGroupName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetHash() const [member function]
    cls.add_method('GetHash', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeId::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::GetRegistered(uint32_t i) [member function]
    cls.add_method('GetRegistered', 
                   'ns3::TypeId', 
                   [param('uint32_t', 'i')], 
                   is_static=True)
    ## type-id.h (module 'core'): static uint32_t ns3::TypeId::GetRegisteredN() [member function]
    cls.add_method('GetRegisteredN', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation ns3::TypeId::GetTraceSource(uint32_t i) const [member function]
    cls.add_method('GetTraceSource', 
                   'ns3::TypeId::TraceSourceInformation', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h (module 'core'): uint32_t ns3::TypeId::GetTraceSourceN() const [member function]
    cls.add_method('GetTraceSourceN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): uint16_t ns3::TypeId::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::HasConstructor() const [member function]
    cls.add_method('HasConstructor', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::HasParent() const [member function]
    cls.add_method('HasParent', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::HideFromDocumentation() [member function]
    cls.add_method('HideFromDocumentation', 
                   'ns3::TypeId', 
                   [])
    ## type-id.h (module 'core'): bool ns3::TypeId::IsChildOf(ns3::TypeId other) const [member function]
    cls.add_method('IsChildOf', 
                   'bool', 
                   [param('ns3::TypeId', 'other')], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::LookupAttributeByName(std::string name, ns3::TypeId::AttributeInformation * info) const [member function]
    cls.add_method('LookupAttributeByName', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::TypeId::AttributeInformation *', 'info', transfer_ownership=False)], 
                   is_const=True)
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::LookupByHash(uint32_t hash) [member function]
    cls.add_method('LookupByHash', 
                   'ns3::TypeId', 
                   [param('uint32_t', 'hash')], 
                   is_static=True)
    ## type-id.h (module 'core'): static bool ns3::TypeId::LookupByHashFailSafe(uint32_t hash, ns3::TypeId * tid) [member function]
    cls.add_method('LookupByHashFailSafe', 
                   'bool', 
                   [param('uint32_t', 'hash'), param('ns3::TypeId *', 'tid')], 
                   is_static=True)
    ## type-id.h (module 'core'): static ns3::TypeId ns3::TypeId::LookupByName(std::string name) [member function]
    cls.add_method('LookupByName', 
                   'ns3::TypeId', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## type-id.h (module 'core'): ns3::Ptr<ns3::TraceSourceAccessor const> ns3::TypeId::LookupTraceSourceByName(std::string name) const [member function]
    cls.add_method('LookupTraceSourceByName', 
                   'ns3::Ptr< ns3::TraceSourceAccessor const >', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::MustHideFromDocumentation() const [member function]
    cls.add_method('MustHideFromDocumentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): bool ns3::TypeId::SetAttributeInitialValue(uint32_t i, ns3::Ptr<ns3::AttributeValue const> initialValue) [member function]
    cls.add_method('SetAttributeInitialValue', 
                   'bool', 
                   [param('uint32_t', 'i'), param('ns3::Ptr< ns3::AttributeValue const >', 'initialValue')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::SetGroupName(std::string groupName) [member function]
    cls.add_method('SetGroupName', 
                   'ns3::TypeId', 
                   [param('std::string', 'groupName')])
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeId::SetParent(ns3::TypeId tid) [member function]
    cls.add_method('SetParent', 
                   'ns3::TypeId', 
                   [param('ns3::TypeId', 'tid')])
    ## type-id.h (module 'core'): void ns3::TypeId::SetUid(uint16_t tid) [member function]
    cls.add_method('SetUid', 
                   'void', 
                   [param('uint16_t', 'tid')])
    return

def register_Ns3TypeIdAttributeInformation_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::AttributeInformation() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::AttributeInformation(ns3::TypeId::AttributeInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeId::AttributeInformation const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::accessor [variable]
    cls.add_instance_attribute('accessor', 'ns3::Ptr< ns3::AttributeAccessor const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::checker [variable]
    cls.add_instance_attribute('checker', 'ns3::Ptr< ns3::AttributeChecker const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::flags [variable]
    cls.add_instance_attribute('flags', 'uint32_t', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::help [variable]
    cls.add_instance_attribute('help', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::initialValue [variable]
    cls.add_instance_attribute('initialValue', 'ns3::Ptr< ns3::AttributeValue const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::AttributeInformation::originalInitialValue [variable]
    cls.add_instance_attribute('originalInitialValue', 'ns3::Ptr< ns3::AttributeValue const >', is_const=False)
    return

def register_Ns3TypeIdTraceSourceInformation_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::TraceSourceInformation() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::TraceSourceInformation(ns3::TypeId::TraceSourceInformation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeId::TraceSourceInformation const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::accessor [variable]
    cls.add_instance_attribute('accessor', 'ns3::Ptr< ns3::TraceSourceAccessor const >', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::help [variable]
    cls.add_instance_attribute('help', 'std::string', is_const=False)
    ## type-id.h (module 'core'): ns3::TypeId::TraceSourceInformation::name [variable]
    cls.add_instance_attribute('name', 'std::string', is_const=False)
    return

def register_Ns3UniformVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::UniformVariable::UniformVariable(ns3::UniformVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UniformVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::UniformVariable::UniformVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::UniformVariable::UniformVariable(double s, double l) [constructor]
    cls.add_constructor([param('double', 's'), param('double', 'l')])
    ## random-variable.h (module 'core'): uint32_t ns3::UniformVariable::GetInteger(uint32_t s, uint32_t l) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 's'), param('uint32_t', 'l')])
    ## random-variable.h (module 'core'): double ns3::UniformVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::UniformVariable::GetValue(double s, double l) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 's'), param('double', 'l')])
    return

def register_Ns3WeibullVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(ns3::WeibullVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WeibullVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(double m, double s) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's')])
    ## random-variable.h (module 'core'): ns3::WeibullVariable::WeibullVariable(double m, double s, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's'), param('double', 'b')])
    return

def register_Ns3ZetaVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ZetaVariable::ZetaVariable(ns3::ZetaVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ZetaVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ZetaVariable::ZetaVariable(double alpha) [constructor]
    cls.add_constructor([param('double', 'alpha')])
    ## random-variable.h (module 'core'): ns3::ZetaVariable::ZetaVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3ZipfVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ZipfVariable::ZipfVariable(ns3::ZipfVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ZipfVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ZipfVariable::ZipfVariable(long int N, double alpha) [constructor]
    cls.add_constructor([param('long int', 'N'), param('double', 'alpha')])
    ## random-variable.h (module 'core'): ns3::ZipfVariable::ZipfVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3Empty_methods(root_module, cls):
    ## empty.h (module 'core'): ns3::empty::empty() [constructor]
    cls.add_constructor([])
    ## empty.h (module 'core'): ns3::empty::empty(ns3::empty const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::empty const &', 'arg0')])
    return

def register_Ns3Int64x64_t_methods(root_module, cls):
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('*=', param('ns3::int64x64_t const &', 'right'))
    cls.add_inplace_numeric_operator('+=', param('ns3::int64x64_t const &', 'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::int64x64_t const &', 'right'))
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    cls.add_inplace_numeric_operator('/=', param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_unary_numeric_operator('-')
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t() [constructor]
    cls.add_constructor([])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(double v) [constructor]
    cls.add_constructor([param('double', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long double v) [constructor]
    cls.add_constructor([param('long double', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long int v) [constructor]
    cls.add_constructor([param('long int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long long int v) [constructor]
    cls.add_constructor([param('long long int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(unsigned int v) [constructor]
    cls.add_constructor([param('unsigned int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long unsigned int v) [constructor]
    cls.add_constructor([param('long unsigned int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(long long unsigned int v) [constructor]
    cls.add_constructor([param('long long unsigned int', 'v')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(int64_t hi, uint64_t lo) [constructor]
    cls.add_constructor([param('int64_t', 'hi'), param('uint64_t', 'lo')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(ns3::int64x64_t const & o) [copy constructor]
    cls.add_constructor([param('ns3::int64x64_t const &', 'o')])
    ## int64x64-double.h (module 'core'): double ns3::int64x64_t::GetDouble() const [member function]
    cls.add_method('GetDouble', 
                   'double', 
                   [], 
                   is_const=True)
    ## int64x64-double.h (module 'core'): int64_t ns3::int64x64_t::GetHigh() const [member function]
    cls.add_method('GetHigh', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## int64x64-double.h (module 'core'): uint64_t ns3::int64x64_t::GetLow() const [member function]
    cls.add_method('GetLow', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## int64x64-double.h (module 'core'): static ns3::int64x64_t ns3::int64x64_t::Invert(uint64_t v) [member function]
    cls.add_method('Invert', 
                   'ns3::int64x64_t', 
                   [param('uint64_t', 'v')], 
                   is_static=True)
    ## int64x64-double.h (module 'core'): void ns3::int64x64_t::MulByInvert(ns3::int64x64_t const & o) [member function]
    cls.add_method('MulByInvert', 
                   'void', 
                   [param('ns3::int64x64_t const &', 'o')])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::implementation [variable]
    cls.add_static_attribute('implementation', 'ns3::int64x64_t::impl_type const', is_const=True)
    return

def register_Ns3Chunk_methods(root_module, cls):
    ## chunk.h (module 'network'): ns3::Chunk::Chunk() [constructor]
    cls.add_constructor([])
    ## chunk.h (module 'network'): ns3::Chunk::Chunk(ns3::Chunk const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Chunk const &', 'arg0')])
    ## chunk.h (module 'network'): uint32_t ns3::Chunk::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## chunk.h (module 'network'): static ns3::TypeId ns3::Chunk::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## chunk.h (module 'network'): void ns3::Chunk::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3ConstantVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ConstantVariable::ConstantVariable(ns3::ConstantVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ConstantVariable::ConstantVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ConstantVariable::ConstantVariable(double c) [constructor]
    cls.add_constructor([param('double', 'c')])
    ## random-variable.h (module 'core'): void ns3::ConstantVariable::SetConstant(double c) [member function]
    cls.add_method('SetConstant', 
                   'void', 
                   [param('double', 'c')])
    return

def register_Ns3DeterministicVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::DeterministicVariable::DeterministicVariable(ns3::DeterministicVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeterministicVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::DeterministicVariable::DeterministicVariable(double * d, uint32_t c) [constructor]
    cls.add_constructor([param('double *', 'd'), param('uint32_t', 'c')])
    return

def register_Ns3EmpiricalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::EmpiricalVariable::EmpiricalVariable(ns3::EmpiricalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmpiricalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::EmpiricalVariable::EmpiricalVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): void ns3::EmpiricalVariable::CDF(double v, double c) [member function]
    cls.add_method('CDF', 
                   'void', 
                   [param('double', 'v'), param('double', 'c')])
    return

def register_Ns3ErlangVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ErlangVariable::ErlangVariable(ns3::ErlangVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErlangVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ErlangVariable::ErlangVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ErlangVariable::ErlangVariable(unsigned int k, double lambda) [constructor]
    cls.add_constructor([param('unsigned int', 'k'), param('double', 'lambda')])
    ## random-variable.h (module 'core'): double ns3::ErlangVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::ErlangVariable::GetValue(unsigned int k, double lambda) const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('unsigned int', 'k'), param('double', 'lambda')], 
                   is_const=True)
    return

def register_Ns3ExponentialVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable(ns3::ExponentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ExponentialVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h (module 'core'): ns3::ExponentialVariable::ExponentialVariable(double m, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'b')])
    return

def register_Ns3GammaVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::GammaVariable::GammaVariable(ns3::GammaVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GammaVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::GammaVariable::GammaVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::GammaVariable::GammaVariable(double alpha, double beta) [constructor]
    cls.add_constructor([param('double', 'alpha'), param('double', 'beta')])
    ## random-variable.h (module 'core'): double ns3::GammaVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): double ns3::GammaVariable::GetValue(double alpha, double beta) const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'alpha'), param('double', 'beta')], 
                   is_const=True)
    return

def register_Ns3Header_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## header.h (module 'network'): ns3::Header::Header() [constructor]
    cls.add_constructor([])
    ## header.h (module 'network'): ns3::Header::Header(ns3::Header const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Header const &', 'arg0')])
    ## header.h (module 'network'): uint32_t ns3::Header::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_virtual=True)
    ## header.h (module 'network'): uint32_t ns3::Header::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h (module 'network'): static ns3::TypeId ns3::Header::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## header.h (module 'network'): void ns3::Header::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## header.h (module 'network'): void ns3::Header::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3IntEmpiricalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::IntEmpiricalVariable::IntEmpiricalVariable(ns3::IntEmpiricalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntEmpiricalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::IntEmpiricalVariable::IntEmpiricalVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3LogNormalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::LogNormalVariable::LogNormalVariable(ns3::LogNormalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LogNormalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::LogNormalVariable::LogNormalVariable(double mu, double sigma) [constructor]
    cls.add_constructor([param('double', 'mu'), param('double', 'sigma')])
    return

def register_Ns3NormalVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable(ns3::NormalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NormalVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable(double m, double v) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'v')])
    ## random-variable.h (module 'core'): ns3::NormalVariable::NormalVariable(double m, double v, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'v'), param('double', 'b')])
    return

def register_Ns3Object_methods(root_module, cls):
    ## object.h (module 'core'): ns3::Object::Object() [constructor]
    cls.add_constructor([])
    ## object.h (module 'core'): void ns3::Object::AggregateObject(ns3::Ptr<ns3::Object> other) [member function]
    cls.add_method('AggregateObject', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'other')])
    ## object.h (module 'core'): void ns3::Object::Dispose() [member function]
    cls.add_method('Dispose', 
                   'void', 
                   [])
    ## object.h (module 'core'): ns3::Object::AggregateIterator ns3::Object::GetAggregateIterator() const [member function]
    cls.add_method('GetAggregateIterator', 
                   'ns3::Object::AggregateIterator', 
                   [], 
                   is_const=True)
    ## object.h (module 'core'): ns3::TypeId ns3::Object::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object.h (module 'core'): ns3::Ptr<ns3::ndn::ContentStore> ns3::Object::GetObject() const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::ndn::ContentStore >', 
                   [], 
                   is_const=True, template_parameters=['ns3::ndn::ContentStore'])
    ## object.h (module 'core'): ns3::Ptr<ns3::ndn::Fib> ns3::Object::GetObject() const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::ndn::Fib >', 
                   [], 
                   is_const=True, template_parameters=['ns3::ndn::Fib'])
    ## object.h (module 'core'): ns3::Ptr<ns3::ndn::Pit> ns3::Object::GetObject() const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::ndn::Pit >', 
                   [], 
                   is_const=True, template_parameters=['ns3::ndn::Pit'])
    ## object.h (module 'core'): static ns3::TypeId ns3::Object::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object.h (module 'core'): void ns3::Object::Initialize() [member function]
    cls.add_method('Initialize', 
                   'void', 
                   [])
    ## object.h (module 'core'): ns3::Object::Object(ns3::Object const & o) [copy constructor]
    cls.add_constructor([param('ns3::Object const &', 'o')], 
                        visibility='protected')
    ## object.h (module 'core'): void ns3::Object::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## object.h (module 'core'): void ns3::Object::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## object.h (module 'core'): void ns3::Object::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectAggregateIterator_methods(root_module, cls):
    ## object.h (module 'core'): ns3::Object::AggregateIterator::AggregateIterator(ns3::Object::AggregateIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Object::AggregateIterator const &', 'arg0')])
    ## object.h (module 'core'): ns3::Object::AggregateIterator::AggregateIterator() [constructor]
    cls.add_constructor([])
    ## object.h (module 'core'): bool ns3::Object::AggregateIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## object.h (module 'core'): ns3::Ptr<ns3::Object const> ns3::Object::AggregateIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::Object const >', 
                   [])
    return

def register_Ns3ParetoVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(ns3::ParetoVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ParetoVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(double m, double s) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(double m, double s, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's'), param('double', 'b')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(std::pair<double,double> params) [constructor]
    cls.add_constructor([param('std::pair< double, double >', 'params')])
    ## random-variable.h (module 'core'): ns3::ParetoVariable::ParetoVariable(std::pair<double,double> params, double b) [constructor]
    cls.add_constructor([param('std::pair< double, double >', 'params'), param('double', 'b')])
    return

def register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter< ns3::AttributeAccessor > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter< ns3::AttributeChecker > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter< ns3::AttributeValue > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter< ns3::CallbackImplBase > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::SimpleRefCount(ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter< ns3::EventImpl > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3HashImplementation_Ns3Empty_Ns3DefaultDeleter__lt__ns3HashImplementation__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter< ns3::Hash::Implementation > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Hash::Implementation, ns3::empty, ns3::DefaultDeleter<ns3::Hash::Implementation> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3L2Tracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3L2Tracer__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> >::SimpleRefCount(ns3::SimpleRefCount<ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter< ns3::L2Tracer > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::L2Tracer, ns3::empty, ns3::DefaultDeleter<ns3::L2Tracer> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::SimpleRefCount(ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter< ns3::NixVector > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter< ns3::Packet > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3TopologyReader_Ns3Empty_Ns3DefaultDeleter__lt__ns3TopologyReader__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> >::SimpleRefCount(ns3::SimpleRefCount<ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter< ns3::TopologyReader > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::TopologyReader, ns3::empty, ns3::DefaultDeleter<ns3::TopologyReader> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter< ns3::TraceSourceAccessor > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnAppDelayTracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnAppDelayTracer__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter< ns3::ndn::AppDelayTracer > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::AppDelayTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::AppDelayTracer> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnCsTracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnCsTracer__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter< ns3::ndn::CsTracer > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::CsTracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::CsTracer> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnData_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnData__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter< ns3::ndn::Data > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::Data, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Data> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnExclude_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnExclude__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter< ns3::ndn::Exclude > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::Exclude, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Exclude> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnFaceContainer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnFaceContainer__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter< ns3::ndn::FaceContainer > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::FaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::FaceContainer> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnInterest_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnInterest__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter< ns3::ndn::Interest > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::Interest, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Interest> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnL3Tracer_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnL3Tracer__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter< ns3::ndn::L3Tracer > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::L3Tracer, ns3::empty, ns3::DefaultDeleter<ns3::ndn::L3Tracer> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnName_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnName__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter< ns3::ndn::Name > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::Name, ns3::empty, ns3::DefaultDeleter<ns3::ndn::Name> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnCsEntry_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnCsEntry__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter< ns3::ndn::cs::Entry > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::cs::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::cs::Entry> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NdnPitEntry_Ns3Empty_Ns3DefaultDeleter__lt__ns3NdnPitEntry__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> >::SimpleRefCount(ns3::SimpleRefCount<ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter< ns3::ndn::pit::Entry > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::ndn::pit::Entry, ns3::empty, ns3::DefaultDeleter<ns3::ndn::pit::Entry> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3Time_methods(root_module, cls):
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('+=', param('ns3::Time const &', 'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::Time const &', 'right'))
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    cls.add_binary_numeric_operator('*', root_module['ns3::Time'], root_module['ns3::Time'], param('int64_t const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::Time'], root_module['ns3::Time'], param('int64_t const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    ## nstime.h (module 'core'): ns3::Time::Time() [constructor]
    cls.add_constructor([])
    ## nstime.h (module 'core'): ns3::Time::Time(ns3::Time const & o) [copy constructor]
    cls.add_constructor([param('ns3::Time const &', 'o')])
    ## nstime.h (module 'core'): ns3::Time::Time(double v) [constructor]
    cls.add_constructor([param('double', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long int v) [constructor]
    cls.add_constructor([param('long int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long long int v) [constructor]
    cls.add_constructor([param('long long int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(unsigned int v) [constructor]
    cls.add_constructor([param('unsigned int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long unsigned int v) [constructor]
    cls.add_constructor([param('long unsigned int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(long long unsigned int v) [constructor]
    cls.add_constructor([param('long long unsigned int', 'v')])
    ## nstime.h (module 'core'): ns3::Time::Time(std::string const & s) [constructor]
    cls.add_constructor([param('std::string const &', 's')])
    ## nstime.h (module 'core'): ns3::Time::Time(ns3::int64x64_t const & value) [constructor]
    cls.add_constructor([param('ns3::int64x64_t const &', 'value')])
    ## nstime.h (module 'core'): ns3::TimeWithUnit ns3::Time::As(ns3::Time::Unit const unit) const [member function]
    cls.add_method('As', 
                   'ns3::TimeWithUnit', 
                   [param('ns3::Time::Unit const', 'unit')], 
                   is_const=True)
    ## nstime.h (module 'core'): int ns3::Time::Compare(ns3::Time const & o) const [member function]
    cls.add_method('Compare', 
                   'int', 
                   [param('ns3::Time const &', 'o')], 
                   is_const=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::From(ns3::int64x64_t const & from, ns3::Time::Unit timeUnit) [member function]
    cls.add_method('From', 
                   'ns3::Time', 
                   [param('ns3::int64x64_t const &', 'from'), param('ns3::Time::Unit', 'timeUnit')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::From(ns3::int64x64_t const & value) [member function]
    cls.add_method('From', 
                   'ns3::Time', 
                   [param('ns3::int64x64_t const &', 'value')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::FromDouble(double value, ns3::Time::Unit timeUnit) [member function]
    cls.add_method('FromDouble', 
                   'ns3::Time', 
                   [param('double', 'value'), param('ns3::Time::Unit', 'timeUnit')], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::FromInteger(uint64_t value, ns3::Time::Unit timeUnit) [member function]
    cls.add_method('FromInteger', 
                   'ns3::Time', 
                   [param('uint64_t', 'value'), param('ns3::Time::Unit', 'timeUnit')], 
                   is_static=True)
    ## nstime.h (module 'core'): double ns3::Time::GetDays() const [member function]
    cls.add_method('GetDays', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::GetDouble() const [member function]
    cls.add_method('GetDouble', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetFemtoSeconds() const [member function]
    cls.add_method('GetFemtoSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::GetHours() const [member function]
    cls.add_method('GetHours', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetInteger() const [member function]
    cls.add_method('GetInteger', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetMicroSeconds() const [member function]
    cls.add_method('GetMicroSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetMilliSeconds() const [member function]
    cls.add_method('GetMilliSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::GetMinutes() const [member function]
    cls.add_method('GetMinutes', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetNanoSeconds() const [member function]
    cls.add_method('GetNanoSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetPicoSeconds() const [member function]
    cls.add_method('GetPicoSeconds', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): static ns3::Time::Unit ns3::Time::GetResolution() [member function]
    cls.add_method('GetResolution', 
                   'ns3::Time::Unit', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): double ns3::Time::GetSeconds() const [member function]
    cls.add_method('GetSeconds', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::GetTimeStep() const [member function]
    cls.add_method('GetTimeStep', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::GetYears() const [member function]
    cls.add_method('GetYears', 
                   'double', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsNegative() const [member function]
    cls.add_method('IsNegative', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsPositive() const [member function]
    cls.add_method('IsPositive', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsStrictlyNegative() const [member function]
    cls.add_method('IsStrictlyNegative', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsStrictlyPositive() const [member function]
    cls.add_method('IsStrictlyPositive', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): bool ns3::Time::IsZero() const [member function]
    cls.add_method('IsZero', 
                   'bool', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::Max() [member function]
    cls.add_method('Max', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): static ns3::Time ns3::Time::Min() [member function]
    cls.add_method('Min', 
                   'ns3::Time', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): static void ns3::Time::SetResolution(ns3::Time::Unit resolution) [member function]
    cls.add_method('SetResolution', 
                   'void', 
                   [param('ns3::Time::Unit', 'resolution')], 
                   is_static=True)
    ## nstime.h (module 'core'): static bool ns3::Time::StaticInit() [member function]
    cls.add_method('StaticInit', 
                   'bool', 
                   [], 
                   is_static=True)
    ## nstime.h (module 'core'): ns3::int64x64_t ns3::Time::To(ns3::Time::Unit timeUnit) const [member function]
    cls.add_method('To', 
                   'ns3::int64x64_t', 
                   [param('ns3::Time::Unit', 'timeUnit')], 
                   is_const=True)
    ## nstime.h (module 'core'): double ns3::Time::ToDouble(ns3::Time::Unit timeUnit) const [member function]
    cls.add_method('ToDouble', 
                   'double', 
                   [param('ns3::Time::Unit', 'timeUnit')], 
                   is_const=True)
    ## nstime.h (module 'core'): int64_t ns3::Time::ToInteger(ns3::Time::Unit timeUnit) const [member function]
    cls.add_method('ToInteger', 
                   'int64_t', 
                   [param('ns3::Time::Unit', 'timeUnit')], 
                   is_const=True)
    return

def register_Ns3TopologyReader_methods(root_module, cls):
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::TopologyReader() [constructor]
    cls.add_constructor([])
    ## topology-reader.h (module 'topology-read'): void ns3::TopologyReader::AddLink(ns3::TopologyReader::Link link) [member function]
    cls.add_method('AddLink', 
                   'void', 
                   [param('ns3::TopologyReader::Link', 'link')])
    ## topology-reader.h (module 'topology-read'): std::string ns3::TopologyReader::GetFileName() const [member function]
    cls.add_method('GetFileName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): std::_List_const_iterator<ns3::TopologyReader::Link> ns3::TopologyReader::LinksBegin() const [member function]
    cls.add_method('LinksBegin', 
                   'std::_List_const_iterator< ns3::TopologyReader::Link >', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): bool ns3::TopologyReader::LinksEmpty() const [member function]
    cls.add_method('LinksEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): std::_List_const_iterator<ns3::TopologyReader::Link> ns3::TopologyReader::LinksEnd() const [member function]
    cls.add_method('LinksEnd', 
                   'std::_List_const_iterator< ns3::TopologyReader::Link >', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): int ns3::TopologyReader::LinksSize() const [member function]
    cls.add_method('LinksSize', 
                   'int', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): ns3::NodeContainer ns3::TopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## topology-reader.h (module 'topology-read'): void ns3::TopologyReader::SetFileName(std::string const & fileName) [member function]
    cls.add_method('SetFileName', 
                   'void', 
                   [param('std::string const &', 'fileName')])
    return

def register_Ns3TopologyReaderLink_methods(root_module, cls):
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link::Link(ns3::TopologyReader::Link const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TopologyReader::Link const &', 'arg0')])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link::Link() [constructor]
    cls.add_constructor([])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link::Link(ns3::Ptr<ns3::Node> fromPtr, std::string const & fromName, ns3::Ptr<ns3::Node> toPtr, std::string const & toName) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'fromPtr'), param('std::string const &', 'fromName'), param('ns3::Ptr< ns3::Node >', 'toPtr'), param('std::string const &', 'toName')])
    ## topology-reader.h (module 'topology-read'): std::_Rb_tree_const_iterator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::TopologyReader::Link::AttributesBegin() [member function]
    cls.add_method('AttributesBegin', 
                   'std::_Rb_tree_const_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## topology-reader.h (module 'topology-read'): std::_Rb_tree_const_iterator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::TopologyReader::Link::AttributesEnd() [member function]
    cls.add_method('AttributesEnd', 
                   'std::_Rb_tree_const_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## topology-reader.h (module 'topology-read'): std::string ns3::TopologyReader::Link::GetAttribute(std::string const & name) const [member function]
    cls.add_method('GetAttribute', 
                   'std::string', 
                   [param('std::string const &', 'name')], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): bool ns3::TopologyReader::Link::GetAttributeFailSafe(std::string const & name, std::string & value) const [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string const &', 'name'), param('std::string &', 'value')], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): ns3::Ptr<ns3::NetDevice> ns3::TopologyReader::Link::GetFromNetDevice() const [member function]
    cls.add_method('GetFromNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): ns3::Ptr<ns3::Node> ns3::TopologyReader::Link::GetFromNode() const [member function]
    cls.add_method('GetFromNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): std::string ns3::TopologyReader::Link::GetFromNodeName() const [member function]
    cls.add_method('GetFromNodeName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): ns3::Ptr<ns3::NetDevice> ns3::TopologyReader::Link::GetToNetDevice() const [member function]
    cls.add_method('GetToNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): ns3::Ptr<ns3::Node> ns3::TopologyReader::Link::GetToNode() const [member function]
    cls.add_method('GetToNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): std::string ns3::TopologyReader::Link::GetToNodeName() const [member function]
    cls.add_method('GetToNodeName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): void ns3::TopologyReader::Link::SetAttribute(std::string const & name, std::string const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string const &', 'name'), param('std::string const &', 'value')])
    ## topology-reader.h (module 'topology-read'): void ns3::TopologyReader::Link::SetNetDevices(ns3::Ptr<ns3::NetDevice> from, ns3::Ptr<ns3::NetDevice> to) [member function]
    cls.add_method('SetNetDevices', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'from'), param('ns3::Ptr< ns3::NetDevice >', 'to')])
    return

def register_Ns3TraceSourceAccessor_methods(root_module, cls):
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor::TraceSourceAccessor(ns3::TraceSourceAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TraceSourceAccessor const &', 'arg0')])
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor::TraceSourceAccessor() [constructor]
    cls.add_constructor([])
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::Connect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('Connect', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::ConnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('ConnectWithoutContext', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::Disconnect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('Disconnect', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h (module 'core'): bool ns3::TraceSourceAccessor::DisconnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3Trailer_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## trailer.h (module 'network'): ns3::Trailer::Trailer() [constructor]
    cls.add_constructor([])
    ## trailer.h (module 'network'): ns3::Trailer::Trailer(ns3::Trailer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Trailer const &', 'arg0')])
    ## trailer.h (module 'network'): uint32_t ns3::Trailer::Deserialize(ns3::Buffer::Iterator end) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'end')], 
                   is_pure_virtual=True, is_virtual=True)
    ## trailer.h (module 'network'): uint32_t ns3::Trailer::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h (module 'network'): static ns3::TypeId ns3::Trailer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## trailer.h (module 'network'): void ns3::Trailer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trailer.h (module 'network'): void ns3::Trailer::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AnnotatedTopologyReader_methods(root_module, cls):
    ## annotated-topology-reader.h (module 'ndnSIM'): ns3::AnnotatedTopologyReader::AnnotatedTopologyReader(std::string const & path="", double scale=1.0e+0) [constructor]
    cls.add_constructor([param('std::string const &', 'path', default_value='""'), param('double', 'scale', default_value='1.0e+0')])
    ## annotated-topology-reader.h (module 'ndnSIM'): ns3::NodeContainer ns3::AnnotatedTopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): ns3::NodeContainer ns3::AnnotatedTopologyReader::GetNodes() const [member function]
    cls.add_method('GetNodes', 
                   'ns3::NodeContainer', 
                   [], 
                   is_const=True, is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): std::list<ns3::TopologyReader::Link, std::allocator<ns3::TopologyReader::Link> > const & ns3::AnnotatedTopologyReader::GetLinks() const [member function]
    cls.add_method('GetLinks', 
                   'std::list< ns3::TopologyReader::Link > const &', 
                   [], 
                   is_const=True, is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::AssignIpv4Addresses(ns3::Ipv4Address base) [member function]
    cls.add_method('AssignIpv4Addresses', 
                   'void', 
                   [param('ns3::Ipv4Address', 'base')], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::SetBoundingBox(double ulx, double uly, double lrx, double lry) [member function]
    cls.add_method('SetBoundingBox', 
                   'void', 
                   [param('double', 'ulx'), param('double', 'uly'), param('double', 'lrx'), param('double', 'lry')], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::SetMobilityModel(std::string const & model) [member function]
    cls.add_method('SetMobilityModel', 
                   'void', 
                   [param('std::string const &', 'model')], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::ApplyOspfMetric() [member function]
    cls.add_method('ApplyOspfMetric', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::SaveTopology(std::string const & file) [member function]
    cls.add_method('SaveTopology', 
                   'void', 
                   [param('std::string const &', 'file')], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::SaveGraphviz(std::string const & file) [member function]
    cls.add_method('SaveGraphviz', 
                   'void', 
                   [param('std::string const &', 'file')], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'ndnSIM'): ns3::Ptr<ns3::Node> ns3::AnnotatedTopologyReader::CreateNode(std::string const name, uint32_t systemId) [member function]
    cls.add_method('CreateNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('std::string const', 'name'), param('uint32_t', 'systemId')], 
                   visibility='protected')
    ## annotated-topology-reader.h (module 'ndnSIM'): ns3::Ptr<ns3::Node> ns3::AnnotatedTopologyReader::CreateNode(std::string const name, double posX, double posY, uint32_t systemId) [member function]
    cls.add_method('CreateNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [param('std::string const', 'name'), param('double', 'posX'), param('double', 'posY'), param('uint32_t', 'systemId')], 
                   visibility='protected')
    ## annotated-topology-reader.h (module 'ndnSIM'): void ns3::AnnotatedTopologyReader::ApplySettings() [member function]
    cls.add_method('ApplySettings', 
                   'void', 
                   [], 
                   visibility='protected')
    return

def register_Ns3Application_methods(root_module, cls):
    ## application.h (module 'network'): ns3::Application::Application(ns3::Application const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Application const &', 'arg0')])
    ## application.h (module 'network'): ns3::Application::Application() [constructor]
    cls.add_constructor([])
    ## application.h (module 'network'): ns3::Ptr<ns3::Node> ns3::Application::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## application.h (module 'network'): static ns3::TypeId ns3::Application::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## application.h (module 'network'): void ns3::Application::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## application.h (module 'network'): void ns3::Application::SetStartTime(ns3::Time start) [member function]
    cls.add_method('SetStartTime', 
                   'void', 
                   [param('ns3::Time', 'start')])
    ## application.h (module 'network'): void ns3::Application::SetStopTime(ns3::Time stop) [member function]
    cls.add_method('SetStopTime', 
                   'void', 
                   [param('ns3::Time', 'stop')])
    ## application.h (module 'network'): void ns3::Application::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h (module 'network'): void ns3::Application::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## application.h (module 'network'): void ns3::Application::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## application.h (module 'network'): void ns3::Application::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3AttributeAccessor_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::AttributeAccessor::AttributeAccessor(ns3::AttributeAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeAccessor const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::AttributeAccessor::AttributeAccessor() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::Get(ns3::ObjectBase const * object, ns3::AttributeValue & attribute) const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [param('ns3::ObjectBase const *', 'object'), param('ns3::AttributeValue &', 'attribute')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::HasGetter() const [member function]
    cls.add_method('HasGetter', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::HasSetter() const [member function]
    cls.add_method('HasSetter', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeAccessor::Set(ns3::ObjectBase * object, ns3::AttributeValue const & value) const [member function]
    cls.add_method('Set', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'object', transfer_ownership=False), param('ns3::AttributeValue const &', 'value')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeChecker_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::AttributeChecker::AttributeChecker(ns3::AttributeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeChecker const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::AttributeChecker::AttributeChecker() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): bool ns3::AttributeChecker::Check(ns3::AttributeValue const & value) const [member function]
    cls.add_method('Check', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeChecker::Copy(ns3::AttributeValue const & source, ns3::AttributeValue & destination) const [member function]
    cls.add_method('Copy', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'source'), param('ns3::AttributeValue &', 'destination')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeChecker::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeChecker::CreateValidValue(ns3::AttributeValue const & value) const [member function]
    cls.add_method('CreateValidValue', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_const=True)
    ## attribute.h (module 'core'): std::string ns3::AttributeChecker::GetUnderlyingTypeInformation() const [member function]
    cls.add_method('GetUnderlyingTypeInformation', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): std::string ns3::AttributeChecker::GetValueTypeName() const [member function]
    cls.add_method('GetValueTypeName', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeChecker::HasUnderlyingTypeInformation() const [member function]
    cls.add_method('HasUnderlyingTypeInformation', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeValue_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::AttributeValue::AttributeValue(ns3::AttributeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeValue const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::AttributeValue::AttributeValue() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::AttributeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::AttributeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_pure_virtual=True, is_virtual=True)
    ## attribute.h (module 'core'): std::string ns3::AttributeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3BooleanChecker_methods(root_module, cls):
    ## boolean.h (module 'core'): ns3::BooleanChecker::BooleanChecker() [constructor]
    cls.add_constructor([])
    ## boolean.h (module 'core'): ns3::BooleanChecker::BooleanChecker(ns3::BooleanChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BooleanChecker const &', 'arg0')])
    return

def register_Ns3BooleanValue_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## boolean.h (module 'core'): ns3::BooleanValue::BooleanValue(ns3::BooleanValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BooleanValue const &', 'arg0')])
    ## boolean.h (module 'core'): ns3::BooleanValue::BooleanValue() [constructor]
    cls.add_constructor([])
    ## boolean.h (module 'core'): ns3::BooleanValue::BooleanValue(bool value) [constructor]
    cls.add_constructor([param('bool', 'value')])
    ## boolean.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::BooleanValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## boolean.h (module 'core'): bool ns3::BooleanValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## boolean.h (module 'core'): bool ns3::BooleanValue::Get() const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [], 
                   is_const=True)
    ## boolean.h (module 'core'): std::string ns3::BooleanValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## boolean.h (module 'core'): void ns3::BooleanValue::Set(bool value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('bool', 'value')])
    return

def register_Ns3CallbackBasedApp_methods(root_module, cls):
    ## callback-based-app.h (module 'ndnSIM'): ns3::CallbackBasedApp::CallbackBasedApp(ns3::CallbackBasedApp const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackBasedApp const &', 'arg0')])
    ## callback-based-app.h (module 'ndnSIM'): ns3::CallbackBasedApp::CallbackBasedApp() [constructor]
    cls.add_constructor([])
    ## callback-based-app.h (module 'ndnSIM'): static ns3::TypeId ns3::CallbackBasedApp::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## callback-based-app.h (module 'ndnSIM'): void ns3::CallbackBasedApp::SetOnStartCallback(ns3::Callback<void, ns3::Ptr<ns3::Application>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> onStart) [member function]
    cls.add_method('SetOnStartCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Application >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'onStart')])
    ## callback-based-app.h (module 'ndnSIM'): void ns3::CallbackBasedApp::SetOnStopCallback(ns3::Callback<void, ns3::Ptr<ns3::Application>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> onStart) [member function]
    cls.add_method('SetOnStopCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Application >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'onStart')])
    ## callback-based-app.h (module 'ndnSIM'): void ns3::CallbackBasedApp::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## callback-based-app.h (module 'ndnSIM'): void ns3::CallbackBasedApp::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3CallbackChecker_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackChecker::CallbackChecker() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::CallbackChecker::CallbackChecker(ns3::CallbackChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackChecker const &', 'arg0')])
    return

def register_Ns3CallbackImplBase_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackImplBase::CallbackImplBase() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::CallbackImplBase::CallbackImplBase(ns3::CallbackImplBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackImplBase const &', 'arg0')])
    ## callback.h (module 'core'): bool ns3::CallbackImplBase::IsEqual(ns3::Ptr<ns3::CallbackImplBase const> other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CallbackImplBase const >', 'other')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3CallbackValue_methods(root_module, cls):
    ## callback.h (module 'core'): ns3::CallbackValue::CallbackValue(ns3::CallbackValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackValue const &', 'arg0')])
    ## callback.h (module 'core'): ns3::CallbackValue::CallbackValue() [constructor]
    cls.add_constructor([])
    ## callback.h (module 'core'): ns3::CallbackValue::CallbackValue(ns3::CallbackBase const & base) [constructor]
    cls.add_constructor([param('ns3::CallbackBase const &', 'base')])
    ## callback.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::CallbackValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## callback.h (module 'core'): bool ns3::CallbackValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## callback.h (module 'core'): std::string ns3::CallbackValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## callback.h (module 'core'): void ns3::CallbackValue::Set(ns3::CallbackBase base) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::CallbackBase', 'base')])
    return

def register_Ns3DoubleValue_methods(root_module, cls):
    ## double.h (module 'core'): ns3::DoubleValue::DoubleValue() [constructor]
    cls.add_constructor([])
    ## double.h (module 'core'): ns3::DoubleValue::DoubleValue(ns3::DoubleValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DoubleValue const &', 'arg0')])
    ## double.h (module 'core'): ns3::DoubleValue::DoubleValue(double const & value) [constructor]
    cls.add_constructor([param('double const &', 'value')])
    ## double.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::DoubleValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## double.h (module 'core'): bool ns3::DoubleValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## double.h (module 'core'): double ns3::DoubleValue::Get() const [member function]
    cls.add_method('Get', 
                   'double', 
                   [], 
                   is_const=True)
    ## double.h (module 'core'): std::string ns3::DoubleValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## double.h (module 'core'): void ns3::DoubleValue::Set(double const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('double const &', 'value')])
    return

def register_Ns3EmptyAttributeValue_methods(root_module, cls):
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue::EmptyAttributeValue(ns3::EmptyAttributeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmptyAttributeValue const &', 'arg0')])
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue::EmptyAttributeValue() [constructor]
    cls.add_constructor([])
    ## attribute.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::EmptyAttributeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## attribute.h (module 'core'): bool ns3::EmptyAttributeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   visibility='private', is_virtual=True)
    ## attribute.h (module 'core'): std::string ns3::EmptyAttributeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3EnumChecker_methods(root_module, cls):
    ## enum.h (module 'core'): ns3::EnumChecker::EnumChecker(ns3::EnumChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnumChecker const &', 'arg0')])
    ## enum.h (module 'core'): ns3::EnumChecker::EnumChecker() [constructor]
    cls.add_constructor([])
    ## enum.h (module 'core'): void ns3::EnumChecker::Add(int v, std::string name) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('int', 'v'), param('std::string', 'name')])
    ## enum.h (module 'core'): void ns3::EnumChecker::AddDefault(int v, std::string name) [member function]
    cls.add_method('AddDefault', 
                   'void', 
                   [param('int', 'v'), param('std::string', 'name')])
    ## enum.h (module 'core'): bool ns3::EnumChecker::Check(ns3::AttributeValue const & value) const [member function]
    cls.add_method('Check', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): bool ns3::EnumChecker::Copy(ns3::AttributeValue const & src, ns3::AttributeValue & dst) const [member function]
    cls.add_method('Copy', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'src'), param('ns3::AttributeValue &', 'dst')], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::EnumChecker::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): std::string ns3::EnumChecker::GetUnderlyingTypeInformation() const [member function]
    cls.add_method('GetUnderlyingTypeInformation', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): std::string ns3::EnumChecker::GetValueTypeName() const [member function]
    cls.add_method('GetValueTypeName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): bool ns3::EnumChecker::HasUnderlyingTypeInformation() const [member function]
    cls.add_method('HasUnderlyingTypeInformation', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3EnumValue_methods(root_module, cls):
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue(ns3::EnumValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnumValue const &', 'arg0')])
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue() [constructor]
    cls.add_constructor([])
    ## enum.h (module 'core'): ns3::EnumValue::EnumValue(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## enum.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::EnumValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): bool ns3::EnumValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## enum.h (module 'core'): int ns3::EnumValue::Get() const [member function]
    cls.add_method('Get', 
                   'int', 
                   [], 
                   is_const=True)
    ## enum.h (module 'core'): std::string ns3::EnumValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## enum.h (module 'core'): void ns3::EnumValue::Set(int v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int', 'v')])
    return

def register_Ns3EventImpl_methods(root_module, cls):
    ## event-impl.h (module 'core'): ns3::EventImpl::EventImpl(ns3::EventImpl const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EventImpl const &', 'arg0')])
    ## event-impl.h (module 'core'): ns3::EventImpl::EventImpl() [constructor]
    cls.add_constructor([])
    ## event-impl.h (module 'core'): void ns3::EventImpl::Cancel() [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [])
    ## event-impl.h (module 'core'): void ns3::EventImpl::Invoke() [member function]
    cls.add_method('Invoke', 
                   'void', 
                   [])
    ## event-impl.h (module 'core'): bool ns3::EventImpl::IsCancelled() [member function]
    cls.add_method('IsCancelled', 
                   'bool', 
                   [])
    ## event-impl.h (module 'core'): void ns3::EventImpl::Notify() [member function]
    cls.add_method('Notify', 
                   'void', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    return

def register_Ns3IntegerValue_methods(root_module, cls):
    ## integer.h (module 'core'): ns3::IntegerValue::IntegerValue() [constructor]
    cls.add_constructor([])
    ## integer.h (module 'core'): ns3::IntegerValue::IntegerValue(ns3::IntegerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntegerValue const &', 'arg0')])
    ## integer.h (module 'core'): ns3::IntegerValue::IntegerValue(int64_t const & value) [constructor]
    cls.add_constructor([param('int64_t const &', 'value')])
    ## integer.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::IntegerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## integer.h (module 'core'): bool ns3::IntegerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## integer.h (module 'core'): int64_t ns3::IntegerValue::Get() const [member function]
    cls.add_method('Get', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## integer.h (module 'core'): std::string ns3::IntegerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## integer.h (module 'core'): void ns3::IntegerValue::Set(int64_t const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int64_t const &', 'value')])
    return

def register_Ns3Ipv4AddressChecker_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker::Ipv4AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressChecker::Ipv4AddressChecker(ns3::Ipv4AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv4AddressValue_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue::Ipv4AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4AddressValue const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4AddressValue::Ipv4AddressValue(ns3::Ipv4Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Address const &', 'value')])
    ## ipv4-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv4AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h (module 'network'): ns3::Ipv4Address ns3::Ipv4AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Address', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): std::string ns3::Ipv4AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4AddressValue::Set(ns3::Ipv4Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Address const &', 'value')])
    return

def register_Ns3Ipv4MaskChecker_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker::Ipv4MaskChecker() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskChecker::Ipv4MaskChecker(ns3::Ipv4MaskChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskChecker const &', 'arg0')])
    return

def register_Ns3Ipv4MaskValue_methods(root_module, cls):
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue::Ipv4MaskValue() [constructor]
    cls.add_constructor([])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4MaskValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv4MaskValue const &', 'arg0')])
    ## ipv4-address.h (module 'network'): ns3::Ipv4MaskValue::Ipv4MaskValue(ns3::Ipv4Mask const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv4Mask const &', 'value')])
    ## ipv4-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv4MaskValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): bool ns3::Ipv4MaskValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv4-address.h (module 'network'): ns3::Ipv4Mask ns3::Ipv4MaskValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv4Mask', 
                   [], 
                   is_const=True)
    ## ipv4-address.h (module 'network'): std::string ns3::Ipv4MaskValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv4-address.h (module 'network'): void ns3::Ipv4MaskValue::Set(ns3::Ipv4Mask const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv4Mask const &', 'value')])
    return

def register_Ns3Ipv6AddressChecker_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker::Ipv6AddressChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressChecker::Ipv6AddressChecker(ns3::Ipv6AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressChecker const &', 'arg0')])
    return

def register_Ns3Ipv6AddressValue_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue::Ipv6AddressValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6AddressValue const &', 'arg0')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6AddressValue::Ipv6AddressValue(ns3::Ipv6Address const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Address const &', 'value')])
    ## ipv6-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv6AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h (module 'network'): ns3::Ipv6Address ns3::Ipv6AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Address', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): std::string ns3::Ipv6AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6AddressValue::Set(ns3::Ipv6Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Address const &', 'value')])
    return

def register_Ns3Ipv6PrefixChecker_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker::Ipv6PrefixChecker() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixChecker::Ipv6PrefixChecker(ns3::Ipv6PrefixChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixChecker const &', 'arg0')])
    return

def register_Ns3Ipv6PrefixValue_methods(root_module, cls):
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue::Ipv6PrefixValue() [constructor]
    cls.add_constructor([])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6PrefixValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ipv6PrefixValue const &', 'arg0')])
    ## ipv6-address.h (module 'network'): ns3::Ipv6PrefixValue::Ipv6PrefixValue(ns3::Ipv6Prefix const & value) [constructor]
    cls.add_constructor([param('ns3::Ipv6Prefix const &', 'value')])
    ## ipv6-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Ipv6PrefixValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6PrefixValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ipv6-address.h (module 'network'): ns3::Ipv6Prefix ns3::Ipv6PrefixValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Ipv6Prefix', 
                   [], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): std::string ns3::Ipv6PrefixValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ipv6-address.h (module 'network'): void ns3::Ipv6PrefixValue::Set(ns3::Ipv6Prefix const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Ipv6Prefix const &', 'value')])
    return

def register_Ns3L2Tracer_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## l2-tracer.h (module 'ndnSIM'): ns3::L2Tracer::L2Tracer(ns3::L2Tracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::L2Tracer const &', 'arg0')])
    ## l2-tracer.h (module 'ndnSIM'): ns3::L2Tracer::L2Tracer(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## l2-tracer.h (module 'ndnSIM'): void ns3::L2Tracer::Connect() [member function]
    cls.add_method('Connect', 
                   'void', 
                   [])
    ## l2-tracer.h (module 'ndnSIM'): void ns3::L2Tracer::Drop(ns3::Ptr<ns3::Packet const> arg0) [member function]
    cls.add_method('Drop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## l2-tracer.h (module 'ndnSIM'): void ns3::L2Tracer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## l2-tracer.h (module 'ndnSIM'): void ns3::L2Tracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3NetDevice_methods(root_module, cls):
    ## net-device.h (module 'network'): ns3::NetDevice::NetDevice() [constructor]
    cls.add_constructor([])
    ## net-device.h (module 'network'): ns3::NetDevice::NetDevice(ns3::NetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDevice const &', 'arg0')])
    ## net-device.h (module 'network'): void ns3::NetDevice::AddLinkChangeCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Ptr<ns3::Channel> ns3::NetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): uint32_t ns3::NetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): uint16_t ns3::NetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Address ns3::NetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): ns3::Ptr<ns3::Node> ns3::NetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): static ns3::TypeId ns3::NetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetPromiscReceiveCallback(ns3::Callback<bool,ns3::Ptr<ns3::NetDevice>,ns3::Ptr<const ns3::Packet>,short unsigned int,const ns3::Address&,const ns3::Address&,ns3::NetDevice::PacketType,ns3::empty,ns3::empty,ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, short unsigned int, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetReceiveCallback(ns3::Callback<bool,ns3::Ptr<ns3::NetDevice>,ns3::Ptr<const ns3::Packet>,short unsigned int,const ns3::Address&,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, short unsigned int, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): bool ns3::NetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3NixVector_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## nix-vector.h (module 'network'): ns3::NixVector::NixVector() [constructor]
    cls.add_constructor([])
    ## nix-vector.h (module 'network'): ns3::NixVector::NixVector(ns3::NixVector const & o) [copy constructor]
    cls.add_constructor([param('ns3::NixVector const &', 'o')])
    ## nix-vector.h (module 'network'): void ns3::NixVector::AddNeighborIndex(uint32_t newBits, uint32_t numberOfBits) [member function]
    cls.add_method('AddNeighborIndex', 
                   'void', 
                   [param('uint32_t', 'newBits'), param('uint32_t', 'numberOfBits')])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::BitCount(uint32_t numberOfNeighbors) const [member function]
    cls.add_method('BitCount', 
                   'uint32_t', 
                   [param('uint32_t', 'numberOfNeighbors')], 
                   is_const=True)
    ## nix-vector.h (module 'network'): ns3::Ptr<ns3::NixVector> ns3::NixVector::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::NixVector >', 
                   [], 
                   is_const=True)
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::Deserialize(uint32_t const * buffer, uint32_t size) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('uint32_t const *', 'buffer'), param('uint32_t', 'size')])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::ExtractNeighborIndex(uint32_t numberOfBits) [member function]
    cls.add_method('ExtractNeighborIndex', 
                   'uint32_t', 
                   [param('uint32_t', 'numberOfBits')])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::GetRemainingBits() [member function]
    cls.add_method('GetRemainingBits', 
                   'uint32_t', 
                   [])
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## nix-vector.h (module 'network'): uint32_t ns3::NixVector::Serialize(uint32_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint32_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    return

def register_Ns3Node_methods(root_module, cls):
    ## node.h (module 'network'): ns3::Node::Node(ns3::Node const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Node const &', 'arg0')])
    ## node.h (module 'network'): ns3::Node::Node() [constructor]
    cls.add_constructor([])
    ## node.h (module 'network'): ns3::Node::Node(uint32_t systemId) [constructor]
    cls.add_constructor([param('uint32_t', 'systemId')])
    ## node.h (module 'network'): uint32_t ns3::Node::AddApplication(ns3::Ptr<ns3::Application> application) [member function]
    cls.add_method('AddApplication', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::Application >', 'application')])
    ## node.h (module 'network'): uint32_t ns3::Node::AddDevice(ns3::Ptr<ns3::NetDevice> device) [member function]
    cls.add_method('AddDevice', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'device')])
    ## node.h (module 'network'): static bool ns3::Node::ChecksumEnabled() [member function]
    cls.add_method('ChecksumEnabled', 
                   'bool', 
                   [], 
                   is_static=True)
    ## node.h (module 'network'): ns3::Ptr<ns3::Application> ns3::Node::GetApplication(uint32_t index) const [member function]
    cls.add_method('GetApplication', 
                   'ns3::Ptr< ns3::Application >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::Node::GetDevice(uint32_t index) const [member function]
    cls.add_method('GetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [param('uint32_t', 'index')], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetNApplications() const [member function]
    cls.add_method('GetNApplications', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetNDevices() const [member function]
    cls.add_method('GetNDevices', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): uint32_t ns3::Node::GetSystemId() const [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## node.h (module 'network'): static ns3::TypeId ns3::Node::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## node.h (module 'network'): void ns3::Node::RegisterDeviceAdditionListener(ns3::Callback<void,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> listener) [member function]
    cls.add_method('RegisterDeviceAdditionListener', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'listener')])
    ## node.h (module 'network'): void ns3::Node::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler, uint16_t protocolType, ns3::Ptr<ns3::NetDevice> device, bool promiscuous=false) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler'), param('uint16_t', 'protocolType'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'promiscuous', default_value='false')])
    ## node.h (module 'network'): void ns3::Node::UnregisterDeviceAdditionListener(ns3::Callback<void,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> listener) [member function]
    cls.add_method('UnregisterDeviceAdditionListener', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'listener')])
    ## node.h (module 'network'): void ns3::Node::UnregisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('UnregisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'handler')])
    ## node.h (module 'network'): void ns3::Node::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## node.h (module 'network'): void ns3::Node::DoInitialize() [member function]
    cls.add_method('DoInitialize', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectFactoryChecker_methods(root_module, cls):
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker::ObjectFactoryChecker() [constructor]
    cls.add_constructor([])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryChecker::ObjectFactoryChecker(ns3::ObjectFactoryChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactoryChecker const &', 'arg0')])
    return

def register_Ns3ObjectFactoryValue_methods(root_module, cls):
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue::ObjectFactoryValue() [constructor]
    cls.add_constructor([])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue::ObjectFactoryValue(ns3::ObjectFactoryValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactoryValue const &', 'arg0')])
    ## object-factory.h (module 'core'): ns3::ObjectFactoryValue::ObjectFactoryValue(ns3::ObjectFactory const & value) [constructor]
    cls.add_constructor([param('ns3::ObjectFactory const &', 'value')])
    ## object-factory.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::ObjectFactoryValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object-factory.h (module 'core'): bool ns3::ObjectFactoryValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## object-factory.h (module 'core'): ns3::ObjectFactory ns3::ObjectFactoryValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::ObjectFactory', 
                   [], 
                   is_const=True)
    ## object-factory.h (module 'core'): std::string ns3::ObjectFactoryValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## object-factory.h (module 'core'): void ns3::ObjectFactoryValue::Set(ns3::ObjectFactory const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::ObjectFactory const &', 'value')])
    return

def register_Ns3Packet_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## packet.h (module 'network'): ns3::Packet::Packet() [constructor]
    cls.add_constructor([])
    ## packet.h (module 'network'): ns3::Packet::Packet(ns3::Packet const & o) [copy constructor]
    cls.add_constructor([param('ns3::Packet const &', 'o')])
    ## packet.h (module 'network'): ns3::Packet::Packet(uint32_t size) [constructor]
    cls.add_constructor([param('uint32_t', 'size')])
    ## packet.h (module 'network'): ns3::Packet::Packet(uint8_t const * buffer, uint32_t size, bool magic) [constructor]
    cls.add_constructor([param('uint8_t const *', 'buffer'), param('uint32_t', 'size'), param('bool', 'magic')])
    ## packet.h (module 'network'): ns3::Packet::Packet(uint8_t const * buffer, uint32_t size) [constructor]
    cls.add_constructor([param('uint8_t const *', 'buffer'), param('uint32_t', 'size')])
    ## packet.h (module 'network'): ns3::Packet::Packet(std::string const & buffer) [constructor]
    cls.add_constructor([param('std::string const &', 'buffer')])
    ## packet.h (module 'network'): void ns3::Packet::AddAtEnd(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('AddAtEnd', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## packet.h (module 'network'): void ns3::Packet::AddByteTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddByteTag', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::AddHeader(ns3::Header const & header) [member function]
    cls.add_method('AddHeader', 
                   'void', 
                   [param('ns3::Header const &', 'header')])
    ## packet.h (module 'network'): void ns3::Packet::AddPacketTag(ns3::Tag const & tag) const [member function]
    cls.add_method('AddPacketTag', 
                   'void', 
                   [param('ns3::Tag const &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::AddPaddingAtEnd(uint32_t size) [member function]
    cls.add_method('AddPaddingAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h (module 'network'): void ns3::Packet::AddTrailer(ns3::Trailer const & trailer) [member function]
    cls.add_method('AddTrailer', 
                   'void', 
                   [param('ns3::Trailer const &', 'trailer')])
    ## packet.h (module 'network'): ns3::PacketMetadata::ItemIterator ns3::Packet::BeginItem() const [member function]
    cls.add_method('BeginItem', 
                   'ns3::PacketMetadata::ItemIterator', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Packet::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::CopyData(uint8_t * buffer, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'size')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::CopyData(std::ostream * os, uint32_t size) const [member function]
    cls.add_method('CopyData', 
                   'void', 
                   [param('std::ostream *', 'os'), param('uint32_t', 'size')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Packet::CreateFragment(uint32_t start, uint32_t length) const [member function]
    cls.add_method('CreateFragment', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'start'), param('uint32_t', 'length')], 
                   is_const=True)
    ## packet.h (module 'network'): static void ns3::Packet::EnableChecking() [member function]
    cls.add_method('EnableChecking', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h (module 'network'): static void ns3::Packet::EnablePrinting() [member function]
    cls.add_method('EnablePrinting', 
                   'void', 
                   [], 
                   is_static=True)
    ## packet.h (module 'network'): bool ns3::Packet::FindFirstMatchingByteTag(ns3::Tag & tag) const [member function]
    cls.add_method('FindFirstMatchingByteTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::ByteTagIterator ns3::Packet::GetByteTagIterator() const [member function]
    cls.add_method('GetByteTagIterator', 
                   'ns3::ByteTagIterator', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::Ptr<ns3::NixVector> ns3::Packet::GetNixVector() const [member function]
    cls.add_method('GetNixVector', 
                   'ns3::Ptr< ns3::NixVector >', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): ns3::PacketTagIterator ns3::Packet::GetPacketTagIterator() const [member function]
    cls.add_method('GetPacketTagIterator', 
                   'ns3::PacketTagIterator', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint64_t ns3::Packet::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## packet.h (module 'network'): uint8_t const * ns3::Packet::PeekData() const [member function]
    cls.add_method('PeekData', 
                   'uint8_t const *', 
                   [], 
                   deprecated=True, is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::PeekHeader(ns3::Header & header) const [member function]
    cls.add_method('PeekHeader', 
                   'uint32_t', 
                   [param('ns3::Header &', 'header')], 
                   is_const=True)
    ## packet.h (module 'network'): bool ns3::Packet::PeekPacketTag(ns3::Tag & tag) const [member function]
    cls.add_method('PeekPacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')], 
                   is_const=True)
    ## packet.h (module 'network'): uint32_t ns3::Packet::PeekTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('PeekTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer &', 'trailer')])
    ## packet.h (module 'network'): void ns3::Packet::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::PrintByteTags(std::ostream & os) const [member function]
    cls.add_method('PrintByteTags', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::PrintPacketTags(std::ostream & os) const [member function]
    cls.add_method('PrintPacketTags', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::RemoveAllByteTags() [member function]
    cls.add_method('RemoveAllByteTags', 
                   'void', 
                   [])
    ## packet.h (module 'network'): void ns3::Packet::RemoveAllPacketTags() [member function]
    cls.add_method('RemoveAllPacketTags', 
                   'void', 
                   [])
    ## packet.h (module 'network'): void ns3::Packet::RemoveAtEnd(uint32_t size) [member function]
    cls.add_method('RemoveAtEnd', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h (module 'network'): void ns3::Packet::RemoveAtStart(uint32_t size) [member function]
    cls.add_method('RemoveAtStart', 
                   'void', 
                   [param('uint32_t', 'size')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::RemoveHeader(ns3::Header & header) [member function]
    cls.add_method('RemoveHeader', 
                   'uint32_t', 
                   [param('ns3::Header &', 'header')])
    ## packet.h (module 'network'): bool ns3::Packet::RemovePacketTag(ns3::Tag & tag) [member function]
    cls.add_method('RemovePacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::RemoveTrailer(ns3::Trailer & trailer) [member function]
    cls.add_method('RemoveTrailer', 
                   'uint32_t', 
                   [param('ns3::Trailer &', 'trailer')])
    ## packet.h (module 'network'): bool ns3::Packet::ReplacePacketTag(ns3::Tag & tag) [member function]
    cls.add_method('ReplacePacketTag', 
                   'bool', 
                   [param('ns3::Tag &', 'tag')])
    ## packet.h (module 'network'): uint32_t ns3::Packet::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::SetNixVector(ns3::Ptr<ns3::NixVector> nixVector) [member function]
    cls.add_method('SetNixVector', 
                   'void', 
                   [param('ns3::Ptr< ns3::NixVector >', 'nixVector')])
    return

def register_Ns3RandomVariableChecker_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker::RandomVariableChecker() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker::RandomVariableChecker(ns3::RandomVariableChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariableChecker const &', 'arg0')])
    return

def register_Ns3RandomVariableValue_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::RandomVariableValue::RandomVariableValue() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue::RandomVariableValue(ns3::RandomVariableValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariableValue const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue::RandomVariableValue(ns3::RandomVariable const & value) [constructor]
    cls.add_constructor([param('ns3::RandomVariable const &', 'value')])
    ## random-variable.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::RandomVariableValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## random-variable.h (module 'core'): bool ns3::RandomVariableValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## random-variable.h (module 'core'): ns3::RandomVariable ns3::RandomVariableValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::RandomVariable', 
                   [], 
                   is_const=True)
    ## random-variable.h (module 'core'): std::string ns3::RandomVariableValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## random-variable.h (module 'core'): void ns3::RandomVariableValue::Set(ns3::RandomVariable const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::RandomVariable const &', 'value')])
    return

def register_Ns3RocketfuelWeightsReader_methods(root_module, cls):
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): ns3::RocketfuelWeightsReader::RocketfuelWeightsReader(std::string const & path="", double scale=1.0e+0) [constructor]
    cls.add_constructor([param('std::string const &', 'path', default_value='""'), param('double', 'scale', default_value='1.0e+0')])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): void ns3::RocketfuelWeightsReader::SetFileType(uint8_t inputType) [member function]
    cls.add_method('SetFileType', 
                   'void', 
                   [param('uint8_t', 'inputType')])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): ns3::NodeContainer ns3::RocketfuelWeightsReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_virtual=True)
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): void ns3::RocketfuelWeightsReader::Commit() [member function]
    cls.add_method('Commit', 
                   'void', 
                   [])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): void ns3::RocketfuelWeightsReader::SetDefaultBandwidth(std::string const & bw) [member function]
    cls.add_method('SetDefaultBandwidth', 
                   'void', 
                   [param('std::string const &', 'bw')])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): std::string ns3::RocketfuelWeightsReader::GetDefaultBandwidth() const [member function]
    cls.add_method('GetDefaultBandwidth', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): void ns3::RocketfuelWeightsReader::SetDefaultQueue(std::string const & queue) [member function]
    cls.add_method('SetDefaultQueue', 
                   'void', 
                   [param('std::string const &', 'queue')])
    ## rocketfuel-weights-reader.h (module 'ndnSIM'): std::string ns3::RocketfuelWeightsReader::GetDefaultQueue() const [member function]
    cls.add_method('GetDefaultQueue', 
                   'std::string', 
                   [], 
                   is_const=True)
    return

def register_Ns3TimeValue_methods(root_module, cls):
    ## nstime.h (module 'core'): ns3::TimeValue::TimeValue() [constructor]
    cls.add_constructor([])
    ## nstime.h (module 'core'): ns3::TimeValue::TimeValue(ns3::TimeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeValue const &', 'arg0')])
    ## nstime.h (module 'core'): ns3::TimeValue::TimeValue(ns3::Time const & value) [constructor]
    cls.add_constructor([param('ns3::Time const &', 'value')])
    ## nstime.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::TimeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## nstime.h (module 'core'): bool ns3::TimeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## nstime.h (module 'core'): ns3::Time ns3::TimeValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## nstime.h (module 'core'): std::string ns3::TimeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## nstime.h (module 'core'): void ns3::TimeValue::Set(ns3::Time const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Time const &', 'value')])
    return

def register_Ns3TypeIdChecker_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeIdChecker::TypeIdChecker() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeIdChecker::TypeIdChecker(ns3::TypeIdChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeIdChecker const &', 'arg0')])
    return

def register_Ns3TypeIdValue_methods(root_module, cls):
    ## type-id.h (module 'core'): ns3::TypeIdValue::TypeIdValue() [constructor]
    cls.add_constructor([])
    ## type-id.h (module 'core'): ns3::TypeIdValue::TypeIdValue(ns3::TypeIdValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeIdValue const &', 'arg0')])
    ## type-id.h (module 'core'): ns3::TypeIdValue::TypeIdValue(ns3::TypeId const & value) [constructor]
    cls.add_constructor([param('ns3::TypeId const &', 'value')])
    ## type-id.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::TypeIdValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## type-id.h (module 'core'): bool ns3::TypeIdValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## type-id.h (module 'core'): ns3::TypeId ns3::TypeIdValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## type-id.h (module 'core'): std::string ns3::TypeIdValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## type-id.h (module 'core'): void ns3::TypeIdValue::Set(ns3::TypeId const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::TypeId const &', 'value')])
    return

def register_Ns3UintegerValue_methods(root_module, cls):
    ## uinteger.h (module 'core'): ns3::UintegerValue::UintegerValue() [constructor]
    cls.add_constructor([])
    ## uinteger.h (module 'core'): ns3::UintegerValue::UintegerValue(ns3::UintegerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UintegerValue const &', 'arg0')])
    ## uinteger.h (module 'core'): ns3::UintegerValue::UintegerValue(uint64_t const & value) [constructor]
    cls.add_constructor([param('uint64_t const &', 'value')])
    ## uinteger.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::UintegerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uinteger.h (module 'core'): bool ns3::UintegerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## uinteger.h (module 'core'): uint64_t ns3::UintegerValue::Get() const [member function]
    cls.add_method('Get', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## uinteger.h (module 'core'): std::string ns3::UintegerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## uinteger.h (module 'core'): void ns3::UintegerValue::Set(uint64_t const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint64_t const &', 'value')])
    return

def register_Ns3AddressChecker_methods(root_module, cls):
    ## address.h (module 'network'): ns3::AddressChecker::AddressChecker() [constructor]
    cls.add_constructor([])
    ## address.h (module 'network'): ns3::AddressChecker::AddressChecker(ns3::AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressChecker const &', 'arg0')])
    return

def register_Ns3AddressValue_methods(root_module, cls):
    ## address.h (module 'network'): ns3::AddressValue::AddressValue() [constructor]
    cls.add_constructor([])
    ## address.h (module 'network'): ns3::AddressValue::AddressValue(ns3::AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AddressValue const &', 'arg0')])
    ## address.h (module 'network'): ns3::AddressValue::AddressValue(ns3::Address const & value) [constructor]
    cls.add_constructor([param('ns3::Address const &', 'value')])
    ## address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## address.h (module 'network'): bool ns3::AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## address.h (module 'network'): ns3::Address ns3::AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## address.h (module 'network'): std::string ns3::AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## address.h (module 'network'): void ns3::AddressValue::Set(ns3::Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Address const &', 'value')])
    return

def register_Ns3L2RateTracer_methods(root_module, cls):
    ## l2-rate-tracer.h (module 'ndnSIM'): ns3::L2RateTracer::L2RateTracer(ns3::L2RateTracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::L2RateTracer const &', 'arg0')])
    ## l2-rate-tracer.h (module 'ndnSIM'): ns3::L2RateTracer::L2RateTracer(boost::shared_ptr<std::ostream> os, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('ns3::Ptr< ns3::Node >', 'node')])
    ## l2-rate-tracer.h (module 'ndnSIM'): static void ns3::L2RateTracer::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## l2-rate-tracer.h (module 'ndnSIM'): void ns3::L2RateTracer::Drop(ns3::Ptr<ns3::Packet const> arg0) [member function]
    cls.add_method('Drop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'arg0')], 
                   is_virtual=True)
    ## l2-rate-tracer.h (module 'ndnSIM'): static void ns3::L2RateTracer::InstallAll(std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## l2-rate-tracer.h (module 'ndnSIM'): void ns3::L2RateTracer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## l2-rate-tracer.h (module 'ndnSIM'): void ns3::L2RateTracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## l2-rate-tracer.h (module 'ndnSIM'): void ns3::L2RateTracer::SetAveragingPeriod(ns3::Time const & period) [member function]
    cls.add_method('SetAveragingPeriod', 
                   'void', 
                   [param('ns3::Time const &', 'period')])
    return

def register_Ns3HashImplementation_methods(root_module, cls):
    ## hash-function.h (module 'core'): ns3::Hash::Implementation::Implementation(ns3::Hash::Implementation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Implementation const &', 'arg0')])
    ## hash-function.h (module 'core'): ns3::Hash::Implementation::Implementation() [constructor]
    cls.add_constructor([])
    ## hash-function.h (module 'core'): uint32_t ns3::Hash::Implementation::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_pure_virtual=True, is_virtual=True)
    ## hash-function.h (module 'core'): uint64_t ns3::Hash::Implementation::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): void ns3::Hash::Implementation::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3HashFunctionFnv1a_methods(root_module, cls):
    ## hash-fnv.h (module 'core'): ns3::Hash::Function::Fnv1a::Fnv1a(ns3::Hash::Function::Fnv1a const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Fnv1a const &', 'arg0')])
    ## hash-fnv.h (module 'core'): ns3::Hash::Function::Fnv1a::Fnv1a() [constructor]
    cls.add_constructor([])
    ## hash-fnv.h (module 'core'): uint32_t ns3::Hash::Function::Fnv1a::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-fnv.h (module 'core'): uint64_t ns3::Hash::Function::Fnv1a::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-fnv.h (module 'core'): void ns3::Hash::Function::Fnv1a::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3HashFunctionHash32_methods(root_module, cls):
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash32::Hash32(ns3::Hash::Function::Hash32 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Hash32 const &', 'arg0')])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash32::Hash32(ns3::Hash::Hash32Function_ptr hp) [constructor]
    cls.add_constructor([param('ns3::Hash::Hash32Function_ptr', 'hp')])
    ## hash-function.h (module 'core'): uint32_t ns3::Hash::Function::Hash32::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): void ns3::Hash::Function::Hash32::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3HashFunctionHash64_methods(root_module, cls):
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash64::Hash64(ns3::Hash::Function::Hash64 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Hash64 const &', 'arg0')])
    ## hash-function.h (module 'core'): ns3::Hash::Function::Hash64::Hash64(ns3::Hash::Hash64Function_ptr hp) [constructor]
    cls.add_constructor([param('ns3::Hash::Hash64Function_ptr', 'hp')])
    ## hash-function.h (module 'core'): uint32_t ns3::Hash::Function::Hash64::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): uint64_t ns3::Hash::Function::Hash64::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-function.h (module 'core'): void ns3::Hash::Function::Hash64::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3HashFunctionMurmur3_methods(root_module, cls):
    ## hash-murmur3.h (module 'core'): ns3::Hash::Function::Murmur3::Murmur3(ns3::Hash::Function::Murmur3 const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Hash::Function::Murmur3 const &', 'arg0')])
    ## hash-murmur3.h (module 'core'): ns3::Hash::Function::Murmur3::Murmur3() [constructor]
    cls.add_constructor([])
    ## hash-murmur3.h (module 'core'): uint32_t ns3::Hash::Function::Murmur3::GetHash32(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash32', 
                   'uint32_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-murmur3.h (module 'core'): uint64_t ns3::Hash::Function::Murmur3::GetHash64(char const * buffer, size_t const size) [member function]
    cls.add_method('GetHash64', 
                   'uint64_t', 
                   [param('char const *', 'buffer'), param('size_t const', 'size')], 
                   is_virtual=True)
    ## hash-murmur3.h (module 'core'): void ns3::Hash::Function::Murmur3::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3NdnApp_methods(root_module, cls):
    ## ndn-app.h (module 'ndnSIM'): ns3::ndn::App::App(ns3::ndn::App const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::App const &', 'arg0')])
    ## ndn-app.h (module 'ndnSIM'): ns3::ndn::App::App() [constructor]
    cls.add_constructor([])
    ## ndn-app.h (module 'ndnSIM'): uint32_t ns3::ndn::App::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-app.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::App::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-app.h (module 'ndnSIM'): void ns3::ndn::App::OnData(ns3::Ptr<ns3::ndn::Data const> contentObject) [member function]
    cls.add_method('OnData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'contentObject')], 
                   is_virtual=True)
    ## ndn-app.h (module 'ndnSIM'): void ns3::ndn::App::OnInterest(ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('OnInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   is_virtual=True)
    ## ndn-app.h (module 'ndnSIM'): void ns3::ndn::App::OnNack(ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('OnNack', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   is_virtual=True)
    ## ndn-app.h (module 'ndnSIM'): void ns3::ndn::App::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ndn-app.h (module 'ndnSIM'): void ns3::ndn::App::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ndn-app.h (module 'ndnSIM'): void ns3::ndn::App::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NdnAppDelayTracer_methods(root_module, cls):
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): ns3::ndn::AppDelayTracer::AppDelayTracer(ns3::ndn::AppDelayTracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::AppDelayTracer const &', 'arg0')])
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): ns3::ndn::AppDelayTracer::AppDelayTracer(boost::shared_ptr<std::ostream> os, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): ns3::ndn::AppDelayTracer::AppDelayTracer(boost::shared_ptr<std::ostream> os, std::string const & node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('std::string const &', 'node')])
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): static void ns3::ndn::AppDelayTracer::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): static void ns3::ndn::AppDelayTracer::Install(ns3::NodeContainer const & nodes, std::string const & file) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer const &', 'nodes'), param('std::string const &', 'file')], 
                   is_static=True)
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): static void ns3::ndn::AppDelayTracer::Install(ns3::Ptr<ns3::Node> node, std::string const & file) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'file')], 
                   is_static=True)
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::AppDelayTracer> ns3::ndn::AppDelayTracer::Install(ns3::Ptr<ns3::Node> node, boost::shared_ptr<std::ostream> outputStream) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::AppDelayTracer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('boost::shared_ptr< std::ostream >', 'outputStream')], 
                   is_static=True)
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): static void ns3::ndn::AppDelayTracer::InstallAll(std::string const & file) [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [param('std::string const &', 'file')], 
                   is_static=True)
    ## ndn-app-delay-tracer.h (module 'ndnSIM'): void ns3::ndn::AppDelayTracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3NdnAppHelper_methods(root_module, cls):
    ## ndn-app-helper.h (module 'ndnSIM'): ns3::ndn::AppHelper::AppHelper(ns3::ndn::AppHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::AppHelper const &', 'arg0')])
    ## ndn-app-helper.h (module 'ndnSIM'): ns3::ndn::AppHelper::AppHelper(std::string const & prefix) [constructor]
    cls.add_constructor([param('std::string const &', 'prefix')])
    ## ndn-app-helper.h (module 'ndnSIM'): ns3::ApplicationContainer ns3::ndn::AppHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## ndn-app-helper.h (module 'ndnSIM'): ns3::ApplicationContainer ns3::ndn::AppHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-app-helper.h (module 'ndnSIM'): ns3::ApplicationContainer ns3::ndn::AppHelper::Install(std::string nodeName) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')])
    ## ndn-app-helper.h (module 'ndnSIM'): void ns3::ndn::AppHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## ndn-app-helper.h (module 'ndnSIM'): void ns3::ndn::AppHelper::SetPrefix(std::string const & prefix) [member function]
    cls.add_method('SetPrefix', 
                   'void', 
                   [param('std::string const &', 'prefix')])
    return

def register_Ns3NdnBlob_methods(root_module, cls):
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    ## blob.h (module 'ndnSIM'): ns3::ndn::Blob::Blob(ns3::ndn::Blob const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Blob const &', 'arg0')])
    ## blob.h (module 'ndnSIM'): ns3::ndn::Blob::Blob() [constructor]
    cls.add_constructor([])
    ## blob.h (module 'ndnSIM'): ns3::ndn::Blob::Blob(std::string const & data) [constructor]
    cls.add_constructor([param('std::string const &', 'data')])
    ## blob.h (module 'ndnSIM'): ns3::ndn::Blob::Blob(void const * buf, size_t length) [constructor]
    cls.add_constructor([param('void const *', 'buf'), param('size_t', 'length')])
    ## blob.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > ns3::ndn::Blob::begin() [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< char *, std::vector< char > >', 
                   [])
    ## blob.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<char const*, std::vector<char, std::allocator<char> > > ns3::ndn::Blob::begin() const [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< char const *, std::vector< char > >', 
                   [], 
                   is_const=True)
    ## blob.h (module 'ndnSIM'): char * ns3::ndn::Blob::buf() [member function]
    cls.add_method('buf', 
                   'char *', 
                   [])
    ## blob.h (module 'ndnSIM'): char const * ns3::ndn::Blob::buf() const [member function]
    cls.add_method('buf', 
                   'char const *', 
                   [], 
                   is_const=True)
    ## blob.h (module 'ndnSIM'): void ns3::ndn::Blob::clear() [member function]
    cls.add_method('clear', 
                   'void', 
                   [])
    ## blob.h (module 'ndnSIM'): bool ns3::ndn::Blob::empty() const [member function]
    cls.add_method('empty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## blob.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<char*, std::vector<char, std::allocator<char> > > ns3::ndn::Blob::end() [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< char *, std::vector< char > >', 
                   [])
    ## blob.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<char const*, std::vector<char, std::allocator<char> > > ns3::ndn::Blob::end() const [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< char const *, std::vector< char > >', 
                   [], 
                   is_const=True)
    ## blob.h (module 'ndnSIM'): char ns3::ndn::Blob::getItem(size_t pos) const [member function]
    cls.add_method('getItem', 
                   'char', 
                   [param('size_t', 'pos')], 
                   is_const=True)
    ## blob.h (module 'ndnSIM'): void ns3::ndn::Blob::push_back(char val) [member function]
    cls.add_method('push_back', 
                   'void', 
                   [param('char', 'val')])
    ## blob.h (module 'ndnSIM'): size_t ns3::ndn::Blob::size() const [member function]
    cls.add_method('size', 
                   'size_t', 
                   [], 
                   is_const=True)
    ## blob.h (module 'ndnSIM'): void ns3::ndn::Blob::swap(ns3::ndn::Blob & x) [member function]
    cls.add_method('swap', 
                   'void', 
                   [param('ns3::ndn::Blob &', 'x')])
    return

def register_Ns3NdnContentStore_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::ContentStore::ContentStore() [constructor]
    cls.add_constructor([])
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::ContentStore::ContentStore(ns3::ndn::ContentStore const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::ContentStore const &', 'arg0')])
    ## ndn-content-store.h (module 'ndnSIM'): bool ns3::ndn::ContentStore::Add(ns3::Ptr<ns3::ndn::Data const> data) [member function]
    cls.add_method('Add', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'data')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-content-store.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::cs::Entry> ns3::ndn::ContentStore::Begin() [member function]
    cls.add_method('Begin', 
                   'ns3::Ptr< ns3::ndn::cs::Entry >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-content-store.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::cs::Entry> ns3::ndn::ContentStore::End() [member function]
    cls.add_method('End', 
                   'ns3::Ptr< ns3::ndn::cs::Entry >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-content-store.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::ContentStore> ns3::ndn::ContentStore::GetContentStore(ns3::Ptr<ns3::Object> node) [member function]
    cls.add_method('GetContentStore', 
                   'ns3::Ptr< ns3::ndn::ContentStore >', 
                   [param('ns3::Ptr< ns3::Object >', 'node')], 
                   is_static=True)
    ## ndn-content-store.h (module 'ndnSIM'): uint32_t ns3::ndn::ContentStore::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-content-store.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::ContentStore::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-content-store.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Data> ns3::ndn::ContentStore::Lookup(ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('Lookup', 
                   'ns3::Ptr< ns3::ndn::Data >', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-content-store.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::cs::Entry> ns3::ndn::ContentStore::Next(ns3::Ptr<ns3::ndn::cs::Entry> arg0) [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::ndn::cs::Entry >', 
                   [param('ns3::Ptr< ns3::ndn::cs::Entry >', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-content-store.h (module 'ndnSIM'): void ns3::ndn::ContentStore::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3NdnCsTracer_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::CsTracer::CsTracer(ns3::ndn::CsTracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::CsTracer const &', 'arg0')])
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::CsTracer::CsTracer(boost::shared_ptr<std::ostream> os, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::CsTracer::CsTracer(boost::shared_ptr<std::ostream> os, std::string const & node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('std::string const &', 'node')])
    ## ndn-cs-tracer.h (module 'ndnSIM'): static void ns3::ndn::CsTracer::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## ndn-cs-tracer.h (module 'ndnSIM'): static void ns3::ndn::CsTracer::Install(ns3::NodeContainer const & nodes, std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer const &', 'nodes'), param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-cs-tracer.h (module 'ndnSIM'): static void ns3::ndn::CsTracer::Install(ns3::Ptr<ns3::Node> node, std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-cs-tracer.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::CsTracer> ns3::ndn::CsTracer::Install(ns3::Ptr<ns3::Node> node, boost::shared_ptr<std::ostream> outputStream, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::CsTracer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('boost::shared_ptr< std::ostream >', 'outputStream'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-cs-tracer.h (module 'ndnSIM'): static void ns3::ndn::CsTracer::InstallAll(std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-cs-tracer.h (module 'ndnSIM'): void ns3::ndn::CsTracer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ndn-cs-tracer.h (module 'ndnSIM'): void ns3::ndn::CsTracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3NdnData_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::Data::Data(ns3::Ptr<ns3::Packet> payload=ns3::Create( )) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet >', 'payload', default_value='ns3::Create( )')])
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::Data::Data(ns3::ndn::Data const & other) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Data const &', 'other')])
    ## ndn-data.h (module 'ndnSIM'): ns3::Time ns3::ndn::Data::GetFreshness() const [member function]
    cls.add_method('GetFreshness', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Name const> ns3::ndn::Data::GetKeyLocator() const [member function]
    cls.add_method('GetKeyLocator', 
                   'ns3::Ptr< ns3::ndn::Name const >', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::Name const & ns3::ndn::Data::GetName() const [member function]
    cls.add_method('GetName', 
                   'ns3::ndn::Name const &', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Name const> ns3::ndn::Data::GetNamePtr() const [member function]
    cls.add_method('GetNamePtr', 
                   'ns3::Ptr< ns3::ndn::Name const >', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): ns3::Ptr<ns3::Packet const> ns3::ndn::Data::GetPayload() const [member function]
    cls.add_method('GetPayload', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): uint32_t ns3::ndn::Data::GetSignature() const [member function]
    cls.add_method('GetSignature', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): ns3::Time ns3::ndn::Data::GetTimestamp() const [member function]
    cls.add_method('GetTimestamp', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): ns3::Ptr<ns3::Packet const> ns3::ndn::Data::GetWire() const [member function]
    cls.add_method('GetWire', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetFreshness(ns3::Time const & freshness) [member function]
    cls.add_method('SetFreshness', 
                   'void', 
                   [param('ns3::Time const &', 'freshness')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetKeyLocator(ns3::Ptr<ns3::ndn::Name> keyLocator) [member function]
    cls.add_method('SetKeyLocator', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Name >', 'keyLocator')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetName(ns3::Ptr<ns3::ndn::Name> name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Name >', 'name')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetName(ns3::ndn::Name const & name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('ns3::ndn::Name const &', 'name')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetPayload(ns3::Ptr<ns3::Packet> payload) [member function]
    cls.add_method('SetPayload', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'payload')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetSignature(uint32_t signature) [member function]
    cls.add_method('SetSignature', 
                   'void', 
                   [param('uint32_t', 'signature')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetTimestamp(ns3::Time const & timestamp) [member function]
    cls.add_method('SetTimestamp', 
                   'void', 
                   [param('ns3::Time const &', 'timestamp')])
    ## ndn-data.h (module 'ndnSIM'): void ns3::ndn::Data::SetWire(ns3::Ptr<ns3::Packet const> packet) const [member function]
    cls.add_method('SetWire', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_const=True)
    return

def register_Ns3NdnDataException_methods(root_module, cls):
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::DataException::DataException() [constructor]
    cls.add_constructor([])
    ## ndn-data.h (module 'ndnSIM'): ns3::ndn::DataException::DataException(ns3::ndn::DataException const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::DataException const &', 'arg0')])
    return

def register_Ns3NdnExclude_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude::Exclude(ns3::ndn::Exclude const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Exclude const &', 'arg0')])
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude::Exclude() [constructor]
    cls.add_constructor([])
    ## exclude.h (module 'ndnSIM'): void ns3::ndn::Exclude::appendExclude(ns3::ndn::name::Component const & name, bool any) [member function]
    cls.add_method('appendExclude', 
                   'void', 
                   [param('ns3::ndn::name::Component const &', 'name'), param('bool', 'any')])
    ## exclude.h (module 'ndnSIM'): std::_Rb_tree_const_iterator<std::pair<ns3::ndn::name::Component const, bool> > ns3::ndn::Exclude::begin() const [member function]
    cls.add_method('begin', 
                   'std::_Rb_tree_const_iterator< std::pair< ns3::ndn::name::Component const, bool > >', 
                   [], 
                   is_const=True)
    ## exclude.h (module 'ndnSIM'): std::_Rb_tree_const_iterator<std::pair<ns3::ndn::name::Component const, bool> > ns3::ndn::Exclude::end() const [member function]
    cls.add_method('end', 
                   'std::_Rb_tree_const_iterator< std::pair< ns3::ndn::name::Component const, bool > >', 
                   [], 
                   is_const=True)
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude & ns3::ndn::Exclude::excludeAfter(ns3::ndn::name::Component const & from) [member function]
    cls.add_method('excludeAfter', 
                   'ns3::ndn::Exclude &', 
                   [param('ns3::ndn::name::Component const &', 'from')])
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude & ns3::ndn::Exclude::excludeBefore(ns3::ndn::name::Component const & to) [member function]
    cls.add_method('excludeBefore', 
                   'ns3::ndn::Exclude &', 
                   [param('ns3::ndn::name::Component const &', 'to')])
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude & ns3::ndn::Exclude::excludeOne(ns3::ndn::name::Component const & comp) [member function]
    cls.add_method('excludeOne', 
                   'ns3::ndn::Exclude &', 
                   [param('ns3::ndn::name::Component const &', 'comp')])
    ## exclude.h (module 'ndnSIM'): ns3::ndn::Exclude & ns3::ndn::Exclude::excludeRange(ns3::ndn::name::Component const & from, ns3::ndn::name::Component const & to) [member function]
    cls.add_method('excludeRange', 
                   'ns3::ndn::Exclude &', 
                   [param('ns3::ndn::name::Component const &', 'from'), param('ns3::ndn::name::Component const &', 'to')])
    ## exclude.h (module 'ndnSIM'): bool ns3::ndn::Exclude::isExcluded(ns3::ndn::name::Component const & comp) const [member function]
    cls.add_method('isExcluded', 
                   'bool', 
                   [param('ns3::ndn::name::Component const &', 'comp')], 
                   is_const=True)
    ## exclude.h (module 'ndnSIM'): std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<ns3::ndn::name::Component const, bool> > > ns3::ndn::Exclude::rbegin() const [member function]
    cls.add_method('rbegin', 
                   'std::reverse_iterator< std::_Rb_tree_const_iterator< std::pair< ns3::ndn::name::Component const, bool > > >', 
                   [], 
                   is_const=True)
    ## exclude.h (module 'ndnSIM'): std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<ns3::ndn::name::Component const, bool> > > ns3::ndn::Exclude::rend() const [member function]
    cls.add_method('rend', 
                   'std::reverse_iterator< std::_Rb_tree_const_iterator< std::pair< ns3::ndn::name::Component const, bool > > >', 
                   [], 
                   is_const=True)
    ## exclude.h (module 'ndnSIM'): size_t ns3::ndn::Exclude::size() const [member function]
    cls.add_method('size', 
                   'size_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NdnFace_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## ndn-face.h (module 'ndnSIM'): ns3::ndn::Face::Face(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-face.h (module 'ndnSIM'): uint32_t ns3::ndn::Face::GetFlags() const [member function]
    cls.add_method('GetFlags', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-face.h (module 'ndnSIM'): uint32_t ns3::ndn::Face::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-face.h (module 'ndnSIM'): uint16_t ns3::ndn::Face::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): ns3::Ptr<ns3::Node> ns3::ndn::Face::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True)
    ## ndn-face.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::Face::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::IsUp() const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndn-face.h (module 'ndnSIM'): std::ostream & ns3::ndn::Face::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'std::ostream &', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::ReceiveData(ns3::Ptr<ns3::ndn::Data> data) [member function]
    cls.add_method('ReceiveData', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Data >', 'data')], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::ReceiveInterest(ns3::Ptr<ns3::ndn::Interest> interest) [member function]
    cls.add_method('ReceiveInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Interest >', 'interest')], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): void ns3::ndn::Face::RegisterProtocolHandlers(ns3::Callback<void, ns3::Ptr<ns3::ndn::Face>, ns3::Ptr<ns3::ndn::Interest>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> const & interestHandler, ns3::Callback<void, ns3::Ptr<ns3::ndn::Face>, ns3::Ptr<ns3::ndn::Data>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> const & dataHandler) [member function]
    cls.add_method('RegisterProtocolHandlers', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Face >, ns3::Ptr< ns3::ndn::Interest >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty > const &', 'interestHandler'), param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Face >, ns3::Ptr< ns3::ndn::Data >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty > const &', 'dataHandler')], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::SendData(ns3::Ptr<ns3::ndn::Data const> data) [member function]
    cls.add_method('SendData', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'data')], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::SendInterest(ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('SendInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): void ns3::ndn::Face::SetId(uint32_t id) [member function]
    cls.add_method('SetId', 
                   'void', 
                   [param('uint32_t', 'id')])
    ## ndn-face.h (module 'ndnSIM'): void ns3::ndn::Face::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): void ns3::ndn::Face::SetUp(bool up=true) [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [param('bool', 'up', default_value='true')])
    ## ndn-face.h (module 'ndnSIM'): void ns3::ndn::Face::UnRegisterProtocolHandlers() [member function]
    cls.add_method('UnRegisterProtocolHandlers', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::Receive(ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Receive', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   visibility='protected', is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): bool ns3::ndn::Face::Send(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   visibility='protected', is_virtual=True)
    ## ndn-face.h (module 'ndnSIM'): void ns3::ndn::Face::SetFlags(uint32_t flags) [member function]
    cls.add_method('SetFlags', 
                   'void', 
                   [param('uint32_t', 'flags')], 
                   visibility='protected')
    return

def register_Ns3NdnFaceContainer_methods(root_module, cls):
    ## ndn-face-container.h (module 'ndnSIM'): ns3::ndn::FaceContainer::FaceContainer() [constructor]
    cls.add_constructor([])
    ## ndn-face-container.h (module 'ndnSIM'): ns3::ndn::FaceContainer::FaceContainer(ns3::ndn::FaceContainer const & other) [copy constructor]
    cls.add_constructor([param('ns3::ndn::FaceContainer const &', 'other')])
    ## ndn-face-container.h (module 'ndnSIM'): void ns3::ndn::FaceContainer::Add(ns3::Ptr<ns3::ndn::Face> const & face) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face > const &', 'face')])
    ## ndn-face-container.h (module 'ndnSIM'): void ns3::ndn::FaceContainer::AddAll(ns3::Ptr<ns3::ndn::FaceContainer> other) [member function]
    cls.add_method('AddAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::FaceContainer >', 'other')])
    ## ndn-face-container.h (module 'ndnSIM'): void ns3::ndn::FaceContainer::AddAll(ns3::ndn::FaceContainer const & other) [member function]
    cls.add_method('AddAll', 
                   'void', 
                   [param('ns3::ndn::FaceContainer const &', 'other')])
    ## ndn-face-container.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::ndn::Face>*,std::vector<ns3::Ptr<ns3::ndn::Face>, std::allocator<ns3::Ptr<ns3::ndn::Face> > > > ns3::ndn::FaceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::ndn::Face > const, std::vector< ns3::Ptr< ns3::ndn::Face > > >', 
                   [], 
                   is_const=True)
    ## ndn-face-container.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::ndn::Face>*,std::vector<ns3::Ptr<ns3::ndn::Face>, std::allocator<ns3::Ptr<ns3::ndn::Face> > > > ns3::ndn::FaceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::ndn::Face > const, std::vector< ns3::Ptr< ns3::ndn::Face > > >', 
                   [], 
                   is_const=True)
    ## ndn-face-container.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Face> ns3::ndn::FaceContainer::Get(__gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::ndn::Face>*,std::vector<ns3::Ptr<ns3::ndn::Face>, std::allocator<ns3::Ptr<ns3::ndn::Face> > > > i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::ndn::Face >', 
                   [param('__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::ndn::Face > const, std::vector< ns3::Ptr< ns3::ndn::Face > > >', 'i')], 
                   is_const=True)
    ## ndn-face-container.h (module 'ndnSIM'): uint32_t ns3::ndn::FaceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NdnFib_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-fib.h (module 'ndnSIM'): ns3::ndn::Fib::Fib() [constructor]
    cls.add_constructor([])
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::Add(ns3::ndn::Name const & prefix, ns3::Ptr<ns3::ndn::Face> face, int32_t metric) [member function]
    cls.add_method('Add', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [param('ns3::ndn::Name const &', 'prefix'), param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('int32_t', 'metric')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::Add(ns3::Ptr<ns3::ndn::Name const> const & prefix, ns3::Ptr<ns3::ndn::Face> face, int32_t metric) [member function]
    cls.add_method('Add', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [param('ns3::Ptr< ns3::ndn::Name const > const &', 'prefix'), param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('int32_t', 'metric')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<const ns3::ndn::fib::Entry> ns3::ndn::Fib::Begin() const [member function]
    cls.add_method('Begin', 
                   'ns3::Ptr< ns3::ndn::fib::Entry const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::Begin() [member function]
    cls.add_method('Begin', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<const ns3::ndn::fib::Entry> ns3::ndn::Fib::End() const [member function]
    cls.add_method('End', 
                   'ns3::Ptr< ns3::ndn::fib::Entry const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::End() [member function]
    cls.add_method('End', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::Find(ns3::ndn::Name const & prefix) [member function]
    cls.add_method('Find', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [param('ns3::ndn::Name const &', 'prefix')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Fib> ns3::ndn::Fib::GetFib(ns3::Ptr<ns3::Object> node) [member function]
    cls.add_method('GetFib', 
                   'ns3::Ptr< ns3::ndn::Fib >', 
                   [param('ns3::Ptr< ns3::Object >', 'node')], 
                   is_static=True)
    ## ndn-fib.h (module 'ndnSIM'): uint32_t ns3::ndn::Fib::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::Fib::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-fib.h (module 'ndnSIM'): void ns3::ndn::Fib::InvalidateAll() [member function]
    cls.add_method('InvalidateAll', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::LongestPrefixMatch(ns3::ndn::Interest const & interest) [member function]
    cls.add_method('LongestPrefixMatch', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [param('ns3::ndn::Interest const &', 'interest')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<const ns3::ndn::fib::Entry> ns3::ndn::Fib::Next(ns3::Ptr<const ns3::ndn::fib::Entry> arg0) const [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::ndn::fib::Entry const >', 
                   [param('ns3::Ptr< ns3::ndn::fib::Entry const >', 'arg0')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::Fib::Next(ns3::Ptr<ns3::ndn::fib::Entry> arg0) [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [param('ns3::Ptr< ns3::ndn::fib::Entry >', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): void ns3::ndn::Fib::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): void ns3::ndn::Fib::Remove(ns3::Ptr<ns3::ndn::Name const> const & prefix) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Name const > const &', 'prefix')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-fib.h (module 'ndnSIM'): void ns3::ndn::Fib::RemoveFromAll(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('RemoveFromAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3NdnForwardingStrategy_methods(root_module, cls):
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): ns3::ndn::ForwardingStrategy::ForwardingStrategy(ns3::ndn::ForwardingStrategy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::ForwardingStrategy const &', 'arg0')])
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): ns3::ndn::ForwardingStrategy::ForwardingStrategy() [constructor]
    cls.add_constructor([])
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::AddFace(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('AddFace', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidAddFibEntry(ns3::Ptr<ns3::ndn::fib::Entry> fibEntry) [member function]
    cls.add_method('DidAddFibEntry', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::fib::Entry >', 'fibEntry')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): static std::string ns3::ndn::ForwardingStrategy::GetLogName() [member function]
    cls.add_method('GetLogName', 
                   'std::string', 
                   [], 
                   is_static=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::ForwardingStrategy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::OnData(ns3::Ptr<ns3::ndn::Face> face, ns3::Ptr<ns3::ndn::Data> data) [member function]
    cls.add_method('OnData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('ns3::Ptr< ns3::ndn::Data >', 'data')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::OnInterest(ns3::Ptr<ns3::ndn::Face> face, ns3::Ptr<ns3::ndn::Interest> interest) [member function]
    cls.add_method('OnInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('ns3::Ptr< ns3::ndn::Interest >', 'interest')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::RemoveFace(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('RemoveFace', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::WillEraseTimedOutPendingInterest(ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('WillEraseTimedOutPendingInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::WillRemoveFibEntry(ns3::Ptr<ns3::ndn::fib::Entry> fibEntry) [member function]
    cls.add_method('WillRemoveFibEntry', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::fib::Entry >', 'fibEntry')], 
                   is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): bool ns3::ndn::ForwardingStrategy::CanSendOutInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Face> outFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('CanSendOutInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Face >', 'outFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): bool ns3::ndn::ForwardingStrategy::DetectRetransmittedInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DetectRetransmittedInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidCreatePitEntry(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidCreatePitEntry', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidExhaustForwardingOptions(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidExhaustForwardingOptions', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidForwardSimilarInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidForwardSimilarInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidReceiveDuplicateInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidReceiveDuplicateInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidReceiveSolicitedData(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Data const> data, bool didCreateCacheEntry) [member function]
    cls.add_method('DidReceiveSolicitedData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Data const >', 'data'), param('bool', 'didCreateCacheEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidReceiveUnsolicitedData(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Data const> data, bool didCreateCacheEntry) [member function]
    cls.add_method('DidReceiveUnsolicitedData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Data const >', 'data'), param('bool', 'didCreateCacheEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidSendOutData(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Face> outFace, ns3::Ptr<ns3::ndn::Data const> data, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidSendOutData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Face >', 'outFace'), param('ns3::Ptr< ns3::ndn::Data const >', 'data'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidSendOutInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Face> outFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidSendOutInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Face >', 'outFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DidSuppressSimilarInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DidSuppressSimilarInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): bool ns3::ndn::ForwardingStrategy::DoPropagateInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('DoPropagateInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::FailedToCreatePitEntry(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('FailedToCreatePitEntry', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::PropagateInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('PropagateInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::SatisfyPendingInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Data const> data, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('SatisfyPendingInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Data const >', 'data'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): bool ns3::ndn::ForwardingStrategy::ShouldSuppressIncomingInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('ShouldSuppressIncomingInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): bool ns3::ndn::ForwardingStrategy::TrySendOutInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::Face> outFace, ns3::Ptr<ns3::ndn::Interest const> interest, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('TrySendOutInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::Face >', 'outFace'), param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    ## ndn-forwarding-strategy.h (module 'ndnSIM'): void ns3::ndn::ForwardingStrategy::WillSatisfyPendingInterest(ns3::Ptr<ns3::ndn::Face> inFace, ns3::Ptr<ns3::ndn::pit::Entry> pitEntry) [member function]
    cls.add_method('WillSatisfyPendingInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'inFace'), param('ns3::Ptr< ns3::ndn::pit::Entry >', 'pitEntry')], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NdnGlobalRoutingHelper_methods(root_module, cls):
    ## ndn-global-routing-helper.h (module 'ndnSIM'): ns3::ndn::GlobalRoutingHelper::GlobalRoutingHelper() [constructor]
    cls.add_constructor([])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): ns3::ndn::GlobalRoutingHelper::GlobalRoutingHelper(ns3::ndn::GlobalRoutingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::GlobalRoutingHelper const &', 'arg0')])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::AddOrigin(std::string const & prefix, ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('AddOrigin', 
                   'void', 
                   [param('std::string const &', 'prefix'), param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::AddOrigin(std::string const & prefix, std::string const & nodeName) [member function]
    cls.add_method('AddOrigin', 
                   'void', 
                   [param('std::string const &', 'prefix'), param('std::string const &', 'nodeName')])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::AddOrigins(std::string const & prefix, ns3::NodeContainer const & nodes) [member function]
    cls.add_method('AddOrigins', 
                   'void', 
                   [param('std::string const &', 'prefix'), param('ns3::NodeContainer const &', 'nodes')])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::AddOriginsForAll() [member function]
    cls.add_method('AddOriginsForAll', 
                   'void', 
                   [])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): static void ns3::ndn::GlobalRoutingHelper::CalculateAllPossibleRoutes(bool invalidatedRoutes=true) [member function]
    cls.add_method('CalculateAllPossibleRoutes', 
                   'void', 
                   [param('bool', 'invalidatedRoutes', default_value='true')], 
                   is_static=True)
    ## ndn-global-routing-helper.h (module 'ndnSIM'): static void ns3::ndn::GlobalRoutingHelper::CalculateRoutes(bool invalidatedRoutes=true) [member function]
    cls.add_method('CalculateRoutes', 
                   'void', 
                   [param('bool', 'invalidatedRoutes', default_value='true')], 
                   is_static=True)
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::Install(ns3::NodeContainer const & nodes) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer const &', 'nodes')])
    ## ndn-global-routing-helper.h (module 'ndnSIM'): void ns3::ndn::GlobalRoutingHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [])
    return

def register_Ns3NdnHeaderHelper_methods(root_module, cls):
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::HeaderHelper::HeaderHelper() [constructor]
    cls.add_constructor([])
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::HeaderHelper::HeaderHelper(ns3::ndn::HeaderHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::HeaderHelper const &', 'arg0')])
    ## ndn-header-helper.h (module 'ndnSIM'): static ns3::ndn::HeaderHelper::Type ns3::ndn::HeaderHelper::GetNdnHeaderType(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('GetNdnHeaderType', 
                   'ns3::ndn::HeaderHelper::Type', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_static=True)
    return

def register_Ns3NdnInterest_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::Interest::Interest(ns3::Ptr<ns3::Packet> payload=ns3::Create( )) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Packet >', 'payload', default_value='ns3::Create( )')])
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::Interest::Interest(ns3::ndn::Interest const & interest) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Interest const &', 'interest')])
    ## ndn-interest.h (module 'ndnSIM'): ns3::Ptr<const ns3::ndn::Exclude> ns3::ndn::Interest::GetExclude() const [member function]
    cls.add_method('GetExclude', 
                   'ns3::Ptr< ns3::ndn::Exclude const >', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): ns3::Time ns3::ndn::Interest::GetInterestLifetime() const [member function]
    cls.add_method('GetInterestLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): uint8_t ns3::ndn::Interest::GetNack() const [member function]
    cls.add_method('GetNack', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::Name const & ns3::ndn::Interest::GetName() const [member function]
    cls.add_method('GetName', 
                   'ns3::ndn::Name const &', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Name const> ns3::ndn::Interest::GetNamePtr() const [member function]
    cls.add_method('GetNamePtr', 
                   'ns3::Ptr< ns3::ndn::Name const >', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): uint32_t ns3::ndn::Interest::GetNonce() const [member function]
    cls.add_method('GetNonce', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): ns3::Ptr<ns3::Packet const> ns3::ndn::Interest::GetPayload() const [member function]
    cls.add_method('GetPayload', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): int8_t ns3::ndn::Interest::GetScope() const [member function]
    cls.add_method('GetScope', 
                   'int8_t', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): ns3::Ptr<ns3::Packet const> ns3::ndn::Interest::GetWire() const [member function]
    cls.add_method('GetWire', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetExclude(ns3::Ptr<ns3::ndn::Exclude> exclude) [member function]
    cls.add_method('SetExclude', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Exclude >', 'exclude')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetInterestLifetime(ns3::Time time) [member function]
    cls.add_method('SetInterestLifetime', 
                   'void', 
                   [param('ns3::Time', 'time')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetNack(uint8_t nackType) [member function]
    cls.add_method('SetNack', 
                   'void', 
                   [param('uint8_t', 'nackType')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetName(ns3::Ptr<ns3::ndn::Name> name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Name >', 'name')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetName(ns3::ndn::Name const & name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('ns3::ndn::Name const &', 'name')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetNonce(uint32_t nonce) [member function]
    cls.add_method('SetNonce', 
                   'void', 
                   [param('uint32_t', 'nonce')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetPayload(ns3::Ptr<ns3::Packet> payload) [member function]
    cls.add_method('SetPayload', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'payload')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetScope(int8_t scope) [member function]
    cls.add_method('SetScope', 
                   'void', 
                   [param('int8_t', 'scope')])
    ## ndn-interest.h (module 'ndnSIM'): void ns3::ndn::Interest::SetWire(ns3::Ptr<ns3::Packet const> packet) const [member function]
    cls.add_method('SetWire', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_const=True)
    return

def register_Ns3NdnInterestException_methods(root_module, cls):
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::InterestException::InterestException() [constructor]
    cls.add_constructor([])
    ## ndn-interest.h (module 'ndnSIM'): ns3::ndn::InterestException::InterestException(ns3::ndn::InterestException const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::InterestException const &', 'arg0')])
    return

def register_Ns3NdnIpFacesHelper_methods(root_module, cls):
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): ns3::ndn::IpFacesHelper::IpFacesHelper() [constructor]
    cls.add_constructor([])
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): ns3::ndn::IpFacesHelper::IpFacesHelper(ns3::ndn::IpFacesHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::IpFacesHelper const &', 'arg0')])
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): static void ns3::ndn::IpFacesHelper::CreateTcpFace(ns3::Time const & when, ns3::Ptr<ns3::Node> node, ns3::Ipv4Address address, std::string const & prefix, int16_t metric=1) [member function]
    cls.add_method('CreateTcpFace', 
                   'void', 
                   [param('ns3::Time const &', 'when'), param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ipv4Address', 'address'), param('std::string const &', 'prefix'), param('int16_t', 'metric', default_value='1')], 
                   is_static=True)
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): static void ns3::ndn::IpFacesHelper::CreateUdpFace(ns3::Time const & when, ns3::Ptr<ns3::Node> node, ns3::Ipv4Address address, std::string const & prefix, int16_t metric=1) [member function]
    cls.add_method('CreateUdpFace', 
                   'void', 
                   [param('ns3::Time const &', 'when'), param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ipv4Address', 'address'), param('std::string const &', 'prefix'), param('int16_t', 'metric', default_value='1')], 
                   is_static=True)
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): static void ns3::ndn::IpFacesHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_static=True)
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): static void ns3::ndn::IpFacesHelper::Install(ns3::NodeContainer const & nodes) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer const &', 'nodes')], 
                   is_static=True)
    ## ndn-ip-faces-helper.h (module 'ndnSIM'): static void ns3::ndn::IpFacesHelper::InstallAll() [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3NdnL3Protocol_methods(root_module, cls):
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::ndn::L3Protocol::ETHERNET_FRAME_TYPE [variable]
    cls.add_static_attribute('ETHERNET_FRAME_TYPE', 'uint16_t const', is_const=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::ndn::L3Protocol::IP_STACK_PORT [variable]
    cls.add_static_attribute('IP_STACK_PORT', 'uint16_t const', is_const=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::L3Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::ndn::L3Protocol::L3Protocol() [constructor]
    cls.add_constructor([])
    ## ndn-l3-protocol.h (module 'ndnSIM'): uint32_t ns3::ndn::L3Protocol::AddFace(ns3::Ptr<ns3::ndn::Face> const & face) [member function]
    cls.add_method('AddFace', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::ndn::Face > const &', 'face')], 
                   is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): uint32_t ns3::ndn::L3Protocol::GetNFaces() const [member function]
    cls.add_method('GetNFaces', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Face> ns3::ndn::L3Protocol::GetFace(uint32_t face) const [member function]
    cls.add_method('GetFace', 
                   'ns3::Ptr< ns3::ndn::Face >', 
                   [param('uint32_t', 'face')], 
                   is_const=True, is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Face> ns3::ndn::L3Protocol::GetFaceById(uint32_t face) const [member function]
    cls.add_method('GetFaceById', 
                   'ns3::Ptr< ns3::ndn::Face >', 
                   [param('uint32_t', 'face')], 
                   is_const=True, is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): void ns3::ndn::L3Protocol::RemoveFace(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('RemoveFace', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Face> ns3::ndn::L3Protocol::GetFaceByNetDevice(ns3::Ptr<ns3::NetDevice> netDevice) const [member function]
    cls.add_method('GetFaceByNetDevice', 
                   'ns3::Ptr< ns3::ndn::Face >', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'netDevice')], 
                   is_const=True, is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): void ns3::ndn::L3Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-protocol.h (module 'ndnSIM'): void ns3::ndn::L3Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NdnL3Tracer_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-l3-tracer.h (module 'ndnSIM'): ns3::ndn::L3Tracer::L3Tracer(ns3::ndn::L3Tracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::L3Tracer const &', 'arg0')])
    ## ndn-l3-tracer.h (module 'ndnSIM'): ns3::ndn::L3Tracer::L3Tracer(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-l3-tracer.h (module 'ndnSIM'): ns3::ndn::L3Tracer::L3Tracer(std::string const & node) [constructor]
    cls.add_constructor([param('std::string const &', 'node')])
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::Connect() [member function]
    cls.add_method('Connect', 
                   'void', 
                   [], 
                   visibility='protected')
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::DropData(ns3::Ptr<ns3::ndn::Data const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::DropInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::DropNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::InData(ns3::Ptr<ns3::ndn::Data const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::InInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::InNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::OutData(ns3::Ptr<ns3::ndn::Data const> arg0, bool fromCache, ns3::Ptr<const ns3::ndn::Face> arg2) [member function]
    cls.add_method('OutData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('bool', 'fromCache'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg2')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::OutInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('OutInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::OutNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('OutNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::SatisfiedInterests(ns3::Ptr<const ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('SatisfiedInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry const >', 'arg0')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## ndn-l3-tracer.h (module 'ndnSIM'): void ns3::ndn::L3Tracer::TimedOutInterests(ns3::Ptr<const ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('TimedOutInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry const >', 'arg0')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    return

def register_Ns3NdnLimits_methods(root_module, cls):
    ## ndn-limits.h (module 'ndnSIM'): ns3::ndn::Limits::Limits(ns3::ndn::Limits const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Limits const &', 'arg0')])
    ## ndn-limits.h (module 'ndnSIM'): ns3::ndn::Limits::Limits() [constructor]
    cls.add_constructor([])
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::BorrowLimit() [member function]
    cls.add_method('BorrowLimit', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): double ns3::ndn::Limits::GetCurrentLimit() const [member function]
    cls.add_method('GetCurrentLimit', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): double ns3::ndn::Limits::GetCurrentLimitRate() const [member function]
    cls.add_method('GetCurrentLimitRate', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): double ns3::ndn::Limits::GetLinkDelay() const [member function]
    cls.add_method('GetLinkDelay', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): double ns3::ndn::Limits::GetMaxDelay() const [member function]
    cls.add_method('GetMaxDelay', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): double ns3::ndn::Limits::GetMaxLimit() const [member function]
    cls.add_method('GetMaxLimit', 
                   'double', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): double ns3::ndn::Limits::GetMaxRate() const [member function]
    cls.add_method('GetMaxRate', 
                   'double', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::Limits::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-limits.h (module 'ndnSIM'): bool ns3::ndn::Limits::IsBelowLimit() [member function]
    cls.add_method('IsBelowLimit', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): bool ns3::ndn::Limits::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::RegisterAvailableSlotCallback(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('RegisterAvailableSlotCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'handler')])
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::ReturnLimit() [member function]
    cls.add_method('ReturnLimit', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::SetLimits(double rate, double delay) [member function]
    cls.add_method('SetLimits', 
                   'void', 
                   [param('double', 'rate'), param('double', 'delay')], 
                   is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::SetLinkDelay(double delay) [member function]
    cls.add_method('SetLinkDelay', 
                   'void', 
                   [param('double', 'delay')], 
                   is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::UpdateCurrentLimit(double limit) [member function]
    cls.add_method('UpdateCurrentLimit', 
                   'void', 
                   [param('double', 'limit')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-limits.h (module 'ndnSIM'): void ns3::ndn::Limits::FireAvailableSlotCallback() [member function]
    cls.add_method('FireAvailableSlotCallback', 
                   'void', 
                   [], 
                   visibility='protected')
    return

def register_Ns3NdnLinkControlHelper_methods(root_module, cls):
    ## ndn-link-control-helper.h (module 'ndnSIM'): ns3::ndn::LinkControlHelper::LinkControlHelper() [constructor]
    cls.add_constructor([])
    ## ndn-link-control-helper.h (module 'ndnSIM'): ns3::ndn::LinkControlHelper::LinkControlHelper(ns3::ndn::LinkControlHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::LinkControlHelper const &', 'arg0')])
    ## ndn-link-control-helper.h (module 'ndnSIM'): static void ns3::ndn::LinkControlHelper::FailLink(ns3::Ptr<ns3::Node> node1, ns3::Ptr<ns3::Node> node2) [member function]
    cls.add_method('FailLink', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node1'), param('ns3::Ptr< ns3::Node >', 'node2')], 
                   is_static=True)
    ## ndn-link-control-helper.h (module 'ndnSIM'): static void ns3::ndn::LinkControlHelper::FailLinkByName(std::string const & node1, std::string const & node2) [member function]
    cls.add_method('FailLinkByName', 
                   'void', 
                   [param('std::string const &', 'node1'), param('std::string const &', 'node2')], 
                   is_static=True)
    ## ndn-link-control-helper.h (module 'ndnSIM'): static void ns3::ndn::LinkControlHelper::UpLink(ns3::Ptr<ns3::Node> node1, ns3::Ptr<ns3::Node> node2) [member function]
    cls.add_method('UpLink', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node1'), param('ns3::Ptr< ns3::Node >', 'node2')], 
                   is_static=True)
    ## ndn-link-control-helper.h (module 'ndnSIM'): static void ns3::ndn::LinkControlHelper::UpLinkByName(std::string const & node1, std::string const & node2) [member function]
    cls.add_method('UpLinkByName', 
                   'void', 
                   [param('std::string const &', 'node1'), param('std::string const &', 'node2')], 
                   is_static=True)
    return

def register_Ns3NdnName_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_numeric_operator('+', root_module['ns3::ndn::Name'], root_module['ns3::ndn::Name'], param('ns3::ndn::Name const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name() [constructor]
    cls.add_constructor([])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name(ns3::ndn::Name const & other) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Name const &', 'other')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name(std::string const & url) [constructor]
    cls.add_constructor([param('std::string const &', 'url')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::Name(__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > begin, __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > end) [constructor]
    cls.add_constructor([param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'begin'), param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'end')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(ns3::ndn::name::Component const & comp) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('ns3::ndn::name::Component const &', 'comp')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > begin, __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > end) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'begin'), param('__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 'end')], 
                   template_parameters=['__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > >'])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(ns3::ndn::Name const & comp) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('ns3::ndn::Name const &', 'comp')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(std::string const & compStr) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('std::string const &', 'compStr')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::append(void const * buf, size_t size) [member function]
    cls.add_method('append', 
                   'ns3::ndn::Name &', 
                   [param('void const *', 'buf'), param('size_t', 'size')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendBlkId(uint64_t blkid) [member function]
    cls.add_method('appendBlkId', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'blkid')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendBySwap(ns3::ndn::name::Component & comp) [member function]
    cls.add_method('appendBySwap', 
                   'ns3::ndn::Name &', 
                   [param('ns3::ndn::name::Component &', 'comp')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendControlNum(uint64_t control) [member function]
    cls.add_method('appendControlNum', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'control')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendNumber(uint64_t number) [member function]
    cls.add_method('appendNumber', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'number')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendNumberWithMarker(uint64_t number, unsigned char marker) [member function]
    cls.add_method('appendNumberWithMarker', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'number'), param('unsigned char', 'marker')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendSeqNum(uint64_t seqno) [member function]
    cls.add_method('appendSeqNum', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'seqno')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name & ns3::ndn::Name::appendVersion(uint64_t version=ns3::ndn::Name::nversion) [member function]
    cls.add_method('appendVersion', 
                   'ns3::ndn::Name &', 
                   [param('uint64_t', 'version', default_value='ns3::ndn::Name::nversion')])
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::begin() const [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::begin() [member function]
    cls.add_method('begin', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > >', 
                   [])
    ## name.h (module 'ndnSIM'): int ns3::ndn::Name::compare(ns3::ndn::Name const & name) const [member function]
    cls.add_method('compare', 
                   'int', 
                   [param('ns3::ndn::Name const &', 'name')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::end() const [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): __gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > ns3::ndn::Name::end() [member function]
    cls.add_method('end', 
                   '__gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > >', 
                   [])
    ## name.h (module 'ndnSIM'): ns3::ndn::name::Component const & ns3::ndn::Name::get(int index) const [member function]
    cls.add_method('get', 
                   'ns3::ndn::name::Component const &', 
                   [param('int', 'index')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::Name::get(int index) [member function]
    cls.add_method('get', 
                   'ns3::ndn::name::Component &', 
                   [param('int', 'index')])
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::Name::getPostfix(size_t len, size_t skip=0) const [member function]
    cls.add_method('getPostfix', 
                   'ns3::ndn::Name', 
                   [param('size_t', 'len'), param('size_t', 'skip', default_value='0')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::Name::getPrefix(size_t len, size_t skip=0) const [member function]
    cls.add_method('getPrefix', 
                   'ns3::ndn::Name', 
                   [param('size_t', 'len'), param('size_t', 'skip', default_value='0')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::Name::getSubName(size_t pos=0, size_t len=ns3::ndn::Name::npos) const [member function]
    cls.add_method('getSubName', 
                   'ns3::ndn::Name', 
                   [param('size_t', 'pos', default_value='0'), param('size_t', 'len', default_value='ns3::ndn::Name::npos')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rbegin() const [member function]
    cls.add_method('rbegin', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rbegin() [member function]
    cls.add_method('rbegin', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > > >', 
                   [])
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component const*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rend() const [member function]
    cls.add_method('rend', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component const *, std::vector< ns3::ndn::name::Component > > >', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::reverse_iterator<__gnu_cxx::__normal_iterator<ns3::ndn::name::Component*, std::vector<ns3::ndn::name::Component, std::allocator<ns3::ndn::name::Component> > > > ns3::ndn::Name::rend() [member function]
    cls.add_method('rend', 
                   'std::reverse_iterator< __gnu_cxx::__normal_iterator< ns3::ndn::name::Component *, std::vector< ns3::ndn::name::Component > > >', 
                   [])
    ## name.h (module 'ndnSIM'): size_t ns3::ndn::Name::size() const [member function]
    cls.add_method('size', 
                   'size_t', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::string ns3::ndn::Name::toUri() const [member function]
    cls.add_method('toUri', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): void ns3::ndn::Name::toUri(std::ostream & os) const [member function]
    cls.add_method('toUri', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::npos [variable]
    cls.add_static_attribute('npos', 'size_t const', is_const=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name::nversion [variable]
    cls.add_static_attribute('nversion', 'uint64_t const', is_const=True)
    return

def register_Ns3NdnNameChecker_methods(root_module, cls):
    ## name.h (module 'ndnSIM'): ns3::ndn::NameChecker::NameChecker() [constructor]
    cls.add_constructor([])
    ## name.h (module 'ndnSIM'): ns3::ndn::NameChecker::NameChecker(ns3::ndn::NameChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::NameChecker const &', 'arg0')])
    return

def register_Ns3NdnNameValue_methods(root_module, cls):
    ## name.h (module 'ndnSIM'): ns3::ndn::NameValue::NameValue() [constructor]
    cls.add_constructor([])
    ## name.h (module 'ndnSIM'): ns3::ndn::NameValue::NameValue(ns3::ndn::NameValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::NameValue const &', 'arg0')])
    ## name.h (module 'ndnSIM'): ns3::ndn::NameValue::NameValue(ns3::ndn::Name const & value) [constructor]
    cls.add_constructor([param('ns3::ndn::Name const &', 'value')])
    ## name.h (module 'ndnSIM'): ns3::Ptr<ns3::AttributeValue> ns3::ndn::NameValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## name.h (module 'ndnSIM'): bool ns3::ndn::NameValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## name.h (module 'ndnSIM'): ns3::ndn::Name ns3::ndn::NameValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::ndn::Name', 
                   [], 
                   is_const=True)
    ## name.h (module 'ndnSIM'): std::string ns3::ndn::NameValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## name.h (module 'ndnSIM'): void ns3::ndn::NameValue::Set(ns3::ndn::Name const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::ndn::Name const &', 'value')])
    return

def register_Ns3NdnNetDeviceFace_methods(root_module, cls):
    ## ndn-net-device-face.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::NetDeviceFace::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-net-device-face.h (module 'ndnSIM'): ns3::ndn::NetDeviceFace::NetDeviceFace(ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::NetDevice> const & netDevice) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::NetDevice > const &', 'netDevice')])
    ## ndn-net-device-face.h (module 'ndnSIM'): void ns3::ndn::NetDeviceFace::RegisterProtocolHandlers(ns3::Callback<void, ns3::Ptr<ns3::ndn::Face>, ns3::Ptr<ns3::ndn::Interest>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> const & interestHandler, ns3::Callback<void, ns3::Ptr<ns3::ndn::Face>, ns3::Ptr<ns3::ndn::Data>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> const & dataHandler) [member function]
    cls.add_method('RegisterProtocolHandlers', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Face >, ns3::Ptr< ns3::ndn::Interest >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty > const &', 'interestHandler'), param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Face >, ns3::Ptr< ns3::ndn::Data >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty > const &', 'dataHandler')], 
                   is_virtual=True)
    ## ndn-net-device-face.h (module 'ndnSIM'): void ns3::ndn::NetDeviceFace::UnRegisterProtocolHandlers() [member function]
    cls.add_method('UnRegisterProtocolHandlers', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-net-device-face.h (module 'ndnSIM'): std::ostream & ns3::ndn::NetDeviceFace::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'std::ostream &', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ndn-net-device-face.h (module 'ndnSIM'): ns3::Ptr<ns3::NetDevice> ns3::ndn::NetDeviceFace::GetNetDevice() const [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ndn-net-device-face.h (module 'ndnSIM'): bool ns3::ndn::NetDeviceFace::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NdnPit_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-pit.h (module 'ndnSIM'): ns3::ndn::Pit::Pit(ns3::ndn::Pit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Pit const &', 'arg0')])
    ## ndn-pit.h (module 'ndnSIM'): ns3::ndn::Pit::Pit() [constructor]
    cls.add_constructor([])
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::Begin() [member function]
    cls.add_method('Begin', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::Create(ns3::Ptr<ns3::ndn::Interest const> header) [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'header')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::End() [member function]
    cls.add_method('End', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::Find(ns3::ndn::Name const & prefix) [member function]
    cls.add_method('Find', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [param('ns3::ndn::Name const &', 'prefix')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Time const & ns3::ndn::Pit::GetMaxPitEntryLifetime() const [member function]
    cls.add_method('GetMaxPitEntryLifetime', 
                   'ns3::Time const &', 
                   [], 
                   is_const=True)
    ## ndn-pit.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Pit> ns3::ndn::Pit::GetPit(ns3::Ptr<ns3::Object> node) [member function]
    cls.add_method('GetPit', 
                   'ns3::Ptr< ns3::ndn::Pit >', 
                   [param('ns3::Ptr< ns3::Object >', 'node')], 
                   is_static=True)
    ## ndn-pit.h (module 'ndnSIM'): uint32_t ns3::ndn::Pit::GetSize() const [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::Pit::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::Lookup(ns3::ndn::Data const & header) [member function]
    cls.add_method('Lookup', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [param('ns3::ndn::Data const &', 'header')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::Lookup(ns3::ndn::Interest const & header) [member function]
    cls.add_method('Lookup', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [param('ns3::ndn::Interest const &', 'header')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): void ns3::ndn::Pit::MarkErased(ns3::Ptr<ns3::ndn::pit::Entry> entry) [member function]
    cls.add_method('MarkErased', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry >', 'entry')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::pit::Entry> ns3::ndn::Pit::Next(ns3::Ptr<ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::ndn::pit::Entry >', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry >', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): void ns3::ndn::Pit::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-pit.h (module 'ndnSIM'): void ns3::ndn::Pit::SetMaxPitEntryLifetime(ns3::Time const & maxLifetime) [member function]
    cls.add_method('SetMaxPitEntryLifetime', 
                   'void', 
                   [param('ns3::Time const &', 'maxLifetime')])
    return

def register_Ns3NdnRttEstimator_methods(root_module, cls):
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttEstimator::RttEstimator() [constructor]
    cls.add_constructor([])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttEstimator::RttEstimator(ns3::ndn::RttEstimator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::RttEstimator const &', 'arg0')])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::Time ns3::ndn::RttEstimator::AckSeq(ns3::SequenceNumber32 ackSeq) [member function]
    cls.add_method('AckSeq', 
                   'ns3::Time', 
                   [param('ns3::SequenceNumber32', 'ackSeq')], 
                   is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::ClearSent() [member function]
    cls.add_method('ClearSent', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::RttEstimator> ns3::ndn::RttEstimator::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::ndn::RttEstimator >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::Time ns3::ndn::RttEstimator::GetCurrentEstimate() const [member function]
    cls.add_method('GetCurrentEstimate', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::TypeId ns3::ndn::RttEstimator::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::Time ns3::ndn::RttEstimator::GetMaxRto() const [member function]
    cls.add_method('GetMaxRto', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::Time ns3::ndn::RttEstimator::GetMinRto() const [member function]
    cls.add_method('GetMinRto', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::RttEstimator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::IncreaseMultiplier() [member function]
    cls.add_method('IncreaseMultiplier', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::Measurement(ns3::Time t) [member function]
    cls.add_method('Measurement', 
                   'void', 
                   [param('ns3::Time', 't')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::ResetMultiplier() [member function]
    cls.add_method('ResetMultiplier', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::Time ns3::ndn::RttEstimator::RetransmitTimeout() [member function]
    cls.add_method('RetransmitTimeout', 
                   'ns3::Time', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::SentSeq(ns3::SequenceNumber32 seq, uint32_t size) [member function]
    cls.add_method('SentSeq', 
                   'void', 
                   [param('ns3::SequenceNumber32', 'seq'), param('uint32_t', 'size')], 
                   is_virtual=True)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::SetCurrentEstimate(ns3::Time estimate) [member function]
    cls.add_method('SetCurrentEstimate', 
                   'void', 
                   [param('ns3::Time', 'estimate')])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::SetMaxRto(ns3::Time maxRto) [member function]
    cls.add_method('SetMaxRto', 
                   'void', 
                   [param('ns3::Time', 'maxRto')])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): void ns3::ndn::RttEstimator::SetMinRto(ns3::Time minRto) [member function]
    cls.add_method('SetMinRto', 
                   'void', 
                   [param('ns3::Time', 'minRto')])
    return

def register_Ns3NdnRttHistory_methods(root_module, cls):
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory::RttHistory(ns3::SequenceNumber32 s, uint32_t c, ns3::Time t) [constructor]
    cls.add_constructor([param('ns3::SequenceNumber32', 's'), param('uint32_t', 'c'), param('ns3::Time', 't')])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory::RttHistory(ns3::ndn::RttHistory const & h) [copy constructor]
    cls.add_constructor([param('ns3::ndn::RttHistory const &', 'h')])
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory::count [variable]
    cls.add_instance_attribute('count', 'uint32_t', is_const=False)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory::retx [variable]
    cls.add_instance_attribute('retx', 'bool', is_const=False)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory::seq [variable]
    cls.add_instance_attribute('seq', 'ns3::SequenceNumber32', is_const=False)
    ## ndn-rtt-estimator.h (module 'ndnSIM'): ns3::ndn::RttHistory::time [variable]
    cls.add_instance_attribute('time', 'ns3::Time', is_const=False)
    return

def register_Ns3NdnStackHelper_methods(root_module, cls):
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::ndn::StackHelper::StackHelper() [constructor]
    cls.add_constructor([])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetStackAttributes(std::string const & attr1="", std::string const & value1="", std::string const & attr2="", std::string const & value2="", std::string const & attr3="", std::string const & value3="", std::string const & attr4="", std::string const & value4="") [member function]
    cls.add_method('SetStackAttributes', 
                   'void', 
                   [param('std::string const &', 'attr1', default_value='""'), param('std::string const &', 'value1', default_value='""'), param('std::string const &', 'attr2', default_value='""'), param('std::string const &', 'value2', default_value='""'), param('std::string const &', 'attr3', default_value='""'), param('std::string const &', 'value3', default_value='""'), param('std::string const &', 'attr4', default_value='""'), param('std::string const &', 'value4', default_value='""')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetForwardingStrategy(std::string const & forwardingStrategyClass, std::string const & attr1="", std::string const & value1="", std::string const & attr2="", std::string const & value2="", std::string const & attr3="", std::string const & value3="", std::string const & attr4="", std::string const & value4="") [member function]
    cls.add_method('SetForwardingStrategy', 
                   'void', 
                   [param('std::string const &', 'forwardingStrategyClass'), param('std::string const &', 'attr1', default_value='""'), param('std::string const &', 'value1', default_value='""'), param('std::string const &', 'attr2', default_value='""'), param('std::string const &', 'value2', default_value='""'), param('std::string const &', 'attr3', default_value='""'), param('std::string const &', 'value3', default_value='""'), param('std::string const &', 'attr4', default_value='""'), param('std::string const &', 'value4', default_value='""')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetContentStore(std::string const & contentStoreClass, std::string const & attr1="", std::string const & value1="", std::string const & attr2="", std::string const & value2="", std::string const & attr3="", std::string const & value3="", std::string const & attr4="", std::string const & value4="") [member function]
    cls.add_method('SetContentStore', 
                   'void', 
                   [param('std::string const &', 'contentStoreClass'), param('std::string const &', 'attr1', default_value='""'), param('std::string const &', 'value1', default_value='""'), param('std::string const &', 'attr2', default_value='""'), param('std::string const &', 'value2', default_value='""'), param('std::string const &', 'attr3', default_value='""'), param('std::string const &', 'value3', default_value='""'), param('std::string const &', 'attr4', default_value='""'), param('std::string const &', 'value4', default_value='""')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetPit(std::string const & pitClass, std::string const & attr1="", std::string const & value1="", std::string const & attr2="", std::string const & value2="", std::string const & attr3="", std::string const & value3="", std::string const & attr4="", std::string const & value4="") [member function]
    cls.add_method('SetPit', 
                   'void', 
                   [param('std::string const &', 'pitClass'), param('std::string const &', 'attr1', default_value='""'), param('std::string const &', 'value1', default_value='""'), param('std::string const &', 'attr2', default_value='""'), param('std::string const &', 'value2', default_value='""'), param('std::string const &', 'attr3', default_value='""'), param('std::string const &', 'value3', default_value='""'), param('std::string const &', 'attr4', default_value='""'), param('std::string const &', 'value4', default_value='""')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetFib(std::string const & fibClass, std::string const & attr1="", std::string const & value1="", std::string const & attr2="", std::string const & value2="", std::string const & attr3="", std::string const & value3="", std::string const & attr4="", std::string const & value4="") [member function]
    cls.add_method('SetFib', 
                   'void', 
                   [param('std::string const &', 'fibClass'), param('std::string const &', 'attr1', default_value='""'), param('std::string const &', 'value1', default_value='""'), param('std::string const &', 'attr2', default_value='""'), param('std::string const &', 'value2', default_value='""'), param('std::string const &', 'attr3', default_value='""'), param('std::string const &', 'value3', default_value='""'), param('std::string const &', 'attr4', default_value='""'), param('std::string const &', 'value4', default_value='""')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::AddNetDeviceFaceCreateCallback(ns3::TypeId netDeviceType, ns3::Callback<ns3::Ptr<ns3::ndn::NetDeviceFace>,ns3::Ptr<ns3::Node>,ns3::Ptr<ns3::ndn::L3Protocol>,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('AddNetDeviceFaceCreateCallback', 
                   'void', 
                   [param('ns3::TypeId', 'netDeviceType'), param('ns3::Callback< ns3::Ptr< ns3::ndn::NetDeviceFace >, ns3::Ptr< ns3::Node >, ns3::Ptr< ns3::ndn::L3Protocol >, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::UpdateNetDeviceFaceCreateCallback(ns3::TypeId netDeviceType, ns3::Callback<ns3::Ptr<ns3::ndn::NetDeviceFace>,ns3::Ptr<ns3::Node>,ns3::Ptr<ns3::ndn::L3Protocol>,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('UpdateNetDeviceFaceCreateCallback', 
                   'void', 
                   [param('ns3::TypeId', 'netDeviceType'), param('ns3::Callback< ns3::Ptr< ns3::ndn::NetDeviceFace >, ns3::Ptr< ns3::Node >, ns3::Ptr< ns3::ndn::L3Protocol >, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::RemoveNetDeviceFaceCreateCallback(ns3::TypeId netDeviceType, ns3::Callback<ns3::Ptr<ns3::ndn::NetDeviceFace>,ns3::Ptr<ns3::Node>,ns3::Ptr<ns3::ndn::L3Protocol>,ns3::Ptr<ns3::NetDevice>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('RemoveNetDeviceFaceCreateCallback', 
                   'void', 
                   [param('ns3::TypeId', 'netDeviceType'), param('ns3::Callback< ns3::Ptr< ns3::ndn::NetDeviceFace >, ns3::Ptr< ns3::Node >, ns3::Ptr< ns3::ndn::L3Protocol >, ns3::Ptr< ns3::NetDevice >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::EnableLimits(bool enable=true, ns3::Time avgRtt=ns3::Seconds( ), uint32_t avgData=1100, uint32_t avgInterest=40) [member function]
    cls.add_method('EnableLimits', 
                   'void', 
                   [param('bool', 'enable', default_value='true'), param('ns3::Time', 'avgRtt', default_value='ns3::Seconds(0)'), param('uint32_t', 'avgData', default_value='1100'), param('uint32_t', 'avgInterest', default_value='40')])
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::Install(std::string const & nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [param('std::string const &', 'nodeName')], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::Install(ns3::NodeContainer const & c) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [param('ns3::NodeContainer const &', 'c')], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::FaceContainer> ns3::ndn::StackHelper::InstallAll() const [member function]
    cls.add_method('InstallAll', 
                   'ns3::Ptr< ns3::ndn::FaceContainer >', 
                   [], 
                   is_const=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): static void ns3::ndn::StackHelper::AddRoute(std::string const & nodeName, std::string const & prefix, uint32_t faceId, int32_t metric) [member function]
    cls.add_method('AddRoute', 
                   'void', 
                   [param('std::string const &', 'nodeName'), param('std::string const &', 'prefix'), param('uint32_t', 'faceId'), param('int32_t', 'metric')], 
                   is_static=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): static void ns3::ndn::StackHelper::AddRoute(ns3::Ptr<ns3::Node> node, std::string const & prefix, uint32_t faceId, int32_t metric) [member function]
    cls.add_method('AddRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'prefix'), param('uint32_t', 'faceId'), param('int32_t', 'metric')], 
                   is_static=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): static void ns3::ndn::StackHelper::AddRoute(ns3::Ptr<ns3::Node> node, std::string const & prefix, ns3::Ptr<ns3::ndn::Face> face, int32_t metric) [member function]
    cls.add_method('AddRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'prefix'), param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('int32_t', 'metric')], 
                   is_static=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): static void ns3::ndn::StackHelper::AddRoute(ns3::Ptr<ns3::Node> node, std::string const & prefix, ns3::Ptr<ns3::Node> otherNode, int32_t metric) [member function]
    cls.add_method('AddRoute', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'prefix'), param('ns3::Ptr< ns3::Node >', 'otherNode'), param('int32_t', 'metric')], 
                   is_static=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): static void ns3::ndn::StackHelper::AddRoute(std::string const & nodeName, std::string const & prefix, std::string const & otherNodeName, int32_t metric) [member function]
    cls.add_method('AddRoute', 
                   'void', 
                   [param('std::string const &', 'nodeName'), param('std::string const &', 'prefix'), param('std::string const &', 'otherNodeName'), param('int32_t', 'metric')], 
                   is_static=True)
    ## ndn-stack-helper.h (module 'ndnSIM'): void ns3::ndn::StackHelper::SetDefaultRoutes(bool needSet) [member function]
    cls.add_method('SetDefaultRoutes', 
                   'void', 
                   [param('bool', 'needSet')])
    return

def register_Ns3NdnUnknownHeaderException_methods(root_module, cls):
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::UnknownHeaderException::UnknownHeaderException() [constructor]
    cls.add_constructor([])
    ## ndn-header-helper.h (module 'ndnSIM'): ns3::ndn::UnknownHeaderException::UnknownHeaderException(ns3::ndn::UnknownHeaderException const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::UnknownHeaderException const &', 'arg0')])
    return

def register_Ns3NdnWire_methods(root_module, cls):
    ## ndn-wire.h (module 'ndnSIM'): ns3::ndn::Wire::Wire() [constructor]
    cls.add_constructor([])
    ## ndn-wire.h (module 'ndnSIM'): ns3::ndn::Wire::Wire(ns3::ndn::Wire const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::Wire const &', 'arg0')])
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::Packet> ns3::ndn::Wire::FromData(ns3::Ptr<ns3::ndn::Data const> data, int8_t wireFormat=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)) [member function]
    cls.add_method('FromData', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'data'), param('int8_t', 'wireFormat', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static std::string ns3::ndn::Wire::FromDataStr(ns3::Ptr<ns3::ndn::Data const> data, int8_t wireFormat=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)) [member function]
    cls.add_method('FromDataStr', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'data'), param('int8_t', 'wireFormat', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::Packet> ns3::ndn::Wire::FromInterest(ns3::Ptr<ns3::ndn::Interest const> interest, int8_t wireFormat=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)) [member function]
    cls.add_method('FromInterest', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('int8_t', 'wireFormat', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static std::string ns3::ndn::Wire::FromInterestStr(ns3::Ptr<ns3::ndn::Interest const> interest, int8_t wireFormat=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)) [member function]
    cls.add_method('FromInterestStr', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest'), param('int8_t', 'wireFormat', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static std::string ns3::ndn::Wire::FromName(ns3::Ptr<ns3::ndn::Name const> name, int8_t wireFormat=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)) [member function]
    cls.add_method('FromName', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::ndn::Name const >', 'name'), param('int8_t', 'wireFormat', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Data> ns3::ndn::Wire::ToData(ns3::Ptr<ns3::Packet> packet, int8_t type=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)) [member function]
    cls.add_method('ToData', 
                   'ns3::Ptr< ns3::ndn::Data >', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('int8_t', 'type', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Data> ns3::ndn::Wire::ToDataStr(std::string const & wire, int8_t type=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)) [member function]
    cls.add_method('ToDataStr', 
                   'ns3::Ptr< ns3::ndn::Data >', 
                   [param('std::string const &', 'wire'), param('int8_t', 'type', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Interest> ns3::ndn::Wire::ToInterest(ns3::Ptr<ns3::Packet> packet, int8_t type=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)) [member function]
    cls.add_method('ToInterest', 
                   'ns3::Ptr< ns3::ndn::Interest >', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('int8_t', 'type', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Interest> ns3::ndn::Wire::ToInterestStr(std::string const & wire, int8_t type=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)) [member function]
    cls.add_method('ToInterestStr', 
                   'ns3::Ptr< ns3::ndn::Interest >', 
                   [param('std::string const &', 'wire'), param('int8_t', 'type', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_AUTODETECT)')], 
                   is_static=True)
    ## ndn-wire.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::Name> ns3::ndn::Wire::ToName(std::string const & wire, int8_t wireFormat=::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)) [member function]
    cls.add_method('ToName', 
                   'ns3::Ptr< ns3::ndn::Name >', 
                   [param('std::string const &', 'wire'), param('int8_t', 'wireFormat', default_value='::int8_t(::ns3::ndn::Wire::WIRE_FORMAT_DEFAULT)')], 
                   is_static=True)
    return

def register_Ns3NdnApiFace_methods(root_module, cls):
    ## ndn-api-face.h (module 'ndnSIM'): ns3::ndn::ApiFace::ApiFace(ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-api-face.h (module 'ndnSIM'): void ns3::ndn::ApiFace::Shutdown() [member function]
    cls.add_method('Shutdown', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-api-face.h (module 'ndnSIM'): void ns3::ndn::ApiFace::ExpressInterest(ns3::Ptr<ns3::ndn::Interest> interest, ns3::Callback<void, ns3::Ptr<ns3::ndn::Interest const>, ns3::Ptr<ns3::ndn::Data const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> onData, ns3::Callback<void, ns3::Ptr<ns3::ndn::Interest const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> onTimeout) [member function]
    cls.add_method('ExpressInterest', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest >', 'interest'), param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Interest const >, ns3::Ptr< ns3::ndn::Data const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'onData'), param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Interest const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'onTimeout')])
    ## ndn-api-face.h (module 'ndnSIM'): void ns3::ndn::ApiFace::SetInterestFilter(ns3::Ptr<ns3::ndn::Name const> prefix, ns3::Callback<void, ns3::Ptr<ns3::ndn::Name const>, ns3::Ptr<ns3::ndn::Interest const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> onInterest) [member function]
    cls.add_method('SetInterestFilter', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Name const >', 'prefix'), param('ns3::Callback< void, ns3::Ptr< ns3::ndn::Name const >, ns3::Ptr< ns3::ndn::Interest const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'onInterest')])
    ## ndn-api-face.h (module 'ndnSIM'): void ns3::ndn::ApiFace::ClearInterestFilter(ns3::Ptr<ns3::ndn::Name const> prefix) [member function]
    cls.add_method('ClearInterestFilter', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Name const >', 'prefix')])
    ## ndn-api-face.h (module 'ndnSIM'): void ns3::ndn::ApiFace::Put(ns3::Ptr<ns3::ndn::Data> data) [member function]
    cls.add_method('Put', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data >', 'data')])
    ## ndn-api-face.h (module 'ndnSIM'): bool ns3::ndn::ApiFace::SendInterest(ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('SendInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   is_virtual=True)
    ## ndn-api-face.h (module 'ndnSIM'): bool ns3::ndn::ApiFace::SendData(ns3::Ptr<ns3::ndn::Data const> data) [member function]
    cls.add_method('SendData', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'data')], 
                   is_virtual=True)
    ## ndn-api-face.h (module 'ndnSIM'): std::ostream & ns3::ndn::ApiFace::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'std::ostream &', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3NdnAppFace_methods(root_module, cls):
    ## ndn-app-face.h (module 'ndnSIM'): static ns3::TypeId ns3::ndn::AppFace::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ndn-app-face.h (module 'ndnSIM'): ns3::ndn::AppFace::AppFace(ns3::Ptr<ns3::ndn::App> app) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ndn::App >', 'app')])
    ## ndn-app-face.h (module 'ndnSIM'): bool ns3::ndn::AppFace::SendInterest(ns3::Ptr<ns3::ndn::Interest const> interest) [member function]
    cls.add_method('SendInterest', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'interest')], 
                   is_virtual=True)
    ## ndn-app-face.h (module 'ndnSIM'): bool ns3::ndn::AppFace::SendData(ns3::Ptr<ns3::ndn::Data const> data) [member function]
    cls.add_method('SendData', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'data')], 
                   is_virtual=True)
    ## ndn-app-face.h (module 'ndnSIM'): std::ostream & ns3::ndn::AppFace::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'std::ostream &', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3NdnL3AggregateTracer_methods(root_module, cls):
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): ns3::ndn::L3AggregateTracer::L3AggregateTracer(ns3::ndn::L3AggregateTracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::L3AggregateTracer const &', 'arg0')])
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): ns3::ndn::L3AggregateTracer::L3AggregateTracer(boost::shared_ptr<std::ostream> os, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): ns3::ndn::L3AggregateTracer::L3AggregateTracer(boost::shared_ptr<std::ostream> os, std::string const & nodeName) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('std::string const &', 'nodeName')])
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3AggregateTracer::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3AggregateTracer::Install(ns3::NodeContainer const & nodes, std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer const &', 'nodes'), param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3AggregateTracer::Install(ns3::Ptr<ns3::Node> node, std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::L3AggregateTracer> ns3::ndn::L3AggregateTracer::Install(ns3::Ptr<ns3::Node> node, boost::shared_ptr<std::ostream> outputStream, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::L3AggregateTracer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('boost::shared_ptr< std::ostream >', 'outputStream'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3AggregateTracer::InstallAll(std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::DropData(ns3::Ptr<ns3::ndn::Data const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::DropInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::DropNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::InData(ns3::Ptr<ns3::ndn::Data const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::InInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::InNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::OutData(ns3::Ptr<ns3::ndn::Data const> arg0, bool fromCache, ns3::Ptr<const ns3::ndn::Face> arg2) [member function]
    cls.add_method('OutData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('bool', 'fromCache'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg2')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::OutInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('OutInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::OutNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('OutNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::PeriodicPrinter() [member function]
    cls.add_method('PeriodicPrinter', 
                   'void', 
                   [], 
                   visibility='protected')
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [], 
                   visibility='protected')
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::SatisfiedInterests(ns3::Ptr<const ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('SatisfiedInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry const >', 'arg0')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::SetAveragingPeriod(ns3::Time const & period) [member function]
    cls.add_method('SetAveragingPeriod', 
                   'void', 
                   [param('ns3::Time const &', 'period')], 
                   visibility='protected')
    ## ndn-l3-aggregate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3AggregateTracer::TimedOutInterests(ns3::Ptr<const ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('TimedOutInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry const >', 'arg0')], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NdnL3RateTracer_methods(root_module, cls):
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): ns3::ndn::L3RateTracer::L3RateTracer(ns3::ndn::L3RateTracer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::L3RateTracer const &', 'arg0')])
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): ns3::ndn::L3RateTracer::L3RateTracer(boost::shared_ptr<std::ostream> os, ns3::Ptr<ns3::Node> node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('ns3::Ptr< ns3::Node >', 'node')])
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): ns3::ndn::L3RateTracer::L3RateTracer(boost::shared_ptr<std::ostream> os, std::string const & node) [constructor]
    cls.add_constructor([param('boost::shared_ptr< std::ostream >', 'os'), param('std::string const &', 'node')])
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3RateTracer::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3RateTracer::Install(ns3::NodeContainer const & nodes, std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::NodeContainer const &', 'nodes'), param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3RateTracer::Install(ns3::Ptr<ns3::Node> node, std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): static ns3::Ptr<ns3::ndn::L3RateTracer> ns3::ndn::L3RateTracer::Install(ns3::Ptr<ns3::Node> node, boost::shared_ptr<std::ostream> outputStream, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::ndn::L3RateTracer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('boost::shared_ptr< std::ostream >', 'outputStream'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): static void ns3::ndn::L3RateTracer::InstallAll(std::string const & file, ns3::Time averagingPeriod=ns3::Seconds( )) [member function]
    cls.add_method('InstallAll', 
                   'void', 
                   [param('std::string const &', 'file'), param('ns3::Time', 'averagingPeriod', default_value='ns3::Seconds(0)')], 
                   is_static=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::PrintHeader(std::ostream & os) const [member function]
    cls.add_method('PrintHeader', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::DropData(ns3::Ptr<ns3::ndn::Data const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::DropInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::DropNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('DropNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::InData(ns3::Ptr<ns3::ndn::Data const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::InInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::InNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('InNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::OutData(ns3::Ptr<ns3::ndn::Data const> arg0, bool fromCache, ns3::Ptr<const ns3::ndn::Face> arg2) [member function]
    cls.add_method('OutData', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Data const >', 'arg0'), param('bool', 'fromCache'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg2')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::OutInterests(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('OutInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::OutNacks(ns3::Ptr<ns3::ndn::Interest const> arg0, ns3::Ptr<const ns3::ndn::Face> arg1) [member function]
    cls.add_method('OutNacks', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Interest const >', 'arg0'), param('ns3::Ptr< ns3::ndn::Face const >', 'arg1')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::SatisfiedInterests(ns3::Ptr<const ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('SatisfiedInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry const >', 'arg0')], 
                   visibility='protected', is_virtual=True)
    ## ndn-l3-rate-tracer.h (module 'ndnSIM'): void ns3::ndn::L3RateTracer::TimedOutInterests(ns3::Ptr<const ns3::ndn::pit::Entry> arg0) [member function]
    cls.add_method('TimedOutInterests', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry const >', 'arg0')], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3NdnCsEntry_methods(root_module, cls):
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::cs::Entry::Entry(ns3::ndn::cs::Entry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::cs::Entry const &', 'arg0')])
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::cs::Entry::Entry(ns3::Ptr<ns3::ndn::ContentStore> cs, ns3::Ptr<ns3::ndn::Data const> data) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ndn::ContentStore >', 'cs'), param('ns3::Ptr< ns3::ndn::Data const >', 'data')])
    ## ndn-content-store.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::ContentStore> ns3::ndn::cs::Entry::GetContentStore() [member function]
    cls.add_method('GetContentStore', 
                   'ns3::Ptr< ns3::ndn::ContentStore >', 
                   [])
    ## ndn-content-store.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Data const> ns3::ndn::cs::Entry::GetData() const [member function]
    cls.add_method('GetData', 
                   'ns3::Ptr< ns3::ndn::Data const >', 
                   [], 
                   is_const=True)
    ## ndn-content-store.h (module 'ndnSIM'): ns3::ndn::Name const & ns3::ndn::cs::Entry::GetName() const [member function]
    cls.add_method('GetName', 
                   'ns3::ndn::Name const &', 
                   [], 
                   is_const=True)
    return

def register_Ns3NdnCsStats_methods(root_module, cls):
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::cs::Stats::Stats() [constructor]
    cls.add_constructor([])
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::cs::Stats::Stats(ns3::ndn::cs::Stats const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::cs::Stats const &', 'arg0')])
    ## ndn-cs-tracer.h (module 'ndnSIM'): void ns3::ndn::cs::Stats::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::cs::Stats::m_cacheHits [variable]
    cls.add_instance_attribute('m_cacheHits', 'double', is_const=False)
    ## ndn-cs-tracer.h (module 'ndnSIM'): ns3::ndn::cs::Stats::m_cacheMisses [variable]
    cls.add_instance_attribute('m_cacheMisses', 'double', is_const=False)
    return

def register_Ns3NdnFibEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::Entry(ns3::ndn::fib::Entry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::Entry const &', 'arg0')])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::Entry(ns3::Ptr<ns3::ndn::Fib> fib, ns3::Ptr<ns3::ndn::Name const> const & prefix) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ndn::Fib >', 'fib'), param('ns3::Ptr< ns3::ndn::Name const > const &', 'prefix')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::Entry::AddOrUpdateRoutingMetric(ns3::Ptr<ns3::ndn::Face> face, int32_t metric) [member function]
    cls.add_method('AddOrUpdateRoutingMetric', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('int32_t', 'metric')])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetric const & ns3::ndn::fib::Entry::FindBestCandidate(uint32_t skip=0) const [member function]
    cls.add_method('FindBestCandidate', 
                   'ns3::ndn::fib::FaceMetric const &', 
                   [param('uint32_t', 'skip', default_value='0')], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Fib> ns3::ndn::fib::Entry::GetFib() [member function]
    cls.add_method('GetFib', 
                   'ns3::Ptr< ns3::ndn::Fib >', 
                   [])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::Name const & ns3::ndn::fib::Entry::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::ndn::Name const &', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::Entry::Invalidate() [member function]
    cls.add_method('Invalidate', 
                   'void', 
                   [])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::Entry::RemoveFace(ns3::Ptr<ns3::ndn::Face> const & face) [member function]
    cls.add_method('RemoveFace', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face > const &', 'face')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::Entry::SetRealDelayToProducer(ns3::Ptr<ns3::ndn::Face> face, ns3::Time delay) [member function]
    cls.add_method('SetRealDelayToProducer', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('ns3::Time', 'delay')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::Entry::UpdateFaceRtt(ns3::Ptr<ns3::ndn::Face> face, ns3::Time const & sample) [member function]
    cls.add_method('UpdateFaceRtt', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('ns3::Time const &', 'sample')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::Entry::UpdateStatus(ns3::Ptr<ns3::ndn::Face> face, ns3::ndn::fib::FaceMetric::Status status) [member function]
    cls.add_method('UpdateStatus', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('ns3::ndn::fib::FaceMetric::Status', 'status')])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::m_faces [variable]
    cls.add_instance_attribute('m_faces', 'boost::multi_index::multi_index_container< ns3::ndn::fib::FaceMetric, boost::multi_index::indexed_by< boost::multi_index::ordered_unique< boost::multi_index::tag< ns3::ndn::fib::i_face, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na >, boost::multi_index::const_mem_fun< ns3::ndn::fib::FaceMetric, ns3::Ptr< ns3::ndn::Face >, & ( ns3::ndn::fib::FaceMetric::GetFace (  ) const ) >, mpl_::na >, boost::multi_index::ordered_non_unique< boost::multi_index::tag< ns3::ndn::fib::i_metric, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na >, boost::multi_index::composite_key< ns3::ndn::fib::FaceMetric, boost::multi_index::const_mem_fun< ns3::ndn::fib::FaceMetric, ns3::ndn::fib::FaceMetric::Status, & ( ns3::ndn::fib::FaceMetric::GetStatus (  ) const ) >, boost::multi_index::const_mem_fun< ns3::ndn::fib::FaceMetric, int, & ( ns3::ndn::fib::FaceMetric::GetRoutingCost (  ) const ) >, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type >, mpl_::na >, boost::multi_index::random_access< boost::multi_index::tag< ns3::ndn::fib::i_nth, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na > >, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na >, std::allocator< ns3::ndn::fib::FaceMetric > >', is_const=False)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::m_fib [variable]
    cls.add_instance_attribute('m_fib', 'ns3::Ptr< ns3::ndn::Fib >', is_const=False)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::m_needsProbing [variable]
    cls.add_instance_attribute('m_needsProbing', 'bool', is_const=False)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::m_prefix [variable]
    cls.add_instance_attribute('m_prefix', 'ns3::Ptr< ns3::ndn::Name const >', is_const=False)
    return

def register_Ns3NdnFibEntryNoFaces_methods(root_module, cls):
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::NoFaces::NoFaces() [constructor]
    cls.add_constructor([])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::Entry::NoFaces::NoFaces(ns3::ndn::fib::Entry::NoFaces const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::Entry::NoFaces const &', 'arg0')])
    return

def register_Ns3NdnFibFaceMetric_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<')
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetric::FaceMetric(ns3::ndn::fib::FaceMetric const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::FaceMetric const &', 'arg0')])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetric::FaceMetric(ns3::Ptr<ns3::ndn::Face> face, int32_t cost) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ndn::Face >', 'face'), param('int32_t', 'cost')])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Face> ns3::ndn::fib::FaceMetric::GetFace() const [member function]
    cls.add_method('GetFace', 
                   'ns3::Ptr< ns3::ndn::Face >', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::Time ns3::ndn::fib::FaceMetric::GetRealDelay() const [member function]
    cls.add_method('GetRealDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): int32_t ns3::ndn::fib::FaceMetric::GetRoutingCost() const [member function]
    cls.add_method('GetRoutingCost', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::Time ns3::ndn::fib::FaceMetric::GetRttVar() const [member function]
    cls.add_method('GetRttVar', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::Time ns3::ndn::fib::FaceMetric::GetSRtt() const [member function]
    cls.add_method('GetSRtt', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetric::Status ns3::ndn::fib::FaceMetric::GetStatus() const [member function]
    cls.add_method('GetStatus', 
                   'ns3::ndn::fib::FaceMetric::Status', 
                   [], 
                   is_const=True)
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::TracedValue<ns3::ndn::fib::FaceMetric::Status> & ns3::ndn::fib::FaceMetric::GetStatusTrace() [member function]
    cls.add_method('GetStatusTrace', 
                   'ns3::TracedValue< ns3::ndn::fib::FaceMetric::Status > &', 
                   [])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::FaceMetric::SetRealDelay(ns3::Time realDelay) [member function]
    cls.add_method('SetRealDelay', 
                   'void', 
                   [param('ns3::Time', 'realDelay')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::FaceMetric::SetRoutingCost(int32_t routingCost) [member function]
    cls.add_method('SetRoutingCost', 
                   'void', 
                   [param('int32_t', 'routingCost')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::FaceMetric::SetStatus(ns3::ndn::fib::FaceMetric::Status status) [member function]
    cls.add_method('SetStatus', 
                   'void', 
                   [param('ns3::ndn::fib::FaceMetric::Status', 'status')])
    ## ndn-fib-entry.h (module 'ndnSIM'): void ns3::ndn::fib::FaceMetric::UpdateRtt(ns3::Time const & rttSample) [member function]
    cls.add_method('UpdateRtt', 
                   'void', 
                   [param('ns3::Time const &', 'rttSample')])
    return

def register_Ns3NdnFibFaceMetricContainer_methods(root_module, cls):
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetricContainer::FaceMetricContainer() [constructor]
    cls.add_constructor([])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::FaceMetricContainer::FaceMetricContainer(ns3::ndn::fib::FaceMetricContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::FaceMetricContainer const &', 'arg0')])
    return

def register_Ns3NdnFibI_face_methods(root_module, cls):
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_face::i_face() [constructor]
    cls.add_constructor([])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_face::i_face(ns3::ndn::fib::i_face const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::i_face const &', 'arg0')])
    return

def register_Ns3NdnFibI_metric_methods(root_module, cls):
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_metric::i_metric() [constructor]
    cls.add_constructor([])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_metric::i_metric(ns3::ndn::fib::i_metric const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::i_metric const &', 'arg0')])
    return

def register_Ns3NdnFibI_nth_methods(root_module, cls):
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_nth::i_nth() [constructor]
    cls.add_constructor([])
    ## ndn-fib-entry.h (module 'ndnSIM'): ns3::ndn::fib::i_nth::i_nth(ns3::ndn::fib::i_nth const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fib::i_nth const &', 'arg0')])
    return

def register_Ns3NdnFwTag_methods(root_module, cls):
    ## ndn-fw-tag.h (module 'ndnSIM'): ns3::ndn::fw::Tag::Tag() [constructor]
    cls.add_constructor([])
    ## ndn-fw-tag.h (module 'ndnSIM'): ns3::ndn::fw::Tag::Tag(ns3::ndn::fw::Tag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::fw::Tag const &', 'arg0')])
    return

def register_Ns3NdnNameComponent_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(ns3::ndn::name::Component const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::name::Component const &', 'arg0')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component() [constructor]
    cls.add_constructor([])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(std::string const & uri) [constructor]
    cls.add_constructor([param('std::string const &', 'uri')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(__gnu_cxx::__normal_iterator<char const*, std::string> begin, __gnu_cxx::__normal_iterator<char const*, std::string> end) [constructor]
    cls.add_constructor([param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'begin'), param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'end')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component::Component(void const * buf, size_t length) [constructor]
    cls.add_constructor([param('void const *', 'buf'), param('size_t', 'length')])
    ## name-component.h (module 'ndnSIM'): int ns3::ndn::name::Component::compare(ns3::ndn::name::Component const & other) const [member function]
    cls.add_method('compare', 
                   'int', 
                   [param('ns3::ndn::name::Component const &', 'other')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromNumber(uint64_t number) [member function]
    cls.add_method('fromNumber', 
                   'ns3::ndn::name::Component &', 
                   [param('uint64_t', 'number')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromNumberWithMarker(uint64_t number, unsigned char marker) [member function]
    cls.add_method('fromNumberWithMarker', 
                   'ns3::ndn::name::Component &', 
                   [param('uint64_t', 'number'), param('unsigned char', 'marker')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromUri(std::string const & uri) [member function]
    cls.add_method('fromUri', 
                   'ns3::ndn::name::Component &', 
                   [param('std::string const &', 'uri')])
    ## name-component.h (module 'ndnSIM'): ns3::ndn::name::Component & ns3::ndn::name::Component::fromUri(__gnu_cxx::__normal_iterator<char const*, std::string> begin, __gnu_cxx::__normal_iterator<char const*, std::string> end) [member function]
    cls.add_method('fromUri', 
                   'ns3::ndn::name::Component &', 
                   [param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'begin'), param('__gnu_cxx::__normal_iterator< char const *, std::string >', 'end')])
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toBlkId() const [member function]
    cls.add_method('toBlkId', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): std::string ns3::ndn::name::Component::toBlob() const [member function]
    cls.add_method('toBlob', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): void ns3::ndn::name::Component::toBlob(std::ostream & os) const [member function]
    cls.add_method('toBlob', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toControlNum() const [member function]
    cls.add_method('toControlNum', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toNumber() const [member function]
    cls.add_method('toNumber', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toNumberWithMarker(unsigned char marker) const [member function]
    cls.add_method('toNumberWithMarker', 
                   'uint64_t', 
                   [param('unsigned char', 'marker')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toSeqNum() const [member function]
    cls.add_method('toSeqNum', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): std::string ns3::ndn::name::Component::toUri() const [member function]
    cls.add_method('toUri', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): void ns3::ndn::name::Component::toUri(std::ostream & os) const [member function]
    cls.add_method('toUri', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## name-component.h (module 'ndnSIM'): uint64_t ns3::ndn::name::Component::toVersion() const [member function]
    cls.add_method('toVersion', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    return

def register_Ns3NdnPitEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::pit::Entry::Entry(ns3::ndn::pit::Entry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::pit::Entry const &', 'arg0')])
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::pit::Entry::Entry(ns3::ndn::Pit & container, ns3::Ptr<ns3::ndn::Interest const> header, ns3::Ptr<ns3::ndn::fib::Entry> fibEntry) [constructor]
    cls.add_constructor([param('ns3::ndn::Pit &', 'container'), param('ns3::Ptr< ns3::ndn::Interest const >', 'header'), param('ns3::Ptr< ns3::ndn::fib::Entry >', 'fibEntry')])
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::AddFwTag(boost::shared_ptr<ns3::ndn::fw::Tag> tag) [member function]
    cls.add_method('AddFwTag', 
                   'void', 
                   [param('boost::shared_ptr< ns3::ndn::fw::Tag >', 'tag')])
    ## ndn-pit-entry.h (module 'ndnSIM'): std::_Rb_tree_const_iterator<ns3::ndn::pit::IncomingFace> ns3::ndn::pit::Entry::AddIncoming(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('AddIncoming', 
                   'std::_Rb_tree_const_iterator< ns3::ndn::pit::IncomingFace >', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): std::_Rb_tree_const_iterator<ns3::ndn::pit::OutgoingFace> ns3::ndn::pit::Entry::AddOutgoing(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('AddOutgoing', 
                   'std::_Rb_tree_const_iterator< ns3::ndn::pit::OutgoingFace >', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::AddSeenNonce(uint32_t nonce) [member function]
    cls.add_method('AddSeenNonce', 
                   'void', 
                   [param('uint32_t', 'nonce')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): bool ns3::ndn::pit::Entry::AreAllOutgoingInVain() const [member function]
    cls.add_method('AreAllOutgoingInVain', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): bool ns3::ndn::pit::Entry::AreTherePromisingOutgoingFacesExcept(ns3::Ptr<ns3::ndn::Face> face) const [member function]
    cls.add_method('AreTherePromisingOutgoingFacesExcept', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::ClearIncoming() [member function]
    cls.add_method('ClearIncoming', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::ClearOutgoing() [member function]
    cls.add_method('ClearOutgoing', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::Time const & ns3::ndn::pit::Entry::GetExpireTime() const [member function]
    cls.add_method('GetExpireTime', 
                   'ns3::Time const &', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::fib::Entry> ns3::ndn::pit::Entry::GetFibEntry() [member function]
    cls.add_method('GetFibEntry', 
                   'ns3::Ptr< ns3::ndn::fib::Entry >', 
                   [])
    ## ndn-pit-entry.h (module 'ndnSIM'): std::set<ns3::ndn::pit::IncomingFace, std::less<ns3::ndn::pit::IncomingFace>, std::allocator<ns3::ndn::pit::IncomingFace> > const & ns3::ndn::pit::Entry::GetIncoming() const [member function]
    cls.add_method('GetIncoming', 
                   'std::set< ns3::ndn::pit::IncomingFace > const &', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::Ptr<ns3::ndn::Interest const> ns3::ndn::pit::Entry::GetInterest() const [member function]
    cls.add_method('GetInterest', 
                   'ns3::Ptr< ns3::ndn::Interest const >', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): uint32_t ns3::ndn::pit::Entry::GetMaxRetxCount() const [member function]
    cls.add_method('GetMaxRetxCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): std::set<ns3::ndn::pit::OutgoingFace, std::less<ns3::ndn::pit::OutgoingFace>, std::allocator<ns3::ndn::pit::OutgoingFace> > const & ns3::ndn::pit::Entry::GetOutgoing() const [member function]
    cls.add_method('GetOutgoing', 
                   'std::set< ns3::ndn::pit::OutgoingFace > const &', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): uint32_t ns3::ndn::pit::Entry::GetOutgoingCount() const [member function]
    cls.add_method('GetOutgoingCount', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::Name const & ns3::ndn::pit::Entry::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::ndn::Name const &', 
                   [], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::IncreaseAllowedRetxCount() [member function]
    cls.add_method('IncreaseAllowedRetxCount', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): bool ns3::ndn::pit::Entry::IsNonceSeen(uint32_t nonce) const [member function]
    cls.add_method('IsNonceSeen', 
                   'bool', 
                   [param('uint32_t', 'nonce')], 
                   is_const=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::OffsetLifetime(ns3::Time const & offsetTime) [member function]
    cls.add_method('OffsetLifetime', 
                   'void', 
                   [param('ns3::Time const &', 'offsetTime')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::RemoveAllReferencesToFace(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('RemoveAllReferencesToFace', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::RemoveIncoming(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('RemoveIncoming', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::SetWaitingInVain(ns3::Ptr<ns3::ndn::Face> face) [member function]
    cls.add_method('SetWaitingInVain', 
                   'void', 
                   [param('ns3::Ptr< ns3::ndn::Face >', 'face')], 
                   is_virtual=True)
    ## ndn-pit-entry.h (module 'ndnSIM'): void ns3::ndn::pit::Entry::UpdateLifetime(ns3::Time const & lifetime) [member function]
    cls.add_method('UpdateLifetime', 
                   'void', 
                   [param('ns3::Time const &', 'lifetime')], 
                   is_virtual=True)
    return

def register_Ns3NdnPitEntryIsNotEmpty_methods(root_module, cls):
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::pit::EntryIsNotEmpty::EntryIsNotEmpty() [constructor]
    cls.add_constructor([])
    ## ndn-pit-entry.h (module 'ndnSIM'): ns3::ndn::pit::EntryIsNotEmpty::EntryIsNotEmpty(ns3::ndn::pit::EntryIsNotEmpty const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::pit::EntryIsNotEmpty const &', 'arg0')])
    ## ndn-pit-entry.h (module 'ndnSIM'): bool ns3::ndn::pit::EntryIsNotEmpty::operator()(ns3::Ptr<ns3::ndn::pit::Entry> entry) [member operator]
    cls.add_method('operator()', 
                   'bool', 
                   [param('ns3::Ptr< ns3::ndn::pit::Entry >', 'entry')], 
                   custom_name='__call__')
    return

def register_Ns3NdnPitIncomingFace_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ndn-pit-entry-incoming-face.h (module 'ndnSIM'): ns3::ndn::pit::IncomingFace::IncomingFace(ns3::ndn::pit::IncomingFace const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::pit::IncomingFace const &', 'arg0')])
    ## ndn-pit-entry-incoming-face.h (module 'ndnSIM'): ns3::ndn::pit::IncomingFace::IncomingFace(ns3::Ptr<ns3::ndn::Face> face) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ndn::Face >', 'face')])
    ## ndn-pit-entry-incoming-face.h (module 'ndnSIM'): ns3::ndn::pit::IncomingFace::IncomingFace() [constructor]
    cls.add_constructor([])
    ## ndn-pit-entry-incoming-face.h (module 'ndnSIM'): ns3::ndn::pit::IncomingFace::m_arrivalTime [variable]
    cls.add_instance_attribute('m_arrivalTime', 'ns3::Time', is_const=False)
    ## ndn-pit-entry-incoming-face.h (module 'ndnSIM'): ns3::ndn::pit::IncomingFace::m_face [variable]
    cls.add_instance_attribute('m_face', 'ns3::Ptr< ns3::ndn::Face >', is_const=False)
    return

def register_Ns3NdnPitOutgoingFace_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::OutgoingFace(ns3::ndn::pit::OutgoingFace const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ndn::pit::OutgoingFace const &', 'arg0')])
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::OutgoingFace(ns3::Ptr<ns3::ndn::Face> face) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::ndn::Face >', 'face')])
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::OutgoingFace() [constructor]
    cls.add_constructor([])
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): void ns3::ndn::pit::OutgoingFace::UpdateOnRetransmit() [member function]
    cls.add_method('UpdateOnRetransmit', 
                   'void', 
                   [])
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::m_face [variable]
    cls.add_instance_attribute('m_face', 'ns3::Ptr< ns3::ndn::Face >', is_const=False)
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::m_retxCount [variable]
    cls.add_instance_attribute('m_retxCount', 'uint32_t', is_const=False)
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::m_sendTime [variable]
    cls.add_instance_attribute('m_sendTime', 'ns3::Time', is_const=False)
    ## ndn-pit-entry-outgoing-face.h (module 'ndnSIM'): ns3::ndn::pit::OutgoingFace::m_waitingInVain [variable]
    cls.add_instance_attribute('m_waitingInVain', 'bool', is_const=False)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_Hash(module.get_submodule('Hash'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_ndn(module.get_submodule('ndn'), root_module)
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_Hash(module, root_module):
    register_functions_ns3_Hash_Function(module.get_submodule('Function'), root_module)
    return

def register_functions_ns3_Hash_Function(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_ndn(module, root_module):
    ## ndn-wire.h (module 'ndnSIM'): ns3::Ptr<ns3::Packet> ns3::ndn::BufferToPacket(std::string const & buffer) [free function]
    module.add_function('BufferToPacket', 
                        'ns3::Ptr< ns3::Packet >', 
                        [param('std::string const &', 'buffer')])
    ## name.h (module 'ndnSIM'): extern ns3::Ptr<ns3::AttributeChecker const> ns3::ndn::MakeNameChecker() [free function]
    module.add_function('MakeNameChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## ndn-wire.h (module 'ndnSIM'): std::string ns3::ndn::PacketToBuffer(ns3::Ptr<ns3::Packet const> pkt) [free function]
    module.add_function('PacketToBuffer', 
                        'std::string', 
                        [param('ns3::Ptr< ns3::Packet const >', 'pkt')])
    register_functions_ns3_ndn_cs(module.get_submodule('cs'), root_module)
    register_functions_ns3_ndn_fib(module.get_submodule('fib'), root_module)
    register_functions_ns3_ndn_fw(module.get_submodule('fw'), root_module)
    register_functions_ns3_ndn_name(module.get_submodule('name'), root_module)
    register_functions_ns3_ndn_pit(module.get_submodule('pit'), root_module)
    register_functions_ns3_ndn_time(module.get_submodule('time'), root_module)
    return

def register_functions_ns3_ndn_cs(module, root_module):
    return

def register_functions_ns3_ndn_fib(module, root_module):
    return

def register_functions_ns3_ndn_fw(module, root_module):
    return

def register_functions_ns3_ndn_name(module, root_module):
    return

def register_functions_ns3_ndn_pit(module, root_module):
    return

def register_functions_ns3_ndn_time(module, root_module):
    ## ndn-common.h (module 'ndnSIM'): ns3::Time ns3::ndn::time::NowUnixTimestamp() [free function]
    module.add_function('NowUnixTimestamp', 
                        'ns3::Time', 
                        [])
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

