Getting Started
===============

Portability
------------

.. image:: https://travis-ci.org/named-data-ndnSIM/ndnSIM.svg?branch=test-travis-ci
    :target: https://travis-ci.org/named-data-ndnSIM/ndnSIM

ndnSIM 2.x has been successfully compiled and used on following platforms:

- Ubuntu Linux 16.04 (32- and 64-bit platform)
- Ubuntu Linux 17.10 (64-bit platform)
- macOS 10.11
- macOS 10.12
- macOS 10.13

.. _requirements:

Prerequisites
-------------

**1. Core dependencies**

-  ``python`` >= 2.6
-  ``libsqlite3``
-  ``libcrypto++``
-  ``pkg-config``
-  ``openssl``
-  Boost libraries >= 1.54

.. role:: red

.. note::
   If you do not have root permissions to install boost, you can install it in your home
   folder.  However, you need to be make sure that `libboost_iostreams` library is successfully
   compiled and is installed.  Please refer to :ref:`the following example <Installing boost
   libraries>` for the hints how to successfully compile and install boost libraries on Ubuntu
   Linux.

Following are the detailed steps for each platform to install the compiler, all necessary
development tools and libraries, and ndn-cxx prerequisites.

- macOS

  * macOS with MacPorts:

   .. code-block:: bash

       sudo port install pkgconfig boost sqlite3 libcryptopp openssl

  * macOS with HomeBrew:

   .. code-block:: bash

       brew install boost cryptopp pkg-config openssl libxml2
       brew link --force libxml2

- Linux

  * Ubuntu Linux

   .. code-block:: bash

       sudo apt-get install build-essential libsqlite3-dev libcrypto++-dev libboost-all-dev libssl-dev git python-setuptools

  * Fedora Linux

   .. code-block:: bash

       sudo yum install gcc-g++ git sqlite-devel cryptopp-devel boost-devel openssl-devel

   .. note::
      :red:`ndnSIM requires boost version at least 1.54.` Many linux distribution
      (Fedora 16, 17 at the time of this writing) ship an old version of boost, making it
      impossible to compile ndnSIM out-of-the-box.  Please install the latest version, following
      :ref:`these simple instructions <Installing boost libraries>`.

**2. Dependencies for NS-3 Python bindings**

If you are planning to use NS-3 python bindings, a number of additional dependencies
should be installed.  For example, in order to run `visualizer`_ module, the following
should be installed:

- macOS

  * macOS with MacPorts:

    .. code-block:: bash

        sudo port install  py27-pygraphviz py27-goocanvas py27-kiwi

  * macOS with HomeBrew

    Currently, there are many missing dependencies, so it is impossible to use visualizer module with HomeBrew.  Use MacPorts instead.

- Linux

  * Ubuntu Linux

    .. code-block:: bash

        sudo apt-get install python-dev python-pygraphviz python-kiwi python-pygoocanvas python-gnome2 python-rsvg ipython

  * Fedora Linux

    .. code-block:: bash

        sudo yum install pygoocanvas python-kiwi graphviz-python

        # easy_install method, since pygraphviz is not (yet?) packaged into Fedora (https://bugzilla.redhat.com/show_bug.cgi?id=740687)
        sudo yum install graphviz-devel
        sudo yum install python-pip
        sudo easy_install pygraphviz

.. _visualizer: http://www.nsnam.org/wiki/index.php/PyViz

Downloading ndnSIM source
-------------------------

ndnSIM package consists of three pieces:

- a custom branch of NS-3 that contains a few useful patches
- a customized python binding generation library (necessary if you want to use NS-3's python
  bindings and/or visualizer module)
- the source code of ndnSIM module
- modified source code of ndn-cxx library and NDN Forwarding Daemon (NFD), attached to
  ndnSIM git repository as git submodules

The following commands download all pieces from GitHub repositories:

.. code-block:: bash

    mkdir ndnSIM
    cd ndnSIM
    git clone https://github.com/named-data-ndnSIM/ns-3-dev.git ns-3
    git clone https://github.com/named-data-ndnSIM/pybindgen.git pybindgen
    git clone --recursive https://github.com/named-data-ndnSIM/ndnSIM.git ns-3/src/ndnSIM

The last command downloads ndnSIM source code and source code of all submodules (i.e.,
ndn-cxx and NFD).  If you previously cloned without ``--recursive`` flag, the correct
versions of submodules can be retrieved using:

.. code-block:: bash

    git submodule update --init

The same command should be run to update submodules when there are new changes available.

.. note::
    A few modification to the base NS-3 and pybindgen are necessary to run successfully
    compile and run ndnSIM.  Some of the changes are specific to ndnSIM and some are
    bugfixes that we are submitting to NS-3 upstream.  We also periodically update
    repository with the new NS-3 releases, usually in form of rebasing (and if necessary
    updating or eliminating) our custom patches on top of the released commits.


Compiling and running ndnSIM
----------------------------

ndnSIM uses a standard NS-3 compilation procedure.  Normally the following commands should be
sufficient to configure and build ndnSIM with python bindings enabled:

.. code-block:: bash

   cd <ns-3-folder>
   ./waf configure --enable-examples
   ./waf

On macOS (with MacPorts), you may need to modify the configure command to use MacPorts
version of python:

.. code-block:: bash

   cd <ns-3-folder>
   ./waf configure --with-python=/opt/local/bin/python2.7 --enable-examples
   # or run ``sudo port select python python27``
   ./waf

.. note::
   On macOS configuration stage may get :ref:`stuck at detecting gtk module <Problems with
   the gtk python module on macOS>`.  Make sure you have `XQuartz
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

- Example simulation scenarios

  When NS-3 is configured with ``--with-examples`` flag, you can directly run all examples
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

  Simulation scenarios can be written directly inside NS-3 in ``scratch/`` or ``src/ndnSIM/examples`` folder.

  Alternative and a recommended way is to write simulation scenarios in a separate
  repository, not related to either NS-3 or ndnSIM.  For example, you can use the
  following template to write your extensions, simulation scenarios, and metric processing
  scripts: `<http://github.com/cawka/ndnSIM-scenario-template>`_:

  .. code-block:: bash

      mkdir ndnSIM
      cd ndnSIM
      git clone https://github.com/named-data-ndnSIM/ns-3-dev.git ns-3
      git clone https://github.com/named-data-ndnSIM/pybindgen.git pybindgen
      git clone --recursive https://github.com/named-data-ndnSIM/ndnSIM.git ns-3/src/ndnSIM

      # Build and install NS-3 and ndnSIM
      cd ns-3
      ./waf configure -d optimized
      ./waf

      sudo ./waf install
      cd ..

      git clone https://github.com/named-data-ndnSIM/scenario-template.git scenario
      cd scenario
      export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
      export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

      ./waf configure

      ./waf --run <scenario>

  For more detailed information, refer to `README file
  <https://github.com/cawka/ndnSIM-scenario-template/blob/master/README.md>`_.

..
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
