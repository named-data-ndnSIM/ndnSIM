## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

from waflib import Utils, Logs

def build(bld):
    # This may be necessary for visualizer to work
    all_modules = [mod[len("ns3-"):] for mod in bld.env['NS3_ENABLED_MODULES']]

    examples = bld.path.ant_glob(['*.cpp'], excl=['*-mpi.cpp'])
    if 'NS3_MPI' in bld.env['DEFINES_MPI']:
        examples += bld.path.ant_glob(['*-mpi.cpp'])

    for i in examples:
        name = str(i)[:-len(".cpp")]
        obj = bld.create_ns3_program(name, all_modules)
        obj.source = [i] + bld.path.ant_glob(['%s/**/*.cpp' % name])

