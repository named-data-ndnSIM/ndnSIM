Examples
========

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

