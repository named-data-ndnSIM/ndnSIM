Examples
========

.. _simple-scenario:

Simple scenario
---------------

The first example (``ndn-simple.cc``) shows very basics of ndnSIM.  In the simulated
topology there are 3 nodes, connected with point-to-point links, one
NDN consumer, and one NDN producer:

.. aafig::
    :aspect: 60
    :scale: 120

      +----------+                +--------+                +----------+
      |          |     1Mbps      |        |      1Mbps     |          |
      | Consumer |<-------------->| Router |<-------------->| Producer |
      |          |         10ms   |        |         10ms   |          |
      +----------+                +--------+                +----------+

Consumer is simulated using :ndnsim:`ConsumerCbr` reference application and generates Interests towards the producer
with frequency of 10 Interests per second (see :doc:`applications`).

Producer is simulated using :ndnsim:`Producer` class, which is used to satisfy all incoming Interests with virtual payload data (1024 bytes).

FIB on every node is populated using default routes (see :doc:`helpers`).

The following code represents all that is necessary to run such a
simple scenario

.. literalinclude:: ../../examples/ndn-simple.cc
   :language: c++
   :linenos:
   :lines: 20-27,48-
   :emphasize-lines: 30-33,37-49

If this code is placed into ``scratch/ndn-simple.cc`` and NS-3 is compiled in debug mode, you can run and see progress of the
simulation using the following command (in optimized mode nothing will be printed out)::

     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple


.. _9-node-grid-example:
 
9-node grid example
-------------------

This scenario (``ndn-grid.cc``) simulates a grid topology, which is constructed using PointToPointLayout NS-3 module

.. aafig::
    :aspect: 60
    :scale: 120

    /--------\	    /-\	        /-\
    |Consumer|<---->| |<------->| |
    \--------/	    \-/	        \-/
	^   	     ^ 	         ^
        |            |           |   1Mbps/10ms delay
        v            v           v
       /-\          /-\         /-\
       | |<-------->| |<------->| |
       \-/          \-/         \-/
	^   	     ^ 	         ^
        |            |           |
        v            v           v
       /-\	    /-\	     /--------\
       | |<-------->| |<---->|Producer|
       \-/          \-/      \--------/


FIB is populated using :ndnsim:`GlobalRoutingHelper` (see :doc:`helpers`).

Consumer is simulated using :ndnsim:`ConsumerCbr` reference application and generates Interests towards the producer
with frequency of 100 interests per second (see :doc:`applications`).

Producer is simulated using :ndnsim:`Producer` class, which is used to satisfy all incoming Interests with virtual payload data (1024 bytes).

The following code represents all that is necessary to run such a simple scenario

.. literalinclude:: ../../examples/ndn-grid.cc
   :language: c++
   :linenos:
   :lines: 20-27,53-
   :emphasize-lines: 28,31-33,35-38,53-57
    

If this code is placed into ``scratch/ndn-grid.cc`` and NS-3 is compiled in debug mode, you can run and see progress of the
simulation using the following command (in optimized mode nothing will be printed out)::

    NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-grid

.. _9-node-grid-example-using-topology-plugin:

9-node grid example using topology plugin
-----------------------------------------

Instead of defining topology directly as in :ref:`simple-scenario` or using specialized helpers as in :ref:`9-node-grid-example`, ndnSIM provides experimental extended versions of TopologyReader classes: :ndnsim:`AnnotatedTopologyReader` and :ndnsim:`RocketfuelWeightsReader`.

While :ndnsim:`RocketfuelWeightsReader` is a specialized version intended to be used with `Rocketfuel <http://www.cs.washington.edu/research/networking/rocketfuel/>`_ topology and link weights files (examples will be provided later), :ndnsim:`AnnotatedTopologyReader` is a more general-use class that uses simple user-readable format.

:ndnsim:`AnnotatedTopologyReader` expects the following format:

.. literalinclude:: ../../examples/topologies/topo-grid-3x3.txt
   :language: bash
   :linenos:
   :emphasize-lines: 8,24


This scenario (``ndn-grid-topo-plugin.cc``) duplicates the functionality of :ref:`9-node-grid-example` but with the use of :ndnsim:`AnnotatedTopologyReader`.

.. aafig::
    :aspect: 60
    :scale: 120

    /--------\	    /-\	        /-\
    |Consumer|<---->| |<------->| |
    \--------/	    \-/	        \-/
	^   	     ^ 	         ^
        |            |           |   1Mbps/10ms delay
        v            v           v
       /-\          /-\         /-\
       | |<-------->| |<------->| |
       \-/          \-/         \-/
	^   	     ^ 	         ^
        |            |           |
        v            v           v
       /-\	    /-\	     /--------\
       | |<-------->| |<---->|Producer|
       \-/          \-/      \--------/

.. literalinclude:: ../../examples/ndn-grid-topo-plugin.cc
   :language: c++
   :linenos:
   :lines: 20-26,51-
   :emphasize-lines: 14-16,20,27-30
    
As you can see, scenario code became more compact and more readable.

:ndnsim:`AnnotatedTopologyReader` provides two ways to access topology nodes.  
First, you can use the method :ndnsim:`AnnotatedTopologyReader::GetNodes` which returns NodeContainer.

Alternatively, nodes can be accessed by name using `Names::Find<Node> ("nodename")` call, as in the above example.
For this purpose,:ndnsim:`AnnotatedTopologyReader` automatically registers all created nodes with names specified in topology file.
For more information about `Names` class, please refer to `NS-3 documentation <.. http://www.nsnam.org/doxygen/classns3_1_1_names.html>`_
.

If the topology file is placed into ``src/ndnSIM/examples/topologies/topo-grid-3x3.txt`` and the code is placed into ``scratch/ndn-grid-topo-plugin.cc``, you can run and see progress of the simulation using the following command (in optimized mode nothing will be printed out)::

    NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-grid-topo-plugin


6-node bottleneck topology
--------------------------

This scenario (``ndn-congestion-topo-plugin.cc``) can be used for congestion-related scenarios 

.. aafig::
    :aspect: 60
    :scale: 120

    /------\	                                                /------\
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

.. literalinclude:: ../../examples/topologies/topo-6-node.txt
    :language: bash
    :linenos:
    :emphasize-lines: 3,13
   
.. literalinclude:: ../../examples/ndn-congestion-topo-plugin.cc
   :language: c++
   :linenos:
   :lines: 20-26,47-
   :emphasize-lines: 15,21-22,29-34,41-47,52-62

.. :lines: 20-25,53-
