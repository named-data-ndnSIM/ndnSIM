FAQ
===

Boost libraries
---------------

.. note::
    **My ubuntu/redhat/freebsd have an old version of boost libraries.  How can I get the latest one?**

.. _Installing boost libraries:

Installing boost libraries to ``/usr/local``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. role:: red

.. note::
    **The following instructions are for those who want to install latest version of boost libraries** :red:`and has root access`.

The following commands would install the latest version of boost libraries (at the time of writing, version 1.53) ot ``/usr/local``, assuming you have a root access to your machine.
If you don't have root access, please refer to section :ref:`Installing boost libraries to a non-privileged location`.

.. note::
    If you are using Ubuntyu, make sure that you have installed ``libbz2-dev``, otherwise not all libraries required by ndnSIM will be installed (see :ref:`Common pitfalls`)

.. code-block:: bash
   :linenos:

    wget http://downloads.sourceforge.net/project/boost/boost/1.53.0/boost_1_53_0.tar.bz2
    tar jxf boost_1_53_0.tar.bz2
    cd boost_1_53_0
    ./bootstrap.sh
    sudo ./b2 --prefix=/usr/local install


The following commands should allow compilation and run of NS-3 simulations with custom install of boost libraries:

.. code-block:: bash
   :linenos:

    cd <ns-3>
    ./waf configure --boost-includes=/usr/local/include --boost-libs=/usr/local/lib --enable-examples
    ./waf
    LD_LIBRARY_PATH=/usr/local/lib NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

.. note::
    `LD_LIBRARY_PATH=/usr/local/lib` is necessary on Linux platform in order for the dynamic linker to find libraries installed in a location different from one of the folders specified in /etc/ld.so.conf.

.. _Installing boost libraries to a non-privileged location:

Installing boost libraries to a non-privileged location
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. note::
    **Follow these general instructions if you are trying to installboost libraries to a non-privileged location** :red:`(i.e., you do not have root access),` **but something is going wrong.**

Normally, to compile and install boost libraries in non-privileged mode, you would need to issue following commands (e.g., for boost version 1.53.0):

.. code-block:: bash
   :linenos:

    export BOOSTDIR=/home/non-privileged-user/boost
    wget http://downloads.sourceforge.net/project/boost/boost/1.53.0/boost_1_53_0.tar.bz2
    tar jxf boost_1_53_0.tar.bz2
    cd boost_1_53_0
    ./bootstrap.sh
    ./b2 --prefix=$BOOSTDIR install

However, after the following steps you may still not able to use ndnSIM.

.. _Common pitfalls:

Common pitfalls
^^^^^^^^^^^^^^^

The common pitfalls is with the **boost iostreams** library, which is required by ndnSIM, but failed to build because of the missing bzip2 library.
This problem can be easily fixed by downloading and installing bzip2 library, e.g., using the following steps:

.. code-block:: bash
   :linenos:

    wget http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz
    tar zxf bzip2-1.0.6.tar.gz
    cd bzip2-1.0.6
    make PREFIX=$BOOSTDIR CFLAGS="-fPIC -O2 -g" install

Afte bzip2 library is installed, you may recompile and reinstall boost libraries using custom compilation flags::

    ./b2 --prefix=$BOOSTDIR cxxflags=-I$BOOSTDIR/include linkflags=-L$BOOSTDIR/lib install

Alternatively, you can solve this particular problem by installing development package for bzip2 library (:red:`if you have root access`).  For example, on Ubuntu 12.04 it would be the following command::

    sudo apt-get install libbz2-dev

And then compiling and installing boost without custom compilation flags::

    ./b2 --prefix=$BOOSTDIR


The following commands should allow compilation and run of NS-3 simulations with custom install of boost libraries::

    cd <ns-3>
    ./waf configure --boost-includes=$BOOSTDIR/include --boost-libs=$BOOSTDIR/lib --enable-examples --enable-ndn-plugins=topology,mobility
    ./waf
    LD_LIBRARY_PATH=$BOOSTDIR/lib NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

.. note::
    `LD_LIBRARY_PATH=$BOOSTDIR/lib` is necessary on Linux platform in order for the dynamic linker to find libraries installed in a location different from one of the folders specified in /etc/ld.so.conf.



Visualizer problems
-------------------

.. topic:: Visualizer module is not working

    Every time I'm trying to run visualizer, I get the following error::

        Waf: Entering directory `/ndnSIM/ns-3/build'
        Could not find a task generator for the name 'ns3-visualizer'..

Something is wrong with your python bindings and python bindings dependencies.
Please follow the :ref:`requirements` section that lists what should be installed in order to run visualizer.

Code questions
--------------

.. topic:: Failing a link between nodes

    How can I fail a link between to NDN nodes?


Right now, NS-3 does not provide ability to actually "break" the link between nodes in NS-3.
However, exactly the same effect can be achieved by making an interface (:ndnsim:`ndn::Face`) up or down (:ndnsim:`ndn::Face::SetUp(true)` or :ndnsim:`ndn::Face::SetUp(false)`).

You can use :ndnsim:`ndn::LinkControlHelper` to schedule failing links.  For example, refer to :ref:`Simple scenario with link failures` example.

General questions
-----------------

.. topic:: Errors/bugs reporting

    I found an error in the documentation / bug in the code. What should I do?

Please create an issue for the documentation error or code bug on `github <http://github.com/NDN-Routing/ndnSIM>`_, or tell us about the error on `our mailing list <http://www.lists.cs.ucla.edu/mailman/listinfo/ndnsim>`_ .
