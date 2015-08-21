Examples
========

.. role:: red

.. note::
   :red:`!!! This page only shows up examples of how to config topology and perform basic
   operations in ndnSIM (an example equivalent to "Hello, world1") !!!  These are **NOT**
   examples of real experimentations (just like "Hello, world!" is not a real program).`

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

.. _simple-scenario:

Simple scenario
---------------

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 90

          +----------+                +--------+                +----------+
          |          |     1Mbps      |        |      1Mbps     |          |
          | Consumer |<-------------->| Router |<-------------->| Producer |
          |          |         10ms   |        |         10ms   |          |
          +----------+                +--------+                +----------+

The first example (``ndn-simple.cpp``) shows very basics of ndnSIM.  In the simulated
topology there are 3 nodes, connected with point-to-point links, one
NDN consumer, and one NDN producer:

Consumer is simulated using :ndnsim:`ConsumerCbr` reference application and generates Interests
towards the producer with frequency of 10 Interests per second (see :doc:`applications`).

Producer is simulated using :ndnsim:`Producer` class, which is used to satisfy all incoming
Interests with virtual payload data (1024 bytes).

FIB on every node is populated using default routes (see :doc:`helpers`) and the content store
structure of the original ndnSIM is used.

The following code represents all that is necessary to run such a
simple scenario

.. literalinclude:: ../../examples/ndn-simple.cpp
   :language: c++
   :linenos:
   :lines: 20-28,49-
   :emphasize-lines: 23-24,27-29,42-53

If this code is placed into ``scratch/ndn-simple.cpp`` and NS-3 is compiled in debug mode, you
can run and see progress of the simulation using the following command (in optimized mode
nothing will be printed out)::

     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

.. _9-node-grid-example:

9-node grid example
-------------------

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 120

        /--------\      /-\         /-\
        |Consumer|<---->| |<------->| |
        \--------/      \-/         \-/
            ^            ^           ^
            |            |           |   1Mbps/10ms delay
            v            v           v
           /-\          /-\         /-\
           | |<-------->| |<------->| |
           \-/          \-/         \-/
            ^            ^           ^
            |            |           |
            v            v           v
           /-\          /-\      /--------\
           | |<-------->| |<---->|Producer|
           \-/          \-/      \--------/

This scenario (``ndn-grid.cpp``) simulates a grid topology, which is constructed using
PointToPointLayout NS-3 module.

FIB is populated using :ndnsim:`GlobalRoutingHelper` (see :doc:`helpers`). The content store
structure of NFD is used in all the nodes.

Consumer is simulated using :ndnsim:`ConsumerCbr` reference application and generates Interests
towards the producer with frequency of 100 interests per second (see :doc:`applications`).

Producer is simulated using :ndnsim:`Producer` class, which is used to satisfy all incoming
Interests with virtual payload data (1024 bytes).

The following code represents all that is necessary to run such a simple scenario

.. literalinclude:: ../../examples/ndn-grid.cpp
   :language: c++
   :linenos:
   :lines: 20-29,54-
   :emphasize-lines: 24-26,33,36-37,40-42,58,61

If this code is placed into ``scratch/ndn-grid.cpp`` and NS-3 is compiled in debug mode, you
can run and see progress of the simulation using the following command (in optimized mode
nothing will be printed out)::

    NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-grid

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

.. _9-node-grid-example-using-topology-plugin:

9-node grid example using topology plugin
-----------------------------------------

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 120

        /--------\      /-\         /-\
        |Consumer|<---->| |<------->| |
        \--------/      \-/         \-/
            ^            ^           ^
            |            |           |   1Mbps/10ms delay
            v            v           v
           /-\          /-\         /-\
           | |<-------->| |<------->| |
           \-/          \-/         \-/
            ^            ^           ^
            |            |           |
            v            v           v
           /-\          /-\      /--------\
           | |<-------->| |<---->|Producer|
           \-/          \-/      \--------/

Instead of defining topology directly as in :ref:`simple-scenario` or using specialized helpers
as in :ref:`9-node-grid-example`, ndnSIM provides experimental extended versions of
TopologyReader classes: :ndnsim:`AnnotatedTopologyReader` and
:ndnsim:`RocketfuelWeightsReader`.

While :ndnsim:`RocketfuelWeightsReader` is a specialized version intended to be used with
`Rocketfuel <http://www.cs.washington.edu/research/networking/rocketfuel/>`_ topology and link
weights files (examples will be provided later), :ndnsim:`AnnotatedTopologyReader` is a more
general-use class that uses simple user-readable format.

:ndnsim:`AnnotatedTopologyReader` expects the following format:

.. literalinclude:: ../../examples/topologies/topo-grid-3x3.txt
   :language: bash
   :linenos:
   :lines: 1-2,19-
   :emphasize-lines: 8,24


This scenario (``ndn-grid-topo-plugin.cpp``) duplicates the functionality of
:ref:`9-node-grid-example` but with the use of :ndnsim:`AnnotatedTopologyReader`.

.. literalinclude:: ../../examples/ndn-grid-topo-plugin.cpp
   :language: c++
   :linenos:
   :lines: 20-27,52-
   :emphasize-lines: 15-17,31-33

As you can see, scenario code became more compact and more readable.

:ndnsim:`AnnotatedTopologyReader` provides two ways to access topology nodes.  First, you can
use the method :ndnsim:`AnnotatedTopologyReader::GetNodes` which returns NodeContainer.

Alternatively, nodes can be accessed by name using `Names::Find<Node> ("nodename")` call, as in
the above example.  For this purpose,:ndnsim:`AnnotatedTopologyReader` automatically registers
all created nodes with names specified in topology file.  For more information about `Names`
class, please refer to `NS-3 documentation <http://www.nsnam.org/doxygen/classns3_1_1_names.html>`_.

If the topology file is placed into ``src/ndnSIM/examples/topologies/topo-grid-3x3.txt`` and
the code is placed into ``scratch/ndn-grid-topo-plugin.cpp``, you can run and see progress of
the simulation using the following command (in optimized mode nothing will be printed out)::

    NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-grid-topo-plugin

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

6-node bottleneck topology
--------------------------

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 90

         /------\                                                    /------\
         | Src1 |<--+                                            +-->| Dst1 |
         \------/    \                                          /    \------/
                      \                                        /
                       +-->/------\   "bottleneck"  /------\<-+
                           | Rtr1 |<===============>| Rtr2 |
                       +-->\------/                 \------/<-+
                      /                                        \
         /------\    /                                          \    /------\
         | Src2 |<--+                                            +-->| Dst2 |
         \------/                                                    \------/


This scenario (``ndn-congestion-topo-plugin.cpp``) can be used for congestion-related scenarios

.. literalinclude:: ../../examples/topologies/topo-6-node.txt
    :language: bash
    :linenos:
    :lines: 1-2,15-

.. literalinclude:: ../../examples/ndn-congestion-topo-plugin.cpp
   :language: c++
   :linenos:
   :lines: 20-27,48-

To run this scenario and see what is happening, use the following command::

        NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-congestion-topo-plugin

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

.. _11-node 2-bottleneck topology:

11-node 2-bottleneck topology
------------------------------

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 90

         /------\ 0                                                 0 /------\
         |  c1  |<-----+                                       +----->|  p1  |
         \------/       \                                     /       \------/
                         \              /-----\              /
         /------\ 0       \         +==>| r12 |<==+         /       0 /------\
         |  c2  |<--+      \       /    \-----/    \       /      +-->|  p2  |
         \------/    \      \     |                 |     /      /    \------/
                      \      |    |   1Mbps links   |    |      /
                       \  1  v0   v5               1v   2v  3  /
                        +->/------\                 /------\<-+
                          2|  r1  |<===============>|  r2  |4
                        +->\------/4               0\------/<-+
                       /    3^                           ^5    \
                      /      |                           |      \
         /------\ 0  /      /                             \      \  0 /------\
         |  c3  |<--+      /                               \      +-->|  p3  |
         \------/         /                                 \         \------/
                         /     "All consumer-router and"     \
         /------\ 0     /      "router-producer links are"    \    0 /------\
         |  c4  |<-----+       "10Mbps"                        +---->|  p4  |
         \------/                                                    \------/

         "Numbers near nodes denote face IDs. Face ID is assigned based on the order of link"
         "definitions in the topology file"

Firstly, we define a meaningful topology:

The corresponding topology file (``topo-11-node-two-bottlenecks.txt``):

.. literalinclude:: ../../examples/topologies/topo-11-node-two-bottlenecks.txt
    :language: bash
    :linenos:
    :lines: 1-2,28-

After that, we define the simulation scenario:

Example simulation (``ndn-congestion-alt-topo-plugin.cpp``) scenario:

.. literalinclude:: ../../examples/ndn-congestion-alt-topo-plugin.cpp
    :language: c++
    :linenos:
    :lines: 20-27,60-

To run this scenario and see what is happening, use the following command::

        NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-congestion-alt-topo-plugin

You can also run using visualizer module to verify that both bottleneck links are utilized::

        ./waf --run=ndn-congestion-alt-topo-plugin --visualize

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

.. _6-node topology with custom NFD forwarding strategy:

6-node topology with custom NFD forwarding strategy
---------------------------------------------------

.. sidebar:: Topology

    .. aafig::
        :aspect: 60
        :scale: 90

                                /-----\
                                | CSU |
                       +----->  | HUB |  <----+
                       |        \-----/       |
                       |                      |  1Mbps/10ms delay
                       v                      v
                     /------\               /----------\
                     | UCLA |               | Consumer |
                     | HUB  |               |   CSU-1  |
             +-----> \------/ <-----+       \----------/
             |                      |
             |                      |
             v                      v
         /----------\           /----------\
         | Producer |           | Producer |
         |  UCLA-1  |           |  UCLA-2  |
         \----------/           \----------/

        "All links are 1Mbps with propagation 10ms delay."
        "FIB is populated using NdnGlobalRoutingHelper."

This scenario simulates a load balancer topology (using topology reader module).  The
corresponding topology file (``topo-load-balancer.txt``):

.. literalinclude:: ../../examples/topologies/topo-load-balancer.txt
    :language: bash
    :linenos:
    :lines: 30-36,43-

After that, we define the simulation scenario:

Example simulation (``ndn-load-balancer.cpp``) scenario:

.. literalinclude:: ../../examples/ndn-load-balancer.cpp
    :language: c++
    :linenos:
    :lines: 20-27,55-
    :emphasize-lines: 7,44-45

In this simulation scenario, the node called "UCLA-HUB" implements a random load balancing
strategy for the name prefix "/ucla/hello". In this way, the Interest packets will be
forwarded randomly either to the producer node called "UCLA-1" or the producer node called
"UCLA-2".

To run this scenario and see what is happening, use the following command::

        NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-load-balancer

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

9-node grid topology with different forwarding strategies per name prefix
-------------------------------------------------------------------------

This scenario simulates a grid topology (using PointToPointGrid module). In this scenario,
thanks to NFD, we can choose a different forwarding strategy for each prefix in each node.

Consumer requests data from producer with frequency 100 interests per second (interests
contain constantly increasing sequence number).

For every received interest, producer replies with a data packet, containing 1024 bytes of
virtual payload.

In this scenario, we choose the multicast strategy to be installed for the name prefix
"/prefix1" in all the nodes, while for the name prefix "/prefix2", the best-route strategy
will be installed in all the topology nodes.

Example simulation (``ndn-different-strategy-per-prefix.cpp``) scenario:

.. literalinclude:: ../../examples/ndn-different-strategy-per-prefix.cpp
    :language: c++
    :linenos:
    :lines: 20-33,61-
    :emphasize-lines: 51-52

To run this scenario and see what is happening, use the following command::

        NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-different-strategy-per-prefix

9-node grid topology with different forwarding strategy for each node
---------------------------------------------------------------------

This scenario simulates a grid topology (using PointToPointGrid module). The first six nodes
use the best route forwarding strategy, whereas the three remaining nodes use the multicast
forwarding strategy.

Consumer requests data from producer with frequency 100 interests per second (interests
contain constantly increasing sequence number).

For every received interest, producer replies with a data packet, containing 1024 bytes of
virtual payload.

Example simulation (``ndn-grid-multiple-strategies.cpp``) scenario:

.. literalinclude:: ../../examples/ndn-grid-multiple-strategies.cpp
    :language: c++
    :linenos:
    :lines: 20-33,61-
    :emphasize-lines: 49-58

To run this scenario and see what is happening, use the following command::

        NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-grid-multiple-strategies

Simple parallel scenario using MPI
----------------------------------

:ref:`simple scenario with MPI support`

3-level binary tree with packet-level trace helpers
---------------------------------------------------

:ref:`packet trace helper example`


3-level binary tree with content store trace helper
---------------------------------------------------

:ref:`cs trace helper example`

3-level binary tree with  application-level Interest-Data delay tracer
----------------------------------------------------------------------

:ref:`app delay trace helper example`

1-node topology with custom application
---------------------------------------

:ref:`Custom applications`

Simple scenario with pcap dump
------------------------------

The following example (``ndn-simple-with-pcap.cpp``) demonstrates how to dump all simulated
traffic in pcap-formatted data, which can be used for later analysis by conventional tools,
like tcpdump and wireshark.

.. literalinclude:: ../../examples/ndn-simple-with-pcap.cpp
   :language: c++
   :linenos:
   :lines: 20-
   :emphasize-lines: 24-43,89-91

If this code is placed into ``scratch/ndn-simple-with-pcap.cpp`` and NS-3 is compiled in debug
mode, you can run and see progress of the simulation using the following command (in optimized
mode nothing will be printed out)::

     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple-with-pcap

This will generate ``ndn-simple-trace.pcap``, which can be fed to tcpdump::

     tcpdump -r ndn-simple-trace.pcap

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.

.. _Simple scenario with link failures:

Simple scenario with link failures
----------------------------------

The following example (``ndn-simple-with-link-failure.cpp``) shows how to "fail" links in
ndnSIM simulation.  The basic idea is to set ndn::Faces that correspond to the failed link to
DOWN state.  ndnSIM now includes a simple helper that simplifies this process.

.. literalinclude:: ../../examples/ndn-simple-with-link-failure.cpp
   :language: c++
   :linenos:
   :lines: 20-31,52-
   :emphasize-lines: 56-57

If this code is placed into ``scratch/ndn-simple-with-link-failure.cpp`` and NS-3 is compiled
in debug mode, you can run and see progress of the simulation using the following command (in
optimized mode nothing will be printed out)::

     NS_LOG=ndn.Consumer:ndn.Producer:ndn.LinkControlHelper ./waf --run=ndn-simple-with-link-failure

.. note::
   If you compiled ndnSIM with examples (``./waf configure --enable-examples``) you can
   directly run the example without putting scenario into ``scratch/`` folder.


25-node tree topology with L2Tracer
-----------------------------------

:ref:`Example of packet drop tracer (L2Tracer)`
