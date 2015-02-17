How to speed up simulations by parallel execution
-------------------------------------------------

A way to speed up your simulations is to run them in parallel taking advantage of the power of
all the processors and the memory availability of your machine. This can be done by using the
Message Passing Interface (MPI) along with the distributed simulator class `provided by NS-3
<http://www.nsnam.org/docs/models/html/distributed.html#mpi-for-distributed-simulation>`_.

To make use of MPI, the network topology needs to be partitioned in a proper way, as the
potential speedup will not be able to exceed the number of topology partitions. However, it
should be noted that dividing the simulation for distributed purposes in NS-3 can only occur
across point-to-point links.  Currently, only the applications running on a node can be
executed in a separate logical processor, while the whole network topology will be created in
each parallel execution.  Lastly, MPI requires the exchange of messages among the logical
processors, thus imposing a communication overhead during the execution time.

Designing a parallel simulation scenario
----------------------------------------

In order to run simulation scenarios using MPI, all you need is to partition your network
topology in a proper way.  That is to say, to maximize benefits of the parallelization, you
need to equally distribute the workload for each logical processor.

The full topology will always be created in each parallel execution (on each "rank" in MPI
terms), regardless of the individual node system IDs.  Only the applications are specific to a
rank. For example, consider node 1 on logical processor (LP) 1 and node 2 on LP 2, with a
traffic generator on node 1. Both node 1 and node 2 will be created on both LP 1 and LP 2;
however, the traffic generator will only be installed on LP 1. While this is not optimal for
memory efficiency, it does simplify routing, since all current routing implementations in ns-3
will work with distributed simulation.

For more information, you can take a look at the `NS-3 MPI documentation
<http://www.nsnam.org/docs/models/html/distributed.html#mpi-for-distributed-simulation>`_.

Compiling and running ndnSIM with MPI support
---------------------------------------------

- Install MPI

    On Ubuntu:

    .. code-block:: bash

        sudo apt-get install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev

    On Fedora:

    .. code-block:: bash

        sudo yum install openmpi openmpi-devel

    On OS X with HomeBrew:

    .. code-block:: bash

        brew install open-mpi

- Compile ndnSIM with MPI support

    You can compile ndnSIM with MPI support using ./waf configure by adding the parameter
    ``--enable-mpi`` along with the parameters of your preference. For example, to configure
    with examples and MPI support in optimized mode:

    .. code-block:: bash

        cd <ns-3-folder>
        ./waf configure -d optimized --enable-examples --enable-mpi

- Run ndnSIM with MPI support

    To run a simulation scenario using MPI, you need to type:

    .. code-block:: bash

        mpirun -np <number_of_processors> ./waf --run=<scenario_name>


.. _simple scenario with MPI support:

Simple parallel scenario using MPI
----------------------------------

This scenario simulates a network topology consisting of two nodes in parallel. Each node
is assigned to a dedicated logical processor.

The default parallel synchronization strategy implemented in the DistributedSimulatorImpl
class is based on a globally synchronized algorithm using an MPI collective operation to
synchronize simulation time across all LPs. A second synchronization strategy based on local
communication and null messages is implemented in the NullMessageSimulatorImpl class, For
the null message strategy the global all to all gather is not required; LPs only need to
communication with LPs that have shared point-to-point links. The algorithm to use is
controlled by which the ns-3 global value SimulatorImplementationType.

The strategy can be selected according to the value of nullmsg. If nullmsg is true, then
the local communication strategy is selected. If nullmsg is false, then the globally
synchronized strategy is selected.  This parameter can be passed either as a command line
argument or by directly modifying the simulation scenario.

The best algorithm to use is dependent on the communication and event scheduling pattern for
the application. In general, null message synchronization algorithms will scale better due
to local communication scaling better than a global all-to-all gather that is required by
DistributedSimulatorImpl. There are two known cases where the global synchronization performs
better. The first is when most LPs have point-to-point link with most other LPs, in other
words the LPs are nearly fully connected. In this case the null message algorithm will
generate more message passing traffic than the all-to-all gather. A second case where the
global all-to-all gather is more efficient is when there are long periods of simulation time
when no events are occurring. The all-to-all gather algorithm is able to quickly determine
then next event time globally. The nearest neighbor behavior of the null message algorithm
will require more communications to propagate that knowledge; each LP is only aware of
neighbor next event times.

The following code represents all that is necessary to run such this simple parallel scenario

.. literalinclude:: ../../examples/ndn-simple-mpi.cpp
   :language: c++
   :linenos:
   :lines: 22-35,71-
   :emphasize-lines: 41-44, 54-58, 78-79, 89-90

If this code is placed into ``scratch/ndn-simple-mpi.cpp`` or NS-3 is compiled with examples
enabled, you can compare runtime on one and two CPUs using the following commands::

    # 1 CPU
    mpirun -np 1 ./waf --run=ndn-simple-mpi

    # 2 CPUs
    mpirun -np 2 ./waf --run=ndn-simple-mpi


The following table summarizes 9 executions on OS X 10.10 and 2.3 GHz Intel Core i7 a single
CPU, on two CPUs with global synchronization, and on two CPUs with null message
synchronization:

+-------------+-----------------+------------------+----------------+
| # of CPUs   | Real time, s    | User time, s     | System time, s |
+=============+=================+==================+================+
| 1           | 20.9 +- 0.14    | 20.6 +- 0.13     | 0.2 +- 0.01    |
+-------------+-----------------+------------------+----------------+
| 2 (global)  | 11.1 +- 0.13    | 21.9 +- 0.24     | 0.2 +- 0.02    |
+-------------+-----------------+------------------+----------------+
| 2 (nullmsg) | 11.4 +- 0.12    | 22.4 +- 0.21     | 0.2 +- 0.02    |
+-------------+-----------------+------------------+----------------+

Note that MPI not always will result in simulation speedup and can actually result in
performance degradation.  This means that either network is not properly partitioned or the
simulation cannot take advantage of the partitioning (e.g., the simulation time is dominated by
the application on one node).
