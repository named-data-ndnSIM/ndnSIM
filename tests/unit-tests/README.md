ndnSIM unit tests
=================

ndnSIM uses [Boost Unit Test Framework](http://www.boost.org/doc/libs/1_48_0/libs/test/doc/html/index.html)
for testing features of the implementation.

ndnSIM unit tests should be placed into `ndnSIM/tests/unit-tests/` folder.  All `.cpp` files placed
in this folder will be automatically compiled together.

Running unit-tests
------------------

To run unit tests, NS-3 and ndnSIM need to be configured and build with the unit test support.  To
enable both general NS-3 unit tests and ndnSIM unit tests, add `--enable-tests` during configuration
phase.  For example, run the following from the NS-3 root folder:

    ./waf configure --enable-tests
    ./waf build

To run unit tests:

    ./waf --run ndnSIM-unit-tests
