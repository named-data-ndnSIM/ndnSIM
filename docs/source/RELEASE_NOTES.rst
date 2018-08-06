ndnSIM Release Notes
====================

This file contains ndnSIM release notes.

All of the ndnSIM documentation is accessible from the `ndnSIM website <http://ndnsim.net>`__.

Release 2.6
-----------

Release date: August 7, 2018

Overview
~~~~~~~~

- The submodules of NFD and ndn-cxx have been upgraded to version 0.6.2
  (:issue:`4514`).

  `NFD 0.6.2 release notes <https://named-data.net/doc/NFD/0.6.2/RELEASE_NOTES.html>`__

  `ndn-cxx 0.6.2 release notes <https://named-data.net/doc/ndn-cxx/0.6.2/RELEASE_NOTES.html>`__

    .. note::
       In order to retrieve the marked versions of ndn-cxx and NFD, use
       ``--recursive`` option to the git clone command or run ``git
       submodule update --init`` after clone, pull, or merge.

- This will be the last release that supports "old" API for content store implementations.
  Going forward, only NFD-based implementations will be supported.

Improvements and bug fixes
~~~~~~~~~~~~~~~~~~~~~~~~~~

- Add queue length congestion detection and signaling (:issue:`4578`)

- Implement BIC and CUBIC congestion control in ConsumerPcon (:issue:`4672`)

- Fix compilation failure on some systems with Python 3

- Fix outdated and remove deprecated APIs in the examples

- Updates of the ndnSIM documentation.


Release 2.5
-----------

Release date: February 23, 2018

Overview
~~~~~~~~

- The submodules of NFD and ndn-cxx have been upgraded to version 0.6.1
  (:issue:`4514`).

  `NFD 0.6.1 release notes <https://named-data.net/doc/NFD/0.6.1/RELEASE_NOTES.html>`__

  `ndn-cxx 0.6.1 release notes <https://named-data.net/doc/ndn-cxx/0.6.1/RELEASE_NOTES.html>`__

    .. note::
       In order to retrieve the marked versions of ndn-cxx and NFD, use
       ``--recursive`` option to the git clone command or run ``git
       submodule update --init`` after clone, pull, or merge.

Improvements and bug fixes
~~~~~~~~~~~~~~~~~~~~~~~~~~

- Fix memory leak related to canceling events scheduled via ndn-cxx's Scheduler interface
  adapted for NS-3 (:issue:`4509`).

- Fix compilation failure with boost >= 1.66 (:issue:`4471`).

- Updates of the ndnSIM documentation.

Release 2.4
-----------

Release date: November 1, 2017

Overview
~~~~~~~~

- The submodules of NFD and ndn-cxx have been upgraded to version 0.6.0
  (:issue:`4306`).

  `NFD 0.6.0 release notes <https://named-data.net/doc/NFD/0.6.0/RELEASE_NOTES.html>`__

  `ndn-cxx 0.6.0 release notes <https://named-data.net/doc/ndn-cxx/0.6.0/RELEASE_NOTES.html>`__

- Upgrade the base NS-3 version to ``ns-3.27-22-g90fb309d5`` (:issue:`4311`).

    .. note::
       In order to retrieve the marked versions of ndn-cxx and NFD, use
       ``--recursive`` option to the git clone command or run ``git
       submodule update --init`` after clone, pull, or merge.

    .. note::
       To set the maximum size of the used queue when porting already
       implemented scenarios to ndnSIM 2.4, the QueueBase::MaxPackets attribute
       has to be set instead of DropTailQueue::MaxPackets.

    .. note::
       ndnSIM 2.4 requires the following minimal compiler requirements:
         - g++-4.9 or later (Note:  this is an upgraded requirement from ndnSIM 2.3)
         - Apple LLVM version 7.0.2 or later
         - clang-3.3 or later.

Improvements and bug fixes
~~~~~~~~~~~~~~~~~~~~~~~~~~

- Internal refactoring of dummy keychain to work with security v2 of ndn-cxx
  (:issue:`4306`)

- Updates of the ndnSIM documentation and Technical Report (:issue:`4039`, :issue:`4213`)

  * Added page about community contributions to the simulator.
  * CS tracer documentation update.

Release 2.3
-----------

Release date: December 12, 2016

Overview
~~~~~~~~

- The submodules of NFD and ndn-cxx have been both upgraded to version 0.5
  (:issue:`3875`).

  Features of NFD:

    * Adaptive SRTT-based Forwarding strategy has been added.
    * Breaking change -- Configurable policy for admission of unsolicited data packets into the
      content store have been introduced.
    * Introduce mechanism to update properties (e.g., flags, persistency) of
      an existing Face.
    * Breaking change -- ForwarderStatus dataset can now be requested only
      with /localhost/nfd/status/general interest.
    * Breaking change -- Strategy API update. FIB entry is no longer supplied
      to the Strategy::afterReceiveInterest method (i.e., FIB lookup is not
      performed by the forwarding pipelines). When necessary, a strategy can
      request FIB lookup using Strategy::lookupFib.
    * Refactor implementation of RIB Manager to make it uniform with
      other managers.

  Features of ndn-cxx:

    * New transformation API.
    * Introduce Name::deepCopy to allow memory optimizations when working
      with Name objects.
    * New ndn::security::CommandInterestValidator class.
    * New FaceUpdateCommand structure for NFD management protocols.
    * Breaking change - Expose ControlResponse as part of
      Controller::CommandFailCallback.
    * Breaking change - Change security constants to corresponding strongly
      typed enumerations.

    .. note::
       In order to retrieve the marked versions of ndn-cxx and NFD, use
       ``--recursive`` option to the git clone command or run ``git
       submodule update --init`` after clone, pull, or merge.

- Replace NetDeviceFaceLinkService with NetDeviceTransport to add
  full support of NDNLPv2 and, thus, network-layer NACK handling to
  ndnSIM (:issue:`3871`).

  ndnSIM now uses an implementation of nfd::face::Transport that enables the
  full support of NDNLPv2 and the handling of network-layer NACKs generated
  by NFD. NACKs can reach the ndnSIM applications.

  .. note::
     NACK handling by ndnSIM came at the cost of losing the NS3 related
     packet tags. The hopCount tag is now implemented as a tag of a packet
     directly at the NDNLPv2 layer.

New features
~~~~~~~~~~~~

- Enable NACK tracing by the network layer tracers (:issue:`3872`).

- NetworkRegionTable helper was added to allow the configuration
  of the simulated nodes' NetworkRegionTable (:issue:`3806`).

Improvements and bug fixes
~~~~~~~~~~~~~~~~~~~~~~~~~~

- Internal refactoring to use the ndnSIM-specific transport implementation
  (ndn::L3Protocol, ndn::StackHelper, ndn::LinkControlHelper,
  ndn::GlobalRoutingHelper, ndn::Consumer, ndn::Producer).

- Updates of the ndnSIM documentation (:issue:`3876`)

  * Added explanation about the support of NDNLPv2 and its implications.

Release 2.2
-----------

Release date: November 11, 2016

Overview
~~~~~~~~

- The submodules of NFD and ndn-cxx have been both upgraded to version 0.4.1
  (:issue:`3560`).

  Features of NFD:

    * Face system is refactored.
    * Data Retrieval using full names is fixed.
    * Allow setting CS capacity to 0.
    * LinkService provides an "adaptation" layer to
      translate between NDN packets and data blocks communicated through Transport.
    * Face provides combines Transport and LinkServices, providing high-level
      interface to work with Interest/Data/Nack packets inside NFD.
    * Networking NACK in pipelines and best-route strategy.
    * Refactored implementation of NFD management.
    * Interest forwarding processes Link included in interest packets.

  Features of ndn-cxx:

    * LocalControlHeader for special signaling between application and NFD has
      been replaced with NDNLPv2 signaling.
    * NDNLPv2 Network NACK support in Face abstraction.
    * New API in Face class to remove all pending Interests.

  .. note::
     In order to retrieve the marked versions of ndn-cxx and NFD, use
     ``--recursive`` option to the git clone command or run ``git
     submodule update --init`` after clone, pull, or merge.

- Replace NetDeviceFace with NetDeviceFaceLinkService and AppFace with
  AppFaceModel to match NFD's v0.4+ Face model (:issue:`3560`).

  ndnSIM (for now) intentionally uses LinkService instead of Transport for
  optimization purposes and in order to preserve ns3::Packet Tags. This
  may be fixed in the future when there is a different mechanism to
  propagate ns3 Tags.

  .. note::
     This version of ndnSIM does not include support for NDNLPv2 and, thus, cannot
     yet be used to simulate network-level NACKs across the simulated nodes.
     This will be addressed in the next release of ndnSIM.

- ndnSIM no longer officially support Ubuntu Linux 12.04, as it now requires
  a more modern compiler version and dependent libraries.

New features
~~~~~~~~~~~~

- The NetDevice address is now represented as a LocalUri instance for
  NetDevice-based Faces (:issue:`2665`).

- Enable configurability of NFD's managers (:issue:`3328`).

  The managers of NFD can be enabled/disabled as specified in a simulation
  scenario.

Improvements and bug fixes
~~~~~~~~~~~~~~~~~~~~~~~~~~

- Bugfix in RandomPolicy for the "old-style" ContentStore implementation.

- Updates of the `ndnSIM 2 Technical Report <https://named-data.net/publications/techreports/ndn-0028-2-ndnsim-v2/>`__

  Technical Report became up-to-date with the latest version of ndnSIM.
  Please see report's change-log for more detailed information.

- Updates of the ndnSIM documentation (:issue:`3835`)

  * Updated out-dated statements about NFD's CS implementation (:issue:`3827`).
  * Added explanation about the limited support of NDNLPv2 and its implications.
  * Fixed description of the ConsumerBatches application.
  * Added homebrew instructions for dependency installation on OS X.
  * Added specification of ndn::CsTracer output format.

********************************************************************************

Release 2.1
-----------

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


Release 2.0
-----------

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
