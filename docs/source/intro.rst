.. ndnSIM: NS-3 based NDN simulator
.. ============================================================

.. .. toctree::
..    :maxdepth: 2

Introduction
==============

The ndnSIM is NS-3 module that implements Named Data Networking (NDN) communication model, the clean slate Internet design. ndnSIM is specially optimized for simulation purposes and has a cleaner and more extensible internal structure comparing to the existing NDN implementation (Project CCNx).

Following the NDN architecture, ndnSIM is implemented as a new network-layer protocol model, which can run on top of any available link-layer protocol model (point-to-point, CSMA, wireless, etc.).

.. note::
    It will also be possible to run ndnSIM on top of network-layer (IPv4, IPv6) and transport-layer (TCP, UDP) protocols.
    However, it is not yet implemented and patches are welcome.

.. This flexibility allows ndnSIM to simulate scenarios of various homogeneous and heterogeneous networks (e.g., NDN-only, NDN-over-IP, etc.).

The simulator is implemented in a modular fashion, using separate C++ classes to model behavior of each network-layer entity in NDN: :ndnsim:`pending Interest table (PIT) <CcnxPit>`, :ndnsim:`forwarding information base (FIB) <CcnxFib>`, :ndnsim:`content store <CcnxContentStore>`, :ndnsim:`network <CcnxNetDeviceFace>` and :ndnsim:`application <CcnxAppFace>` interfaces, :ndnsim:`Interest forwarding strategies <CcnxForwardingStrategy>`, etc.
This modular structure allows any component to be easily modified or replaced with no or minimal impact on other components.
In addition, the simulator provides an extensive collection of interfaces and helpers to perform detailed tracing behavior of every component, as well as NDN traffic flow.


.. aafig::
    :aspect: 60
    :scale: 120

             +----------------+			      +-----------+
             | "Applications" |			      | NetDevice |
             +----------------+			      +-----------+
		     ^ 					    ^
    .................|......................................|......................
    .		     v			     	            v			  .
    .		+------------------+	     +----------------------+		  .
    .           |    "CcnxFace"    |	     |      "CcnxFace"      |		  .
    .           |  "(CcnxAppFace)" |	     | "(CcnxNetDeviceFace)"|		  .
    .		+------------------+         +----------------------+		  .
    .		               ^                   ^				  .
    .			       |                   |				  .
    .			       v                   v				  .
    .			    XXXXXXXXXXXXXXXXXXXXXXXXXXXXX			  .
    .			    XX                         XX			  .
    .			    XX    Core NDN protocol    XX  			  .
    .                       XX    "(CcnxL3Protocol)"   XX
    .			    XX                         XX			  .
    .			    XXXXXXXXXXXXXXXXXXXXXXXXXXXXX			  .
    .			      ^       ^       ^       ^				  .
    .			      |	      |	      |	      |				  .
    .     +-------------------+   +---+       +---+   +------------+		  .
    .	  | 		    	  |		  |    		   |		  .
    .	  v			  v		  v		   v		  .
    . +-------------------+      +-------+      +-------+        +-------------+  .
    . | "CcnxContentStore"|      |  PIT  |      |  FIB  |        | "Pluggable" |  .
    . +-------------------+      +-------+      +-------+        | "Forwarding"|  .
    .							         | "Strategy"  |  .
    .							         +-------------+  .
    .										  .
    ...............................................................................


The wire format of Interest and Data packets follows the format of the existing `CCNx Project's NDN implementation`_ (CCNx Binary Encoding), allowing reuse of the existing traffic analysis tools, as well as driving simulations using real NDN traffic traces.

.. _CCNx Project's NDN implementation: http://www.ccnx.org/

Getting Started
===============

Portability
------------

ndnSIM has been successfully compiled and used under Ubuntu Linux 11.04 (stock gcc), Mac OS 10.6/10.7 (gcc-4.2 apple/llvm, macports gcc 4.6), FreeBSD 8.2 (requires gcc from ports - the stock gcc will not work!).

Requirements
-------------

1. ndnSIM requires the latest version of NS-3 simulator (as of May 31st, 2012, development branch of NS-3).

2. ndnSIM requires boost libraries:

   * For Ubuntu::

       sudo aptitude install libboost-all-dev

   * For MacOS (macports)::

       sudo port instal boost

3. Other NS-3 modules have additional dependencies.  For example, in
order to run `visualizer`_ module, the following should be installed:

   * For Ubuntu::

       sudo apt-get install python-dev python-pygraphviz python-kiwi
       sudo apt-get install python-pygoocanvas python-gnome2
       sudo apt-get install python-gnomedesktop python-rsvg ipython

   * For MacOS (macports)::

       sudo port install  py27-pygraphviz py27-kiwi py27-goocanvas

.. _visualizer: http://www.nsnam.org/wiki/index.php/PyViz

Downloading ndnSIM source
-------------------------

(Recommended) Custom/unsupported branch of NS-3
+++++++++++++++++++++++++++++++++++++++++++++++

Alternatively, it is possible to download a custom (unsupported) branch of NS-3 that contains all necessary patches and more::

	mkdir ndnSIM
	cd ndnSIM
	git clone git://github.com/cawka/ns-3-dev-ndnSIM.git ns-3
	git clone git://github.com/cawka/pybindgen.git pybindgen
	git clone git://github.com/NDN-Routing/ndnSIM.git ns-3/src/ndnSIM

The first command is to create a directory, which will contain everything NS-3 related.  The bare minimum is just base NS-3 (the first clone above). The second clone gets a module necessary to build python bindings, which are necessary for the visualizer module.  The third clone gets actual ndnSIM code and places it in src/ directory.

There are quite a few modification to the base NS-3 code that are necessary to run ndnSIM, and the code is periodically synchronized with the official developer branch.  Eventually, all the changes will be merged to the official branch, but for the time being, it is necessary to use the customized branch.


Only ndnSIM
+++++++++++

Download NS-3 simulator. For example::

	hg clone http://code.nsnam.org/ns-3-allinone/ ns-3-all
	cd ns-3-all
	./download.py

ndnSIM source code should be placed in ``src/ndnSIM`` folder under NS-3 simulator source tree.  For example::

	cd ns-3-dev
	git clone gitolite@git.irl.cs.ucla.edu:ndn/ndnSIM.git ns-3/src/ndnSIM

.. git clone git://github.com/NDN-Routing/ndnSIM.git ns-3/src/ndnSIM

After cloning, a number of patches need to be applied to the base NS-3 to make sure ndnSIM compiles and works::

	find src/ndnSIM/patches/ -type f -print 0 | xargs -0 patch -p1

Compiling and running ndnSIM
----------------------------

ndnSIM uses standard NS-3 compilation procedure.  For example::

	cd <ns-3-folder>
	./waf configure --enable-examples
	./waf

To run :doc:`sample ndnSIM simulations <examples>`::

	./waf --run=ccnx-simple

or::

	./waf --run=ccnx-grid

.. note::
   Do not forget to configure and compile NS-3 in optimized mode (``./waf configure -d optimized``) in order to run actual simulations.


Documentation
=============

Overall structure of ndnSIM is described in our technical report.

`ndnSIM API documentation <doxygen/index.html>`_

.. It is also possible to build doxygen documentation of ndnSIM API (in ``ns-3/doc/html/``), provided that ``doxygen`` and ``graphviz`` modules are installed on system::

..     ./waf doxygen


A very short guide to the code
------------------------------

All the NDN related code is in ``ns-3/src/ndnSIM``

+-----------------+---------------------------------------------------------------------+
| Folder          | Description                                                         |
+=================+=====================================================================+
| ``model/``      | implementation of NDN base: :ndnsim:`CcnxL3Protocol`, faces         |
|                 | (:ndnsim:`CcnxFace`, :ndnsim:`CcnxNetDeviceFace`, forwarding        |
|                 | :ndnsim:`CcnxAppFace`),                                             |
|                 | strategies (:ndnsim:`CcnxForwardingStrategy`,                       |
|                 | :ndnsim:`CcnxFloodingStrategy`, :ndnsim:`CcnxBestRouteStrategy`),   |
|                 | etc.                                                                |
+-----------------+---------------------------------------------------------------------+
| ``apps/``       | applications (in NS-3 sense) that can be installed on the nodes.    |
|                 | Right now we have one producer (:ndnsim:`CcnxProducer`) and a       |
|                 | collection  of consumer (:ndnsim:`CcnxConsumerCbr`,                 |
|                 | :ndnsim:`CcnxConsumerWindow`,                                       |
|                 | :ndnsim:`CcnxConsumerBatches`).  See doxygen documentation or       |
|                 | source  code for details                                            |
+-----------------+---------------------------------------------------------------------+
| ``helper/``     | a number of :doc:`useful helpers <helpers>`                         |
+-----------------+---------------------------------------------------------------------+
| ``examples/``   | contain :doc:`several example scenarios <examples>`                 |
+-----------------+---------------------------------------------------------------------+
| ``utils/``      | helper classes                                                      |
+-----------------+---------------------------------------------------------------------+
| ``plugins/``    | a number of plugins that may be helpful to run simulation scenarios |
+-----------------+---------------------------------------------------------------------+

Logging
-----------------

Almost every component in ndnSIM exports logging interface, so it is possible in debug compilation of simulator to track many details. For example, by enabling logging of :ndnsim:`CcnxFace` and :ndnsim:`CcnxConsumer` will show everything what happens on :ndnsim:`CcnxFace` and :ndnsim:`CcnxConsumer` classes::

    NS_LOG=CcnxFace:CcnxConsumer ./waf --run=ccnx-simple

Refer to the source code and NS-3 documentation to see what logging interfaces are available and about details how enable one or more logging interfaces.

.. Indices and tables
.. ==================

.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`

