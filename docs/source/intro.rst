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

The simulator is implemented in a modular fashion, using separate C++ classes to model behavior of each network-layer entity in NDN: :ndnsim:`pending Interest table (PIT) <Pit>`, :ndnsim:`forwarding information base (FIB) <Fib>`, :ndnsim:`content store <ContentStore>`, :ndnsim:`network <NetDeviceFace>` and :ndnsim:`application <AppFace>` interfaces, :ndnsim:`Interest forwarding strategies <ForwardingStrategy>`, etc.
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
    .           |      "Face"      |	     |        "Face"        |		  .
    .           |    "(AppFace)"   |	     |   "(NetDeviceFace)"  |		  .
    .		+------------------+         +----------------------+		  .
    .		               ^                   ^				  .
    .			       |                   |				  .
    .			       v                   v				  .
    .			    XXXXXXXXXXXXXXXXXXXXXXXXXXXXX			  .
    .			    XX                         XX			  .
    .			    XX    Core NDN protocol    XX  			  .
    .                       XX      "(L3Protocol)"     XX
    .			    XX                         XX			  .
    .			    XXXXXXXXXXXXXXXXXXXXXXXXXXXXX			  .
    .			      ^       ^       ^       ^				  .
    .			      |	      |	      |	      |				  .
    .     +-------------------+   +---+       +---+   +------------+		  .
    .	  | 		    	  |		  |    		   |		  .
    .	  v			  v		  v		   v		  .
    . +-------------------+      +-------+      +-------+        +-------------+  .
    . |   "ContentStore"  |      |  PIT  |      |  FIB  |        | "Forwarding"|  .
    . +-------------------+      +-------+      +-------+        | "Strategy"  |  .
    .							         +-------------+  .
    .										  .
    ...............................................................................


The wire format of Interest and Data packets follows the format of the existing `CCNx Project's NDN implementation`_ (CCNx Binary Encoding), allowing reuse of the existing traffic analysis tools, as well as driving simulations using real NDN traffic traces.

.. _CCNx Project's NDN implementation: http://www.ccnx.org/

Getting Started
===============

Portability
------------

ndnSIM has been successfully compiled and used under Ubuntu Linux 12.04 (stock gcc, boost 1.48), Mac OS 10.8 (gcc-4.2 apple/llvm, macports gcc 4.7, boost 1.49 or 1.50).

Requirements
-------------

1. ndnSIM requires the customized version of NS-3 simulator (a number of patches required to make ndnSIM work with the latest development branch of NS-3).

2. Boost libraries should be installed on the system:

   * For Ubuntu::

       sudo aptitude install libboost-all-dev

   * For MacOS (macports)::

       sudo port instal boost

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

ndnSIM uses standard NS-3 compilation procedure.  For example::

	cd <ns-3-folder>
	./waf configure --enable-examples --enable-ndn-plugins=topology,mobility
	./waf



To run :doc:`sample ndnSIM simulations <examples>`::

	./waf --run=ccnx-simple

or::

	./waf --run=ccnx-grid

.. note::
   Do not forget to configure and compile NS-3 in optimized mode (``./waf configure -d optimized``) in order to run actual simulations.

Additional compiling options
++++++++++++++++++++++++++++

ndnSIM contains a number of NS-3 extensions that are not technically part of the ndnSIM.  Right now there are two optional plugins---topology and mobility---which can be enabled using the following configuration option::

	./waf configure --enable-ndn-plugins=topology,mobility


Documentation
=============

Overall structure of ndnSIM is described in our `technical report <http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ndnSIM-TR.pdf>`_.

`ndnSIM API documentation <doxygen/index.html>`_

.. It is also possible to build doxygen documentation of ndnSIM API (in ``ns-3/doc/html/``), provided that ``doxygen`` and ``graphviz`` modules are installed on system::

..     ./waf doxygen

Support
=======

The code of ndnSIM is in active development.  Bug reports (issues) as well as new feature implementation are always welcome. 

To file a bug report, please use `GitHub Issues <https://github.com/NDN-Routing/ndnSIM/issues>`_.

To create new feature, please fork the code and submit Pull Request on GitHub.


A very short guide to the code
------------------------------

All the NDN related code is in ``ns-3/src/ndnSIM``

+-----------------+---------------------------------------------------------------------+
| Folder          | Description                                                         |
+=================+=====================================================================+
| ``model/``      | implementation of NDN base: :ndnsim:`L3Protocol`, faces             |
|                 | (:ndnsim:`Face`, :ndnsim:`NetDeviceFace`, forwarding                |
|                 | :ndnsim:`AppFace`),                                                 |
|                 | strategies (:ndnsim:`ForwardingStrategy`,                           |
|                 | :ndnsim:`Flooding`, :ndnsim:`SmartFlooding`, :ndnsim:`BestRoute`),  |
|                 | etc.                                                                |
+-----------------+---------------------------------------------------------------------+
| ``apps/``       | applications (in NS-3 sense) that can be installed on the nodes.    |
|                 | Right now we have one producer (:ndnsim:`Producer`) and a           |
|                 | collection  of consumer (:ndnsim:`ConsumerCbr`,                     |
|                 | :ndnsim:`ConsumerWindow`,                                           |
|                 | :ndnsim:`ConsumerBatches`).  See doxygen documentation or           |
|                 | source  code for details                                            |
+-----------------+---------------------------------------------------------------------+
| ``helper/``     | a number of :doc:`useful helpers <helpers>`                         |
+-----------------+---------------------------------------------------------------------+
| ``examples/``   | contain :doc:`several example scenarios <examples>`                 |
+-----------------+---------------------------------------------------------------------+
| ``utils/``      | helper classes, including implementation of generalized data        |
|                 | structures                                                          |
+-----------------+---------------------------------------------------------------------+
| ``plugins/``    | a number of plugins that may be helpful to run simulation scenarios |
+-----------------+---------------------------------------------------------------------+

Logging
-----------------

Almost every component in ndnSIM exports logging interface, so it is possible in debug compilation of simulator to track many details. For example, by enabling logging of :ndnsim:`Face` and :ndnsim:`Consumer` will show everything what happens on :ndnsim:`Face` and :ndnsim:`Consumer` classes::

    NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

Refer to the source code and NS-3 documentation to see what logging interfaces are available and about details how enable one or more logging interfaces.

.. Indices and tables
.. ==================

.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`

