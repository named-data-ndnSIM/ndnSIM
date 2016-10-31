ndnSIM helpers
==============

Helpers are very important components of ndnSIM, especially for writing simulation
scenarios.  The following summarizes helpers and their basic usage.

NDN Stack Helpers
-----------------

:ndnsim:`StackHelper` is used to install ndnSIM network stack on requested nodes, as well
        to provide a simple way configure several important parameters of NDN simulation.

Example:

.. code-block:: c++

        StackHelper ndnHelper;
        NodeContainer nodes;
        ...
        ndnHelper.Install(nodes);

Routing
+++++++

All forwarding strategies require knowledge of where Interests can be forwarded
(Forwarding Information Base).

.. note::
   By default, all nodes have empty FIB.  You need either to manually configure routes,
   use global routing controller, or (not recommended) enable default routes.

Manually routes (FIB Helper)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The :ndnsim:`FIB helper <FibHelper>` interacts with the FIB manager of NFD by sending
special Interest commands to the manager in order to add/remove a next hop from FIB
entries or add routes to the FIB manually (manual configuration of FIB).

Adding a route to the FIB manually:

    .. code-block:: c++

       Ptr<Node> node = ...     // some node
       std::string prefix = ... // some prefix
       Ptr<ndn::Face> face = ... // NDN face that belongs to the node and through which prefix is accessible
       int32_t metric = ...     // some routing metric
       FibHelper::AddRoute(node, prefix, face, metric);

:ndnsim:`FIB helper <FibHelper>` has few other AddRoute overloads that may be easier to
        use.  For example, when setting up manual routes between nodes connected with
        PointToPointNetDevice's, it is simpler to use the overload that accepts two nodes
        (face will be automatically determined by the helper).

.. @todo Implement RemoveRoute and add documentation about it

..
   Adding a next hop to a FIB entry (if any) that matches a given name prefix for a topology node:

       .. code-block:: c++

          Ptr<Node> node = .. // Get the desired node
          FibHelper::AddRoute(parameters, node);

..
   Removing a next hop from a FIB entry (if any) that matches a given name prefix for a topology node:

       .. code-block:: c++

          Ptr<Node> node = // Get the desired node
          nfd::ControlParameters parameters;
          parameters.setName(prefix);
          FibHelper::RemoveNextHop(parameters, node);


Automatic Shortest Path Routes (Global Routing Helper)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To simplify FIB management in large topologies, ndnSIM contains a global routing
controller (:ndnsim:`helper <GlobalRoutingHelper>` and :ndnsim:`special interface
<GlobalRouter>`), similar in spirit to ``Ipv4GlobalRoutingHelper``.

There are several necessary steps, in order to take advantage of the global routing controller:

* install :ndnsim:`special interfaces <GlobalRouter>` on nodes

   .. code-block:: c++

     NodeContainer nodes;
     ...
     GlobalRoutingHelper ndnGlobalRoutingHelper;
     ndnGlobalRoutingHelper.Install(nodes);

* specify which node exports which prefix using :ndnsim:`GlobalRoutingHelper::AddOrigins`

   .. code-block:: c++

     Ptr<Node> producer; // producer node that exports prefix
     std::string prefix; // exported prefix
     ...
     ndnGlobalRoutingHelper.AddOrigins(prefix, producer);

* calculate and install FIBs on every node using :ndnsim:`GlobalRoutingHelper::CalculateRoutes`

   .. code-block:: c++

     GlobalRoutingHelper::CalculateRoutes();

Forwarding Strategy
+++++++++++++++++++

ndnSIM 2.0 exactly like NFD allows different namespaces to be associated with different
forwarding strategies.  By default, the following forwarding strategy configuration is defined:

+--------------------+------------------------------+-----------------------------------------------+
| Namespace          | Strategy                     | Strategy Name                                 |
+====================+==============================+===============================================+
| ``/``              | :nfd:`fw::BestRouteStrategy` | ``/localhost/nfd/strategy/best-route``        |
+--------------------+------------------------------+-----------------------------------------------+
| ``/localhost``     | :nfd:`fw::MulticastStrategy` | ``/localhost/nfd/strategy/multicast``         |
+--------------------+------------------------------+-----------------------------------------------+
| ``/localhost/nfd`` | :nfd:`fw::BestRouteStrategy` | ``/localhost/nfd/strategy/best-route``        |
+--------------------+------------------------------+-----------------------------------------------+
| ``/ndn/multicast`` | :nfd:`fw::MulticastStrategy` | ``/localhost/nfd/strategy/multicast``         |
+--------------------+------------------------------+-----------------------------------------------+




The :ndnsim:`Strategy Choice helper <StrategyChoiceHelper>` interacts with the Strategy
Choice manager of NFD by sending special Interest commands to the manager in order to
specify the desired per-name prefix forwarding strategy for one, more or all the nodes of a topology.

This helper should be used as follows:

    .. code-block:: c++

       StrategyChoiceHelper::Install(nodes, prefix, strategyName);

or (for a forwarding strategy to be installed in all the topology nodes):

    .. code-block:: c++

       StrategyChoiceHelper::InstallAll(prefix, strategyName);

Content Store
+++++++++++++

ndnSIM uses NFD's content store implementation, maximum size of which can be controlled using
:ndnsim:`StackHelper::setCsSize()`:

      .. code-block:: c++

         ndnHelper.setCsSize(<max-size-in-packets>);
         ...
         ndnHelper.Install(nodes);

.. note::

    Unless specified in the simulation scenario, default maximum size of the content store is
    100 Data packets.

.. note::

    NFD's content store implementation takes full consideration of Interest selectors.
    In contrast to that, the old ndnSIM 1.0 content store implementations have very limited
    support for Interest selectors, but features a number of different replacement policies.
    If your scenario relies on proper selector processing, do not use these implementations as
    the simulation results most likely be incorrect.

    To select old content store implementations, use :ndnsim:`SetOldContentStore
    <StackHelper::SetOldContentStore>` StackHelper method:

          .. code-block:: c++

             ndnHelper.SetOldContentStore("<content store implementation>",
                                         ["<optional parameter>", "<optional parameter's value>" [, ...]]);
             ...
             ndnHelper.Install (nodes);

    In simulation scenarios it is possible to select one of :ref:`the existing implementations
    of the content store or implement your own <content store>`.


Application Helper
------------------

:ndnsim:`AppHelper` simplifies task of creating, configuring, and installing ndnSIM
applications.


The basic usage of the :ndnsim:`AppHelper`:

* Create helper for specific applications class:

   .. code-block:: c++

      // Create helper for the consumer generating Interests with constant rate
      AppHelper consumerHelper("ns3::ndn::ConsumerCbr");

* Assign prefix on which application operates (either generating Interests using this name
  or satisfying Interests for this name) using :ndnsim:`AppHelper::SetPrefix`:

   .. code-block:: c++

      consumerHelper.SetPrefix(prefix);

* Assign application-specific attributes using :ndnsim:`AppHelper::SetAttribute`:

   .. code-block:: c++

      // Set frequency parameter
      consumerHelper.SetAttribute("Frequency", StringValue ("10")); // 10 interests a second

* Install application on one or more nodes:

   .. code-block:: c++

      NodeContainer nodes;
      ...
      consumerHelper.Install(nodes)

.. _Link Control Helper:

Link Control Helper
-------------------

Some scenarios require failing certain links between NDN nodes at certain times.  NS-3 does not
provide ability to actually "break" the link between nodes.  However, it provides facility to
set up a loss model to simulate packet drops in the channel.  Using the properly set up loss
model on both sides of the point-to-point link, it is possible to emulate link breakage.

To simplify these operations, ndnSIM includes :ndnsim:`ndn::LinkControlHelper` that allows
scheduling of link failures and failure recoveries:


    .. code-block:: c++

        #include "ns3/ndnSIM/helper/ndn-link-control-helper.hpp"

        ...

        Simulator::Schedule(Seconds(10.0), ndn::LinkControlHelper::FailLink, node1, node2);
        Simulator::Schedule(Seconds(15.0), ndn::LinkControlHelper::UpLink, node1, node2);

Usage of this helper is demonstrated in :ref:`Simple scenario with link failures`.
