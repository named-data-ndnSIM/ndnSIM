ndnSIM Release Notes
====================

This file contains ndnSIM release notes.

All of the ndnSIM documentation is accessible from the `ndnSIM website <http://ndnsim.net>`__.

Release 2.0
-----------

Overview
~~~~~~~~

ndnSIM 2.0 is a new release of NS-3 based Named Data Networking (NDN) simulator that went through
extensive refactoring and rewriting. The key new features of the new version:

-  ndnSIM no longer re-implements basic NDN primitives and directly uses implementation from
   `ndn-cxx library (NDN C++ library with eXperimental
   eXtensions) <http://named-data.net/doc/ndn-cxx/>`__.

-  All NDN forwarding and management is implemented directly using source code of `Named Data
   Networking Forwarding Daemon (NFD) <http://named-data.net/doc/NFD/>`__. The current code is based
   on ``a22a2172611b1cb93b2e2f53d9d5da122b384f3e`` commit of `NFD
   repository <https://github.com/named-data/NFD/tree/a22a2172611b1cb93b2e2f53d9d5da122b384f3e>`__.

Note RIB Manager is not yet available in ndnSIM.

-  Packet format changed to the `NDN packet format <http://named-data.net/doc/ndn-tlv/>`__.

-  Code style changes to conform to `ndn-cxx Code Style and Coding
   Guidelines <http://named-data.net/doc/ndn-cxx/current/code-style.html>`__ This change also
   includes renaming of the header and source files: ``*.h`` -> ``*.hpp``, ``*.cc`` -> ``*.cpp``

-  ndnSIM now uses C++11.

New Features
~~~~~~~~~~~~

-  Integration with NFD codebase:
-  A realistic behavior is added to the simulations.
-  Forwarding plane extensions can be used in both ndnSIM simulations and real NFD deployment.
-  Per namespace forwarding strategies for different namespaces (one strategy per namespace).
-  New examples:
-  ``ndn-load-balancer``
-  ``ndn-grid-multiple-strategies``
-  ``ndn-different-strategy-per-prefix``
-  Basic examples using python bindings: ``ndn-simple.py`` and ``ndn-grid.py``
-  Use of the the full-featured NDN packet format.
-  Full-featured support for Interest selectors.
-  Full-featured crypto operations can be simulated (disabled by default).
-  FibHelper to manage FIB entries.
-  StrategyChoiceHelper to manage per-namespace forwarding strategy selection.

Changes
~~~~~~~

-  HopCount tracing now includes only one way network-level hop count (e.g., the number of physical
   links traversed by a packet). Previously, this tracing was round-trip and included applications
   hops.
-  Python bindings have changed and, due to limitations of pybindgen, currently cover a smaller
   subset of C++ code (`Task #2341 <http://redmine.named-data.net/issues/2341>`__).
-  LinkControlHelper now uses ErrorRate to simulate link failure/recovery. Previously it was relying
   on Up/Down flag on NDN level.
-  The Face abstraction of NFD is now used.

Bug fixes
~~~~~~~~~

-  Fix processing files with customized LossRate or Queue model in AnnotatedTopologyReader `Bug
   #2354 <http://redmine.named-data.net/issues/2354>`__.

Removals
~~~~~~~~

-  PyNDN emulation
-  (temporarily) ApiFace. Will be replaced with emulation of ndn-cxx ``ndn::Face`` in future
   releases (`Issue #2370 <http://redmine.named-data.net/issues/2370>`__).
-  (temporarily) UdpFace, TcpFace (`Issue #2371 <http://redmine.named-data.net/issues/2371>`__).
-  Limits, LimitsWindow, LimitsRate.
-  PIT, FIB with "replacement" policies.
-  Old deprecated packet formats.
