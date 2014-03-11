
Obtaining metrics
=================

To obtain simulation results, you would need to connect to one or more `trace sources <doxygen/group___trace_source_list.html>`_ provided by ndnSIM classes.

It is also possible to use existing trace helpers, which collects and aggregates requested statistical information in text files.

.. _trace classes:

Packet-level trace helpers
--------------------------

- :ndnsim:`ndn::L3AggregateTracer`

    Tracing the aggregate number of Interests/Data packets forwarded by an NDN node

    The following example enables tracing on all simulation nodes:

    .. code-block:: c++

        // the following should be put just before calling Simulator::Run in the scenario

        ndn::L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (1.0));

        Simulator::Run ();

        ...


    Output file format is tab-separated values, with first row specifying names of the columns.  Refer to the following table for the description of the columns:

    +------------------+---------------------------------------------------------------------+
    | Column           | Description                                                         |
    +==================+=====================================================================+
    | ``Time``         | simulation time                                                     |
    +------------------+---------------------------------------------------------------------+
    | ``Node``         | node id, globally unique                                            |
    +------------------+---------------------------------------------------------------------+
    | ``FaceId``       | interface ID (-1 for combined metric)                               |
    +------------------+---------------------------------------------------------------------+
    | ``Type``         | Type of measurements:                                               |
    |                  |                                                                     |
    |                  | - ``InInterests``  measurements of incoming Interests               |
    |                  | - ``OutInterests``  measurements of outgoing Interests              |
    |                  | - ``DropInterests``  measurements of dropped Interests              |
    |                  | - ``InData``  measurements of incoming Data                         |
    |                  | - ``OutData``  measurements of outgoing Data                        |
    |                  | - ``DropData``  measurements of dropped Data                        |
    |                  | - ``InNacks``  measurements of incoming NACKs                       |
    |                  | - ``OutNacks``  measurements of outgoing NACKs                      |
    |                  | - ``DropNacks``  measurements of dropped NACKs                      |
    |                  | - ``SatisfiedInterests`` measurements of satisfied Interests        |
    |                  | - ``TimedOutInterests`` measurements of timed out Interests         |
    +------------------+---------------------------------------------------------------------+
    | ``Packets``      | absolute number of packets within last averaging period             |
    |                  | (number of packets).                                                |
    +------------------+---------------------------------------------------------------------+
    | ``Kilobytes``    | absolute number of kilobytes transferred within the last averaging  |
    |                  | period  (number of packets).                                        |
    +------------------+---------------------------------------------------------------------+

- :ndnsim:`ndn::L3RateTracer`

    Tracing the rate in bytes and in number of packets of Interest/Data packets forwarded by an NDN node

    The following example enables tracing on all simulation nodes:

    .. code-block:: c++

        // the following should be put just before calling Simulator::Run in the scenario

        ndn::L3RateTracer::InstallAll ("rate-trace.txt", Seconds (1.0));

        Simulator::Run ();

        ...

    Output file format is tab-separated values, with first row specifying names of the columns.  Refer to the following table for the description of the columns:

    +------------------+---------------------------------------------------------------------+
    | Column           | Description                                                         |
    +==================+=====================================================================+
    | ``Time``         | simulation time                                                     |
    +------------------+---------------------------------------------------------------------+
    | ``Node``         | node id, globally unique                                            |
    +------------------+---------------------------------------------------------------------+
    | ``FaceId``       | interface ID (-1 for combined metric)                               |
    +------------------+---------------------------------------------------------------------+
    | ``Type``         | Type of measurements:                                               |
    |                  |                                                                     |
    |                  | - ``InInterests``  measurements of incoming Interests               |
    |                  | - ``OutInterests``  measurements of outgoing Interests              |
    |                  | - ``DropInterests``  measurements of dropped Interests              |
    |                  | - ``InData``  measurements of incoming Data                         |
    |                  | - ``OutData``  measurements of outgoing Data                        |
    |                  | - ``DropData``  measurements of dropped Data                        |
    |                  | - ``InNacks``  measurements of incoming NACKs                       |
    |                  | - ``OutNacks``  measurements of outgoing NACKs                      |
    |                  | - ``DropNacks``  measurements of dropped NACKs                      |
    |                  | - ``InSatisfiedInterests`` measurements of incoming satisfied       |
    |                  |   Interests                                                         |
    |                  | - ``InTimedOutInterests`` measurements of incoming timed out        |
    |                  |   Interests                                                         |
    |                  | - ``OutSatisfiedInterests`` measurements of outgoing satisfied      |
    |                  |   Interests                                                         |
    |                  | - ``OutTimedOutInterests`` measurements of outgoing satisfied       |
    |                  |   Interests                                                         |
    +------------------+---------------------------------------------------------------------+
    | ``Packets``      | estimated rate (EWMA average) of packets within the last averaging  |
    |                  | period (number of packets/s).                                       |
    +------------------+---------------------------------------------------------------------+
    | ``Kilobytes``    | estimated rate (EWMA average) within last averaging                 |
    |                  | period (kilobytes/s)                                                |
    +------------------+---------------------------------------------------------------------+
    | ``PacketsRaw``   | absolute number of packets within last averaging period             |
    |                  | (number of packets).                                                |
    +------------------+---------------------------------------------------------------------+
    | ``KilobytesRaw`` | absolute number of kilobytes transferred within the last averaging  |
    |                  | period  (number of packets).                                        |
    +------------------+---------------------------------------------------------------------+

- :ndnsim:`L2Tracer`

    This tracer is similar in spirit to :ndnsim:`ndn::L3RateTracer`, but it currently traces only packet drop on layer 2 (e.g.,
    due to transmission queue overflow).

    The following example enables tracing on all simulation nodes:

    .. code-block:: c++

        // the following should be put just before calling Simulator::Run in the scenario

        L2RateTracer::InstallAll ("drop-trace.txt", Seconds (0.5));

        Simulator::Run ();

        ...

    Output file format is tab-separated values, with first row specifying names of the columns.  Refer to the following table for the description of the columns:

    +------------------+---------------------------------------------------------------------+
    | Column           | Description                                                         |
    +==================+=====================================================================+
    | ``Time``         | simulation time                                                     |
    +------------------+---------------------------------------------------------------------+
    | ``Node``         | node id, globally unique                                            |
    +------------------+---------------------------------------------------------------------+
    | ``Interface``    | interface name (currently only "combined")                          |
    +------------------+---------------------------------------------------------------------+
    | ``Type``         | Type of measurements:                                               |
    |                  |                                                                     |
    |                  | - ``Drop``  measurements of dropped packets                         |
    +------------------+---------------------------------------------------------------------+
    | ``Packets``      | estimated rate (EWMA average) of packets within the last averaging  |
    |                  | period (number of packets/s).                                       |
    +------------------+---------------------------------------------------------------------+
    | ``Kilobytes``    | estimated rate (EWMA average) within last averaging                 |
    |                  | period (kilobytes/s)                                                |
    +------------------+---------------------------------------------------------------------+
    | ``PacketsRaw``   | absolute number of packets within last averaging period             |
    |                  | (number of packets).                                                |
    +------------------+---------------------------------------------------------------------+
    | ``KilobytesRaw`` | absolute number of kilobytes transferred within the last averaging  |
    |                  | period  (number of packets).                                        |
    +------------------+---------------------------------------------------------------------+

.. note::

    A number of other tracers are available in ``plugins/tracers-broken`` folder, but they do not yet work with the current code.
    Eventually, we will port most of them to the current code, but it is not our main priority at the moment and would really appreciate help with writing new tracers and porting the old ones.

.. _packet trace helper example:

Example of packet-level trace helpers
+++++++++++++++++++++++++++++++++++++

This example (``ndn-tree-tracers.cc``) demonstrates basic usage of :ref:`trace classes`.

In this scenario we will use a tree-like topology, where consumers are installed on leaf nodes and producer is in the root of the tree:

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 100

         /--------\    /--------\    /--------\    /--------\
         |"leaf-1"|    |"leaf-2"|    |"leaf-3"|    |"leaf-4"|
         \--------/    \--------/    \--------/    \--------/
               ^          ^                ^           ^
               |          |                |           |
                \        /                  \         /
                 \      /                    \       /    10Mbps / 1ms
                  \    /                      \     /
                   |  |                        |   |
                   v  v                        v   v
                /-------\                    /-------\
                |"rtr-1"|                    |"rtr-2"|
                \-------/                    \-------/
                      ^                        ^
                      |                        |
                       \                      /  10 Mpbs / 1ms
                        +--------\  /--------+
                                 |  |
                                 v  v
                              /--------\
                              | "root" |
                              \--------/

The corresponding topology file (``topo-tree.txt``):

.. literalinclude:: ../../examples/topologies/topo-tree.txt
    :language: bash
    :linenos:
    :lines: 1-2,27-

Example simulation (``ndn-tree-tracers.cc``) scenario that utilizes trace helpers:

.. literalinclude:: ../../examples/ndn-tree-tracers.cc
    :language: c++
    :linenos:
    :lines: 21-28,61-
    :emphasize-lines: 57-58

To run this scenario, use the following command::

        ./waf --run=ndn-tree-tracers

The successful run will create ``rate-trace.txt`` and ``aggregate-trace.txt`` files in the current directly, which can be analyzed manually or used as input to some graph/stats packages.

.. sidebar:: Graph build using the `R script <http://www.r-project.org/>`_

    .. image:: _static/root-rates.png
        :alt: Interest/Data packet rates at the root node

    .. image:: _static/root-5sec-counts.png
        :alt: Interest/Data packet counts at the root node in 5-second intervals

.. literalinclude:: ../../examples/graphs/rate-graph.R
    :language: r
    :linenos:

For more information about R and ggplot2, please refer to `R language manual <http://cran.r-project.org/manuals.html>`_, `ggplot2 module manual <http://docs.ggplot2.org/current/>`_.

.. _Example of packet drop tracer (L2Tracer):

Example of packet drop tracer (L2Tracer)
----------------------------------------

.. sidebar:: Topology

    .. image:: _static/topo-tree-25-node.png
        :alt: 25-node tree topology
        :width: 550px

This example (``ndn-tree-with-l2tracer.cc``) demonstrates basic usage of :ref:`trace classes`.

The corresponding topology file (``topo-tree-25-node.txt``):

.. literalinclude:: ../../examples/topologies/topo-tree-25-node.txt
    :language: bash
    :linenos:
    :lines: 2-

Example simulation (``ndn-tree-with-l2tracer.cc``) scenario that utilizes trace helpers:

.. literalinclude:: ../../examples/ndn-tree-with-l2tracer.cc
    :language: c++
    :linenos:
    :lines: 1-
    :emphasize-lines: 16,135

To run this scenario, use the following command::

        ./waf --run=ndn-tree-with-l2tracer

The successful run will create ``drop-trace.txt`` file in the current directly, which can be analyzed manually or used as input to some graph/stats packages.

For example, the following `R script <http://www.r-project.org/>`_ will build a number of nice graphs:

.. literalinclude:: ../../examples/graphs/drop-graph.R
    :language: r
    :linenos:

.. image:: _static/l2-rate-tracer.png
   :alt: Packet drop rates on routers

.. _cs trace helper:

Content store trace helper
--------------------------

- :ndnsim:`ndn::CsTracer`

    With the use of :ndnsim:`ndn::CsTracer` it is possible to obtain statistics of cache hits/cache misses on simulation nodes.

    The following code enables content store tracing:

    .. code-block:: c++

        // the following should be put just before calling Simulator::Run in the scenario

        ndn::CsTracer::InstallAll ("cs-trace.txt", Seconds (1));

        Simulator::Run ();

        ...

.. - Tracing lifetime of content store entries

..     Evaluate lifetime of the content store entries can be accomplished using modified version of the content stores.
..     In particular,

.. _cs trace helper example:

Example of content store trace helper
+++++++++++++++++++++++++++++++++++++

This example (``ndn-tree-cs-tracers.cc``) demonstrates basic usage of content store tracer.

In this scenario we will use the same tree-like topology as in :ref:`previous example <packet trace helper example>`, where consumers are installed on leaf nodes and producer is in the root of the tree.
The main difference is that each client request data from the same namespace: /root/1, /root/2, ...  Another small difference is that in this scenario we start our application not at the same time, but 10 ms apart.

Example simulation (``ndn-tree-cs-tracers.cc``) scenario that utilizes trace helpers:

.. literalinclude:: ../../examples/ndn-tree-cs-tracers.cc
    :language: c++
    :linenos:
    :lines: 21-28,61-
    :emphasize-lines: 59


To run this scenario, use the following command::

        ./waf --run=ndn-tree-cs-tracers

The successful run will create ``cs-trace.txt``, which similarly to trace file from the :ref:`tracing example <packet trace helper example>` can be analyzed manually or used as input to some graph/stats packages.


Application-level trace helper
------------------------------

- :ndnsim:`ndn::AppDelayTracer`

    With the use of :ndnsim:`ndn::AppDelayTracer` it is possible to obtain data about for delays between issuing Interest and receiving corresponding Data packet.

    The following code enables application-level Interest-Data delay tracing:

    .. code-block:: c++

        // the following should be put just before calling Simulator::Run in the scenario

        ndn::AppDelayTracer::InstallAll ("app-delays-trace.txt");

        Simulator::Run ();

        ...

    Output file format is tab-separated values, with first row specifying names of the columns.  Refer to the following table for the description of the columns:

    +-----------------+---------------------------------------------------------------------+
    | Column          | Description                                                         |
    +=================+=====================================================================+
    | ``Time``        | simulation time when SeqNo was receivied                            |
    +-----------------+---------------------------------------------------------------------+
    | ``Node``        | node id, global unique                                              |
    +-----------------+---------------------------------------------------------------------+
    | ``AppId``       | app id, local unique on the node, not global                        |
    +-----------------+---------------------------------------------------------------------+
    | ``SeqNo``       | seq number of the Interest-Data                                     |
    +-----------------+---------------------------------------------------------------------+
    | ``Type``        | Type of delay:                                                      |
    |                 |                                                                     |
    |                 | - ``LastDelay`` means that ``DelayS`` and ``DelayUS`` represent     |
    |                 |   delay between last Interest sent and Data packet received         |
    |                 | - ``FullDelay`` means that ``DelayS`` and ``DelayUS`` represent     |
    |                 |   delay between first Interest sent and Data packet received        |
    |                 |   (i.e., includes time of Interest retransmissions)                 |
    +-----------------+---------------------------------------------------------------------+
    | ``DelayS``      | delay value, specified in seconds                                   |
    +-----------------+---------------------------------------------------------------------+
    | ``DelayUS``     | delay value, specified in microseconds (10^-6)                      |
    +-----------------+---------------------------------------------------------------------+
    | ``RetxCount``   | number of Interest retransmissions (for LastDelay always equal to 1)|
    +-----------------+---------------------------------------------------------------------+
    | ``HopCount``    | combined number of number of hops for Interest and Data packet.     |
    |                 | Note that HopCount is increased anytime somebody calls Send method  |
    |                 | on a face, including delivery of Interest/Data to application via   |
    |                 | an AppFace (but not from application to ndn::L3Protocol!).          |
    |                 |                                                                     |
    |                 | One consequence is that Interests satisfied by an app will have     |
    |                 | even hop count (min hop count = 2), and Interests satisfied from    |
    |                 | caches will have odd hop count (min hop count = 1)                  |
    +-----------------+---------------------------------------------------------------------+

.. _app delay trace helper example:

Example of application-level trace helper
+++++++++++++++++++++++++++++++++++++++++

This example (``ndn-tree-app-delay-tracer.cc``) demonstrates basic usage of application-level Interest-Data delay tracer.

In this scenario we will use the same tree-like topology as in :ref:`packet trace helper example <packet trace helper example>`, where consumers are installed on leaf nodes and producer is in the root of the tree and clients request data from the same namespace: /root/1, /root/2, ...

Example simulation (``ndn-tree-app-delay-tracer.cc``) scenario that utilizes trace helpers:

.. literalinclude:: ../../examples/ndn-tree-app-delay-tracer.cc
    :language: c++
    :linenos:
    :lines: 21-28,61-
    :emphasize-lines: 59


To run this scenario, use the following command::

        ./waf --run=ndn-tree-app-delay-tracer

The successful run will create ``app-delays-trace.txt``, which similarly to trace file from the :ref:`packet trace helper example <packet trace helper example>` can be analyzed manually or used as input to some graph/stats packages.


Other types of stats
--------------------

.. _periodic tracing of Pending Interest Table (PIT) size:

Periodic tracing of Pending Interest Table (PIT) size
+++++++++++++++++++++++++++++++++++++++++++++++++++++

This example (``ndn-simple-with-pit-count-stats.cc``) shows how you can periodically print out current size of PIT on the selected nodes.

.. literalinclude:: ../../examples/ndn-simple-with-pit-count-stats.cc
    :language: c++
    :linenos:
    :lines: 20-26,47-
    :emphasize-lines: 9-20,43-46,59-61

To run this scenario, use the following command::

        ./waf --run=ndn-simple-with-pit-count-stats
