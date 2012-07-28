ndnSIM helpers
==============

Helpers are very important components of ndnSIM, especially for writing simulation scenarios.
The following summarizes helpers and their basic usage.

CcnxStackHelper
---------------

:ndnsim:`CcnxStackHelper` is used to install ndnSIM network stack on requested nodes, as well to provide a simple way configure several important parameters of NDN simulation.

Example::

   CcnxStackHelper ccnxHelper;
   NodeContainer nodes;
   ...
   ccnxHelper.Install (nodes);

Forwarding strategy
+++++++++++++++++++

Forwarding strategy parameter **must** be set before installing stack on a node.

  Currently, there are 2 implemented forwarding strategies that can be used in simulations:

  - :ndnsim:`Flooding` (default)

      Interests will be forwarded to all available faces available for a route (FIB entry).
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ccnxHelper.SetForwardingStrategy ("ns3::ndnSIM::Flooding");
	 ...
	 ccnxHelper.Install (nodes);
	 
      

  - :ndnsim:`SmartFlooding`

      If GREEN face is available, Interest will be sent to the highest-ranked GREEN face. 
      If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ccnxHelper.SetForwardingStrategy ("ns3::ndnSIM::SmartFlooding");
	 ...
	 ccnxHelper.Install (nodes);

  - :ndnsim:`BestRoute`

      If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
      If not, Interest will be forwarded to the highest-ranked YELLOW face.
      If there are no available GREEN or YELLOW faces, interests is dropped.

      .. code-block:: c++

         ccnxHelper.SetForwardingStrategy ("ns3::ndnSIM::BestRoute");
	 ...
	 ccnxHelper.Install (nodes);

Default routes
++++++++++++++

.. note::
   Disabled by default

In simple topologies, like in :doc:`examples <examples>`, or when
simulating broadcast environment, it is possible to set up *default*
FIB entries using :ndnsim:`CcnxStackHelper::SetDefaultRoutes` call.
More specifically, every installed NDN stack will have a FIB entry to ``/`` prefix, containing all available faces.

The following should be done before installing stack on a node:

  .. code-block:: c++

     ccnxHelper.SetDefaultRoutes (true);
     ...
     ccnxHelper.Install (nodes);


Manually routes
+++++++++++++++

Routes can be configured manually using :ndnsim:`CcnxStackHelper::AddRoute` static methods of :ndnsim:`CcnxStackHelper`.

These routes **should** be created **after** installing NDN stack on a node:

  .. code-block:: c++

     ccnxHelper.Install (nodes);
     ...
     Ptr<Node> node = ...     // FIB entry will be added to FIB on this node
     std::string prefix = ... // some prefix
     Ptr<CcnxFace> face = ... // NDN face that belongs to the node and through which prefix is accessible
     int32_t metric = ...     // some routing metric
     CcnxStackHelper::AddRoute (node, prefix, face, metric);


.. Enable optional interest limiting
.. +++++++++++++++++++++++++++++++++

.. EnableLimits

CcnxGlobalRoutingHelper
-----------------------

To simplify FIB management in large topologies, ndnSIM contains a global routing controller (:ndnsim:`helper <CcnxGlobalRoutingHelper>` and :ndnsim:`special interface <CcnxGlobalRouter>`), similar in spirit to ``Ipv4GlobalRoutingHelper``.

There are several necessary steps, in order to take advantage of the global routing controller:

* install :ndnsim:`special interfaces <CcnxGlobalRouter>` on nodes

   .. code-block:: c++
   
     NodeContainer nodes;
     ...
     CcnxGlobalRoutingHelper ccnxGlobalRoutingHelper;
     ccnxGlobalRoutingHelper.Install (nodes);
   
* specify which node exports which prefix using :ndnsim:`CcnxGlobalRoutingHelper::AddOrigins`

   .. code-block:: c++
   
     Ptr<Node> producer; // producer node that exports prefix
     std::string prefix; // exported prefix
     ...
     ccnxGlobalRoutingHelper.AddOrigins (prefix, producer);
   
* calculate and install FIBs on every node using :ndnsim:`CcnxGlobalRoutingHelper::CalculateRoutes`

   .. code-block:: c++
   
     ccnxGlobalRoutingHelper.CalculateRoutes ();
   

CcnxAppHelper
---------------

:ndnsim:`CcnxAppHelper` simplifies task of creating, configuring, and installing ndnSIM applications.


The basic usage of the :ndnsim:`CcnxAppHelper`:

* Create helper for specific applications class:

   .. code-block:: c++

      // Create helper for the consumer generating Interests with constant rate
      CcnxAppHelper consumerHelper ("ns3::CcnxConsumerCbr");

* Assign prefix on which application operates (either generating Interests using this name or satisfying Interests for this name) using :ndnsim:`CcnxAppHelper::SetPrefix`:

   .. code-block:: c++

      consumerHelper.SetPrefix (prefix);

* Assign application-specific attributes using :ndnsim:`CcnxAppHelper::SetAttribute`:

   .. code-block:: c++

      // Set frequency parameter
      consumerHelper.SetAttribute ("Frequency", StringValue ("10")); // 10 interests a second

* Install application on one or more nodes:

   .. code-block:: c++

      NodeContainer nodes;
      ...
      consumerHelper.Install (nodes)
