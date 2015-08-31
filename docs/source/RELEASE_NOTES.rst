ndnSIM Release Notes
====================

This file contains ndnSIM release notes.

All of the ndnSIM documentation is accessible from the `ndnSIM website <http://ndnsim.net>`__.

Release 2.1 (Changes since release 2.0)
---------------------------------------

Release date: September 4, 2015

Overview
~~~~~~~~

- Integration with ndn-cxx and NFD codebases has been refactored to include ndn-cxx and
  NFD repositories as git submodules within ndnSIM repository (:issue:`3138`).

  This refactoring simplifies upgrading ndn-cxx and NFD to new versions and prevents old
  simulation code to break because of API changes in newer versions of ndn-cxx library.

  .. note::
     In order to retrieve the marked versions of ndn-cxx and NFD, use ``--recursive``
     option to the git clone command or run ``git submodule update --init`` after clone,
     pull, or merge.

- The official home for ndnSIM codebase has been moved to `GitHub named-data-ndnSIM
  organization <https://github.com/named-data-ndnSIM>`__ (:issue:`3123`):

  * `ndnSIM codebase <https://github.com/named-data-ndnSIM/ndnSIM>`__
  * `Modified version of ndn-cxx <https://github.com/named-data-ndnSIM/ndn-cxx>`__
  * `Modified version of NFD <https://github.com/named-data-ndnSIM/NFD>`__
  * `Modified version of NS-3 <https://github.com/named-data-ndnSIM/ns-3-dev>`__
  * `Modified version of python bindings generator <https://github.com/named-data-ndnSIM/pybindgen>`__

- Modified version of NS-3 was updated to (rebased on top of) version 2.23-dev, with
  ndnSIM codebase adjusted to reflect API changes (:issue:`3122`)

- NFD and ndn-cxx has been upgraded to version 0.3.4 (:issue:`3125`)
  
New features
~~~~~~~~~~~~

- ndnSIM-specific version of :ndnsim:`ndn::Face` (:issue:`2370`)

  The updated version of :ndnsim:`ndn::Face` specially designed to allow writing
  simulation applications in the same way as real applications.  It is also possible to
  directly use codebase of the existing applications to drive simulations, provided that
  the codebase meets or can be adjusted to meet the requirements listed in
  :doc:`guide-to-simulate-real-apps`.

- Full support for NFD'S RIB manager (:issue:`2370`)

  .. note::
     RIB manager support is currently available only for applications based on ndn-cxx.
     :ndnsim:`FibHelper::AddRoute` and :ndnsim:`FibHelper::RemoveRoute` used by
     :ndnsim:`ndn::Producer` and :ndnsim:`ndn::GlobalRoutingHelper` are currently
     interacting directly with NFD's FIB manager.  This issue will be resolved in the next
     release of ndnSIM (:issue:`3121`)

- Tutorial and example on how to speed up simulations with MPI module of NS-3:
  `<http://ndnsim.net/2.0/parallel-simulations.html>`__

- Two new helpers to simplify writing basic simulation scenarios:

  - :ndnsim:`ScenarioHelper` leverages C++11 constructs to write scenarios. Example:

    .. code-block:: c++

         ScenarioHelper helper;
         helper.createTopology({
             {"1", "2"},
             {"2", "3"}
           });

         helper.addRoutes({
             {"1", "2", "/prefix", 1},
             {"2", "3", "/prefix", 1}
           });

         helper.addApps({
             {"1", "ns3::ndn::ConsumerCbr",
                 {{"Prefix", "/prefix"}, {"Frequency", "1"}},
                 "0s", "100s"},
             {"3", "ns3::ndn::Producer",
                 {{"Prefix", "/prefix"}, {"PayloadSize", "1024"}},
                 "0s", "100s"}
           });


  - :ndnsim:`FactoryCallbackApp` simplifies creation of basic apps without creating a
    separate class that is derived from ``ns3::Applications``. Example:

    .. code-block:: c++

        class SomeApp
        {
        public:
          SomeApp(size_t initParameter);
          ...
        };

        FactoryCallbackApp::Install(node, [] () -> shared_ptr<void> {
            return make_shared<SomeApp>(42);
          })
          .Start(Seconds(1.01));

Improvements and bug fixes
~~~~~~~~~~~~~~~~~~~~~~~~~~

- Updates of ndnSIM documentation

  * Updated the structure for the `ndnSIM website index page <http://ndnsim.net>`__
  * Updated installation instructions to reflect refactoring and relocation of ndnSIM codebase
  * API documentation (doxygen) improvements
  * Updated list of ndnSIM research papers

- The NDN stack can now be updated to handle any simulation topology changes after
  its initial installation on a node (:issue:`2717`)

- Application ID that appears in :ndnsim:`ndn::AppDelayTracer` output is now ID of the
  application on the node, not ID of the application face that was used previously.

- FibHelper has been extended to support route removals (:issue:`2358`)

- ndnSIM codebase now partially covered with unit-tests (:issue:`2369`, :issue:`3059`,
  :issue:`2783`)

- Bugfixes:

  * In :ndnsim:`ndn::GlobalRoutingHelper::CalculateAllPossibleRoutes` that caused crash in
    some cases (:issue:`2535`)

  * In FailLink and Uplink methods of :ndnsim:`ndn::LinkControlHelper` class that affected
    more links than requested (:issue:`2783`)

  * With hop count of data packets retrieved from the Contest Store of NFD (:issue:`2764`)

  * In :ndnsim:`ndn::Producer` application that caused a wrong dummy signature to be added
    to the constructed data packets (:issue:`2927`)



********************************************************************************



Release 2.0 (Changes since release 1.0)
---------------------------------------

Release date: January 13, 2015

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

-  Integration with NFD codebase.
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
