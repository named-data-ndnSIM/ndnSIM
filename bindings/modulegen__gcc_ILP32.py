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
    root_module = Module('ns.NDNabstraction', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    root_module = module.get_root()
    
    ## log.h (module 'core'): ns3::LogLevel [enumeration]
    module.add_enum('LogLevel', ['LOG_NONE', 'LOG_ERROR', 'LOG_LEVEL_ERROR', 'LOG_WARN', 'LOG_LEVEL_WARN', 'LOG_DEBUG', 'LOG_LEVEL_DEBUG', 'LOG_INFO', 'LOG_LEVEL_INFO', 'LOG_FUNCTION', 'LOG_LEVEL_FUNCTION', 'LOG_LOGIC', 'LOG_LEVEL_LOGIC', 'LOG_ALL', 'LOG_LEVEL_ALL', 'LOG_PREFIX_FUNC', 'LOG_PREFIX_TIME', 'LOG_PREFIX_NODE'], import_from_module='ns.core')
    ## address.h (module 'network'): ns3::Address [class]
    module.add_class('Address', import_from_module='ns.network')
    ## address.h (module 'network'): ns3::Address::MaxSize_e [enumeration]
    module.add_enum('MaxSize_e', ['MAX_SIZE'], outer_class=root_module['ns3::Address'], import_from_module='ns.network')
    ## application-container.h (module 'network'): ns3::ApplicationContainer [class]
    module.add_class('ApplicationContainer', import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelper [class]
    module.add_class('AsciiTraceHelper', import_from_module='ns.network')
    ## ccnx-trace-helper.h (module 'NDNabstraction'): ns3::AsciiTraceHelperForCcnx [class]
    module.add_class('AsciiTraceHelperForCcnx', allow_subclassing=True)
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelperForDevice [class]
    module.add_class('AsciiTraceHelperForDevice', allow_subclassing=True, import_from_module='ns.network')
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
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): ns3::CcnxConsumerHelper [class]
    module.add_class('CcnxConsumerHelper')
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStoreContainer [struct]
    module.add_class('CcnxContentStoreContainer')
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStoreEntry [class]
    module.add_class('CcnxContentStoreEntry')
    ## ccnx-decoding-helper.h (module 'NDNabstraction'): ns3::CcnxDecodingHelper [class]
    module.add_class('CcnxDecodingHelper')
    ## ccnx-encoding-helper.h (module 'NDNabstraction'): ns3::CcnxEncodingHelper [class]
    module.add_class('CcnxEncodingHelper')
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFib [class]
    module.add_class('CcnxFib')
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibEntryContainer [struct]
    module.add_class('CcnxFibEntryContainer')
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric [class]
    module.add_class('CcnxFibFaceMetric')
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetricContainer [struct]
    module.add_class('CcnxFibFaceMetricContainer')
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): ns3::CcnxForwardingHelper [class]
    module.add_class('CcnxForwardingHelper', allow_subclassing=True)
    ## ccnx-header-helper.h (module 'NDNabstraction'): ns3::CcnxHeaderHelper [class]
    module.add_class('CcnxHeaderHelper')
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): ns3::CcnxInterestSenderHelper [class]
    module.add_class('CcnxInterestSenderHelper')
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry [struct]
    module.add_class('CcnxPitEntry')
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::AddIncoming [struct]
    module.add_class('AddIncoming', outer_class=root_module['ns3::CcnxPitEntry'])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::AddOutgoing [struct]
    module.add_class('AddOutgoing', outer_class=root_module['ns3::CcnxPitEntry'])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::ClearIncoming [struct]
    module.add_class('ClearIncoming', outer_class=root_module['ns3::CcnxPitEntry'])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::DeleteIncoming [struct]
    module.add_class('DeleteIncoming', outer_class=root_module['ns3::CcnxPitEntry'])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::SetFibEntry [struct]
    module.add_class('SetFibEntry', outer_class=root_module['ns3::CcnxPitEntry'])
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntryContainer [struct]
    module.add_class('CcnxPitEntryContainer')
    ## ccnx-pit-entry-incoming-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFace [struct]
    module.add_class('CcnxPitEntryIncomingFace')
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFaceContainer [struct]
    module.add_class('CcnxPitEntryIncomingFaceContainer')
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntryNotFound [class]
    module.add_class('CcnxPitEntryNotFound')
    ## ccnx-pit-entry-outgoing-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFace [struct]
    module.add_class('CcnxPitEntryOutgoingFace')
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFaceContainer [struct]
    module.add_class('CcnxPitEntryOutgoingFaceContainer')
    ## ccnx-producer-helper.h (module 'NDNabstraction'): ns3::CcnxProducerHelper [class]
    module.add_class('CcnxProducerHelper')
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitContainer [struct]
    module.add_class('CcnxRitContainer')
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitEntry [struct]
    module.add_class('CcnxRitEntry')
    ## ccnx-header-helper.h (module 'NDNabstraction'): ns3::CcnxUnknownHeaderException [class]
    module.add_class('CcnxUnknownHeaderException')
    ## data-rate.h (module 'network'): ns3::DataRate [class]
    module.add_class('DataRate', import_from_module='ns.network')
    ## event-id.h (module 'core'): ns3::EventId [class]
    module.add_class('EventId', import_from_module='ns.core')
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
    ## log.h (module 'core'): ns3::LogComponent [class]
    module.add_class('LogComponent', import_from_module='ns.core')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    module.add_class('Mac48Address', import_from_module='ns.network')
    ## mac48-address.h (module 'network'): ns3::Mac48Address [class]
    root_module['ns3::Mac48Address'].implicitly_converts_to(root_module['ns3::Address'])
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
    ## pcap-file.h (module 'network'): ns3::PcapFile [class]
    module.add_class('PcapFile', import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::PcapHelper [class]
    module.add_class('PcapHelper', import_from_module='ns.network')
    ## trace-helper.h (module 'network'): ns3::PcapHelper [enumeration]
    module.add_enum('', ['DLT_NULL', 'DLT_EN10MB', 'DLT_PPP', 'DLT_RAW', 'DLT_IEEE802_11', 'DLT_PRISM_HEADER', 'DLT_IEEE802_11_RADIO'], outer_class=root_module['ns3::PcapHelper'], import_from_module='ns.network')
    ## ccnx-trace-helper.h (module 'NDNabstraction'): ns3::PcapHelperForCcnx [class]
    module.add_class('PcapHelperForCcnx', allow_subclassing=True)
    ## trace-helper.h (module 'network'): ns3::PcapHelperForDevice [class]
    module.add_class('PcapHelperForDevice', allow_subclassing=True, import_from_module='ns.network')
    ## point-to-point-helper.h (module 'point-to-point'): ns3::PointToPointHelper [class]
    module.add_class('PointToPointHelper', import_from_module='ns.point_to_point', parent=[root_module['ns3::PcapHelperForDevice'], root_module['ns3::AsciiTraceHelperForDevice']])
    ## random-variable.h (module 'core'): ns3::RandomVariable [class]
    module.add_class('RandomVariable', import_from_module='ns.core')
    ## random-variable.h (module 'core'): ns3::SeedManager [class]
    module.add_class('SeedManager', import_from_module='ns.core')
    ## random-variable.h (module 'core'): ns3::SequentialVariable [class]
    module.add_class('SequentialVariable', import_from_module='ns.core', parent=root_module['ns3::RandomVariable'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Chunk', 'ns3::ObjectBase', 'ns3::DefaultDeleter<ns3::Chunk>'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simulator.h (module 'core'): ns3::Simulator [class]
    module.add_class('Simulator', destructor_visibility='private', import_from_module='ns.core')
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): ns3::StupidInterestGeneratorHelper [class]
    module.add_class('StupidInterestGeneratorHelper')
    ## tag.h (module 'network'): ns3::Tag [class]
    module.add_class('Tag', import_from_module='ns.network', parent=root_module['ns3::ObjectBase'])
    ## tag-buffer.h (module 'network'): ns3::TagBuffer [class]
    module.add_class('TagBuffer', import_from_module='ns.network')
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
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::CcnxStackHelper [class]
    module.add_class('CcnxStackHelper', parent=[root_module['ns3::PcapHelperForCcnx'], root_module['ns3::AsciiTraceHelperForCcnx']])
    ## chunk.h (module 'network'): ns3::Chunk [class]
    module.add_class('Chunk', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> >'])
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
    ## pcap-file-wrapper.h (module 'network'): ns3::PcapFileWrapper [class]
    module.add_class('PcapFileWrapper', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## queue.h (module 'network'): ns3::Queue [class]
    module.add_class('Queue', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeChecker', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeChecker>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::AttributeValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::CallbackImplBase', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CallbackImplBase>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::CcnbParser::Block', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CcnbParser::Block>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::CcnxFaceContainer', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CcnxFaceContainer>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::CcnxFibEntry', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CcnxFibEntry>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::CcnxNameComponents', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CcnxNameComponents>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::CcnxRoute', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CcnxRoute>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::EventImpl', 'ns3::empty', 'ns3::DefaultDeleter<ns3::EventImpl>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::NixVector', 'ns3::empty', 'ns3::DefaultDeleter<ns3::NixVector>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::OutputStreamWrapper', 'ns3::empty', 'ns3::DefaultDeleter<ns3::OutputStreamWrapper>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::Packet', 'ns3::empty', 'ns3::DefaultDeleter<ns3::Packet>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, import_from_module='ns.core', template_parameters=['ns3::TraceSourceAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::TraceSourceAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## socket.h (module 'network'): ns3::Socket [class]
    module.add_class('Socket', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## socket.h (module 'network'): ns3::Socket::SocketErrno [enumeration]
    module.add_enum('SocketErrno', ['ERROR_NOTERROR', 'ERROR_ISCONN', 'ERROR_NOTCONN', 'ERROR_MSGSIZE', 'ERROR_AGAIN', 'ERROR_SHUTDOWN', 'ERROR_OPNOTSUPP', 'ERROR_AFNOSUPPORT', 'ERROR_INVAL', 'ERROR_BADF', 'ERROR_NOROUTETOHOST', 'ERROR_NODEV', 'ERROR_ADDRNOTAVAIL', 'ERROR_ADDRINUSE', 'SOCKET_ERRNO_LAST'], outer_class=root_module['ns3::Socket'], import_from_module='ns.network')
    ## socket.h (module 'network'): ns3::Socket::SocketType [enumeration]
    module.add_enum('SocketType', ['NS3_SOCK_STREAM', 'NS3_SOCK_SEQPACKET', 'NS3_SOCK_DGRAM', 'NS3_SOCK_RAW'], outer_class=root_module['ns3::Socket'], import_from_module='ns.network')
    ## socket.h (module 'network'): ns3::SocketAddressTag [class]
    module.add_class('SocketAddressTag', import_from_module='ns.network', parent=root_module['ns3::Tag'])
    ## socket-factory.h (module 'network'): ns3::SocketFactory [class]
    module.add_class('SocketFactory', import_from_module='ns.network', parent=root_module['ns3::Object'])
    ## socket.h (module 'network'): ns3::SocketIpTtlTag [class]
    module.add_class('SocketIpTtlTag', import_from_module='ns.network', parent=root_module['ns3::Tag'])
    ## socket.h (module 'network'): ns3::SocketSetDontFragmentTag [class]
    module.add_class('SocketSetDontFragmentTag', import_from_module='ns.network', parent=root_module['ns3::Tag'])
    ## nstime.h (module 'core'): ns3::Time [class]
    module.add_class('Time', import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time::Unit [enumeration]
    module.add_enum('Unit', ['S', 'MS', 'US', 'NS', 'PS', 'FS', 'LAST'], outer_class=root_module['ns3::Time'], import_from_module='ns.core')
    ## nstime.h (module 'core'): ns3::Time [class]
    root_module['ns3::Time'].implicitly_converts_to(root_module['ns3::int64x64_t'])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader [class]
    module.add_class('TopologyReader', import_from_module='ns.topology_read', parent=root_module['ns3::Object'])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link [class]
    module.add_class('Link', import_from_module='ns.topology_read', outer_class=root_module['ns3::TopologyReader'])
    ## trace-source-accessor.h (module 'core'): ns3::TraceSourceAccessor [class]
    module.add_class('TraceSourceAccessor', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    ## trailer.h (module 'network'): ns3::Trailer [class]
    module.add_class('Trailer', import_from_module='ns.network', parent=root_module['ns3::Chunk'])
    ## udp-socket-factory.h (module 'internet'): ns3::UdpSocketFactory [class]
    module.add_class('UdpSocketFactory', import_from_module='ns.internet', parent=root_module['ns3::SocketFactory'])
    ## annotated-topology-reader.h (module 'NDNabstraction'): ns3::AnnotatedTopologyReader [class]
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
    ## callback.h (module 'core'): ns3::CallbackChecker [class]
    module.add_class('CallbackChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## callback.h (module 'core'): ns3::CallbackImplBase [class]
    module.add_class('CallbackImplBase', import_from_module='ns.core', parent=root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    ## callback.h (module 'core'): ns3::CallbackValue [class]
    module.add_class('CallbackValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## ccnx.h (module 'NDNabstraction'): ns3::Ccnx [class]
    module.add_class('Ccnx', parent=root_module['ns3::Object'])
    ## ccnx-consumer.h (module 'NDNabstraction'): ns3::CcnxConsumer [class]
    module.add_class('CcnxConsumer', parent=root_module['ns3::Application'])
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxContentObjectHeader [class]
    module.add_class('CcnxContentObjectHeader', parent=root_module['ns3::Header'])
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxContentObjectTail [class]
    module.add_class('CcnxContentObjectTail', parent=root_module['ns3::Trailer'])
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStore [class]
    module.add_class('CcnxContentStore', parent=root_module['ns3::Object'])
    ## ccnx-face.h (module 'NDNabstraction'): ns3::CcnxFace [class]
    module.add_class('CcnxFace', parent=root_module['ns3::Object'])
    ## ccnx-face-container.h (module 'NDNabstraction'): ns3::CcnxFaceContainer [class]
    module.add_class('CcnxFaceContainer', parent=root_module['ns3::SimpleRefCount< ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> >'])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibEntry [class]
    module.add_class('CcnxFibEntry', parent=root_module['ns3::SimpleRefCount< ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> >'])
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): ns3::CcnxForwardingStrategy [class]
    module.add_class('CcnxForwardingStrategy', parent=root_module['ns3::Object'])
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::CcnxInterestHeader [class]
    module.add_class('CcnxInterestHeader', parent=root_module['ns3::Header'])
    ## ccnx-interest-sender.h (module 'NDNabstraction'): ns3::CcnxInterestSender [class]
    module.add_class('CcnxInterestSender', parent=root_module['ns3::Application'])
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::CcnxL3Protocol [class]
    module.add_class('CcnxL3Protocol', parent=root_module['ns3::Ccnx'])
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::CcnxL3Protocol::DropReason [enumeration]
    module.add_enum('DropReason', ['DROP_DUPLICATE_INTEREST', 'DROP_CONGESTION', 'DROP_NO_ROUTE', 'DROP_INTERFACE_DOWN'], outer_class=root_module['ns3::CcnxL3Protocol'])
    ## ccnx-local-face.h (module 'NDNabstraction'): ns3::CcnxLocalFace [class]
    module.add_class('CcnxLocalFace', parent=root_module['ns3::CcnxFace'])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents [class]
    module.add_class('CcnxNameComponents', parent=root_module['ns3::SimpleRefCount< ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> >'])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsChecker [class]
    module.add_class('CcnxNameComponentsChecker', parent=root_module['ns3::AttributeChecker'])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsValue [class]
    module.add_class('CcnxNameComponentsValue', parent=root_module['ns3::AttributeValue'])
    ## ccnx-net-device-face.h (module 'NDNabstraction'): ns3::CcnxNetDeviceFace [class]
    module.add_class('CcnxNetDeviceFace', parent=root_module['ns3::CcnxFace'])
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPit [class]
    module.add_class('CcnxPit', parent=root_module['ns3::Object'])
    ## ccnx-producer.h (module 'NDNabstraction'): ns3::CcnxProducer [class]
    module.add_class('CcnxProducer', parent=root_module['ns3::Application'])
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRit [class]
    module.add_class('CcnxRit', parent=root_module['ns3::Object'])
    ## ccnx-route.h (module 'NDNabstraction'): ns3::CcnxRoute [class]
    module.add_class('CcnxRoute', parent=root_module['ns3::SimpleRefCount< ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> >'])
    ## data-rate.h (module 'network'): ns3::DataRateChecker [class]
    module.add_class('DataRateChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## data-rate.h (module 'network'): ns3::DataRateValue [class]
    module.add_class('DataRateValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
    ## drop-tail-queue.h (module 'network'): ns3::DropTailQueue [class]
    module.add_class('DropTailQueue', import_from_module='ns.network', parent=root_module['ns3::Queue'])
    ## drop-tail-queue.h (module 'network'): ns3::DropTailQueue::Mode [enumeration]
    module.add_enum('Mode', ['ILLEGAL', 'PACKETS', 'BYTES'], outer_class=root_module['ns3::DropTailQueue'], import_from_module='ns.network')
    ## attribute.h (module 'core'): ns3::EmptyAttributeValue [class]
    module.add_class('EmptyAttributeValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
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
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker [class]
    module.add_class('Mac48AddressChecker', import_from_module='ns.network', parent=root_module['ns3::AttributeChecker'])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue [class]
    module.add_class('Mac48AddressValue', import_from_module='ns.network', parent=root_module['ns3::AttributeValue'])
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
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper [class]
    module.add_class('OutputStreamWrapper', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    ## packet.h (module 'network'): ns3::Packet [class]
    module.add_class('Packet', import_from_module='ns.network', parent=root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::PointToPointNetDevice [class]
    module.add_class('PointToPointNetDevice', import_from_module='ns.point_to_point', parent=root_module['ns3::NetDevice'])
    ## pointer.h (module 'core'): ns3::PointerChecker [class]
    module.add_class('PointerChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## pointer.h (module 'core'): ns3::PointerValue [class]
    module.add_class('PointerValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## random-variable.h (module 'core'): ns3::RandomVariableChecker [class]
    module.add_class('RandomVariableChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## random-variable.h (module 'core'): ns3::RandomVariableValue [class]
    module.add_class('RandomVariableValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## string.h (module 'core'): ns3::StringChecker [class]
    module.add_class('StringChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
    ## string.h (module 'core'): ns3::StringValue [class]
    module.add_class('StringValue', import_from_module='ns.core', parent=root_module['ns3::AttributeValue'])
    ## stupid-interest-generator.h (module 'NDNabstraction'): ns3::StupidInterestGenerator [class]
    module.add_class('StupidInterestGenerator', parent=root_module['ns3::Application'])
    ## stupid-interest-sink.h (module 'NDNabstraction'): ns3::StupidInterestSink [class]
    module.add_class('StupidInterestSink', parent=root_module['ns3::Application'])
    ## nstime.h (module 'core'): ns3::TimeChecker [class]
    module.add_class('TimeChecker', import_from_module='ns.core', parent=root_module['ns3::AttributeChecker'])
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
    module.add_container('std::map< std::string, std::string >', ('std::string', 'std::string'), container_type='map')
    module.add_container('std::list< boost::reference_wrapper< std::string const > >', 'boost::reference_wrapper< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const >', container_type='list')
    module.add_container('std::list< std::string >', 'std::string', container_type='list')
    module.add_container('std::list< ns3::Ptr< ns3::Socket > >', 'ns3::Ptr< ns3::Socket >', container_type='list')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *', 'ns3::LogNodePrinter')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) **', 'ns3::LogNodePrinter*')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *&', 'ns3::LogNodePrinter&')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *', 'ns3::LogTimePrinter')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) **', 'ns3::LogTimePrinter*')
    typehandlers.add_type_alias('void ( * ) ( std::ostream & ) *&', 'ns3::LogTimePrinter&')
    
    ## Register a nested module for the namespace CcnbParser
    
    nested_module = module.add_cpp_namespace('CcnbParser')
    register_types_ns3_CcnbParser(nested_module)
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    

def register_types_ns3_CcnbParser(module):
    root_module = module.get_root()
    
    ## ccnb-parser-common.h (module 'NDNabstraction'): ns3::CcnbParser::ccn_dtag [enumeration]
    module.add_enum('ccn_dtag', ['CCN_DTAG_Any', 'CCN_DTAG_Name', 'CCN_DTAG_Component', 'CCN_DTAG_Certificate', 'CCN_DTAG_Collection', 'CCN_DTAG_CompleteName', 'CCN_DTAG_Content', 'CCN_DTAG_SignedInfo', 'CCN_DTAG_ContentDigest', 'CCN_DTAG_ContentHash', 'CCN_DTAG_Count', 'CCN_DTAG_Header', 'CCN_DTAG_Interest', 'CCN_DTAG_Key', 'CCN_DTAG_KeyLocator', 'CCN_DTAG_KeyName', 'CCN_DTAG_Length', 'CCN_DTAG_Link', 'CCN_DTAG_LinkAuthenticator', 'CCN_DTAG_NameComponentCount', 'CCN_DTAG_RootDigest', 'CCN_DTAG_Signature', 'CCN_DTAG_Start', 'CCN_DTAG_Timestamp', 'CCN_DTAG_Type', 'CCN_DTAG_Nonce', 'CCN_DTAG_Scope', 'CCN_DTAG_Exclude', 'CCN_DTAG_Bloom', 'CCN_DTAG_BloomSeed', 'CCN_DTAG_AnswerOriginKind', 'CCN_DTAG_InterestLifetime', 'CCN_DTAG_Witness', 'CCN_DTAG_SignatureBits', 'CCN_DTAG_DigestAlgorithm', 'CCN_DTAG_BlockSize', 'CCN_DTAG_FreshnessSeconds', 'CCN_DTAG_FinalBlockID', 'CCN_DTAG_PublisherPublicKeyDigest', 'CCN_DTAG_PublisherCertificateDigest', 'CCN_DTAG_PublisherIssuerKeyDigest', 'CCN_DTAG_PublisherIssuerCertificateDigest', 'CCN_DTAG_ContentObject', 'CCN_DTAG_WrappedKey', 'CCN_DTAG_WrappingKeyIdentifier', 'CCN_DTAG_WrapAlgorithm', 'CCN_DTAG_KeyAlgorithm', 'CCN_DTAG_Label', 'CCN_DTAG_EncryptedKey', 'CCN_DTAG_EncryptedNonceKey', 'CCN_DTAG_WrappingKeyName', 'CCN_DTAG_Action', 'CCN_DTAG_FaceID', 'CCN_DTAG_IPProto', 'CCN_DTAG_Host', 'CCN_DTAG_Port', 'CCN_DTAG_MulticastInterface', 'CCN_DTAG_ForwardingFlags', 'CCN_DTAG_FaceInstance', 'CCN_DTAG_ForwardingEntry', 'CCN_DTAG_MulticastTTL', 'CCN_DTAG_MinSuffixComponents', 'CCN_DTAG_MaxSuffixComponents', 'CCN_DTAG_ChildSelector', 'CCN_DTAG_RepositoryInfo', 'CCN_DTAG_Version', 'CCN_DTAG_RepositoryVersion', 'CCN_DTAG_GlobalPrefix', 'CCN_DTAG_LocalName', 'CCN_DTAG_Policy', 'CCN_DTAG_Namespace', 'CCN_DTAG_GlobalPrefixName', 'CCN_DTAG_PolicyVersion', 'CCN_DTAG_KeyValueSet', 'CCN_DTAG_KeyValuePair', 'CCN_DTAG_IntegerValue', 'CCN_DTAG_DecimalValue', 'CCN_DTAG_StringValue', 'CCN_DTAG_BinaryValue', 'CCN_DTAG_NameValue', 'CCN_DTAG_Entry', 'CCN_DTAG_ACL', 'CCN_DTAG_ParameterizedName', 'CCN_DTAG_Prefix', 'CCN_DTAG_Suffix', 'CCN_DTAG_Root', 'CCN_DTAG_ProfileName', 'CCN_DTAG_Parameters', 'CCN_DTAG_InfoString', 'CCN_DTAG_StatusResponse', 'CCN_DTAG_StatusCode', 'CCN_DTAG_StatusText', 'CCN_DTAG_SequenceNumber', 'CCN_DTAG_CCNProtocolDataUnit'])
    ## ccnb-parser-common.h (module 'NDNabstraction'): ns3::CcnbParser [enumeration]
    module.add_enum('', ['CCN_CLOSE'])
    ## ccnb-parser-common.h (module 'NDNabstraction'): ns3::CcnbParser::ccn_tt [enumeration]
    module.add_enum('ccn_tt', ['CCN_EXT', 'CCN_TAG', 'CCN_DTAG', 'CCN_ATTR', 'CCN_DATTR', 'CCN_BLOB', 'CCN_UDATA', 'CCN_NO_TOKEN'])
    ## ccnb-parser-block.h (module 'NDNabstraction'): ns3::CcnbParser::Block [class]
    module.add_class('Block', parent=root_module['ns3::SimpleRefCount< ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> >'])
    ## ccnb-parser-common.h (module 'NDNabstraction'): ns3::CcnbParser::CcnbDecodingException [class]
    module.add_class('CcnbDecodingException')
    ## ccnb-parser-ext.h (module 'NDNabstraction'): ns3::CcnbParser::Ext [class]
    module.add_class('Ext', parent=root_module['ns3::CcnbParser::Block'])
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NoArguVisitor [class]
    module.add_class('NoArguVisitor', allow_subclassing=True)
    ## ccnb-parser-udata.h (module 'NDNabstraction'): ns3::CcnbParser::Udata [class]
    module.add_class('Udata', parent=root_module['ns3::CcnbParser::Block'])
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::Visitor [class]
    module.add_class('Visitor', allow_subclassing=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidNoArguVisitor [class]
    module.add_class('VoidNoArguVisitor', allow_subclassing=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidVisitor [class]
    module.add_class('VoidVisitor', allow_subclassing=True)
    ## ccnb-parser-base-attr.h (module 'NDNabstraction'): ns3::CcnbParser::BaseAttr [class]
    module.add_class('BaseAttr', parent=root_module['ns3::CcnbParser::Block'])
    ## ccnb-parser-base-tag.h (module 'NDNabstraction'): ns3::CcnbParser::BaseTag [class]
    module.add_class('BaseTag', parent=root_module['ns3::CcnbParser::Block'])
    ## ccnb-parser-blob.h (module 'NDNabstraction'): ns3::CcnbParser::Blob [class]
    module.add_class('Blob', parent=root_module['ns3::CcnbParser::Block'])
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): ns3::CcnbParser::Dattr [class]
    module.add_class('Dattr', parent=root_module['ns3::CcnbParser::BaseAttr'])
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::DepthFirstVisitor [class]
    module.add_class('DepthFirstVisitor', parent=root_module['ns3::CcnbParser::Visitor'])
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): ns3::CcnbParser::Dtag [class]
    module.add_class('Dtag', parent=root_module['ns3::CcnbParser::BaseTag'])
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NoArguDepthFirstVisitor [class]
    module.add_class('NoArguDepthFirstVisitor', parent=root_module['ns3::CcnbParser::NoArguVisitor'])
    ## ccnb-parser-non-negative-integer-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NonNegativeIntegerVisitor [class]
    module.add_class('NonNegativeIntegerVisitor', parent=root_module['ns3::CcnbParser::NoArguDepthFirstVisitor'])
    ## ccnb-parser-nonce-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NonceVisitor [class]
    module.add_class('NonceVisitor', parent=root_module['ns3::CcnbParser::NoArguDepthFirstVisitor'])
    ## ccnb-parser-string-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::StringVisitor [class]
    module.add_class('StringVisitor', parent=root_module['ns3::CcnbParser::NoArguDepthFirstVisitor'])
    ## ccnb-parser-tag.h (module 'NDNabstraction'): ns3::CcnbParser::Tag [class]
    module.add_class('Tag', parent=root_module['ns3::CcnbParser::BaseTag'])
    ## ccnb-parser-timestamp-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::TimestampVisitor [class]
    module.add_class('TimestampVisitor', parent=root_module['ns3::CcnbParser::NoArguDepthFirstVisitor'])
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidDepthFirstVisitor [class]
    module.add_class('VoidDepthFirstVisitor', parent=root_module['ns3::CcnbParser::VoidVisitor'])
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidNoArguDepthFirstVisitor [class]
    module.add_class('VoidNoArguDepthFirstVisitor', parent=root_module['ns3::CcnbParser::VoidNoArguVisitor'])
    ## ccnb-parser-attr.h (module 'NDNabstraction'): ns3::CcnbParser::Attr [class]
    module.add_class('Attr', parent=root_module['ns3::CcnbParser::BaseAttr'])
    ## ccnb-parser-content-object-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::ContentObjectVisitor [class]
    module.add_class('ContentObjectVisitor', parent=root_module['ns3::CcnbParser::VoidDepthFirstVisitor'])
    ## ccnb-parser-interest-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::InterestVisitor [class]
    module.add_class('InterestVisitor', parent=root_module['ns3::CcnbParser::VoidDepthFirstVisitor'])
    ## ccnb-parser-name-components-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NameComponentsVisitor [class]
    module.add_class('NameComponentsVisitor', parent=root_module['ns3::CcnbParser::VoidDepthFirstVisitor'])
    module.add_container('std::list< ns3::Ptr< ns3::CcnbParser::Block > >', 'ns3::Ptr< ns3::CcnbParser::Block >', container_type='list')

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Address_methods(root_module, root_module['ns3::Address'])
    register_Ns3ApplicationContainer_methods(root_module, root_module['ns3::ApplicationContainer'])
    register_Ns3AsciiTraceHelper_methods(root_module, root_module['ns3::AsciiTraceHelper'])
    register_Ns3AsciiTraceHelperForCcnx_methods(root_module, root_module['ns3::AsciiTraceHelperForCcnx'])
    register_Ns3AsciiTraceHelperForDevice_methods(root_module, root_module['ns3::AsciiTraceHelperForDevice'])
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
    register_Ns3CcnxConsumerHelper_methods(root_module, root_module['ns3::CcnxConsumerHelper'])
    register_Ns3CcnxContentStoreContainer_methods(root_module, root_module['ns3::CcnxContentStoreContainer'])
    register_Ns3CcnxContentStoreEntry_methods(root_module, root_module['ns3::CcnxContentStoreEntry'])
    register_Ns3CcnxDecodingHelper_methods(root_module, root_module['ns3::CcnxDecodingHelper'])
    register_Ns3CcnxEncodingHelper_methods(root_module, root_module['ns3::CcnxEncodingHelper'])
    register_Ns3CcnxFib_methods(root_module, root_module['ns3::CcnxFib'])
    register_Ns3CcnxFibEntryContainer_methods(root_module, root_module['ns3::CcnxFibEntryContainer'])
    register_Ns3CcnxFibFaceMetric_methods(root_module, root_module['ns3::CcnxFibFaceMetric'])
    register_Ns3CcnxFibFaceMetricContainer_methods(root_module, root_module['ns3::CcnxFibFaceMetricContainer'])
    register_Ns3CcnxForwardingHelper_methods(root_module, root_module['ns3::CcnxForwardingHelper'])
    register_Ns3CcnxHeaderHelper_methods(root_module, root_module['ns3::CcnxHeaderHelper'])
    register_Ns3CcnxInterestSenderHelper_methods(root_module, root_module['ns3::CcnxInterestSenderHelper'])
    register_Ns3CcnxPitEntry_methods(root_module, root_module['ns3::CcnxPitEntry'])
    register_Ns3CcnxPitEntryAddIncoming_methods(root_module, root_module['ns3::CcnxPitEntry::AddIncoming'])
    register_Ns3CcnxPitEntryAddOutgoing_methods(root_module, root_module['ns3::CcnxPitEntry::AddOutgoing'])
    register_Ns3CcnxPitEntryClearIncoming_methods(root_module, root_module['ns3::CcnxPitEntry::ClearIncoming'])
    register_Ns3CcnxPitEntryDeleteIncoming_methods(root_module, root_module['ns3::CcnxPitEntry::DeleteIncoming'])
    register_Ns3CcnxPitEntrySetFibEntry_methods(root_module, root_module['ns3::CcnxPitEntry::SetFibEntry'])
    register_Ns3CcnxPitEntryContainer_methods(root_module, root_module['ns3::CcnxPitEntryContainer'])
    register_Ns3CcnxPitEntryIncomingFace_methods(root_module, root_module['ns3::CcnxPitEntryIncomingFace'])
    register_Ns3CcnxPitEntryIncomingFaceContainer_methods(root_module, root_module['ns3::CcnxPitEntryIncomingFaceContainer'])
    register_Ns3CcnxPitEntryNotFound_methods(root_module, root_module['ns3::CcnxPitEntryNotFound'])
    register_Ns3CcnxPitEntryOutgoingFace_methods(root_module, root_module['ns3::CcnxPitEntryOutgoingFace'])
    register_Ns3CcnxPitEntryOutgoingFaceContainer_methods(root_module, root_module['ns3::CcnxPitEntryOutgoingFaceContainer'])
    register_Ns3CcnxProducerHelper_methods(root_module, root_module['ns3::CcnxProducerHelper'])
    register_Ns3CcnxRitContainer_methods(root_module, root_module['ns3::CcnxRitContainer'])
    register_Ns3CcnxRitEntry_methods(root_module, root_module['ns3::CcnxRitEntry'])
    register_Ns3CcnxUnknownHeaderException_methods(root_module, root_module['ns3::CcnxUnknownHeaderException'])
    register_Ns3DataRate_methods(root_module, root_module['ns3::DataRate'])
    register_Ns3EventId_methods(root_module, root_module['ns3::EventId'])
    register_Ns3Ipv4Address_methods(root_module, root_module['ns3::Ipv4Address'])
    register_Ns3Ipv4Mask_methods(root_module, root_module['ns3::Ipv4Mask'])
    register_Ns3Ipv6Address_methods(root_module, root_module['ns3::Ipv6Address'])
    register_Ns3Ipv6Prefix_methods(root_module, root_module['ns3::Ipv6Prefix'])
    register_Ns3LogComponent_methods(root_module, root_module['ns3::LogComponent'])
    register_Ns3Mac48Address_methods(root_module, root_module['ns3::Mac48Address'])
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
    register_Ns3PcapFile_methods(root_module, root_module['ns3::PcapFile'])
    register_Ns3PcapHelper_methods(root_module, root_module['ns3::PcapHelper'])
    register_Ns3PcapHelperForCcnx_methods(root_module, root_module['ns3::PcapHelperForCcnx'])
    register_Ns3PcapHelperForDevice_methods(root_module, root_module['ns3::PcapHelperForDevice'])
    register_Ns3PointToPointHelper_methods(root_module, root_module['ns3::PointToPointHelper'])
    register_Ns3RandomVariable_methods(root_module, root_module['ns3::RandomVariable'])
    register_Ns3SeedManager_methods(root_module, root_module['ns3::SeedManager'])
    register_Ns3SequentialVariable_methods(root_module, root_module['ns3::SequentialVariable'])
    register_Ns3SimpleRefCount__Ns3Chunk_Ns3ObjectBase_Ns3DefaultDeleter__lt__ns3Chunk__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> >'])
    register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    register_Ns3Simulator_methods(root_module, root_module['ns3::Simulator'])
    register_Ns3StupidInterestGeneratorHelper_methods(root_module, root_module['ns3::StupidInterestGeneratorHelper'])
    register_Ns3Tag_methods(root_module, root_module['ns3::Tag'])
    register_Ns3TagBuffer_methods(root_module, root_module['ns3::TagBuffer'])
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
    register_Ns3CcnxStackHelper_methods(root_module, root_module['ns3::CcnxStackHelper'])
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
    register_Ns3PcapFileWrapper_methods(root_module, root_module['ns3::PcapFileWrapper'])
    register_Ns3Queue_methods(root_module, root_module['ns3::Queue'])
    register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    register_Ns3SimpleRefCount__Ns3CcnbParserBlock_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnbParserBlock__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> >'])
    register_Ns3SimpleRefCount__Ns3CcnxFaceContainer_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxFaceContainer__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> >'])
    register_Ns3SimpleRefCount__Ns3CcnxFibEntry_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxFibEntry__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> >'])
    register_Ns3SimpleRefCount__Ns3CcnxNameComponents_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxNameComponents__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> >'])
    register_Ns3SimpleRefCount__Ns3CcnxRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxRoute__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> >'])
    register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    register_Ns3SimpleRefCount__Ns3OutputStreamWrapper_Ns3Empty_Ns3DefaultDeleter__lt__ns3OutputStreamWrapper__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    register_Ns3Socket_methods(root_module, root_module['ns3::Socket'])
    register_Ns3SocketAddressTag_methods(root_module, root_module['ns3::SocketAddressTag'])
    register_Ns3SocketFactory_methods(root_module, root_module['ns3::SocketFactory'])
    register_Ns3SocketIpTtlTag_methods(root_module, root_module['ns3::SocketIpTtlTag'])
    register_Ns3SocketSetDontFragmentTag_methods(root_module, root_module['ns3::SocketSetDontFragmentTag'])
    register_Ns3Time_methods(root_module, root_module['ns3::Time'])
    register_Ns3TopologyReader_methods(root_module, root_module['ns3::TopologyReader'])
    register_Ns3TopologyReaderLink_methods(root_module, root_module['ns3::TopologyReader::Link'])
    register_Ns3TraceSourceAccessor_methods(root_module, root_module['ns3::TraceSourceAccessor'])
    register_Ns3Trailer_methods(root_module, root_module['ns3::Trailer'])
    register_Ns3UdpSocketFactory_methods(root_module, root_module['ns3::UdpSocketFactory'])
    register_Ns3AnnotatedTopologyReader_methods(root_module, root_module['ns3::AnnotatedTopologyReader'])
    register_Ns3Application_methods(root_module, root_module['ns3::Application'])
    register_Ns3AttributeAccessor_methods(root_module, root_module['ns3::AttributeAccessor'])
    register_Ns3AttributeChecker_methods(root_module, root_module['ns3::AttributeChecker'])
    register_Ns3AttributeValue_methods(root_module, root_module['ns3::AttributeValue'])
    register_Ns3BooleanChecker_methods(root_module, root_module['ns3::BooleanChecker'])
    register_Ns3BooleanValue_methods(root_module, root_module['ns3::BooleanValue'])
    register_Ns3CallbackChecker_methods(root_module, root_module['ns3::CallbackChecker'])
    register_Ns3CallbackImplBase_methods(root_module, root_module['ns3::CallbackImplBase'])
    register_Ns3CallbackValue_methods(root_module, root_module['ns3::CallbackValue'])
    register_Ns3Ccnx_methods(root_module, root_module['ns3::Ccnx'])
    register_Ns3CcnxConsumer_methods(root_module, root_module['ns3::CcnxConsumer'])
    register_Ns3CcnxContentObjectHeader_methods(root_module, root_module['ns3::CcnxContentObjectHeader'])
    register_Ns3CcnxContentObjectTail_methods(root_module, root_module['ns3::CcnxContentObjectTail'])
    register_Ns3CcnxContentStore_methods(root_module, root_module['ns3::CcnxContentStore'])
    register_Ns3CcnxFace_methods(root_module, root_module['ns3::CcnxFace'])
    register_Ns3CcnxFaceContainer_methods(root_module, root_module['ns3::CcnxFaceContainer'])
    register_Ns3CcnxFibEntry_methods(root_module, root_module['ns3::CcnxFibEntry'])
    register_Ns3CcnxForwardingStrategy_methods(root_module, root_module['ns3::CcnxForwardingStrategy'])
    register_Ns3CcnxInterestHeader_methods(root_module, root_module['ns3::CcnxInterestHeader'])
    register_Ns3CcnxInterestSender_methods(root_module, root_module['ns3::CcnxInterestSender'])
    register_Ns3CcnxL3Protocol_methods(root_module, root_module['ns3::CcnxL3Protocol'])
    register_Ns3CcnxLocalFace_methods(root_module, root_module['ns3::CcnxLocalFace'])
    register_Ns3CcnxNameComponents_methods(root_module, root_module['ns3::CcnxNameComponents'])
    register_Ns3CcnxNameComponentsChecker_methods(root_module, root_module['ns3::CcnxNameComponentsChecker'])
    register_Ns3CcnxNameComponentsValue_methods(root_module, root_module['ns3::CcnxNameComponentsValue'])
    register_Ns3CcnxNetDeviceFace_methods(root_module, root_module['ns3::CcnxNetDeviceFace'])
    register_Ns3CcnxPit_methods(root_module, root_module['ns3::CcnxPit'])
    register_Ns3CcnxProducer_methods(root_module, root_module['ns3::CcnxProducer'])
    register_Ns3CcnxRit_methods(root_module, root_module['ns3::CcnxRit'])
    register_Ns3CcnxRoute_methods(root_module, root_module['ns3::CcnxRoute'])
    register_Ns3DataRateChecker_methods(root_module, root_module['ns3::DataRateChecker'])
    register_Ns3DataRateValue_methods(root_module, root_module['ns3::DataRateValue'])
    register_Ns3DropTailQueue_methods(root_module, root_module['ns3::DropTailQueue'])
    register_Ns3EmptyAttributeValue_methods(root_module, root_module['ns3::EmptyAttributeValue'])
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
    register_Ns3Mac48AddressChecker_methods(root_module, root_module['ns3::Mac48AddressChecker'])
    register_Ns3Mac48AddressValue_methods(root_module, root_module['ns3::Mac48AddressValue'])
    register_Ns3NetDevice_methods(root_module, root_module['ns3::NetDevice'])
    register_Ns3NixVector_methods(root_module, root_module['ns3::NixVector'])
    register_Ns3Node_methods(root_module, root_module['ns3::Node'])
    register_Ns3ObjectFactoryChecker_methods(root_module, root_module['ns3::ObjectFactoryChecker'])
    register_Ns3ObjectFactoryValue_methods(root_module, root_module['ns3::ObjectFactoryValue'])
    register_Ns3OutputStreamWrapper_methods(root_module, root_module['ns3::OutputStreamWrapper'])
    register_Ns3Packet_methods(root_module, root_module['ns3::Packet'])
    register_Ns3PointToPointNetDevice_methods(root_module, root_module['ns3::PointToPointNetDevice'])
    register_Ns3PointerChecker_methods(root_module, root_module['ns3::PointerChecker'])
    register_Ns3PointerValue_methods(root_module, root_module['ns3::PointerValue'])
    register_Ns3RandomVariableChecker_methods(root_module, root_module['ns3::RandomVariableChecker'])
    register_Ns3RandomVariableValue_methods(root_module, root_module['ns3::RandomVariableValue'])
    register_Ns3StringChecker_methods(root_module, root_module['ns3::StringChecker'])
    register_Ns3StringValue_methods(root_module, root_module['ns3::StringValue'])
    register_Ns3StupidInterestGenerator_methods(root_module, root_module['ns3::StupidInterestGenerator'])
    register_Ns3StupidInterestSink_methods(root_module, root_module['ns3::StupidInterestSink'])
    register_Ns3TimeChecker_methods(root_module, root_module['ns3::TimeChecker'])
    register_Ns3TimeValue_methods(root_module, root_module['ns3::TimeValue'])
    register_Ns3TypeIdChecker_methods(root_module, root_module['ns3::TypeIdChecker'])
    register_Ns3TypeIdValue_methods(root_module, root_module['ns3::TypeIdValue'])
    register_Ns3UintegerValue_methods(root_module, root_module['ns3::UintegerValue'])
    register_Ns3AddressChecker_methods(root_module, root_module['ns3::AddressChecker'])
    register_Ns3AddressValue_methods(root_module, root_module['ns3::AddressValue'])
    register_Ns3CcnbParserBlock_methods(root_module, root_module['ns3::CcnbParser::Block'])
    register_Ns3CcnbParserCcnbDecodingException_methods(root_module, root_module['ns3::CcnbParser::CcnbDecodingException'])
    register_Ns3CcnbParserExt_methods(root_module, root_module['ns3::CcnbParser::Ext'])
    register_Ns3CcnbParserNoArguVisitor_methods(root_module, root_module['ns3::CcnbParser::NoArguVisitor'])
    register_Ns3CcnbParserUdata_methods(root_module, root_module['ns3::CcnbParser::Udata'])
    register_Ns3CcnbParserVisitor_methods(root_module, root_module['ns3::CcnbParser::Visitor'])
    register_Ns3CcnbParserVoidNoArguVisitor_methods(root_module, root_module['ns3::CcnbParser::VoidNoArguVisitor'])
    register_Ns3CcnbParserVoidVisitor_methods(root_module, root_module['ns3::CcnbParser::VoidVisitor'])
    register_Ns3CcnbParserBaseAttr_methods(root_module, root_module['ns3::CcnbParser::BaseAttr'])
    register_Ns3CcnbParserBaseTag_methods(root_module, root_module['ns3::CcnbParser::BaseTag'])
    register_Ns3CcnbParserBlob_methods(root_module, root_module['ns3::CcnbParser::Blob'])
    register_Ns3CcnbParserDattr_methods(root_module, root_module['ns3::CcnbParser::Dattr'])
    register_Ns3CcnbParserDepthFirstVisitor_methods(root_module, root_module['ns3::CcnbParser::DepthFirstVisitor'])
    register_Ns3CcnbParserDtag_methods(root_module, root_module['ns3::CcnbParser::Dtag'])
    register_Ns3CcnbParserNoArguDepthFirstVisitor_methods(root_module, root_module['ns3::CcnbParser::NoArguDepthFirstVisitor'])
    register_Ns3CcnbParserNonNegativeIntegerVisitor_methods(root_module, root_module['ns3::CcnbParser::NonNegativeIntegerVisitor'])
    register_Ns3CcnbParserNonceVisitor_methods(root_module, root_module['ns3::CcnbParser::NonceVisitor'])
    register_Ns3CcnbParserStringVisitor_methods(root_module, root_module['ns3::CcnbParser::StringVisitor'])
    register_Ns3CcnbParserTag_methods(root_module, root_module['ns3::CcnbParser::Tag'])
    register_Ns3CcnbParserTimestampVisitor_methods(root_module, root_module['ns3::CcnbParser::TimestampVisitor'])
    register_Ns3CcnbParserVoidDepthFirstVisitor_methods(root_module, root_module['ns3::CcnbParser::VoidDepthFirstVisitor'])
    register_Ns3CcnbParserVoidNoArguDepthFirstVisitor_methods(root_module, root_module['ns3::CcnbParser::VoidNoArguDepthFirstVisitor'])
    register_Ns3CcnbParserAttr_methods(root_module, root_module['ns3::CcnbParser::Attr'])
    register_Ns3CcnbParserContentObjectVisitor_methods(root_module, root_module['ns3::CcnbParser::ContentObjectVisitor'])
    register_Ns3CcnbParserInterestVisitor_methods(root_module, root_module['ns3::CcnbParser::InterestVisitor'])
    register_Ns3CcnbParserNameComponentsVisitor_methods(root_module, root_module['ns3::CcnbParser::NameComponentsVisitor'])
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

def register_Ns3AsciiTraceHelper_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelper::AsciiTraceHelper(ns3::AsciiTraceHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelper const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelper::AsciiTraceHelper() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): ns3::Ptr<ns3::OutputStreamWrapper> ns3::AsciiTraceHelper::CreateFileStream(std::string filename, std::_Ios_Openmode filemode=std::ios_base::out) [member function]
    cls.add_method('CreateFileStream', 
                   'ns3::Ptr< ns3::OutputStreamWrapper >', 
                   [param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode', default_value='std::ios_base::out')])
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDequeueSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDequeueSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDequeueSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDequeueSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDropSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDropSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultDropSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultDropSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultEnqueueSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultEnqueueSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultEnqueueSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultEnqueueSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultReceiveSinkWithContext(ns3::Ptr<ns3::OutputStreamWrapper> file, std::string context, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultReceiveSinkWithContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('std::string', 'context'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): static void ns3::AsciiTraceHelper::DefaultReceiveSinkWithoutContext(ns3::Ptr<ns3::OutputStreamWrapper> file, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('DefaultReceiveSinkWithoutContext', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'file'), param('ns3::Ptr< ns3::Packet const >', 'p')], 
                   is_static=True)
    ## trace-helper.h (module 'network'): std::string ns3::AsciiTraceHelper::GetFilenameFromDevice(std::string prefix, ns3::Ptr<ns3::NetDevice> device, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromDevice', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'useObjectNames', default_value='true')])
    ## trace-helper.h (module 'network'): std::string ns3::AsciiTraceHelper::GetFilenameFromInterfacePair(std::string prefix, ns3::Ptr<ns3::Object> object, uint32_t interface, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromInterfacePair', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Object >', 'object'), param('uint32_t', 'interface'), param('bool', 'useObjectNames', default_value='true')])
    return

def register_Ns3AsciiTraceHelperForCcnx_methods(root_module, cls):
    ## ccnx-trace-helper.h (module 'NDNabstraction'): ns3::AsciiTraceHelperForCcnx::AsciiTraceHelperForCcnx(ns3::AsciiTraceHelperForCcnx const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelperForCcnx const &', 'arg0')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): ns3::AsciiTraceHelperForCcnx::AsciiTraceHelperForCcnx() [constructor]
    cls.add_constructor([])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(std::string prefix, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface, bool explicitFilename=false) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface'), param('bool', 'explicitFilename', default_value='false')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(std::string prefix, std::string ccnxName, uint32_t interface, bool explicitFilename=false) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ccnxName'), param('uint32_t', 'interface'), param('bool', 'explicitFilename', default_value='false')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string ccnxName, uint32_t interface) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'ccnxName'), param('uint32_t', 'interface')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(std::string prefix, ns3::CcnxFaceContainer c) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::CcnxFaceContainer', 'c')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::CcnxFaceContainer c) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::CcnxFaceContainer', 'c')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(std::string prefix, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NodeContainer', 'n')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'explicitFilename')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnx(ns3::Ptr<ns3::OutputStreamWrapper> stream, uint32_t nodeid, uint32_t interface, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiCcnx', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('uint32_t', 'nodeid'), param('uint32_t', 'interface'), param('bool', 'explicitFilename')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnxAll(std::string prefix) [member function]
    cls.add_method('EnableAsciiCcnxAll', 
                   'void', 
                   [param('std::string', 'prefix')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnxAll(ns3::Ptr<ns3::OutputStreamWrapper> stream) [member function]
    cls.add_method('EnableAsciiCcnxAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::AsciiTraceHelperForCcnx::EnableAsciiCcnxInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiCcnxInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3AsciiTraceHelperForDevice_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelperForDevice::AsciiTraceHelperForDevice(ns3::AsciiTraceHelperForDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AsciiTraceHelperForDevice const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::AsciiTraceHelperForDevice::AsciiTraceHelperForDevice() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename=false) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::Ptr<ns3::NetDevice> nd) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::Ptr< ns3::NetDevice >', 'nd')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, std::string ndName, bool explicitFilename=false) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ndName'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string ndName) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'ndName')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NetDeviceContainer', 'd')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NetDeviceContainer d) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NetDeviceContainer', 'd')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, ns3::NodeContainer n) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('ns3::NodeContainer', 'n')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool explicitFilename) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'explicitFilename')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAscii(ns3::Ptr<ns3::OutputStreamWrapper> stream, uint32_t nodeid, uint32_t deviceid) [member function]
    cls.add_method('EnableAscii', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAsciiAll(std::string prefix) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('std::string', 'prefix')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAsciiAll(ns3::Ptr<ns3::OutputStreamWrapper> stream) [member function]
    cls.add_method('EnableAsciiAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')])
    ## trace-helper.h (module 'network'): void ns3::AsciiTraceHelperForDevice::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
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

def register_Ns3CcnxConsumerHelper_methods(root_module, cls):
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): ns3::CcnxConsumerHelper::CcnxConsumerHelper(ns3::CcnxConsumerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxConsumerHelper const &', 'arg0')])
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): ns3::CcnxConsumerHelper::CcnxConsumerHelper(ns3::Ptr<ns3::CcnxNameComponents> interestName) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxNameComponents >', 'interestName')])
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxConsumerHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxConsumerHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxConsumerHelper::Install(std::string nodeName) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')])
    ## ccnx-consumer-helper.h (module 'NDNabstraction'): void ns3::CcnxConsumerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3CcnxContentStoreContainer_methods(root_module, cls):
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStoreContainer::CcnxContentStoreContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStoreContainer::CcnxContentStoreContainer(ns3::CcnxContentStoreContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxContentStoreContainer const &', 'arg0')])
    return

def register_Ns3CcnxContentStoreEntry_methods(root_module, cls):
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStoreEntry::CcnxContentStoreEntry(ns3::CcnxContentStoreEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxContentStoreEntry const &', 'arg0')])
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStoreEntry::CcnxContentStoreEntry(ns3::Ptr<ns3::CcnxContentObjectHeader> header, ns3::Ptr<ns3::Packet const> packet) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxContentObjectHeader >', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::Ptr<ns3::Packet> ns3::CcnxContentStoreEntry::GetFullyFormedCcnxPacket() const [member function]
    cls.add_method('GetFullyFormedCcnxPacket', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_const=True)
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxContentObjectHeader const> ns3::CcnxContentStoreEntry::GetHeader() const [member function]
    cls.add_method('GetHeader', 
                   'ns3::Ptr< ns3::CcnxContentObjectHeader const >', 
                   [], 
                   is_const=True)
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxContentStoreEntry::GetName() const [member function]
    cls.add_method('GetName', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::Ptr<ns3::Packet const> ns3::CcnxContentStoreEntry::GetPacket() const [member function]
    cls.add_method('GetPacket', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    return

def register_Ns3CcnxDecodingHelper_methods(root_module, cls):
    ## ccnx-decoding-helper.h (module 'NDNabstraction'): ns3::CcnxDecodingHelper::CcnxDecodingHelper() [constructor]
    cls.add_constructor([])
    ## ccnx-decoding-helper.h (module 'NDNabstraction'): ns3::CcnxDecodingHelper::CcnxDecodingHelper(ns3::CcnxDecodingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxDecodingHelper const &', 'arg0')])
    ## ccnx-decoding-helper.h (module 'NDNabstraction'): static size_t ns3::CcnxDecodingHelper::Deserialize(ns3::Buffer::Iterator start, ns3::CcnxInterestHeader const & interest) [member function]
    cls.add_method('Deserialize', 
                   'size_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::CcnxInterestHeader const &', 'interest')], 
                   is_static=True)
    ## ccnx-decoding-helper.h (module 'NDNabstraction'): static size_t ns3::CcnxDecodingHelper::Deserialize(ns3::Buffer::Iterator start, ns3::CcnxContentObjectHeader const & contentObject) [member function]
    cls.add_method('Deserialize', 
                   'size_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::CcnxContentObjectHeader const &', 'contentObject')], 
                   is_static=True)
    return

def register_Ns3CcnxEncodingHelper_methods(root_module, cls):
    ## ccnx-encoding-helper.h (module 'NDNabstraction'): ns3::CcnxEncodingHelper::CcnxEncodingHelper() [constructor]
    cls.add_constructor([])
    ## ccnx-encoding-helper.h (module 'NDNabstraction'): ns3::CcnxEncodingHelper::CcnxEncodingHelper(ns3::CcnxEncodingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxEncodingHelper const &', 'arg0')])
    ## ccnx-encoding-helper.h (module 'NDNabstraction'): static size_t ns3::CcnxEncodingHelper::Serialize(ns3::Buffer::Iterator start, ns3::CcnxInterestHeader const & interest) [member function]
    cls.add_method('Serialize', 
                   'size_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::CcnxInterestHeader const &', 'interest')], 
                   is_static=True)
    ## ccnx-encoding-helper.h (module 'NDNabstraction'): static size_t ns3::CcnxEncodingHelper::Serialize(ns3::Buffer::Iterator start, ns3::CcnxContentObjectHeader const & contentObject) [member function]
    cls.add_method('Serialize', 
                   'size_t', 
                   [param('ns3::Buffer::Iterator', 'start'), param('ns3::CcnxContentObjectHeader const &', 'contentObject')], 
                   is_static=True)
    return

def register_Ns3CcnxFib_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFib::CcnxFib(ns3::CcnxFib const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxFib const &', 'arg0')])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFib::CcnxFib(ns3::Ptr<ns3::Ccnx> node) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Ccnx >', 'node')])
    ## ccnx-fib.h (module 'NDNabstraction'): std::pair<boost::multi_index::detail::hashed_index_iterator<boost::multi_index::detail::hashed_index_node<boost::multi_index::detail::index_node_base<ns3::CcnxFibEntry, std::allocator<ns3::CcnxFibEntry> > >, boost::multi_index::detail::bucket_array<std::allocator<ns3::CcnxFibEntry> > >,bool> ns3::CcnxFib::LongestPrefixMatch(ns3::CcnxInterestHeader const & interest) const [member function]
    cls.add_method('LongestPrefixMatch', 
                   'std::pair< boost::multi_index::detail::hashed_index_iterator< boost::multi_index::detail::hashed_index_node< boost::multi_index::detail::index_node_base< ns3::CcnxFibEntry, std::allocator< ns3::CcnxFibEntry > > >, boost::multi_index::detail::bucket_array< std::allocator< ns3::CcnxFibEntry > > >, bool >', 
                   [param('ns3::CcnxInterestHeader const &', 'interest')], 
                   is_const=True)
    return

def register_Ns3CcnxFibEntryContainer_methods(root_module, cls):
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibEntryContainer::CcnxFibEntryContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibEntryContainer::CcnxFibEntryContainer(ns3::CcnxFibEntryContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxFibEntryContainer const &', 'arg0')])
    return

def register_Ns3CcnxFibFaceMetric_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<')
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::CcnxFibFaceMetric(ns3::CcnxFibFaceMetric const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxFibFaceMetric const &', 'arg0')])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::CcnxFibFaceMetric(ns3::Ptr<ns3::CcnxFace> face, int cost) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFace >', 'face'), param('int', 'cost')])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFace> ns3::CcnxFibFaceMetric::GetFace() const [member function]
    cls.add_method('GetFace', 
                   'ns3::Ptr< ns3::CcnxFace >', 
                   [], 
                   is_const=True)
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::m_face [variable]
    cls.add_instance_attribute('m_face', 'ns3::Ptr< ns3::CcnxFace >', is_const=False)
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::m_routingCost [variable]
    cls.add_instance_attribute('m_routingCost', 'uint32_t', is_const=False)
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::m_rttVar [variable]
    cls.add_instance_attribute('m_rttVar', 'ns3::Time', is_const=False)
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::m_sRtt [variable]
    cls.add_instance_attribute('m_sRtt', 'ns3::Time', is_const=False)
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetric::m_status [variable]
    cls.add_instance_attribute('m_status', 'uint8_t', is_const=False)
    return

def register_Ns3CcnxFibFaceMetricContainer_methods(root_module, cls):
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetricContainer::CcnxFibFaceMetricContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibFaceMetricContainer::CcnxFibFaceMetricContainer(ns3::CcnxFibFaceMetricContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxFibFaceMetricContainer const &', 'arg0')])
    return

def register_Ns3CcnxForwardingHelper_methods(root_module, cls):
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): ns3::CcnxForwardingHelper::CcnxForwardingHelper() [constructor]
    cls.add_constructor([])
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): ns3::CcnxForwardingHelper::CcnxForwardingHelper(ns3::CcnxForwardingHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxForwardingHelper const &', 'arg0')])
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): ns3::CcnxForwardingHelper * ns3::CcnxForwardingHelper::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::CcnxForwardingHelper *', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxForwardingStrategy> ns3::CcnxForwardingHelper::Create(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::CcnxForwardingStrategy >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): void ns3::CcnxForwardingHelper::PrintForwardingTableAllAt(ns3::Time printTime, ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintForwardingTableAllAt', 
                   'void', 
                   [param('ns3::Time', 'printTime'), param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): void ns3::CcnxForwardingHelper::PrintForwardingTableAllEvery(ns3::Time printInterval, ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintForwardingTableAllEvery', 
                   'void', 
                   [param('ns3::Time', 'printInterval'), param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): void ns3::CcnxForwardingHelper::PrintForwardingTableAt(ns3::Time printTime, ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintForwardingTableAt', 
                   'void', 
                   [param('ns3::Time', 'printTime'), param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    ## ccnx-forwarding-helper.h (module 'NDNabstraction'): void ns3::CcnxForwardingHelper::PrintForwardingTableEvery(ns3::Time printInterval, ns3::Ptr<ns3::Node> node, ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintForwardingTableEvery', 
                   'void', 
                   [param('ns3::Time', 'printInterval'), param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_const=True)
    return

def register_Ns3CcnxHeaderHelper_methods(root_module, cls):
    ## ccnx-header-helper.h (module 'NDNabstraction'): ns3::CcnxHeaderHelper::CcnxHeaderHelper() [constructor]
    cls.add_constructor([])
    ## ccnx-header-helper.h (module 'NDNabstraction'): ns3::CcnxHeaderHelper::CcnxHeaderHelper(ns3::CcnxHeaderHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxHeaderHelper const &', 'arg0')])
    ## ccnx-header-helper.h (module 'NDNabstraction'): static ns3::Ptr<ns3::Header> ns3::CcnxHeaderHelper::CreateCorrectCcnxHeader(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('CreateCorrectCcnxHeader', 
                   'ns3::Ptr< ns3::Header >', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_static=True)
    return

def register_Ns3CcnxInterestSenderHelper_methods(root_module, cls):
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): ns3::CcnxInterestSenderHelper::CcnxInterestSenderHelper(ns3::CcnxInterestSenderHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxInterestSenderHelper const &', 'arg0')])
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): ns3::CcnxInterestSenderHelper::CcnxInterestSenderHelper(ns3::Ptr<ns3::CcnxNameComponents> interestName) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxNameComponents >', 'interestName')])
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxInterestSenderHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxInterestSenderHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxInterestSenderHelper::Install(std::string nodeName) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')])
    ## ccnx-interest-sender-helper.h (module 'NDNabstraction'): void ns3::CcnxInterestSenderHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3CcnxPitEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::CcnxPitEntry(ns3::CcnxPitEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntry const &', 'arg0')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::CcnxPitEntry(ns3::Ptr<ns3::CcnxNameComponents> prefix) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxNameComponents >', 'prefix')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::Time const & ns3::CcnxPitEntry::GetExpireTime() const [member function]
    cls.add_method('GetExpireTime', 
                   'ns3::Time const &', 
                   [], 
                   is_const=True)
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxPitEntry::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    return

def register_Ns3CcnxPitEntryAddIncoming_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::AddIncoming::AddIncoming(ns3::CcnxPitEntry::AddIncoming const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntry::AddIncoming const &', 'arg0')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::AddIncoming::AddIncoming(ns3::Ptr<ns3::CcnxFace> incomingFace) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFace >', 'incomingFace')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): void ns3::CcnxPitEntry::AddIncoming::operator()(ns3::CcnxPitEntry & entry) [member operator]
    cls.add_method('operator()', 
                   'void', 
                   [param('ns3::CcnxPitEntry &', 'entry')], 
                   custom_name='__call__')
    return

def register_Ns3CcnxPitEntryAddOutgoing_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::AddOutgoing::AddOutgoing(ns3::CcnxPitEntry::AddOutgoing const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntry::AddOutgoing const &', 'arg0')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::AddOutgoing::AddOutgoing(ns3::Ptr<ns3::CcnxFace> outgoingFace) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFace >', 'outgoingFace')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): void ns3::CcnxPitEntry::AddOutgoing::operator()(ns3::CcnxPitEntry & entry) [member operator]
    cls.add_method('operator()', 
                   'void', 
                   [param('ns3::CcnxPitEntry &', 'entry')], 
                   custom_name='__call__')
    return

def register_Ns3CcnxPitEntryClearIncoming_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::ClearIncoming::ClearIncoming(ns3::CcnxPitEntry::ClearIncoming const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntry::ClearIncoming const &', 'arg0')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::ClearIncoming::ClearIncoming() [constructor]
    cls.add_constructor([])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): void ns3::CcnxPitEntry::ClearIncoming::operator()(ns3::CcnxPitEntry & entry) [member operator]
    cls.add_method('operator()', 
                   'void', 
                   [param('ns3::CcnxPitEntry &', 'entry')], 
                   custom_name='__call__')
    return

def register_Ns3CcnxPitEntryDeleteIncoming_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::DeleteIncoming::DeleteIncoming(ns3::CcnxPitEntry::DeleteIncoming const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntry::DeleteIncoming const &', 'arg0')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::DeleteIncoming::DeleteIncoming(ns3::Ptr<ns3::CcnxFace> face) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFace >', 'face')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): void ns3::CcnxPitEntry::DeleteIncoming::operator()(ns3::CcnxPitEntry & entry) [member operator]
    cls.add_method('operator()', 
                   'void', 
                   [param('ns3::CcnxPitEntry &', 'entry')], 
                   custom_name='__call__')
    return

def register_Ns3CcnxPitEntrySetFibEntry_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::SetFibEntry::SetFibEntry(ns3::CcnxPitEntry::SetFibEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntry::SetFibEntry const &', 'arg0')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntry::SetFibEntry::SetFibEntry(ns3::Ptr<ns3::CcnxFibEntry> fib) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFibEntry >', 'fib')])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): void ns3::CcnxPitEntry::SetFibEntry::operator()(ns3::CcnxPitEntry & entry) [member operator]
    cls.add_method('operator()', 
                   'void', 
                   [param('ns3::CcnxPitEntry &', 'entry')], 
                   custom_name='__call__')
    return

def register_Ns3CcnxPitEntryContainer_methods(root_module, cls):
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntryContainer::CcnxPitEntryContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntryContainer::CcnxPitEntryContainer(ns3::CcnxPitEntryContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntryContainer const &', 'arg0')])
    return

def register_Ns3CcnxPitEntryIncomingFace_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ccnx-pit-entry-incoming-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFace::CcnxPitEntryIncomingFace(ns3::CcnxPitEntryIncomingFace const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntryIncomingFace const &', 'arg0')])
    ## ccnx-pit-entry-incoming-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFace::CcnxPitEntryIncomingFace(ns3::Ptr<ns3::CcnxFace> face) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFace >', 'face')])
    ## ccnx-pit-entry-incoming-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFace::m_arrivalTime [variable]
    cls.add_instance_attribute('m_arrivalTime', 'ns3::Time', is_const=False)
    ## ccnx-pit-entry-incoming-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFace::m_face [variable]
    cls.add_instance_attribute('m_face', 'ns3::Ptr< ns3::CcnxFace >', is_const=False)
    return

def register_Ns3CcnxPitEntryIncomingFaceContainer_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFaceContainer::CcnxPitEntryIncomingFaceContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntryIncomingFaceContainer::CcnxPitEntryIncomingFaceContainer(ns3::CcnxPitEntryIncomingFaceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntryIncomingFaceContainer const &', 'arg0')])
    return

def register_Ns3CcnxPitEntryNotFound_methods(root_module, cls):
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntryNotFound::CcnxPitEntryNotFound() [constructor]
    cls.add_constructor([])
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntryNotFound::CcnxPitEntryNotFound(ns3::CcnxPitEntryNotFound const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntryNotFound const &', 'arg0')])
    return

def register_Ns3CcnxPitEntryOutgoingFace_methods(root_module, cls):
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## ccnx-pit-entry-outgoing-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFace::CcnxPitEntryOutgoingFace(ns3::CcnxPitEntryOutgoingFace const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntryOutgoingFace const &', 'arg0')])
    ## ccnx-pit-entry-outgoing-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFace::CcnxPitEntryOutgoingFace(ns3::Ptr<ns3::CcnxFace> face) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxFace >', 'face')])
    ## ccnx-pit-entry-outgoing-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFace::m_face [variable]
    cls.add_instance_attribute('m_face', 'ns3::Ptr< ns3::CcnxFace >', is_const=False)
    ## ccnx-pit-entry-outgoing-face.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFace::m_sendTime [variable]
    cls.add_instance_attribute('m_sendTime', 'ns3::Time', is_const=False)
    return

def register_Ns3CcnxPitEntryOutgoingFaceContainer_methods(root_module, cls):
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFaceContainer::CcnxPitEntryOutgoingFaceContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-pit-entry.h (module 'NDNabstraction'): ns3::CcnxPitEntryOutgoingFaceContainer::CcnxPitEntryOutgoingFaceContainer(ns3::CcnxPitEntryOutgoingFaceContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPitEntryOutgoingFaceContainer const &', 'arg0')])
    return

def register_Ns3CcnxProducerHelper_methods(root_module, cls):
    ## ccnx-producer-helper.h (module 'NDNabstraction'): ns3::CcnxProducerHelper::CcnxProducerHelper(ns3::CcnxProducerHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxProducerHelper const &', 'arg0')])
    ## ccnx-producer-helper.h (module 'NDNabstraction'): ns3::CcnxProducerHelper::CcnxProducerHelper(uint32_t storeCapacity) [constructor]
    cls.add_constructor([param('uint32_t', 'storeCapacity')])
    ## ccnx-producer-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxProducerHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## ccnx-producer-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxProducerHelper::Install(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ccnx-producer-helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::CcnxProducerHelper::Install(std::string nodeName) [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')])
    ## ccnx-producer-helper.h (module 'NDNabstraction'): void ns3::CcnxProducerHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3CcnxRitContainer_methods(root_module, cls):
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitContainer::CcnxRitContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitContainer::CcnxRitContainer(ns3::CcnxRitContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxRitContainer const &', 'arg0')])
    return

def register_Ns3CcnxRitEntry_methods(root_module, cls):
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitEntry::CcnxRitEntry(ns3::CcnxRitEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxRitEntry const &', 'arg0')])
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitEntry::CcnxRitEntry(ns3::CcnxNameComponents const & prefix, uint32_t nonce, ns3::Time const & timeout) [constructor]
    cls.add_constructor([param('ns3::CcnxNameComponents const &', 'prefix'), param('uint32_t', 'nonce'), param('ns3::Time const &', 'timeout')])
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitEntry::m_expireTime [variable]
    cls.add_instance_attribute('m_expireTime', 'ns3::Time', is_const=False)
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitEntry::m_nonce [variable]
    cls.add_instance_attribute('m_nonce', 'uint32_t', is_const=False)
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRitEntry::m_prefix [variable]
    cls.add_instance_attribute('m_prefix', 'ns3::CcnxNameComponents', is_const=False)
    return

def register_Ns3CcnxUnknownHeaderException_methods(root_module, cls):
    ## ccnx-header-helper.h (module 'NDNabstraction'): ns3::CcnxUnknownHeaderException::CcnxUnknownHeaderException() [constructor]
    cls.add_constructor([])
    ## ccnx-header-helper.h (module 'NDNabstraction'): ns3::CcnxUnknownHeaderException::CcnxUnknownHeaderException(ns3::CcnxUnknownHeaderException const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxUnknownHeaderException const &', 'arg0')])
    return

def register_Ns3DataRate_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('!=')
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>')
    cls.add_binary_comparison_operator('>=')
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate(ns3::DataRate const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRate const &', 'arg0')])
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate() [constructor]
    cls.add_constructor([])
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate(uint64_t bps) [constructor]
    cls.add_constructor([param('uint64_t', 'bps')])
    ## data-rate.h (module 'network'): ns3::DataRate::DataRate(std::string rate) [constructor]
    cls.add_constructor([param('std::string', 'rate')])
    ## data-rate.h (module 'network'): double ns3::DataRate::CalculateTxTime(uint32_t bytes) const [member function]
    cls.add_method('CalculateTxTime', 
                   'double', 
                   [param('uint32_t', 'bytes')], 
                   is_const=True)
    ## data-rate.h (module 'network'): uint64_t ns3::DataRate::GetBitRate() const [member function]
    cls.add_method('GetBitRate', 
                   'uint64_t', 
                   [], 
                   is_const=True)
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
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsEqual(ns3::Ipv6Address const & other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ipv6Address const &', 'other')], 
                   is_const=True)
    ## ipv6-address.h (module 'network'): bool ns3::Ipv6Address::IsLinkLocal() const [member function]
    cls.add_method('IsLinkLocal', 
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
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredAddress(ns3::Mac48Address addr, ns3::Ipv6Address prefix) [member function]
    cls.add_method('MakeAutoconfiguredAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'addr'), param('ns3::Ipv6Address', 'prefix')], 
                   is_static=True)
    ## ipv6-address.h (module 'network'): static ns3::Ipv6Address ns3::Ipv6Address::MakeAutoconfiguredLinkLocalAddress(ns3::Mac48Address mac) [member function]
    cls.add_method('MakeAutoconfiguredLinkLocalAddress', 
                   'ns3::Ipv6Address', 
                   [param('ns3::Mac48Address', 'mac')], 
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

def register_Ns3LogComponent_methods(root_module, cls):
    ## log.h (module 'core'): ns3::LogComponent::LogComponent(ns3::LogComponent const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LogComponent const &', 'arg0')])
    ## log.h (module 'core'): ns3::LogComponent::LogComponent(char const * name) [constructor]
    cls.add_constructor([param('char const *', 'name')])
    ## log.h (module 'core'): void ns3::LogComponent::Disable(ns3::LogLevel level) [member function]
    cls.add_method('Disable', 
                   'void', 
                   [param('ns3::LogLevel', 'level')])
    ## log.h (module 'core'): void ns3::LogComponent::Enable(ns3::LogLevel level) [member function]
    cls.add_method('Enable', 
                   'void', 
                   [param('ns3::LogLevel', 'level')])
    ## log.h (module 'core'): void ns3::LogComponent::EnvVarCheck(char const * name) [member function]
    cls.add_method('EnvVarCheck', 
                   'void', 
                   [param('char const *', 'name')])
    ## log.h (module 'core'): bool ns3::LogComponent::IsEnabled(ns3::LogLevel level) const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [param('ns3::LogLevel', 'level')], 
                   is_const=True)
    ## log.h (module 'core'): bool ns3::LogComponent::IsNoneEnabled() const [member function]
    cls.add_method('IsNoneEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## log.h (module 'core'): char const * ns3::LogComponent::Name() const [member function]
    cls.add_method('Name', 
                   'char const *', 
                   [], 
                   is_const=True)
    return

def register_Ns3Mac48Address_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## mac48-address.h (module 'network'): ns3::Mac48Address::Mac48Address(ns3::Mac48Address const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'arg0')])
    ## mac48-address.h (module 'network'): ns3::Mac48Address::Mac48Address() [constructor]
    cls.add_constructor([])
    ## mac48-address.h (module 'network'): ns3::Mac48Address::Mac48Address(char const * str) [constructor]
    cls.add_constructor([param('char const *', 'str')])
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::Allocate() [member function]
    cls.add_method('Allocate', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::ConvertFrom(ns3::Address const & address) [member function]
    cls.add_method('ConvertFrom', 
                   'ns3::Mac48Address', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
    ## mac48-address.h (module 'network'): void ns3::Mac48Address::CopyFrom(uint8_t const * buffer) [member function]
    cls.add_method('CopyFrom', 
                   'void', 
                   [param('uint8_t const *', 'buffer')])
    ## mac48-address.h (module 'network'): void ns3::Mac48Address::CopyTo(uint8_t * buffer) const [member function]
    cls.add_method('CopyTo', 
                   'void', 
                   [param('uint8_t *', 'buffer')], 
                   is_const=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetBroadcast() [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv4Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv4Address', 'address')], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticast(ns3::Ipv6Address address) [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Mac48Address', 
                   [param('ns3::Ipv6Address', 'address')], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticast6Prefix() [member function]
    cls.add_method('GetMulticast6Prefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): static ns3::Mac48Address ns3::Mac48Address::GetMulticastPrefix() [member function]
    cls.add_method('GetMulticastPrefix', 
                   'ns3::Mac48Address', 
                   [], 
                   is_static=True)
    ## mac48-address.h (module 'network'): bool ns3::Mac48Address::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h (module 'network'): bool ns3::Mac48Address::IsGroup() const [member function]
    cls.add_method('IsGroup', 
                   'bool', 
                   [], 
                   is_const=True)
    ## mac48-address.h (module 'network'): static bool ns3::Mac48Address::IsMatchingType(ns3::Address const & address) [member function]
    cls.add_method('IsMatchingType', 
                   'bool', 
                   [param('ns3::Address const &', 'address')], 
                   is_static=True)
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

def register_Ns3PcapFile_methods(root_module, cls):
    ## pcap-file.h (module 'network'): ns3::PcapFile::PcapFile() [constructor]
    cls.add_constructor([])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Close() [member function]
    cls.add_method('Close', 
                   'void', 
                   [])
    ## pcap-file.h (module 'network'): static bool ns3::PcapFile::Diff(std::string const & f1, std::string const & f2, uint32_t & sec, uint32_t & usec, uint32_t snapLen=ns3::PcapFile::SNAPLEN_DEFAULT) [member function]
    cls.add_method('Diff', 
                   'bool', 
                   [param('std::string const &', 'f1'), param('std::string const &', 'f2'), param('uint32_t &', 'sec'), param('uint32_t &', 'usec'), param('uint32_t', 'snapLen', default_value='ns3::PcapFile::SNAPLEN_DEFAULT')], 
                   is_static=True)
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::Eof() const [member function]
    cls.add_method('Eof', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::Fail() const [member function]
    cls.add_method('Fail', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetDataLinkType() [member function]
    cls.add_method('GetDataLinkType', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetMagic() [member function]
    cls.add_method('GetMagic', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetSigFigs() [member function]
    cls.add_method('GetSigFigs', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint32_t ns3::PcapFile::GetSnapLen() [member function]
    cls.add_method('GetSnapLen', 
                   'uint32_t', 
                   [])
    ## pcap-file.h (module 'network'): bool ns3::PcapFile::GetSwapMode() [member function]
    cls.add_method('GetSwapMode', 
                   'bool', 
                   [])
    ## pcap-file.h (module 'network'): int32_t ns3::PcapFile::GetTimeZoneOffset() [member function]
    cls.add_method('GetTimeZoneOffset', 
                   'int32_t', 
                   [])
    ## pcap-file.h (module 'network'): uint16_t ns3::PcapFile::GetVersionMajor() [member function]
    cls.add_method('GetVersionMajor', 
                   'uint16_t', 
                   [])
    ## pcap-file.h (module 'network'): uint16_t ns3::PcapFile::GetVersionMinor() [member function]
    cls.add_method('GetVersionMinor', 
                   'uint16_t', 
                   [])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Init(uint32_t dataLinkType, uint32_t snapLen=ns3::PcapFile::SNAPLEN_DEFAULT, int32_t timeZoneCorrection=ns3::PcapFile::ZONE_DEFAULT, bool swapMode=false) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='ns3::PcapFile::SNAPLEN_DEFAULT'), param('int32_t', 'timeZoneCorrection', default_value='ns3::PcapFile::ZONE_DEFAULT'), param('bool', 'swapMode', default_value='false')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Open(std::string const & filename, std::_Ios_Openmode mode) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'filename'), param('std::_Ios_Openmode', 'mode')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Read(uint8_t * const data, uint32_t maxBytes, uint32_t & tsSec, uint32_t & tsUsec, uint32_t & inclLen, uint32_t & origLen, uint32_t & readLen) [member function]
    cls.add_method('Read', 
                   'void', 
                   [param('uint8_t * const', 'data'), param('uint32_t', 'maxBytes'), param('uint32_t &', 'tsSec'), param('uint32_t &', 'tsUsec'), param('uint32_t &', 'inclLen'), param('uint32_t &', 'origLen'), param('uint32_t &', 'readLen')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, uint8_t const * const data, uint32_t totalLen) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('uint8_t const * const', 'data'), param('uint32_t', 'totalLen')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file.h (module 'network'): void ns3::PcapFile::Write(uint32_t tsSec, uint32_t tsUsec, ns3::Header & header, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('uint32_t', 'tsSec'), param('uint32_t', 'tsUsec'), param('ns3::Header &', 'header'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file.h (module 'network'): ns3::PcapFile::SNAPLEN_DEFAULT [variable]
    cls.add_static_attribute('SNAPLEN_DEFAULT', 'uint32_t const', is_const=True)
    ## pcap-file.h (module 'network'): ns3::PcapFile::ZONE_DEFAULT [variable]
    cls.add_static_attribute('ZONE_DEFAULT', 'int32_t const', is_const=True)
    return

def register_Ns3PcapHelper_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::PcapHelper::PcapHelper(ns3::PcapHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelper const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::PcapHelper::PcapHelper() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): ns3::Ptr<ns3::PcapFileWrapper> ns3::PcapHelper::CreateFile(std::string filename, std::_Ios_Openmode filemode, uint32_t dataLinkType, uint32_t snapLen=65535, int32_t tzCorrection=0) [member function]
    cls.add_method('CreateFile', 
                   'ns3::Ptr< ns3::PcapFileWrapper >', 
                   [param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode'), param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='65535'), param('int32_t', 'tzCorrection', default_value='0')])
    ## trace-helper.h (module 'network'): std::string ns3::PcapHelper::GetFilenameFromDevice(std::string prefix, ns3::Ptr<ns3::NetDevice> device, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromDevice', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'device'), param('bool', 'useObjectNames', default_value='true')])
    ## trace-helper.h (module 'network'): std::string ns3::PcapHelper::GetFilenameFromInterfacePair(std::string prefix, ns3::Ptr<ns3::Object> object, uint32_t interface, bool useObjectNames=true) [member function]
    cls.add_method('GetFilenameFromInterfacePair', 
                   'std::string', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Object >', 'object'), param('uint32_t', 'interface'), param('bool', 'useObjectNames', default_value='true')])
    return

def register_Ns3PcapHelperForCcnx_methods(root_module, cls):
    ## ccnx-trace-helper.h (module 'NDNabstraction'): ns3::PcapHelperForCcnx::PcapHelperForCcnx(ns3::PcapHelperForCcnx const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelperForCcnx const &', 'arg0')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): ns3::PcapHelperForCcnx::PcapHelperForCcnx() [constructor]
    cls.add_constructor([])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnx(std::string prefix, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcapCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface'), param('bool', 'explicitFilename', default_value='false')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnx(std::string prefix, std::string ccnxName, uint32_t interface, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcapCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ccnxName'), param('uint32_t', 'interface'), param('bool', 'explicitFilename', default_value='false')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnx(std::string prefix, ns3::CcnxFaceContainer c) [member function]
    cls.add_method('EnablePcapCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::CcnxFaceContainer', 'c')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnx(std::string prefix, ns3::NodeContainer n) [member function]
    cls.add_method('EnablePcapCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnx(std::string prefix, uint32_t nodeid, uint32_t interface, bool explicitFilename) [member function]
    cls.add_method('EnablePcapCcnx', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'interface'), param('bool', 'explicitFilename')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnxAll(std::string prefix) [member function]
    cls.add_method('EnablePcapCcnxAll', 
                   'void', 
                   [param('std::string', 'prefix')])
    ## ccnx-trace-helper.h (module 'NDNabstraction'): void ns3::PcapHelperForCcnx::EnablePcapCcnxInternal(std::string prefix, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface, bool explicitFilename) [member function]
    cls.add_method('EnablePcapCcnxInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3PcapHelperForDevice_methods(root_module, cls):
    ## trace-helper.h (module 'network'): ns3::PcapHelperForDevice::PcapHelperForDevice(ns3::PcapHelperForDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PcapHelperForDevice const &', 'arg0')])
    ## trace-helper.h (module 'network'): ns3::PcapHelperForDevice::PcapHelperForDevice() [constructor]
    cls.add_constructor([])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous=false, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous', default_value='false'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, std::string ndName, bool promiscuous=false, bool explicitFilename=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('std::string', 'ndName'), param('bool', 'promiscuous', default_value='false'), param('bool', 'explicitFilename', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::NetDeviceContainer d, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NetDeviceContainer', 'd'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, ns3::NodeContainer n, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::NodeContainer', 'n'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcap(std::string prefix, uint32_t nodeid, uint32_t deviceid, bool promiscuous=false) [member function]
    cls.add_method('EnablePcap', 
                   'void', 
                   [param('std::string', 'prefix'), param('uint32_t', 'nodeid'), param('uint32_t', 'deviceid'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcapAll(std::string prefix, bool promiscuous=false) [member function]
    cls.add_method('EnablePcapAll', 
                   'void', 
                   [param('std::string', 'prefix'), param('bool', 'promiscuous', default_value='false')])
    ## trace-helper.h (module 'network'): void ns3::PcapHelperForDevice::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3PointToPointHelper_methods(root_module, cls):
    ## point-to-point-helper.h (module 'point-to-point'): ns3::PointToPointHelper::PointToPointHelper(ns3::PointToPointHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointToPointHelper const &', 'arg0')])
    ## point-to-point-helper.h (module 'point-to-point'): ns3::PointToPointHelper::PointToPointHelper() [constructor]
    cls.add_constructor([])
    ## point-to-point-helper.h (module 'point-to-point'): ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::NodeContainer c) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::NodeContainer', 'c')])
    ## point-to-point-helper.h (module 'point-to-point'): ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h (module 'point-to-point'): ns3::NetDeviceContainer ns3::PointToPointHelper::Install(ns3::Ptr<ns3::Node> a, std::string bName) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'a'), param('std::string', 'bName')])
    ## point-to-point-helper.h (module 'point-to-point'): ns3::NetDeviceContainer ns3::PointToPointHelper::Install(std::string aName, ns3::Ptr<ns3::Node> b) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'aName'), param('ns3::Ptr< ns3::Node >', 'b')])
    ## point-to-point-helper.h (module 'point-to-point'): ns3::NetDeviceContainer ns3::PointToPointHelper::Install(std::string aNode, std::string bNode) [member function]
    cls.add_method('Install', 
                   'ns3::NetDeviceContainer', 
                   [param('std::string', 'aNode'), param('std::string', 'bNode')])
    ## point-to-point-helper.h (module 'point-to-point'): void ns3::PointToPointHelper::SetChannelAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetChannelAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h (module 'point-to-point'): void ns3::PointToPointHelper::SetDeviceAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetDeviceAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## point-to-point-helper.h (module 'point-to-point'): void ns3::PointToPointHelper::SetQueue(std::string type, std::string n1="", ns3::AttributeValue const & v1=ns3::EmptyAttributeValue(), std::string n2="", ns3::AttributeValue const & v2=ns3::EmptyAttributeValue(), std::string n3="", ns3::AttributeValue const & v3=ns3::EmptyAttributeValue(), std::string n4="", ns3::AttributeValue const & v4=ns3::EmptyAttributeValue()) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('std::string', 'type'), param('std::string', 'n1', default_value='""'), param('ns3::AttributeValue const &', 'v1', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n2', default_value='""'), param('ns3::AttributeValue const &', 'v2', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n3', default_value='""'), param('ns3::AttributeValue const &', 'v3', default_value='ns3::EmptyAttributeValue()'), param('std::string', 'n4', default_value='""'), param('ns3::AttributeValue const &', 'v4', default_value='ns3::EmptyAttributeValue()')])
    ## point-to-point-helper.h (module 'point-to-point'): void ns3::PointToPointHelper::EnableAsciiInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## point-to-point-helper.h (module 'point-to-point'): void ns3::PointToPointHelper::EnablePcapInternal(std::string prefix, ns3::Ptr<ns3::NetDevice> nd, bool promiscuous, bool explicitFilename) [member function]
    cls.add_method('EnablePcapInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::NetDevice >', 'nd'), param('bool', 'promiscuous'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
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

def register_Ns3SeedManager_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::SeedManager::SeedManager() [constructor]
    cls.add_constructor([])
    ## random-variable.h (module 'core'): ns3::SeedManager::SeedManager(ns3::SeedManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SeedManager const &', 'arg0')])
    ## random-variable.h (module 'core'): static bool ns3::SeedManager::CheckSeed(uint32_t seed) [member function]
    cls.add_method('CheckSeed', 
                   'bool', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    ## random-variable.h (module 'core'): static uint32_t ns3::SeedManager::GetRun() [member function]
    cls.add_method('GetRun', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## random-variable.h (module 'core'): static uint32_t ns3::SeedManager::GetSeed() [member function]
    cls.add_method('GetSeed', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## random-variable.h (module 'core'): static void ns3::SeedManager::SetRun(uint32_t run) [member function]
    cls.add_method('SetRun', 
                   'void', 
                   [param('uint32_t', 'run')], 
                   is_static=True)
    ## random-variable.h (module 'core'): static void ns3::SeedManager::SetSeed(uint32_t seed) [member function]
    cls.add_method('SetSeed', 
                   'void', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    return

def register_Ns3SequentialVariable_methods(root_module, cls):
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(ns3::SequentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SequentialVariable const &', 'arg0')])
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(double f, double l, double i=1, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('double', 'i', default_value='1'), param('uint32_t', 'c', default_value='1')])
    ## random-variable.h (module 'core'): ns3::SequentialVariable::SequentialVariable(double f, double l, ns3::RandomVariable const & i, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('ns3::RandomVariable const &', 'i'), param('uint32_t', 'c', default_value='1')])
    return

def register_Ns3SimpleRefCount__Ns3Chunk_Ns3ObjectBase_Ns3DefaultDeleter__lt__ns3Chunk__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter< ns3::Chunk > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::Chunk, ns3::ObjectBase, ns3::DefaultDeleter<ns3::Chunk> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
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
    ## simulator.h (module 'core'): static ns3::Time ns3::Simulator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::Time', 
                   [], 
                   is_static=True, deprecated=True)
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
    ## simulator.h (module 'core'): static void ns3::Simulator::RunOneEvent() [member function]
    cls.add_method('RunOneEvent', 
                   'void', 
                   [], 
                   is_static=True, deprecated=True)
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

def register_Ns3StupidInterestGeneratorHelper_methods(root_module, cls):
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): ns3::StupidInterestGeneratorHelper::StupidInterestGeneratorHelper(ns3::StupidInterestGeneratorHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StupidInterestGeneratorHelper const &', 'arg0')])
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): ns3::StupidInterestGeneratorHelper::StupidInterestGeneratorHelper(std::string protocol, ns3::Address address) [constructor]
    cls.add_constructor([param('std::string', 'protocol'), param('ns3::Address', 'address')])
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::StupidInterestGeneratorHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::StupidInterestGeneratorHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): ns3::ApplicationContainer ns3::StupidInterestGeneratorHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::ApplicationContainer', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## ndn_stupidinterestgenerator_helper.h (module 'NDNabstraction'): void ns3::StupidInterestGeneratorHelper::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
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
                   [param('std::string', 'name'), param('ns3::TypeId::AttributeInformation *', 'info')], 
                   is_const=True)
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
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_binary_numeric_operator('*', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_binary_numeric_operator('+', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_unary_numeric_operator('-')
    cls.add_binary_numeric_operator('-', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short unsigned int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('unsigned char const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long long int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('long int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('short int const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('signed char const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('double const', 'right'))
    cls.add_binary_numeric_operator('/', root_module['ns3::int64x64_t'], root_module['ns3::int64x64_t'], param('ns3::int64x64_t const &', 'right'))
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('>')
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t() [constructor]
    cls.add_constructor([])
    ## int64x64-double.h (module 'core'): ns3::int64x64_t::int64x64_t(double v) [constructor]
    cls.add_constructor([param('double', 'v')])
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
    return

def register_Ns3CcnxStackHelper_methods(root_module, cls):
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::CcnxStackHelper::CcnxStackHelper() [constructor]
    cls.add_constructor([])
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::CcnxStackHelper::CcnxStackHelper(ns3::CcnxStackHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxStackHelper const &', 'arg0')])
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFaceContainer> ns3::CcnxStackHelper::Install(std::string nodeName) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::CcnxFaceContainer >', 
                   [param('std::string', 'nodeName')], 
                   is_const=True)
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFaceContainer> ns3::CcnxStackHelper::Install(ns3::Ptr<ns3::Node> node) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::CcnxFaceContainer >', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_const=True)
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFaceContainer> ns3::CcnxStackHelper::Install(ns3::NodeContainer c) const [member function]
    cls.add_method('Install', 
                   'ns3::Ptr< ns3::CcnxFaceContainer >', 
                   [param('ns3::NodeContainer', 'c')], 
                   is_const=True)
    ## ccnx-stack-helper.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFaceContainer> ns3::CcnxStackHelper::InstallAll() const [member function]
    cls.add_method('InstallAll', 
                   'ns3::Ptr< ns3::CcnxFaceContainer >', 
                   [], 
                   is_const=True)
    ## ccnx-stack-helper.h (module 'NDNabstraction'): void ns3::CcnxStackHelper::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## ccnx-stack-helper.h (module 'NDNabstraction'): void ns3::CcnxStackHelper::SetCcnxStackInstall(bool enable) [member function]
    cls.add_method('SetCcnxStackInstall', 
                   'void', 
                   [param('bool', 'enable')])
    ## ccnx-stack-helper.h (module 'NDNabstraction'): void ns3::CcnxStackHelper::SetForwardingHelper(ns3::CcnxForwardingHelper const & forwarding) [member function]
    cls.add_method('SetForwardingHelper', 
                   'void', 
                   [param('ns3::CcnxForwardingHelper const &', 'forwarding')])
    ## ccnx-stack-helper.h (module 'NDNabstraction'): void ns3::CcnxStackHelper::EnableAsciiCcnxInternal(ns3::Ptr<ns3::OutputStreamWrapper> stream, std::string prefix, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface, bool explicitFilename) [member function]
    cls.add_method('EnableAsciiCcnxInternal', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream'), param('std::string', 'prefix'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
    ## ccnx-stack-helper.h (module 'NDNabstraction'): void ns3::CcnxStackHelper::EnablePcapCcnxInternal(std::string prefix, ns3::Ptr<ns3::Ccnx> ccnx, uint32_t interface, bool explicitFilename) [member function]
    cls.add_method('EnablePcapCcnxInternal', 
                   'void', 
                   [param('std::string', 'prefix'), param('ns3::Ptr< ns3::Ccnx >', 'ccnx'), param('uint32_t', 'interface'), param('bool', 'explicitFilename')], 
                   visibility='private', is_virtual=True)
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
    ## object.h (module 'core'): static ns3::TypeId ns3::Object::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object.h (module 'core'): void ns3::Object::Start() [member function]
    cls.add_method('Start', 
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
    ## object.h (module 'core'): void ns3::Object::DoStart() [member function]
    cls.add_method('DoStart', 
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

def register_Ns3PcapFileWrapper_methods(root_module, cls):
    ## pcap-file-wrapper.h (module 'network'): static ns3::TypeId ns3::PcapFileWrapper::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## pcap-file-wrapper.h (module 'network'): ns3::PcapFileWrapper::PcapFileWrapper() [constructor]
    cls.add_constructor([])
    ## pcap-file-wrapper.h (module 'network'): bool ns3::PcapFileWrapper::Fail() const [member function]
    cls.add_method('Fail', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file-wrapper.h (module 'network'): bool ns3::PcapFileWrapper::Eof() const [member function]
    cls.add_method('Eof', 
                   'bool', 
                   [], 
                   is_const=True)
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Open(std::string const & filename, std::_Ios_Openmode mode) [member function]
    cls.add_method('Open', 
                   'void', 
                   [param('std::string const &', 'filename'), param('std::_Ios_Openmode', 'mode')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Close() [member function]
    cls.add_method('Close', 
                   'void', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Init(uint32_t dataLinkType, uint32_t snapLen=std::numeric_limits<unsigned int>::max(), int32_t tzCorrection=ns3::PcapFile::ZONE_DEFAULT) [member function]
    cls.add_method('Init', 
                   'void', 
                   [param('uint32_t', 'dataLinkType'), param('uint32_t', 'snapLen', default_value='std::numeric_limits<unsigned int>::max()'), param('int32_t', 'tzCorrection', default_value='ns3::PcapFile::ZONE_DEFAULT')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Write(ns3::Time t, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Write(ns3::Time t, ns3::Header & header, ns3::Ptr<ns3::Packet const> p) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('ns3::Header &', 'header'), param('ns3::Ptr< ns3::Packet const >', 'p')])
    ## pcap-file-wrapper.h (module 'network'): void ns3::PcapFileWrapper::Write(ns3::Time t, uint8_t const * buffer, uint32_t length) [member function]
    cls.add_method('Write', 
                   'void', 
                   [param('ns3::Time', 't'), param('uint8_t const *', 'buffer'), param('uint32_t', 'length')])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetMagic() [member function]
    cls.add_method('GetMagic', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint16_t ns3::PcapFileWrapper::GetVersionMajor() [member function]
    cls.add_method('GetVersionMajor', 
                   'uint16_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint16_t ns3::PcapFileWrapper::GetVersionMinor() [member function]
    cls.add_method('GetVersionMinor', 
                   'uint16_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): int32_t ns3::PcapFileWrapper::GetTimeZoneOffset() [member function]
    cls.add_method('GetTimeZoneOffset', 
                   'int32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetSigFigs() [member function]
    cls.add_method('GetSigFigs', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetSnapLen() [member function]
    cls.add_method('GetSnapLen', 
                   'uint32_t', 
                   [])
    ## pcap-file-wrapper.h (module 'network'): uint32_t ns3::PcapFileWrapper::GetDataLinkType() [member function]
    cls.add_method('GetDataLinkType', 
                   'uint32_t', 
                   [])
    return

def register_Ns3Queue_methods(root_module, cls):
    ## queue.h (module 'network'): ns3::Queue::Queue(ns3::Queue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Queue const &', 'arg0')])
    ## queue.h (module 'network'): ns3::Queue::Queue() [constructor]
    cls.add_constructor([])
    ## queue.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Queue::Dequeue() [member function]
    cls.add_method('Dequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## queue.h (module 'network'): void ns3::Queue::DequeueAll() [member function]
    cls.add_method('DequeueAll', 
                   'void', 
                   [])
    ## queue.h (module 'network'): bool ns3::Queue::Enqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Enqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## queue.h (module 'network'): uint32_t ns3::Queue::GetNBytes() const [member function]
    cls.add_method('GetNBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): uint32_t ns3::Queue::GetNPackets() const [member function]
    cls.add_method('GetNPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): uint32_t ns3::Queue::GetTotalDroppedBytes() const [member function]
    cls.add_method('GetTotalDroppedBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): uint32_t ns3::Queue::GetTotalDroppedPackets() const [member function]
    cls.add_method('GetTotalDroppedPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): uint32_t ns3::Queue::GetTotalReceivedBytes() const [member function]
    cls.add_method('GetTotalReceivedBytes', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): uint32_t ns3::Queue::GetTotalReceivedPackets() const [member function]
    cls.add_method('GetTotalReceivedPackets', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): static ns3::TypeId ns3::Queue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## queue.h (module 'network'): bool ns3::Queue::IsEmpty() const [member function]
    cls.add_method('IsEmpty', 
                   'bool', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): ns3::Ptr<ns3::Packet const> ns3::Queue::Peek() const [member function]
    cls.add_method('Peek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True)
    ## queue.h (module 'network'): void ns3::Queue::ResetStatistics() [member function]
    cls.add_method('ResetStatistics', 
                   'void', 
                   [])
    ## queue.h (module 'network'): void ns3::Queue::Drop(ns3::Ptr<ns3::Packet> packet) [member function]
    cls.add_method('Drop', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet')], 
                   visibility='protected')
    ## queue.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Queue::DoDequeue() [member function]
    cls.add_method('DoDequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## queue.h (module 'network'): bool ns3::Queue::DoEnqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoEnqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## queue.h (module 'network'): ns3::Ptr<ns3::Packet const> ns3::Queue::DoPeek() const [member function]
    cls.add_method('DoPeek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
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

def register_Ns3SimpleRefCount__Ns3CcnbParserBlock_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnbParserBlock__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter< ns3::CcnbParser::Block > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CcnbParser::Block, ns3::empty, ns3::DefaultDeleter<ns3::CcnbParser::Block> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CcnxFaceContainer_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxFaceContainer__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter< ns3::CcnxFaceContainer > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CcnxFaceContainer, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFaceContainer> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CcnxFibEntry_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxFibEntry__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter< ns3::CcnxFibEntry > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CcnxFibEntry, ns3::empty, ns3::DefaultDeleter<ns3::CcnxFibEntry> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CcnxNameComponents_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxNameComponents__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter< ns3::CcnxNameComponents > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CcnxNameComponents, ns3::empty, ns3::DefaultDeleter<ns3::CcnxNameComponents> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CcnxRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3CcnxRoute__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter< ns3::CcnxRoute > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::CcnxRoute, ns3::empty, ns3::DefaultDeleter<ns3::CcnxRoute> >::Cleanup() [member function]
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

def register_Ns3SimpleRefCount__Ns3OutputStreamWrapper_Ns3Empty_Ns3DefaultDeleter__lt__ns3OutputStreamWrapper__gt___methods(root_module, cls):
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h (module 'core'): ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::SimpleRefCount(ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter< ns3::OutputStreamWrapper > > const &', 'o')])
    ## simple-ref-count.h (module 'core'): static void ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::Cleanup() [member function]
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

def register_Ns3Socket_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::Socket::Socket(ns3::Socket const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Socket const &', 'arg0')])
    ## socket.h (module 'network'): ns3::Socket::Socket() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): int ns3::Socket::Bind(ns3::Address const & address) [member function]
    cls.add_method('Bind', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Bind() [member function]
    cls.add_method('Bind', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::BindToNetDevice(ns3::Ptr<ns3::NetDevice> netdevice) [member function]
    cls.add_method('BindToNetDevice', 
                   'void', 
                   [param('ns3::Ptr< ns3::NetDevice >', 'netdevice')], 
                   is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Close() [member function]
    cls.add_method('Close', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Connect(ns3::Address const & address) [member function]
    cls.add_method('Connect', 
                   'int', 
                   [param('ns3::Address const &', 'address')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::Ptr<ns3::Socket> ns3::Socket::CreateSocket(ns3::Ptr<ns3::Node> node, ns3::TypeId tid) [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [param('ns3::Ptr< ns3::Node >', 'node'), param('ns3::TypeId', 'tid')], 
                   is_static=True)
    ## socket.h (module 'network'): bool ns3::Socket::GetAllowBroadcast() const [member function]
    cls.add_method('GetAllowBroadcast', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::NetDevice> ns3::Socket::GetBoundNetDevice() [member function]
    cls.add_method('GetBoundNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [])
    ## socket.h (module 'network'): ns3::Socket::SocketErrno ns3::Socket::GetErrno() const [member function]
    cls.add_method('GetErrno', 
                   'ns3::Socket::SocketErrno', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Node> ns3::Socket::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::Socket::GetRxAvailable() const [member function]
    cls.add_method('GetRxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::GetSockName(ns3::Address & address) const [member function]
    cls.add_method('GetSockName', 
                   'int', 
                   [param('ns3::Address &', 'address')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Socket::SocketType ns3::Socket::GetSocketType() const [member function]
    cls.add_method('GetSocketType', 
                   'ns3::Socket::SocketType', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::Socket::GetTxAvailable() const [member function]
    cls.add_method('GetTxAvailable', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Listen() [member function]
    cls.add_method('Listen', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::Recv(uint32_t maxSize, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::Recv() [member function]
    cls.add_method('Recv', 
                   'ns3::Ptr< ns3::Packet >', 
                   [])
    ## socket.h (module 'network'): int ns3::Socket::Recv(uint8_t * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Recv', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(uint32_t maxSize, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('uint32_t', 'maxSize'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::Socket::RecvFrom(ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Address &', 'fromAddress')])
    ## socket.h (module 'network'): int ns3::Socket::RecvFrom(uint8_t * buf, uint32_t size, uint32_t flags, ns3::Address & fromAddress) [member function]
    cls.add_method('RecvFrom', 
                   'int', 
                   [param('uint8_t *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address &', 'fromAddress')])
    ## socket.h (module 'network'): int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## socket.h (module 'network'): int ns3::Socket::Send(uint8_t const * buf, uint32_t size, uint32_t flags) [member function]
    cls.add_method('Send', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags')])
    ## socket.h (module 'network'): int ns3::Socket::SendTo(ns3::Ptr<ns3::Packet> p, uint32_t flags, ns3::Address const & toAddress) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('uint32_t', 'flags'), param('ns3::Address const &', 'toAddress')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::SendTo(uint8_t const * buf, uint32_t size, uint32_t flags, ns3::Address const & address) [member function]
    cls.add_method('SendTo', 
                   'int', 
                   [param('uint8_t const *', 'buf'), param('uint32_t', 'size'), param('uint32_t', 'flags'), param('ns3::Address const &', 'address')])
    ## socket.h (module 'network'): void ns3::Socket::SetAcceptCallback(ns3::Callback<bool, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionRequest, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> newConnectionCreated) [member function]
    cls.add_method('SetAcceptCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionRequest'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'newConnectionCreated')])
    ## socket.h (module 'network'): bool ns3::Socket::SetAllowBroadcast(bool allowBroadcast) [member function]
    cls.add_method('SetAllowBroadcast', 
                   'bool', 
                   [param('bool', 'allowBroadcast')], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::SetCloseCallbacks(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> normalClose, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> errorClose) [member function]
    cls.add_method('SetCloseCallbacks', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'normalClose'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'errorClose')])
    ## socket.h (module 'network'): void ns3::Socket::SetConnectCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionSucceeded, ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> connectionFailed) [member function]
    cls.add_method('SetConnectCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionSucceeded'), param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'connectionFailed')])
    ## socket.h (module 'network'): void ns3::Socket::SetDataSentCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> dataSent) [member function]
    cls.add_method('SetDataSentCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'dataSent')])
    ## socket.h (module 'network'): void ns3::Socket::SetRecvCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> arg0) [member function]
    cls.add_method('SetRecvCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'arg0')])
    ## socket.h (module 'network'): void ns3::Socket::SetRecvPktInfo(bool flag) [member function]
    cls.add_method('SetRecvPktInfo', 
                   'void', 
                   [param('bool', 'flag')])
    ## socket.h (module 'network'): void ns3::Socket::SetSendCallback(ns3::Callback<void, ns3::Ptr<ns3::Socket>, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> sendCb) [member function]
    cls.add_method('SetSendCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::Socket >, unsigned int, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'sendCb')])
    ## socket.h (module 'network'): int ns3::Socket::ShutdownRecv() [member function]
    cls.add_method('ShutdownRecv', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): int ns3::Socket::ShutdownSend() [member function]
    cls.add_method('ShutdownSend', 
                   'int', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## socket.h (module 'network'): void ns3::Socket::NotifyConnectionFailed() [member function]
    cls.add_method('NotifyConnectionFailed', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): bool ns3::Socket::NotifyConnectionRequest(ns3::Address const & from) [member function]
    cls.add_method('NotifyConnectionRequest', 
                   'bool', 
                   [param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyConnectionSucceeded() [member function]
    cls.add_method('NotifyConnectionSucceeded', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyDataRecv() [member function]
    cls.add_method('NotifyDataRecv', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyDataSent(uint32_t size) [member function]
    cls.add_method('NotifyDataSent', 
                   'void', 
                   [param('uint32_t', 'size')], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyErrorClose() [member function]
    cls.add_method('NotifyErrorClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyNewConnectionCreated(ns3::Ptr<ns3::Socket> socket, ns3::Address const & from) [member function]
    cls.add_method('NotifyNewConnectionCreated', 
                   'void', 
                   [param('ns3::Ptr< ns3::Socket >', 'socket'), param('ns3::Address const &', 'from')], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifyNormalClose() [member function]
    cls.add_method('NotifyNormalClose', 
                   'void', 
                   [], 
                   visibility='protected')
    ## socket.h (module 'network'): void ns3::Socket::NotifySend(uint32_t spaceAvailable) [member function]
    cls.add_method('NotifySend', 
                   'void', 
                   [param('uint32_t', 'spaceAvailable')], 
                   visibility='protected')
    return

def register_Ns3SocketAddressTag_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::SocketAddressTag::SocketAddressTag(ns3::SocketAddressTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketAddressTag const &', 'arg0')])
    ## socket.h (module 'network'): ns3::SocketAddressTag::SocketAddressTag() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): void ns3::SocketAddressTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h (module 'network'): ns3::Address ns3::SocketAddressTag::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): ns3::TypeId ns3::SocketAddressTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::SocketAddressTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::SocketAddressTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): void ns3::SocketAddressTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketAddressTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketAddressTag::SetAddress(ns3::Address addr) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'addr')])
    return

def register_Ns3SocketFactory_methods(root_module, cls):
    ## socket-factory.h (module 'network'): ns3::SocketFactory::SocketFactory(ns3::SocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketFactory const &', 'arg0')])
    ## socket-factory.h (module 'network'): ns3::SocketFactory::SocketFactory() [constructor]
    cls.add_constructor([])
    ## socket-factory.h (module 'network'): ns3::Ptr<ns3::Socket> ns3::SocketFactory::CreateSocket() [member function]
    cls.add_method('CreateSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## socket-factory.h (module 'network'): static ns3::TypeId ns3::SocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3SocketIpTtlTag_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::SocketIpTtlTag::SocketIpTtlTag(ns3::SocketIpTtlTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketIpTtlTag const &', 'arg0')])
    ## socket.h (module 'network'): ns3::SocketIpTtlTag::SocketIpTtlTag() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h (module 'network'): ns3::TypeId ns3::SocketIpTtlTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::SocketIpTtlTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint8_t ns3::SocketIpTtlTag::GetTtl() const [member function]
    cls.add_method('GetTtl', 
                   'uint8_t', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::SocketIpTtlTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketIpTtlTag::SetTtl(uint8_t ttl) [member function]
    cls.add_method('SetTtl', 
                   'void', 
                   [param('uint8_t', 'ttl')])
    return

def register_Ns3SocketSetDontFragmentTag_methods(root_module, cls):
    ## socket.h (module 'network'): ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag(ns3::SocketSetDontFragmentTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SocketSetDontFragmentTag const &', 'arg0')])
    ## socket.h (module 'network'): ns3::SocketSetDontFragmentTag::SocketSetDontFragmentTag() [constructor]
    cls.add_constructor([])
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Deserialize(ns3::TagBuffer i) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Disable() [member function]
    cls.add_method('Disable', 
                   'void', 
                   [])
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Enable() [member function]
    cls.add_method('Enable', 
                   'void', 
                   [])
    ## socket.h (module 'network'): ns3::TypeId ns3::SocketSetDontFragmentTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): uint32_t ns3::SocketSetDontFragmentTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): static ns3::TypeId ns3::SocketSetDontFragmentTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## socket.h (module 'network'): bool ns3::SocketSetDontFragmentTag::IsEnabled() const [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_const=True)
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## socket.h (module 'network'): void ns3::SocketSetDontFragmentTag::Serialize(ns3::TagBuffer i) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'i')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3Time_methods(root_module, cls):
    cls.add_binary_comparison_operator('<=')
    cls.add_binary_comparison_operator('!=')
    cls.add_inplace_numeric_operator('+=', param('ns3::Time const &', 'right'))
    cls.add_inplace_numeric_operator('-=', param('ns3::Time const &', 'right'))
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('>=')
    cls.add_binary_numeric_operator('+', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
    cls.add_binary_numeric_operator('-', root_module['ns3::Time'], root_module['ns3::Time'], param('ns3::Time const &', 'right'))
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
    ## nstime.h (module 'core'): static void ns3::Time::SetResolution(ns3::Time::Unit resolution) [member function]
    cls.add_method('SetResolution', 
                   'void', 
                   [param('ns3::Time::Unit', 'resolution')], 
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
    ## topology-reader.h (module 'topology-read'): static ns3::TypeId ns3::TopologyReader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
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
    ## topology-reader.h (module 'topology-read'): void ns3::TopologyReader::SetFileName(std::string const fileName) [member function]
    cls.add_method('SetFileName', 
                   'void', 
                   [param('std::string const', 'fileName')])
    return

def register_Ns3TopologyReaderLink_methods(root_module, cls):
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link::Link(ns3::TopologyReader::Link const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TopologyReader::Link const &', 'arg0')])
    ## topology-reader.h (module 'topology-read'): ns3::TopologyReader::Link::Link(ns3::Ptr<ns3::Node> fromPtr, std::string fromName, ns3::Ptr<ns3::Node> toPtr, std::string toName) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Node >', 'fromPtr'), param('std::string', 'fromName'), param('ns3::Ptr< ns3::Node >', 'toPtr'), param('std::string', 'toName')])
    ## topology-reader.h (module 'topology-read'): std::_Rb_tree_const_iterator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::TopologyReader::Link::AttributesBegin() [member function]
    cls.add_method('AttributesBegin', 
                   'std::_Rb_tree_const_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## topology-reader.h (module 'topology-read'): std::_Rb_tree_const_iterator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > ns3::TopologyReader::Link::AttributesEnd() [member function]
    cls.add_method('AttributesEnd', 
                   'std::_Rb_tree_const_iterator< std::pair< std::basic_string< char, std::char_traits< char >, std::allocator< char > > const, std::basic_string< char, std::char_traits< char >, std::allocator< char > > > >', 
                   [])
    ## topology-reader.h (module 'topology-read'): std::string ns3::TopologyReader::Link::GetAttribute(std::string name) const [member function]
    cls.add_method('GetAttribute', 
                   'std::string', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## topology-reader.h (module 'topology-read'): bool ns3::TopologyReader::Link::GetAttributeFailSafe(std::string name, std::string & value) const [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string &', 'value')], 
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
    ## topology-reader.h (module 'topology-read'): void ns3::TopologyReader::Link::SetAttribute(std::string name, std::string & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('std::string &', 'value')])
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

def register_Ns3UdpSocketFactory_methods(root_module, cls):
    ## udp-socket-factory.h (module 'internet'): ns3::UdpSocketFactory::UdpSocketFactory() [constructor]
    cls.add_constructor([])
    ## udp-socket-factory.h (module 'internet'): ns3::UdpSocketFactory::UdpSocketFactory(ns3::UdpSocketFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UdpSocketFactory const &', 'arg0')])
    ## udp-socket-factory.h (module 'internet'): static ns3::TypeId ns3::UdpSocketFactory::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    return

def register_Ns3AnnotatedTopologyReader_methods(root_module, cls):
    ## annotated-topology-reader.h (module 'NDNabstraction'): static ns3::TypeId ns3::AnnotatedTopologyReader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## annotated-topology-reader.h (module 'NDNabstraction'): ns3::AnnotatedTopologyReader::AnnotatedTopologyReader() [constructor]
    cls.add_constructor([])
    ## annotated-topology-reader.h (module 'NDNabstraction'): ns3::NodeContainer ns3::AnnotatedTopologyReader::Read() [member function]
    cls.add_method('Read', 
                   'ns3::NodeContainer', 
                   [], 
                   is_virtual=True)
    ## annotated-topology-reader.h (module 'NDNabstraction'): void ns3::AnnotatedTopologyReader::ApplySettings(ns3::NetDeviceContainer * ndc, ns3::NodeContainer * nc) [member function]
    cls.add_method('ApplySettings', 
                   'void', 
                   [param('ns3::NetDeviceContainer *', 'ndc'), param('ns3::NodeContainer *', 'nc')])
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
    ## application.h (module 'network'): void ns3::Application::DoStart() [member function]
    cls.add_method('DoStart', 
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

def register_Ns3Ccnx_methods(root_module, cls):
    ## ccnx.h (module 'NDNabstraction'): ns3::Ccnx::Ccnx() [constructor]
    cls.add_constructor([])
    ## ccnx.h (module 'NDNabstraction'): ns3::Ccnx::Ccnx(ns3::Ccnx const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Ccnx const &', 'arg0')])
    ## ccnx.h (module 'NDNabstraction'): uint32_t ns3::Ccnx::AddFace(ns3::Ptr<ns3::CcnxFace> const & face) [member function]
    cls.add_method('AddFace', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFace> ns3::Ccnx::GetFace(uint32_t face) const [member function]
    cls.add_method('GetFace', 
                   'ns3::Ptr< ns3::CcnxFace >', 
                   [param('uint32_t', 'face')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxForwardingStrategy> ns3::Ccnx::GetForwardingStrategy() const [member function]
    cls.add_method('GetForwardingStrategy', 
                   'ns3::Ptr< ns3::CcnxForwardingStrategy >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): uint32_t ns3::Ccnx::GetNFaces() const [member function]
    cls.add_method('GetNFaces', 
                   'uint32_t', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): static ns3::TypeId ns3::Ccnx::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx.h (module 'NDNabstraction'): void ns3::Ccnx::Receive(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::Packet const> const & p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::Packet const > const &', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): void ns3::Ccnx::Send(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::Packet> const & packet) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::Packet > const &', 'packet')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): void ns3::Ccnx::SetForwardingStrategy(ns3::Ptr<ns3::CcnxForwardingStrategy> forwardingStrategy) [member function]
    cls.add_method('SetForwardingStrategy', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxForwardingStrategy >', 'forwardingStrategy')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx.h (module 'NDNabstraction'): ns3::Ccnx::m_contentStore [variable]
    cls.add_instance_attribute('m_contentStore', 'ns3::Ptr< ns3::CcnxContentStore >', is_const=False)
    return

def register_Ns3CcnxConsumer_methods(root_module, cls):
    ## ccnx-consumer.h (module 'NDNabstraction'): ns3::CcnxConsumer::CcnxConsumer(ns3::CcnxConsumer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxConsumer const &', 'arg0')])
    ## ccnx-consumer.h (module 'NDNabstraction'): ns3::CcnxConsumer::CcnxConsumer() [constructor]
    cls.add_constructor([])
    ## ccnx-consumer.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxConsumer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-consumer.h (module 'NDNabstraction'): void ns3::CcnxConsumer::HandlePacket(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::Packet const> const & packet) [member function]
    cls.add_method('HandlePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::Packet const > const &', 'packet')])
    ## ccnx-consumer.h (module 'NDNabstraction'): void ns3::CcnxConsumer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ccnx-consumer.h (module 'NDNabstraction'): void ns3::CcnxConsumer::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ccnx-consumer.h (module 'NDNabstraction'): void ns3::CcnxConsumer::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3CcnxContentObjectHeader_methods(root_module, cls):
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxContentObjectHeader::CcnxContentObjectHeader(ns3::CcnxContentObjectHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxContentObjectHeader const &', 'arg0')])
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxContentObjectHeader::CcnxContentObjectHeader() [constructor]
    cls.add_constructor([])
    ## ccnx-content-object-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxContentObjectHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::TypeId ns3::CcnxContentObjectHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxContentObjectHeader::GetName() const [member function]
    cls.add_method('GetName', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxContentObjectHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxContentObjectHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): void ns3::CcnxContentObjectHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): void ns3::CcnxContentObjectHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): void ns3::CcnxContentObjectHeader::SetName(ns3::Ptr<ns3::CcnxNameComponents> const & name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxNameComponents > const &', 'name')])
    return

def register_Ns3CcnxContentObjectTail_methods(root_module, cls):
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxContentObjectTail::CcnxContentObjectTail(ns3::CcnxContentObjectTail const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxContentObjectTail const &', 'arg0')])
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::CcnxContentObjectTail::CcnxContentObjectTail() [constructor]
    cls.add_constructor([])
    ## ccnx-content-object-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxContentObjectTail::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): ns3::TypeId ns3::CcnxContentObjectTail::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxContentObjectTail::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxContentObjectTail::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): void ns3::CcnxContentObjectTail::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ccnx-content-object-header.h (module 'NDNabstraction'): void ns3::CcnxContentObjectTail::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3CcnxContentStore_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::CcnxContentStore::CcnxContentStore() [constructor]
    cls.add_constructor([])
    ## ccnx-content-store.h (module 'NDNabstraction'): void ns3::CcnxContentStore::Add(ns3::Ptr<ns3::CcnxContentObjectHeader> header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxContentObjectHeader >', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## ccnx-content-store.h (module 'NDNabstraction'): uint32_t ns3::CcnxContentStore::GetMaxSize() const [member function]
    cls.add_method('GetMaxSize', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ccnx-content-store.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxContentStore::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-content-store.h (module 'NDNabstraction'): ns3::Ptr<ns3::Packet> ns3::CcnxContentStore::Lookup(ns3::Ptr<const ns3::CcnxInterestHeader> interest) [member function]
    cls.add_method('Lookup', 
                   'ns3::Ptr< ns3::Packet >', 
                   [param('ns3::Ptr< ns3::CcnxInterestHeader const >', 'interest')])
    ## ccnx-content-store.h (module 'NDNabstraction'): void ns3::CcnxContentStore::Print() const [member function]
    cls.add_method('Print', 
                   'void', 
                   [], 
                   is_const=True)
    ## ccnx-content-store.h (module 'NDNabstraction'): void ns3::CcnxContentStore::SetMaxSize(uint32_t maxSize) [member function]
    cls.add_method('SetMaxSize', 
                   'void', 
                   [param('uint32_t', 'maxSize')])
    return

def register_Ns3CcnxFace_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## ccnx-face.h (module 'NDNabstraction'): ns3::CcnxFace::CcnxFace() [constructor]
    cls.add_constructor([])
    ## ccnx-face.h (module 'NDNabstraction'): uint32_t ns3::CcnxFace::GetId() const [member function]
    cls.add_method('GetId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ccnx-face.h (module 'NDNabstraction'): uint16_t ns3::CcnxFace::GetMetric() const [member function]
    cls.add_method('GetMetric', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxFace::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-face.h (module 'NDNabstraction'): bool ns3::CcnxFace::IsDown() const [member function]
    cls.add_method('IsDown', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): bool ns3::CcnxFace::IsUp() const [member function]
    cls.add_method('IsUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::CcnxFace> const&, ns3::Ptr<ns3::Packet const> const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::CcnxFace >, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'handler')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::SetDown() [member function]
    cls.add_method('SetDown', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::SetId(uint32_t id) [member function]
    cls.add_method('SetId', 
                   'void', 
                   [param('uint32_t', 'id')])
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::SetMetric(uint16_t metric) [member function]
    cls.add_method('SetMetric', 
                   'void', 
                   [param('uint16_t', 'metric')], 
                   is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::SetUp() [member function]
    cls.add_method('SetUp', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## ccnx-face.h (module 'NDNabstraction'): void ns3::CcnxFace::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3CcnxFaceContainer_methods(root_module, cls):
    ## ccnx-face-container.h (module 'NDNabstraction'): ns3::CcnxFaceContainer::CcnxFaceContainer() [constructor]
    cls.add_constructor([])
    ## ccnx-face-container.h (module 'NDNabstraction'): ns3::CcnxFaceContainer::CcnxFaceContainer(ns3::CcnxFaceContainer const & other) [copy constructor]
    cls.add_constructor([param('ns3::CcnxFaceContainer const &', 'other')])
    ## ccnx-face-container.h (module 'NDNabstraction'): void ns3::CcnxFaceContainer::Add(ns3::Ptr<ns3::CcnxFace> const & face) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face')])
    ## ccnx-face-container.h (module 'NDNabstraction'): void ns3::CcnxFaceContainer::AddAll(ns3::Ptr<ns3::CcnxFaceContainer> other) [member function]
    cls.add_method('AddAll', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFaceContainer >', 'other')])
    ## ccnx-face-container.h (module 'NDNabstraction'): void ns3::CcnxFaceContainer::AddAll(ns3::CcnxFaceContainer const & other) [member function]
    cls.add_method('AddAll', 
                   'void', 
                   [param('ns3::CcnxFaceContainer const &', 'other')])
    ## ccnx-face-container.h (module 'NDNabstraction'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::CcnxFace>*,std::vector<ns3::Ptr<ns3::CcnxFace>, std::allocator<ns3::Ptr<ns3::CcnxFace> > > > ns3::CcnxFaceContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::CcnxFace > const, std::vector< ns3::Ptr< ns3::CcnxFace > > >', 
                   [], 
                   is_const=True)
    ## ccnx-face-container.h (module 'NDNabstraction'): __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::CcnxFace>*,std::vector<ns3::Ptr<ns3::CcnxFace>, std::allocator<ns3::Ptr<ns3::CcnxFace> > > > ns3::CcnxFaceContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::CcnxFace > const, std::vector< ns3::Ptr< ns3::CcnxFace > > >', 
                   [], 
                   is_const=True)
    ## ccnx-face-container.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFace> ns3::CcnxFaceContainer::Get(__gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::CcnxFace>*,std::vector<ns3::Ptr<ns3::CcnxFace>, std::allocator<ns3::Ptr<ns3::CcnxFace> > > > i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::CcnxFace >', 
                   [param('__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::CcnxFace > const, std::vector< ns3::Ptr< ns3::CcnxFace > > >', 'i')], 
                   is_const=True)
    ## ccnx-face-container.h (module 'NDNabstraction'): uint32_t ns3::CcnxFaceContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ccnx-face-container.h (module 'NDNabstraction'): void ns3::CcnxFaceContainer::SetMetricToAll(uint16_t metric) [member function]
    cls.add_method('SetMetricToAll', 
                   'void', 
                   [param('uint16_t', 'metric')])
    return

def register_Ns3CcnxFibEntry_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibEntry::CcnxFibEntry(ns3::CcnxFibEntry const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxFibEntry const &', 'arg0')])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxFibEntry::CcnxFibEntry(ns3::Ptr<ns3::CcnxNameComponents> prefix) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CcnxNameComponents >', 'prefix')])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFace> ns3::CcnxFibEntry::FindBestCandidate(int skip=0) [member function]
    cls.add_method('FindBestCandidate', 
                   'ns3::Ptr< ns3::CcnxFace >', 
                   [param('int', 'skip', default_value='0')])
    ## ccnx-fib.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxFibEntry::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    ## ccnx-fib.h (module 'NDNabstraction'): void ns3::CcnxFibEntry::UpdateStatus(ns3::Ptr<ns3::CcnxFace> face, uint8_t status) [member function]
    cls.add_method('UpdateStatus', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace >', 'face'), param('uint8_t', 'status')])
    return

def register_Ns3CcnxForwardingStrategy_methods(root_module, cls):
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): ns3::CcnxForwardingStrategy::CcnxForwardingStrategy() [constructor]
    cls.add_constructor([])
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): ns3::CcnxForwardingStrategy::CcnxForwardingStrategy(ns3::CcnxForwardingStrategy const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxForwardingStrategy const &', 'arg0')])
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxForwardingStrategy::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): void ns3::CcnxForwardingStrategy::NotifyInterfaceDown(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceDown', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): void ns3::CcnxForwardingStrategy::NotifyInterfaceUp(uint32_t interface) [member function]
    cls.add_method('NotifyInterfaceUp', 
                   'void', 
                   [param('uint32_t', 'interface')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): void ns3::CcnxForwardingStrategy::PrintForwardingTable(ns3::Ptr<ns3::OutputStreamWrapper> stream) const [member function]
    cls.add_method('PrintForwardingTable', 
                   'void', 
                   [param('ns3::Ptr< ns3::OutputStreamWrapper >', 'stream')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): bool ns3::CcnxForwardingStrategy::RouteInput(ns3::Ptr<ns3::Packet> p, ns3::Ptr<ns3::CcnxFace> iface, ns3::Callback<void,ns3::Ptr<ns3::Packet>,ns3::Ptr<ns3::CcnxRoute>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ucb, ns3::Callback<void,ns3::Ptr<ns3::Packet>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ecb) [member function]
    cls.add_method('RouteInput', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Ptr< ns3::CcnxFace >', 'iface'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::Ptr< ns3::CcnxRoute >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ucb'), param('ns3::Callback< void, ns3::Ptr< ns3::Packet >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'ecb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnx-forwarding-strategy.h (module 'NDNabstraction'): void ns3::CcnxForwardingStrategy::SetCcnx(ns3::Ptr<ns3::Ccnx> ccnx) [member function]
    cls.add_method('SetCcnx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Ccnx >', 'ccnx')], 
                   is_virtual=True)
    return

def register_Ns3CcnxInterestHeader_methods(root_module, cls):
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::CcnxInterestHeader::CcnxInterestHeader(ns3::CcnxInterestHeader const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxInterestHeader const &', 'arg0')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::CcnxInterestHeader::CcnxInterestHeader() [constructor]
    cls.add_constructor([])
    ## ccnx-interest-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxInterestHeader::Deserialize(ns3::Buffer::Iterator start) [member function]
    cls.add_method('Deserialize', 
                   'uint32_t', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_virtual=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxInterestHeader::GetExclude() const [member function]
    cls.add_method('GetExclude', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::TypeId ns3::CcnxInterestHeader::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::Time ns3::CcnxInterestHeader::GetInterestLifetime() const [member function]
    cls.add_method('GetInterestLifetime', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): int32_t ns3::CcnxInterestHeader::GetMaxSuffixComponents() const [member function]
    cls.add_method('GetMaxSuffixComponents', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): int32_t ns3::CcnxInterestHeader::GetMinSuffixComponents() const [member function]
    cls.add_method('GetMinSuffixComponents', 
                   'int32_t', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxInterestHeader::GetName() const [member function]
    cls.add_method('GetName', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxInterestHeader::GetNonce() const [member function]
    cls.add_method('GetNonce', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): int8_t ns3::CcnxInterestHeader::GetScope() const [member function]
    cls.add_method('GetScope', 
                   'int8_t', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): uint32_t ns3::CcnxInterestHeader::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxInterestHeader::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): bool ns3::CcnxInterestHeader::IsEnabledAnswerOriginKind() const [member function]
    cls.add_method('IsEnabledAnswerOriginKind', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): bool ns3::CcnxInterestHeader::IsEnabledChildSelector() const [member function]
    cls.add_method('IsEnabledChildSelector', 
                   'bool', 
                   [], 
                   is_const=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::Serialize(ns3::Buffer::Iterator start) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::Buffer::Iterator', 'start')], 
                   is_const=True, is_virtual=True)
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetAnswerOriginKind(bool value) [member function]
    cls.add_method('SetAnswerOriginKind', 
                   'void', 
                   [param('bool', 'value')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetChildSelector(bool value) [member function]
    cls.add_method('SetChildSelector', 
                   'void', 
                   [param('bool', 'value')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetExclude(ns3::Ptr<ns3::CcnxNameComponents> const & exclude) [member function]
    cls.add_method('SetExclude', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxNameComponents > const &', 'exclude')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetInterestLifetime(ns3::Time time) [member function]
    cls.add_method('SetInterestLifetime', 
                   'void', 
                   [param('ns3::Time', 'time')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetMaxSuffixComponents(int32_t value) [member function]
    cls.add_method('SetMaxSuffixComponents', 
                   'void', 
                   [param('int32_t', 'value')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetMinSuffixComponents(int32_t value) [member function]
    cls.add_method('SetMinSuffixComponents', 
                   'void', 
                   [param('int32_t', 'value')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetName(ns3::Ptr<ns3::CcnxNameComponents> const & name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxNameComponents > const &', 'name')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetNonce(uint32_t nonce) [member function]
    cls.add_method('SetNonce', 
                   'void', 
                   [param('uint32_t', 'nonce')])
    ## ccnx-interest-header.h (module 'NDNabstraction'): void ns3::CcnxInterestHeader::SetScope(int8_t scope) [member function]
    cls.add_method('SetScope', 
                   'void', 
                   [param('int8_t', 'scope')])
    return

def register_Ns3CcnxInterestSender_methods(root_module, cls):
    ## ccnx-interest-sender.h (module 'NDNabstraction'): ns3::CcnxInterestSender::CcnxInterestSender(ns3::CcnxInterestSender const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxInterestSender const &', 'arg0')])
    ## ccnx-interest-sender.h (module 'NDNabstraction'): ns3::CcnxInterestSender::CcnxInterestSender() [constructor]
    cls.add_constructor([])
    ## ccnx-interest-sender.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxInterestSender::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-interest-sender.h (module 'NDNabstraction'): void ns3::CcnxInterestSender::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ccnx-interest-sender.h (module 'NDNabstraction'): void ns3::CcnxInterestSender::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ccnx-interest-sender.h (module 'NDNabstraction'): void ns3::CcnxInterestSender::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3CcnxL3Protocol_methods(root_module, cls):
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::CcnxL3Protocol::ETHERNET_FRAME_TYPE [variable]
    cls.add_static_attribute('ETHERNET_FRAME_TYPE', 'uint16_t const', is_const=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::CcnxL3Protocol::IP_PROTOCOL_TYPE [variable]
    cls.add_static_attribute('IP_PROTOCOL_TYPE', 'uint16_t const', is_const=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::CcnxL3Protocol::UDP_PORT [variable]
    cls.add_static_attribute('UDP_PORT', 'uint16_t const', is_const=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxL3Protocol::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::CcnxL3Protocol::CcnxL3Protocol() [constructor]
    cls.add_constructor([])
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')])
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::SetForwardingStrategy(ns3::Ptr<ns3::CcnxForwardingStrategy> forwardingStrategy) [member function]
    cls.add_method('SetForwardingStrategy', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxForwardingStrategy >', 'forwardingStrategy')], 
                   is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxForwardingStrategy> ns3::CcnxL3Protocol::GetForwardingStrategy() const [member function]
    cls.add_method('GetForwardingStrategy', 
                   'ns3::Ptr< ns3::CcnxForwardingStrategy >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::Send(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::Packet> const & packet) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::Packet > const &', 'packet')], 
                   is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::Receive(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::Packet const> const & p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::Packet const > const &', 'p')], 
                   is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): uint32_t ns3::CcnxL3Protocol::AddFace(ns3::Ptr<ns3::CcnxFace> const & face) [member function]
    cls.add_method('AddFace', 
                   'uint32_t', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face')], 
                   is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): uint32_t ns3::CcnxL3Protocol::GetNFaces() const [member function]
    cls.add_method('GetNFaces', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFace> ns3::CcnxL3Protocol::GetFace(uint32_t face) const [member function]
    cls.add_method('GetFace', 
                   'ns3::Ptr< ns3::CcnxFace >', 
                   [param('uint32_t', 'face')], 
                   is_const=True, is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::ReceiveAndProcess(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::CcnxInterestHeader> const & header, ns3::Ptr<ns3::Packet> const & p) [member function]
    cls.add_method('ReceiveAndProcess', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::CcnxInterestHeader > const &', 'header'), param('ns3::Ptr< ns3::Packet > const &', 'p')], 
                   visibility='protected', is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::ReceiveAndProcess(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::CcnxContentObjectHeader> const & header, ns3::Ptr<ns3::Packet> const & p) [member function]
    cls.add_method('ReceiveAndProcess', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::CcnxContentObjectHeader > const &', 'header'), param('ns3::Ptr< ns3::Packet > const &', 'p')], 
                   visibility='protected', is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ccnx-l3-protocol.h (module 'NDNabstraction'): void ns3::CcnxL3Protocol::ReceiveAndProcess(ns3::Ptr<ns3::CcnxFace> face, ns3::Ptr<ns3::Header> header, ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('ReceiveAndProcess', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace >', 'face'), param('ns3::Ptr< ns3::Header >', 'header'), param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3CcnxLocalFace_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-local-face.h (module 'NDNabstraction'): ns3::CcnxLocalFace::CcnxLocalFace() [constructor]
    cls.add_constructor([])
    ## ccnx-local-face.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxLocalFace::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-local-face.h (module 'NDNabstraction'): void ns3::CcnxLocalFace::Receive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## ccnx-local-face.h (module 'NDNabstraction'): void ns3::CcnxLocalFace::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::CcnxFace> const&, ns3::Ptr<ns3::Packet const> const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::CcnxFace >, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'handler')], 
                   is_virtual=True)
    ## ccnx-local-face.h (module 'NDNabstraction'): void ns3::CcnxLocalFace::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## ccnx-local-face.h (module 'NDNabstraction'): void ns3::CcnxLocalFace::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3CcnxNameComponents_methods(root_module, cls):
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('<')
    cls.add_binary_comparison_operator('==')
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents::CcnxNameComponents(ns3::CcnxNameComponents const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxNameComponents const &', 'arg0')])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents::CcnxNameComponents() [constructor]
    cls.add_constructor([])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents::CcnxNameComponents(std::string const & s) [constructor]
    cls.add_constructor([param('std::string const &', 's')])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents::CcnxNameComponents(std::list<boost::reference_wrapper<std::string const>, std::allocator<boost::reference_wrapper<std::string const> > > const & components) [constructor]
    cls.add_constructor([param('std::list< boost::reference_wrapper< std::string const > > const &', 'components')])
    ## ccnx-name-components.h (module 'NDNabstraction'): void ns3::CcnxNameComponents::Add(std::string const & s) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string const &', 's')])
    ## ccnx-name-components.h (module 'NDNabstraction'): std::list<std::string, std::allocator<std::string> > const & ns3::CcnxNameComponents::GetComponents() const [member function]
    cls.add_method('GetComponents', 
                   'std::list< std::string > const &', 
                   [], 
                   is_const=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): std::list<boost::reference_wrapper<std::string const>, std::allocator<boost::reference_wrapper<std::string const> > > ns3::CcnxNameComponents::GetSubComponents(size_t num) const [member function]
    cls.add_method('GetSubComponents', 
                   'std::list< boost::reference_wrapper< std::string const > >', 
                   [param('size_t', 'num')], 
                   is_const=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): void ns3::CcnxNameComponents::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): size_t ns3::CcnxNameComponents::size() const [member function]
    cls.add_method('size', 
                   'size_t', 
                   [], 
                   is_const=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents & ns3::CcnxNameComponents::operator()(std::string const & s) [member operator]
    cls.add_method('operator()', 
                   'ns3::CcnxNameComponents &', 
                   [param('std::string const &', 's')], 
                   custom_name='__call__')
    return

def register_Ns3CcnxNameComponentsChecker_methods(root_module, cls):
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsChecker::CcnxNameComponentsChecker() [constructor]
    cls.add_constructor([])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsChecker::CcnxNameComponentsChecker(ns3::CcnxNameComponentsChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxNameComponentsChecker const &', 'arg0')])
    return

def register_Ns3CcnxNameComponentsValue_methods(root_module, cls):
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsValue::CcnxNameComponentsValue() [constructor]
    cls.add_constructor([])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsValue::CcnxNameComponentsValue(ns3::CcnxNameComponentsValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxNameComponentsValue const &', 'arg0')])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponentsValue::CcnxNameComponentsValue(ns3::CcnxNameComponents const & value) [constructor]
    cls.add_constructor([param('ns3::CcnxNameComponents const &', 'value')])
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::Ptr<ns3::AttributeValue> ns3::CcnxNameComponentsValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): bool ns3::CcnxNameComponentsValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): ns3::CcnxNameComponents ns3::CcnxNameComponentsValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::CcnxNameComponents', 
                   [], 
                   is_const=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): std::string ns3::CcnxNameComponentsValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## ccnx-name-components.h (module 'NDNabstraction'): void ns3::CcnxNameComponentsValue::Set(ns3::CcnxNameComponents const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::CcnxNameComponents const &', 'value')])
    return

def register_Ns3CcnxNetDeviceFace_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-net-device-face.h (module 'NDNabstraction'): ns3::CcnxNetDeviceFace::CcnxNetDeviceFace(ns3::Ptr<ns3::NetDevice> const & netDevice) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::NetDevice > const &', 'netDevice')])
    ## ccnx-net-device-face.h (module 'NDNabstraction'): ns3::Ptr<ns3::NetDevice> ns3::CcnxNetDeviceFace::GetNetDevice() const [member function]
    cls.add_method('GetNetDevice', 
                   'ns3::Ptr< ns3::NetDevice >', 
                   [], 
                   is_const=True)
    ## ccnx-net-device-face.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxNetDeviceFace::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-net-device-face.h (module 'NDNabstraction'): void ns3::CcnxNetDeviceFace::RegisterProtocolHandler(ns3::Callback<void, ns3::Ptr<ns3::CcnxFace> const&, ns3::Ptr<ns3::Packet const> const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> handler) [member function]
    cls.add_method('RegisterProtocolHandler', 
                   'void', 
                   [param('ns3::Callback< void, ns3::Ptr< ns3::CcnxFace >, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'handler')], 
                   is_virtual=True)
    ## ccnx-net-device-face.h (module 'NDNabstraction'): void ns3::CcnxNetDeviceFace::Send(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Send', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_virtual=True)
    ## ccnx-net-device-face.h (module 'NDNabstraction'): void ns3::CcnxNetDeviceFace::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3CcnxPit_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPit::CcnxPit(ns3::CcnxPit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxPit const &', 'arg0')])
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPit::CcnxPit() [constructor]
    cls.add_constructor([])
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::Time ns3::CcnxPit::GetCleanupTimeout() const [member function]
    cls.add_method('GetCleanupTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ccnx-pit.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxPit::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntry const & ns3::CcnxPit::Lookup(ns3::CcnxContentObjectHeader const & header) const [member function]
    cls.add_method('Lookup', 
                   'ns3::CcnxPitEntry const &', 
                   [param('ns3::CcnxContentObjectHeader const &', 'header')], 
                   is_const=True)
    ## ccnx-pit.h (module 'NDNabstraction'): ns3::CcnxPitEntry const & ns3::CcnxPit::Lookup(ns3::CcnxInterestHeader const & header) const [member function]
    cls.add_method('Lookup', 
                   'ns3::CcnxPitEntry const &', 
                   [param('ns3::CcnxInterestHeader const &', 'header')], 
                   is_const=True)
    ## ccnx-pit.h (module 'NDNabstraction'): void ns3::CcnxPit::SetCleanupTimeout(ns3::Time const & timeout) [member function]
    cls.add_method('SetCleanupTimeout', 
                   'void', 
                   [param('ns3::Time const &', 'timeout')])
    return

def register_Ns3CcnxProducer_methods(root_module, cls):
    ## ccnx-producer.h (module 'NDNabstraction'): ns3::CcnxProducer::CcnxProducer(ns3::CcnxProducer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxProducer const &', 'arg0')])
    ## ccnx-producer.h (module 'NDNabstraction'): ns3::CcnxProducer::CcnxProducer() [constructor]
    cls.add_constructor([])
    ## ccnx-producer.h (module 'NDNabstraction'): void ns3::CcnxProducer::AddContentStoreEntry(ns3::Ptr<ns3::CcnxContentObjectHeader> header, ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('AddContentStoreEntry', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxContentObjectHeader >', 'header'), param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## ccnx-producer.h (module 'NDNabstraction'): uint32_t ns3::CcnxProducer::GetStoreCapacity() [member function]
    cls.add_method('GetStoreCapacity', 
                   'uint32_t', 
                   [])
    ## ccnx-producer.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxProducer::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-producer.h (module 'NDNabstraction'): void ns3::CcnxProducer::HandlePacket(ns3::Ptr<ns3::CcnxFace> const & face, ns3::Ptr<ns3::Packet const> const & packet) [member function]
    cls.add_method('HandlePacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace > const &', 'face'), param('ns3::Ptr< ns3::Packet const > const &', 'packet')])
    ## ccnx-producer.h (module 'NDNabstraction'): void ns3::CcnxProducer::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## ccnx-producer.h (module 'NDNabstraction'): void ns3::CcnxProducer::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## ccnx-producer.h (module 'NDNabstraction'): void ns3::CcnxProducer::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3CcnxRit_methods(root_module, cls):
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRit::CcnxRit(ns3::CcnxRit const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxRit const &', 'arg0')])
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::CcnxRit::CcnxRit() [constructor]
    cls.add_constructor([])
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::Time ns3::CcnxRit::GetCleanupTimeout() const [member function]
    cls.add_method('GetCleanupTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ccnx-rit.h (module 'NDNabstraction'): ns3::Time ns3::CcnxRit::GetRitTimeout() const [member function]
    cls.add_method('GetRitTimeout', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## ccnx-rit.h (module 'NDNabstraction'): static ns3::TypeId ns3::CcnxRit::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## ccnx-rit.h (module 'NDNabstraction'): void ns3::CcnxRit::SetCleanupTimeout(ns3::Time const & timeout) [member function]
    cls.add_method('SetCleanupTimeout', 
                   'void', 
                   [param('ns3::Time const &', 'timeout')])
    ## ccnx-rit.h (module 'NDNabstraction'): void ns3::CcnxRit::SetRecentlySatisfied(ns3::CcnxInterestHeader const & header) [member function]
    cls.add_method('SetRecentlySatisfied', 
                   'void', 
                   [param('ns3::CcnxInterestHeader const &', 'header')])
    ## ccnx-rit.h (module 'NDNabstraction'): void ns3::CcnxRit::SetRitTimeout(ns3::Time const & timeout) [member function]
    cls.add_method('SetRitTimeout', 
                   'void', 
                   [param('ns3::Time const &', 'timeout')])
    ## ccnx-rit.h (module 'NDNabstraction'): bool ns3::CcnxRit::WasRecentlySatisfied(ns3::CcnxInterestHeader const & header) [member function]
    cls.add_method('WasRecentlySatisfied', 
                   'bool', 
                   [param('ns3::CcnxInterestHeader const &', 'header')])
    return

def register_Ns3CcnxRoute_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## ccnx-route.h (module 'NDNabstraction'): ns3::CcnxRoute::CcnxRoute(ns3::CcnxRoute const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnxRoute const &', 'arg0')])
    ## ccnx-route.h (module 'NDNabstraction'): ns3::CcnxRoute::CcnxRoute() [constructor]
    cls.add_constructor([])
    ## ccnx-route.h (module 'NDNabstraction'): ns3::Ptr<ns3::CcnxFace> ns3::CcnxRoute::GetOutputFace() const [member function]
    cls.add_method('GetOutputFace', 
                   'ns3::Ptr< ns3::CcnxFace >', 
                   [], 
                   is_const=True)
    ## ccnx-route.h (module 'NDNabstraction'): ns3::CcnxNameComponents const & ns3::CcnxRoute::GetPrefix() const [member function]
    cls.add_method('GetPrefix', 
                   'ns3::CcnxNameComponents const &', 
                   [], 
                   is_const=True)
    ## ccnx-route.h (module 'NDNabstraction'): void ns3::CcnxRoute::SetOutputFace(ns3::Ptr<ns3::CcnxFace> outputDevice) [member function]
    cls.add_method('SetOutputFace', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxFace >', 'outputDevice')])
    ## ccnx-route.h (module 'NDNabstraction'): void ns3::CcnxRoute::SetPrefix(ns3::Ptr<ns3::CcnxNameComponents> const & dest) [member function]
    cls.add_method('SetPrefix', 
                   'void', 
                   [param('ns3::Ptr< ns3::CcnxNameComponents > const &', 'dest')])
    return

def register_Ns3DataRateChecker_methods(root_module, cls):
    ## data-rate.h (module 'network'): ns3::DataRateChecker::DataRateChecker() [constructor]
    cls.add_constructor([])
    ## data-rate.h (module 'network'): ns3::DataRateChecker::DataRateChecker(ns3::DataRateChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateChecker const &', 'arg0')])
    return

def register_Ns3DataRateValue_methods(root_module, cls):
    ## data-rate.h (module 'network'): ns3::DataRateValue::DataRateValue() [constructor]
    cls.add_constructor([])
    ## data-rate.h (module 'network'): ns3::DataRateValue::DataRateValue(ns3::DataRateValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DataRateValue const &', 'arg0')])
    ## data-rate.h (module 'network'): ns3::DataRateValue::DataRateValue(ns3::DataRate const & value) [constructor]
    cls.add_constructor([param('ns3::DataRate const &', 'value')])
    ## data-rate.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::DataRateValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## data-rate.h (module 'network'): bool ns3::DataRateValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## data-rate.h (module 'network'): ns3::DataRate ns3::DataRateValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::DataRate', 
                   [], 
                   is_const=True)
    ## data-rate.h (module 'network'): std::string ns3::DataRateValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## data-rate.h (module 'network'): void ns3::DataRateValue::Set(ns3::DataRate const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::DataRate const &', 'value')])
    return

def register_Ns3DropTailQueue_methods(root_module, cls):
    ## drop-tail-queue.h (module 'network'): ns3::DropTailQueue::DropTailQueue(ns3::DropTailQueue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DropTailQueue const &', 'arg0')])
    ## drop-tail-queue.h (module 'network'): ns3::DropTailQueue::DropTailQueue() [constructor]
    cls.add_constructor([])
    ## drop-tail-queue.h (module 'network'): ns3::DropTailQueue::Mode ns3::DropTailQueue::GetMode() [member function]
    cls.add_method('GetMode', 
                   'ns3::DropTailQueue::Mode', 
                   [])
    ## drop-tail-queue.h (module 'network'): static ns3::TypeId ns3::DropTailQueue::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## drop-tail-queue.h (module 'network'): void ns3::DropTailQueue::SetMode(ns3::DropTailQueue::Mode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::DropTailQueue::Mode', 'mode')])
    ## drop-tail-queue.h (module 'network'): ns3::Ptr<ns3::Packet> ns3::DropTailQueue::DoDequeue() [member function]
    cls.add_method('DoDequeue', 
                   'ns3::Ptr< ns3::Packet >', 
                   [], 
                   visibility='private', is_virtual=True)
    ## drop-tail-queue.h (module 'network'): bool ns3::DropTailQueue::DoEnqueue(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoEnqueue', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='private', is_virtual=True)
    ## drop-tail-queue.h (module 'network'): ns3::Ptr<ns3::Packet const> ns3::DropTailQueue::DoPeek() const [member function]
    cls.add_method('DoPeek', 
                   'ns3::Ptr< ns3::Packet const >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
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

def register_Ns3Mac48AddressChecker_methods(root_module, cls):
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker::Mac48AddressChecker() [constructor]
    cls.add_constructor([])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressChecker::Mac48AddressChecker(ns3::Mac48AddressChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressChecker const &', 'arg0')])
    return

def register_Ns3Mac48AddressValue_methods(root_module, cls):
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue::Mac48AddressValue() [constructor]
    cls.add_constructor([])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48AddressValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Mac48AddressValue const &', 'arg0')])
    ## mac48-address.h (module 'network'): ns3::Mac48AddressValue::Mac48AddressValue(ns3::Mac48Address const & value) [constructor]
    cls.add_constructor([param('ns3::Mac48Address const &', 'value')])
    ## mac48-address.h (module 'network'): ns3::Ptr<ns3::AttributeValue> ns3::Mac48AddressValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h (module 'network'): bool ns3::Mac48AddressValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## mac48-address.h (module 'network'): ns3::Mac48Address ns3::Mac48AddressValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Mac48Address', 
                   [], 
                   is_const=True)
    ## mac48-address.h (module 'network'): std::string ns3::Mac48AddressValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## mac48-address.h (module 'network'): void ns3::Mac48AddressValue::Set(ns3::Mac48Address const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Mac48Address const &', 'value')])
    return

def register_Ns3NetDevice_methods(root_module, cls):
    ## net-device.h (module 'network'): ns3::NetDevice::NetDevice() [constructor]
    cls.add_constructor([])
    ## net-device.h (module 'network'): ns3::NetDevice::NetDevice(ns3::NetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NetDevice const &', 'arg0')])
    ## net-device.h (module 'network'): void ns3::NetDevice::AddLinkChangeCallback(ns3::Callback<void,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
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
    ## net-device.h (module 'network'): void ns3::NetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_pure_virtual=True, is_virtual=True)
    ## net-device.h (module 'network'): void ns3::NetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
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
    ## node.h (module 'network'): void ns3::Node::DoStart() [member function]
    cls.add_method('DoStart', 
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

def register_Ns3OutputStreamWrapper_methods(root_module, cls):
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper::OutputStreamWrapper(ns3::OutputStreamWrapper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::OutputStreamWrapper const &', 'arg0')])
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper::OutputStreamWrapper(std::string filename, std::_Ios_Openmode filemode) [constructor]
    cls.add_constructor([param('std::string', 'filename'), param('std::_Ios_Openmode', 'filemode')])
    ## output-stream-wrapper.h (module 'network'): ns3::OutputStreamWrapper::OutputStreamWrapper(std::ostream * os) [constructor]
    cls.add_constructor([param('std::ostream *', 'os')])
    ## output-stream-wrapper.h (module 'network'): std::ostream * ns3::OutputStreamWrapper::GetStream() [member function]
    cls.add_method('GetStream', 
                   'std::ostream *', 
                   [])
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
    ## packet.h (module 'network'): uint32_t ns3::Packet::Serialize(uint8_t * buffer, uint32_t maxSize) const [member function]
    cls.add_method('Serialize', 
                   'uint32_t', 
                   [param('uint8_t *', 'buffer'), param('uint32_t', 'maxSize')], 
                   is_const=True)
    ## packet.h (module 'network'): void ns3::Packet::SetNixVector(ns3::Ptr<ns3::NixVector> arg0) [member function]
    cls.add_method('SetNixVector', 
                   'void', 
                   [param('ns3::Ptr< ns3::NixVector >', 'arg0')])
    return

def register_Ns3PointToPointNetDevice_methods(root_module, cls):
    ## point-to-point-net-device.h (module 'point-to-point'): static ns3::TypeId ns3::PointToPointNetDevice::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::PointToPointNetDevice::PointToPointNetDevice() [constructor]
    cls.add_constructor([])
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetDataRate(ns3::DataRate bps) [member function]
    cls.add_method('SetDataRate', 
                   'void', 
                   [param('ns3::DataRate', 'bps')])
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetInterframeGap(ns3::Time t) [member function]
    cls.add_method('SetInterframeGap', 
                   'void', 
                   [param('ns3::Time', 't')])
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::Attach(ns3::Ptr<ns3::PointToPointChannel> ch) [member function]
    cls.add_method('Attach', 
                   'bool', 
                   [param('ns3::Ptr< ns3::PointToPointChannel >', 'ch')])
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetQueue(ns3::Ptr<ns3::Queue> queue) [member function]
    cls.add_method('SetQueue', 
                   'void', 
                   [param('ns3::Ptr< ns3::Queue >', 'queue')])
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Ptr<ns3::Queue> ns3::PointToPointNetDevice::GetQueue() const [member function]
    cls.add_method('GetQueue', 
                   'ns3::Ptr< ns3::Queue >', 
                   [], 
                   is_const=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetReceiveErrorModel(ns3::Ptr<ns3::ErrorModel> em) [member function]
    cls.add_method('SetReceiveErrorModel', 
                   'void', 
                   [param('ns3::Ptr< ns3::ErrorModel >', 'em')])
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::Receive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('Receive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetIfIndex(uint32_t const index) [member function]
    cls.add_method('SetIfIndex', 
                   'void', 
                   [param('uint32_t const', 'index')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): uint32_t ns3::PointToPointNetDevice::GetIfIndex() const [member function]
    cls.add_method('GetIfIndex', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Ptr<ns3::Channel> ns3::PointToPointNetDevice::GetChannel() const [member function]
    cls.add_method('GetChannel', 
                   'ns3::Ptr< ns3::Channel >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetAddress(ns3::Address address) [member function]
    cls.add_method('SetAddress', 
                   'void', 
                   [param('ns3::Address', 'address')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Address ns3::PointToPointNetDevice::GetAddress() const [member function]
    cls.add_method('GetAddress', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::SetMtu(uint16_t const mtu) [member function]
    cls.add_method('SetMtu', 
                   'bool', 
                   [param('uint16_t const', 'mtu')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): uint16_t ns3::PointToPointNetDevice::GetMtu() const [member function]
    cls.add_method('GetMtu', 
                   'uint16_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::IsLinkUp() const [member function]
    cls.add_method('IsLinkUp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::AddLinkChangeCallback(ns3::Callback<void,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> callback) [member function]
    cls.add_method('AddLinkChangeCallback', 
                   'void', 
                   [param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::IsBroadcast() const [member function]
    cls.add_method('IsBroadcast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Address ns3::PointToPointNetDevice::GetBroadcast() const [member function]
    cls.add_method('GetBroadcast', 
                   'ns3::Address', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::IsMulticast() const [member function]
    cls.add_method('IsMulticast', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Address ns3::PointToPointNetDevice::GetMulticast(ns3::Ipv4Address multicastGroup) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv4Address', 'multicastGroup')], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::IsPointToPoint() const [member function]
    cls.add_method('IsPointToPoint', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::IsBridge() const [member function]
    cls.add_method('IsBridge', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::Send(ns3::Ptr<ns3::Packet> packet, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('Send', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::SendFrom(ns3::Ptr<ns3::Packet> packet, ns3::Address const & source, ns3::Address const & dest, uint16_t protocolNumber) [member function]
    cls.add_method('SendFrom', 
                   'bool', 
                   [param('ns3::Ptr< ns3::Packet >', 'packet'), param('ns3::Address const &', 'source'), param('ns3::Address const &', 'dest'), param('uint16_t', 'protocolNumber')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Ptr<ns3::Node> ns3::PointToPointNetDevice::GetNode() const [member function]
    cls.add_method('GetNode', 
                   'ns3::Ptr< ns3::Node >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetNode(ns3::Ptr<ns3::Node> node) [member function]
    cls.add_method('SetNode', 
                   'void', 
                   [param('ns3::Ptr< ns3::Node >', 'node')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::NeedsArp() const [member function]
    cls.add_method('NeedsArp', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): ns3::Address ns3::PointToPointNetDevice::GetMulticast(ns3::Ipv6Address addr) const [member function]
    cls.add_method('GetMulticast', 
                   'ns3::Address', 
                   [param('ns3::Ipv6Address', 'addr')], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::SetPromiscReceiveCallback(ns3::Callback<bool, ns3::Ptr<ns3::NetDevice>, ns3::Ptr<ns3::Packet const>, unsigned short, ns3::Address const&, ns3::Address const&, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty> cb) [member function]
    cls.add_method('SetPromiscReceiveCallback', 
                   'void', 
                   [param('ns3::Callback< bool, ns3::Ptr< ns3::NetDevice >, ns3::Ptr< ns3::Packet const >, unsigned short, ns3::Address const &, ns3::Address const &, ns3::NetDevice::PacketType, ns3::empty, ns3::empty, ns3::empty >', 'cb')], 
                   is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): bool ns3::PointToPointNetDevice::SupportsSendFrom() const [member function]
    cls.add_method('SupportsSendFrom', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::DoMpiReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoMpiReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   visibility='protected')
    ## point-to-point-net-device.h (module 'point-to-point'): void ns3::PointToPointNetDevice::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3PointerChecker_methods(root_module, cls):
    ## pointer.h (module 'core'): ns3::PointerChecker::PointerChecker() [constructor]
    cls.add_constructor([])
    ## pointer.h (module 'core'): ns3::PointerChecker::PointerChecker(ns3::PointerChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointerChecker const &', 'arg0')])
    ## pointer.h (module 'core'): ns3::TypeId ns3::PointerChecker::GetPointeeTypeId() const [member function]
    cls.add_method('GetPointeeTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3PointerValue_methods(root_module, cls):
    ## pointer.h (module 'core'): ns3::PointerValue::PointerValue(ns3::PointerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointerValue const &', 'arg0')])
    ## pointer.h (module 'core'): ns3::PointerValue::PointerValue() [constructor]
    cls.add_constructor([])
    ## pointer.h (module 'core'): ns3::PointerValue::PointerValue(ns3::Ptr<ns3::Object> object) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Object >', 'object')])
    ## pointer.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::PointerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## pointer.h (module 'core'): bool ns3::PointerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## pointer.h (module 'core'): ns3::Ptr<ns3::Object> ns3::PointerValue::GetObject() const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## pointer.h (module 'core'): std::string ns3::PointerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## pointer.h (module 'core'): void ns3::PointerValue::SetObject(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('SetObject', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'object')])
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

def register_Ns3StringChecker_methods(root_module, cls):
    ## string.h (module 'core'): ns3::StringChecker::StringChecker() [constructor]
    cls.add_constructor([])
    ## string.h (module 'core'): ns3::StringChecker::StringChecker(ns3::StringChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StringChecker const &', 'arg0')])
    return

def register_Ns3StringValue_methods(root_module, cls):
    ## string.h (module 'core'): ns3::StringValue::StringValue() [constructor]
    cls.add_constructor([])
    ## string.h (module 'core'): ns3::StringValue::StringValue(ns3::StringValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StringValue const &', 'arg0')])
    ## string.h (module 'core'): ns3::StringValue::StringValue(std::string const & value) [constructor]
    cls.add_constructor([param('std::string const &', 'value')])
    ## string.h (module 'core'): ns3::Ptr<ns3::AttributeValue> ns3::StringValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## string.h (module 'core'): bool ns3::StringValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## string.h (module 'core'): std::string ns3::StringValue::Get() const [member function]
    cls.add_method('Get', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## string.h (module 'core'): std::string ns3::StringValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## string.h (module 'core'): void ns3::StringValue::Set(std::string const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string const &', 'value')])
    return

def register_Ns3StupidInterestGenerator_methods(root_module, cls):
    ## stupid-interest-generator.h (module 'NDNabstraction'): ns3::StupidInterestGenerator::StupidInterestGenerator(ns3::StupidInterestGenerator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StupidInterestGenerator const &', 'arg0')])
    ## stupid-interest-generator.h (module 'NDNabstraction'): ns3::StupidInterestGenerator::StupidInterestGenerator() [constructor]
    cls.add_constructor([])
    ## stupid-interest-generator.h (module 'NDNabstraction'): static ns3::TypeId ns3::StupidInterestGenerator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## stupid-interest-generator.h (module 'NDNabstraction'): void ns3::StupidInterestGenerator::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## stupid-interest-generator.h (module 'NDNabstraction'): void ns3::StupidInterestGenerator::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## stupid-interest-generator.h (module 'NDNabstraction'): void ns3::StupidInterestGenerator::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3StupidInterestSink_methods(root_module, cls):
    ## stupid-interest-sink.h (module 'NDNabstraction'): ns3::StupidInterestSink::StupidInterestSink(ns3::StupidInterestSink const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StupidInterestSink const &', 'arg0')])
    ## stupid-interest-sink.h (module 'NDNabstraction'): ns3::StupidInterestSink::StupidInterestSink() [constructor]
    cls.add_constructor([])
    ## stupid-interest-sink.h (module 'NDNabstraction'): std::list<ns3::Ptr<ns3::Socket>, std::allocator<ns3::Ptr<ns3::Socket> > > ns3::StupidInterestSink::GetAcceptedSockets() const [member function]
    cls.add_method('GetAcceptedSockets', 
                   'std::list< ns3::Ptr< ns3::Socket > >', 
                   [], 
                   is_const=True)
    ## stupid-interest-sink.h (module 'NDNabstraction'): ns3::Ptr<ns3::Socket> ns3::StupidInterestSink::GetListeningSocket() const [member function]
    cls.add_method('GetListeningSocket', 
                   'ns3::Ptr< ns3::Socket >', 
                   [], 
                   is_const=True)
    ## stupid-interest-sink.h (module 'NDNabstraction'): uint32_t ns3::StupidInterestSink::GetTotalRx() const [member function]
    cls.add_method('GetTotalRx', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## stupid-interest-sink.h (module 'NDNabstraction'): static ns3::TypeId ns3::StupidInterestSink::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## stupid-interest-sink.h (module 'NDNabstraction'): void ns3::StupidInterestSink::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## stupid-interest-sink.h (module 'NDNabstraction'): void ns3::StupidInterestSink::StartApplication() [member function]
    cls.add_method('StartApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## stupid-interest-sink.h (module 'NDNabstraction'): void ns3::StupidInterestSink::StopApplication() [member function]
    cls.add_method('StopApplication', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    return

def register_Ns3TimeChecker_methods(root_module, cls):
    ## nstime.h (module 'core'): ns3::TimeChecker::TimeChecker() [constructor]
    cls.add_constructor([])
    ## nstime.h (module 'core'): ns3::TimeChecker::TimeChecker(ns3::TimeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TimeChecker const &', 'arg0')])
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

def register_Ns3CcnbParserBlock_methods(root_module, cls):
    ## ccnb-parser-block.h (module 'NDNabstraction'): ns3::CcnbParser::Block::Block() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-block.h (module 'NDNabstraction'): ns3::CcnbParser::Block::Block(ns3::CcnbParser::Block const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Block const &', 'arg0')])
    ## ccnb-parser-block.h (module 'NDNabstraction'): static ns3::Ptr<ns3::CcnbParser::Block> ns3::CcnbParser::Block::ParseBlock(ns3::Buffer::Iterator & start) [member function]
    cls.add_method('ParseBlock', 
                   'ns3::Ptr< ns3::CcnbParser::Block >', 
                   [param('ns3::Buffer::Iterator &', 'start')], 
                   is_static=True)
    ## ccnb-parser-block.h (module 'NDNabstraction'): void ns3::CcnbParser::Block::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-block.h (module 'NDNabstraction'): void ns3::CcnbParser::Block::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-block.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Block::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-block.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Block::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3CcnbParserCcnbDecodingException_methods(root_module, cls):
    ## ccnb-parser-common.h (module 'NDNabstraction'): ns3::CcnbParser::CcnbDecodingException::CcnbDecodingException() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-common.h (module 'NDNabstraction'): ns3::CcnbParser::CcnbDecodingException::CcnbDecodingException(ns3::CcnbParser::CcnbDecodingException const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::CcnbDecodingException const &', 'arg0')])
    return

def register_Ns3CcnbParserExt_methods(root_module, cls):
    ## ccnb-parser-ext.h (module 'NDNabstraction'): ns3::CcnbParser::Ext::Ext(ns3::CcnbParser::Ext const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Ext const &', 'arg0')])
    ## ccnb-parser-ext.h (module 'NDNabstraction'): ns3::CcnbParser::Ext::Ext(ns3::Buffer::Iterator & start, uint32_t extSubtype) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'extSubtype')])
    ## ccnb-parser-ext.h (module 'NDNabstraction'): void ns3::CcnbParser::Ext::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-ext.h (module 'NDNabstraction'): void ns3::CcnbParser::Ext::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-ext.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Ext::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-ext.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Ext::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-ext.h (module 'NDNabstraction'): ns3::CcnbParser::Ext::m_extSubtype [variable]
    cls.add_instance_attribute('m_extSubtype', 'uint64_t', is_const=False)
    return

def register_Ns3CcnbParserNoArguVisitor_methods(root_module, cls):
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NoArguVisitor::NoArguVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NoArguVisitor::NoArguVisitor(ns3::CcnbParser::NoArguVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::NoArguVisitor const &', 'arg0')])
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Blob & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Udata & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Tag & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Tag &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Attr & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Attr &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Dtag & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Dattr & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-no-argu-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguVisitor::visit(ns3::CcnbParser::Ext & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Ext &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3CcnbParserUdata_methods(root_module, cls):
    ## ccnb-parser-udata.h (module 'NDNabstraction'): ns3::CcnbParser::Udata::Udata(ns3::CcnbParser::Udata const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Udata const &', 'arg0')])
    ## ccnb-parser-udata.h (module 'NDNabstraction'): ns3::CcnbParser::Udata::Udata(ns3::Buffer::Iterator & start, uint32_t length) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'length')])
    ## ccnb-parser-udata.h (module 'NDNabstraction'): void ns3::CcnbParser::Udata::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-udata.h (module 'NDNabstraction'): void ns3::CcnbParser::Udata::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-udata.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Udata::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-udata.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Udata::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-udata.h (module 'NDNabstraction'): ns3::CcnbParser::Udata::m_udata [variable]
    cls.add_instance_attribute('m_udata', 'std::string', is_const=False)
    return

def register_Ns3CcnbParserVisitor_methods(root_module, cls):
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::Visitor::Visitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::Visitor::Visitor(ns3::CcnbParser::Visitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Visitor const &', 'arg0')])
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Blob & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Udata & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Tag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Tag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Attr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Attr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Dtag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Dattr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Visitor::visit(ns3::CcnbParser::Ext & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Ext &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3CcnbParserVoidNoArguVisitor_methods(root_module, cls):
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidNoArguVisitor::VoidNoArguVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidNoArguVisitor::VoidNoArguVisitor(ns3::CcnbParser::VoidNoArguVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::VoidNoArguVisitor const &', 'arg0')])
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Blob & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Blob &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Udata & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Udata &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Tag & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Tag &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Attr & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Attr &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Dtag & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Dattr & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-no-argu-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguVisitor::visit(ns3::CcnbParser::Ext & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Ext &', 'arg0')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3CcnbParserVoidVisitor_methods(root_module, cls):
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidVisitor::VoidVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidVisitor::VoidVisitor(ns3::CcnbParser::VoidVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::VoidVisitor const &', 'arg0')])
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Blob & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Blob &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Udata & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Udata &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Tag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Tag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Attr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Attr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Dtag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Dattr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    ## ccnb-parser-void-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidVisitor::visit(ns3::CcnbParser::Ext & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Ext &', 'arg0'), param('boost::any', 'arg1')], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3CcnbParserBaseAttr_methods(root_module, cls):
    ## ccnb-parser-base-attr.h (module 'NDNabstraction'): ns3::CcnbParser::BaseAttr::BaseAttr() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-base-attr.h (module 'NDNabstraction'): ns3::CcnbParser::BaseAttr::BaseAttr(ns3::CcnbParser::BaseAttr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::BaseAttr const &', 'arg0')])
    ## ccnb-parser-base-attr.h (module 'NDNabstraction'): ns3::CcnbParser::BaseAttr::m_value [variable]
    cls.add_instance_attribute('m_value', 'ns3::Ptr< ns3::CcnbParser::Udata >', is_const=False)
    return

def register_Ns3CcnbParserBaseTag_methods(root_module, cls):
    ## ccnb-parser-base-tag.h (module 'NDNabstraction'): ns3::CcnbParser::BaseTag::BaseTag(ns3::CcnbParser::BaseTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::BaseTag const &', 'arg0')])
    ## ccnb-parser-base-tag.h (module 'NDNabstraction'): ns3::CcnbParser::BaseTag::m_attrs [variable]
    cls.add_instance_attribute('m_attrs', 'std::list< ns3::Ptr< ns3::CcnbParser::Block > >', is_const=False)
    ## ccnb-parser-base-tag.h (module 'NDNabstraction'): ns3::CcnbParser::BaseTag::m_nestedTags [variable]
    cls.add_instance_attribute('m_nestedTags', 'std::list< ns3::Ptr< ns3::CcnbParser::Block > >', is_const=False)
    ## ccnb-parser-base-tag.h (module 'NDNabstraction'): ns3::CcnbParser::BaseTag::BaseTag() [constructor]
    cls.add_constructor([], 
                        visibility='protected')
    return

def register_Ns3CcnbParserBlob_methods(root_module, cls):
    ## ccnb-parser-blob.h (module 'NDNabstraction'): ns3::CcnbParser::Blob::Blob(ns3::CcnbParser::Blob const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Blob const &', 'arg0')])
    ## ccnb-parser-blob.h (module 'NDNabstraction'): ns3::CcnbParser::Blob::Blob(ns3::Buffer::Iterator & start, uint32_t length) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'length')])
    ## ccnb-parser-blob.h (module 'NDNabstraction'): void ns3::CcnbParser::Blob::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-blob.h (module 'NDNabstraction'): void ns3::CcnbParser::Blob::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-blob.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Blob::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-blob.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Blob::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-blob.h (module 'NDNabstraction'): ns3::CcnbParser::Blob::m_blob [variable]
    cls.add_instance_attribute('m_blob', 'ns3::Buffer', is_const=False)
    return

def register_Ns3CcnbParserDattr_methods(root_module, cls):
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): ns3::CcnbParser::Dattr::Dattr(ns3::CcnbParser::Dattr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Dattr const &', 'arg0')])
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): ns3::CcnbParser::Dattr::Dattr(ns3::Buffer::Iterator & start, uint32_t dattr) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'dattr')])
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): void ns3::CcnbParser::Dattr::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): void ns3::CcnbParser::Dattr::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Dattr::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Dattr::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-dattr.h (module 'NDNabstraction'): ns3::CcnbParser::Dattr::m_dattr [variable]
    cls.add_instance_attribute('m_dattr', 'uint32_t', is_const=False)
    return

def register_Ns3CcnbParserDepthFirstVisitor_methods(root_module, cls):
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::DepthFirstVisitor::DepthFirstVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::DepthFirstVisitor::DepthFirstVisitor(ns3::CcnbParser::DepthFirstVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::DepthFirstVisitor const &', 'arg0')])
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Blob & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Udata & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Tag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Tag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Attr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Attr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Dtag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Dattr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::DepthFirstVisitor::visit(ns3::CcnbParser::Ext & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Ext &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserDtag_methods(root_module, cls):
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): ns3::CcnbParser::Dtag::Dtag(ns3::CcnbParser::Dtag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Dtag const &', 'arg0')])
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): ns3::CcnbParser::Dtag::Dtag(ns3::Buffer::Iterator & start, uint32_t dtag) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'dtag')])
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): void ns3::CcnbParser::Dtag::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): void ns3::CcnbParser::Dtag::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Dtag::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Dtag::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-dtag.h (module 'NDNabstraction'): ns3::CcnbParser::Dtag::m_dtag [variable]
    cls.add_instance_attribute('m_dtag', 'uint32_t', is_const=False)
    return

def register_Ns3CcnbParserNoArguDepthFirstVisitor_methods(root_module, cls):
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NoArguDepthFirstVisitor::NoArguDepthFirstVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NoArguDepthFirstVisitor::NoArguDepthFirstVisitor(ns3::CcnbParser::NoArguDepthFirstVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::NoArguDepthFirstVisitor const &', 'arg0')])
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Blob & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Udata & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Tag & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Tag &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Attr & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Attr &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Dtag & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Dattr & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-no-argu-depth-first-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NoArguDepthFirstVisitor::visit(ns3::CcnbParser::Ext & arg0) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Ext &', 'arg0')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserNonNegativeIntegerVisitor_methods(root_module, cls):
    ## ccnb-parser-non-negative-integer-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NonNegativeIntegerVisitor::NonNegativeIntegerVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-non-negative-integer-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NonNegativeIntegerVisitor::NonNegativeIntegerVisitor(ns3::CcnbParser::NonNegativeIntegerVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::NonNegativeIntegerVisitor const &', 'arg0')])
    ## ccnb-parser-non-negative-integer-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NonNegativeIntegerVisitor::visit(ns3::CcnbParser::Blob & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'n')], 
                   is_virtual=True)
    ## ccnb-parser-non-negative-integer-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NonNegativeIntegerVisitor::visit(ns3::CcnbParser::Udata & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'n')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserNonceVisitor_methods(root_module, cls):
    ## ccnb-parser-nonce-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NonceVisitor::NonceVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-nonce-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NonceVisitor::NonceVisitor(ns3::CcnbParser::NonceVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::NonceVisitor const &', 'arg0')])
    ## ccnb-parser-nonce-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NonceVisitor::visit(ns3::CcnbParser::Blob & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'n')], 
                   is_virtual=True)
    ## ccnb-parser-nonce-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::NonceVisitor::visit(ns3::CcnbParser::Udata & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'n')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserStringVisitor_methods(root_module, cls):
    ## ccnb-parser-string-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::StringVisitor::StringVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-string-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::StringVisitor::StringVisitor(ns3::CcnbParser::StringVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::StringVisitor const &', 'arg0')])
    ## ccnb-parser-string-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::StringVisitor::visit(ns3::CcnbParser::Blob & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'n')], 
                   is_virtual=True)
    ## ccnb-parser-string-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::StringVisitor::visit(ns3::CcnbParser::Udata & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'n')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserTag_methods(root_module, cls):
    ## ccnb-parser-tag.h (module 'NDNabstraction'): ns3::CcnbParser::Tag::Tag(ns3::CcnbParser::Tag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Tag const &', 'arg0')])
    ## ccnb-parser-tag.h (module 'NDNabstraction'): ns3::CcnbParser::Tag::Tag(ns3::Buffer::Iterator & start, uint32_t length) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'length')])
    ## ccnb-parser-tag.h (module 'NDNabstraction'): void ns3::CcnbParser::Tag::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-tag.h (module 'NDNabstraction'): void ns3::CcnbParser::Tag::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-tag.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Tag::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-tag.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Tag::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-tag.h (module 'NDNabstraction'): ns3::CcnbParser::Tag::m_tag [variable]
    cls.add_instance_attribute('m_tag', 'std::string', is_const=False)
    return

def register_Ns3CcnbParserTimestampVisitor_methods(root_module, cls):
    ## ccnb-parser-timestamp-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::TimestampVisitor::TimestampVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-timestamp-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::TimestampVisitor::TimestampVisitor(ns3::CcnbParser::TimestampVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::TimestampVisitor const &', 'arg0')])
    ## ccnb-parser-timestamp-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::TimestampVisitor::visit(ns3::CcnbParser::Blob & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Blob &', 'n')], 
                   is_virtual=True)
    ## ccnb-parser-timestamp-visitor.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::TimestampVisitor::visit(ns3::CcnbParser::Udata & n) [member function]
    cls.add_method('visit', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Udata &', 'n')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserVoidDepthFirstVisitor_methods(root_module, cls):
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidDepthFirstVisitor::VoidDepthFirstVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidDepthFirstVisitor::VoidDepthFirstVisitor(ns3::CcnbParser::VoidDepthFirstVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::VoidDepthFirstVisitor const &', 'arg0')])
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Blob & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Blob &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Udata & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Udata &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Tag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Tag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Attr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Attr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Dtag & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Dattr & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    ## ccnb-parser-void-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidDepthFirstVisitor::visit(ns3::CcnbParser::Ext & arg0, boost::any arg1) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Ext &', 'arg0'), param('boost::any', 'arg1')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserVoidNoArguDepthFirstVisitor_methods(root_module, cls):
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidNoArguDepthFirstVisitor::VoidNoArguDepthFirstVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::VoidNoArguDepthFirstVisitor::VoidNoArguDepthFirstVisitor(ns3::CcnbParser::VoidNoArguDepthFirstVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::VoidNoArguDepthFirstVisitor const &', 'arg0')])
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Blob & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Blob &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Udata & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Udata &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Tag & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Tag &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Attr & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Attr &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Dtag & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Dattr & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dattr &', 'arg0')], 
                   is_virtual=True)
    ## ccnb-parser-void-no-argu-depth-first-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::VoidNoArguDepthFirstVisitor::visit(ns3::CcnbParser::Ext & arg0) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Ext &', 'arg0')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserAttr_methods(root_module, cls):
    ## ccnb-parser-attr.h (module 'NDNabstraction'): ns3::CcnbParser::Attr::Attr(ns3::CcnbParser::Attr const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::Attr const &', 'arg0')])
    ## ccnb-parser-attr.h (module 'NDNabstraction'): ns3::CcnbParser::Attr::Attr(ns3::Buffer::Iterator & start, uint32_t length) [constructor]
    cls.add_constructor([param('ns3::Buffer::Iterator &', 'start'), param('uint32_t', 'length')])
    ## ccnb-parser-attr.h (module 'NDNabstraction'): void ns3::CcnbParser::Attr::accept(ns3::CcnbParser::VoidNoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidNoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-attr.h (module 'NDNabstraction'): void ns3::CcnbParser::Attr::accept(ns3::CcnbParser::VoidVisitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'void', 
                   [param('ns3::CcnbParser::VoidVisitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-attr.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Attr::accept(ns3::CcnbParser::NoArguVisitor & v) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::NoArguVisitor &', 'v')], 
                   is_virtual=True)
    ## ccnb-parser-attr.h (module 'NDNabstraction'): boost::any ns3::CcnbParser::Attr::accept(ns3::CcnbParser::Visitor & v, boost::any param) [member function]
    cls.add_method('accept', 
                   'boost::any', 
                   [param('ns3::CcnbParser::Visitor &', 'v'), param('boost::any', 'param')], 
                   is_virtual=True)
    ## ccnb-parser-attr.h (module 'NDNabstraction'): ns3::CcnbParser::Attr::m_attr [variable]
    cls.add_instance_attribute('m_attr', 'std::string', is_const=False)
    return

def register_Ns3CcnbParserContentObjectVisitor_methods(root_module, cls):
    ## ccnb-parser-content-object-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::ContentObjectVisitor::ContentObjectVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-content-object-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::ContentObjectVisitor::ContentObjectVisitor(ns3::CcnbParser::ContentObjectVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::ContentObjectVisitor const &', 'arg0')])
    ## ccnb-parser-content-object-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::ContentObjectVisitor::visit(ns3::CcnbParser::Dtag & n, boost::any param) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'n'), param('boost::any', 'param')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserInterestVisitor_methods(root_module, cls):
    ## ccnb-parser-interest-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::InterestVisitor::InterestVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-interest-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::InterestVisitor::InterestVisitor(ns3::CcnbParser::InterestVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::InterestVisitor const &', 'arg0')])
    ## ccnb-parser-interest-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::InterestVisitor::visit(ns3::CcnbParser::Dtag & n, boost::any param) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'n'), param('boost::any', 'param')], 
                   is_virtual=True)
    return

def register_Ns3CcnbParserNameComponentsVisitor_methods(root_module, cls):
    ## ccnb-parser-name-components-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NameComponentsVisitor::NameComponentsVisitor() [constructor]
    cls.add_constructor([])
    ## ccnb-parser-name-components-visitor.h (module 'NDNabstraction'): ns3::CcnbParser::NameComponentsVisitor::NameComponentsVisitor(ns3::CcnbParser::NameComponentsVisitor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CcnbParser::NameComponentsVisitor const &', 'arg0')])
    ## ccnb-parser-name-components-visitor.h (module 'NDNabstraction'): void ns3::CcnbParser::NameComponentsVisitor::visit(ns3::CcnbParser::Dtag & n, boost::any param) [member function]
    cls.add_method('visit', 
                   'void', 
                   [param('ns3::CcnbParser::Dtag &', 'n'), param('boost::any', 'param')], 
                   is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## ccnx-name-components.h (module 'NDNabstraction'): extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeCcnxNameComponentsChecker() [free function]
    module.add_function('MakeCcnxNameComponentsChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    register_functions_ns3_CcnbParser(module.get_submodule('CcnbParser'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    return

def register_functions_ns3_CcnbParser(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
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

