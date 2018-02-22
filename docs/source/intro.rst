.. ndnSIM: NS-3 based NDN simulator
.. ============================================================

============
Introduction
============

.. sidebar:: ndnSIM components

    .. image:: _static/ndnSIM-structure.*
        :width: 100%

A new release of `NS-3 based Named Data Networking (NDN) simulator <http://ndnsim.net/1.0/>`__
went through a number of extensive refactoring and rewriting.  The key new features of the
new version:

- Packet format changed to the `NDN packet format <http://named-data.net/doc/ndn-tlv/>`_

- ndnSIM uses implementation of basic NDN primitives from
  `ndn-cxx library (NDN C++ library with eXperimental eXtensions)
  <http://named-data.net/doc/ndn-cxx/>`_

- All NDN forwarding and management is implemented directly using source code of `Named
  Data Networking Forwarding Daemon (NFD) <http://named-data.net/doc/NFD/>`_

- It is possible to :doc:`simulate some real applications <guide-to-simulate-real-apps>`
  written against ndn-cxx library

.. note::
   Please note that ndnSIM >= 2.0 has been through major refactoring and simulation scenario written for
   ndnSIM 1.0 most likely would need to be changed to run on ndnSIM >= 2.0 platforms.

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

Overall structure of ndnSIM is described in our paper and technical reports.

**For ndnSIM >= 2.0:**

- **S. Mastorakis, A. Afanasyev, and L. Zhang, "On the Evolution of ndnSIM: an Open-Source Simulator for NDN Experimentation," ACM SIGCOMM Computer Communication Review (CCR), July 2017** (`PDF <https://ccronline.sigcomm.org/wp-content/uploads/2017/08/sigcomm-ccr-final112-full-letter.pdf>`__, `BibTex <http://web.cs.ucla.edu/~mastorakis/bibtex-ndnsim>`__)

- **S. Mastorakis, A. Afanasyev, I. Moiseenko, and L. Zhang, "ndnSIM 2: An updated NDN simulator for NS-3," NDN, Technical Report NDN-0028, Revision 2, 2016** (`PDF <https://named-data.net/wp-content/uploads/2016/11/ndn-0028-2-ndnsim-v2.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/399>`__)

**For ndnSIM 1.0:**

- **A. Afanasyev, I. Moiseenko, and L. Zhang, "ndnSIM: NDN simulator for NS-3," NDN, Technical Report NDN-0005, 2012** (`PDF <https://named-data.net/wp-content/uploads/TRndnsim.pdf>`__, `BibTex <http://lasr.cs.ucla.edu/afanasyev/bibwiki/bibtex/367>`__)

`ndnSIM API documentation <doxygen/index.html>`_

Presentations on ndnSIM:

- **S. Mastorakis, A. Afanasyev, and L. Zhang, "On the Evolution of ndnSIM: an Open-Source Ecosystem for NDN Experimentation", NDN Community Meeting (NDNcomm), Memphis, TN, March 2017** (`Slides <https://www.caida.org/workshops/ndn/1703/slides/ndn1703_smastorakis.pptx>`__)

- **A. Afanasyev and others, “Developing Simple Simulations with ndnSIM.” NDN Tutorial at ACM ICN 2016, Kyoto, Japan, Sep-2016** (`Slides <https://users.cs.fiu.edu/~afanasyev/assets/papers/talks-afanasyev2016ndn-tutorial-ndnsim.pdf>`__)

- **S. Mastorakis, "ndnSIM: Current Status & Latest Advancements", 6th NDN Retreat, San Diego, CA, March 2016** (`Slides <https://www.caida.org/workshops/ndn/1603/slides/ndn1603_smastorakis2.pdf>`__)

- **A. Afanasyev, J. Burke, P. Crowley, S. DiBenedetto, V. Jacobson, J. Thompson, B. Zhang, and L. Zhang, “Named Data Networking. ndnSIM: a modular NDN simulator.” An Introduction to NDN and its Software Architecture Tutorial at 1st ACM Conference on Information-Centric Networking (ICN-2014), Sep-2014** (`Slides <https://users.cs.fiu.edu/~afanasyev/assets/papers/talks-afanasyev2014ndnsim.pptx>`__)

- **A. Afanasyev, “Tutorial for students: ndnSIM.” AsiaFI 2013 Summer School, Aug-2013** (`Slides <https://users.cs.fiu.edu/~afanasyev/assets/papers/talks-afanasyev2013ndnsim-tutorial.pptx>`__)

- **A. Afanasyev, I. Moiseenko, and L. Zhang, “ndnSIM: a modular NDN simulator. Introduction and Tutorial.” 3rd NDN retreat, Oct-2012** (`Slides <https://users.cs.fiu.edu/~afanasyev/assets/papers/talks-afanasyev2012ndnsim.pdf>`__)

- **A. Afanasyev, I. Moiseenko, and L. Zhang, “ndnSIM: a modular NDN simulator.” CCNxCon 2012, Sep-2012** (`Slides <https://users.cs.fiu.edu/~afanasyev/assets/papers/talks-ccnxcon-afanasyev2012ndnsim.pdf>`__)

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
|                 | (:ndnsim:`Face`, :ndnsim:`NetDeviceTransport`,                      |
|                 | :ndnsim:`AppLinkService`),                                          |
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
