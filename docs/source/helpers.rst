ndnSIM helpers
==============

Helpers are very important components of ndnSIM, especially for writing simulation scenarios.
The following summarizes helpers and their basic usage.

StackHelper
---------------

:ndnsim:`StackHelper` is used to install ndnSIM network stack on requested nodes, as well to provide a simple way configure several important parameters of NDN simulation.

Example:

.. code-block:: c++

        ndn::StackHelper ndnHelper;
        NodeContainer nodes;
        ...
        ndnHelper.Install (nodes);

Routing
+++++++

All forwarding strategies require knowledge of where Interests can be forwarded (Forwarding Information Base).
Unlike IP routing, this knowledge may be imprecise, but without such knowledge forwarding strategies will not be able to make any decision and will drop any Interests.

.. note::
   By default, all nodes have empty FIB.  You need either to manually configure routes, use global routing controller, or (not recommended) enable default routes.

Manually routes
^^^^^^^^^^^^^^^

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

Global routing controller
^^^^^^^^^^^^^^^^^^^^^^^^^

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

Default routes
^^^^^^^^^^^^^^

In simple topologies, like in :doc:`examples <examples>`, or when
simulating broadcast environment, it is possible to set up *default*
FIB entries using :ndnsim:`StackHelper::SetDefaultRoutes` call.
More specifically, every installed NDN stack will have a FIB entry to ``/`` prefix, containing all available faces.

The following should be done before installing stack on a node:

  .. code-block:: c++

     ndnHelper.SetDefaultRoutes (true);
     ...
     ndnHelper.Install (nodes);


Content Store
+++++++++++++

ndnSIM comes with several different in-memory :ndnsim:`content store <ndn::ContentStore>` implementations, featuring different cache replacement policies.

To select a particular content store and configure its capacity, use :ndnsim:`SetContentStore <ndn::StackHelper::SetContentStore>` helper method:

      .. code-block:: c++

         ndnHelper.SetContentStore ("<content store implementation>",
                                    ["<optional parameter>", "<optional parameter's value>" [, ...]]);
	 ...
	 ndnHelper.Install (nodes);

In simulation scenarios it is possible to select one of :ref:`the existing implementations of the content store or implement your own <content store>`.


Pending Interest Table
++++++++++++++++++++++

The current version of ndnSIM provides :ndnsim:`templated realizations <ndn::pit::PitImpl>` of :ndnsim:`PIT abstraction <ndn::Pit>`, allowing optional bounding the number of PIT entries and different replacement policies (i.e., perform different actions when limit on number of PIT entries is reached).

To select a particular PIT implementation and configure its policies, use :ndnsim:`SetPit <ndn::StackHelper::SetPit>` helper method:

- :ndnsim:`persistent <ndn::pit::Persistent>` (default):

    New entries will be rejected if PIT size reached its limit

      .. code-block:: c++

         ndnHelper.SetPit ("ns3::ndn::pit::Persistent",
                           "MaxSize", "0");
	 ...
	 ndnHelper.Install (nodes);

- :ndnsim:`random <ndn::pit::Random>`:

    when PIT reaches its limit, random entry (could be the newly created one) will be removed from PIT;

      .. code-block:: c++

         ndnHelper.SetPit ("ns3::ndn::pit::Random",
                           "MaxSize", "0");
	 ...
	 ndnHelper.Install (nodes);

- :ndnsim:`least-recently-used <ndn::pit::Lru>`:

    the least recently used entry (the oldest entry with minimum number of incoming faces) will be removed when PIT size reached its limit.

      .. code-block:: c++

         ndnHelper.SetPit ("ns3::ndn::pit::Lru",
                           "MaxSize", "0");
	 ...
	 ndnHelper.Install (nodes);

Forwarding strategy
+++++++++++++++++++

A desired :ndnsim:`forwarding strategy <ForwardingStrategy>` parameter need to be set before installing stack on a node.

To select a particular forwarding strategy implementation and configure its parameters, use :ndnsim:`SetForwardingStrategy <ndn::StackHelper::SetForwardingStrategy>` helper method:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("<forwarding strategy implementation>",
                                          ["<optional parameter>", "<optional parameter's value>" [, ...]]);
	 ...
	 ndnHelper.Install (nodes);

In simulation scenarios it is possible to select one of :ref:`the existing implementations of the forwarding strategy or implement your own <forwarding strategies>`.


.. Currently, there are following forwarding strategies that can be used in simulations:

..   - :ndnsim:`Flooding` (default)

..       Interests will be forwarded to all available faces available for a route (FIB entry).
..       If there are no available GREEN or YELLOW faces, interests is dropped.

..       .. code-block:: c++

..          ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding");
.. 	 ...
.. 	 ndnHelper.Install (nodes);


..   - :ndnsim:`SmartFlooding`

..       If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
..       If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
..       If there are no available GREEN or YELLOW faces, interests is dropped.

..       .. code-block:: c++

..          ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding");
.. 	 ...
.. 	 ndnHelper.Install (nodes);

..   - :ndnsim:`BestRoute`

..       If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
..       If not, Interest will be forwarded to the highest-ranked YELLOW face.
..       If there are no available GREEN or YELLOW faces, interests is dropped.

..       .. code-block:: c++

..          ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
.. 	 ...
.. 	 ndnHelper.Install (nodes);




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


In simulation scenarios it is possible to select one of :ref:`the existing applications or implement your own <applications>`.
