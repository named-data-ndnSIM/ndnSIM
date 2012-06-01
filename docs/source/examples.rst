Examples
========

Simple scenario
---------------

The first example (``ccnx-simple.cc``) shows very basics of ndnSIM.  In the simulated
topology there are 3 nodes, connected with point-to-point links, one
NDN consumer, and one NDN producer::

      +----------+     1Mbps      +--------+     1Mbps      +----------+
      | consumer | <------------> | router | <------------> | producer |
      +----------+         10ms   +--------+          10ms  +----------+

Consumer requests data from producer with frequency 10 interests per second
(interests contain constantly increasing sequence number).

For every received interest, producer replies with a data packet, containing
1024 bytes of virtual payload.

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
      CcnxStackHelper ccnxHelper;
      ccnxHelper.SetDefaultRoutes (true);
      ccnxHelper.InstallAll ();

      // Installing applications    

      // Consumer
      CcnxAppHelper consumerHelper ("ns3::CcnxConsumerCbr");
      // Consumer will request /prefix/0, /prefix/1, ...
      consumerHelper.SetPrefix ("/prefix");
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second
      consumerHelper.Install (nodes.Get (0)); // first node
      
      // Producer
      CcnxAppHelper producerHelper ("ns3::CcnxProducer");
      // Producer will reply to all requests starting with /prefix
      producerHelper.SetPrefix ("/prefix");
      producerHelper.SetAttribute ("PayloadSize", StringValue("1024"));
      producerHelper.Install (nodes.Get (2)); // last node
      
      Simulator::Stop (Seconds (20.0));
        
      Simulator::Run ();
      Simulator::Destroy ();
        
      return 0;
    }
    
If NS-3 is compiled in debug mode, you can run and see progress of the
simulation using the following command::

     NS_LOG=CcnxConsumer:CcnxProducer ./waf --run=ccnx-simple
