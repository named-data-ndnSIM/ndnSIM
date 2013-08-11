Getting Started
===============

Portability
------------

ndnSIM has been successfully compiled and used under Ubuntu Linux 12.04 (boost libraries **1.48**, with default version 1.46 compilation will probably fail), 12.10 (default version of boost 1.49), 13.04 (default version of boost 1.49), Fedora 18, Mac OS 10.7 and 10.8 (gcc-4.2 apple/llvm, macports gcc 4.7, boost 1.49-1.54).

.. _requirements:

Requirements
-------------

1. ndnSIM requires the customized version of NS-3 simulator (a number of patches required to make ndnSIM work with the latest development branch of NS-3).

2. Boost libraries should be installed on the system:

    * For Ubuntu

        * 12.04

            .. code-block:: bash

                sudo aptitude install libboost1.48-all-dev

        * 12.10, 13.04, and newer versions

            .. code-block:: bash

                sudo aptitude install libboost-all-dev

    * For Fedora (for Fedora 18 and later only):

        .. code-block:: bash

            sudo yum install boost-devel

    * For MacOS (macports):

        .. code-block:: bash

            sudo port instal boost

.. role:: red

.. note::
   :red:`!!! ndnSIM requires boost version at least 1.48.`   Many linux distribution (Fedora 16, 17 at the time of this writing) ship an old version of boost, making it impossible to compile ndnSIM out-of-the-box.  Please install the latest version, following :ref:`these simple instructions <Installing boost libraries>`.

.. note::
   :red:`For Ubuntu 12.04`  Ubuntu 12.04 ships with two versions of boost libraries and it is known that if both are installed, then compilation of ndnSIM will most likely fail.  Please install ``libboost1.48-dev-all`` package and uninstall ``libboost-dev-all``.  If you want to install the latest version of boost libraries, then uninstall both ``libboost1.48-dev-all`` and ``libboost-dev-all``, so the libraries do not interfere with each other.

.. note::
   !!! If you do not have root permissions to install boost, you can install it in your home folder.  However, you need to be make sure that `libboost_iostreams` library is successfully compiled and is installed.  Please refer to :ref:`the following example <Installing boost libraries>` for the hints how to successfully compile and install boost libraries on Ubuntu Linux.


3. If you are planning to use other modules, like visualizer, a number of additional dependencies should be installed.  For example, in
order to run `visualizer`_ module, the following should be installed:

   * For Ubuntu (tested on Ubuntu 12.04, 12.10, 13.04, should work on later versions as well):

       .. code-block:: bash

           sudo apt-get install python-dev python-pygraphviz python-kiwi
           sudo apt-get install python-pygoocanvas python-gnome2
           sudo apt-get install python-gnomedesktop python-rsvg ipython

   * For Fedora (tested on Fedora 16):

       .. code-block:: bash

           sudo yum install pygoocanvas python-kiwi graphviz-python

           # easy_install method, since pygraphviz is not (yet?) packaged into Fedora (https://bugzilla.redhat.com/show_bug.cgi?id=740687)
           sudo yum install graphviz-devel
           sudo yum install python-pip
           sudo easy_install pygraphviz

   * For MacOS (macports):

       .. code-block:: bash

           sudo port install  py27-pygraphviz py27-goocanvas

.. py27-kiwi

.. _visualizer: http://www.nsnam.org/wiki/index.php/PyViz

Downloading ndnSIM source
-------------------------

Download a custom branch of NS-3 that contains all necessary patches, python binding generation library (optional), and clone actual ndnSIM code and place it in src/ folder:

.. code-block:: bash

	mkdir ndnSIM
	cd ndnSIM
	git clone git://github.com/cawka/ns-3-dev-ndnSIM.git ns-3
	git clone git://github.com/cawka/pybindgen.git pybindgen
	git clone git://github.com/NDN-Routing/ndnSIM.git ns-3/src/ndnSIM

There are quite a few modification to the base NS-3 code that are necessary to run ndnSIM, and the code is periodically synchronized with the official developer branch.  Eventually, all the changes will be merged to the official branch, but for the time being, it is necessary to use the customized branch.

If you have problems connecting to github, you can try to clone from google servers:

.. code-block:: bash

        mkdir ndnSIM
        cd ndnSIM
        git clone https://code.google.com/p/ndnsim.ns3-base/ ns-3
        git clone https://code.google.com/p/ndnsim.pybindgen/ pybindgen
        git clone https://code.google.com/p/ndnsim/ ns-3/src/ndnSIM

Compiling and running ndnSIM
----------------------------

ndnSIM uses standard NS-3 compilation procedure.  Normally the following commands should be sufficient to configure and build ndnSIM with python bindings enabled:

.. code-block:: bash

	cd <ns-3-folder>
	./waf configure --enable-examples
	./waf

On MacOS (with macports), you may need to modify the configure command to use macports version of python:

.. code-block:: bash

	cd <ns-3-folder>
	./waf configure --with-python=/opt/local/bin/python2.7 --enable-examples
	./waf

Python bindings is an optional and not very stable feature of NS-3 simulator.  It is possible to disable python bindings compilation either to speed up compilation or to avoid certain compilation errors (e.g., "Could not find a task generator for the name 'ns3-visualizer'"):

.. code-block:: bash

	cd <ns-3-folder>
	./waf configure --disable-python --enable-examples
	./waf

For more configuration options, please refer to ``./waf --help``.

To run :doc:`sample ndnSIM simulations <examples>`:

.. code-block:: bash

	./waf --run=ndn-simple

or:

.. code-block:: bash

	./waf --run=ndn-grid

If you have compiled with python bindings, then you can try to run these simulations with visualizer:

.. code-block:: bash

	./waf --run=ndn-simple --vis

or:

.. code-block:: bash

	./waf --run=ndn-grid --vis

.. note::
   Do not forget to configure and compile NS-3 in optimized mode (``./waf configure -d optimized``) in order to run actual simulations.


Simulating using ndnSIM
-----------------------

While it is possible to write simulations directly inside NS-3 (in ``scratch/`` folder) or ndnSIM (in ``examples/``), the recommended way is to write your simulation scenarios, as well as any custom extensions, separately from the NS-3 or ndnSIM core.

For example, you can use the following template to write your extensions, simulation scenarios, and metric processing scripts: `<http://github.com/cawka/ndnSIM-scenario-template>`_:

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

For more detailed information, refer to `README file <https://github.com/cawka/ndnSIM-scenario-template/blob/master/README.md>`_.

Examples of template-based simulations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. ndnSIM examples from `<http://ndnsim.net>`_ website and more:

- `<http://github.com/cawka/ndnSIM-examples>`_, or
- `<http://code.google.com/p/ndnsim.ndnsim-examples/>`_

2. Script scenarios and graph processing scripts for simulations used in "A Case for Stateful Forwarding Plane" paper by Yi et al. (`<http://dx.doi.org/10.1016/j.comcom.2013.01.005>`_):

- `<http://github.com/cawka/ndnSIM-comcom-stateful-fw>`_, or
- `<http://code.google.com/p/ndnsim.ndnsim-comcom/>`_

3. Script scenarios and graph processing scripts for simulations used in "Rapid Traffic Information Dissemination Using Named Data" paper by Wang et al. (`<http://dx.doi.org/10.1145/2248361.2248365>`_):

- `<http://github.com/cawka/ndnSIM-nom-rapid-car2car>`_, or
- `<http://code.google.com/p/ndnsim.ndnsim-nom-rapid/>`_

- Rocketfuel-based topology generator for ndnSIM preferred format (randomly assigned link delays and bandwidth, based on estimated types of connections between nodes):

- `<http://github.com/cawka/ndnSIM-sample-topologies>`_, or
- `<http://code.google.com/p/ndnsim.ndnsim-sample-topo/>`_
