.. _forwarding strategies:

Forwarding Strategies
=====================

ndnSIM provides simple ways to experiment with custom Interest/Data forwarding strategies.
A new forwarding strategy can be implement completely different processing or override just specific actions/events of the :ndnsim:`forwarding strategy interface <ndn::ForwardingStrategy>`.
Please refer to :ndnsim:`API documentation <ndn::ForwardingStrategy>` of the forwarding strategy interface, which lists all default actions/events.

Available forwarding strategies
+++++++++++++++++++++++++++++++

Basic forwarding strategies
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Flooding
########

Interests will be forwarded to all available faces available for a route (FIB entry).
If there are no available GREEN or YELLOW faces, interests is dropped.

Implementation name: :ndnsim:`ns3::ndn::fw::Flooding` (default)

Usage example:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding");
	 ...
	 ndnHelper.Install (nodes);

SmartFlooding
#############

If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
If not, Interest will be forwarded to all available faces available for a route (FIB entry)/
If there are no available GREEN or YELLOW faces, interests is dropped.

Implementation name :ndnsim:`ns3::ndn::fw::SmartFlooding`

Usage example:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding");
	 ...
	 ndnHelper.Install (nodes);

BestRoute
#########

If GREEN face is available, Interest will be sent to the highest-ranked GREEN face.
If not, Interest will be forwarded to the highest-ranked YELLOW face.
If there are no available GREEN or YELLOW faces, interests is dropped.

Implementation name: :ndnsim:`ns3::ndn::fw::BestRoute`

Usage example:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute");
	 ...
	 ndnHelper.Install (nodes);

Strategies with Interest limits
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following strategies enforce different granularities of Interest limits.  Each strategy is an extension of the basic one (custom strategies can also be extended with limits, refer to the source code).

Currently, ndnSIM implements two types of Interest limit enforcements, both based on a Token Bucket approach:

   - :ndnsim:`ns3::ndn::Limits::Window` (default)

        Interest token is borrowed when Interest is send out.  The token is returned only when Interest is satisfied or times out.

   - :ndnsim:`ns3::ndn::Limits::Rate`

        Interest token is borrowed when Interest is send out.  The token is returned periodically based on link capacity.

In both cases, limit is set according to the following equation:

.. math::

    \mathrm{Interest\ Limit} = Delay\ [s] \cdot
       \frac{\mathrm{Bandwidth\ [Bytes/s]}}
       {\mathrm{Data\ packet\ size\ [Bytes]} + \mathrm{Interest\ packet\ size\ [Bytes]}}

To configure packet sizes and delay parameters, use the following :ndnsim:`ndn::StackHelper` method:

      .. code-block:: c++

         // ndnHelper.EnableLimits (true, <delay>, <average interest packet size>, <average data packet size>);
         ndnHelper.EnableLimits (true, Seconds (0.2), 40, 1100);
	 ...
	 ndnHelper.Install (nodes);

Usage examples
##############

Per outgoing Face limits
%%%%%%%%%%%%%%%%%%%%%%%%

- :ndnsim:`ns3::ndn::fw::Flooding::PerOutFaceLimits`

    With :ndnsim:`Limits::Window`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding::PerOutFaceLimits"
                                          "Limit", "ns3::ndn::Limits::Window");
	 ...
	 ndnHelper.Install (nodes);


    With :ndnsim:`Limits::Rate`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding::PerOutFaceLimits"
                                          "Limit", "ns3::ndn::Limits::Rate");
	 ...
	 ndnHelper.Install (nodes);

- :ndnsim:`ns3::ndn::fw::SmartFlooding::PerOutFaceLimits`

    With :ndnsim:`Limits::Window`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding::PerOutFaceLimits"
                                          "Limit", "ns3::ndn::Limits::Window");
	 ...
	 ndnHelper.Install (nodes);


    With :ndnsim:`Limits::Rate`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding::PerOutFaceLimits"
                                          "Limit", "ns3::ndn::Limits::Rate");
	 ...
	 ndnHelper.Install (nodes);

- :ndnsim:`ns3::ndn::fw::BestRoute::PerOutFaceLimits`

    With :ndnsim:`Limits::Window`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute::PerOutFaceLimits"
                                          "Limit", "ns3::ndn::Limits::Window");
	 ...
	 ndnHelper.Install (nodes);


    With :ndnsim:`Limits::Rate`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute::PerOutFaceLimits"
                                          "Limit", "ns3::ndn::Limits::Rate");
	 ...
	 ndnHelper.Install (nodes);


Per FIB entry, per outgoing face limits
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

- :ndnsim:`ns3::ndn::fw::Flooding::PerOutFaceLimits::PerFibLimits`

    With :ndnsim:`Limits::Window`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ndn::Limits::Window");
	 ...
	 ndnHelper.Install (nodes);


    With :ndnsim:`Limits::Rate`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::Flooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ndn::Limits::Rate");
	 ...
	 ndnHelper.Install (nodes);

- :ndnsim:`ns3::ndn::fw::SmartFlooding::PerOutFaceLimits::PerFibLimits`

    With :ndnsim:`Limits::Window`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ndn::Limits::Window");
	 ...
	 ndnHelper.Install (nodes);


    With :ndnsim:`Limits::Rate`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::SmartFlooding::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ndn::Limits::Rate");
	 ...
	 ndnHelper.Install (nodes);

- :ndnsim:`ns3::ndn::fw::BestRoute::PerOutFaceLimits::PerFibLimits`

    With :ndnsim:`Limits::Window`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ndn::Limits::Window");
	 ...
	 ndnHelper.Install (nodes);


    With :ndnsim:`Limits::Rate`:

      .. code-block:: c++

         ndnHelper.SetForwardingStrategy ("ns3::ndn::fw::BestRoute::PerOutFaceLimits::PerFibLimits"
                                          "Limit", "ns3::ndn::Limits::Rate");
	 ...
	 ndnHelper.Install (nodes);

.. _Writing your own custom strategy:

Writing your own custom strategy
++++++++++++++++++++++++++++++++

First step in creating your own strategy is to decide which existing strategy you want to extend.  You can either use realize :ndnsim:`forwarding strategy interface <ndn::ForwardingStrategy>` (:ndnsim:`ndn::ForwardingStrategy::DoPropagateInterest` call must be implemented) or extended one of the available forwarding strategies (:ndnsim:`fw::BestRoute` or :ndnsim:`fw::Flooding`).
The following example assumes that we are realizing :ndnsim:`forwarding strategy interface <ndn::ForwardingStrategy>`.

The follwoing are template strategy h/cc files:

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.h
   :language: c++
   :linenos:
   :lines: 1-36,51-55,59-

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.cc
   :language: c++
   :linenos:
   :lines: 1-40,42-50,75-76,115-
   :emphasize-lines: 21,27

After having the template, we can fill the necesasry functionality.

Let us say, that we want Interest be forwarded to first two best-metric faces specified by FIB.
That is, if node has two or more alternative paths to forward the Interests, this Interest will be forwarded to the best two neighbors.
The following implementation of CustomStrategy::DoPropagateInterest accomplishes the task:

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.cc
   :language: c++
   :linenos:
   :lines: 45-75
   :emphasize-lines: 7-30

After the compilation, you can use ``"ns3::ndn::fw::CustomStrategy"`` as a parameter to :ndnsim:`ndn::StackHelper::SetForwardingStrategy` helper method.

 .. as well as NS_LOG=ndn.fw.CustomStrategy when running in a debug mode

Extending strategy
++++++++++++++++++

If you need more customization for the forwarding strategy, there are many forwarding strategy events that can be overriden.
For example, if we want to perform special logging of all forwarded, timed out, and satisfied Intersts, we can override the following events (for more events, refer to :ndnsim:`ForwardingStrategy API documentation <ForwardingStrategy>`):

- :ndnsim:`DidSendOutInterest <ForwardingStrategy::DidSendOutInterest>`, which fired just after forwarding the Interest

- :ndnsim:`WillEraseTimedOutPendingInterest <ForwardingStrategy::WillEraseTimedOutPendingInterest>`, which fired just before PIT entry is removed by timeout

- :ndnsim:`WillSatisfyPendingInterest <ForwardingStrategy::WillSatisfyPendingInterest>`, which fired just before Interest will be satisfied.

The highlighted ares of the following code demonstrates how it can be impelmented:

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.h
   :language: c++
   :linenos:
   :emphasize-lines: 37-50,56-58

.. literalinclude:: ../../examples/custom-strategies/custom-strategy.cc
   :language: c++
   :linenos:
   :emphasize-lines: 41,77-114


Example of using custom strategy
++++++++++++++++++++++++++++++++

Please refer to :ref:`this example <11-node 2-bottleneck topology with custom forwarding strategy>`.

