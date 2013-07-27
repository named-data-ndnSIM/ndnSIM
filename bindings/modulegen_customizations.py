import re
import os
import sys

from pybindgen.typehandlers import base as typehandlers
from pybindgen import ReturnValue, Parameter
from pybindgen.cppmethod import CustomCppMethodWrapper, CustomCppConstructorWrapper
from pybindgen.typehandlers.codesink import MemoryCodeSink
from pybindgen.typehandlers import ctypeparser
from pybindgen import cppclass, param, retval
import warnings

from pybindgen.typehandlers.base import CodeGenerationError

def add_vector_defs (module):
    # mod = module.get_submodule('ndn')
    # nested_module = mod.add_cpp_namespace('error')
    # nested_module.add_class ('Name')


    # # module.add_class ('Name', 
    # # module.add_exception ('Error', foreign_cpp_namespace='ns3::ndn::error', message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # # module.add_exception ('Uri', foreign_cpp_namespace='ns3::ndn::error', message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # # module.add_exception ('StringTransform', foreign_cpp_namespace='ns3::ndn::error', message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # nested_module.add_exception ('Name', message_rvalue='fuck you')
    # # message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # # module.add_exception ('Component', foreign_cpp_namespace='ns3::ndn::error::name', message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # # module.add_exception ('Exclude', foreign_cpp_namespace='ns3::ndn::error', message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # # module.add_exception ('KeyLocator', foreign_cpp_namespace='ns3::ndn::error', message_rvalue='ns3::ndn::error::get_msg (%(EXC)s)')
    # # print dir(module)
    # # module.add_container ('ns3::ndn::Blob', 'char', container_type='vector')
    # # module.add_stl_container('ns3::ndn::Blob', 'char', 'vector', custom_name="Blob2")
    pass


def post_register_types (root_module):
    add_vector_defs (root_module)
