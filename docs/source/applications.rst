ndnSIM applications
===================

ndnSIM includes a few reference applications that can be used as a base for NDN simulations.

Reference applications
++++++++++++++++++++++

CcnxConsumerCbr
^^^^^^^^^^^^^^^

:ndnsim:`CcnxConsumerCbr` an application that generates Interest traffic with predefined pattern (constant frequency, constant average rate with inter-Interest gap distributed uniformly at random, exponentially at random, etc.).

.. code-block:: c++

   // Create application using the app helper
   CcnxAppHelper helper ("ns3::CcnxConsumerCbr");

This applications has the following attributes:

* Frequency

  .. note::
     default: ``1.0`` (1 per second)

  Either exact (for contant version) or expected (for randomized version) frequency with which Interests are generated

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Frequency", DoubleValue (1.0));

* Randomize

  .. note::
     default: ``"none"``

  Specify whether to do randomization for inter-Interest gap or not.  The following variants are currently supported:
  
  - ``"none"``: no randomization
 
  - ``"uniform"``: uniform distribution in range (0, 1/Frequency)

  - ``"exponential"``: exponential distribution with mean 1/Frequency

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Randomize", StringValue ("uniform"));

CcnxConsumerBatches
^^^^^^^^^^^^^^^^^^^

:ndnsim:`CcnxConsumerBatches` is an on-off-style application gen- erating a specified number of Interests at specified points of simulation.

.. code-block:: c++

   // Create application using the app helper
   CcnxAppHelper consumerHelper ("ns3::CcnxConsumerBatches");

This applications has the following attributes:

* Batches

  .. note::
     default: Empty

  Specify exact pattern of Interest packets, specifying when and how many Interest packets should be sent. 
  The following example defines that 1 Interest should be requested at time 1s, 5 Interests at time 5s, and 2 Interests at time 10s.:

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Batches", StringValue ("1s 1 2s 5 10s 2"));


CcnxConsumerWindow
^^^^^^^^^^^^^^^^^^

:ndnsim:`CcnxConsumerWindow` is an application generating a variable rate Interest traffic. It relies on an optional NACK-Interest feature and implements a simple sliding-window-based Interest generation mecha- nism.

.. code-block:: c++

   // Create application using the app helper
   CcnxAppHelper consumerHelper ("ns3::CcnxConsumerWindow");


This applications has the following attributes:

* Window

  .. note::
     default: ``1``

  Initial number of Interests that will be send out without waiting for the data (number of outstanding Interests)

* PayloadSize

  .. note::
     default: ``1040``

  Expected size of the Data payload (necessary only when Size is specified)

* Size

  .. note::
     default: ``-1``

  Amount of data to be requested (will stop issuing Interests after ``Size`` data is received)

  If ```Size`` is set to -1, Interests will be requested till the end of the simulation.

CcnxProducer
^^^^^^^^^^^^

:ndnsim:`CcnxProducer` a simple Interest-sink application, which replying every incoming Interest with Data packet with a specified size and name same as in Interest.

.. code-block:: c++

   // Create application using the app helper
   CcnxAppHelper consumerHelper ("ns3::CcnxProducer");


Custom applications
+++++++++++++++++++

Applications interact with the core of the system using AppFace realization of Face abstraction. 
To simplify implementation of specific NDN application, ndnSIM provides a base CcnxApp class that takes care of creating CcnxAppFace and registering it inside the NDN protocol stack, as well as provides default processing for incoming Interest and Data packets.

Base CcnxApp class
^^^^^^^^^^^^^^^^^^



Example
^^^^^^^
