.. _content store:

NFD's Content Store
++++++++++++++++++++

The Contest Store (CS) is implemented as a set of :ndnsim:`CS entries <nfd::cs::EntryImpl>`),
ordered by the full data name including the implicit digest.  To manage entries, CS adopts a cache policy interface (:ndnsim:`nfd::cs::Policy`), invoked any time a CS entry is added, removed, updated, or used.

The supported cache replacement policies are the following:

+----------------------------------------------+----------------------------------------------------------+
| **NFD's Content Store Policies**                                                                        |
+----------------------------------------------+----------------------------------------------------------+
|   ``nfd::cs::lru``                           | Last Recently Used (LRU)                                 |
+----------------------------------------------+----------------------------------------------------------+
|   ``nfd::cs::priority_fifo``                 | Priority-Based First-In-First-Out (FIFO)                 |
+----------------------------------------------+----------------------------------------------------------+

For more detailed specification refer to the `NFD Developer's Guide
<https://named-data.net/wp-content/uploads/2016/03/ndn-0021-6-nfd-developer-guide.pdf>`_, section 3.3.


To control the maximum size and the policy of NFD's Content Store use ``StackHelper::setCsSize()`` and
``StackHelper::setPolicy()`` methods:

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

  Minimum allowed value for NFD content store maximum size is 1.

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

Misc
~~~~
  
- Get aggregate statistics of CS hit/miss ratio (works with any policy)

  The simplest way tro track CS hit/miss statistics is to use :ndnsim:`CsTracer`, in more
  details described in :ref:`Metrics Section <cs trace helper>`.

      .. code-block:: c++

         CsTracer::InstallAll("cs-trace.txt", Seconds(1));
