Getting Started
===============

Portability
------------

ndnSIM has been successfully compiled and used under Ubuntu Linux 12.04 (stock gcc, boost 1.48), Mac OS 10.8 (gcc-4.2 apple/llvm, macports gcc 4.7, boost 1.49-1.52).

.. _requirements:

Requirements
-------------

1. ndnSIM requires the customized version of NS-3 simulator (a number of patches required to make ndnSIM work with the latest development branch of NS-3).

2. Boost libraries should be installed on the system:

   * For Ubuntu::

       sudo aptitude install libboost-all-dev

   * For MacOS (macports)::

       sudo port instal boost

.. role:: red

.. note::
   :red:`!!! ndnSIM requires boost version at least 1.48.`   Many linux distribution (including Ubuntu 12.04.1 at the time of this writing) ship an old version of boost, making it impossible to compile ndnSIM out-of-the-box.  Please install the latest version, following :ref:`these simple instructions <Installing boost libraries>`.

.. note::
   !!! If you do not have root permissions to install boost, you can install it in your home folder.  However, you need to be make sure that `libboost_iostreams` library is successfully compiled and is installed.  Please refer to :ref:`the following example <Installing boost libraries>` for the hints how to successfully compile and install boost libraries on Ubuntu Linux. 


3. If you are planning to use other modules, like visualizer, a number of additional dependencies should be installed.  For example, in
order to run `visualizer`_ module, the following should be installed:

   * For Ubuntu::

       sudo apt-get install python-dev python-pygraphviz python-kiwi
       sudo apt-get install python-pygoocanvas python-gnome2
       sudo apt-get install python-gnomedesktop python-rsvg ipython

   * For MacOS (macports)::

       sudo port install  py27-pygraphviz py27-goocanvas

.. py27-kiwi 

.. _visualizer: http://www.nsnam.org/wiki/index.php/PyViz

Downloading ndnSIM source
-------------------------

Download a custom branch of NS-3 that contains all necessary patches and more::

	mkdir ndnSIM
	cd ndnSIM
	git clone git://github.com/cawka/ns-3-dev-ndnSIM.git ns-3
	git clone git://github.com/cawka/pybindgen.git pybindgen

The first command is to create a directory, which will contain everything NS-3 related.  The bare minimum is just base NS-3 (the first clone above). The second clone gets a module necessary to build python bindings, which are necessary for the visualizer module.  

Finally, clone actual ndnSIM code and place it in src/ folder::

	git clone git://github.com/NDN-Routing/ndnSIM.git ns-3/src/ndnSIM

There are quite a few modification to the base NS-3 code that are necessary to run ndnSIM, and the code is periodically synchronized with the official developer branch.  Eventually, all the changes will be merged to the official branch, but for the time being, it is necessary to use the customized branch.

Compiling and running ndnSIM
----------------------------

ndnSIM uses standard NS-3 compilation procedure.  Normally the following commands should be sufficient to configure and build ndnSIM with python bindings enabled::

	cd <ns-3-folder>
	./waf configure --enable-examples
	./waf

On MacOS (with macports), you may need to modify the configure command to use macports version of python::

	cd <ns-3-folder>
	./waf configure --with-python=/opt/local/bin/python2.7 --enable-examples
	./waf

Python bindings is an optional and not very stable feature of NS-3 simulator.  It is possible to disable python bindings compilation either to speed up compilation or to avoid certain compilation errors (e.g., "Could not find a task generator for the name 'ns3-visualizer'")::

	cd <ns-3-folder>
	./waf configure --disable-python --enable-examples
	./waf

For more configuration options, please refer to ``./waf --help``.

To run :doc:`sample ndnSIM simulations <examples>`::

	./waf --run=ndn-simple

or::

	./waf --run=ndn-grid

If you have compiled with python bindings, then you can try to run these simulations with visualizer::

	./waf --run=ndn-simple --vis

or::

	./waf --run=ndn-grid --vis

.. note::
   Do not forget to configure and compile NS-3 in optimized mode (``./waf configure -d optimized``) in order to run actual simulations.


