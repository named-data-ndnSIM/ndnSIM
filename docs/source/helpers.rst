ndnSIM helpers
==============

Helpers are very important components of ndnSIM, especially for writing simulation scenarios.
The following summarizes helpers and their basic usage.

StackHelper
---------------

:ndnsim:`StackHelper` is used to install ndnSIM network stack on requested nodes, as well to provide a simple way configure several important parameters of NDN simulation.

Example::

   ndn::StackHelper ndnHelper;
   NodeContainer nodes;
   ...
   ndnHelper.Install (nodes);

Forwarding strategy
+++++++++++++++++++

A desired :ndnsim:`forwarding strategy <ForwardingStrategy>` parameter need to be set before installing stack on a node.

  Currently, there are 2 implemented forwarding strategies that can be used in simulations:

  - :ndnsim:`Flooding` (default)

      Interests will be forwarded to all available faces available for a route (FIB entry).
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding");
	 ...
	 ndnHelper.Install (nodes);
	 
      

  - :ndnsim:`SmartFlooding`

      If GREEN face is available, Interest will be sent to the highest-ranked GREEN face. 
      If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding");
	 ...
	 ndnHelper.Install (nodes);

  - :ndnsim:`BestRoute`

      If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
      If not, Interest will be forwarded to the highest-ranked YELLOW face.
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
	 ...
	 ndnHelper.Install (nodes);

Default routes
++++++++++++++

.. note::
   Disabled by default

In simple topologies, like in :doc:`examples <examples>`, or when
simulating broadcast environment, it is possible to set up *default*
FIB entries using :ndnsim:`StackHelper::SetDefaultRoutes` call.
More specifically, every installed NDN stack will have a FIB entry to ``/`` prefix, containing all available faces.

The following should be done before installing stack on a node:

  .. code-block:: c++

     ndnHelper.SetDefaultRoutes (true);
     ...
     ndnHelper.Install (nodes);


Manually routes
+++++++++++++++

Routes can be configured manually using :ndnsim:`StackHelper::AddRoute` static methods of :ndnsim:`StackHelper`.

These routes **should** be created **after** installing NDN stack on a node:

  .. code-block:: c++

     ndnHelper.Install (nodes);
     ...
     Ptr<Node> node = ...     // FIB entry will be added to FIB on this node
     std::string prefix = ... // some prefix
     Ptr<ndn::Face> face = ... // NDN face that belongs to the node and through which prefix is accessible
     int32_t metric = ...     // some routing metric
     ndn::StackHelper::AddRoute (node, prefix, face, metric);


.. Enable optional interest limiting
.. +++++++++++++++++++++++++++++++++

.. EnableLimits

GlobalRoutingHelper
-----------------------

To simplify FIB management in large topologies, ndnSIM contains a global routing controller (:ndnsim:`helper <GlobalRoutingHelper>` and :ndnsim:`special interface <GlobalRouter>`), similar in spirit to ``Ipv4GlobalRoutingHelper``.

There are several necessary steps, in order to take advantage of the global routing controller:

* install :ndnsim:`special interfaces <GlobalRouter>` on nodes

   .. code-block:: c++
   
     NodeContainer nodes;
     ...
     ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
     ndnGlobalRoutingHelper.Install (nodes);
   
* specify which node exports which prefix using :ndnsim:`GlobalRoutingHelper::AddOrigins`

   .. code-block:: c++
   
     Ptr<Node> producer; // producer node that exports prefix
     std::string prefix; // exported prefix
     ...
     ndnGlobalRoutingHelper.AddOrigins (prefix, producer);
   
* calculate and install FIBs on every node using :ndnsim:`GlobalRoutingHelper::CalculateRoutes`

   .. code-block:: c++
   
     cdnGlobalRoutingHelper.CalculateRoutes ();
   

AppHelper
---------------

:ndnsim:`AppHelper` simplifies task of creating, configuring, and installing ndnSIM applications.


The basic usage of the :ndnsim:`AppHelper`:

* Create helper for specific applications class:

   .. code-block:: c++

      // Create helper for the consumer generating Interests with constant rate
      ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerCbr");

* Assign prefix on which application operates (either generating Interests using this name or satisfying Interests for this name) using :ndnsim:`AppHelper::SetPrefix`:

   .. code-block:: c++

      consumerHelper.SetPrefix (prefix);

* Assign application-specific attributes using :ndnsim:`AppHelper::SetAttribute`:

   .. code-block:: c++

      // Set frequency parameter
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second

* Install application on one or more nodes:

   .. code-block:: c++

      NodeContainer nodes;
      ...
      consumerHelper.Install (nodes)
