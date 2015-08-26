Simulating real NDN applications
================================

The version of `ndn-cxx library <http://named-data.net/doc/ndn-cxx/>`__ bundled with ndnSIM includes
a modified version of :ndnsim:`ndn::Face` to directly send and receive Interest and Data packets to
and from the simulated instances of NFD.  With this modification, ndnSIM enables support to simulate
real NDN applications written against the ndn-cxx, if they satisfy requirements listed in this guide
or can be modified to satisfy these requirements.


Requirements
++++++++++++

#. **Source code of the application must be available**

   The application (parts of the application) needs to be compiled against the ndnSIM version of
   ndn-cxx library.

#. **Source code should separate** ``main`` **function from the functional components of the
   application that will be simulated**

   The entry point to the application (its functional component) will be NS-3 application class,
   which should be able to create and destroy an instance of the simulated component when scheduled
   by the scenario.

#. **The application should not use global variables, if they define a state for the application
   instance**

   ndnSIM should be able to create multiple different instances of the application, e.g., for each
   simulated node.

   Exception to this requirement is :ndnsim:`ndn::Scheduler <ndn::util::scheduler::Scheduler>`: its
   implementation has been rewired to use NS-3's scheduling routines.

#. **The application MUST NOT contain any GUI or command-line terminal interactions**

#. **The application SHOULD NOT use disk operations, unless application instances access unique
   parts of the file system**

   In the simulated environment, all application instances will be accessing the same local file
   system, which can result in undefined behavior if not properly handled.

#. **The application MUST use a subset of** :ndnsim:`ndn::Face` **API:**

   - If the application create :ndnsim:`ndn::Face`, it MUST BE created either with a default
     constructor or constructor that accepts a single ``boost::asio::io_service`` parameter.

     .. code-block:: c++

         // Supported
         ndn::Face face1();
         ndn::Face face2(ioService);

         // Not supported in ndnSIM
         ndn::Face face4(host_name, port_number)
         ndn::Face face3(transport);
         // and others

   - :ndnsim:`ndn::Face::getIoService()` should be used only to obtain a reference to
     ``boost::asio::io_service``.  **Application MUST NOT use any methods of**
     ``boost::asio::io_service``, **otherwise the simulation will crash.**

     .. code-block:: c++

         ndn::Face face;
         ...
         // Supported (the rewired Scheduler implementation does not access io_service methods)
         Scheduler scheduler(face.getIoService());

         // Not supported in ndnSIM and will result in crash
         face.getIoService().stop();


   - Application should avoid use of :ndnsim:`Face::processEvents` or use it with caution

     In real applications, processEvents blocks until some data is received or the timeout callback
     is called. In this case, any variables created before calling this method will still exist
     after the method returns. However, in ndnSIM, such an assumption cannot be made, since the
     scope of a variable is local.

    .. code-block:: c++

        void
        foo
        {
          ndn::Face face;
          face.expressInterest(...);
          face.setInterestFilter(...);

          // ndnSIM version of processEvents will not block!
          face.processEvents();
        }
        // after existing foo scope, face variable is deallocated and all scheduled operations
        // will be canceled

#. **Application (simulated component) MUST NOT create instances of** ``boost::asio::io_service``
   **and use their methods**

   ``boost::asio::io_service`` is inherently incompatible with NS-3, as both are providing mechanisms
   for asynchronous event processing.

#. We also recommend that functional part of the application accepts reference to the
   :ndnsim:`KeyChain` instance, instead of creating instance itself.

   When simulating non-security aspects of the application, in simulation scenario it will be
   possible to use a dummy implementation of the :ndnsim:`KeyChain` that does not perform crypto
   operations, but signs Data and Interests with fake signatures.

   For example, this can be achieved by enabling the constructor of the real application to accept a reference
   to the :ndnsim:`KeyChain`:

    .. code-block:: c++

        // Real applications should accept a reference to the KeyChain instance
        RealApp::RealApp(KeyChain& keyChain)
          : m_keyChain(keyChain)
        {
        }


How to simulate real applications using ndnSIM
++++++++++++++++++++++++++++++++++++++++++++++

To simulate a real application, the simulation scenario should contain a class derived from
``ns3::Application``.  This class needs to create an instance of the :ndnsim:`ndn::Face` and/or real
application in the overloaded ``StartApplication`` method.  This class also need to ensure that the
created instance is not deallocated until ``StopApplication`` method is called.

For example, if the functional class of the real application looks like:

.. literalinclude:: ../../examples/ndn-cxx-simple/real-app.hpp
    :language: c++
    :linenos:
    :lines: 23-75


The corresponding NS-3 "entry point" application class can be like this:

.. literalinclude:: ../../examples/ndn-cxx-simple/real-app-starter.hpp
    :language: c++
    :linenos:
    :lines: 25-65

.. note::
   There is a requirement that :ndnsim:`ndn::Face` MUST BE created within the context of a specific
   ``ns3::Node``.  In simple words this means that :ndnsim:`ndn::Face` constructor must be called
   somewhere within the overloaded ``StartApplication`` method.

   Attempt to create a :ndnsim:`ndn::Face` outside ``ns3::Node`` (e.g., if the example included
   member variable ``Face m_face`` in ``RealAppStarter`` class) will result in simulation crash.

The final step is to actually write a simulation scenario that defines network topology, routing
information between nodes, on which nodes the application should be installed and when it should be
started and stopped.

For the trivial example, let us assume that we have only one simulation node and we want to start
the application at time moment 6.5 seconds.  This scenario can look like:

.. literalinclude:: ../../examples/ndn-cxx-simple.cpp
    :language: c++
    :linenos:
    :lines: 20-


Example of a real application simulation
++++++++++++++++++++++++++++++++++++++++

To demonstrate functionality of ndnSIM in a more complex and realistic case, we will use the NDN
ping application included as part of `NDN Essential Tools`_.

For this example, we used a `scenario template repository`_ as a base to write simulation-specific
extensions and define scenarios, and the `final version of the scenario is available in GitHub
<https://github.com/named-data-ndnSIM/scenario-ndn-ping>`__.

The following lists steps we did to simulate `ndnping` and `ndnpingserver` apps on a simple
three-node topology:

- forked `scenario template repository`_

- imported the latest version of `NDN Essential Tools`_ source code as a git submodule

- updated the build script (``wscript``) to compile the source code of ``ndnping`` and
  ``ndnpingserver`` (with the exception of compilation units that contain ``main`` function) against
  ndnSIM

  `View changes <https://github.com/named-data-ndnSIM/scenario-ndn-ping/commit/74269dc4de6afe2b6e13a0bcc8c0faac350d8fa3>`__

- defined ``PingClient`` and ``PingServer`` classes to hold state of application instances

  `View changes <https://github.com/named-data-ndnSIM/scenario-ndn-ping/commit/4f087a16e3171af38c05b53c6cfd9e752e7cda72>`__

- defined ``PingClientApp`` and ``PingServerApp`` NS-3 applications, that create and destroy
  instances of ``PingClient`` and ``PingServer`` per NS-3 logic.

  `View changes <https://github.com/named-data-ndnSIM/scenario-ndn-ping/commit/2b317860f55b71b34ffdccac31444246d9b804fe>`__

- defined a simple scenario that creates a three node topology, installs NDN stacks, and installs
  ``PingClientApp`` and ``PingServerApp`` applications on different simulation nodes.

  `View changes <https://github.com/named-data-ndnSIM/scenario-ndn-ping/commit/2b317860f55b71b34ffdccac31444246d9b804fe>`__

After all these steps, the repository is ready to run the simulation (see `README.md
<https://github.com/named-data-ndnSIM/scenario-ndn-ping/blob/master/README.md>`__ for more details).


.. note::
   The listed steps did not include any modification of `NDN Essential Tools`_ source code.
   However, this was not the case when we initially attempted to run the simulation, as the source
   code was violating a few requirements of this guide.  `The changes that we made
   <https://github.com/named-data/ndn-tools/commit/1e7a7b20c93014e86639e3d07f357c95b48b34ac>`__ are
   an example of how to adapt the source code to be compatible with ndnSIM simulations.


.. _NDN Essential Tools: http://github.com/named-data/ndn-tools

.. _scenario template repository: https://github.com/named-data-ndnSIM/scenario-template
