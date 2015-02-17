Getting Started
===============

Portability
------------

ndnSIM 2.0 has been successfully compiled and used on following platforms:

- Ubuntu Linux 12.04 (see the note)
- Ubuntu Linux 14.04
- OS X 10.10

.. note::
    ndnSIM is currently cannot be compiled on Ubuntu Linux 12.04 with the packaged boost
    libraries (there is an `issue with boost 1.48 and gcc 4.6
    <https://svn.boost.org/trac/boost/ticket/6153>`_).  It is still possible to compile ndnSIM
    on this platform, but either compiler or boost libraries (or both) need to get upgraded.

    More recent version of boost can be installed from "Boost C++ Libraries" team PPA::

        sudo apt-get install python-software-properties
        sudo add-apt-repository ppa:boost-latest/ppa
        sudo apt-get update
        sudo apt-get install libboost1.55-all-dev

        # add  --boost-libs=/usr/lib/x86_64-linux-gnu  to ./waf configure for ndn-cxx and ns3
        # ./waf configure --boost-libs=/usr/lib/x86_64-linux-gnu

    Make sure that all other version of boost libraries (``-dev`` packages) are removed,
    otherwise compilation will fail.

.. _requirements:

Prerequisites
-------------

1. `ndnSIM also required ndn-cxx library and all of its prerequisites
   <http://named-data.net/doc/ndn-cxx/current/INSTALL.html>`_.

.. role:: red

.. note::
   :red:`ndnSIM requires boost version at least 1.49.` Many linux distribution
   (Fedora 16, 17 at the time of this writing) ship an old version of boost, making it
   impossible to compile ndnSIM out-of-the-box.  Please install the latest version, following
   :ref:`these simple instructions <Installing boost libraries>`.

.. note::
   If you do not have root permissions to install boost, you can install it in your home
   folder.  However, you need to be make sure that `libboost_iostreams` library is successfully
   compiled and is installed.  Please refer to :ref:`the following example <Installing boost
   libraries>` for the hints how to successfully compile and install boost libraries on Ubuntu
   Linux.


2. If you are planning to use other modules, like visualizer, a number of additional
dependencies should be installed.  For example, in order to run `visualizer`_ module, the
following should be installed:

   * For Ubuntu:

       .. code-block:: bash

           sudo apt-get install python-dev python-pygraphviz python-kiwi
           sudo apt-get install python-pygoocanvas python-gnome2
           sudo apt-get install python-rsvg ipython

   * For Fedora:

       .. code-block:: bash

           sudo yum install pygoocanvas python-kiwi graphviz-python

           # easy_install method, since pygraphviz is not (yet?) packaged into Fedora (https://bugzilla.redhat.com/show_bug.cgi?id=740687)
           sudo yum install graphviz-devel
           sudo yum install python-pip
           sudo easy_install pygraphviz

   * For OS X with MacPorts:

       .. code-block:: bash

           sudo port install  py27-pygraphviz py27-goocanvas

           # If you add NDN macports repository, as described in
           # http://named-data.net/doc/NFD/current/INSTALL.html#install-nfd-using-the-ndn-macports-repository-on-os-x
           # you will be able to install another useful python module
           # sudo port install py27-kiwi

   * For OS X with HomeBrew

       .. code-block:: bash

           brew install boost cryptopp pkg-config libxml2
           brew link --force libxml2

.. _visualizer: http://www.nsnam.org/wiki/index.php/PyViz

Downloading ndnSIM source
-------------------------

ndnSIM package consists of three pieces:

- `ndn-cxx library <http://named-data.net/doc/ndn-cxx/>`_
- a custom branch of NS-3 that contains a few useful patches
- a customized python binding generation library (necessary if you want to use NS-3's python
  bindings and/or visualizer module)
- the source code of ndnSIM module

The following commands download all pieces from GitHub repositories:

.. code-block:: bash

    mkdir ndnSIM
    cd ndnSIM
    git clone https://github.com/named-data/ndn-cxx.git ndn-cxx
    git clone https://github.com/cawka/ns-3-dev-ndnSIM.git ns-3
    git clone https://github.com/cawka/pybindgen.git pybindgen
    git clone https://github.com/named-data/ndnSIM.git ns-3/src/ndnSIM

The few modification to the base NS-3 code are necessary to run ndnSIM, and the code is
periodically synchronized with the official developer branch.  Eventually, all the changes will
be merged to the official branch, but for the time being, it is necessary to use the customized
branch.


Compiling and running ndnSIM
----------------------------

- Compile and install ndn-cxx library

    .. code-block:: bash

        cd ndnSIM/ndn-cxx
        ./waf configure
        ./waf
        sudo ./waf install

    .. note::
       On Ubuntu platform you can also install ndn-cxx library from `NDN
       PPA repository <http://named-data.net/doc/NFD/current/INSTALL.html#installing-nfd-from-binaries>`_

       .. code-block:: bash

           sudo apt-get install ndn-cxx-dev

       If you are using ndn-cxx from PPA, please make sure that you do not have the existing installation
       of ndn-cxx library compiled from source (e.g., in /usr/local).  Otherwise, the version mistmach
       may result in compilation errors.

- Compile NS-3 with ndnSIM module

    ndnSIM uses standard NS-3 compilation procedure.  Normally the following commands should be
    sufficient to configure and build ndnSIM with python bindings enabled:

    .. code-block:: bash

        cd <ns-3-folder>
        ./waf configure --enable-examples
        ./waf

    On MacOS (with macports), you may need to modify the configure command to use macports
    version of python:

    .. code-block:: bash

        cd <ns-3-folder>
        ./waf configure --with-python=/opt/local/bin/python2.7 --enable-examples
        # or run ``sudo port select python python27``
        ./waf

    .. note::
        On OS X configuration stage may get :ref:`stuck at detecting gtk module <Problems with
        the gtk python module on OS X>`.  Make sure you have `XQuartz
        <http://xquartz.macosforge.org>`_ installed or disable python as described in the
        following instructions.

    Python bindings is an optional and not very stable feature of NS-3 simulator.  It is
    possible to disable python bindings compilation either to speed up compilation or to avoid
    certain compilation errors (e.g., "Could not find a task generator for the name
    'ns3-visualizer'"):

    .. code-block:: bash

        cd <ns-3-folder>
        ./waf configure --disable-python --enable-examples
        ./waf

    For more configuration options, please refer to ``./waf --help``.


Simulating using ndnSIM
-----------------------

- Examples simulations

    When NS-3 is compiled with ``--with-examples`` flag, you can directly run all examples
    described in :doc:`examples section of this tutorial <examples>`.  For example, to run
    ``ndn-simple.cpp`` scenario, you can run the following command:

    .. code-block:: bash

        ./waf --run=ndn-simple

    To run ``ndn-grid.cpp`` scenario:

    .. code-block:: bash

        ./waf --run=ndn-grid

    To run the sample simulation scenarios with the logging module of NS-3 enabled (note that
    this will work only when NS-3 is compiled in debug mode):

    .. code-block:: bash

        NS_LOG=ndn.Producer:ndn.Consumer ./waf --run=<scenario name>

    If you have compiled with python bindings, then you can try to run these simulations with
    visualizer:

    .. code-block:: bash

        ./waf --run=ndn-simple --vis

    or:

    .. code-block:: bash

        ./waf --run=ndn-grid --vis

    .. note::
       Do not forget to configure and compile NS-3 in optimized mode (``./waf configure -d
       optimized``) in order to run actual simulations.

- Real experimentation

    While it is possible to write simulations directly inside NS-3 (in ``scratch/`` folder) or
    ndnSIM (in ``examples/``), the recommended way is to write your simulation scenarios, as
    well as any custom extensions, separately from the NS-3 or ndnSIM core.

    For example, you can use the following template to write your extensions, simulation
    scenarios, and metric processing scripts:
    `<http://github.com/cawka/ndnSIM-scenario-template>`_:

    .. code-block:: bash

        mkdir ndnSIM
        cd ndnSIM
        git clone git://github.com/cawka/ns-3-dev-ndnSIM.git ns-3
        git clone git://github.com/cawka/pybindgen.git pybindgen
        git clone git://github.com/NDN-Routing/ndnSIM.git ns-3/src/ndnSIM

        # Build and install NS-3 and ndnSIM
        cd ns-3
        ./waf configure -d optimized
        ./waf

        sudo ./waf install
        cd ..

        git clone git://github.com/cawka/ndnSIM-scenario-template.git scenario
        cd scenario
        export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
        export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

        ./waf configure

        ./waf --run <scenario>

    For more detailed information, refer to `README file
    <https://github.com/cawka/ndnSIM-scenario-template/blob/master/README.md>`_.

Examples of template-based simulations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. ndnSIM examples from `<http://ndnsim.net>`_ website and more:

- `<http://github.com/cawka/ndnSIM-examples>`_, or

2. Script scenarios and graph processing scripts for simulations used in "A Case for Stateful
   Forwarding Plane" paper by Yi et al. (`<http://dx.doi.org/10.1016/j.comcom.2013.01.005>`_):

- `<http://github.com/cawka/ndnSIM-comcom-stateful-fw>`_, or

3. Script scenarios and graph processing scripts for simulations used in "Rapid Traffic
   Information Dissemination Using Named Data" paper by Wang et
   al. (`<http://dx.doi.org/10.1145/2248361.2248365>`_):

- `<http://github.com/cawka/ndnSIM-nom-rapid-car2car>`_, or

- Rocketfuel-based topology generator for ndnSIM preferred format (randomly assigned link
  delays and bandwidth, based on estimated types of connections between nodes):

- `<http://github.com/cawka/ndnSIM-sample-topologies>`_, or
