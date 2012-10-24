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

.. code-block:: c++

    #include "ns3/core-module.h"
    #include "ns3/network-module.h"
    #include "ns3/point-to-point-module.h"
    #include "ns3/ndnSIM-module.h"

    using namespace ns3;

    int
    main (int argc, char *argv[])
    {
      // setting default parameters for PointToPoint links and channels
      Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
      Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
      Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));

      // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
      CommandLine cmd;
      cmd.Parse (argc, argv);

      // Creating nodes
      NodeContainer nodes;
      nodes.Create (3);

      // Connecting nodes using two links
      PointToPointHelper p2p;
      p2p.Install (nodes.Get (0), nodes.Get (1));
      p2p.Install (nodes.Get (1), nodes.Get (2));

      // Install CCNx stack on all nodes
      ndn::StackHelper ccnxHelper;
      ccnxHelper.SetDefaultRoutes (true);
      ccnxHelper.InstallAll ();

      // Installing applications

      // Consumer
      ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
      // Consumer will request /prefix/0, /prefix/1, ...
      consumerHelper.SetPrefix ("/prefix");
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
      consumerHelper.Install (nodes.Get (0)); // first node

      // Producer
      ndn::AppHelper producerHelper ("ns3::ndn::Producer");
      // Producer will reply to all requests starting with /prefix
      producerHelper.SetPrefix ("/prefix");
      producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
      producerHelper.Install (nodes.Get (2)); // last node

      Simulator::Stop (Seconds (20.0));

      Simulator::Run ();
      Simulator::Destroy ();

      return 0;
    }

If this code is placed into ``scratch/ndn-simple.cc`` and NS-3 is compiled in debug mode, you can run and see progress of the
simulation using the following command (in optimized mode nothing will be printed out)::

     NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-simple


.. _9-node-grid-example:
 
9-node grid example
-------------------

This scenario (``ndn-grid.cc``) simulates using a grid topology build with PointToPointGrid NS-3 module

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
with frequency of 10 Interests per second (see :doc:`applications`).

Producer is simulated using :ndnsim:`Producer` class, which is used to satisfy all incoming Interests with virtual payload data (1024 bytes).


The following code represents all that is necessary to run such a
simple scenario

.. code-block:: c++

    #include "ns3/core-module.h"
    #include "ns3/network-module.h"
    #include "ns3/point-to-point-module.h"
    #include "ns3/point-to-point-grid.h"
    #include "ns3/ndnSIM-module.h"
    
    using namespace ns3;
    
    int 
    main (int argc, char *argv[])
    {
      // Setting default parameters for PointToPoint links and channels
      Config::SetDefault ("ns3::PointToPointNetDevice::DataRate", StringValue ("1Mbps"));
      Config::SetDefault ("ns3::PointToPointChannel::Delay", StringValue ("10ms"));
      Config::SetDefault ("ns3::DropTailQueue::MaxPackets", StringValue ("20"));
        
      // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
      CommandLine cmd;
      cmd.Parse (argc, argv);

      // Creating 3x3 topology
      PointToPointHelper p2p;
      PointToPointGridHelper grid (3, 3, p2p);
      grid.BoundingBox(100,100,200,200);
    
      // Install CCNx stack on all nodes
      ndn::StackHelper ccnxHelper;
      ccnxHelper.InstallAll ();
    
      // Installing global routing interface on all nodes
      ndn::GlobalRoutingHelper ccnxGlobalRoutingHelper;
      ccnxGlobalRoutingHelper.InstallAll ();
      
      // Getting containers for the consumer/producer
      Ptr<Node> producer = grid.GetNode (nGrid-1, nGrid-1);
      NodeContainer consumerNodes;
      consumerNodes.Add (grid.GetNode (0,0));
      
      // Install CCNx applications
      std::string prefix = "/prefix";
      
      ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
      consumerHelper.SetPrefix (prefix);
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
      consumerHelper.Install (consumerNodes);
      
      ndn::AppHelper producerHelper ("ns3::ndn::Producer");
      producerHelper.SetPrefix (prefix);
      producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
      producerHelper.Install (producer);
    
      // Add /prefix origins to ndn::GlobalRouter
      ccnxGlobalRoutingHelper.AddOrigins (prefix, producer);
    
      // Calculate and install FIBs
      ccnxGlobalRoutingHelper.CalculateRoutes ();
      
      Simulator::Stop (Seconds (20.0));
        
      Simulator::Run ();
      Simulator::Destroy ();
        
      return 0;
    }
    

If this code is placed into ``scratch/ndn-grid.cc`` and NS-3 is compiled in debug mode, you can run and see progress of the
simulation using the following command (in optimized mode nothing will be printed out)::

    NS_LOG=ndn.Consumer:ndn.Producer ./waf --run=ndn-grid

.. _9-node-grid-example-using-topology-plugin:

9-node grid example using topology plugin
-----------------------------------------

Instead of defining topology directly as in :ref:`simple-scenario` or using specialized helpers as in :ref:`9-node-grid-example`, ndnSIM provides experimental extended versions of TopologyReader classes: :ndnsim:`AnnotatedTopologyReader` and :ndnsim:`RocketfuelWeightsReader`.

While :ndnsim:`RocketfuelWeightsReader` is a specialized version intended to be used with `Rocketfuel <http://www.cs.washington.edu/research/networking/rocketfuel/>`_ topology and link weights files (examples will be provided later), :ndnsim:`AnnotatedTopologyReader` is a general-use tool that allows creation of any custom topologies.  
The based format for the input file the :ndnsim:`AnnotatedTopologyReader` expects::

    # any empty lines and lines starting with '#' symbol is ignored

    # The file should contain exactly two sections: router and link, each starting with the corresponding keyword

    # router section defines topology nodes and their relative positions (e.g., to use in visualizer)
    router

    # each line in this section represents one router and should have the following data
    # node  comment     yPos    xPos
    Node0   NA          3       1
    Node1   NA          3       2
    Node2   NA          3       3
    Node3   NA          2       1
    Node4   NA          2       2
    Node5   NA          2       3
    Node6   NA          1       1
    Node7   NA          1       2
    Node8   NA          1       3
    # Note that `node` can be any string. It is possible to access to the node by name using Names::Find, see examples.

    # link section defines point-to-point links between nodes and characteristics of these links
    link

    # Each line should be in the following format (only first two are required, the rest can be omitted)
    # srcNode   dstNode     bandwidth   metric  delay   queue
    # bandwidth: link bandwidth
    # metric: routing metric
    # delay:  link delay
    # queue:  MaxPackets for transmission queue on the link (both directions)
    Node0       Node1       1Mbps       1       10ms    10
    Node0       Node3       1Mbps       1       10ms    10
    Node1       Node2       1Mbps       1       10ms    10
    Node1       Node4       1Mbps       1       10ms    10
    Node2       Node5       1Mbps       1       10ms    10
    Node3       Node4       1Mbps       1       10ms    10
    Node3       Node6       1Mbps       1       10ms    10
    Node4       Node5       1Mbps       1       10ms    10
    Node4       Node7       1Mbps       1       10ms    10
    Node5       Node8       1Mbps       1       10ms    10
    Node6       Node7       1Mbps       1       10ms    10
    Node7       Node8       1Mbps       1       10ms    10


If you save the topology file to `topo.txt` in the current directory, then the following code will duplicate the functionality of :ref:`9-node-grid-example` but with the use of :ndnsim:`AnnotatedTopologyReader`:

.. code-block:: c++

    #include "ns3/core-module.h"
    #include "ns3/network-module.h"
    #include "ns3/ndnSIM-module.h"
    
    using namespace ns3;
    
    int 
    main (int argc, char *argv[])
    {
      CommandLine cmd;
      cmd.Parse (argc, argv);
    
      AnnotatedTopologyReader topologyReader ("", 25);
      topologyReader.SetFileName ("topo.txt");
      topologyReader.Read (); 
        
      // Install CCNx stack on all nodes
      ndn::StackHelper ccnxHelper;
      ccnxHelper.InstallAll ();
        
      // Installing global routing interface on all nodes
      ndn::GlobalRoutingHelper ccnxGlobalRoutingHelper;
      ccnxGlobalRoutingHelper.InstallAll ();
          
      // Getting containers for the consumer/producer
      Ptr<Node> producer = Names::Find<Node> ("Node8");
      NodeContainer consumerNodes;
      consumerNodes.Add (Names::Find<Node> ("Node0"));
          
      // Install CCNx applications
      std::string prefix = "/prefix";
          
      ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");
      consumerHelper.SetPrefix (prefix);
      consumerHelper.SetAttribute ("Frequency", StringValue ("100")); // 100 interests a second
      consumerHelper.Install (consumerNodes);
          
      ndn::AppHelper producerHelper ("ns3::ndn::Producer");
      producerHelper.SetPrefix (prefix);
      producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
      producerHelper.Install (producer);
        
      // Add /prefix origins to ndn::GlobalRouter
      ccnxGlobalRoutingHelper.AddOrigins (prefix, producer);
        
      // Calculate and install FIBs
      ccnxGlobalRoutingHelper.CalculateRoutes ();
          
      Simulator::Stop (Seconds (20.0));
            
      Simulator::Run ();
      Simulator::Destroy ();
            
      return 0;
    }
    
As you can see, scenario code became more compact and more readable.

:ndnsim:`AnnotatedTopologyReader` provides two ways to access topology nodes.  
First, you can use the method :ndnsim:`AnnotatedTopologyReader::GetNodes` which returns NodeContainer.

Alternatively, nodes can be accessed by name using `Names::Find<Node> ("nodename")` call, as in the above example.
For this purpose,:ndnsim:`AnnotatedTopologyReader` automatically registers all created nodes with names specified in topology file.
For more information about `Names` class, please refer to `NS-3 documentation <.. http://www.nsnam.org/doxygen/classns3_1_1_names.html>`_
.
