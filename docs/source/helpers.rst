ndnSIM helpers
==============

Helpers are very important components of ndnSIM, especially for writing simulation scenarios.
The following summarizes helpers and their basic usage.

NdnStackHelper
---------------

:ndnsim:`NdnStackHelper` is used to install ndnSIM network stack on requested nodes, as well to provide a simple way configure several important parameters of NDN simulation.

Example::

   NdnStackHelper ndnHelper;
   NodeContainer nodes;
   ...
   ndnHelper.Install (nodes);

Forwarding strategy
+++++++++++++++++++

Forwarding strategy parameter **must** be set before installing stack on a node.

  Currently, there are 2 implemented forwarding strategies that can be used in simulations:

  - :ndnsim:`Flooding` (default)

      Interests will be forwarded to all available faces available for a route (FIB entry).
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndnSIM::Flooding");
	 ...
	 ndnHelper.Install (nodes);
	 
      

  - :ndnsim:`SmartFlooding`

      If GREEN face is available, Interest will be sent to the highest-ranked GREEN face. 
      If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndnSIM::SmartFlooding");
	 ...
	 ndnHelper.Install (nodes);

  - :ndnsim:`BestRoute`

      If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
      If not, Interest will be forwarded to the highest-ranked YELLOW face.
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndnSIM::BestRoute");
	 ...
	 ndnHelper.Install (nodes);

Default routes
++++++++++++++

.. note::
   Disabled by default

In simple topologies, like in :doc:`examples <examples>`, or when
simulating broadcast environment, it is possible to set up *default*
FIB entries using :ndnsim:`NdnStackHelper::SetDefaultRoutes` call.
More specifically, every installed NDN stack will have a FIB entry to ``/`` prefix, containing all available faces.

The following should be done before installing stack on a node:

  .. code-block:: c++

     ndnHelper.SetDefaultRoutes (true);
     ...
     ndnHelper.Install (nodes);


Manually routes
+++++++++++++++

Routes can be configured manually using :ndnsim:`NdnStackHelper::AddRoute` static methods of :ndnsim:`NdnStackHelper`.

These routes **should** be created **after** installing NDN stack on a node:

  .. code-block:: c++

     ndnHelper.Install (nodes);
     ...
     Ptr<Node> node = ...     // FIB entry will be added to FIB on this node
     std::string prefix = ... // some prefix
     Ptr<NdnFace> face = ... // NDN face that belongs to the node and through which prefix is accessible
     int32_t metric = ...     // some routing metric
     NdnStackHelper::AddRoute (node, prefix, face, metric);


.. Enable optional interest limiting
.. +++++++++++++++++++++++++++++++++

.. EnableLimits

NdnGlobalRoutingHelper
-----------------------

To simplify FIB management in large topologies, ndnSIM contains a global routing controller (:ndnsim:`helper <NdnGlobalRoutingHelper>` and :ndnsim:`special interface <NdnGlobalRouter>`), similar in spirit to ``Ipv4GlobalRoutingHelper``.

There are several necessary steps, in order to take advantage of the global routing controller:

* install :ndnsim:`special interfaces <NdnGlobalRouter>` on nodes

   .. code-block:: c++
   
     NodeContainer nodes;
     ...
     NdnGlobalRoutingHelper ndnGlobalRoutingHelper;
     ndnGlobalRoutingHelper.Install (nodes);
   
* specify which node exports which prefix using :ndnsim:`NdnGlobalRoutingHelper::AddOrigins`

   .. code-block:: c++
   
     Ptr<Node> producer; // producer node that exports prefix
     std::string prefix; // exported prefix
     ...
     ndnGlobalRoutingHelper.AddOrigins (prefix, producer);
   
* calculate and install FIBs on every node using :ndnsim:`NdnGlobalRoutingHelper::CalculateRoutes`

   .. code-block:: c++
   
     cdnGlobalRoutingHelper.CalculateRoutes ();
   

NdnAppHelper
---------------

:ndnsim:`NdnAppHelper` simplifies task of creating, configuring, and installing ndnSIM applications.


The basic usage of the :ndnsim:`NdnAppHelper`:

* Create helper for specific applications class:

   .. code-block:: c++

      // Create helper for the consumer generating Interests with constant rate
      NdnAppHelper consumerHelper ("ns3::NdnConsumerCbr");

* Assign prefix on which application operates (either generating Interests using this name or satisfying Interests for this name) using :ndnsim:`NdnAppHelper::SetPrefix`:

   .. code-block:: c++

      consumerHelper.SetPrefix (prefix);

* Assign application-specific attributes using :ndnsim:`NdnAppHelper::SetAttribute`:

   .. code-block:: c++

      // Set frequency parameter
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second

* Install application on one or more nodes:

   .. code-block:: c++

      NodeContainer nodes;
      ...
      consumerHelper.Install (nodes)
