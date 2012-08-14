ndnSIM applications
===================

ndnSIM includes a few reference applications that can be used as a base for NDN simulations.

Reference applications
++++++++++++++++++++++

ConsumerCbr
^^^^^^^^^^^^^^^

:ndnsim:`ConsumerCbr` an application that generates Interest traffic with predefined pattern (constant frequency, constant average rate with inter-Interest gap distributed uniformly at random, exponentially at random, etc.).

.. code-block:: c++

   // Create application using the app helper
   ndn::AppHelper helper ("ns3::ndn::ConsumerCbr");

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

ConsumerBatches
^^^^^^^^^^^^^^^^^^^

:ndnsim:`ConsumerBatches` is an on-off-style application gen- erating a specified number of Interests at specified points of simulation.

.. code-block:: c++

   // Create application using the app helper
   ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerBatches");

This applications has the following attributes:

* Batches

  .. note::
     default: Empty

  Specify exact pattern of Interest packets, specifying when and how many Interest packets should be sent. 
  The following example defines that 1 Interest should be requested at time 1s, 5 Interests at time 5s, and 2 Interests at time 10s.:

  .. code-block:: c++

     // Set attribute using the app helper
     helper.SetAttribute ("Batches", StringValue ("1s 1 2s 5 10s 2"));


ConsumerWindow
^^^^^^^^^^^^^^^^^^

:ndnsim:`ConsumerWindow` is an application generating a variable rate Interest traffic. It relies on an optional NACK-Interest feature and implements a simple sliding-window-based Interest generation mechanism.

.. code-block:: c++

   // Create application using the app helper
   ndn::AppHelper consumerHelper ("ns3::ndn::ConsumerWindow");


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

  If ``Size`` is set to -1, Interests will be requested till the end of the simulation.

Producer
^^^^^^^^^^^^

:ndnsim:`Producer` a simple Interest-sink application, which replying every incoming Interest with Data packet with a specified size and name same as in Interest.

.. code-block:: c++

   // Create application using the app helper
   ndn::AppHelper consumerHelper ("ns3::ndn::Producer");


Custom applications
+++++++++++++++++++

Applications interact with the core of the system using :ndnsim:`AppFace` realization of Face abstraction. 
To simplify implementation of specific NDN application, ndnSIM provides a base :ndnsim:`App` class that takes care of creating :ndnsim:`AppFace` and registering it inside the NDN protocol stack, as well as provides default processing for incoming Interest and Data packets.

.. Base App class
.. ^^^^^^^^^^^^^^^^^^



Example
^^^^^^^

The following code shows how a simple ndnSIM application can be created.  For details refer to API documentation of ndnSIM and NS-3.

.. code-block:: c++

    class CustomApp : public ndn::App
    {
    public:
      // overridden from ndn::App

      // Processing upon start of the application
      virtual void
      StartApplication ()
      {
        // initialize ndn::App
        ndn::App::StartApplication ();
        
        // Create a name components object for name ``/prefix/sub``
	Ptr<ndn::NameComponents> prefix = Create<ndn::NameComponents> (); // now prefix contains ``/``
	prefix->Add ("prefix"); // now prefix contains ``/prefix``
	prefix->Add ("sub"); // now prefix contains ``/prefix/sub``

        /////////////////////////////////////////////////////////////////////////////
        // Creating FIB entry that ensures that we will receive incoming Interests //
        /////////////////////////////////////////////////////////////////////////////

        // Get FIB object        
        Ptr<ndn::Fib> fib = GetNode ()->GetObject<ndn::Fib> ();

        // Add entry to FIB
        // Note that ``m_face`` is cretaed by ndn::App
        ndn::fib::EntryContainer::type::iterator fibEntry = fib->Add (*prefix, m_face, 0);
      
        /////////////////////////////////////
	// Sending one Interest packet out //
        /////////////////////////////////////

        // Create and configure ndn::InterestHeader
        ndn::InterestHeader interestHeader;
        UniformVariable rand (0,std::numeric_limits<uint32_t>::max ());
        interestHeader.SetNonce            (rand.GetValue ());
        interestHeader.SetName             (prefix);
        interestHeader.SetInterestLifetime (Seconds (1.0));
      
        // Create packet and add ndn::InterestHeader
        Ptr<Packet> packet = Create<Packet> ();
        packet->AddHeader (interestHeader);

        // Forward packet to lower (network) layer       
        m_protocolHandler (packet);
      
        // Call trace (for logging purposes)
        m_transmittedInterests (&interestHeader, this, m_face);
      }
    
      // Processing when application is stopped
      virtual void
      StopApplication ()
      {
        // cleanup ndn::App
        ndn::App::StopApplication ();
      }
    
      // Callback that will be called when Interest arrives
      virtual void
      OnInterest (const Ptr<const ndn::InterestHeader> &interest, Ptr<Packet> packet)
      {
        // Create and configure ndn::ContentObjectHeader and ndn::ContentObjectTail
        // (header is added in front of the packet, tail is added at the end of the packet)
        
        ndn::ContentObjectHeader data;
        data.SetName (Create<ndn::NameComponents> (interest->GetName ())); // data will have the same name as Interests
      
        ndn::ContentObjectTail trailer; // doesn't require any configuration

        // Create packet and add header and trailer
        Ptr<Packet> packet = Create<Packet> (1024);
        packet->AddHeader (data);
        packet->AddTrailer (trailer);
      
        // Forward packet to lower (network) layer       
        m_protocolHandler (packet);
      
        // Call trace (for logging purposes)
        m_transmittedInterests (&interestHeader, this, m_face);
        m_transmittedContentObjects (&data, packet, this, m_face);
      }
     
      // Callback that will be called when Data arrives
      virtual void
      OnContentObject (const Ptr<const ndn::ContentObjectHeader> &contentObject,
                       Ptr<Packet> payload)
      {
        std::cout << "DATA received for name " << contentObject->GetName () << std::endl; 
      }
    };
