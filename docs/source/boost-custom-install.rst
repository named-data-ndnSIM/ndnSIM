Installing boost libraries
==========================

Normally, to compile and install boost libraries in non-privileged mode, you would need to issue following commands (e.g., for boost version 1.51.0)::

    export BOOSTDIR=/home/non-privileged-user/boost    

    wget http://downloads.sourceforge.net/project/boost/boost/1.51.0/boost_1_51_0.tar.bz2
    tar jxf boost_1_51_0.tar.bz2
    cd boost_1_51_0
    ./bootstrap.sh
    ./b2 --prefix=$BOOSTDIR install

However, after the following steps you may still not able to use ndnSIM.

Common pitfalls
---------------

The common pitfalls is with boost iostreams library, which is failed to build because of missing bzip2 library.
This problem can be easily fixed by downloading and installing bzip2 library, e.g., using the following steps::

    wget http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz
    tar zxf bzip2-1.0.6.tar.gz
    cd bzip2-1.0.6
    make PREFIX=$BOOSTDIR CFLAGS="-fPIC -O2 -g" install

Afte bzip2 library is installed, you may recompile and reinstall boost libraries using custom compilation flags::

    ./b2 --prefix=$BOOSTDIR cxxflags=-I$BOOSTDIR/include linkflags=-L$BOOSTDIR/lib install

Alternatively, you can solve this particular problem by installing development package for bzip2 library (if you have root access).  For example, on Ubuntu 12.04 it would be the following command::

    sudo apt-get install libbz2-dev

And then compiling and installing boost without custom compilation flags::

    ./b2 --prefix=$BOOSTDIR


Compiling NS-3 with custom boost libraries
==========================================

The following commands should allow compilation and run of NS-3 simulations with custom install of boost libraries::

    cd <ns-3>
    ./waf configure --boost-includes=$BOOSTDIR/include --boost-libs=$BOOSTDIR/lib --enable-examples --enable-ndn-plugins=topology,mobility
    LD_LIBRARY_PATH=$BOOSTDIR/lib NS_LOG=ndn.Face:ndn.Consumer ./waf --run=ndn-simple

.. note::
    `LD_LIBRARY_PATH=$BOOSTDIR/lib` is necessary on Linux platform in order for the dynamic linker to find libraries installed in a location different from one of the folders specified in /etc/ld.so.conf.
