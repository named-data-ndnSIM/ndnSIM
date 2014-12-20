from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

import pybindgen.settings
import warnings


import sys

def module_init():
    root_module = Module('ns.ndnSIM', cpp_namespace='::ns3')
    return root_module

def register_types(module):
    return

def register_methods(root_module):
    return

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

