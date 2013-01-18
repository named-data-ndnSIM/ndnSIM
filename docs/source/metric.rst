
Obtaining metrics
=================

To obtain simulation results, you would need to connect to one or more `trace sources <doxygen/group___trace_source_list.html>`_ provided by ndnSIM classes.

It is also possible to use existing trace helpers, which collects and aggregates requested statistical information in text files.

.. _trace classes:

Packet-level trace helpers
--------------------------

- :ndnsim:`ndn::L3AggregateTracer`

    Tracing the aggregate number of Interests/Data packets forwarded by an NDN node 

    The following example enables tracing on all simulation nodes:

    .. code-block:: c++

        // necessary includes
	#include <ns3/ndnSIM/utils/tracers/ndn-l3-aggregate-tracer.h>

	...        

        // the following should be put just before calling Simulator::Run in the scenario

        boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<ndn::L3AggregateTracer> > >
          aggTracers = ndn::L3AggregateTracer::InstallAll ("aggregate-trace.txt", Seconds (1.0));
        
        Simulator::Run ();
        
        ...


- :ndnsim:`ndn::L3RateTracer`

    Tracing the rate in bytes and in number of packets of Interest/Data packets forwarded by an NDN node 

    The following example enables tracing on all simulation nodes:

    .. code-block:: c++

        // necessary includes
	#include <ns3/ndnSIM/utils/tracers/ndn-l3-rate-tracer.h>

	...        

        // the following should be put just before calling Simulator::Run in the scenario

        boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<ndn::L3RateTracer> > >
          rateTracers = ndn::L3RateTracer::InstallAll ("rate-trace.txt", Seconds (1.0));
        
        Simulator::Run ();
        
        ...


.. note::

    A number of other tracers are available in ``plugins/tracers-broken`` folder, but they do not yet work with the current code.
    Eventually, we will port most of them to the current code, but it is not our main priority at the moment and would really appreciate help with writing new tracers and porting the old ones. 


Example
+++++++

Please refer to the :ref:`this example <trace example>`.

.. _cs trace helper:

Content store trace helper
--------------------------

- :ndnsim:`ndn::CsTracer`

    With the use of :ndnsim:`ndn::CsTracer` it is possible to obtain statistics of cache hits/cache misses on simulation nodes.

    The following code enables content store tracing:

    .. code-block:: c++

        // necessary includes
        #include <ns3/ndnSIM/utils/tracers/ndn-cs-tracer.h>

	...        

        // Select implementation of content store. By default, the following is applied:
        // ndnHelper.SetContentStore ("ns3::ndn::cs::Stats::Lru", "MaxSize", "100");

        // the following should be put just before calling Simulator::Run in the scenario

        boost::tuple< boost::shared_ptr<std::ostream>, std::list<Ptr<ndn::CsTracer> > >
           aggTracers = ndn::CsTracer::InstallAll ("cs-trace.txt", Seconds (1));
        
        Simulator::Run ();
        
        ...

.. - Tracing lifetime of content store entries

..     Evaluate lifetime of the content store entries can be accomplished using modified version of the content stores.
..     In particular,



Example
+++++++

:ref:`This example <cs trace helper example>` demonstrates one usage of content store tracer.

