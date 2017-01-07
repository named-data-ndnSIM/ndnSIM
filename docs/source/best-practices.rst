..  _best-practices:

Best Practices on Using ndnSIM
==============================

We highly recommend that ndnSIM users follow the principles below:

#. **The latest version of the simulator should be used whenever possible.**

   The ndnSIM team is able to address users's questions more efficiently
   when the latest released version of the simulator is used.

#. **Users should fork and use the provided scenario template in order to
   implement their simulation scenarios.**

   The scenario template can be found `here <https://github.com/named-data-ndnSIM/scenario-template>`__.

   Examples of using the scenario template can be found `here <https://github.com/named-data-ndnSIM/scenario-ndn-ping>`__
   and `here <https://github.com/named-data-ndnSIM/scenario-ChronoSync>`__.

   - Scenario template structure

      The scenario template contains the following directories:

      #. **extensions:** This directory is supposed to contain simulator extensions
         necessary for the written simulation scenarios. Specifically, these are
         any .cpp/.hpp files that will be linked to each of the included scenario.

      #. **scenarios:** Under this directory, the actual simulations are
         supposed to be placed (each .cpp file becomes its own simulation
         scenario -- it must contain the ``main`` function).

    - How to compile a scenario template

      To configure in optimized mode without logging **(default)**:

        .. code-block:: c++

          ./waf configure

      To configure in optimized mode with scenario logging enabled (logging in NS-3 and ndnSIM modules will still be disabled,
      but you can see output from NS_LOG* calls from your scenarios and extensions):

        .. code-block:: c++

          ./waf configure --logging

      To configure in debug mode with all logging enabled

        .. code-block:: c++

          ./waf configure --debug

      If you have installed NS-3 in a non-standard location, you may need to set up ``PKG_CONFIG_PATH`` variable.

    - How to run the simulation scenarios **(without visualizer)**

      Normally, you can run scenarios either directly:

        .. code-block:: shell

          ./build/<scenario_name>

      or using waf:

        .. code-block:: shell

          ./waf --run <scenario_name>

      If NS-3 is installed in a non-standard location, on some platforms (e.g., Linux) you need to specify the ``LD_LIBRARY_PATH`` variable:

        .. code-block:: shell

          LD_LIBRARY_PATH=/usr/local/lib ./build/<scenario_name>

      or:

        .. code-block:: shell

          LD_LIBRARY_PATH=/usr/local/lib ./waf --run <scenario_name>

      To run a scenario using the debugger, use the following command:

        .. code-block:: shell

          gdb --args ./build/<scenario_name>

    - How to run the simulation scenarios **(with visualizer)**

      To run a scenario with the visualizer, you will have to setup some python
      environment variables to find the visualizer module. The easiest way to do
      so is by using the following commands:

        .. code-block:: shell

          cd ns-dev/ns-3
          ./waf shell

      After these commands, you will have the complete environment to run the vizualizer.

      The following command will run a scenario with the visualizer:

        .. code-block:: shell

          ./waf --run <scenario_name> --vis

      or:

        .. code-block:: shell

          PKG_LIBRARY_PATH=/usr/local/lib ./waf --run <scenario_name> --vis

      If you need automatic node placement, set up an additional environment variable:

        .. code-block:: shell

          NS_VIS_ASSIGN=1 ./waf --run <scenario_name> --vis

      or:

        .. code-block:: shell

          PKG_LIBRARY_PATH=/usr/local/lib NS_VIS_ASSIGN=1 ./waf --run <scenario_name> --vis

#. **Users should modify the README file of their fork of the scenario
   template repo to clearly note the version/fork of ndnSIM that their
   scenarios are supposed to work in.**

    Our effort is to keep ndnSIM updated with the latest advancements of NDN
    research. As a result, the ndnSIM API might change from one release to
    another, while the API of a new release might not be fully compatible with
    the API of previous releases. Therefore, we **strongly** encourage you to
    note the specific version (commit hash of the ndnSIM repository and
    base NS-3 repository) that was used for your simulations. This will
    guarantee the reproducibility of your results, which is critical
    for simulations that are published in scientific papers.

.. note:: We highly encourage that users share their simulation scenarios
    with the rest of the ndnSIM users' community. To achieve that, please let
    us know if you would like us to fork your simulation
    scenarios under the named-data-ndnSIM GitHub organization.
