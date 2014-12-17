.. _content store:

NFD's Content Store
++++++++++++++++++++

The current implementation of NFD's Content Store uses a `skip list
<http://en.wikipedia.org/wiki/Skip_list>`_ as its underlying data structure. Skip lists are a
probabilistic alternative to balanced trees. Skip lists are balanced by virtue of a random
number generator. Its average insertion and lookup complexity is O(log n). CS entries are
placed in the Skip List in ascending order (by Name).

The current implementation evicts CS entries based on prioritized FIFO (First In First Out)
strategy.  The entries that get removed first are unsolicited Data packets, which are the Data
packets that got cached opportunistically without preceding forwarding of the corresponding
Interest packet. Next, the Data packets with expired freshness are removed. Lastly, the Data
packets are removed from the Content Store on a pure FIFO basis. This cache replacement policy
is currently hard-coded; it is intended to be replaceable in the future by the NFD developer
team.

The only currently available option to control behavior of NFD's Content Store is to set its
maximum size using :ndnsim:`StackHelper::setCsSize()`:

      .. code-block:: c++

         ndnHelper.setCsSize(<max-size-in-packets>);
         ...
         ndnHelper.Install(nodes);

Examples:

- Effectively disable NFD content store an all nodes

  Minimum allowed value for NFD content store size is 1.  If 0 is specified, it will be assumed
  that the old content store implementation should be used.

      .. code-block:: c++

         ndnHelper.setCsSize(1);
         ...
         ndnHelper.Install(nodes);

- Set CS size 100 on node1, size 1000 on node1, and size 2000 on all other nodes:

      .. code-block:: c++

         ndnHelper.setCsSize(100);
         ndnHelper.Install(node1);

         ndnHelper.setCsSize(1000);
         ndnHelper.Install(node2);

         NodeContainer allOtherNodes;
         for (NodeList::Iterator i = NodeList::Begin(); i != NodeList::End(); ++i) {
           if (*i != node1 && *i != node2) {
             allOtherNodes.Add(*i);
           }
         }
         ndnHelper.Install(allOtherNodes);

CS entry
~~~~~~~~

The Data packet, along with other necessary fields, are stored in a CS entry.  Each entry
contains:

- the Data packet
- flag indicating whether the Data packet is unsolicited
- the timestamp at which the cached Data becomes stale

CS
~~

A multi-index container is maintained in order to support the prioritized FIFO cache
replacement policy.  In this way, pointers to the Data packets in a particular order are
kept. Note that this multi-index container is completely separated from the skip list
container, which indexes Content Store entries by name.

The container (Cs::CleanupIndex) currently supports indexing of unsolicited Data packets,
indexing by packet staleness and indexing by packet arrival time. Calculation of the indexes is
performed in the container during the Data packet insertion (Cs::insert) in the Content Store.

Eviction (Cs::evictItem) is performed during the insertion when the CS is full, and when the
capacity is decreased by management. We decided not to perform periodical cleanups, because its
CPU overhead causes jitter in packet forwarding.

In the current version of NFD, cache replacement policy can be modified by adding different
indexes in the Cs::CleanupIndex container (refer to Boost.multiIndex documentation) and
implementing additional logic in Cs::evictItem function.

For more detailed specification refer to the `NFD Developer's Guide
<http://named-data.net/wp-content/uploads/2014/07/NFD-developer-guide.pdf>`_, section 3.2.

Old Content Store Implementations
+++++++++++++++++++++++++++++++++

NFD's content store implementation takes full consideration of Interest selectors, however is
not yet flexible when it comes to cache replacement policies.  Feature to extend CS flexibility
is currently in active development (refer to `Issue #2219 on NFD Redmine
<http://redmine.named-data.net/issues/2219>`_) and for the time being, we have ported the old
ndnSIM 1.0 content store implementations to the new code base.  These implementations feature
different cache replacement policies, but have very limited support for Interest selectors.  If
your scenario relies on proper selector processing, do not use these implementations as the
simulation results most likely be incorrect.

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
