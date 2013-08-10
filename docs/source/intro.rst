.. ndnSIM: NS-3 based NDN simulator
.. ============================================================

============
Introduction
============

.. sidebar:: ndnSIM components

    .. image:: _static/ndnsim-components.*
        :width: 100%

The ndnSIM is NS-3 module that implements Named Data Networking (NDN) communication model, the clean slate Internet design. ndnSIM is specially optimized for simulation purposes and has a cleaner and more extensible internal structure comparing to the existing NDN implementation (NDNx).

Following the NDN architecture, ndnSIM is implemented as a new network-layer protocol model, which can run on top of any available link-layer protocol model (point-to-point, CSMA, wireless, etc.).

.. note::
    It will also be possible to run ndnSIM on top of network-layer (IPv4, IPv6) and transport-layer (TCP, UDP) protocols.
    However, it is not yet implemented and patches are welcome.

.. This flexibility allows ndnSIM to simulate scenarios of various homogeneous and heterogeneous networks (e.g., NDN-only, NDN-over-IP, etc.).

The simulator is implemented in a modular fashion, using separate C++ classes to model behavior of each network-layer entity in NDN: :ndnsim:`pending Interest table (PIT) <Pit>`, :ndnsim:`forwarding information base (FIB) <Fib>`, :ndnsim:`content store <ContentStore>`, :ndnsim:`network <NetDeviceFace>` and :ndnsim:`application <AppFace>` interfaces, :ndnsim:`Interest forwarding strategies <ForwardingStrategy>`, etc.
This modular structure allows any component to be easily modified or replaced with no or minimal impact on other components.
In addition, the simulator provides an extensive collection of interfaces and helpers to perform detailed tracing behavior of every component, as well as NDN traffic flow.

The current wire format for the Interest and Data packets used by ndnSIM is defined in :ref:`ndnSIM packet format`.
ndnSIM also has an option to be compatible to wire format used by `NDNx implementation <http://named-data.net/>`_ (NDNx binary XML encoding).  However currently, this option is deprecated, but can be reintroduced in the future as an optional wire format.

More documentation
------------------

Overall structure of ndnSIM is described in our `technical report <http://lasr.cs.ucla.edu/afanasyev/data/files/Afanasyev/ndnSIM-TR.pdf>`_.

`ndnSIM API documentation <doxygen/index.html>`_

Also, you can `join our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_ to see and participate in discussions about ndnSIM implementation and simulations in general.   
Do not forget to check mailling list `archives <http://www.lists.cs.ucla.edu/pipermail/ndnsim/>`_.  


Support
-------

The code of ndnSIM is in active development.  Bug reports (issues) as well as new feature implementation are always welcome. 

To file a bug report, please use `GitHub Issues <https://github.com/NDN-Routing/ndnSIM/issues>`_.

To create new feature, please fork the code and submit Pull Request on GitHub.

And of course, `our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_ is the best way to communicate with and get support from ndnSIM team and other users of ndnSIM.

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
-------

Almost every component in ndnSIM exports logging interface, so in debug compilation it is possible to track many internal details. 
For example, logging of :ndnsim:`Face` and :ndnsim:`Consumer` shows everything what happens in :ndnsim:`Face` and :ndnsim:`Consumer` classes::

    NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

Refer to the source code and NS-3 documentation to see what logging interfaces are available and about details how enable one or more logging interfaces.

