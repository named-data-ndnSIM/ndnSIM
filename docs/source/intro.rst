.. ndnSIM: NS-3 based NDN simulator
.. ============================================================

============
Introduction
============

.. sidebar:: ndnSIM components

    .. image:: _static/ndnSIM-structure.*
        :width: 100%

- Packet format changed to [NDN Packet Specification](http://named-data.net/doc/ndn-tlv/)

- ndnSIM uses implementation of basic NDN primitives from
  [ndn-cxx library (NDN C++ library with eXperimental eXtensions)](http://named-data.net/doc/ndn-cxx/)

- All NDN forwarding and management is implemented directly using source code of
  [Named Data Networking Forwarding Daemon (NFD)](http://named-data.net/doc/NFD/)

- Allows [simulation of real applications](http://ndnsim.net/2.1/guide-to-simulate-real-apps.html)
  written against ndn-cxx library

                
A new release of `NS-3 based Named Data Networking (NDN) simulator <http://ndnsim.net/1.0/>`__
went through a number of extensive refactoring and rewriting.  The key new features of the
new version:

- Packet format changed to the `NDN packet format <http://named-data.net/doc/ndn-tlv/>`_

- ndnSIM uses implementation of basic NDN primitives from
  `ndn-cxx library (NDN C++ library with eXperimental eXtensions)
  <http://named-data.net/doc/ndn-cxx/>`_

- All NDN forwarding and management is implemented directly using source code of `Named
  Data Networking Forwarding Daemon (NFD) <http://named-data.net/doc/NFD/>`_

- It is possible to :doc:`simulation some real applications <guide-to-simulate-real-apps>`
  written against ndn-cxx library

.. note::
   Please note that ndnSIM 2.0 has major refactoring and simulation scenario written for
   previous releases most likely would need to be changed to run on ndnSIM 2.0 platform.

Such integration with ndn-cxx and NFD ensures that the simulations are maximally realistic
and can be reproduced in real environments with virtually no changes to the source code.
In addition to that, any experiments with NDN forwarding (e.g., custom forwarding
strategies) inside ndnSIM can be directly used within the real NFD implementation.

Specifically, the simulator directly uses NFD implementations of :nfd:`pending Interest
table (PIT) <nfd::Pit>`, :nfd:`forwarding information base (FIB) <nfd::Fib>`, and
:nfd:`content store <nfd::cs::Cs>` data structures.  In addition to that, ndnSIM allows
experimentation with :ndnSIM:`ndnSIM-specific content store <ndn::ContentStore>` along
with its cache replacement policies ported from the previous version of ndnSIM.

ndnSIM is implemented as a new network-layer protocol model and can run on top of any
available link-layer protocol model (point-to-point, CSMA, wireless, etc.).
In addition, the simulator provides an extensive collection of interfaces and helpers to
perform detailed tracing behavior of every component, as well as NDN traffic flow.


More documentation
------------------

Overall structure of ndnSIM is described in our `technical report
<http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ndnSIM-TR.pdf>`_.

`ndnSIM API documentation <doxygen/index.html>`_

Also, you can `join our mailing list
<http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_ to see and participate in
discussions about ndnSIM implementation and simulations in general.  Do not forget to
check mailling list `archives <http://www.lists.cs.ucla.edu/pipermail/ndnsim/>`_.


Support
-------

The code of ndnSIM is in active development.  Bug reports (issues) as well as new feature
implementation are always welcome.

To file a bug report, please use `NDN Redmine
<http://redmine.named-data.net/projects/ndnsim>`_.

To create new feature, please fork the code and submit Pull Request on GitHub.

And of course, `our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_
is the best way to communicate with and get support from ndnSIM team and other users of
ndnSIM.

A very short guide to the code
------------------------------

All the NDN related code is in ``ns-3/src/ndnSIM``

+-----------------+---------------------------------------------------------------------+
| Folder          | Description                                                         |
+=================+=====================================================================+
| ``model/``      | implementation of NDN base: :ndnsim:`L3Protocol`, faces             |
|                 | (:ndnsim:`Face`, :ndnsim:`NetDeviceFace`,                           |
|                 | :ndnsim:`AppFace`),                                                 |
|                 | etc.                                                                |
+-----------------+---------------------------------------------------------------------+
| ``NFD/``        | submodule of `NDN Forwarding Daemon (NFD)`_ source code with few    |
|                 | modifications to make it compatible with the simulator              |
+-----------------+---------------------------------------------------------------------+
| ``ndn-cxx/``    | submodule of `ndn-cxx library`_ source code with few                |
|                 | modifications to make it compatible with the simulator              |
+-----------------+---------------------------------------------------------------------+
| ``apps/``       | applications (in NS-3 sense) that can be installed on the nodes.    |
|                 | Right now we have one producer (:ndnsim:`Producer`) and a           |
|                 | collection  of consumer applications (:ndnsim:`ConsumerCbr`,        |
|                 | :ndnsim:`ConsumerWindow`, :ndnsim:`ConsumerBatches`,                |
|                 | :ndnsim:`ConsumerZipfMandelbrot`).  See doxygen documentation or    |
|                 | source  code for details                                            |
+-----------------+---------------------------------------------------------------------+
| ``utils/``      | helper classes, including implementation of generalized data        |
|                 | structures, topology readers and tracers                            |
+-----------------+---------------------------------------------------------------------+
| ``helper/``     | a number of :doc:`useful helpers <helpers>`                         |
+-----------------+---------------------------------------------------------------------+
| ``examples/``   | contain :doc:`several example scenarios <examples>`                 |
+-----------------+---------------------------------------------------------------------+

.. _NDN Forwarding Daemon (NFD): http://named-data.net/doc/NFD/

.. _ndn-cxx library: http://named-data.net/doc/ndn-cxx/

Logging
-------

Almost every component in ndnSIM exports logging interface, so in debug compilation it is
possible to track many internal details.  For example, logging of :ndnsim:`Face` and
:ndnsim:`Consumer` shows everything what happens in :ndnsim:`Face` and :ndnsim:`Consumer`
classes::

    NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

Refer to the source code and NS-3 documentation to see what logging interfaces are
available and about details how enable one or more logging interfaces.

.. note::

   Please remember that logging is enabled **only** in debug mode.  When simulator is
   compiled in optimized mode (``./waf configure -d optimized``), logging will be
   completely disabled as it significantly slows down execution.

.. note::

   A list of log components available in the current version of NS-3 and ndnSIM is listed
   `in this page <doxygen/LogComponentList.html>`_.
