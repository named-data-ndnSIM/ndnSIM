.. _content store:

NFD's Content Store
++++++++++++++++++++

The Contest Store (CS) is implemented as a set of :ndnsim:`CS entries <nfd::cs::EntryImpl>`),
ordered by the full data name including the implicit digest.  To manage entries, CS adopts a cache policy interface (:ndnsim:`nfd::cs::Policy`), invoked any time a CS entry is added, removed, updated, or used.

By default, ndnSIM uses NFD's Content Store with an option to use an old-style ndnSIM-specific content store implementations (see :ref:`old_cs`). The supported cache replacement policies are the following:

+----------------------------------------------+----------------------------------------------------------+
| **NFD's Content Store Policies**                                                                        |
+----------------------------------------------+----------------------------------------------------------+
|   ``nfd::cs::lru``                           | Last Recently Used (LRU)                                 |
+----------------------------------------------+----------------------------------------------------------+
|   ``nfd::cs::priority_fifo``                 | Priority-Based First-In-First-Out (FIFO)                 |
+----------------------------------------------+----------------------------------------------------------+

For more detailed specification refer to the `NFD Developer's Guide
<https://named-data.net/wp-content/uploads/2016/03/ndn-0021-6-nfd-developer-guide.pdf>`_, section 3.3.


To control the maximum size and the policy of NFD's Content Store use :ndnsim:`StackHelper::setCsSize()` and
:ndnsim:`StackHelper::setPolicy()` methods:

      .. code-block:: c++

         ndnHelper.setCsSize(<max-size-in-packets>);
         ndnHelper.setPolicy(<replacement-policy>);
         ...
         ndnHelper.Install(nodes);

Examples:

- To set CS size 100 on node1, size 1000 on node2, and size 2000 on all other nodes.
  LRU replacement policy for node 1, priority FIFO for the rest:

      .. code-block:: c++

         ndnHelper.setCsSize(100);
         ndnHelper.setPolicy("nfd::cs::lru");
         ndnHelper.Install(node1);

         ndnHelper.setCsSize(1000);
         ndnHelper.setPolicy("nfd::cs::priority_fifo");
         ndnHelper.Install(node2);

         NodeContainer allOtherNodes;
         for (NodeList::Iterator i = NodeList::Begin(); i != NodeList::End(); ++i) {
           if (*i != node1 && *i != node2) {
             allOtherNodes.Add(*i);
           }
         }
         ndnHelper.Install(allOtherNodes);

- To effectively disable NFD content store an all nodes

  Minimum allowed value for NFD content store maximum size is 1.  If 0 is specified, it will be assumed
  that the old content store implementation should be used.

      .. code-block:: c++

         ndnHelper.setCsSize(1);
         ...
         ndnHelper.Install(nodes);


CS entry
~~~~~~~~

The Data packet, along with other necessary fields, are stored in a CS entry.  Each entry
contains:

- the Data packet
- flag indicating whether the Data packet is unsolicited
- the timestamp at which the cached Data becomes stale

.. _old_cs:

Old Content Store Implementations
+++++++++++++++++++++++++++++++++

We have also ported the old ndnSIM 1.0 content store implementations to the new code base.
These implementations feature different cache replacement policies, but have very limited
support for Interest selectors.  If your scenario relies on proper selector processing,
do not use these implementations as the simulation results most likely be incorrect.

To select old content store implementations, use :ndnsim:`StackHelper::SetOldContentStore`:

.. code-block:: c++

    ndnHelper.SetOldContentStore("<content store implementation>",
                                ["<optional parameter>", "<optional parameter's value>" [, ...]]);
    ...
    ndnHelper.Install (nodes);

Available old content store implementations are listed in the following table:


+----------------------------------------------+----------------------------------------------------------+
| **Simple content stores**                                                                               |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Lru``                      | **Least recently used (LRU) (default)**                  |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Fifo``                     | First-in-first-Out (FIFO)                                |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Lfu``                      | Least frequently used (LFU)                              |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Random``                   | Random                                                   |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Nocache``                  | Policy that completely disables caching                  |
+----------------------------------------------+----------------------------------------------------------+
+----------------------------------------------+----------------------------------------------------------+
| **Content stores with entry lifetime tracking**                                                         |
|                                                                                                         |
| These policies allow evaluation of CS enties lifetime (i.e., how long entries stay in CS)               |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Stats::Lru``               | Least recently used (LRU)                                |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Stats::Fifo``              | Least frequently used (LFU)                              |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Stats::Lfu``               | Random                                                   |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Stats::Random``            | Policy that completely disables caching                  |
+----------------------------------------------+----------------------------------------------------------+
+----------------------------------------------+----------------------------------------------------------+
| **Content stores respecting freshness field of Data packets**                                           |
|                                                                                                         |
| These policies cache Data packets only for the time indicated by FreshnessPeriod.                       |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Freshness::Lru``           | Least recently used (LRU)                                |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Freshness::Fifo``          | Least frequently used (LFU)                              |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Freshness::Lfu``           | Random                                                   |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Freshness::Random``        | Policy that completely disables caching                  |
+----------------------------------------------+----------------------------------------------------------+
+----------------------------------------------+----------------------------------------------------------+
| **Content store realization that probabilistically accepts data packet into CS (placement policy)**     |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Probability::Lru``         | Least recently used (LRU)                                |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Probability::Fifo``        | Least frequently used (LFU)                              |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Probability::Lfu``         | Random                                                   |
+----------------------------------------------+----------------------------------------------------------+
|   ``ns3::ndn::cs::Probability::Random``      | Policy that completely disables caching                  |
+----------------------------------------------+----------------------------------------------------------+

Examples:


- Select simple LRU policy on node1, simple FIFO policy on node2, and simple random policy on
  other nodes with maximum CS sizes of 10000 packets:

      .. code-block:: c++

         ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10000");
         ndnHelper.Install(node1);

         ndnHelper.SetOldContentStore("ns3::ndn::cs::Fifo", "MaxSize", "10000");
         ndnHelper.Install(node2);

         ndnHelper.SetOldContentStore("ns3::ndn::cs::Random", "MaxSize", "10000");
         ...
         ndnHelper.Install(otherNodes);

.. note::

    If ``MaxSize`` parameter is omitted, then will be used a default value (100).

.. note::

    If ``MaxSize`` is set to 0, then no limit on ContentStore will be enforced

- Disable CS on node2

      .. code-block:: c++

         ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache");
         ndnHelper.Install(node3);

- Track lifetime of CS entries (must use ``ns3::ndn::cs::*::LifetimeStats`` policy):

      .. code-block:: c++

         void
         CacheEntryRemoved(std::string context, Ptr<const ndn::cs::Entry> entry, Time lifetime)
         {
             std::cout << entry->GetName() << " " << lifetime.ToDouble(Time::S) << "s" << std::endl;
         }

         ...

         ndnHelper.SetOldContentStore("ns3::ndn::cs::Stats::Lru", "MaxSize", "10000");
         ...
         ndnHelper.Install(nodes);

         // connect to lifetime trace
         Config::Connect("/NodeList/*/$ns3::ndn::cs::Stats::Lru/WillRemoveEntry", MakeCallback(CacheEntryRemoved));

- Get aggregate statistics of CS hit/miss ratio (works with any policy)

  The simplest way tro track CS hit/miss statistics is to use :ndnsim:`CsTracer`, in more
  details described in :ref:`Metrics Section <cs trace helper>`.

      .. code-block:: c++

         CsTracer::InstallAll("cs-trace.txt", Seconds(1));
